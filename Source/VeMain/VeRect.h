////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeRect.h
//  Created:     2015/07/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

struct VePoint
{
	VeInt32 x = 0, y = 0;
};

class VE_MAIN_API VeRect : public VeMemA16Object
{
public:
	VeInt32 x = 0, y = 0, w = 0, h = 0;

	VeRect() noexcept;

	VeRect(VeInt32 _X, VeInt32 _Y, VeInt32 _W, VeInt32 _H) noexcept;

	inline bool IsEmpty() const noexcept;

	inline bool IsEqual(const VeRect& kRect) const noexcept;

	static bool HasIntersection(const VeRect& A,
		const VeRect& B) noexcept;

	static bool IntersectRect(VeRect& kResult, const VeRect& A,
		const VeRect& B) noexcept;

	static void UnionRect(VeRect& kResult, const VeRect& A,
		const VeRect& B) noexcept;

	static bool EnclosePoints(VeRect* pkResult, const VePoint* pkPoints,
		VeInt32 i32Count, const VeRect* pkClip) noexcept;

	static bool IntersectRectAndLine(const VeRect& kRect, VeInt32& X1,
		VeInt32& Y1, VeInt32& X2, VeInt32& Y2) noexcept;

	static bool GetSpanEnclosingRect(VeInt32 width, VeInt32 height,
		VeInt32 numrects, const VeRect* rects, VeRect* span) noexcept;

};

#include "VeRect.inl"
