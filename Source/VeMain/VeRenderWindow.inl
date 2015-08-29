////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderWindow.inl
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline const VeWindowPtr& VeRenderWindow::GetTargetWindow() noexcept
{
	return m_spTargetWindow;
}
//--------------------------------------------------------------------------
inline VeUInt32 VeRenderWindow::GetID() const noexcept
{
	return m_spTargetWindow->GetID();
}
//--------------------------------------------------------------------------
inline VeUInt32 VeRenderWindow::GetFlags() const noexcept
{
	return m_spTargetWindow->GetFlags();
}
//--------------------------------------------------------------------------
inline const VeChar8* VeRenderWindow::GetTitle() const noexcept
{
	return m_spTargetWindow->GetTitle();
}
//--------------------------------------------------------------------------
inline VeInt32 VeRenderWindow::GetPosX() const noexcept
{
	return m_spTargetWindow->GetPosX();
}
//--------------------------------------------------------------------------
inline VeInt32 VeRenderWindow::GetPosY() const noexcept
{
	return m_spTargetWindow->GetPosY();
}
//--------------------------------------------------------------------------
inline VeInt32 VeRenderWindow::GetWidth() const noexcept
{
	return m_spTargetWindow->GetWidth();
}
//--------------------------------------------------------------------------
inline VeInt32 VeRenderWindow::GetHeight() const noexcept
{
	return m_spTargetWindow->GetHeight();
}
//--------------------------------------------------------------------------
inline VeInt32 VeRenderWindow::GetMinimumWidth() const noexcept
{
	return m_spTargetWindow->GetMinimumWidth();
}
//--------------------------------------------------------------------------
inline VeInt32 VeRenderWindow::GetMinimumHeight() const noexcept
{
	return m_spTargetWindow->GetMinimumHeight();
}
//--------------------------------------------------------------------------
inline VeInt32 VeRenderWindow::GetMaximumWidth() const noexcept
{
	return m_spTargetWindow->GetMaximumWidth();
}
//--------------------------------------------------------------------------
inline VeInt32 VeRenderWindow::GetMaximumHeight() const noexcept
{
	return m_spTargetWindow->GetMaximumHeight();
}
//--------------------------------------------------------------------------
inline VeFloat32 VeRenderWindow::GetBrightness() const noexcept
{
	return m_spTargetWindow->GetBrightness();
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetTitle(const VeChar8* pcTitle) noexcept
{
	return m_spTargetWindow->SetTitle(pcTitle);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetPosition(VeInt32 x, VeInt32 y) noexcept
{
	m_spTargetWindow->SetPosition(x, y);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetSize(VeInt32 w, VeInt32 h) noexcept
{
	m_spTargetWindow->SetSize(w, h);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetMinimumSize(
	VeInt32 min_w, VeInt32 min_h) noexcept
{
	m_spTargetWindow->SetMinimumSize(min_w, min_h);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetMaximumSize(
	VeInt32 max_w, VeInt32 max_h) noexcept
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
inline void VeRenderWindow::SetFullscreen(VeUInt32 u32Flags) noexcept
{
	m_spTargetWindow->SetFullscreen(u32Flags);
}
//--------------------------------------------------------------------------
inline bool VeRenderWindow::SetBrightness(VeFloat32 f32Brightness) noexcept
{
	return m_spTargetWindow->SetBrightness(f32Brightness);
}
//--------------------------------------------------------------------------
inline void VeRenderWindow::SetGrab(bool bGrabbed) noexcept
{
	m_spTargetWindow->SetGrab(bGrabbed);
}
//--------------------------------------------------------------------------
