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
inline uint64_t VeRenderWindow::GetFrameIndex() noexcept
{
	return m_u64FrameIndex;
}
//--------------------------------------------------------------------------
inline bool VeRenderWindow::IsVisible() noexcept
{
	VE_ASSERT(IsValid());
	return m_spTargetWindow->IsVisible();
}
//--------------------------------------------------------------------------
inline bool VeRenderWindow::IsHidden() noexcept
{
	VE_ASSERT(IsValid());
	return m_spTargetWindow->IsHidden();
}
//--------------------------------------------------------------------------
inline uint32_t VeRenderWindow::GetWidth() noexcept
{
	VE_ASSERT(IsValid());
	return m_spTargetWindow->GetWidth();
}
//--------------------------------------------------------------------------
inline uint32_t VeRenderWindow::GetHeight() noexcept
{
	VE_ASSERT(IsValid());
	return m_spTargetWindow->GetHeight();
}
//--------------------------------------------------------------------------
void VeRenderWindow::Show() noexcept
{
	VE_ASSERT(IsValid());
	m_spTargetWindow->Show();
}
//--------------------------------------------------------------------------
void VeRenderWindow::Hide() noexcept
{
	VE_ASSERT(IsValid());
	m_spTargetWindow->Hide();
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetTitle(const char* pcTitle) noexcept
{
	VE_ASSERT(IsValid());
	m_spTargetWindow->SetTitle(pcTitle);
}
//--------------------------------------------------------------------------
