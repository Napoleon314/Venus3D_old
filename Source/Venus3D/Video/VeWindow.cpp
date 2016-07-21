////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Video
//  File name:   VeWindow.cpp
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

#include "stdafx.h"

//--------------------------------------------------------------------------
VeWindow::VeWindow() noexcept
{
	m_kData.m_kNode._Content = &m_kData;
}
//--------------------------------------------------------------------------
VeWindow::~VeWindow() noexcept
{
	assert(!IsValid());
}
//--------------------------------------------------------------------------
bool VeWindow::Create(const char* pcTitle, int32_t x, int32_t y,
	int32_t w, int32_t h, uint32_t u32Flags) noexcept
{
	if (!Venus3D::Ref().GetVideoDevice()) return false;
	if (IsValid()) return false;
	return Venus3D::Ref().GetVideoDevice()->CreateWindowBy(&m_kData, pcTitle, x, y, w, h, u32Flags);
}
//--------------------------------------------------------------------------
bool VeWindow::Create(const void* pvData) noexcept
{
	if (!Venus3D::Ref().GetVideoDevice()) return false;
	if (IsValid()) return false;
	return Venus3D::Ref().GetVideoDevice()->CreateWindowFrom(&m_kData, pvData);
}
//--------------------------------------------------------------------------
void VeWindow::SetTitle(const char* pcTitle) noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->SetWindowTitle(&m_kData, pcTitle);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetPosition(int32_t x, int32_t y) noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->SetWindowPosition(&m_kData, x, y);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetSize(int32_t w, int32_t h) noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->SetWindowSize(&m_kData, w, h);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetMinimumSize(int32_t min_w, int32_t min_h) noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->SetWindowMinimumSize(&m_kData, min_w, min_h);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetMaximumSize(int32_t max_w, int32_t max_h) noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->SetWindowMaximumSize(&m_kData, max_w, max_h);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Show() noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->ShowWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Hide() noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->HideWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Raise() noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->RaiseWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Maximize() noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->MaximizeWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Minimize() noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->MinimizeWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Restore() noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->RestoreWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetBordered(bool bBordered) noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->SetWindowBordered(&m_kData, bBordered ? VE_TRUE : VE_FALSE);
	}
}
//--------------------------------------------------------------------------
void VeWindow::SetFullscreen(uint32_t u32Flags) noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->SetWindowFullscreen(&m_kData, u32Flags);
	}
}
//--------------------------------------------------------------------------
bool VeWindow::SetBrightness(float f32Brightness) noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		return Venus3D::Ref().GetVideoDevice()->SetWindowBrightness(&m_kData, f32Brightness);
	}
	return false;
}
//--------------------------------------------------------------------------
void VeWindow::SetGrab(bool bGrabbed) noexcept
{
	if (Venus3D::Ref().GetVideoDevice())
	{
		Venus3D::Ref().GetVideoDevice()->SetWindowGrab(&m_kData, bGrabbed ? VE_TRUE : VE_FALSE);
	}
}
//--------------------------------------------------------------------------
void VeWindow::Destory() noexcept
{
	if (Venus3D::Ref().GetVideoDevice() && IsValid())
	{
		Venus3D::Ref().GetVideoDevice()->DestroyWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
VeSysWMInfo VeWindow::GetWMInfo() noexcept
{
	VeSysWMInfo kInfo;
	if (Venus3D::Ref().GetVideoDevice() && IsValid())
	{
		Venus3D::Ref().GetVideoDevice()->GetWindowWMInfo(&m_kData, &kInfo);
	}
	return kInfo;
}
//--------------------------------------------------------------------------
VeWindow* VeWindow::Cast(Data* pkData) noexcept
{
	return pkData ? vtd::member_cast(&VeWindow::m_kData, pkData) : nullptr;
}
//--------------------------------------------------------------------------
