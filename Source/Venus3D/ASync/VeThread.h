////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      ASync
//  File name:   VeThread.h
//  Created:     2016/07/11 by Albert
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

#ifdef BUILD_PLATFORM_WIN

typedef CRITICAL_SECTION VeThreadMutex;
typedef HANDLE VeThreadEvent;
typedef HANDLE VeThreadHandle;
typedef DWORD VeThreadID;
typedef unsigned int VeThreadCallbackResult;

#define VeThreadMutexInit(x) InitializeCriticalSection(&x)
#define VeThreadMutexTerm(x) DeleteCriticalSection(&x)
#define VeThreadMutexLock(x) EnterCriticalSection(&x)
#define VeThreadMutexUnlock(x) LeaveCriticalSection(&x)

#define VE_THREAD_PRIORITY_IDLE THREAD_BASE_PRIORITY_IDLE
#define VE_THREAD_PRIORITY_LOWEST THREAD_PRIORITY_LOWEST
#define VE_THREAD_PRIORITY_BELOW_NORMAL THREAD_PRIORITY_BELOW_NORMAL
#define VE_THREAD_PRIORITY_NORMAL THREAD_PRIORITY_NORMAL
#define VE_THREAD_PRIORITY_ABOVE_NORMAL THREAD_PRIORITY_ABOVE_NORMAL
#define VE_THREAD_PRIORITY_HIGHEST THREAD_PRIORITY_HIGHEST
#define VE_THREAD_PRIORITY_TIME_CRITICAL THREAD_PRIORITY_TIME_CRITICAL

#else

typedef pthread_mutex_t VeThreadMutex;
typedef struct
{
	pthread_cond_t m_kCond;
	pthread_mutex_t m_kMutex;
	VeUInt32 m_u32State;
} VeThreadEvent;
typedef pthread_t VeThreadHandle;
typedef pthread_t VeThreadID;
typedef void* VeThreadCallbackResult;

#define VeThreadMutexInit(x) pthread_mutex_init(&x, NULL)
#define VeThreadMutexTerm(x) pthread_mutex_destroy(&x)
#define VeThreadMutexLock(x) pthread_mutex_lock(&x)
#define VeThreadMutexUnlock(x) pthread_mutex_unlock(&x)

#define VE_THREAD_PRIORITY_IDLE 1
#define VE_THREAD_PRIORITY_LOWEST 16
#define VE_THREAD_PRIORITY_BELOW_NORMAL 24
#define VE_THREAD_PRIORITY_NORMAL 32
#define VE_THREAD_PRIORITY_ABOVE_NORMAL 40
#define VE_THREAD_PRIORITY_HIGHEST 48
#define VE_THREAD_PRIORITY_TIME_CRITICAL 100

#endif

enum VeThreadResult
{
	VE_THREAD_WAIT_SUCCEED,
	VE_THREAD_WAIT_TIMEOUT,
	VE_THREAD_WAIT_ABANDON,
	VE_THREAD_WAIT_FAILED
};

typedef VeThreadCallbackResult(VE_CALLBACK * VeThreadCallback)(void* pvParam);

VENUS_API VeThreadHandle VeCreateThread(VeThreadCallback pfuncThreadProc, void* pvParam, uint32_t u32Priority = VE_THREAD_PRIORITY_NORMAL, size_t stStackSize = 32768);

VENUS_API bool VeJoinThread(VeThreadHandle hThread);

VENUS_API bool VeIsThreadActive(VeThreadHandle hThread);

VENUS_API VeThreadID VeGetLocalThread();

VENUS_API void VeSleep(uint32_t u32Millisecond);

VENUS_API bool VeThreadEventInit(VeThreadEvent* phEvent, bool bInitState = false);

VENUS_API void VeThreadEventTerm(VeThreadEvent* phEvent);

VENUS_API void VeThreadEventWait(VeThreadEvent* phEvent);

VENUS_API void VeThreadEventWait(VeThreadEvent* phEvent, uint32_t u32Milliseconds);

VENUS_API void VeThreadEventSet(VeThreadEvent* phEvent);

VENUS_API void VeThreadEventReset(VeThreadEvent* phEvent);

class VENUS_API VeThread : public VeRefObject
{
	VeNoCopy(VeThread);
	VeNoMove(VeThread);
public:
	class Event
	{
		VeNoCopy(Event);
		VeNoMove(Event);
	public:
		Event() noexcept
		{

		}

		~Event() noexcept
		{

		}

		void Wait() noexcept
		{
			std::unique_lock<std::mutex> kLock(m_kMutex);
			while (m_i32Count <= 0)
			{
				m_kCondition.wait(kLock);
			}
		}

		void Set(int32_t i32Step = 1) noexcept
		{
			std::lock_guard<std::mutex> kLock(m_kMutex);
			m_i32Count += i32Step;
			m_kCondition.notify_all();
		}

		void Reset(int32_t i32Count = 1) noexcept
		{
			assert(i32Count > 0);
			std::lock_guard<std::mutex> kLock(m_kMutex);
			m_i32Count = 1 - i32Count;
		}

	private:
		std::mutex m_kMutex;
		std::condition_variable m_kCondition;
		int32_t m_i32Count = 0;

	};

	typedef std::function<void()> Entry;

	VeThread() noexcept;

	VeThread(const std::function<void()>& kEntry) noexcept;

	virtual ~VeThread() noexcept;

	inline bool IsRunning() noexcept;

	void Start() noexcept;

	void StartEntry(const std::function<void()>& kEntry) noexcept;

	void SetEntry(const std::function<void()>& kEntry) noexcept;

	void Join() noexcept;

	void Suspend() noexcept;

	void Resume() noexcept;

	static void Init();

	static void Term();

private:
	void Callback() noexcept;

	Entry m_kEntry;
	std::thread m_kCore;
	Event m_kLoop;
	Event m_kJoin;
	std::atomic_uint m_u32State;

};

#include "VeThread.inl"
