////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Video
//  File name:   VeWindow.inl
//  Created:     2016/07/10 by Albert
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
inline bool VeWindow::IsValid() const noexcept
{
	return m_kData.m_kNode.is_attach();
}
//--------------------------------------------------------------------------
inline uint32_t VeWindow::GetID() const noexcept
{
	return m_kData.m_u32Id;
}
//--------------------------------------------------------------------------
inline uint32_t VeWindow::GetFlags() const noexcept
{
	return m_kData.m_u32Flags;
}
//--------------------------------------------------------------------------
inline const char* VeWindow::GetTitle() const noexcept
{
	return m_kData.m_kTitle;
}
//--------------------------------------------------------------------------
inline int32_t VeWindow::GetPosX() const noexcept
{
	return m_kData.x;
}
//--------------------------------------------------------------------------
inline int32_t VeWindow::GetPosY() const noexcept
{
	return m_kData.y;
}
//--------------------------------------------------------------------------
inline int32_t VeWindow::GetWidth() const noexcept
{
	return m_kData.w;
}
//--------------------------------------------------------------------------
inline int32_t VeWindow::GetHeight() const noexcept
{
	return m_kData.h;
}
//--------------------------------------------------------------------------
inline int32_t VeWindow::GetMinimumWidth() const noexcept
{
	return m_kData.min_w;
}
//--------------------------------------------------------------------------
inline int32_t VeWindow::GetMinimumHeight() const noexcept
{
	return m_kData.min_h;
}
//--------------------------------------------------------------------------
inline int32_t VeWindow::GetMaximumWidth() const noexcept
{
	return m_kData.max_w;
}
//--------------------------------------------------------------------------
inline int32_t VeWindow::GetMaximumHeight() const noexcept
{
	return m_kData.max_h;
}
//--------------------------------------------------------------------------
inline float VeWindow::GetBrightness() const noexcept
{
	return m_kData.m_f32Brightness;
}
//--------------------------------------------------------------------------
