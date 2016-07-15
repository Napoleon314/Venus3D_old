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
	VeJobSystem& s = VeJobSystem::Ref();
	fore_thread& t = *(fore_thread*)pvParam;
	while (true)
	{
		VeThreadMutexLock(s.m_akFGLoop.mutex);
		while (t.cond_val <= 0)
		{
			VeConditionVariableWait(s.m_akFGLoop.cond, s.m_akFGLoop.mutex);
		}
		VeThreadMutexUnlock(s.m_akFGLoop.mutex);
		switch (s.m_i32FGState.load(std::memory_order_relaxed))
		{
		case 0:
			continue;
		case -1:
			break;		
		default:
			if (s.m_spParallel)
			{
				s.m_spParallel->Work(t.index);
			}
			t.cond_val = 0;
			continue;
		}
		break;
	}

	VeCoreLogI("Thread:", t.index, "exited");
	return 0;
}
//--------------------------------------------------------------------------
VeJobSystem::VeJobSystem(size_t stFGNum, size_t) noexcept
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
}
//--------------------------------------------------------------------------
VeJobSystem::~VeJobSystem() noexcept
{
	if (m_kFGThreads.size())
	{
		m_i32FGState.store(-1, std::memory_order_relaxed);
		VeThreadMutexLock(m_akFGLoop.mutex);
		for (auto& t : m_kFGThreads)
		{
			t.cond_val = 1;
		}
		VeConditionVariableWakeAll(m_akFGLoop.cond);
		VeThreadMutexUnlock(m_akFGLoop.mutex);
		for (auto& t : m_kFGThreads)
		{
			VeJoinThread(t.handle);
		}
	}
}
//--------------------------------------------------------------------------
void VeJobSystem::ParallelCompute(const VeJobPtr& spJob) noexcept
{
	assert(spJob && spJob->GetType() == VeJob::TYPE_PARALLEL_COMPUTE);
	int32_t check(0);
	if (m_i32FGState.compare_exchange_weak(check, 1, std::memory_order_relaxed))
	{
		/*m_spParallel = spJob;
		m_akFGJoin.reset();
		m_akFGLoop.step_all();
		m_akFGJoin.wait(m_kFGThreads.size());
		m_i32FGState.store(0, std::memory_order_relaxed);
		m_spParallel = nullptr;*/
	}
}
//--------------------------------------------------------------------------
void VeJobSystem::ParallelCompute(void*) noexcept
{
	
}
//--------------------------------------------------------------------------
