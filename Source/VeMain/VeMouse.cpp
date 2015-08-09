////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMouse.cpp
//  Created:     2015/08/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeCursor::VeCursor() noexcept
{
	m_kData.m_kNode.m_Content = &m_kData;
}
//--------------------------------------------------------------------------
VeCursor::~VeCursor() noexcept
{

}
//--------------------------------------------------------------------------
VeCursor* VeCursor::Cast(VeCursor::Data* pkData) noexcept
{
	return pkData ? VeMemberCast(&VeCursor::m_kData, pkData) : nullptr;
}
//--------------------------------------------------------------------------
VeMouse::VeMouse() noexcept
{

}
//--------------------------------------------------------------------------
VeMouse::~VeMouse() noexcept
{

}
//--------------------------------------------------------------------------
void VeMouse::SetCursor(VeCursor::Data* pkCursor) noexcept
{
	if (pkCursor)
	{
		if (pkCursor != &(m_spDefCursor->m_kData))
		{
			if (!pkCursor->m_kNode.is_attach(m_kCursorList))
			{
				return;
			}
		}
		m_spCurCursor = VeCursor::Cast(pkCursor);
	}
	else
	{
		if (m_pkFocus)
		{
			pkCursor = &(m_spCurCursor->m_kData);
		}
		else
		{
			pkCursor = &(m_spDefCursor->m_kData);
		}
	}

	if (pkCursor && m_bCursorShown && !m_bRelativeMode)
	{
		_ShowCursor(pkCursor);
	}
	else
	{
		_ShowCursor(nullptr);
	}
}
//--------------------------------------------------------------------------
void VeMouse::WarpInWindow(VeWindow::Data* pkWindow,
	VeInt32 x, VeInt32 y) noexcept
{
	if (pkWindow == nullptr)
		pkWindow = m_pkFocus;

	if (pkWindow == nullptr)
		return;

	_WarpMouse(pkWindow, x, y);
}
//--------------------------------------------------------------------------
void VeMouse::Init() noexcept
{
	m_bCursorShown = VE_TRUE;
	_Init();
}
//--------------------------------------------------------------------------
void VeMouse::Term() noexcept
{
	

	_Term();
}
//--------------------------------------------------------------------------
void VeMouse::SetDoubleClickTime(VeUInt32 u32Interval) noexcept
{
	m_u32DoubleClickTime = u32Interval;
}
//--------------------------------------------------------------------------
void VeMouse::SetDefaultCursor(VeCursor::Data* pkCursor) noexcept
{
	m_spDefCursor = VeCursor::Cast(pkCursor);
	if (!m_spCurCursor)
	{
		SetCursor(pkCursor);
	}
}
//--------------------------------------------------------------------------
void VeMouse::SetMouseFocus(VeWindow::Data* pkWindow) noexcept
{
	VE_ASSERT(ve_video_ptr);
	if (m_pkFocus == pkWindow) return;

	if (m_pkFocus)
	{
		ve_video_ptr->SendWindowEvent(m_pkFocus, VE_WINDOWEVENT_LEAVE, 0, 0);
	}

	m_pkFocus = pkWindow;

	if (m_pkFocus)
	{
		ve_video_ptr->SendWindowEvent(m_pkFocus, VE_WINDOWEVENT_ENTER, 0, 0);
	}

	SetCursor(nullptr);
}
//--------------------------------------------------------------------------
bool VeMouse::UpdateMouseFocus(VeWindow::Data* pkWindow, VeInt32 x, VeInt32 y,
	VeUInt32 u32ButtonState) noexcept
{
	VE_ASSERT(pkWindow);
	bool inWindow;
	VeInt32 w = pkWindow->w;
	VeInt32 h = pkWindow->h;
	if (x < 0 || y < 0 || x >= w || y >= h)
	{
		inWindow = false;
	}
	else
	{
		inWindow = true;
	}

	if (!inWindow)
	{
		if (pkWindow == m_pkFocus)
		{

			PrivateSendMouseMotion(pkWindow, m_u32MouseID, 0, x, y);
			SetMouseFocus(nullptr);
		}
		return false;
	}

	if (pkWindow != m_pkFocus)
	{

		SetMouseFocus(pkWindow);
		PrivateSendMouseMotion(pkWindow, m_u32MouseID, 0, x, y);
	}
	return true;
}
//--------------------------------------------------------------------------
void VeMouse::PrivateSendMouseMotion(VeWindow::Data* pkWindow,
	VeMouseID u32mouseID, VeInt32 i32Relative,
	VeInt32 x, VeInt32 y) noexcept
{
	VeInt32 xrel;
	VeInt32 yrel;
	VeInt32 x_max = 0, y_max = 0;

	if (m_bRelativeModeWarp)
	{
		VeInt32 center_x = 0, center_y = 0;
		center_x = pkWindow->w >> 1;
		center_y = pkWindow->h >> 1;
		if (x == center_x && y == center_y)
		{
			m_i32LastX = center_x;
			m_i32LastY = center_y;
			return;
		}
		WarpInWindow(pkWindow, center_x, center_y);
	}

	if (i32Relative)
	{
		xrel = x;
		yrel = y;
		x = (m_i32LastX + xrel);
		y = (m_i32LastY + yrel);
	}
	else
	{
		xrel = x - m_i32LastX;
		yrel = y - m_i32LastY;
	}

	if (!xrel && !yrel)
	{
		return;
	}

	if (!m_bRelativeMode)
	{
		m_i32PosX = x;
		m_i32PosY = y;
	}
	else
	{
		m_i32PosX += xrel;
		m_i32PosY += yrel;
	}

	x_max = m_pkFocus->w - 1;
	y_max = m_pkFocus->h - 1;

	if (m_i32PosX > x_max)
	{
		m_i32PosX = x_max;
	}
	if (m_i32PosX < 0)
	{
		m_i32PosX = 0;
	}

	if (m_i32PosY > y_max)
	{
		m_i32PosY = y_max;
	}
	if (m_i32PosY < 0)
	{
		m_i32PosY = 0;
	}

	m_i32XDelta += xrel;
	m_i32YDelta += yrel;

	if (m_bCursorShown && !m_bRelativeMode && m_spCurCursor)
	{
		_MoveCursor(&(m_spCurCursor->m_kData));
	}

	VE_ASSERT(ve_event_queue_ptr);
	
	if (ve_event_queue_ptr->IsEventTypeEnable(VE_MOUSEMOTION))
	{
		VeEvent* pkEvent = ve_event_queue_ptr->AddEvent();
		pkEvent->m_kMotion.m_u32Type = VE_MOUSEMOTION;
		pkEvent->m_kMotion.m_u32WindowID = m_pkFocus ? m_pkFocus->m_u32Id : 0;
		pkEvent->m_kMotion.m_u32Which = m_u32MouseID;
		pkEvent->m_kMotion.m_u32State = m_u32ButtonState;
		pkEvent->m_kMotion.x = m_i32PosX;
		pkEvent->m_kMotion.y = m_i32PosY;
		pkEvent->m_kMotion.xrel = xrel;
		pkEvent->m_kMotion.yrel = yrel;
	}
	
	m_i32LastX = x;
	m_i32LastY = y;
}
//--------------------------------------------------------------------------
VeMouseClickState* VeMouse::GetMouseClickState(VeUInt8 u8Button) noexcept
{
	if (u8Button >= m_kClickStateList.size())
	{
		m_kClickStateList.resize(u8Button + 1);
	}
	return &m_kClickStateList[u8Button];
}
//--------------------------------------------------------------------------
void VeMouse::SendMouseMotion(VeWindow::Data* pkWindow,
	VeMouseID u32MouseID, VeInt32 i32Relative,
	VeInt32 x, VeInt32 y) noexcept
{
	if (pkWindow && !i32Relative)
	{
		if (!UpdateMouseFocus(pkWindow, x, y, m_u32ButtonState))
		{
			return;
		}
	}

	PrivateSendMouseMotion(pkWindow, u32MouseID, i32Relative, x, y);
}
//--------------------------------------------------------------------------
void VeMouse::SendMouseButton(VeWindow::Data* pkWindow,
	VeMouseID u32MouseID, VeUInt8 u8State, VeUInt8 u8Button) noexcept
{
	VeUInt32 type;
	VeUInt32 buttonstate = m_u32ButtonState;
	VeMouseClickState* clickstate = GetMouseClickState(u8Button);
	VeUInt8 click_count;

	switch (u8State)
	{
	case VE_PRESSED:
		type = VE_MOUSEBUTTONDOWN;
		buttonstate |= VE_BUTTON(u8Button);
		break;
	case VE_RELEASED:
		type = VE_MOUSEBUTTONUP;
		buttonstate &= ~VE_BUTTON(u8Button);
		break;
	default:
		return;
	}

	if (pkWindow && u8State == VE_PRESSED)
	{
		UpdateMouseFocus(pkWindow, m_i32PosX, m_i32PosY, buttonstate);
	}

	if (buttonstate == m_u32ButtonState)
	{
		return;
	}

	m_u32ButtonState = buttonstate;

	if (clickstate)
	{
		if (u8State == VE_PRESSED)
		{
			VeUInt32 now = VeEventQueue::GetTicks();

			if (VE_TICKS_PASSED(now, clickstate->m_u32LastTimeStamp + m_u32DoubleClickTime)
				|| VeFabsf(m_i32PosX - clickstate->m_i32LastX) > m_u32DoubleClickRadius
				|| VeFabsf(m_i32PosY - clickstate->m_i32LastY) > m_u32DoubleClickRadius)
			{
				clickstate->m_u8ClickCount = 0;
			}
			clickstate->m_u32LastTimeStamp = now;
			clickstate->m_i32LastX = m_i32PosX;
			clickstate->m_i32LastY = m_i32PosY;
			if (clickstate->m_u8ClickCount < 255)
			{
				++clickstate->m_u8ClickCount;
			}
		}
		click_count = clickstate->m_u8ClickCount;
	}
	else
	{
		click_count = 1;
	}

	VE_ASSERT(ve_event_queue_ptr);
	if (ve_event_queue_ptr->IsEventTypeEnable(type))
	{
		VeEvent* pkEvent = ve_event_queue_ptr->AddEvent();
		pkEvent->m_u32Type = type;
		pkEvent->m_kButton.m_u32WindowID = m_pkFocus ? m_pkFocus->m_u32Id : 0;
		pkEvent->m_kButton.m_u32Which = m_u32MouseID;
		pkEvent->m_kButton.m_u8State = u8State;
		pkEvent->m_kButton.m_u8Button = u8Button;
		pkEvent->m_kButton.m_u8Clicks = click_count;
		pkEvent->m_kButton.x = m_i32PosX;
		pkEvent->m_kButton.y = m_i32PosY;
	}
	
	if (pkWindow && u8State == VE_RELEASED)
	{
		UpdateMouseFocus(pkWindow, m_i32PosX, m_i32PosY, buttonstate);
	}
}
//--------------------------------------------------------------------------
void VeMouse::_WarpMouse(VeWindow::Data* pkWindow,
	VeInt32 x, VeInt32 y) noexcept
{
	SendMouseMotion(pkWindow, m_u32MouseID, 0, x, y);
}
//--------------------------------------------------------------------------
