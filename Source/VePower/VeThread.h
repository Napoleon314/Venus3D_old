////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeThread.h
//  Created:     2015/02/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#if defined(VE_PLATFORM_WIN) || defined(VE_PLATFORM_MOBILE_SIM)
#	define VeAtomicIncrement32(val) InterlockedIncrement((volatile LONG*)&(val))
#	define VeAtomicDecrement32(val) InterlockedDecrement((volatile LONG*)&(val))
#elif defined(VE_PLATFORM_LINUX) || defined(VE_PLATFORM_ANDROID)
#	define VeAtomicIncrement32(val) __sync_add_and_fetch(&val,1)
#	define VeAtomicDecrement32(val) __sync_sub_and_fetch(&val,1)
#elif defined(VE_PLATFORM_APPLE)
#define VeAtomicIncrement32(val) OSAtomicIncrement32((volatile int32_t*)&(val))
#define VeAtomicDecrement32(val) OSAtomicDecrement32((volatile int32_t*)&(val))
#endif

#if defined(VE_PLATFORM_WIN) || defined(VE_PLATFORM_MOBILE_SIM)

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

#define VeThreadMutexInit(x) pthread_mutex_init(&x, nullptr)
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

VE_POWER_API VeThreadHandle VeCreateThread(VeThreadCallback pfuncThreadProc, void* pvParam, VeUInt32 u32Priority = VE_THREAD_PRIORITY_NORMAL, VeSizeT stStackSize = 32768) noexcept;

VE_POWER_API bool VeJoinThread(VeThreadHandle hThread) noexcept;

VE_POWER_API bool VeIsThreadActive(VeThreadHandle hThread) noexcept;

VE_POWER_API VeThreadID VeGetLocalThread() noexcept;

VE_POWER_API void VeSleep(VeUInt32 u32Millisecond) noexcept;

VE_POWER_API bool VeThreadEventInit(VeThreadEvent* phEvent, bool bInitState = false) noexcept;

VE_POWER_API void VeThreadEventTerm(VeThreadEvent* phEvent) noexcept;

VE_POWER_API void VeThreadEventWait(VeThreadEvent* phEvent) noexcept;

VE_POWER_API void VeThreadEventWait(VeThreadEvent* phEvent, VeUInt32 u32Milliseconds) noexcept;

VE_POWER_API void VeThreadEventSet(VeThreadEvent* phEvent) noexcept;

VE_POWER_API void VeThreadEventReset(VeThreadEvent* phEvent) noexcept;

class VE_POWER_API VeThread : public VeMemObject
{
public:
	class VE_POWER_API Mutex : public VeMemObject
	{
	public:
		Mutex() noexcept
		{
			VeThreadMutexInit(m_kMutex);
		}

		~Mutex() noexcept
		{
			VeThreadMutexTerm(m_kMutex);
		}

		void Lock() noexcept
		{
			VeThreadMutexLock(m_kMutex);
		}

		void Unlock() noexcept
		{
			VeThreadMutexUnlock(m_kMutex);
		}

		Mutex& GetRef() noexcept
		{
			return *this;
		}

	private:
		VeThreadMutex m_kMutex;

	};

	class VE_POWER_API MutexHolder : public VeMemObject
	{
	public:
		MutexHolder(Mutex* pkMutex) noexcept
			: m_pkMutex(pkMutex)
		{
			VE_ASSERT(pkMutex);
			m_pkMutex->Lock();
		}

		~MutexHolder() noexcept
		{
			m_pkMutex->Unlock();
		}

	private:
		Mutex* m_pkMutex;

	};

	class VE_POWER_API Event : public VeMemObject
	{
	public:
		Event(bool bState = false) noexcept
		{
			VeThreadEventInit(&m_kEvent, bState);
		}

		~Event() noexcept
		{
			VeThreadEventTerm(&m_kEvent);
		}

		void Set() noexcept
		{
			VeThreadEventSet(&m_kEvent);
		}

		void Reset() noexcept
		{
			VeThreadEventReset(&m_kEvent);
		}

	private:
		friend class VeThread;

		VeThreadEvent m_kEvent;
	};

	VeThread(VeUInt32 u32Priority = VE_THREAD_PRIORITY_NORMAL, VeSizeT stStackSize = 32768) noexcept;

	virtual ~VeThread() noexcept;

	virtual void Run() noexcept = 0;

	inline void Start() noexcept;

	inline bool IsRunning() noexcept;

	inline static void Wait(Event& kEvent) noexcept;

	inline static void Wait(Event& kEvent, VeUInt32 u32Milliseconds) noexcept;

	inline static void Join(VeThread& kThread) noexcept;

private:
	typedef VeThread* ThisPointer;

	struct ThreadParams : public VeMemObject
	{
		ThreadParams() noexcept {}

		Event m_kEvent;
		Event m_kEventLoop;
		volatile ThisPointer m_pkThis;
	};
	VeThreadHandle m_hThread;
	volatile VeUInt32 m_u32State;
	ThreadParams* m_pkParams;
	Mutex m_kMutex;

	static VeThreadCallbackResult VE_CALLBACK Callback(void* pvParam) noexcept;

};

class VE_POWER_API VeTickThread : public VeThread
{
public:
	class TickListener : public VeMemObject
	{
	public:
		virtual void Tick() noexcept = 0;
	};

	VeTickThread(VeUInt32 u32Sleep = 10) noexcept;

	virtual ~VeTickThread() noexcept;

	void SetListener(TickListener* pkListener) noexcept;

	void Stop() noexcept;

protected:
	virtual void Run() noexcept;

	TickListener* m_pkListener;
	VeUInt32 m_u32Sleep;
	volatile bool m_bNeedTick;

};

#define VE_AUTO_LOCK_MUTEX(mutex) VeThread::MutexHolder mutex##Holder(&mutex)
#define VE_AUTO_LOCK_MUTEX_NAME(mutex,name) VeThread::MutexHolder name(&mutex)

#include "VeThread.inl"
