////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      vtd
//  File name:   stack.h
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

namespace vtd
{
	template<class _Ty, size_t _Num = 32>
	class stack
	{
	public:
		typedef _Ty value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		stack() noexcept = default;

		void push(const value_type& _Val) noexcept
		{
			assert(_Pointer < _Num);
			_Stack[_Pointer++] = _Val;
		}

		value_type pop() noexcept
		{
			assert(_Pointer > 0);
			return _Stack[--_Pointer];
		}

		value_type& top() noexcept
		{
			assert(_Pointer > 0);
			return _Stack[_Pointer - 1];
		}

		size_type size() noexcept
		{
			return _Pointer;
		}

		bool empty() noexcept
		{
			return size() == 0;
		}

	private:
		value_type _Stack[_Num];
		size_type _Pointer = 0;

	};
}
