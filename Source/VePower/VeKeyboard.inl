////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeKeyboard.inl
//  Created:     2015/08/07 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeWindowPtr VeKeyboard::GetFocus() noexcept
{
	if (m_pkFocus)
	{
		return VeWindow::Cast(m_pkFocus);
	}
	return nullptr;
}
//--------------------------------------------------------------------------
inline const VeUInt8* VeKeyboard::GetState() noexcept
{
	return m_au8KeyState;
}
//--------------------------------------------------------------------------
inline VeUInt16 VeKeyboard::GetModState() noexcept
{
	return m_u16ModState;
}
//--------------------------------------------------------------------------
