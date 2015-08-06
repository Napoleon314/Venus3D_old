////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeWindow.inl
//  Created:     2015/08/06 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline bool VeWindow::IsValid() const noexcept
{
	return m_kData.m_kNode.is_attach();
}
//--------------------------------------------------------------------------
inline VeUInt32 VeWindow::GetID() const noexcept
{
	return m_kData.m_u32Id;
}
//--------------------------------------------------------------------------
inline const VeChar8* VeWindow::GetTitle() const noexcept
{
	return m_kData.m_kTitle;
}
//--------------------------------------------------------------------------
