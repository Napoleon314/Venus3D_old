////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeRect.cpp
//  Created:     2015/07/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeRect::VeRect() noexcept
{

}
//--------------------------------------------------------------------------
VeRect::VeRect(VeInt32 _X, VeInt32 _Y, VeInt32 _W, VeInt32 _H) noexcept
	: x(_X), y(_Y), w(_W), h(_H)
{

}
//--------------------------------------------------------------------------
bool VeRect::HasIntersection(const VeRect& A, const VeRect& B) noexcept
{
	if (A.IsEmpty() || B.IsEmpty())
	{
		return false;
	}

	VeInt32 Amin, Amax, Bmin, Bmax;

	Amin = A.x;
	Amax = Amin + A.w;
	Bmin = B.x;
	Bmax = Bmin + B.w;
	if (Bmin > Amin)
		Amin = Bmin;
	if (Bmax < Amax)
		Amax = Bmax;
	if (Amax <= Amin)
		return false;

	Amin = A.y;
	Amax = Amin + A.h;
	Bmin = B.y;
	Bmax = Bmin + B.h;
	if (Bmin > Amin)
		Amin = Bmin;
	if (Bmax < Amax)
		Amax = Bmax;
	if (Amax <= Amin)
		return false;

	return true;
}
//--------------------------------------------------------------------------
bool VeRect::IntersectRect(VeRect& kResult, const VeRect& A,
	const VeRect& B) noexcept
{
	VeInt32 Amin, Amax, Bmin, Bmax;

	Amin = A.x;
	Amax = Amin + A.w;
	Bmin = B.x;
	Bmax = Bmin + B.w;
	if (Bmin > Amin) Amin = Bmin;
	kResult.x = Amin;
	if (Bmax < Amax) Amax = Bmax;
	kResult.w = Amax - Amin;

	Amin = A.y;
	Amax = Amin + A.h;
	Bmin = B.y;
	Bmax = Bmin + B.h;
	if (Bmin > Amin) Amin = Bmin;
	kResult.y = Amin;
	if (Bmax < Amax) Amax = Bmax;
	kResult.h = Amax - Amin;

	return !kResult.IsEmpty();
}
//--------------------------------------------------------------------------
void VeRect::UnionRect(VeRect& kResult, const VeRect& A,
	const VeRect& B) noexcept
{
	VeInt32 Amin, Amax, Bmin, Bmax;

	if (A.IsEmpty())
	{
		if (B.IsEmpty())
		{
			return;
		}
		else
		{
			kResult = B;
			return;
		}
	}
	else
	{
		if (B.IsEmpty())
		{
			kResult = A;
			return;
		}
	}

	Amin = A.x;
	Amax = Amin + A.w;
	Bmin = B.x;
	Bmax = Bmin + B.w;
	if (Bmin < Amin) Amin = Bmin;
	kResult.x = Amin;
	if (Bmax > Amax) Amax = Bmax;
	kResult.w = Amax - Amin;

	Amin = A.y;
	Amax = Amin + A.h;
	Bmin = B.y;
	Bmax = Bmin + B.h;
	if (Bmin < Amin) Amin = Bmin;
	kResult.y = Amin;
	if (Bmax > Amax) Amax = Bmax;
	kResult.h = Amax - Amin;
}
//--------------------------------------------------------------------------
bool VeRect::EnclosePoints(VeRect* pkResult, const VePoint* points,
	VeInt32 i32Count, const VeRect* pkClip) noexcept
{
	VE_ASSERT(points && i32Count >= 1);
	VeInt32 minx = 0;
	VeInt32 miny = 0;
	VeInt32 maxx = 0;
	VeInt32 maxy = 0;
	VeInt32 x, y, i;

	if (pkClip)
	{
		bool added = false;
		const int clip_minx = pkClip->x;
		const int clip_miny = pkClip->y;
		const int clip_maxx = pkClip->x + pkClip->w - 1;
		const int clip_maxy = pkClip->y + pkClip->h - 1;

		if (pkClip->IsEmpty())
		{
			return false;
		}

		for (i = 0; i < i32Count; ++i) {
			x = points[i].x;
			y = points[i].y;

			if (x < clip_minx || x > clip_maxx ||
				y < clip_miny || y > clip_maxy) {
				continue;
			}
			if (!added)
			{
				if (pkResult == nullptr)
				{
					return true;
				}

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
		if (!added)
		{
			return false;
		}
	}
	else
	{
		if (pkResult == nullptr)
		{
			return true;
		}

		minx = maxx = points[0].x;
		miny = maxy = points[0].y;

		for (i = 1; i < i32Count; ++i)
		{
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

	if (pkResult)
	{
		pkResult->x = minx;
		pkResult->y = miny;
		pkResult->w = (maxx - minx) + 1;
		pkResult->h = (maxy - miny) + 1;
	}
	return true;
}
//--------------------------------------------------------------------------
#define CODE_BOTTOM 1
#define CODE_TOP    2
#define CODE_LEFT   4
#define CODE_RIGHT  8
//--------------------------------------------------------------------------
static VeInt32 ComputeOutCode(const VeRect& kRect, VeInt32 x,
	VeInt32 y) noexcept
{
	int code = 0;
	if (y < kRect.y)
	{
		code |= CODE_TOP;
	}
	else if (y >= kRect.y + kRect.h)
	{
		code |= CODE_BOTTOM;
	}
	if (x < kRect.x)
	{
		code |= CODE_LEFT;
	}
	else if (x >= kRect.x + kRect.w)
	{
		code |= CODE_RIGHT;
	}
	return code;
}
//--------------------------------------------------------------------------
bool VeRect::IntersectRectAndLine(const VeRect& kRect, VeInt32& X1,
	VeInt32& Y1, VeInt32& X2, VeInt32& Y2) noexcept
{
	VeInt32 x = 0;
	VeInt32 y = 0;
	VeInt32 x1, y1;
	VeInt32 x2, y2;
	VeInt32 rectx1;
	VeInt32 recty1;
	VeInt32 rectx2;
	VeInt32 recty2;
	VeInt32 outcode1, outcode2;

	if (kRect.IsEmpty())
	{
		return false;
	}

	x1 = X1;
	y1 = Y1;
	x2 = X2;
	y2 = Y2;
	rectx1 = kRect.x;
	recty1 = kRect.y;
	rectx2 = kRect.x + kRect.w - 1;
	recty2 = kRect.y + kRect.h - 1;

	if (x1 >= rectx1 && x1 <= rectx2 && x2 >= rectx1 && x2 <= rectx2 &&
		y1 >= recty1 && y1 <= recty2 && y2 >= recty1 && y2 <= recty2)
	{
		return true;
	}

	if ((x1 < rectx1 && x2 < rectx1) || (x1 > rectx2 && x2 > rectx2) ||
		(y1 < recty1 && y2 < recty1) || (y1 > recty2 && y2 > recty2))
	{
		return false;
	}

	if (y1 == y2)
	{
		if (x1 < rectx1)
		{
			X1 = rectx1;
		}
		else if (x1 > rectx2)
		{
			X1 = rectx2;
		}
		if (x2 < rectx1)
		{
			X2 = rectx1;
		}
		else if (x2 > rectx2)
		{
			X2 = rectx2;
		}
		return true;
	}

	if (x1 == x2)
	{
		if (y1 < recty1)
		{
			Y1 = recty1;
		}
		else if (y1 > recty2)
		{
			Y1 = recty2;
		}
		if (y2 < recty1)
		{
			Y2 = recty1;
		}
		else if (y2 > recty2)
		{
			Y2 = recty2;
		}
		return true;
	}

	outcode1 = ComputeOutCode(kRect, x1, y1);
	outcode2 = ComputeOutCode(kRect, x2, y2);
	while (outcode1 || outcode2)
	{
		if (outcode1 & outcode2)
		{
			return false;
		}

		if (outcode1)
		{
			if (outcode1 & CODE_TOP)
			{
				y = recty1;
				x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
			}
			else if (outcode1 & CODE_BOTTOM)
			{
				y = recty2;
				x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
			}
			else if (outcode1 & CODE_LEFT)
			{
				x = rectx1;
				y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
			}
			else if (outcode1 & CODE_RIGHT)
			{
				x = rectx2;
				y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
			}
			x1 = x;
			y1 = y;
			outcode1 = ComputeOutCode(kRect, x, y);
		}
		else {
			if (outcode2 & CODE_TOP)
			{
				y = recty1;
				x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
			}
			else if (outcode2 & CODE_BOTTOM)
			{
				y = recty2;
				x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
			}
			else if (outcode2 & CODE_LEFT)
			{
				x = rectx1;
				y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
			}
			else if (outcode2 & CODE_RIGHT)
			{
				x = rectx2;
				y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
			}
			x2 = x;
			y2 = y;
			outcode2 = ComputeOutCode(kRect, x, y);
		}
	}
	X1 = x1;
	Y1 = y1;
	X2 = x2;
	Y2 = y2;
	return true;
}
//--------------------------------------------------------------------------
bool VeRect::GetSpanEnclosingRect(VeInt32 width, VeInt32 height,
	VeInt32 numrects, const VeRect* rects, VeRect* span) noexcept
{
	VE_ASSERT(width >= 1 && height >= 1 && rects && span && numrects >= 1);
	VeInt32 i;
	VeInt32 span_y1, span_y2;
	VeInt32 rect_y1, rect_y2;

	span_y1 = height;
	span_y2 = 0;

	for (i = 0; i < numrects; ++i)
	{
		rect_y1 = rects[i].y;
		rect_y2 = rect_y1 + rects[i].h;

		if (rect_y1 < 0)
		{
			span_y1 = 0;
		}
		else if (rect_y1 < span_y1)
		{
			span_y1 = rect_y1;
		}
		if (rect_y2 > height)
		{
			span_y2 = height;
		}
		else if (rect_y2 > span_y2)
		{
			span_y2 = rect_y2;
		}
	}
	if (span_y2 > span_y1)
	{
		span->x = 0;
		span->y = span_y1;
		span->w = width;
		span->h = (span_y2 - span_y1);
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------
