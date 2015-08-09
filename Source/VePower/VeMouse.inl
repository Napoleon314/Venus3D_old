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
