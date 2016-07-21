////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Event
//  File name:   VeMouse.cpp
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
VeCursor::VeCursor() noexcept
{
	m_kData.m_kNode._Content = &m_kData;
}
//--------------------------------------------------------------------------
VeCursor::~VeCursor() noexcept
{

}
//--------------------------------------------------------------------------
VeCursor* VeCursor::Cast(VeCursor::Data* pkData) noexcept
{
	return pkData ? vtd::member_cast(&VeCursor::m_kData, pkData) : nullptr;
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
	int32_t x, int32_t y) noexcept
{
	if (pkWindow == nullptr)
		pkWindow = m_pkFocus;

	if (pkWindow == nullptr)
		return;

	_Warp(pkWindow, x, y);
}
//--------------------------------------------------------------------------
void VeMouse::SetRelativeMode(bool bEnabled) noexcept
{
	auto spKeyboard = Venus3D::Ref().GetKeyboard();
	VeWindow::Data* pkFocusWindow = spKeyboard ? spKeyboard->m_pkFocus : nullptr;

	if ((bEnabled ? 1u : 0u) == m_bRelativeMode)
	{
		return;
	}

	if (bEnabled && pkFocusWindow)
	{
		SetFocus(pkFocusWindow);
		WarpInWindow(pkFocusWindow, pkFocusWindow->w >> 1, pkFocusWindow->h >> 1);
	}

	if (!bEnabled && m_bRelativeModeWarp)
	{
		m_bRelativeModeWarp = VE_FALSE;
	}
	else if (bEnabled)
	{
		m_bRelativeModeWarp = VE_TRUE;
	}
	else if (!_SetRelativeMode(bEnabled))
	{
		if (bEnabled)
		{
			m_bRelativeModeWarp = VE_TRUE;
		}
	}
	m_bRelativeMode = bEnabled;

	if (m_pkFocus)
	{
		assert(Venus3D::Ref().GetVideoDevice());
		Venus3D::Ref().GetVideoDevice()->UpdateWindowGrab(m_pkFocus);

		if (!bEnabled)
		{
			WarpInWindow(m_pkFocus, m_i32PosX, m_i32PosY);
		}
	}

	assert(Venus3D::Ref().GetEventQueue());
	Venus3D::Ref().GetEventQueue()->FlushEvent(VE_MOUSEMOTION);

	SetCursor(nullptr);
}
//--------------------------------------------------------------------------
void VeMouse::ShowCursor(bool bToggle) noexcept
{
	VE_BOOL bShown = m_bCursorShown;
	if (bToggle)
	{
		m_bCursorShown = VE_TRUE;
	}
	else
	{
		m_bCursorShown = VE_FALSE;
	}

	if (m_bCursorShown != bShown)
	{
		SetCursor(nullptr);
	}
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
	SetRelativeMode(false);
	ShowCursor(true);

	m_kCursorList.clear();

	if (m_spDefCursor)
	{
		_FreeCursor(&(m_spDefCursor->m_kData));
	}

	m_kClickStateList.clear();

	m_u32MouseID = 0;
	m_pkFocus = nullptr;
	m_i32PosX = 0;
	m_i32PosY = 0;
	m_i32DeltaX = 0;
	m_i32DeltaY = 0;
	m_i32LastX = 0;
	m_i32LastY = 0;
	m_u32ButtonState = 0;
	m_u32DoubleClickTime = 500;
	m_i32DoubleClickRadius = 1;
	m_bRelativeMode = VE_FALSE;
	m_bRelativeModeWarp = VE_FALSE;
	m_spDefCursor = nullptr;
	m_spCurCursor = nullptr;
	m_bCursorShown = VE_FALSE;

	_Term();
}
//--------------------------------------------------------------------------
void VeMouse::SetDoubleClickTime(uint32_t u32Interval) noexcept
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
void VeMouse::SetFocus(VeWindow::Data* pkWindow) noexcept
{
	assert(Venus3D::Ref().GetVideoDevice());

	if (m_pkFocus == pkWindow) return;

	if (m_pkFocus)
	{
		Venus3D::Ref().GetVideoDevice()->SendWindowEvent(m_pkFocus, VE_WINDOWEVENT_LEAVE, 0, 0);
	}

	m_pkFocus = pkWindow;

	if (m_pkFocus)
	{
		Venus3D::Ref().GetVideoDevice()->SendWindowEvent(m_pkFocus, VE_WINDOWEVENT_ENTER, 0, 0);
	}

	SetCursor(nullptr);
}
//--------------------------------------------------------------------------
bool VeMouse::UpdateFocus(VeWindow::Data* pkWindow, int32_t x, int32_t y,
	uint32_t) noexcept
{
	assert(pkWindow);
	bool inWindow;
	int32_t w = pkWindow->w;
	int32_t h = pkWindow->h;
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

			PrivateSendMotion(pkWindow, m_u32MouseID, 0, x, y);
			SetFocus(nullptr);
		}
		return false;
	}

	if (pkWindow != m_pkFocus)
	{

		SetFocus(pkWindow);
		PrivateSendMotion(pkWindow, m_u32MouseID, 0, x, y);
	}
	return true;
}
//--------------------------------------------------------------------------
void VeMouse::PrivateSendMotion(VeWindow::Data* pkWindow,
	VeMouseID, int32_t i32Relative, int32_t x, int32_t y) noexcept
{
	int32_t xrel;
	int32_t yrel;
	int32_t x_max = 0, y_max = 0;

	if (m_bRelativeModeWarp)
	{
		int32_t center_x = 0, center_y = 0;
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

	m_i32DeltaX += xrel;
	m_i32DeltaY += yrel;

	if (m_bCursorShown && !m_bRelativeMode && m_spCurCursor)
	{
		_MoveCursor(&(m_spCurCursor->m_kData));
	}

	assert(Venus3D::Ref().GetEventQueue());
	
	if (Venus3D::Ref().GetEventQueue()->IsEventTypeEnable(VE_MOUSEMOTION))
	{
		VeEvent* pkEvent = Venus3D::Ref().GetEventQueue()->AddEvent();
		pkEvent->m_kMotion.m_u32Type = VE_MOUSEMOTION;
		pkEvent->m_kMotion.m_u32TimeStamp = VeEventQueue::GetTicks();
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
VeMouseClickState* VeMouse::GetClickState(uint8_t u8Button) noexcept
{
	if (u8Button >= m_kClickStateList.size())
	{
		m_kClickStateList.resize(u8Button + 1);
	}
	return &m_kClickStateList[u8Button];
}
//--------------------------------------------------------------------------
VeCursor::Data* VeMouse::GetCursorData(VeCursor* pkCursor) noexcept
{
	return pkCursor ? (&pkCursor->m_kData) : nullptr;
}
//--------------------------------------------------------------------------
void VeMouse::SendMotion(VeWindow::Data* pkWindow,
	VeMouseID u32MouseID, int32_t i32Relative,
	int32_t x, int32_t y) noexcept
{
	if (pkWindow && !i32Relative)
	{
		if (!UpdateFocus(pkWindow, x, y, m_u32ButtonState))
		{
			return;
		}
	}

	PrivateSendMotion(pkWindow, u32MouseID, i32Relative, x, y);
}
//--------------------------------------------------------------------------
void VeMouse::SendButton(VeWindow::Data* pkWindow,
	VeMouseID, uint8_t u8State, uint8_t u8Button) noexcept
{
	uint32_t type;
	uint32_t buttonstate = m_u32ButtonState;
	VeMouseClickState* clickstate = GetClickState(u8Button);
	uint8_t click_count;

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
		UpdateFocus(pkWindow, m_i32PosX, m_i32PosY, buttonstate);
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
			uint32_t now = VeEventQueue::GetTicks();

			if (VE_TICKS_PASSED(now, clickstate->m_u32LastTimeStamp + m_u32DoubleClickTime)
				|| vtd::abs(m_i32PosX - clickstate->m_i32LastX) > m_i32DoubleClickRadius
				|| vtd::abs(m_i32PosY - clickstate->m_i32LastY) > m_i32DoubleClickRadius)
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

	assert(Venus3D::Ref().GetEventQueue());
	if (Venus3D::Ref().GetEventQueue()->IsEventTypeEnable(type))
	{
		VeEvent* pkEvent = Venus3D::Ref().GetEventQueue()->AddEvent();
		pkEvent->m_kButton.m_u32Type = type;
		pkEvent->m_kButton.m_u32TimeStamp = VeEventQueue::GetTicks();
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
		UpdateFocus(pkWindow, m_i32PosX, m_i32PosY, buttonstate);
	}
}
//--------------------------------------------------------------------------
void VeMouse::SendWheel(VeWindow::Data* pkWindow, VeMouseID,
	int32_t x, int32_t y) noexcept
{
	if (pkWindow)
	{
		SetFocus(pkWindow);
	}

	if (!x && !y)
	{
		return;
	}

	assert(Venus3D::Ref().GetEventQueue());
	if (Venus3D::Ref().GetEventQueue()->IsEventTypeEnable(VE_MOUSEWHEEL))
	{
		VeEvent* pkEvent = Venus3D::Ref().GetEventQueue()->AddEvent();
		pkEvent->m_kWheel.m_u32Type = VE_MOUSEWHEEL;
		pkEvent->m_kWheel.m_u32TimeStamp = VeEventQueue::GetTicks();
		pkEvent->m_kWheel.m_u32WindowID = m_pkFocus ? m_pkFocus->m_u32Id : 0;
		pkEvent->m_kWheel.m_u32Which = m_u32MouseID;
		pkEvent->m_kWheel.x = x;
		pkEvent->m_kWheel.y = y;
	}
}
//--------------------------------------------------------------------------
void VeMouse::_Warp(VeWindow::Data* pkWindow,
	int32_t x, int32_t y) noexcept
{
	SendMotion(pkWindow, m_u32MouseID, 0, x, y);
}
//--------------------------------------------------------------------------
