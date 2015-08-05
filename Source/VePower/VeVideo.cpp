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

#include "VePowerPch.h"

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
		VeWindowPtr spWindow = m_kWindowList.get_head_node()->m_Content;
		DestroyWindow(spWindow);
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
VeWindowPtr VeVideoDevice::CreateWindowBy(const VeChar8* pcTitle,
	VeInt32 x, VeInt32 y, VeInt32 w, VeInt32 h, VeUInt32 u32Flags) noexcept
{
	VeWindowPtr spWindow;
	
	if (w < 1) w = 1;
	if (h < 1) h = 1;

	spWindow = VE_NEW VeWindow();
	VE_ASSERT(spWindow);
	spWindow->m_kData.m_u32Id = m_u32NextObjectID++;
	spWindow->m_kData.x = x;
	spWindow->m_kData.y = y;
	spWindow->m_kData.w = w;
	spWindow->m_kData.h = h;

	if (VE_WINDOWPOS_ISUNDEFINED(x) || VE_WINDOWPOS_ISUNDEFINED(y)
		|| VE_WINDOWPOS_ISCENTERED(x) || VE_WINDOWPOS_ISCENTERED(y))
	{
		VeVideoDisplay* pkDisplay = GetDisplayForWindow(&(spWindow->m_kData));
		VeInt32 i32DisplayIndex = GetWindowDisplayIndex(&(spWindow->m_kData));
		VeRect kBounds;
		GetDisplayBounds(i32DisplayIndex, &kBounds);

		if (VE_WINDOWPOS_ISUNDEFINED(x) || VE_WINDOWPOS_ISCENTERED(x))
		{
			spWindow->m_kData.x = kBounds.x + ((kBounds.w - w) >> 1);
		}
		if (VE_WINDOWPOS_ISUNDEFINED(y) || VE_WINDOWPOS_ISCENTERED(y))
		{
			spWindow->m_kData.y = kBounds.y + ((kBounds.h - h) >> 1);
		}
	}

	spWindow->m_kData.m_u32Flags = ((u32Flags & CREATE_FLAGS) | VE_WINDOW_HIDDEN);
	spWindow->m_kData.m_u32LastFullscreenFlags = spWindow->m_kData.m_u32Flags;
	spWindow->m_kData.m_f32Brightness = 1.0f;
	spWindow->m_kData.m_bIsDestorying = VE_FALSE;
	spWindow->IncRefCount();
	m_kWindowList.attach_back(spWindow->m_kNode);
	

	if (!_CreateWindow(&(spWindow->m_kData)))
	{
		DestroyWindow(spWindow);
		return nullptr;
	}

	SetWindowTitle(&(spWindow->m_kData), pcTitle);

	FinishWindowCreation(&(spWindow->m_kData), u32Flags);

	UpdateFullscreenMode(&(spWindow->m_kData), FULLSCREEN_VISIBLE(&(spWindow->m_kData)));

	return spWindow;
}
//--------------------------------------------------------------------------
void VeVideoDevice::DestroyWindow(VeWindowPtr& spWindow) noexcept
{
	VE_ASSERT(spWindow && spWindow->GetRefCount() == 2);
	spWindow->m_kData.m_bIsDestorying = VE_TRUE;

	HideWindow(&(spWindow->m_kData));

	/*if (SDL_GetKeyboardFocus() == window)
	{
		SDL_SetKeyboardFocus(NULL);
	}
	if (SDL_GetMouseFocus() == window) {
		SDL_SetMouseFocus(NULL);
	}*/

	_DestroyWindow(&(spWindow->m_kData));

	VeVideoDisplay* pkDisplay = GetDisplayForWindow(&(spWindow->m_kData));
	if (pkDisplay->m_spFullscreenWindow == spWindow)
	{
		pkDisplay->m_spFullscreenWindow = nullptr;
	}

	spWindow->m_kNode.detach();
	spWindow->DecRefCount();
	spWindow = nullptr;
}
//--------------------------------------------------------------------------
void VeVideoDevice::ShowWindow(VeWindow::Data* pkWindow) noexcept
{
	VE_ASSERT(pkWindow);
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
	VE_ASSERT(pkWindow);
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
	VE_ASSERT(pkWindow);
	if (!(pkWindow->m_u32Flags & VE_WINDOW_SHOWN))
	{
		return;
	}

	_RaiseWindow(pkWindow);
}
//--------------------------------------------------------------------------
void VeVideoDevice::SetWindowTitle(VeWindow::Data* pkWindow,
	const VeChar8* pcTitle) noexcept
{
	VE_ASSERT(pkWindow);
	VeFixedString kTitle(pcTitle ? pcTitle : "");
	if (pkWindow->m_kTitle != kTitle)
	{
		pkWindow->m_kTitle = kTitle;
		_SetWindowTitle(pkWindow);
	}
}
//--------------------------------------------------------------------------
const VeChar8* VeVideoDevice::GetWindowTitle(
	VeWindow::Data* pkWindow) noexcept
{
	VE_ASSERT(pkWindow);
	return pkWindow->m_kTitle;
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

		if (display->m_spFullscreenWindow == VeWindow::Cast(pkWindow))
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
	VE_ASSERT(pkWindow);

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

		VeEvent* pkNew = ve_event_queue_ptr->AddEvent();
		pkNew->m_u32Type = VE_WINDOWEVENT;
		pkNew->m_kWindow.m_u8Event = u8Event;
		pkNew->m_kWindow.m_i32Data1 = i32Data1;
		pkNew->m_kWindow.m_i32Data2 = i32Data2;
		pkNew->m_kWindow.m_u32WindowID = pkWindow->m_u32Id;
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

}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowHidden(VeWindow::Data* pkWindow) noexcept
{

}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowResized(VeWindow::Data* pkWindow) noexcept
{

}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowMinimized(VeWindow::Data* pkWindow) noexcept
{

}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowRestored(VeWindow::Data* pkWindow) noexcept
{

}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowEnter(VeWindow::Data* pkWindow) noexcept
{

}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowLeave(VeWindow::Data* pkWindow) noexcept
{

}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowFocusGained(VeWindow::Data* pkWindow) noexcept
{

}
//--------------------------------------------------------------------------
void VeVideoDevice::OnWindowFocusLost(VeWindow::Data* pkWindow) noexcept
{

}
//--------------------------------------------------------------------------
