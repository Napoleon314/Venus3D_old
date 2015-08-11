////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeWindow.cpp
//  Created:     2015/07/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeWindow::VeWindow() noexcept
{
	m_kData.m_kNode.m_Content = &m_kData;
}
//--------------------------------------------------------------------------
VeWindow::~VeWindow() noexcept
{
	VE_ASSERT(!IsValid());
}
//--------------------------------------------------------------------------
bool VeWindow::Create(const VeChar8* pcTitle, VeInt32 x, VeInt32 y,
	VeInt32 w, VeInt32 h, VeUInt32 u32Flags) noexcept
{
	if (!ve_video_ptr) return false;
	if (IsValid()) return false;
	return ve_video_ptr->CreateWindowBy(&m_kData, pcTitle, x, y, w, h, u32Flags);
}
//--------------------------------------------------------------------------
bool VeWindow::Create(const void* pvData) noexcept
{
	if (!ve_video_ptr) return false;
	if (IsValid()) return false;
	return ve_video_ptr->CreateWindowFrom(&m_kData, pvData);
}
//--------------------------------------------------------------------------
void VeWindow::SetTitle(const VeChar8* pcTitle) noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->SetWindowTitle(&m_kData, pcTitle);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetPosition(VeInt32 x, VeInt32 y) noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->SetWindowPosition(&m_kData, x, y);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetSize(VeInt32 w, VeInt32 h) noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->SetWindowSize(&m_kData, w, h);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetMinimumSize(VeInt32 min_w, VeInt32 min_h) noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->SetWindowMinimumSize(&m_kData, min_w, min_h);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetMaximumSize(VeInt32 max_w, VeInt32 max_h) noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->SetWindowMaximumSize(&m_kData, max_w, max_h);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Show() noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->ShowWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Hide() noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->HideWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Raise() noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->RaiseWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Maximize() noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->MaximizeWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Minimize() noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->MinimizeWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Restore() noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->RestoreWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetBordered(bool bBordered) noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->SetWindowBordered(&m_kData, bBordered ? VE_TRUE : VE_FALSE);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetFullscreen(VeUInt32 u32Flags) noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->SetWindowFullscreen(&m_kData, u32Flags);
	}
}
//--------------------------------------------------------------------------
bool VeWindow::SetBrightness(VeFloat32 f32Brightness) noexcept
{
	if (ve_video_ptr)
	{
		return ve_video_ptr->SetWindowBrightness(&m_kData, f32Brightness);
	}
	return false;
}
//--------------------------------------------------------------------------
void VeWindow::SetGrab(bool bGrabbed) noexcept
{
	if (ve_video_ptr)
	{
		ve_video_ptr->SetWindowGrab(&m_kData, bGrabbed ? VE_TRUE : VE_FALSE);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Destory() noexcept
{
	if (ve_video_ptr && IsValid())
	{
		ve_video_ptr->DestroyWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
VeSysWMInfo VeWindow::GetWMInfo() noexcept
{
	VeSysWMInfo kInfo;
	if (ve_video_ptr && IsValid())
	{
		ve_video_ptr->GetWindowWMInfo(&m_kData, &kInfo);
	}
	return kInfo;
}
//--------------------------------------------------------------------------
VeWindow* VeWindow::Cast(Data* pkData) noexcept
{
	return pkData ? VeMemberCast(&VeWindow::m_kData, pkData) : nullptr;
}
//--------------------------------------------------------------------------
