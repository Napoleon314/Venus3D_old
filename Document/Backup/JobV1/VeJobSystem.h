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
#define VE_JOB_BUFFER_SIZE 256

#define VE_JOB_FIBER_STACK (64 * 1024)
#define VE_JOB_FIBER_POOL_SIZE (128)
#define VE_JOB_LARGEB_FIBER_STACK (512 * 1024)
#define VE_JOB_LARGE_FIBER_POOL_SIZE (32)

class VeJob : public VeMemObject
{
public:
	enum Type
	{
		TYPE_PARALLEL_COMPUTE			= 0x0,
		TYPE_FG_IMMEDIATE				= 0x1,
		TYPE_FG_YIELDABLE				= 0x2,
		TYPE_FG_YIELDABLE_LARGE_STACK	= 0x3,
		TYPE_BG_IMMEDIATE				= 0x11,
		TYPE_BG_YIELDABLE				= 0x12,
		TYPE_BG_YIELDABLE_LARGE_STACK	= 0x13,
		TYPE_MASK						= 0xFF
	};

	enum Priority
	{
		PRI_HIGH,
		PRI_NORMAL,
		PRI_LOW,
		PRI_MAX
	};

	VeJob(Type eType, Priority ePriority) noexcept
		: m_eType(eType), m_ePriority(ePriority) {}

	Type GetType() noexcept
	{
		return m_eType;
	}

	Priority GetPriority() noexcept
	{
		return m_ePriority;
	}

	virtual void Work(uint32_t u32Index) noexcept = 0;

protected:
	friend class VeJobSystem;
	VeJob() noexcept : m_eType(TYPE_MASK) {};

	Type m_eType;
	Priority m_ePriority;

private:
	VeCoroutine* m_pkFiber = nullptr;
	void* m_pvData = nullptr;

};

class VeJobFunc : public VeJob
{
public:
	VeJobFunc() noexcept = default;

	void Set(Type eType, std::function<void(uint32_t)> funcWork,
		Priority ePriority = VeJob::PRI_NORMAL) noexcept
	{
		m_eType = eType;
		m_ePriority = ePriority;
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
	VeJobSystem(size_t stFGNum, size_t stBGNum) noexcept;

	~VeJobSystem() noexcept;

	inline VeJobFunc* AcquireJob() noexcept;

	inline VeJobFunc* AcquireJob(VeJob::Type eType,
		std::function<void(uint32_t)> funcWork,
		VeJob::Priority ePriority = VeJob::PRI_NORMAL) noexcept;

	inline void ReleaseJob(VeJobFunc* pkJob) noexcept;

	void ParallelCompute(VeJob* pkJob) noexcept;

	void ParallelCompute(std::function<void(uint32_t)> _Fx) noexcept
	{
		VeJobFunc* pkJob = AcquireJob(VeJob::TYPE_PARALLEL_COMPUTE, std::move(_Fx));
		ParallelCompute(pkJob);
		ReleaseJob(pkJob);
	}

	void ProcessForegroundJobs() noexcept;

	void Appointment(VeJob* pkJob, bool bCurrentFrame = false) noexcept;

private:
	static VeThreadCallbackResult VE_CALLBACK FGThreadCallback(
		void* pvParam) noexcept;

	static VeThreadCallbackResult VE_CALLBACK BGThreadCallback(
		void* pvParam) noexcept;

	void RunForeground() noexcept;

	void ExecuteForeground(uint32_t u32Index) noexcept;

	VeJob* FetchForeground(size_t stFrame) noexcept;

	void RunBackground() noexcept;

	void ExecuteBackground(uint32_t u32Index) noexcept;

	VeJob* FetchBackground() noexcept;

	void Execute(VeJob* pkJob, uint32_t u32Index) noexcept;

	static void* RunJob(void* pvJob) noexcept;

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
		uint32_t index;
	};

	class co_normal : public VeCoroutine
	{
	public:
		co_normal() : VeCoroutine(VE_JOB_FIBER_STACK) {}

	};

	class co_large : public VeCoroutine
	{
	public:
		co_large() : VeCoroutine(VE_JOB_LARGEB_FIBER_STACK) {}

	};

	typedef vtd::ring_buffer<back_thread*, nullptr, VE_JOB_BG_BUFFER_MASK> BGBuffer;
	typedef vtd::ring_buffer<VeJob*, nullptr, VE_JOB_BUFFER_SIZE> JobBuffer;

	struct job_queue
	{
		JobBuffer main;
		JobBuffer waiting;
	};

	VeJob* m_pkParallel = nullptr;
	std::atomic<int32_t> m_i32FGState;
	std::atomic<int32_t> m_i32FGJoinValue;
	signal m_kFGLoop;
	signal m_kFGJoin;
	VeVector<fore_thread> m_kFGThreads;

	std::atomic<int32_t> m_i32BGState;
	std::atomic<int32_t> m_i32BGAvailableNum;
	VeVector<back_thread> m_kBGThreads;
	BGBuffer m_kWaitingThreads;

	job_queue m_akFGQueues[2][VeJob::PRI_MAX];
	job_queue m_akBGQueues[VeJob::PRI_MAX];

	std::atomic<int32_t> m_i32BGJobNum;
	std::atomic<uint32_t> m_u32FrameCount;
	
	vtd::obj_pool<VeJobFunc, VE_JOB_POOL_SIZE> m_kJobPool;
	vtd::obj_pool<co_normal, VE_JOB_FIBER_POOL_SIZE> m_kFiberPool;
	vtd::obj_pool<co_large, VE_JOB_LARGE_FIBER_POOL_SIZE> m_kLargeFiberPool;
	
};

#include "VeJobSystem.inl"
