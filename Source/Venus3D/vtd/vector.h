////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      vtd
//  File name:   vector.h
//  Created:     2016/07/07 by Albert
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

#include "utility.h"
#include <assert.h>
#include <malloc.h>
#include <string.h>

#include <vector>

namespace vtd
{
	template<class _Ty>
	class manipulator
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
			typedef manipulator<_Other> other;
		};

		static void memory_copy(pointer _Dst, const_pointer _Src, size_type _Count) noexcept
		{
			memcpy(_Dst, _Src, _Count * sizeof(value_type))
		}

		static pointer address(reference _Val) noexcept
		{
			return &_Val;
		}

		static const_pointer address(const_reference _Val) noexcept
		{
			return &_Val;
		}

		static void deallocate(pointer _Ptr, size_type _Count) noexcept
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
		manipulator() noexcept = delete;
		~manipulator() noexcept = delete;

	};

	template<class _Ty,
		class _Mani = manipulator<_Ty> >
		class vector
	{
	public:
		typedef typename _Mani::value_type value_type;
		typedef typename _Mani::pointer pointer;
		typedef typename _Mani::const_pointer const_pointer;
		typedef typename _Mani::reference reference;
		typedef typename _Mani::const_reference const_reference;
		typedef typename _Mani::size_type size_type;
		typedef typename _Mani::difference_type difference_type;

		typedef typename pointer iterator;
		typedef typename const_pointer const_iterator;

		vector() noexcept = default;

		vector(size_type _Count) noexcept
		{
			resize(_Count);
		}

		vector(size_type _Count, const value_type& _Val) noexcept
		{
			resize(_Count, _Val);
		}

		vector(const vector& _Copy) noexcept
		{
			assign(_Copy.begin(), _Copy.end());
		}

		vector(vector&& _Move) noexcept
		{
			buffer = _Move.buffer;
			used_size = _Move.used_size;
			max_size = _Move.max_size;
			_Move.buffer = nullptr;
			_Move.used_size = 0;
			_Move.max_size = 0;
		}

		vector& operator = (const vector& _Copy) noexcept
		{
			assign(_Copy.begin(), _Copy.end());
			return *this;
		}

		vector& operator = (vector&& _Move) noexcept
		{
			buffer = _Move.buffer;
			used_size = _Move.used_size;
			max_size = _Move.max_size;
			_Move.buffer = nullptr;
			_Move.used_size = 0;
			_Move.max_size = 0;
		}

		void clear() noexcept
		{
			resize(0);
		}

		void reserve(size_type _Count) noexcept
		{
			_Count = ((_Count + 0xF) >> 4) << 4;

			if (max_size < _Count)
			{
				if (buffer)
				{
					buffer = _Mani::allocate(_Count, buffer);
				}
				else
				{
					buffer = _Mani::allocate(_Count);
				}

				max_size = _Count;
			}
		}

		void resize(size_type _Newsize) noexcept
		{
			if (_Newsize > used_size)
			{
				if (_Newsize > max_size)
				{
					reserve(max(used_size + (used_size >> 1), _Newsize));
				}

				for (size_type i(used_size); i < _Newsize; ++i)
				{
					_Mani::construct(buffer + i);
				}
			}
			else if (_Newsize < used_size)
			{
				for (size_type i(_Newsize); i < used_size; ++i)
				{
					_Mani::destroy(buffer + i);
				}
			}

			used_size = _Newsize;
		}

		void resize(size_type _Newsize, const value_type& _Val) noexcept
		{
			if (_Newsize > used_size)
			{
				if (_Newsize > max_size)
				{
					reserve(max(used_size + (used_size >> 1), _Newsize));
				}

				for (size_type i(used_size); i < _Newsize; ++i)
				{
					_Mani::construct(buffer + i, _Val);
				}
			}
			else if (_Newsize < used_size)
			{
				for (size_type i(_Newsize); i < used_size; ++i)
				{
					_Mani::destroy(buffer + i);
				}
			}

			used_size = _Newsize;
		}

		size_type size() const noexcept
		{
			return used_size;
		}

		size_type capacity() const noexcept
		{
			return max_size;
		}

		bool empty() const noexcept
		{
			return used_size == 0;
		}

		void shrink_to_fit() noexcept
		{
			if (max_size > used_size)
			{
				if (used_size)
				{
					buffer = _Mani::allocate(used_size, buffer);
				}
				else
				{
					_Mani::deallocate(buffer, max_size);
				}
				max_size = used_size;
			}
		}


		iterator begin() noexcept
		{
			return buffer;
		}

		const_iterator begin() const noexcept
		{
			return buffer;
		}

		iterator end() noexcept
		{
			return buffer + used_size;
		}

		const_iterator end() const noexcept
		{
			return buffer + used_size;
		}

		const_iterator cbegin() const noexcept
		{
			return buffer;
		}

		const_iterator cend() const noexcept
		{
			return buffer + used_size;
		}

		reference at(size_type _Pos) noexcept
		{
			assert(_Pos < used_size);
			return buffer[_Pos];
		}

		const_reference at(size_type _Pos) const noexcept
		{
			assert(_Pos < used_size);
			return buffer[_Pos];
		}

		reference operator [] (size_type _Pos) noexcept
		{
			return at(_Pos);
		}

		const_reference operator [] (size_type _Pos) const noexcept
		{
			return at(_Pos);
		}

		const_pointer data() const noexcept
		{
			return buffer;
		}

		reference front() noexcept
		{
			return (*begin());
		}

		const_reference front() const noexcept
		{
			return (*begin());
		}

		reference back() noexcept
		{
			return (*(end() - 1));
		}

		const_reference back() const noexcept
		{
			return (*(end() - 1));
		}

		void assign(const_iterator itStart, const_iterator itLast) noexcept
		{
			difference_type diff = max(itLast - itStart, 0);
			resize(diff);
			_Mani::memory_copy(buffer, itStart, (size_type)diff);
		}

		void push_back(const value_type& _Val) noexcept
		{
			resize(used_size + 1, _Val);
		}

		void pop_back() noexcept
		{
			if (used_size)
			{
				resize(used_size - 1);
			}
		}

		iterator insert(const_iterator _Where, const _Ty& _Val) noexcept
		{
			if ((!_Where) || _Where == end())
			{
				push_back(_Val);
				return end() - 1;
			}
			else
			{
				assert(_Where >= begin() && _Where < end());
				difference_type diff = _Where - begin();
				resize(used_size + 1);
				iterator _Local = begin() + diff;
				for (iterator it = (end() - 1); it > _Local; --it)
				{
					*it = *(it - 1);
				}
				*_Local = _Val;
				return _Local;
			}
		}

		iterator erase(const_iterator _Where) noexcept
		{
			assert(_Where >= begin() && _Where < end());

			iterator _Local = (iterator)_Where;

			for (iterator it = _Local; it < (end() - 1); ++it)
			{
				*it = *(it + 1);
			}
			pop_back();

			return _Where;
		}

	private:
		

		pointer buffer = nullptr;
		size_type used_size = 0;
		size_type max_size = 0;

	};

}
