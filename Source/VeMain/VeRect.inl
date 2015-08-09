////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeRect.inl
//  Created:     2015/07/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline bool VeRect::IsEmpty() const noexcept
{
	return (w <= 0 || h <= 0) ? true : false;
}
//--------------------------------------------------------------------------
inline bool VeRect::IsEqual(const VeRect& kRect) const noexcept
{
	return (x == kRect.x && y == kRect.y && w == kRect.w && h == kRect.h)
		? true : false;
}
//--------------------------------------------------------------------------
