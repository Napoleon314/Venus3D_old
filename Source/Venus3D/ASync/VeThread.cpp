////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      ASync
//  File name:   VeThread.cpp
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

#include "stdafx.h"

//--------------------------------------------------------------------------
VeThreadHandle VeCreateThread(VeThreadCallback pfuncThreadProc,
	void* pvParam, uint32_t u32Priority, size_t stStackSize)
{
#ifdef  BUILD_PLATFORM_WIN
	VeThreadHandle hRes = (VeThreadHandle)_beginthreadex(NULL,
		(unsigned int)stStackSize, pfuncThreadProc, pvParam, 0, NULL);
	SetThreadPriority(hRes, u32Priority);
	return hRes;
#else
	VeThreadHandle hThread(0);
	pthread_attr_t kAttr;
	pthread_attr_init(&kAttr);
	sched_param kParam;
	pthread_attr_getschedparam(&kAttr, &kParam);
	kParam.sched_priority = u32Priority;
	pthread_attr_setschedparam(&kAttr, &kParam);
	pthread_attr_setstacksize(&kAttr, stStackSize ? stStackSize : 4096);
	pthread_create(&hThread, &kAttr, pfuncThreadProc, pvParam);
	return hThread;
#endif
}
//--------------------------------------------------------------------------
bool VeJoinThread(VeThreadHandle hThread)
{
#ifdef  BUILD_PLATFORM_WIN
	return WaitForSingleObject(hThread, UINT32_MAX) == WAIT_OBJECT_0;
#else
	return hThread ? VE_SUCCEEDED(pthread_join(hThread, NULL)) : true;
#endif
}
//--------------------------------------------------------------------------
bool VeIsThreadActive(VeThreadHandle hThread)
{
#ifdef  BUILD_PLATFORM_WIN
	DWORD dwCode(0);
	return (GetExitCodeThread(hThread, &dwCode) && dwCode == STILL_ACTIVE);
#else
	return hThread ? (pthread_kill(hThread, 0) != ESRCH) : false;
#endif
}
//--------------------------------------------------------------------------
VeThreadID VeGetLocalThread()
{
#ifdef  BUILD_PLATFORM_WIN
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}
//--------------------------------------------------------------------------
void VeSleep(uint32_t u32Millisecond)
{
#ifdef  BUILD_PLATFORM_WIN
	Sleep(u32Millisecond);
#else
	timespec kTimeSpec;
	kTimeSpec.tv_sec = u32Millisecond / 1000;
	kTimeSpec.tv_nsec = (u32Millisecond % 1000) * 1000000;
	nanosleep(&kTimeSpec, 0);
#endif
}
//--------------------------------------------------------------------------
bool VeThreadEventInit(VeThreadEvent* phEvent, bool bInitState)
{
#ifdef  BUILD_PLATFORM_WIN
	return ((*phEvent = CreateEvent(NULL, FALSE, bInitState, NULL)) != NULL);
#else
	phEvent->m_u32State = bInitState;
	pthread_mutex_init(&phEvent->m_kMutex, NULL);
	return pthread_cond_init(&phEvent->m_kCond, NULL) == 0;
#endif
}
//--------------------------------------------------------------------------
void VeThreadEventTerm(VeThreadEvent* phEvent)
{
#ifdef  BUILD_PLATFORM_WIN
	if (*phEvent) CloseHandle(*phEvent);
	*phEvent = NULL;
#else
	pthread_mutex_destroy(&phEvent->m_kMutex);
	pthread_cond_destroy(&phEvent->m_kCond);
#endif
}
//--------------------------------------------------------------------------
void VeThreadEventWait(VeThreadEvent* phEvent)
{
#ifdef  BUILD_PLATFORM_WIN
	WaitForSingleObject(*phEvent, INFINITE);
#else
	if (phEvent->m_u32State == 0)
	{
		pthread_mutex_lock(&phEvent->m_kMutex);
		pthread_cond_wait(&phEvent->m_kCond, &phEvent->m_kMutex);
		pthread_mutex_unlock(&phEvent->m_kMutex);
	}
#endif
}
//--------------------------------------------------------------------------
void VeThreadEventWait(VeThreadEvent* phEvent, uint32_t u32Milliseconds)
{
#ifdef  BUILD_PLATFORM_WIN
	WaitForSingleObject(*phEvent, u32Milliseconds);
#else
	if (phEvent->m_u32State == 0)
	{
		timeval kNow;
		timespec kOutTime;
		gettimeofday(&kNow, NULL);
		kOutTime.tv_sec = kNow.tv_sec + u32Milliseconds / 1000;
		kOutTime.tv_nsec = kNow.tv_usec * 1000
			+ (u32Milliseconds % 1000) * 1000000;
		pthread_mutex_lock(&phEvent->m_kMutex);
		pthread_cond_timedwait(&phEvent->m_kCond,
			&phEvent->m_kMutex, &kOutTime);
		pthread_mutex_unlock(&phEvent->m_kMutex);
	}
#endif
}
//--------------------------------------------------------------------------
void VeThreadEventSet(VeThreadEvent* phEvent)
{
#ifdef  BUILD_PLATFORM_WIN
	SetEvent(*phEvent);
#else
	if (phEvent->m_u32State == 0)
	{
		pthread_mutex_lock(&phEvent->m_kMutex);
		phEvent->m_u32State = 1;
		pthread_cond_broadcast(&phEvent->m_kCond);
		pthread_mutex_unlock(&phEvent->m_kMutex);
	}
#endif
}
//--------------------------------------------------------------------------
void VeThreadEventReset(VeThreadEvent* phEvent)
{
#ifdef  BUILD_PLATFORM_WIN
	ResetEvent(*phEvent);
#else
	if (phEvent->m_u32State == 1)
	{
		pthread_mutex_lock(&phEvent->m_kMutex);
		phEvent->m_u32State = 0;
		pthread_mutex_unlock(&phEvent->m_kMutex);
	}
#endif
}

//--------------------------------------------------------------------------
VeThread::VeThread() noexcept
{
	m_u32State.store(0, std::memory_order_relaxed);
	m_kJoin.Set();
	m_kCore = std::thread([this]() noexcept
	{
		Callback();
	});
}
//--------------------------------------------------------------------------
VeThread::VeThread(const std::function<void()>& kEntry) noexcept
{
	SetEntry(kEntry);
}
//--------------------------------------------------------------------------
VeThread::~VeThread() noexcept
{
	while (IsRunning())
		;
	m_u32State.store(2, std::memory_order_relaxed);
	m_kLoop.Set();
	m_kCore.join();
}
//--------------------------------------------------------------------------
void VeThread::Start() noexcept
{
	uint32_t u32Stop(0);
	if (m_u32State.compare_exchange_weak(u32Stop, 1u, std::memory_order_relaxed))
	{
		m_kLoop.Set();
		m_kJoin.Reset();
	}
}
//--------------------------------------------------------------------------
void VeThread::StartEntry(const std::function<void()>& kEntry) noexcept
{
	uint32_t u32Stop(0);
	if (m_u32State.compare_exchange_weak(u32Stop, 1u, std::memory_order_relaxed))
	{
		m_kEntry = kEntry;
		m_kLoop.Set();
		m_kJoin.Reset();
	}
}
//--------------------------------------------------------------------------
void VeThread::SetEntry(const std::function<void()>& kEntry) noexcept
{
	if (m_u32State.load(std::memory_order_relaxed) == 0)
	{
		m_kEntry = kEntry;
	}
}
//--------------------------------------------------------------------------
void VeThread::Join() noexcept
{
	m_kJoin.Wait();
}
//--------------------------------------------------------------------------
#ifndef BUILD_PLATFORM_WIN
//--------------------------------------------------------------------------
#define RESUME_SIG SIGUSR2
#define SUSPEND_SIG SIGUSR1
//--------------------------------------------------------------------------
static sigset_t wait_mask;
static __thread int suspended;
//--------------------------------------------------------------------------
void resume_handler(int) noexcept
{
    suspended = 0;
}
//--------------------------------------------------------------------------
void suspend_handler(int) noexcept
{
    if (suspended) return;
    suspended = 1;
    do sigsuspend(&wait_mask); while (suspended);
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
void VeThread::Suspend() noexcept
{
#	ifdef BUILD_PLATFORM_WIN
    SuspendThread(m_kCore.native_handle());
#   else
    pthread_kill(m_kCore.native_handle(), SUSPEND_SIG);
#	endif
}
//--------------------------------------------------------------------------
void VeThread::Resume() noexcept
{
#	ifdef BUILD_PLATFORM_WIN
    ResumeThread(m_kCore.native_handle());
#   else
    pthread_kill(m_kCore.native_handle(), RESUME_SIG);
#	endif
}
//--------------------------------------------------------------------------
void VeThread::Init()
{
#	ifndef BUILD_PLATFORM_WIN
    struct sigaction sa;
    sigfillset(&wait_mask);
    sigdelset(&wait_mask, SUSPEND_SIG);
    sigdelset(&wait_mask, RESUME_SIG);
    sigfillset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = resume_handler;
    sigaction(RESUME_SIG, &sa, nullptr);
    sa.sa_handler = suspend_handler;
    sigaction(SUSPEND_SIG, &sa, nullptr);
#	endif
}
//--------------------------------------------------------------------------
void VeThread::Term()
{
#	ifndef BUILD_PLATFORM_WIN
	
#	endif
}
//--------------------------------------------------------------------------
void VeThread::Callback() noexcept
{
	do
	{
		m_kLoop.Wait();
		if (m_u32State.load(std::memory_order_relaxed) == 1)
		{
			m_kEntry();
			m_u32State.store(0, std::memory_order_relaxed);
			m_kLoop.Reset();
			m_kJoin.Set();
		}
	} while (m_u32State.load(std::memory_order_relaxed) < 2);
}
//--------------------------------------------------------------------------
