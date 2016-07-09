////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      vtd
//  File name:   allocator.h
//  Created:     2016/07/09 by Albert
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

#ifdef __APPLE__
#   include <stdlib.h>
#else
#   include <malloc.h>
#endif
#include <string.h>
#include <type_traits>

namespace vtd
{
	template<class _Ty>
	class allocator
	{
	public:
		typedef _Ty value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		struct _Shell
		{
			value_type data;
		};

		template<class _Other>
		struct rebind
		{
			typedef allocator<_Other> other;
		};

		static void memory_copy(pointer _Dst, const_pointer _Src, size_type _Count) noexcept
		{
			memcpy(_Dst, _Src, _Count * sizeof(value_type));
		}

		static pointer address(reference _Val) noexcept
		{
			return &_Val;
		}

		static const_pointer address(const_reference _Val) noexcept
		{
			return &_Val;
		}

		static void deallocate(pointer _Ptr) noexcept
		{
			free(_Ptr);
		}

		static pointer allocate(size_type _Count) noexcept
		{
			return (pointer)malloc(_Count * sizeof(value_type));
		}

		static pointer allocate(size_type _Count, pointer _Ptr) noexcept
		{
			return (pointer)realloc(_Ptr, _Count * sizeof(value_type));
		}

		template<class... _Types>
		static void construct(value_type *_Ptr, _Types&&... _Args) noexcept
		{
			::new ((void *)_Ptr) value_type(std::forward<_Types>(_Args)...);
		}

		static void destroy(pointer _Ptr) noexcept
		{
			((_Shell*)_Ptr)->~_Shell();
			_Ptr = nullptr;
		}

		static size_t max_size() noexcept
		{
			return ((size_t)(-1) / sizeof(value_type));
		}

	private:
		allocator() noexcept = delete;
		~allocator() noexcept = delete;

	};
}
