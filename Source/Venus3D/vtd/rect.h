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

#include "point.h"

namespace vtd
{
	template<class _Ty>
	struct alignas(sizeof(_Ty) ) rect
	{
		enum outcode
		{
			CODE_BOTTOM = 1,
			CODE_TOP = 2,
			CODE_LEFT = 4,
			CODE_RIGHT = 8
		};


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

		uint32_t compute_outcode(_Ty _x, _Ty _y) const noexcept
		{
			uint32_t code = 0;
			if (_y < y) {
				code |= CODE_TOP;
			}
			else if (_y >= y + h) {
				code |= CODE_BOTTOM;
			}
			if (_x < x) {
				code |= CODE_LEFT;
			}
			else if (_x >= x + w) {
				code |= CODE_RIGHT;
			}
			return code;
		}

		uint32_t compute_outcode(const point<_Ty>& p) const noexcept
		{
			return compute_outcode(p.x, p.y);
		}

		bool intersects(point<_Ty>& p1, point<_Ty>& p2) const noexcept
		{
			_Ty x = 0;
			_Ty y = 0;
			_Ty x1, y1;
			_Ty x2, y2;
			_Ty rectx1;
			_Ty recty1;
			_Ty rectx2;
			_Ty recty2;
			_Ty outcode1, outcode2;

			/* Special case for empty rect */
			if (empty()) {
				return false;
			}

			x1 = p1.x;
			y1 = p1.y;
			x2 = p2.x;
			y2 = p2.y;
			rectx1 = x;
			recty1 = y;
			rectx2 = x + w - 1;
			recty2 = y + h - 1;

			/* Check to see if entire line is inside rect */
			if (x1 >= rectx1 && x1 <= rectx2 && x2 >= rectx1 && x2 <= rectx2 &&
				y1 >= recty1 && y1 <= recty2 && y2 >= recty1 && y2 <= recty2) {
				return true;
			}

			/* Check to see if entire line is to one side of rect */
			if ((x1 < rectx1 && x2 < rectx1) || (x1 > rectx2 && x2 > rectx2) ||
				(y1 < recty1 && y2 < recty1) || (y1 > recty2 && y2 > recty2)) {
				return false;
			}

			if (y1 == y2) {
				/* Horizontal line, easy to clip */
				if (x1 < rectx1) {
					p1.x = rectx1;
				}
				else if (x1 > rectx2) {
					p1.x = rectx2;
				}
				if (x2 < rectx1) {
					p2.x = rectx1;
				}
				else if (x2 > rectx2) {
					p2.x = rectx2;
				}
				return true;
			}

			if (x1 == x2) {
				/* Vertical line, easy to clip */
				if (y1 < recty1) {
					p1.y = recty1;
				}
				else if (y1 > recty2) {
					p1.y = recty2;
				}
				if (y2 < recty1) {
					p2.y = recty1;
				}
				else if (y2 > recty2) {
					p2.y = recty2;
				}
				return true;
			}

			/* More complicated Cohen-Sutherland algorithm */
			outcode1 = compute_outcode(x1, y1);
			outcode2 = compute_outcode(x2, y2);
			while (outcode1 || outcode2) {
				if (outcode1 & outcode2) {
					return false;
				}

				if (outcode1) {
					if (outcode1 & CODE_TOP) {
						y = recty1;
						x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
					}
					else if (outcode1 & CODE_BOTTOM) {
						y = recty2;
						x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
					}
					else if (outcode1 & CODE_LEFT) {
						x = rectx1;
						y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
					}
					else if (outcode1 & CODE_RIGHT) {
						x = rectx2;
						y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
					}
					x1 = x;
					y1 = y;
					outcode1 = compute_outcode(x, y);
				}
				else {
					if (outcode2 & CODE_TOP) {
						y = recty1;
						x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
					}
					else if (outcode2 & CODE_BOTTOM) {
						y = recty2;
						x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
					}
					else if (outcode2 & CODE_LEFT) {
						x = rectx1;
						y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
					}
					else if (outcode2 & CODE_RIGHT) {
						x = rectx2;
						y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
					}
					x2 = x;
					y2 = y;
					outcode2 = compute_outcode(x, y);
				}
			}
			p1.x = x1;
			p1.y = y1;
			p2.x = x2;
			p2.y = y2;
			return true;
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

	template <class _Ty>
	bool get_span_enclosing_rect(_Ty width, _Ty height,
		size_t numrects, const rect<_Ty> * rects, const rect<_Ty> *span) noexcept
	{
		size_t i;
		_Ty span_y1, span_y2;
		_Ty rect_y1, rect_y2;

		if (width < 1) {
			return false;
		}

		if (height < 1) {
			return false;
		}

		if (!rects) {
			return false;
		}

		if (!span) {
			return false;
		}

		if (numrects < 1) {
			return false;
		}

		/* Initialize to empty rect */
		span_y1 = height;
		span_y2 = 0;

		for (i = 0; i < numrects; ++i) {
			rect_y1 = rects[i].y;
			rect_y2 = rect_y1 + rects[i].h;

			/* Clip out of bounds rectangles, and expand span rect */
			if (rect_y1 < 0) {
				span_y1 = 0;
			}
			else if (rect_y1 < span_y1) {
				span_y1 = rect_y1;
			}
			if (rect_y2 > height) {
				span_y2 = height;
			}
			else if (rect_y2 > span_y2) {
				span_y2 = rect_y2;
			}
		}
		if (span_y2 > span_y1) {
			span->x = 0;
			span->y = span_y1;
			span->w = width;
			span->h = (span_y2 - span_y1);
			return true;
		}
		return false;
	}

}
