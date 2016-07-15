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
struct FGThreadParam : public VeMemObject
{
	FGThreadParam(uint32_t idx) noexcept
		: m_u32Index(idx)
	{
		m_u32Signal.store(0, std::memory_order_relaxed);
	}

	uint32_t m_u32Index;
	std::atomic<uint32_t> m_u32Signal;
};
//--------------------------------------------------------------------------
VeThreadCallbackResult VeJobSystem::FGThreadCallback(void* pvParam) noexcept
{
	VeJobSystem* pkSystem = VeJobSystem::Ptr();
	FGThreadParam* pkParam = (FGThreadParam*)pvParam;

	while (true)
	{
		pkSystem->m_akFGLoop.wait(0);
		switch (pkSystem->m_i32FGState.load(std::memory_order_relaxed))
		{
		case 2:
			break;
		case 1:


		default:
			continue;
		}
		break;
	}

	VeCoreLogI("Thread:", pkParam->m_u32Index, "exited");

	VE_DELETE(pkParam);
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
			m_kFGThreads[i] = VeCreateThread(&FGThreadCallback,
				VE_NEW FGThreadParam((uint32_t)i), VE_JOB_FG_PRIORITY,
				VE_JOB_FG_STACK_SIZE);
		}
		
	}
	
}
//--------------------------------------------------------------------------
VeJobSystem::~VeJobSystem() noexcept
{
	if (m_kFGThreads.size())
	{
		m_i32FGState.store(2, std::memory_order_relaxed);
		m_akFGLoop.set_all(1);
		for (auto& hThread : m_kFGThreads)
		{
			VeJoinThread(hThread);
		}
	}
}
//--------------------------------------------------------------------------
void VeJobSystem::ParallelCompute(const VeJobPtr&) noexcept
{
	
}
//--------------------------------------------------------------------------
void VeJobSystem::ParallelCompute(void*) noexcept
{
	
}
//--------------------------------------------------------------------------
VeJobSystem::signal::signal() noexcept
{
	VeThreadMutexInit(_Mutex);
	VeConditionVariableInit(_Condition);
}
//--------------------------------------------------------------------------
VeJobSystem::signal::~signal() noexcept
{
	VeConditionVariableTerm(_Condition);
	VeThreadMutexTerm(_Mutex);
}
//--------------------------------------------------------------------------
