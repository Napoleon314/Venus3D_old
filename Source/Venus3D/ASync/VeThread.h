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

private:
	void Callback() noexcept;

	Entry m_kEntry;
	std::thread m_kCore;
	Event m_kLoop;
	Event m_kJoin;
	std::atomic_uint m_u32State;

};

#include "VeThread.inl"
