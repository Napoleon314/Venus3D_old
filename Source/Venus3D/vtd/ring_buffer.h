////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   ring_buffer.h
//  Created:     2016/07/10 by Albert
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

#ifndef VTD_QUEUE_MASK
#	define VTD_QUEUE_MASK (0x7FF)
#endif

namespace vtd
{
	template <class _Ty, _Ty _Default, size_t _Mask = VTD_QUEUE_MASK>
	class ring_buffer
	{
	public:
		typedef _Ty value_type;
		typedef value_type* pointer;
		typedef uint64_t size_type;

		ring_buffer() noexcept
		{
			_Head.store(0);
			_Tail.store(0);
		}

		~ring_buffer() noexcept = default;

		void push(value_type _Val) noexcept
		{
			size_type hpos = _Head.fetch_add(1, std::memory_order_relaxed);
			buffer[hpos & _Mask] = _Val;
		}

		value_type pop() noexcept
		{
			size_type h, t;
			do
			{
				h = _Head.load(std::memory_order_relaxed);
				t = _Tail.load(std::memory_order_relaxed);
				if (t >= h)
				{
					return _Default;
				}
			} while (!_Tail.compare_exchange_weak(t, t + 1, std::memory_order_relaxed));
			return buffer[t & _Mask];
		}

	private:
		static constexpr size_type _Max = _Mask + 1;

		ring_buffer(const ring_buffer&) = delete;
		ring_buffer(ring_buffer&&) = delete;
		ring_buffer& operator = (const ring_buffer&) = delete;

		std::atomic<size_type> _Head;
		std::atomic<size_type> _Tail;
		value_type buffer[_Max];

	};

}

#ifdef VTD_QUEUE_MASK
#	undef VTD_QUEUE_MASK
#endif
