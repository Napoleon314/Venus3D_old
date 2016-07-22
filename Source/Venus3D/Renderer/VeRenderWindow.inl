////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      VeRenderer
//  File name:   VeRenderWindow.inl
//  Created:     2016/07/22 by Albert
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
inline const VeWindowPtr& VeRenderWindow::GetTargetWindow() noexcept
{
	return m_spTargetWindow;
}
//--------------------------------------------------------------------------
inline uint32_t VeRenderWindow::GetID() const noexcept
{
	return m_spTargetWindow->GetID();
}
//--------------------------------------------------------------------------
inline uint32_t VeRenderWindow::GetFlags() const noexcept
{
	return m_spTargetWindow->GetFlags();
}
//--------------------------------------------------------------------------
inline const char* VeRenderWindow::GetTitle() const noexcept
{
	return m_spTargetWindow->GetTitle();
}
//--------------------------------------------------------------------------
inline int32_t VeRenderWindow::GetPosX() const noexcept
{
	return m_spTargetWindow->GetPosX();
}
//--------------------------------------------------------------------------
inline int32_t VeRenderWindow::GetPosY() const noexcept
{
	return m_spTargetWindow->GetPosY();
}
//--------------------------------------------------------------------------
inline int32_t VeRenderWindow::GetWidth() const noexcept
{
	return m_spTargetWindow->GetWidth();
}
//--------------------------------------------------------------------------
inline int32_t VeRenderWindow::GetHeight() const noexcept
{
	return m_spTargetWindow->GetHeight();
}
//--------------------------------------------------------------------------
inline int32_t VeRenderWindow::GetMinimumWidth() const noexcept
{
	return m_spTargetWindow->GetMinimumWidth();
}
//--------------------------------------------------------------------------
inline int32_t VeRenderWindow::GetMinimumHeight() const noexcept
{
	return m_spTargetWindow->GetMinimumHeight();
}
//--------------------------------------------------------------------------
inline int32_t VeRenderWindow::GetMaximumWidth() const noexcept
{
	return m_spTargetWindow->GetMaximumWidth();
}
//--------------------------------------------------------------------------
inline int32_t VeRenderWindow::GetMaximumHeight() const noexcept
{
	return m_spTargetWindow->GetMaximumHeight();
}
//--------------------------------------------------------------------------
inline float VeRenderWindow::GetBrightness() const noexcept
{
	return m_spTargetWindow->GetBrightness();
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetTitle(const char* pcTitle) noexcept
{
	return m_spTargetWindow->SetTitle(pcTitle);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetPosition(int32_t x, int32_t y) noexcept
{
	m_spTargetWindow->SetPosition(x, y);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetSize(int32_t w, int32_t h) noexcept
{
	m_spTargetWindow->SetSize(w, h);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetMinimumSize(
	int32_t min_w, int32_t min_h) noexcept
{
	m_spTargetWindow->SetMinimumSize(min_w, min_h);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetMaximumSize(
	int32_t max_w, int32_t max_h) noexcept
{
	m_spTargetWindow->SetMaximumSize(max_w, max_h);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::Show() noexcept
{
	m_spTargetWindow->Show();
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::Hide() noexcept
{
	m_spTargetWindow->Hide();
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::Raise() noexcept
{
	m_spTargetWindow->Raise();
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::Maximize() noexcept
{
	m_spTargetWindow->Maximize();
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::Minimize() noexcept
{
	m_spTargetWindow->Minimize();
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::Restore() noexcept
{
	m_spTargetWindow->Restore();
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetBordered(bool bBordered) noexcept
{
	m_spTargetWindow->SetBordered(bBordered);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetFullscreen(uint32_t u32Flags) noexcept
{
	m_spTargetWindow->SetFullscreen(u32Flags);
}
//--------------------------------------------------------------------------
inline bool VeRenderWindow::SetBrightness(float f32Brightness) noexcept
{
	return m_spTargetWindow->SetBrightness(f32Brightness);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetGrab(bool bGrabbed) noexcept
{
	m_spTargetWindow->SetGrab(bGrabbed);
}
//--------------------------------------------------------------------------
inline bool VeRenderWindow::IsSync() noexcept
{
	return m_bSync;
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetSync(bool bEnable) noexcept
{
	m_bSync = bEnable;
}
//--------------------------------------------------------------------------
