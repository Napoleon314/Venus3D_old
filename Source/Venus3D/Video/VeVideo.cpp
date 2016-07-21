////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Video
//  File name:   VeVideo.cpp
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

#include "stdafx.h"

//--------------------------------------------------------------------------
void VeVideoDevice::Init() noexcept
{
	m_u32NextObjectID = 1;
	_Init();
}
//--------------------------------------------------------------------------
void VeVideoDevice::Term() noexcept
{
	while (m_kWindowList.size())
	{
		VeWindow::Data* pkWindow = m_kWindowList.get_head_node()->_Content;
		DestroyWindow(pkWindow);
		pkWindow->m_kNode.detach();
	}
	_Term();
}
//--------------------------------------------------------------------------
void VeVideoDevice::GetDisplayBounds(int32_t i32DisplayIndex,
	VeRect* pkRect) noexcept
{
	assert(i32DisplayIndex >= 0
		&& i32DisplayIndex < int32_t(m_kDisplayList.size()));
	if (pkRect)
	{
		VeVideoDisplay* pkDisplay = &m_kDisplayList[i32DisplayIndex];

		if (!_GetDisplayBounds(pkDisplay, pkRect))
		{
			if (i32DisplayIndex == 0)
			{
				pkRect->x = 0;
				pkRect->y = 0;
			}
			else
			{
				GetDisplayBounds(i32DisplayIndex - 1, pkRect);
				pkRect->x += pkRect->w;
			}
			pkRect->w = pkDisplay->m_kCurrentMode.m_i32Width;
			pkRect->h = pkDisplay->m_kCurrentMode.m_i32Height;
		}
	}
}
//--------------------------------------------------------------------------
#define CREATE_FLAGS \
    (VE_WINDOW_BORDERLESS | VE_WINDOW_RESIZABLE | VE_WINDOW_ALLOW_HIGHDPI)
//--------------------------------------------------------------------------
#define FULLSCREEN_VISIBLE(W)						\
    (((W)->m_u32Flags & VE_WINDOW_FULLSCREEN) &&	\
     ((W)->m_u32Flags & VE_WINDOW_SHOWN) &&			\
     !((W)->m_u32Flags & VE_WINDOW_MINIMIZED))
//--------------------------------------------------------------------------
bool VeVideoDevice::CreateWindowBy(VeWindow::Data* pkWindow,
	const char* pcTitle,	int32_t x, int32_t y, int32_t w, int32_t h,
	uint32_t u32Flags) noexcept
{
	assert(pkWindow && (!pkWindow->m_kNode.is_attach()));
	
	if (w < 1) w = 1;
	if (h < 1) h = 1;

	pkWindow->m_u32Id = m_u32NextObjectID++;
	pkWindow->x = x;
	pkWindow->y = y;
	pkWindow->w = w;
	pkWindow->h = h;

	if (VE_WINDOWPOS_ISUNDEFINED(x) || VE_WINDOWPOS_ISUNDEFINED(y)
		|| VE_WINDOWPOS_ISCENTERED(x) || VE_WINDOWPOS_ISCENTERED(y))
	{
		//VeVideoDisplay* pkDisplay = GetDisplayForWindow(pkWindow);
		int32_t i32DisplayIndex = GetWindowDisplayIndex(pkWindow);
		VeRect kBounds;
		GetDisplayBounds(i32DisplayIndex, &kBounds);

		if (VE_WINDOWPOS_ISUNDEFINED(x) || VE_WINDOWPOS_ISCENTERED(x))
		{
			pkWindow->x = kBounds.x + ((kBounds.w - w) >> 1);
		}
		if (VE_WINDOWPOS_ISUNDEFINED(y) || VE_WINDOWPOS_ISCENTERED(y))
		{
			pkWindow->y = kBounds.y + ((kBounds.h - h) >> 1);
		}
	}

	pkWindow->m_u32Flags = ((u32Flags & CREATE_FLAGS) | VE_WINDOW_HIDDEN);
	pkWindow->m_u32LastFullscreenFlags = pkWindow->m_u32Flags;
	pkWindow->m_f32Brightness = 1.0f;
	pkWindow->m_bIsDestorying = VE_FALSE;
	m_kWindowList.attach_back(pkWindow->m_kNode);
	

	if (!_CreateWindow(pkWindow))
	{
		DestroyWindow(pkWindow);
		return false;
	}

	SetWindowTitle(pkWindow, pcTitle);

	FinishWindowCreation(pkWindow, u32Flags);

	UpdateFullscreenMode(pkWindow, FULLSCREEN_VISIBLE(pkWindow));

	return true;
}
//--------------------------------------------------------------------------
bool VeVideoDevice::CreateWindowFrom(VeWindow::Data* pkWindow,
	const void* pvData) noexcept
{
	assert(pkWindow && (!pkWindow->m_kNode.is_attach()));

	pkWindow->m_u32Id = m_u32NextObjectID++;
	pkWindow->m_u32Flags = VE_WINDOW_FOREIGN;
	pkWindow->m_u32LastFullscreenFlags = pkWindow->m_u32Flags;
	pkWindow->m_f32Brightness = 1.0f;
	pkWindow->m_bIsDestorying = VE_FALSE;
	m_kWindowList.attach_back(pkWindow->m_kNode);

	if (!_CreateWindowFrom(pkWindow, pvData))
	{
		DestroyWindow(pkWindow);
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowTitle(VeWindow::Data* pkWindow,
	const char* pcTitle) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	vtd::string kTitle(pcTitle ? pcTitle : "");
	if (pkWindow->m_kTitle != kTitle)
	{
		pkWindow->m_kTitle = kTitle;
		_SetWindowTitle(pkWindow);
	}
}
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowIcon(VeWindow::Data* pkWindow,
	const VeSurfacePtr& spIcon) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));

	if (!spIcon) return;

	pkWindow->m_spIcon = spIcon;
	
	if (!pkWindow->m_spIcon) return;

	_SetWindowIcon(pkWindow, pkWindow->m_spIcon);
}
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowPosition(VeWindow::Data* pkWindow,
	int32_t x, int32_t y) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (VE_WINDOWPOS_ISCENTERED(x) || VE_WINDOWPOS_ISCENTERED(y))
	{
		VeVideoDisplay* pkDisplay = GetDisplayForWindow(pkWindow);
		int32_t i32DisplayIndex = GetIndexOfDisplay(pkDisplay);
		VeRect kBounds;
		GetDisplayBounds(i32DisplayIndex, &kBounds);
		if (VE_WINDOWPOS_ISCENTERED(x))
		{
			x = kBounds.x + ((kBounds.w - pkWindow->w) >> 1);
		}
		if (VE_WINDOWPOS_ISCENTERED(y))
		{
			y = kBounds.y + ((kBounds.h - pkWindow->h) >> 1);
		}
	}

	if ((pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN))
	{
		if (!VE_WINDOWPOS_ISUNDEFINED(x))
		{
			pkWindow->m_kWindowed.x = x;
		}
		if (!VE_WINDOWPOS_ISUNDEFINED(y))
		{
			pkWindow->m_kWindowed.y = y;
		}
	}
	else
	{
		if (!VE_WINDOWPOS_ISUNDEFINED(x))
		{
			pkWindow->x = x;
		}
		if (!VE_WINDOWPOS_ISUNDEFINED(y))
		{
			pkWindow->y = y;
		}

		_SetWindowPosition(pkWindow);

		SendWindowEvent(pkWindow, VE_WINDOWEVENT_MOVED, x, y);
	}
}
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowSize(VeWindow::Data* pkWindow,
	int32_t w, int32_t h) noexcept
{
	assert(w > 0 && h > 0);

	if (pkWindow->min_w && w < pkWindow->min_w)
	{
		w = pkWindow->min_w;
	}
	if (pkWindow->max_w && w > pkWindow->max_w)
	{
		w = pkWindow->max_w;
	}
	if (pkWindow->min_h && h < pkWindow->min_h)
	{
		h = pkWindow->min_h;
	}
	if (pkWindow->max_h && h > pkWindow->max_h)
	{
		h = pkWindow->max_h;
	}

	if (pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN)
	{
		pkWindow->m_kWindowed.w = w;
		pkWindow->m_kWindowed.h = h;
	}
	else
	{
		pkWindow->w = w;
		pkWindow->h = h;

		_SetWindowSize(pkWindow);

		if (pkWindow->w == w && pkWindow->h == h)
		{
			OnWindowResized(pkWindow);
		}
	}
}
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowMinimumSize(VeWindow::Data* pkWindow,
	int32_t min_w, int32_t min_h) noexcept
{
	assert(min_w > 0 && min_h > 0);

	if (!(pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN))
	{
		pkWindow->min_w = min_w;
		pkWindow->min_h = min_h;
		_SetWindowMinimumSize(pkWindow);
		
		SetWindowSize(pkWindow, VE_MAX(pkWindow->w, pkWindow->min_w),
			VE_MAX(pkWindow->h, pkWindow->min_h));
	}
}
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowMaximumSize(VeWindow::Data* pkWindow,
	int32_t max_w, int32_t max_h) noexcept
{
	assert(max_w > 0 && max_h > 0);

	if (!(pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN))
	{
		pkWindow->max_w = max_w;
		pkWindow->max_h = max_h;
		_SetWindowMaximumSize(pkWindow);
		
		SetWindowSize(pkWindow, VE_MIN(pkWindow->w, pkWindow->max_w),
			VE_MIN(pkWindow->h, pkWindow->max_h));
	}
}
//--------------------------------------------------------------------------
void VeVideoDevice::ShowWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (pkWindow->m_u32Flags & VE_WINDOW_SHOWN)
	{
		return;
	}

	_ShowWindow(pkWindow);

	SendWindowEvent(pkWindow, VE_WINDOWEVENT_SHOWN, 0, 0);
}
//--------------------------------------------------------------------------
void VeVideoDevice::HideWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!(pkWindow->m_u32Flags & VE_WINDOW_SHOWN))
	{
		return;
	}

	UpdateFullscreenMode(pkWindow, VE_FALSE);
	_HideWindow(pkWindow);

	SendWindowEvent(pkWindow, VE_WINDOWEVENT_HIDDEN, 0, 0);
}
//--------------------------------------------------------------------------
void VeVideoDevice::RaiseWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!(pkWindow->m_u32Flags & VE_WINDOW_SHOWN))
	{
		return;
	}

	_RaiseWindow(pkWindow);
}
//--------------------------------------------------------------------------
void VeVideoDevice::MaximizeWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (pkWindow->m_u32Flags & VE_WINDOW_MAXIMIZED)
	{
		return;
	}

	_MaximizeWindow(pkWindow);
}
//--------------------------------------------------------------------------
void VeVideoDevice::MinimizeWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (pkWindow->m_u32Flags & VE_WINDOW_MINIMIZED)
	{
		return;
	}

	UpdateFullscreenMode(pkWindow, VE_FALSE);

	_MinimizeWindow(pkWindow);
}
//--------------------------------------------------------------------------
void VeVideoDevice::RestoreWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!(pkWindow->m_u32Flags & (VE_WINDOW_MAXIMIZED | VE_WINDOW_MINIMIZED)))
	{
		return;
	}

	_RestoreWindow(pkWindow);
}
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowBordered(VeWindow::Data* pkWindow,
	bool bBordered) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!(pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN))
	{
		const int32_t i32Want = (bBordered != false);
		const int32_t i32Have = ((pkWindow->m_u32Flags & VE_WINDOW_BORDERLESS) == 0);
		if ((i32Want != i32Have))
		{
			if (i32Want)
			{
				pkWindow->m_u32Flags &= ~VE_WINDOW_BORDERLESS;
			}
			else
			{
				pkWindow->m_u32Flags |= VE_WINDOW_BORDERLESS;
			}
			_SetWindowBordered(pkWindow, (VE_BOOL)i32Want);
		}
	}
}
//--------------------------------------------------------------------------
#define FULLSCREEN_MASK ( VE_WINDOW_FULLSCREEN_DESKTOP | VE_WINDOW_FULLSCREEN )
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowFullscreen(VeWindow::Data* pkWindow,
	uint32_t u32Flags) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	u32Flags &= FULLSCREEN_MASK;

	if (u32Flags == (pkWindow->m_u32Flags & FULLSCREEN_MASK))
	{
		return;
	}

	pkWindow->m_u32Flags &= ~FULLSCREEN_MASK;
	pkWindow->m_u32Flags |= u32Flags;

	UpdateFullscreenMode(pkWindow, FULLSCREEN_VISIBLE(pkWindow));
}
//--------------------------------------------------------------------------
bool VeVideoDevice::SetWindowGammaRamp(VeWindow::Data* pkWindow,
	const uint16_t* red, const uint16_t* green,
	const uint16_t* blue) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!pkWindow->m_pu16Gamma)
	{
		if (!GetWindowGammaRamp(pkWindow, nullptr, nullptr, nullptr))
		{
			return false;
		}
	}

	if (red)
	{
		memcpy(&pkWindow->m_pu16Gamma[0 * 256], red, 256 * sizeof(uint16_t));
	}
	if (green)
	{
		memcpy(&pkWindow->m_pu16Gamma[1 * 256], green, 256 * sizeof(uint16_t));
	}
	if (blue)
	{
		memcpy(&pkWindow->m_pu16Gamma[2 * 256], blue, 256 * sizeof(uint16_t));
	}
	if (pkWindow->m_u32Flags & VE_WINDOW_INPUT_FOCUS)
	{
		return _SetWindowGammaRamp(pkWindow, pkWindow->m_pu16Gamma);
	}
	else
	{
		return true;
	}
}
//--------------------------------------------------------------------------
bool VeVideoDevice::GetWindowGammaRamp(VeWindow::Data* pkWindow,
	uint16_t* red, uint16_t* green, uint16_t* blue) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!pkWindow->m_pu16Gamma)
	{
		pkWindow->m_pu16Gamma = (uint16_t*)VeMalloc(256 * 6 * sizeof(uint16_t));
		assert(pkWindow->m_pu16Gamma);
		pkWindow->m_pu16SavedGamma = pkWindow->m_pu16Gamma + 3 * 256;

		if (!_GetWindowGammaRamp(pkWindow, pkWindow->m_pu16Gamma))
		{
			return false;
		}

		memcpy(pkWindow->m_pu16SavedGamma, pkWindow->m_pu16Gamma,
			3 * 256 * sizeof(uint16_t));
	}

	if (red)
	{
		memcpy(red, &pkWindow->m_pu16Gamma[0 * 256], 256 * sizeof(uint16_t));
	}
	if (green)
	{
		memcpy(green, &pkWindow->m_pu16Gamma[1 * 256], 256 * sizeof(uint16_t));
	}
	if (blue)
	{
		memcpy(blue, &pkWindow->m_pu16Gamma[2 * 256], 256 * sizeof(uint16_t));
	}
	return true;
}
//--------------------------------------------------------------------------
bool VeVideoDevice::SetWindowBrightness(VeWindow::Data* pkWindow,
	float f32Brightness) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	uint16_t au16Ramp[256];
	CalculateGammaRamp(f32Brightness, au16Ramp);
	bool bStatus = SetWindowGammaRamp(pkWindow, au16Ramp, au16Ramp, au16Ramp);
	if (bStatus)
	{
		pkWindow->m_f32Brightness = f32Brightness;
	}
	return bStatus;
}
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowGrab(VeWindow::Data* pkWindow,
	bool bGrabbed) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!!bGrabbed == !!(pkWindow->m_u32Flags & VE_WINDOW_INPUT_GRABBED))
	{
		return;
	}
	if (bGrabbed)
	{
		pkWindow->m_u32Flags |= VE_WINDOW_INPUT_GRABBED;
	}
	else
	{
		pkWindow->m_u32Flags &= ~VE_WINDOW_INPUT_GRABBED;
	}
	UpdateWindowGrab(pkWindow);
}
//--------------------------------------------------------------------------
void VeVideoDevice::DestroyWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	pkWindow->m_bIsDestorying = VE_TRUE;

	HideWindow(pkWindow);

	auto spKeyBoard = Venus3D::Ref().GetKeyboard();
	if (spKeyBoard && spKeyBoard->m_pkFocus == pkWindow)
	{
		spKeyBoard->SetFocus(nullptr);
	}
	auto spMouse = Venus3D::Ref().GetMouse();
	if (spMouse && spMouse->m_pkFocus == pkWindow)
	{
		spMouse->SetFocus(nullptr);
	}

	_DestroyWindow(pkWindow);

	VeVideoDisplay* pkDisplay = GetDisplayForWindow(pkWindow);
	if ((VeWindow::Data*)(pkDisplay->m_pvFullscreenWindow) == pkWindow)
	{
		pkDisplay->m_pvFullscreenWindow = nullptr;
	}

	pkWindow->m_kTitle = "";
	pkWindow->m_spIcon = nullptr;

	if (pkWindow->m_pu16Gamma)
	{
		VeFree(pkWindow->m_pu16Gamma);
		pkWindow->m_pu16Gamma = nullptr;
	}

	pkWindow->m_kNode.detach();
}
//--------------------------------------------------------------------------
void VeVideoDevice::GetWindowWMInfo(VeWindow::Data* pkWindow,
	VeSysWMInfo* pkInfo) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	_GetWindowWMInfo(pkWindow, pkInfo);
}
//--------------------------------------------------------------------------
void VeVideoDevice::PeekEvents(VeVector<VeEvent*>& kOutput) noexcept
{
	assert(Venus3D::Ref().GetEventQueue());
	_PumpEvents();
	Venus3D::Ref().GetEventQueue()->PeekEvents(kOutput);
}
//--------------------------------------------------------------------------
int32_t VeVideoDevice::GetWindowDisplayIndex(
	VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow);
	int32_t i32DisplayNum = (int32_t)(m_kDisplayList.size());

	int32_t displayIndex;
	int32_t i, dist;
	int32_t closest = -1;
	int32_t closest_dist = 0x7FFFFFFF;
	VePoint center;
	VePoint delta;
	VeRect rect;

	if (VE_WINDOWPOS_ISUNDEFINED(pkWindow->x)
		|| VE_WINDOWPOS_ISCENTERED(pkWindow->x))
	{
		displayIndex = (pkWindow->x & 0xFFFF);
		if (displayIndex >= i32DisplayNum)
		{
			displayIndex = 0;
		}
		return displayIndex;
	}
	if (VE_WINDOWPOS_ISUNDEFINED(pkWindow->y)
		|| VE_WINDOWPOS_ISCENTERED(pkWindow->y))
	{
		displayIndex = (pkWindow->y & 0xFFFF);
		if (displayIndex >= i32DisplayNum)
		{
			displayIndex = 0;
		}
		return displayIndex;
	}

	for (i = 0; i < i32DisplayNum; ++i)
	{
		VeVideoDisplay* display = &m_kDisplayList[i];

		if (display->m_pvFullscreenWindow == pkWindow)
		{
			return i;
		}
	}
	center.x = pkWindow->x + (pkWindow->w >> 1);
	center.y = pkWindow->y + (pkWindow->h >> 1);
	for (i = 0; i < i32DisplayNum; ++i)
	{
		GetDisplayBounds(i, &rect);
		if (vtd::enclose_points<int32_t>(&center, 1, &rect, nullptr))
		{
			return i;
		}

		delta.x = center.x - (rect.x + (rect.w >> 2));
		delta.y = center.y - (rect.y + (rect.h >> 2));
		dist = (delta.x * delta.x + delta.y * delta.y);
		if (dist < closest_dist)
		{
			closest = i;
			closest_dist = dist;
		}
	}
	if (closest < 0)
	{
		VeCoreLogI("Couldn't find any displays");
	}
	return closest;
}
//--------------------------------------------------------------------------
VeVideoDisplay* VeVideoDevice::GetDisplayForWindow(
	VeWindow::Data* pkWindow) noexcept
{
	int32_t displayIndex = GetWindowDisplayIndex(pkWindow);
	if (displayIndex >= 0)
	{
		return &m_kDisplayList[displayIndex];
	}
	else
	{
		return nullptr;
	}
}
//--------------------------------------------------------------------------
int32_t VeVideoDevice::GetIndexOfDisplay(VeVideoDisplay* pkDisplay) noexcept
{
	for (uint32_t u32Index(0); u32Index < m_kDisplayList.size(); ++u32Index)
	{
		if (pkDisplay == &m_kDisplayList[u32Index])
		{
			return u32Index;
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
void VeVideoDevice::UpdateFullscreenMode(VeWindow::Data*,
	VE_BOOL) noexcept
{
	/*VeVideoDisplay* pkDisplay = GetDisplayForWindow(pkWindow);

	if (bFullscreen)
	{
		if (pkDisplay->m_spFullscreenWindow &&
			pkDisplay->m_spFullscreenWindow != VeWindow::Cast(pkWindow))
		{
			//SDL_MinimizeWindow(display->fullscreen_window);
		}
	}*/

	

}
//--------------------------------------------------------------------------
void VeVideoDevice::FinishWindowCreation(VeWindow::Data* pkWindow,
	uint32_t u32Flags) noexcept
{
	pkWindow->m_kWindowed.x = pkWindow->x;
	pkWindow->m_kWindowed.y = pkWindow->y;
	pkWindow->m_kWindowed.w = pkWindow->w;
	pkWindow->m_kWindowed.h = pkWindow->h;

	if (u32Flags & VE_WINDOW_MAXIMIZED)
	{
		MaximizeWindow(pkWindow);
	}
	if (u32Flags & VE_WINDOW_MINIMIZED)
	{
		MinimizeWindow(pkWindow);
	}
	if (u32Flags & VE_WINDOW_FULLSCREEN)
	{
		SetWindowFullscreen(pkWindow, u32Flags);
	}
	if (u32Flags & VE_WINDOW_INPUT_GRABBED)
	{
		SetWindowGrab(pkWindow, true);
	}
	if (!(u32Flags & VE_WINDOW_HIDDEN))
	{
		ShowWindow(pkWindow);
	}
}
//--------------------------------------------------------------------------
void VeVideoDevice::SendWindowEvent(VeWindow::Data* pkWindow,
	uint8_t u8Event, int32_t i32Data1, int32_t i32Data2) noexcept
{
	assert(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));

	switch (u8Event)
	{
	case VE_WINDOWEVENT_SHOWN:
		if (pkWindow->m_u32Flags & VE_WINDOW_SHOWN)
		{
			return;
		}
		pkWindow->m_u32Flags &= ~VE_WINDOW_HIDDEN;
		pkWindow->m_u32Flags |= VE_WINDOW_SHOWN;
		OnWindowShown(pkWindow);
		break;
	case VE_WINDOWEVENT_HIDDEN:
		if (!(pkWindow->m_u32Flags & VE_WINDOW_SHOWN))
		{
			return;
		}
		pkWindow->m_u32Flags &= ~VE_WINDOW_SHOWN;
		pkWindow->m_u32Flags |= VE_WINDOW_HIDDEN;
		OnWindowHidden(pkWindow);
		break;
	case VE_WINDOWEVENT_MOVED:
		if (VE_WINDOWPOS_ISUNDEFINED(i32Data1) ||
			VE_WINDOWPOS_ISUNDEFINED(i32Data2))
		{
			return;
		}
		if (!(pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN))
		{
			pkWindow->m_kWindowed.x = i32Data1;
			pkWindow->m_kWindowed.y = i32Data2;
		}
		if (i32Data1 == pkWindow->x && i32Data2 == pkWindow->y)
		{
			return;
		}
		pkWindow->x = i32Data1;
		pkWindow->y = i32Data2;
		break;
	case VE_WINDOWEVENT_RESIZED:
		if (!(pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN))
		{
			pkWindow->m_kWindowed.w = i32Data1;
			pkWindow->m_kWindowed.h = i32Data2;
		}
		if (i32Data1 == pkWindow->w && i32Data2 == pkWindow->h)
		{
			return;
		}
		pkWindow->w = i32Data1;
		pkWindow->h = i32Data2;
		OnWindowResized(pkWindow);
		break;
	case VE_WINDOWEVENT_MINIMIZED:
		if (pkWindow->m_u32Flags & VE_WINDOW_MINIMIZED)
		{
			return;
		}
		pkWindow->m_u32Flags |= VE_WINDOW_MINIMIZED;
		OnWindowMinimized(pkWindow);
		break;
	case VE_WINDOWEVENT_MAXIMIZED:
		if (pkWindow->m_u32Flags & VE_WINDOW_MAXIMIZED)
		{
			return;
		}
		pkWindow->m_u32Flags |= VE_WINDOW_MAXIMIZED;
		break;
	case VE_WINDOWEVENT_RESTORED:
		if (!(pkWindow->m_u32Flags & (VE_WINDOW_MINIMIZED | VE_WINDOW_MAXIMIZED)))
		{
			return;
		}
		pkWindow->m_u32Flags &= ~(VE_WINDOW_MINIMIZED | VE_WINDOW_MAXIMIZED);
		OnWindowRestored(pkWindow);
		break;
	case VE_WINDOWEVENT_ENTER:
		if (pkWindow->m_u32Flags & VE_WINDOW_MOUSE_FOCUS)
		{
			return;
		}
		pkWindow->m_u32Flags |= VE_WINDOW_MOUSE_FOCUS;
		OnWindowEnter(pkWindow);
		break;
	case VE_WINDOWEVENT_LEAVE:
		if (!(pkWindow->m_u32Flags & VE_WINDOW_MOUSE_FOCUS))
		{
			return;
		}
		pkWindow->m_u32Flags &= ~VE_WINDOW_MOUSE_FOCUS;
		OnWindowLeave(pkWindow);
		break;
	case VE_WINDOWEVENT_FOCUS_GAINED:
		if (pkWindow->m_u32Flags & VE_WINDOW_INPUT_FOCUS)
		{
			return;
		}
		pkWindow->m_u32Flags |= VE_WINDOW_INPUT_FOCUS;
		OnWindowFocusGained(pkWindow);
		break;
	case VE_WINDOWEVENT_FOCUS_LOST:
		if (!(pkWindow->m_u32Flags & VE_WINDOW_INPUT_FOCUS))
		{
			return;
		}
		pkWindow->m_u32Flags &= ~VE_WINDOW_INPUT_FOCUS;
		OnWindowFocusLost(pkWindow);
		break;
	}

	assert(Venus3D::Ref().GetEventQueue());
	if (Venus3D::Ref().GetEventQueue()->IsEventTypeEnable(VE_WINDOWEVENT))
	{
		switch (u8Event)
		{
		case VE_WINDOWEVENT_RESIZED:
		case VE_WINDOWEVENT_SIZE_CHANGED:
		case VE_WINDOWEVENT_MOVED:
			Venus3D::Ref().GetEventQueue()->FilterEvents([u8Event, pkWindow](VeEvent& kEvent) noexcept
			{
				if (kEvent.m_u32Type == VE_WINDOWEVENT
					&& kEvent.m_kWindow.m_u8Event == u8Event
					&& kEvent.m_kWindow.m_u32WindowID == pkWindow->m_u32Id)
				{
					return false;
				}
				return true;
			});
			break;
		default:
			break;
		}

		VeEvent* pkEvent = Venus3D::Ref().GetEventQueue()->AddEvent();
		pkEvent->m_kWindow.m_u32Type = VE_WINDOWEVENT;
		pkEvent->m_kWindow.m_u32TimeStamp = VeEventQueue::GetTicks();
		pkEvent->m_kWindow.m_u8Event = u8Event;
		pkEvent->m_kWindow.m_i32Data1 = i32Data1;
		pkEvent->m_kWindow.m_i32Data2 = i32Data2;
		pkEvent->m_kWindow.m_u32WindowID = pkWindow->m_u32Id;
	}

	if (u8Event == VE_WINDOWEVENT_CLOSE)
	{
		if (m_kWindowList.size() == 1)
		{
			Venus3D::Ref().GetEventQueue()->SendAppEvent(VE_QUIT);
		}
	}
}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowShown(VeWindow::Data* pkWindow) noexcept
{
	OnWindowRestored(pkWindow);
}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowHidden(VeWindow::Data* pkWindow) noexcept
{
	UpdateFullscreenMode(pkWindow, VE_FALSE);
}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowResized(VeWindow::Data* pkWindow) noexcept
{
	pkWindow->m_bSurfaceValid = VE_FALSE;
	SendWindowEvent(pkWindow, VE_WINDOWEVENT_SIZE_CHANGED, pkWindow->w, pkWindow->h);
}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowMinimized(VeWindow::Data* pkWindow) noexcept
{
	UpdateFullscreenMode(pkWindow, VE_FALSE);
}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowRestored(VeWindow::Data* pkWindow) noexcept
{
	RaiseWindow(pkWindow);

	if (FULLSCREEN_VISIBLE(pkWindow))
	{
		UpdateFullscreenMode(pkWindow, VE_TRUE);
	}
}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowEnter(VeWindow::Data* pkWindow) noexcept
{
	_OnWindowEnter(pkWindow);
}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowLeave(VeWindow::Data*) noexcept
{

}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowFocusGained(VeWindow::Data* pkWindow) noexcept
{
	if (pkWindow->m_pu16Gamma)
	{
		_SetWindowGammaRamp(pkWindow, pkWindow->m_pu16Gamma);
	}

	if (Venus3D::Ref().GetMouse() && Venus3D::Ref().GetMouse()->IsRelativeModeEnable())
	{
		Venus3D::Ref().GetMouse()->SetFocus(pkWindow);
		Venus3D::Ref().GetMouse()->WarpInWindow(pkWindow, pkWindow->w >> 1, pkWindow->h >> 1);
	}

	UpdateWindowGrab(pkWindow);
}
//--------------------------------------------------------------------------
static VE_BOOL ShouldMinimizeOnFocusLoss(VeWindow::Data* pkWindow)
{
	if (!(pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN)
		|| pkWindow->m_bIsDestorying)
	{
		return VE_FALSE;
	}

#ifdef __MACOSX__
	if (Cocoa_IsWindowInFullscreenSpace(window)) {
		return SDL_FALSE;
	}
#endif

	return VE_TRUE;
}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowFocusLost(VeWindow::Data* pkWindow) noexcept
{
	if (pkWindow->m_pu16Gamma)
	{
		_SetWindowGammaRamp(pkWindow, pkWindow->m_pu16SavedGamma);
	}

	UpdateWindowGrab(pkWindow);

	if (ShouldMinimizeOnFocusLoss(pkWindow))
	{
		MinimizeWindow(pkWindow);
	}
}
//--------------------------------------------------------------------------
void VeVideoDevice::UpdateWindowGrab(VeWindow::Data* pkWindow) noexcept
{
		VE_BOOL bGrabbed;
		if ((Venus3D::Ref().GetMouse()->IsRelativeModeEnable()
			|| (pkWindow->m_u32Flags & VE_WINDOW_INPUT_GRABBED))
			&& (pkWindow->m_u32Flags & VE_WINDOW_INPUT_FOCUS))
		{
			bGrabbed = VE_TRUE;
		}
		else
		{
			bGrabbed = VE_FALSE;
		}
		_SetWindowGrab(pkWindow, bGrabbed);
}
//--------------------------------------------------------------------------
VeWindow::Data* VeVideoDevice::GetKeyboardFocus() noexcept
{
	return Venus3D::Ref().GetKeyboard() ? Venus3D::Ref().GetKeyboard()->m_pkFocus : nullptr;
}
//--------------------------------------------------------------------------
VeWindow::Data* VeVideoDevice::GetMouseFocus() noexcept
{
	return Venus3D::Ref().GetMouse() ? Venus3D::Ref().GetMouse()->m_pkFocus : nullptr;
}
//--------------------------------------------------------------------------
bool VeVideoDevice::_GetWindowGammaRamp(VeWindow::Data* pkWindow,
	uint16_t*) noexcept
{
	for (int32_t i(0); i < 256; ++i)
	{
		uint16_t u16Value = (uint16_t)((i << 8) | i);
		pkWindow->m_pu16Gamma[0 * 256 + i] = u16Value;
		pkWindow->m_pu16Gamma[1 * 256 + i] = u16Value;
		pkWindow->m_pu16Gamma[2 * 256 + i] = u16Value;
	}
	return true;
}
//--------------------------------------------------------------------------
void VeVideoDevice::CalculateGammaRamp(float f32Gamma,
	uint16_t* pu16Ramp) noexcept
{
	assert(f32Gamma >= 0.0f && pu16Ramp);

	if (f32Gamma == 0.0f)
	{
		for (int32_t i(0); i < 256; ++i)
		{
			pu16Ramp[i] = 0;
		}
		return;
	}
	else if (f32Gamma == 1.0f)
	{
		for (int32_t i = 0; i < 256; ++i)
		{
			pu16Ramp[i] = (uint16_t)((i << 8) | i);
		}
		return;
	}
	else
	{
		int32_t i32Value;
		f32Gamma = 1.0f / f32Gamma;
		for (int32_t i(0); i < 256; ++i)
		{
			i32Value = (int32_t)(powf((float)i / 256.0f, f32Gamma) * 65535.0f + 0.5f);
			if (i32Value > 65535)
			{
				i32Value = 65535;
			}
			pu16Ramp[i] = (uint16_t)i32Value;
		}
	}
}
//--------------------------------------------------------------------------
