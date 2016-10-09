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
VeJob::~VeJob() noexcept
{

}
//--------------------------------------------------------------------------
void VeJob::ClearTasks() noexcept
{
	std::lock_guard<vtd::spin_lock> lock(m_kLock);
	if (m_u32Pointer < m_kTaskArray.size())
	{
		m_kTaskArray.resize(m_u32Pointer);
	}
}
//--------------------------------------------------------------------------
void VeJob::AddTask(Task funcTask, Execute eExecute,
	Priority ePriority) noexcept
{
	m_kTaskArray.push_back({ eExecute, ePriority, funcTask });
}
//--------------------------------------------------------------------------
bool VeJob::Start() noexcept
{
	IncRefCount();
	{
		std::lock_guard<vtd::spin_lock> lock(m_kLock);
		if (m_u32Pointer == 0 && m_u32Wait == 0 && m_kTaskArray.size())
		{
			VE_ASSERT(m_kRelatedJobs.empty());
			m_u32Pointer = 1;
		}
		else
		{
			return false;
		}
	}
	auto& kAtom = m_kTaskArray.front();
	if (kAtom.m_eExecute == VeJob::EXE_BACKGROUND)
	{
		ve_job_sys.m_akBGQueues[kAtom.m_ePriority].push(this);
		ve_job_sys.m_i32BGJobNum.fetch_add(1, std::memory_order_relaxed);
		auto pt = ve_job_sys.m_kWaitingThreads.pop();
		if (pt)
		{
			pt->loop.set();
		}
	}
	else
	{
		size_t stFrame = ve_job_sys.m_u32FrameCount.load(std::memory_order_relaxed) & 1;
		stFrame = kAtom.m_eExecute == VeJob::EXE_FG_CURRENT ? stFrame : (!stFrame);
		ve_job_sys.m_akFGQueues[stFrame][kAtom.m_ePriority].push(this);
	}
	return true;
}
//--------------------------------------------------------------------------
void VeJob::Wait(const VeJobPtr& spJob) noexcept
{
	if (spJob)
	{
		std::lock_guard<vtd::spin_lock> lock(spJob->m_kLock);
		if (spJob->m_u32Pointer)
		{
			spJob->m_kRelatedJobs.push_back(this);
			++m_u32Wait;
		}
	}
}
//--------------------------------------------------------------------------
void VeJob::Run(uint32_t u32Index) noexcept
{
	VE_ASSERT(m_u32Pointer <= m_kTaskArray.size());
	auto& atom = m_kTaskArray[m_u32Pointer - 1];
	if (atom.m_funcTask(u32Index))
	{
		if (!m_u32Wait)
		{
			Resume();
		}
	}
	else
	{
		End(false);
	}
}
//--------------------------------------------------------------------------
void VeJob::End(bool bSuccess) noexcept
{
	VeVector<VeJobPtr> kRelatedJobs;
	{
		std::lock_guard<vtd::spin_lock> lock(m_kLock);
		VE_ASSERT(!m_u32Wait);
		m_u32Pointer = 0;
		kRelatedJobs = std::move(m_kRelatedJobs);
	}
	if (bSuccess)
	{
		for (auto& job : kRelatedJobs)
		{
			job->Resume();
		}
	}
	else
	{
		for (auto& job : kRelatedJobs)
		{
			job->End(false);
		}
	}
	OnEnd(bSuccess);
	DecRefCount();
}
//--------------------------------------------------------------------------
void VeJob::Resume() noexcept
{
	VE_ASSERT(!m_u32Wait);
	bool bEnd = true;
	{
		std::lock_guard<vtd::spin_lock> lock(m_kLock);
		if (m_u32Pointer < m_kTaskArray.size())
		{
			++m_u32Pointer;
			bEnd = false;
		}
	}
	if (bEnd)
	{
		End(true);
	}
	else
	{
		auto& kAtom = m_kTaskArray[m_u32Pointer - 1];
		if (kAtom.m_eExecute == VeJob::EXE_BACKGROUND)
		{
			ve_job_sys.m_akBGQueues[kAtom.m_ePriority].push(this);
			ve_job_sys.m_i32BGJobNum.fetch_add(1, std::memory_order_relaxed);
			auto pt = ve_job_sys.m_kWaitingThreads.pop();
			if (pt)
			{
				pt->loop.set();
			}
		}
		else
		{
			size_t stFrame = ve_job_sys.m_u32FrameCount.load(std::memory_order_relaxed) & 1;
			stFrame = kAtom.m_eExecute == VeJob::EXE_FG_CURRENT ? stFrame : (!stFrame);
			ve_job_sys.m_akFGQueues[stFrame][kAtom.m_ePriority].push(this);
		}
	}
}
//--------------------------------------------------------------------------
VeThreadCallbackResult VeJobSystem::FGThreadCallback(void* pvParam) noexcept
{
	VeThread::InitPerThread();
	fore_thread& t = *(fore_thread*)pvParam;
	while (true)
	{
		{
			std::unique_lock<std::mutex> ul(ve_job_sys.m_kFGLoop.mute);
			while (t.cond_val <= 0)
			{
				ve_job_sys.m_kFGLoop.cond.wait(ul);
			}
		}
		switch (ve_job_sys.m_i32FGState.load(std::memory_order_relaxed))
		{
		case 0:
			continue;
		case -1:
			break;
		default:
			ve_job_sys.ExecuteForeground(t.index);
			t.cond_val = 0;
			if (ve_job_sys.m_i32FGJoinValue.fetch_add(1, std::memory_order_relaxed)
				== int32_t(ve_job_sys.m_kFGThreads.size() - 1))
			{
				ve_job_sys.m_i32FGState.store(0, std::memory_order_relaxed);
				{
					std::lock_guard<std::mutex> l(ve_job_sys.m_kFGJoin.mute);
					ve_job_sys.m_kFGJoin.cond.notify_all();
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
	back_thread& t = *(back_thread*)pvParam;
	while (true)
	{
		ve_job_sys.m_kWaitingThreads.push(&t);
		ve_job_sys.m_i32BGAvailableNum.fetch_add(1, std::memory_order_relaxed);
		t.loop.wait();
		if (ve_job_sys.m_i32BGState.load(std::memory_order_relaxed) == -1)
		{
			break;
		}
		else
		{
			ve_job_sys.ExecuteBackground(t.index);
			t.loop.reset();
			ve_job_sys.m_i32BGAvailableNum.fetch_sub(1, std::memory_order_relaxed);
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
		VE_ASSERT(stBGNum <= (VE_JOB_BG_BUFFER_MASK + 1));
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
	while (m_i32BGAvailableNum.load(std::memory_order_relaxed) != ((int32_t)m_kBGThreads.size()));
	m_i32BGState.store(-1, std::memory_order_relaxed);
	for (auto& t : m_kBGThreads)
	{
		t.loop.set();
		VeJoinThread(t.handle);
	}
}
//--------------------------------------------------------------------------
void VeJobSystem::ProcessForegroundJobs() noexcept
{
	int32_t check(0);
	if (m_i32FGState.compare_exchange_weak(check, 1, std::memory_order_relaxed))
	{
		RunForeground();
		m_u32FrameCount.fetch_add(1, std::memory_order_relaxed);
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
		pkJob->Run(u32Index);
		pkJob = FetchForeground(stFrame);
	}
}
//--------------------------------------------------------------------------
VeJob* VeJobSystem::FetchForeground(size_t stFrame) noexcept
{
	VeJob* pkJob(nullptr);
	for (auto& queue : m_akFGQueues[stFrame])
	{
		pkJob = queue.pop();
		if (pkJob)
		{
			break;
		}
	}
	return pkJob;
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
		pkJob->Run(u32Index);
		pkJob = FetchBackground();
	}
}
//--------------------------------------------------------------------------
VeJob* VeJobSystem::FetchBackground() noexcept
{
	VeJob* pkJob(nullptr);
	for (auto& queue : m_akBGQueues)
	{
		pkJob = queue.pop();
		if (pkJob)
		{
			m_i32BGJobNum.fetch_sub(1, std::memory_order_relaxed);
			break;
		}
	}
	return pkJob;
}
//--------------------------------------------------------------------------
