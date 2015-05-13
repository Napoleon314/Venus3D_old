////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeThread.cpp
//  Created:     2015/02/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeThreadHandle VeCreateThread(VeThreadCallback pfuncThreadProc,
	void* pvParam, VeUInt32 u32Priority, VeSizeT stStackSize) noexcept
{
#ifdef VE_PLATFORM_WIN
	VeThreadHandle hRes = (VeThreadHandle)_beginthreadex(nullptr,
		(unsigned int)stStackSize, pfuncThreadProc, pvParam, 0, nullptr);
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
bool VeJoinThread(VeThreadHandle hThread) noexcept
{
#ifdef VE_PLATFORM_WIN
	return WaitForSingleObject(hThread, VE_INFINITE) == WAIT_OBJECT_0;
#else
	return hThread ? VE_SUCCEEDED(pthread_join(hThread, nullptr)) : true;
#endif
}
//--------------------------------------------------------------------------
bool VeIsThreadActive(VeThreadHandle hThread) noexcept
{
#ifdef VE_PLATFORM_WIN
	DWORD dwCode(0);
	return (GetExitCodeThread(hThread, &dwCode) && dwCode == STILL_ACTIVE);
#else
	return hThread ? (pthread_kill(hThread, 0) != ESRCH) : false;
#endif
}
//--------------------------------------------------------------------------
VeThreadID VeGetLocalThread() noexcept
{
#ifdef VE_PLATFORM_WIN
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}
//--------------------------------------------------------------------------
void VeSleep(VeUInt32 u32Millisecond) noexcept
{
#ifdef VE_PLATFORM_WIN
	Sleep(u32Millisecond);
#else
	timespec kTimeSpec;
	kTimeSpec.tv_sec = u32Millisecond / 1000;
	kTimeSpec.tv_nsec = (u32Millisecond % 1000) * 1000000;
	nanosleep(&kTimeSpec, 0);
#endif
}
//--------------------------------------------------------------------------
bool VeThreadEventInit(VeThreadEvent* phEvent, bool bInitState) noexcept
{
	VE_ASSERT(phEvent);
#ifdef VE_PLATFORM_WIN
	return ((*phEvent = CreateEvent(nullptr, FALSE, bInitState, nullptr)) != nullptr);
#else
	phEvent->m_u32State = bInitState;
	pthread_mutex_init(&phEvent->m_kMutex, nullptr);
	return pthread_cond_init(&phEvent->m_kCond, nullptr) == 0;
#endif
}
//--------------------------------------------------------------------------
void VeThreadEventTerm(VeThreadEvent* phEvent) noexcept
{
	VE_ASSERT(phEvent);
#ifdef VE_PLATFORM_WIN
	if (*phEvent) CloseHandle(*phEvent);
	*phEvent = nullptr;
#else
	pthread_mutex_destroy(&phEvent->m_kMutex);
	pthread_cond_destroy(&phEvent->m_kCond);
#endif
}
//--------------------------------------------------------------------------
void VeThreadEventWait(VeThreadEvent* phEvent) noexcept
{
	VE_ASSERT(phEvent);
#ifdef VE_PLATFORM_WIN
	WaitForSingleObject(*phEvent, VE_INFINITE);
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
void VeThreadEventWait(VeThreadEvent* phEvent,
	VeUInt32 u32Milliseconds) noexcept
{
	VE_ASSERT(phEvent);
#ifdef VE_PLATFORM_WIN
	WaitForSingleObject(*phEvent, u32Milliseconds);
#else
	if (phEvent->m_u32State == 0)
	{
		timeval kNow;
		timespec kOutTime;
		gettimeofday(&kNow, nullptr);
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
void VeThreadEventSet(VeThreadEvent* phEvent) noexcept
{
	VE_ASSERT(phEvent);
#ifdef VE_PLATFORM_WIN
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
void VeThreadEventReset(VeThreadEvent* phEvent) noexcept
{
	VE_ASSERT(phEvent);
#ifdef VE_PLATFORM_WIN
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
VeThread::VeThread(VeUInt32 u32Priority, VeSizeT stStackSize) noexcept
	: m_u32State(0)
{
	m_pkParams = VE_NEW ThreadParams;
	m_pkParams->m_kEvent.Set();
	m_pkParams->m_pkThis = this;
	m_hThread = VeCreateThread(Callback, m_pkParams, u32Priority, stStackSize);
	VE_ASSERT(m_hThread);
}
//--------------------------------------------------------------------------
VeThread::~VeThread() noexcept
{
	VE_ASSERT(m_u32State == 0);
	m_pkParams->m_pkThis = nullptr;
	m_pkParams->m_kEventLoop.Set();
	m_pkParams = nullptr;
}
//--------------------------------------------------------------------------
VeThreadCallbackResult VE_CALLBACK VeThread::Callback(
	void* pvParam) noexcept
{
	ThreadParams* pkParams = (ThreadParams*)pvParam;
	Wait(pkParams->m_kEventLoop);
	while (pkParams->m_pkThis)
	{
		pkParams->m_pkThis->Run();
		{
			VE_AUTO_LOCK_MUTEX_NAME(pkParams->m_pkThis->m_kMutex, kMutex);
			--(pkParams->m_pkThis->m_u32State);
			pkParams->m_kEventLoop.Reset();
			pkParams->m_kEvent.Set();
		}
		Wait(pkParams->m_kEventLoop);
	}
	VE_DELETE(pkParams);
	return 0;
}
//--------------------------------------------------------------------------
VeTickThread::VeTickThread(VeUInt32 u32Sleep) noexcept
	: m_pkListener(nullptr), m_u32Sleep(u32Sleep), m_bNeedTick(true)
{

}
//--------------------------------------------------------------------------
VeTickThread::~VeTickThread() noexcept
{
	Stop();
}
//--------------------------------------------------------------------------
void VeTickThread::SetListener(TickListener* pkListener) noexcept
{
	m_pkListener = pkListener;
}
//--------------------------------------------------------------------------
void VeTickThread::Stop() noexcept
{
	m_bNeedTick = false;
	while (IsRunning()) {}
}
//--------------------------------------------------------------------------
void VeTickThread::Run() noexcept
{
	while (m_bNeedTick)
	{
		if (m_pkListener)
			m_pkListener->Tick();
		VeSleep(m_u32Sleep);
	}
}
//--------------------------------------------------------------------------
