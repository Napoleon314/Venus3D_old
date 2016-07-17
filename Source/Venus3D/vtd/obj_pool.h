////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      vtd
//  File name:   obj_pool.h
//  Created:     2016/07/16 by Albert
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

#include "ring_buffer.h"
#include "intrusive_ptr.h"

namespace vtd
{
	template <class _Ty, size_t _Num>
	class obj_pool
	{
	public:
		obj_pool() noexcept
		{
			_Free.store(0, std::memory_order_relaxed);
		}

		_Ty* acquire() noexcept
		{
			size_t i = _Free.fetch_add(1, std::memory_order_relaxed);
			if (i < _Num)
			{
				return _Objs + i;
			}
			else
			{
				return _Recycle.pop();
			}
		}

		void release(_Ty* p) noexcept
		{
			assert(size_t(p - _Objs) < _Num);
			_Recycle.push(p);
		}

	private:
		_Ty _Objs[_Num];
		std::atomic<size_t> _Free;
		ring_buffer<_Ty*, nullptr, _Num> _Recycle;

	};

}
