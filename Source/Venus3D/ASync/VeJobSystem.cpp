////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   VeJobSystem.cpp
//  Created:     2016/07/15 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//--------------------------------------------------------------------------
VeThreadCallbackResult VeJobSystem::FGThreadCallback(void* pvParam) noexcept
{
	VeThread::InitPerThread();
	VeJobSystem& s = VeJobSystem::Ref();
	fore_thread& t = *(fore_thread*)pvParam;
	while (true)
	{
		{
			std::unique_lock<std::mutex> ul(s.m_kFGLoop.mute);
			while (t.cond_val <= 0)
			{
				s.m_kFGLoop.cond.wait(ul);
			}
		}
		switch (s.m_i32FGState.load(std::memory_order_relaxed))
		{
		case 0:
			continue;
		case -1:
			break;
		default:
			if (s.m_pkParallel)
			{
				s.m_pkParallel->Work(t.index);
			}
			else
			{
				s.ExecuteForeground(t.index);
			}
			t.cond_val = 0;
			if (s.m_i32FGJoinValue.fetch_add(1, std::memory_order_relaxed)
				== int32_t(s.m_kFGThreads.size() - 1))
			{
				s.m_i32FGState.store(0, std::memory_order_relaxed);
				{
					std::lock_guard<std::mutex> l(s.m_kFGJoin.mute);
					s.m_kFGJoin.cond.notify_all();
				}
			}
			continue;
		}
		break;
	}
	VeThread::TermPerThread();
	return 0;
}
//--------------------------------------------------------------------------
VeThreadCallbackResult VeJobSystem::BGThreadCallback(void* pvParam) noexcept
{
	VeThread::InitPerThread();
	VeJobSystem& s = VeJobSystem::Ref();
	back_thread& t = *(back_thread*)pvParam;
	while (true)
	{
		s.m_kWaitingThreads.push(&t);
		s.m_i32BGAvailableNum.fetch_add(1, std::memory_order_relaxed);
		t.loop.wait();
		if (s.m_i32BGState.load(std::memory_order_relaxed) == -1)
		{
			break;
		}
		else
		{
			s.ExecuteBackground(t.index);
			t.loop.reset();
			s.m_i32BGAvailableNum.fetch_sub(1, std::memory_order_relaxed);
		}
	}
	VeThread::TermPerThread();
	return 0;
}
//--------------------------------------------------------------------------
VeJobSystem::VeJobSystem(size_t stFGNum, size_t stBGNum) noexcept
{
	if (stFGNum)
	{
		m_i32FGState.store(0, std::memory_order_relaxed);
		m_kFGThreads.resize(stFGNum);
		for (size_t i(0); i < m_kFGThreads.size(); ++i)
		{
			fore_thread& t = m_kFGThreads[i];
			t.index = (uint32_t)i;
			t.cond_val = 0;
			t.handle = VeCreateThread(&FGThreadCallback,
				&t, VE_JOB_FG_PRIORITY, VE_JOB_FG_STACK_SIZE);
		}
	}
	if (stBGNum)
	{
		assert(stBGNum <= (VE_JOB_BG_BUFFER_MASK + 1));
		m_i32BGState.store(0, std::memory_order_relaxed);
		m_i32BGAvailableNum.store(0, std::memory_order_relaxed);
		m_kBGThreads.resize(stBGNum);
		for (size_t i(0); i < m_kBGThreads.size(); ++i)
		{
			back_thread& t = m_kBGThreads[i];
			t.handle = VeCreateThread(&BGThreadCallback,
				&t, VE_JOB_BG_PRIORITY, VE_JOB_BG_STACK_SIZE);
			t.index = (uint32_t)i;
		}
	}
	m_i32BGJobNum.store(0, std::memory_order_relaxed);
	m_u32FrameCount.store(0, std::memory_order_relaxed);
}
//--------------------------------------------------------------------------
VeJobSystem::~VeJobSystem() noexcept
{
	m_i32FGState.store(-1, std::memory_order_relaxed);
	if (m_kFGThreads.size())
	{
		{
			std::lock_guard<std::mutex> l(m_kFGLoop.mute);
			for (auto& t : m_kFGThreads)
			{
				t.cond_val = 1;
			}
			m_kFGLoop.cond.notify_all();
		}
		for (auto& t : m_kFGThreads)
		{
			VeJoinThread(t.handle);
		}
	}
	while (m_i32BGAvailableNum.load(std::memory_order_relaxed) != m_kBGThreads.size());
	m_i32BGState.store(-1, std::memory_order_relaxed);
	for (auto& t : m_kBGThreads)
	{
		t.loop.set();
		VeJoinThread(t.handle);
	}
}
//--------------------------------------------------------------------------
void VeJobSystem::ParallelCompute(VeJob* pkJob) noexcept
{
	assert(pkJob && pkJob->GetType() == VeJob::TYPE_PARALLEL_COMPUTE);
	int32_t check(0);
	if (m_i32FGState.compare_exchange_weak(check, 1, std::memory_order_relaxed))
	{
		m_pkParallel = pkJob;
		RunForeground();
		m_pkParallel = nullptr;
	}
}
//--------------------------------------------------------------------------
void VeJobSystem::ProcessForegroundJobs() noexcept
{
	assert(!m_pkParallel);
	int32_t check(0);
	if (m_i32FGState.compare_exchange_weak(check, 1, std::memory_order_relaxed))
	{
		RunForeground();
		m_u32FrameCount.fetch_add(1, std::memory_order_relaxed);
	}
}
//--------------------------------------------------------------------------
void VeJobSystem::Appointment(VeJob* pkJob, bool bCurrentFrame) noexcept
{
	assert(pkJob && pkJob->GetType() && pkJob->GetPriority() < VeJob::PRI_MAX);
	if (pkJob->GetType() & 0xF0)
	{
		assert(m_kBGThreads.size());
		size_t stFrame = m_u32FrameCount.load(std::memory_order_relaxed) & 1;
		stFrame = bCurrentFrame ? stFrame : (!stFrame);
		auto& queue = m_akFGQueues[stFrame][pkJob->GetPriority()];
		queue.main.push(pkJob);
		m_i32BGJobNum.fetch_add(1, std::memory_order_relaxed);
		auto pt = m_kWaitingThreads.pop();
		if (pt)
		{
			pt->loop.set();
		}
	}
	else
	{
		m_akBGQueues[pkJob->GetPriority()].main.push(pkJob);
	}
}
//--------------------------------------------------------------------------
void VeJobSystem::RunForeground() noexcept
{
	m_i32FGJoinValue.store(0, std::memory_order_relaxed);
	{
		std::lock_guard<std::mutex> l(m_kFGLoop.mute);
		for (auto& t : m_kFGThreads)
		{
			t.cond_val = 1;
		}
		m_kFGLoop.cond.notify_all();
	}
	{
		std::unique_lock<std::mutex> ul(m_kFGJoin.mute);
		while (m_i32FGJoinValue.load(std::memory_order_relaxed) < int32_t(m_kFGThreads.size()))
		{
			m_kFGJoin.cond.wait(ul);
		}
	}
}
//--------------------------------------------------------------------------
void VeJobSystem::ExecuteForeground(uint32_t u32Index) noexcept
{
	size_t stFrame = m_u32FrameCount.load(std::memory_order_relaxed) & 1;
	VeJob* pkJob = FetchForeground(stFrame);
	while (pkJob)
	{
		assert(!(pkJob->GetType() & 0xF0));
		Execute(pkJob, u32Index);
		pkJob = FetchForeground(stFrame);
	}
}
//--------------------------------------------------------------------------
VeJob* VeJobSystem::FetchForeground(size_t stFrame) noexcept
{
	VeJob* pkJob;
	for (auto& queue : m_akFGQueues[stFrame])
	{
		pkJob = queue.waiting.pop();
		if (pkJob)
		{
			assert(pkJob->m_pkFiber);
			return pkJob;
		}
		pkJob = queue.main.pop();
		if (pkJob)
		{
			assert(!(pkJob->m_pkFiber));
			return pkJob;
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------
void VeJobSystem::RunBackground() noexcept
{
	if (m_i32BGJobNum.load(std::memory_order_relaxed) > 0)
	{
		auto pt = m_kWaitingThreads.pop();
		if (pt)
		{
			pt->loop.set();
		}
	}
}
//--------------------------------------------------------------------------
void VeJobSystem::ExecuteBackground(uint32_t u32Index) noexcept
{
	VeJob* pkJob = FetchBackground();
	while (pkJob)
	{
		assert(pkJob->GetType() & 0xF0);
		Execute(pkJob, u32Index);
		pkJob = FetchBackground();
	}
}
//--------------------------------------------------------------------------
VeJob* VeJobSystem::FetchBackground() noexcept
{
	VeJob* pkJob;
	for (auto& queue : m_akBGQueues)
	{
		pkJob = queue.waiting.pop();
		if (pkJob)
		{
			assert(pkJob->m_pkFiber);
			return pkJob;
		}
		pkJob = queue.main.pop();
		if (pkJob)
		{
			assert(!(pkJob->m_pkFiber));
			return pkJob;
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------
void VeJobSystem::Execute(VeJob* pkJob, uint32_t u32Index) noexcept
{
	auto type = pkJob->GetType() & 0xF;
	assert(type >= 1 && type <= 3);
	if (type == 1)
	{
		pkJob->Work(u32Index);
		if (m_kJobPool.vertify(pkJob))
		{
			m_kJobPool.release((VeJobFunc*)pkJob);
		}
	}
	else
	{
		assert(type == 2 || type == 3);
		if (pkJob->m_pkFiber)
		{
			pkJob->m_pvData = pkJob->m_pkFiber->resume(pkJob);
		}
		else
		{
			if (type == 2)
			{
				pkJob->m_pkFiber = m_kFiberPool.acquire();
			}
			else
			{
				pkJob->m_pkFiber = m_kLargeFiberPool.acquire();
			}
			pkJob->m_pkFiber->prepare();
			pkJob->m_pvData = (void*)(size_t)u32Index;
			pkJob->m_pvData = pkJob->m_pkFiber->start(&RunJob, pkJob);
		}
		if (pkJob->m_pkFiber->GetState() == VeCoroutine::STATE_DEAD)
		{
			if (m_kFiberPool.vertify(pkJob->m_pkFiber))
			{
				m_kFiberPool.release((co_normal*)pkJob->m_pkFiber);
			}
			else
			{
				assert(m_kLargeFiberPool.vertify(pkJob->m_pkFiber));
				m_kLargeFiberPool.release((co_large*)pkJob->m_pkFiber);
			}
			pkJob->m_pkFiber = nullptr;
			if (m_kJobPool.vertify(pkJob))
			{
				m_kJobPool.release((VeJobFunc*)pkJob);
			}
		}
	}
}
//--------------------------------------------------------------------------
void* VeJobSystem::RunJob(void* pvJob) noexcept
{
	((VeJob*)pvJob)->Work((uint32_t)(size_t)(((VeJob*)pvJob)->m_pvData));
	return nullptr;
}
//--------------------------------------------------------------------------
