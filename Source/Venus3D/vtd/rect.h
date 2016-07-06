////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      vtd
//  File name:   rect.h
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

#include <assert.h>
#include "point.h"

namespace vtd
{
	template<class _Ty>
	struct alignas(sizeof(_Ty) ) rect
	{
		rect() noexcept = default;

		rect(const rect&) noexcept = default;

		rect(rect&&) noexcept = default;

		rect& operator = (const rect&) noexcept = default;

		~rect() noexcept = default;

		rect(std::initializer_list<_Ty> l) noexcept
		{
			for (size_t i(0); i < min(l.size(), 4); ++i)
			{
				(&x)[i] = l.begin()[i];
			}
		}

		rect(_Ty _x, _Ty _y, _Ty _w, _Ty _h) noexcept
			: x(_x), y(_y), w(_w), h(_h) {}

		bool operator == (const rect& r) const noexcept
		{
			return ((x == r.x) && (y == r.y) && (w == r.w) && (h == r.h)) ?
				true : false;
		}

		bool operator != (const rect& r) const noexcept
		{
			return ((x != r.x) || (y != r.y) || (w != r.w) || (h != r.h)) ?
				true : false;
		}

		bool empty() const noexcept
		{
			return ((w <= 0) || (h <= 0)) ? true : false;
		}

		bool inside(const point<_Ty>& p) const noexcept
		{
			return ((p->x >= x) && (p->x < (x + w)) &&
				(p->y >= y) && (p->y < (y + h))) ? true : false;
		}
		
		bool intersects(const rect& r) const noexcept
		{
			/* Special cases for empty rects */
			if (empty() || r.empty()) {
				return false;
			}

			_Ty Amin, Amax, Bmin, Bmax;

			/* Horizontal intersection */
			Amin = x;
			Amax = Amin + w;
			Bmin = r.x;
			Bmax = Bmin + r.w;
			if (Bmin > Amin)
				Amin = Bmin;
			if (Bmax < Amax)
				Amax = Bmax;
			if (Amax <= Amin)
				return false;

			/* Vertical intersection */
			Amin = y;
			Amax = Amin + h;
			Bmin = r.y;
			Bmax = Bmin + r.h;
			if (Bmin > Amin)
				Amin = Bmin;
			if (Bmax < Amax)
				Amax = Bmax;
			if (Amax <= Amin)
				return false;

			return true;
		}

		bool intersects(const rect& r1, const rect& r2) const noexcept
		{
			/* Special cases for empty rects */
			if (r1.empty() || r2.empty()) {
				x = y = w = h = 0;
				return false;
			}

			_Ty Amin, Amax, Bmin, Bmax;

			/* Horizontal intersection */
			Amin = r1.x;
			Amax = Amin + r1.w;
			Bmin = r2.x;
			Bmax = Bmin + r2.w;
			if (Bmin > Amin)
				Amin = Bmin;
			x = Amin;
			if (Bmax < Amax)
				Amax = Bmax;
			w = Amax - Amin;

			/* Vertical intersection */
			Amin = r1.y;
			Amax = Amin + r1.h;
			Bmin = r2.y;
			Bmax = Bmin + r2.h;
			if (Bmin > Amin)
				Amin = Bmin;
			y = Amin;
			if (Bmax < Amax)
				Amax = Bmax;
			h = Amax - Amin;

			return !empty();
		}

		void union_rect(const rect& r1, const rect& r2) noexcept
		{
			/* Special cases for empty Rects */
			if (r1.empty()) {
				if (r2.empty()) {
					/* A and B empty */
					return;
				}
				else {
					/* A empty, B not empty */
					*this = r2;
					return;
				}
			}
			else {
				if (r2.empty()) {
					/* A not empty, B empty */
					*this = r1;
					return;
				}
			}

			_Ty Amin, Amax, Bmin, Bmax;

			/* Horizontal union */
			Amin = r1.x;
			Amax = Amin + r1.w;
			Bmin = r2.x;
			Bmax = Bmin + r2.w;
			if (Bmin < Amin)
				Amin = Bmin;
			x = Amin;
			if (Bmax > Amax)
				Amax = Bmax;
			w = Amax - Amin;

			/* Vertical union */
			Amin = r1.y;
			Amax = Amin + r1.h;
			Bmin = r2.y;
			Bmax = Bmin + r2.h;
			if (Bmin < Amin)
				Amin = Bmin;
			y = Amin;
			if (Bmax > Amax)
				Amax = Bmax;
			h = Amax - Amin;
		}

		_Ty x = 0, y = 0, w = 0, h = 0;

	};

	template <class _Ty>
	bool enclose_points(const point<_Ty> * points, size_t count,
		const rect<_Ty> * clip, rect<_Ty> * result) noexcept
	{
		_Ty minx = 0;
		_Ty miny = 0;
		_Ty maxx = 0;
		_Ty maxy = 0;
		_Ty x, y, i;

		if (!points) {
			return false;
		}

		if (count < 1) {
			return false;
		}

		if (clip) {
			bool added = false;
			const int clip_minx = clip->x;
			const int clip_miny = clip->y;
			const int clip_maxx = clip->x + clip->w - 1;
			const int clip_maxy = clip->y + clip->h - 1;

			/* Special case for empty rectangle */
			if (clip->empty()) {
				return false;
			}

			for (i = 0; i < count; ++i) {
				x = points[i].x;
				y = points[i].y;

				if (x < clip_minx || x > clip_maxx ||
					y < clip_miny || y > clip_maxy) {
					continue;
				}
				if (!added) {
					/* Special case: if no result was requested, we are done */
					if (result == nullptr) {
						return true;
					}

					/* First point added */
					minx = maxx = x;
					miny = maxy = y;
					added = true;
					continue;
				}
				if (x < minx) {
					minx = x;
				}
				else if (x > maxx) {
					maxx = x;
				}
				if (y < miny) {
					miny = y;
				}
				else if (y > maxy) {
					maxy = y;
				}
			}
			if (!added) {
				return false;
			}
		}
		else {
			/* Special case: if no result was requested, we are done */
			if (result == nullptr) {
				return true;
			}

			/* No clipping, always add the first point */
			minx = maxx = points[0].x;
			miny = maxy = points[0].y;

			for (i = 1; i < count; ++i) {
				x = points[i].x;
				y = points[i].y;

				if (x < minx) {
					minx = x;
				}
				else if (x > maxx) {
					maxx = x;
				}
				if (y < miny) {
					miny = y;
				}
				else if (y > maxy) {
					maxy = y;
				}
			}
		}

		if (result) {
			result->x = minx;
			result->y = miny;
			result->w = (maxx - minx) + 1;
			result->h = (maxy - miny) + 1;
		}
		return true;
	}

}
