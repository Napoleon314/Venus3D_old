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

#define VE_JOB_QUEUE_SIZE 256

VeSmartPointer(VeJob);

typedef vtd::ring_buffer<VeJob*, nullptr, VE_JOB_QUEUE_SIZE> VeJobQueue;

class VENUS_API VeJob : public VeRefObject
{
public:
	typedef std::function<bool(uint32_t)> Task;

	enum Execute
	{
		EXE_BACKGROUND,
		EXE_FG_CURRENT,
		EXE_FG_NEXT,
		EXE_MAX
	};

	enum Priority
	{
		PRI_HIGH,
		PRI_NORMAL,
		PRI_LOW,
		PRI_MAX
	};

	struct Atom
	{
		Execute m_eExecute;
		Priority m_ePriority;
		Task m_funcTask;
	};

	VeJob() noexcept = default;

	virtual ~VeJob() noexcept;

	void ClearTasks() noexcept;

	void AddTask(Task funcTask, Execute eExecute,
		Priority ePriority = PRI_NORMAL) noexcept;

	bool Start() noexcept;

	void Wait(const VeJobPtr& spJob) noexcept;

	virtual void OnEnd(bool) noexcept = 0;

private:
	friend class VeJobSystem;

	void Run(uint32_t u32Index) noexcept;

	void End(bool bSuccess) noexcept;

	void Resume() noexcept;

	uint32_t m_u32Pointer = 0;
	uint32_t m_u32Wait = 0;
	VeVector<Atom> m_kTaskArray;
	VeVector<VeJobPtr> m_kRelatedJobs;
	vtd::spin_lock m_kLock;

};

class VENUS_API VeJobSystem : public VeSingleton<VeJobSystem>
{
public:
	VeJobSystem(size_t stFGNum, size_t stBGNum) noexcept;

	~VeJobSystem() noexcept;

	void ProcessForegroundJobs() noexcept;

private:
	friend class VeJob;
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

	typedef vtd::ring_buffer<back_thread*, nullptr, VE_JOB_BG_BUFFER_MASK> BGBuffer;

	std::atomic<int32_t> m_i32FGState;
	std::atomic<int32_t> m_i32FGJoinValue;
	signal m_kFGLoop;
	signal m_kFGJoin;
	VeVector<fore_thread> m_kFGThreads;

	std::atomic<int32_t> m_i32BGState;
	std::atomic<int32_t> m_i32BGAvailableNum;
	VeVector<back_thread> m_kBGThreads;
	BGBuffer m_kWaitingThreads;

	VeJobQueue m_akFGQueues[2][VeJob::PRI_MAX];
	VeJobQueue m_akBGQueues[VeJob::PRI_MAX];

	std::atomic<int32_t> m_i32BGJobNum;
	std::atomic<uint32_t> m_u32FrameCount;
	
};

#include "VeJobSystem.inl" 
