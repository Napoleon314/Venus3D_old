////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeVideo.cpp
//  Created:     2015/07/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

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
		VeWindow::Data* pkWindow = m_kWindowList.get_head_node()->m_Content;
		DestroyWindow(pkWindow);
		pkWindow->m_kNode.detach();
	}
	_Term();
}
//--------------------------------------------------------------------------
void VeVideoDevice::GetDisplayBounds(VeInt32 i32DisplayIndex,
	VeRect* pkRect) noexcept
{
	VE_ASSERT(i32DisplayIndex >= 0
		&& i32DisplayIndex < VeInt32(m_kDisplayList.size()));
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
	const VeChar8* pcTitle,	VeInt32 x, VeInt32 y, VeInt32 w, VeInt32 h,
	VeUInt32 u32Flags) noexcept
{
	VE_ASSERT(pkWindow && (!pkWindow->m_kNode.is_attach()));
	
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
		VeVideoDisplay* pkDisplay = GetDisplayForWindow(pkWindow);
		VeInt32 i32DisplayIndex = GetWindowDisplayIndex(pkWindow);
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
	VE_ASSERT(pkWindow && (!pkWindow->m_kNode.is_attach()));

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
	const VeChar8* pcTitle) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	VeFixedString kTitle(pcTitle ? pcTitle : "");
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
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));

	if (!spIcon) return;

	pkWindow->m_spIcon = spIcon;
	
	if (!pkWindow->m_spIcon) return;

	_SetWindowIcon(pkWindow, pkWindow->m_spIcon);
}
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowPosition(VeWindow::Data* pkWindow,
	VeInt32 x, VeInt32 y) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (VE_WINDOWPOS_ISCENTERED(x) || VE_WINDOWPOS_ISCENTERED(y))
	{
		VeVideoDisplay* pkDisplay = GetDisplayForWindow(pkWindow);
		VeInt32 i32DisplayIndex = GetIndexOfDisplay(pkDisplay);
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
	VeInt32 w, VeInt32 h) noexcept
{
	VE_ASSERT(w > 0 && h > 0);

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
	VeInt32 min_w, VeInt32 min_h) noexcept
{
	VE_ASSERT(min_w > 0 && min_h > 0);

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
	VeInt32 max_w, VeInt32 max_h) noexcept
{
	VE_ASSERT(max_w > 0 && max_h > 0);

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
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
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
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
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
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!(pkWindow->m_u32Flags & VE_WINDOW_SHOWN))
	{
		return;
	}

	_RaiseWindow(pkWindow);
}
//--------------------------------------------------------------------------
void VeVideoDevice::MaximizeWindow(VeWindow::Data* pkWindow) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (pkWindow->m_u32Flags & VE_WINDOW_MAXIMIZED)
	{
		return;
	}

	_MaximizeWindow(pkWindow);
}
//--------------------------------------------------------------------------
void VeVideoDevice::MinimizeWindow(VeWindow::Data* pkWindow) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
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
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!(pkWindow->m_u32Flags & (VE_WINDOW_MAXIMIZED | VE_WINDOW_MINIMIZED)))
	{
		return;
	}

	_RestoreWindow(pkWindow);
}
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowBordered(VeWindow::Data* pkWindow,
	VE_BOOL bBordered) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!(pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN))
	{
		const VeInt32 i32Want = (bBordered != VE_FALSE);
		const VeInt32 i32Have = ((pkWindow->m_u32Flags & VE_WINDOW_BORDERLESS) == 0);
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
	VeUInt32 u32Flags) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
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
	const VeUInt16* red, const VeUInt16* green,
	const VeUInt16* blue) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!pkWindow->m_pu16Gamma)
	{
		if (!GetWindowGammaRamp(pkWindow, nullptr, nullptr, nullptr))
		{
			return false;
		}
	}

	if (red)
	{
		VeMemoryCopy(&pkWindow->m_pu16Gamma[0 * 256], red, 256 * sizeof(VeUInt16));
	}
	if (green)
	{
		VeMemoryCopy(&pkWindow->m_pu16Gamma[1 * 256], green, 256 * sizeof(VeUInt16));
	}
	if (blue)
	{
		VeMemoryCopy(&pkWindow->m_pu16Gamma[2 * 256], blue, 256 * sizeof(VeUInt16));
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
	VeUInt16* red, VeUInt16* green, VeUInt16* blue) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	if (!pkWindow->m_pu16Gamma)
	{
		pkWindow->m_pu16Gamma = (VeUInt16*)VeMalloc(256 * 6 * sizeof(VeUInt16));
		VE_ASSERT(pkWindow->m_pu16Gamma);
		pkWindow->m_pu16SavedGamma = pkWindow->m_pu16Gamma + 3 * 256;

		if (!_GetWindowGammaRamp(pkWindow, pkWindow->m_pu16Gamma))
		{
			return false;
		}

		VeMemoryCopy(pkWindow->m_pu16SavedGamma, pkWindow->m_pu16Gamma,
			3 * 256 * sizeof(VeUInt16));
	}

	if (red)
	{
		VeMemoryCopy(red, &pkWindow->m_pu16Gamma[0 * 256], 256 * sizeof(VeUInt16));
	}
	if (green)
	{
		VeMemoryCopy(green, &pkWindow->m_pu16Gamma[1 * 256], 256 * sizeof(VeUInt16));
	}
	if (blue)
	{
		VeMemoryCopy(blue, &pkWindow->m_pu16Gamma[2 * 256], 256 * sizeof(VeUInt16));
	}
	return true;
}
//--------------------------------------------------------------------------
bool VeVideoDevice::SetWindowBrightness(VeWindow::Data* pkWindow,
	VeFloat32 f32Brightness) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	VeUInt16 au16Ramp[256];
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
	VE_BOOL bGrabbed) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	/*if (!!grabbed == !!(window->flags & SDL_WINDOW_INPUT_GRABBED)) {
		return;
	}
	if (grabbed) {
		window->flags |= SDL_WINDOW_INPUT_GRABBED;
	}
	else {
		window->flags &= ~SDL_WINDOW_INPUT_GRABBED;
	}
	UpdateWindowGrab(window);*/
}
//--------------------------------------------------------------------------
void VeVideoDevice::DestroyWindow(VeWindow::Data* pkWindow) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	pkWindow->m_bIsDestorying = VE_TRUE;

	HideWindow(pkWindow);

	/*if (SDL_GetKeyboardFocus() == window)
	{
		SDL_SetKeyboardFocus(NULL);
	}
	if (SDL_GetMouseFocus() == window) {
		SDL_SetMouseFocus(NULL);
	}*/

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
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));
	_GetWindowWMInfo(pkWindow, pkInfo);
}
//--------------------------------------------------------------------------
void VeVideoDevice::PeekEvents(VeVector<VeEvent*>& kOutput) noexcept
{
	VE_ASSERT(ve_event_queue_ptr);
	_PumpEvents();
	ve_event_queue_ptr->PeekEvents(kOutput);
}
//--------------------------------------------------------------------------
VeInt32 VeVideoDevice::GetWindowDisplayIndex(
	VeWindow::Data* pkWindow) noexcept
{
	VE_ASSERT(pkWindow);
	VeInt32 i32DisplayNum = (VeInt32)(m_kDisplayList.size());

	VeInt32 displayIndex;
	VeInt32 i, dist;
	VeInt32 closest = -1;
	VeInt32 closest_dist = 0x7FFFFFFF;
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
		if (VeRect::EnclosePoints(nullptr, &center, 1, &rect))
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
		VeDebugOutputCore("Couldn't find any displays");
	}
	return closest;
}
//--------------------------------------------------------------------------
VeVideoDisplay* VeVideoDevice::GetDisplayForWindow(
	VeWindow::Data* pkWindow) noexcept
{
	VeInt32 displayIndex = GetWindowDisplayIndex(pkWindow);
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
VeInt32 VeVideoDevice::GetIndexOfDisplay(VeVideoDisplay* pkDisplay) noexcept
{
	for (VeUInt32 u32Index(0); u32Index < m_kDisplayList.size(); ++u32Index)
	{
		if (pkDisplay == &m_kDisplayList[u32Index])
		{
			return u32Index;
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
void VeVideoDevice::UpdateFullscreenMode(VeWindow::Data* pkWindow,
	VE_BOOL bFullscreen) noexcept
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
	VeUInt32 u32Flags) noexcept
{
	pkWindow->m_kWindowed.x = pkWindow->x;
	pkWindow->m_kWindowed.y = pkWindow->y;
	pkWindow->m_kWindowed.w = pkWindow->w;
	pkWindow->m_kWindowed.h = pkWindow->h;

	if (u32Flags & VE_WINDOW_MAXIMIZED)
	{
		//SDL_MaximizeWindow(window);
	}
	if (u32Flags & VE_WINDOW_MINIMIZED)
	{
		//SDL_MinimizeWindow(window);
	}
	if (u32Flags & VE_WINDOW_FULLSCREEN)
	{
		//SDL_SetWindowFullscreen(window, flags);
	}
	if (u32Flags & VE_WINDOW_INPUT_GRABBED)
	{
		//SDL_SetWindowGrab(window, SDL_TRUE);
	}
	if (!(u32Flags & VE_WINDOW_HIDDEN))
	{
		ShowWindow(pkWindow);
	}
}
//--------------------------------------------------------------------------
void VeVideoDevice::SendWindowEvent(VeWindow::Data* pkWindow,
	VeUInt8 u8Event, VeInt32 i32Data1, VeInt32 i32Data2) noexcept
{
	VE_ASSERT(pkWindow && pkWindow->m_kNode.is_attach(m_kWindowList));

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

	VE_ASSERT(ve_event_queue_ptr);
	if (ve_event_queue_ptr->IsEventTypeEnable(VE_WINDOWEVENT))
	{
		switch (u8Event)
		{
		case VE_WINDOWEVENT_RESIZED:
		case VE_WINDOWEVENT_SIZE_CHANGED:
		case VE_WINDOWEVENT_MOVED:
			ve_event_queue_ptr->FilterEvents([u8Event, pkWindow](VeEvent& kEvent) noexcept
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

		VeEvent* pkEvent = ve_event_queue_ptr->AddEvent();
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
			ve_event_queue_ptr->SendAppEvent(VE_QUIT);
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
void VeVideoDevice::OnWindowLeave(VeWindow::Data* pkWindow) noexcept
{

}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowFocusGained(VeWindow::Data* pkWindow) noexcept
{
	//SDL_Mouse *mouse = SDL_GetMouse();

	if (pkWindow->m_pu16Gamma)
	{
		_SetWindowGammaRamp(pkWindow, pkWindow->m_pu16Gamma);
	}

	/*if (mouse && mouse->relative_mode) {
		SDL_SetMouseFocus(window);
		SDL_WarpMouseInWindow(window, window->w / 2, window->h / 2);
	}*/

	//SDL_UpdateWindowGrab(window);*
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

	//SDL_UpdateWindowGrab(window);

	if (ShouldMinimizeOnFocusLoss(pkWindow))
	{
		//SDL_MinimizeWindow(window);
	}
}
//--------------------------------------------------------------------------
bool VeVideoDevice::_GetWindowGammaRamp(VeWindow::Data* pkWindow,
	VeUInt16* pu16Ramp) noexcept
{
	for (VeInt32 i(0); i < 256; ++i)
	{
		VeUInt16 u16Value = (VeUInt16)((i << 8) | i);
		pkWindow->m_pu16Gamma[0 * 256 + i] = u16Value;
		pkWindow->m_pu16Gamma[1 * 256 + i] = u16Value;
		pkWindow->m_pu16Gamma[2 * 256 + i] = u16Value;
	}
	return true;
}
//--------------------------------------------------------------------------
void VeVideoDevice::CalculateGammaRamp(VeFloat32 f32Gamma,
	VeUInt16* pu16Ramp) noexcept
{
	VE_ASSERT(f32Gamma >= 0.0f && pu16Ramp);

	if (f32Gamma == 0.0f)
	{
		for (VeInt32 i(0); i < 256; ++i)
		{
			pu16Ramp[i] = 0;
		}
		return;
	}
	else if (f32Gamma == 1.0f)
	{
		for (VeInt32 i = 0; i < 256; ++i)
		{
			pu16Ramp[i] = (i << 8) | i;
		}
		return;
	}
	else
	{
		VeInt32 i32Value;
		f32Gamma = 1.0f / f32Gamma;
		for (VeInt32 i(0); i < 256; ++i)
		{
			i32Value = (VeInt32)(VePowf((VeFloat32)i / 256.0f, f32Gamma) * 65535.0f + 0.5f);
			if (i32Value > 65535)
			{
				i32Value = 65535;
			}
			pu16Ramp[i] = (VeUInt16)i32Value;
		}
	}
}
//--------------------------------------------------------------------------
