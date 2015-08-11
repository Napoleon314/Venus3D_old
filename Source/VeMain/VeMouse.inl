////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMouse.inl
//  Created:     2015/08/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeWindowPtr VeMouse::GetFocus() noexcept
{
	return m_pkFocus ? VeWindow::Cast(m_pkFocus) : nullptr;
}
//--------------------------------------------------------------------------
inline VeUInt32 VeMouse::GetButtonState() noexcept
{
	return m_u32ButtonState;
}
//--------------------------------------------------------------------------
inline VeUInt32 VeMouse::GetState(VeInt32& x, VeInt32& y) noexcept
{
	x = m_i32PosX;
	y = m_i32PosY;
	return m_u32ButtonState;
}
//--------------------------------------------------------------------------
inline VeUInt32 VeMouse::GetRelativeState(
	VeInt32& x, VeInt32& y) noexcept
{
	x = m_i32DeltaX;
	y = m_i32DeltaY;
	m_i32DeltaX = 0;
	m_i32DeltaY = 0;
	return m_u32ButtonState;
}
//--------------------------------------------------------------------------
inline bool VeMouse::IsRelativeModeEnable() noexcept
{
	return m_bRelativeMode ? true : false;
}
//--------------------------------------------------------------------------
inline VeCursorPtr VeMouse::GetCursor() noexcept
{
	return m_spCurCursor;
}
//--------------------------------------------------------------------------
inline VeCursorPtr VeMouse::GetDefaultCursor() noexcept
{
	return m_spDefCursor;
}
//--------------------------------------------------------------------------
