////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      ASync
//  File name:   VeJobSystem.h
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

#pragma once

#define VE_JOB_FG_PRIORITY VE_THREAD_PRIORITY_HIGHEST
#define VE_JOB_FG_STACK_SIZE 32768

#define VE_JOB_BG_PRIORITY VE_THREAD_PRIORITY_BELOW_NORMAL
#define VE_JOB_BG_STACK_SIZE 32768

class VeJob : public VeRefObject
{
public:
	enum Type
	{
		TYPE_PARALLEL_COMPUTE,
		TYPE_IMMEDIATE,
		TYPE_YIELDABLE,
		TYPE_YIELDABLE_2X_STACK,
		TYPE_MAX
	};

	VeJob(Type eType) noexcept : m_eType(eType) {}

	Type GetType() noexcept
	{
		return m_eType;
	}

	virtual void Work(uint32_t u32Index) noexcept = 0;

private:
	const Type m_eType;

};

VeSmartPointer(VeJob);

class VeJobFunc : public VeJob
{
public:
	VeJobFunc(Type eType, const std::function<void(uint32_t)>& _Fx) noexcept
		: VeJob(eType), m_kWork(_Fx) {}

	virtual void Work(uint32_t u32Index) noexcept
	{
		m_kWork(u32Index);
	}

private:
	std::function<void(uint32_t)> m_kWork;

};

class VENUS_API VeJobSystem : public VeSingleton<VeJobSystem>
{
public:
	VeJobSystem(size_t stFGNum, size_t stBGNum) noexcept;

	~VeJobSystem() noexcept;

	

	void ParallelCompute(const VeJobPtr& spJob) noexcept;

	/*void ParallelCompute(const std::function<void()>& _Fx) noexcept
	{
		VeJobPtr spJob = VE_NEW VeJobFunc(VeJob::TYPE_PARALLEL_COMPUTE, _Fx);
		ParallelCompute(spJob);
	}*/

private:
	static VeThreadCallbackResult VE_CALLBACK FGThreadCallback(
		void* pvParam) noexcept;

	static void ParallelCompute(void* pvJob) noexcept;

	struct signal
	{
		VeThreadMutex mutex;
		VeConditionVariable cond;
	};

	struct fore_thread
	{
		VeThreadHandle handle;
		uint32_t index;
		uint32_t cond_val;
	};

	signal m_akFGLoop;
	signal m_akFGJoin;
	vtd::vector<fore_thread> m_kFGThreads;
	std::atomic<int32_t> m_i32FGState;
	VeJobPtr m_spParallel;

};

#include "VeJobSystem.inl"
