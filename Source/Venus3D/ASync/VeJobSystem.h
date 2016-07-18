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
#define VE_JOB_BG_BUFFER_MASK 0xF

#define VE_JOB_DEP_SIZE 16
#define VE_JOB_POOL_SIZE 1024
#define VE_JOB_BUFFER_SIZE 512

class VeJob : public VeMemObject
{
public:
	enum Type
	{
		TYPE_PARALLEL_COMPUTE			= 0x0,
		TYPE_FG_IMMEDIATE				= 0x1,
		TYPE_FG_YIELDABLE				= 0x2,
		TYPE_FG_YIELDABLE_2X_STACK		= 0x3,
		TYPE_BG_IMMEDIATE				= 0x11,
		TYPE_BG_YIELDABLE				= 0x12,
		TYPE_BG_YIELDABLE_2X_STACK		= 0x13,
		TYPE_MASK						= 0xFF
	};

	VeJob(Type eType) noexcept : m_eType(eType) {}

	Type GetType() noexcept
	{
		return m_eType;
	}

	virtual void Work(uint32_t u32Index) noexcept = 0;

protected:
	VeJob() noexcept : m_eType(TYPE_MASK) {};

	Type m_eType;
	vtd::ring_buffer<VeJob*, nullptr, VE_JOB_DEP_SIZE> m_kDepList;

};

class VeJobFunc : public VeJob
{
public:
	VeJobFunc() noexcept = default;

	void Set(Type eType, std::function<void(uint32_t)> funcWork) noexcept
	{
		m_eType = eType;
		m_kWork = std::move(funcWork);
	}

	virtual void Work(uint32_t u32Index) noexcept override
	{
		m_kWork(u32Index);
	}

private:
	std::function<void(uint32_t)> m_kWork;

};

class VENUS_API VeJobSystem : public VeSingleton<VeJobSystem>
{
public:
	//typedef vtd::obj_ptr_pool<>

	VeJobSystem(size_t stFGNum, size_t stBGNum) noexcept;

	~VeJobSystem() noexcept;

	inline VeJobFunc* AcquireJob() noexcept;

	inline VeJobFunc* AcquireJob(VeJob::Type eType,
		std::function<void(uint32_t)> funcWork) noexcept;

	inline void ReleaseJob(VeJobFunc* pkJob) noexcept;

	inline void ConcurrencyWork(VeJob* pkJob) noexcept;

	void ParallelCompute(VeJob* pkJob) noexcept;

	void ParallelCompute(std::function<void(uint32_t)> _Fx) noexcept
	{
		VeJobFunc* pkJob = AcquireJob(VeJob::TYPE_PARALLEL_COMPUTE, std::move(_Fx));
		ParallelCompute(pkJob);
		ReleaseJob(pkJob);
	}

private:
	static VeThreadCallbackResult VE_CALLBACK FGThreadCallback(
		void* pvParam) noexcept;

	static VeThreadCallbackResult VE_CALLBACK BGThreadCallback(
		void* pvParam) noexcept;

	void RunForeground() noexcept;

	struct signal
	{
		std::mutex mute;
		std::condition_variable cond;
	};

	struct fore_thread
	{
		VeThreadHandle handle;
		uint32_t index;
		uint32_t cond_val;
	};

	struct back_thread
	{
		VeThreadHandle handle;
		VeThread::event loop;
	};

	typedef vtd::ring_buffer<back_thread*, nullptr, VE_JOB_BG_BUFFER_MASK> BGBuffer;
	typedef vtd::ring_buffer<VeJob*, nullptr, VE_JOB_BUFFER_SIZE> JobBuffer;

	std::atomic<int32_t> m_i32FGState;
	std::atomic<int32_t> m_i32FGJoinValue;
	signal m_kFGLoop;
	signal m_kFGJoin;
	vtd::vector<fore_thread> m_kFGThreads;

	std::atomic<int32_t> m_i32BGState;
	std::atomic<int32_t> m_i32BGAvailableNum;
	vtd::vector<back_thread> m_kBGThreads;
	BGBuffer m_kWaitingThreads;
	

	VeJob* m_pkParallel = nullptr;

	vtd::obj_pool<VeJobFunc, VE_JOB_POOL_SIZE> m_kJobPool;
	JobBuffer m_kFGJobList;
	JobBuffer m_kFGJobWaitList;
	JobBuffer m_kBGJobList;
	JobBuffer m_kBGJobWaitList;

};

#include "VeJobSystem.inl"
