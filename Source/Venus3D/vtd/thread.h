////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      vtd
//  File name:   thread.h
//  Created:     2016/07/13 by Albert
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

#include <atomic>
#include <thread>

namespace vtd
{
	class event
	{
		event(const event&) = delete;
		event& operator= (const event&) = delete;
		event(event&&) = delete;
		event& operator= (event&&) = delete;
	public:
		event() noexcept
		{

		}

		~event() noexcept
		{

		}

		void wait() noexcept
		{
			std::unique_lock<std::mutex> ul(_Lock);
			while (_Count <= 0)
			{
				_Condition.wait(ul);
			}
		}

		void set(int32_t s = 1) noexcept
		{
			std::lock_guard<std::mutex> l(_Lock);
			_Count += s;
			_Condition.notify_all();
		}

		void reset(int32_t c = 1) noexcept
		{
			assert(c > 0);
			std::lock_guard<std::mutex> l(_Lock);
			_Count = 1 - c;
		}

	private:
		std::mutex _Lock;
		std::condition_variable _Condition;
		int32_t _Count = 0;

	};

	class thread
	{
		thread(const thread&) = delete;
		thread& operator= (const thread&) = delete;
		thread(thread&&) = delete;
		thread& operator= (thread&&) = delete;
	public:
		typedef std::function<void()> entry;

		thread() noexcept
		{
			_State.store(0, std::memory_order_relaxed);
			_Join.set();
			_Core = std::thread([this]() noexcept
			{
				callback();
			});
		}

		thread(entry&& _Fx) noexcept
			: thread()
		{
			_Entry = _Fx;
		}

		virtual ~thread() noexcept
		{
			while (active())
				;
			_State.store(2, std::memory_order_relaxed);
			_Loop.set();
			_Core.join();
		}

		bool active() noexcept
		{
			return _State.load(std::memory_order_relaxed) == 1;
		}
		
		void start() noexcept
		{
			if (_Entry != nullptr)
			{
				uint32_t check(0);
				if (_State.compare_exchange_weak(check, 1u, std::memory_order_relaxed))
				{
					_Loop.set();
					_Join.reset();
				}
			}
		}

		void start(entry&& _Fx) noexcept
		{
			uint32_t check(0);
			if (_State.compare_exchange_weak(check, 1u, std::memory_order_relaxed))
			{
				_Entry = _Fx;
				_Loop.set();
				_Join.reset();
			}
		}

		void join() noexcept
		{
			_Join.wait();
		}


	private:
		void callback() noexcept
		{
			do
			{
				_Loop.wait();
				if (_State.load(std::memory_order_relaxed) == 1)
				{
					_Entry();
					_State.store(0, std::memory_order_relaxed);
					_Loop.reset();
					_Join.set();
				}
			} while (_State.load(std::memory_order_relaxed) < 2);
		}

		entry _Entry;
		std::thread _Core;
		event _Loop;
		event _Join;
		std::atomic_uint _State;

	};
}
