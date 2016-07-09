////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      vtd
//  File name:   utility.h
//  Created:     2016/04/01 by Albert D Yang
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

#include <stdint.h>
#include <utility>

#ifdef max
#	undef max
#endif
#ifdef min
#	undef min
#endif

namespace vtd
{
	template<class _Ty>
	struct identity
	{
		typedef _Ty type;

		const _Ty& operator()(const _Ty& _Left) const
		{
			return (_Left);
		}
	};

	namespace detail
	{
		template<class... _Types>
		struct comparer;

		template<class _Ty>
		struct comparer<_Ty>
		{
			static _Ty max(_Ty val) noexcept
			{			
				return val;
			}

			static _Ty min(_Ty val) noexcept
			{
				return val;
			}
		};

		template<class _This, class... _Rest>
		struct comparer<_This, _Rest...>
		{
			static _This max(_This val, _Rest... pak) noexcept
			{
				static_assert(sizeof...(_Rest) > 0,
					"Number of factors has to be more than two.");
				_This pre = comparer<_Rest...>::max(pak...);
				return val > pre ? val : pre;
			}

			static _This min(_This val, _Rest... pak) noexcept
			{
				static_assert(sizeof...(_Rest) > 0,
					"Number of factors has to be more than two.");
				_This pre = comparer<_Rest...>::min(pak...);
				return val < pre ? val : pre;
			}
		};


	}

	template<class _This, class... _Rest>
	_This max(_This val, _Rest... pak) noexcept
	{
		return detail::comparer<_This, _Rest...>::max(val, pak...);
	}

	template<class _This, class... _Rest>
	_This min(_This val, _Rest... pak) noexcept
	{
		return detail::comparer<_This, _Rest...>::min(val, pak...);
	}

	template <class _Class, class _Member>
	inline _Class* member_cast(_Member _Class::*offset, const _Member* child) noexcept
	{
		return (_Class*)(void*)((size_t)(child)-(size_t)(&((*(const _Class*)0).*offset)));
	}

	template <class _Base, class _Der>
	inline ptrdiff_t base_offset() noexcept
	{
		return (ptrdiff_t)(void*)static_cast<_Base*>(((_Der*)1)) - 1;
	}

}
