////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      vtd
//  File name:   point.h
//  Created:     2016/07/06 by Albert
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
	template<class _Ty>
	struct point
	{
		static_assert(std::is_arithmetic<_Ty>::value, "_Ty has to be an arithmetic type.");

		point() noexcept = default;

		point(const point&) noexcept = default;

		point(point&&) noexcept = default;

		point& operator = (const point&) noexcept = default;

		~point() noexcept = default;

		point(std::initializer_list<_Ty> l) noexcept
		{
			for (size_t i(0); i < min(l.size(), 2); ++i)
			{
				(&x)[i] = l.begin()[i];
			}
		}

		point(_Ty _1, _Ty _2) noexcept : x(_1), y(_2) {}

		point& operator += (const point& p) noexcept
		{
			x += p.x;
			y += p.y;
			return *this;
		}

		point& operator -= (const point& p) noexcept
		{
			x -= p.x;
			y -= p.y;
			return *this;
		}

		point& operator *= (_Ty s) noexcept
		{
			x *= s;
			y *= s;
			return *this;
		}

		point& operator /= (_Ty s) noexcept
		{
			x /= s;
			y /= s;
			return *this;
		}

		point operator + () const noexcept
		{
			return *this;
		}

		point operator - () const noexcept
		{
			return point(-x, -y);
		}

		point operator + (const point& p) const noexcept
		{
			return point(x + p.x, y + p.y);
		}

		point operator - (const point& p) const noexcept
		{
			return point(x - p.x, y - p.y);
		}

		point operator * (_Ty s) const noexcept
		{
			return point(x * s, y * s);
		}

		point operator / (_Ty s) const noexcept
		{
			return point(x / s, y / s);
		}

		bool operator == (const point& p) const noexcept
		{
			return x == p.x && y == p.y;
		}

		bool operator != (const point& p) const noexcept
		{
			return x != p.x || y != p.y;
		}

		_Ty x = 0, y = 0;

	};

	template<class _Ty>
	point<_Ty> operator * (_Ty s, const point<_Ty>& p) noexcept
	{
		return p * s;
	}

}
