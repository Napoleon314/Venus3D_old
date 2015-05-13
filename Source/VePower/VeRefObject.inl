////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeRefObject.inl
//  Created:     2015/02/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeUInt32 VeRefObject::GetRefCount() const noexcept
{
	return m_u32RefCount;
}
//--------------------------------------------------------------------------
inline VeUInt32 VeRefObject::GetTotalObjectCount() noexcept
{
	return ms_u32Objects;
}
//--------------------------------------------------------------------------
