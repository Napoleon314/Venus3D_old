////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Event
//  File name:   VeMouse.inl
//  Created:     2016/07/20 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeWindowPtr VeMouse::GetFocus() noexcept
{
	return m_pkFocus ? VeWindow::Cast(m_pkFocus) : nullptr;
}
//--------------------------------------------------------------------------
inline uint32_t VeMouse::GetButtonState() noexcept
{
	return m_u32ButtonState;
}
//--------------------------------------------------------------------------
inline uint32_t VeMouse::GetState(int32_t& x, int32_t& y) noexcept
{
	x = m_i32PosX;
	y = m_i32PosY;
	return m_u32ButtonState;
}
//--------------------------------------------------------------------------
inline uint32_t VeMouse::GetRelativeState(
	int32_t& x, int32_t& y) noexcept
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
