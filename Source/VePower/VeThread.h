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

#if defined(VE_PLATFORM_WIN)
#	define VeAtomicIncrement32(val) InterlockedIncrement((volatile LONG*)&(val))
#	define VeAtomicDecrement32(val) InterlockedDecrement((volatile LONG*)&(val))
#elif defined(VE_PLATFORM_LINUX) || defined(VE_PLATFORM_ANDROID)
#	define VeAtomicIncrement32(val) __sync_add_and_fetch(&val,1)
#	define VeAtomicDecrement32(val) __sync_sub_and_fetch(&val,1)
#elif defined(VE_PLATFORM_APPLE)
#define VeAtomicIncrement32(val) OSAtomicIncrement32((volatile int32_t*)&(val))
#define VeAtomicDecrement32(val) OSAtomicDecrement32((volatile int32_t*)&(val))
#endif

inline void VeSleep(VeUInt32 u32Millisecond)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

class VeSpinLock
{
	VE_NO_COPY(VeSpinLock);
public:
	VeSpinLock() noexcept = default;

	~VeSpinLock() noexcept = default;

	void lock() noexcept
	{
		while (m_atomFlag.test_and_set(std::memory_order_acquire))
			;
	}

	void unlock() noexcept
	{
		m_atomFlag.clear(std::memory_order_release);
	}

private:
	std::atomic_flag m_atomFlag = ATOMIC_FLAG_INIT;

};

#define VE_LOCK_MUTEX(mutex) std::lock_guard<VeSpinLock> mutex##Guard(mutex)
#define VE_LOCK_MUTEX_NAME(mutex,name) std::lock_guard<VeSpinLock> name(mutex)

class VE_POWER_API VeThread : public VeRefObject
{
	VE_NO_COPY(VeThread);
public:
	class Event
	{
		VE_NO_COPY(Event);
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

		void Set(VeInt32 i32Step = 1) noexcept
		{
			std::lock_guard<std::mutex> kLock(m_kMutex);
			m_i32Count += i32Step;
			m_kCondition.notify_all();
		}

		void Reset(VeInt32 i32Count = 1) noexcept
		{
			VE_ASSERT(i32Count > 0);
			std::lock_guard<std::mutex> kLock(m_kMutex);
			m_i32Count = 1 - i32Count;
		}

	private:
		std::mutex m_kMutex;
		std::condition_variable m_kCondition;
		volatile VeInt32 m_i32Count = 0;

	};

	VeThread() noexcept;

	virtual ~VeThread() noexcept;

	inline bool IsRunning() noexcept;

	void Start() noexcept;

	void Join() noexcept;

	virtual void Run() noexcept = 0;

private:
	void Callback() noexcept;

	std::thread m_kCore;
	Event m_kLoop;
	Event m_kJoin;
	std::atomic_uint m_u32State;
	
};

#include "VeThread.inl"
