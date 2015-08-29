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
inline VeUInt32 VeWindow::GetFlags() const noexcept
{
	return m_kData.m_u32Flags;
}
//--------------------------------------------------------------------------
inline const VeChar8* VeWindow::GetTitle() const noexcept
{
	return m_kData.m_kTitle;
}
//--------------------------------------------------------------------------
inline VeInt32 VeWindow::GetPosX() const noexcept
{
	return m_kData.x;
}
//--------------------------------------------------------------------------
inline VeInt32 VeWindow::GetPosY() const noexcept
{
	return m_kData.y;
}
//--------------------------------------------------------------------------
inline VeInt32 VeWindow::GetWidth() const noexcept
{
	return m_kData.w;
}
//--------------------------------------------------------------------------
inline VeInt32 VeWindow::GetHeight() const noexcept
{
	return m_kData.h;
}
//--------------------------------------------------------------------------
inline VeInt32 VeWindow::GetMinimumWidth() const noexcept
{
	return m_kData.min_w;
}
//--------------------------------------------------------------------------
inline VeInt32 VeWindow::GetMinimumHeight() const noexcept
{
	return m_kData.min_h;
}
//--------------------------------------------------------------------------
inline VeInt32 VeWindow::GetMaximumWidth() const noexcept
{
	return m_kData.max_w;
}
//--------------------------------------------------------------------------
inline VeInt32 VeWindow::GetMaximumHeight() const noexcept
{
	return m_kData.max_h;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeWindow::GetBrightness() const noexcept
{
	return m_kData.m_f32Brightness;
}
//--------------------------------------------------------------------------
