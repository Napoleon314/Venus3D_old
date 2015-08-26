////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeResource.inl
//  Created:     2015/08/23 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline const VeChar8* VeResource::GetName() const noexcept
{
	return m_kName;
}
//--------------------------------------------------------------------------
inline const VeChar8* VeResource::GetFullName() const noexcept
{
	return m_kFullName;
}
//--------------------------------------------------------------------------
inline const VeChar8* VeResource::GetGroupName() const noexcept
{
	return m_spGroup ? m_spGroup->GetName() : "";
}
//--------------------------------------------------------------------------
inline VeResource::State VeResource::GetState() const noexcept
{
	return m_eState;
}
//--------------------------------------------------------------------------
