////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Video
//  File name:   WindowsWindow.cpp
//  Created:     2016/07/25 by Albert
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
#include "WindowsWindow.h"
#include "WindowsVideo.h"

#pragma warning(disable: 4100)

//--------------------------------------------------------------------------
#ifdef THROW
#	undef THROW
#endif
#define THROW(...) VE_THROW("WindowsWindow Error", __VA_ARGS__)
//--------------------------------------------------------------------------
VeRTTIImpl(WindowsWindow, VeDesktopWindow);
//--------------------------------------------------------------------------
WindowsWindow::~WindowsWindow()
{
	VE_TRY_CALL(Term());
}
//--------------------------------------------------------------------------
void* WindowsWindow::GetNativeHandle() noexcept
{
	return m_hHandle;
}
//--------------------------------------------------------------------------
void WindowsWindow::Init(WindowsVideo& kVideo, const char* pcTitle,
	int32_t x, int32_t y, int32_t w, int32_t h, uint32_t u32Flags)
{
	Term();
	LPWSTR lpwstrTitle = WindowsVideo::UTF8ToWSTR(pcTitle);
	DWORD dwStyle = FlagsToWindowStyle(u32Flags);

	int32_t flag_x = x >> 28;
	int32_t flag_y = y >> 28;
	x = flag_x ? 0 : x;
	y = flag_y ? 0 : y;
	RECT windowRect = { x, y, w, h };
	AdjustWindowRect(&windowRect, dwStyle, FALSE);

	VE_ASSERT(w > 0 && w < UINT16_MAX && h > 0 && h < UINT16_MAX);
	m_u16Width = (uint16_t)w;
	m_u16Height = (uint16_t)h;

	w = windowRect.right - windowRect.left;
	h = windowRect.bottom - windowRect.top;

	switch (flag_x)
	{
	case 1:
		x = CW_USEDEFAULT;
		break;
	case 2:
		x = (GetSystemMetrics(SM_CXSCREEN) - w) >> 1;
		break;
	default:
		x = windowRect.left;
		break;
	}
	switch (flag_y)
	{
	case 1:
		y = CW_USEDEFAULT;
		break;
	case 2:
		y = (GetSystemMetrics(SM_CYSCREEN) - h) >> 1;
		break;
	default:
		y = windowRect.top;
		break;
	}

	x = x == CW_USEDEFAULT ? x : vtd::max(x, 0);
	y = y == CW_USEDEFAULT ? y : vtd::max(y, 0);

	m_hHandle = CreateWindowW(kVideo.m_wstrClassName, lpwstrTitle, dwStyle,
		x, y, w, h, nullptr, nullptr, kVideo.m_hInstance, this);

	VeFree(lpwstrTitle);

	if (!m_hHandle)
	{
		m_u16Width = 0;
		m_u16Height = 0;
		THROW("Couldn't create window");
	}

	if (VE_MASK_HAS_ALL(u32Flags, VE_WINDOW_SHOWN))
	{
		int32_t i32Show = SW_SHOW;
		if (VE_MASK_HAS_ALL(dwStyle, WS_THICKFRAME))
		{
			switch (u32Flags & 0xF)
			{
			case VE_WINDOW_MINIMIZED:
				i32Show = SW_SHOWMINIMIZED;
				break;
			case VE_WINDOW_MAXIMIZED:
				i32Show = SW_SHOWMAXIMIZED;
				break;
			case VE_WINDOW_DEPSTARTUP:
				i32Show = kVideo.m_i32CmdShow;
				break;
			default:
				break;
			}
		}
		ShowWindow(m_hHandle, i32Show);
	}

	m_u32Flags = u32Flags & (VE_WINDOW_ALLOW_HIGHDPI | VE_WINDOW_FOREIGN);
	UpdateFlags();

	kVideo.m_kWindowList.attach_back(m_kNode);
}
//--------------------------------------------------------------------------
void WindowsWindow::Term()
{
	if (m_hHandle)
	{
		m_kNode.detach();
		m_spParent = nullptr;
		for (auto child : m_kChildList)
		{
			WindowsWindow* pkChild = VeDynamicCast(WindowsWindow, child);
			VE_ASSERT(pkChild);
			pkChild->Term();
		}
		VE_ASSERT(m_kChildList.empty());
		if (!VE_MASK_HAS_ALL(m_u32Flags, VE_WINDOW_FOREIGN))
		{
			if (!DestroyWindow(m_hHandle))
			{
				THROW("Couldn't destory window");
			}
		}
		m_hHandle = nullptr;
		UpdateFlags();
	}
	VE_ASSERT(!m_kNode.is_attach());
}
//--------------------------------------------------------------------------
void WindowsWindow::Show() noexcept
{
	if (IsValid() && IsHidden())
	{
		VE_ASSERT_EQ(ShowWindow(m_hHandle, SW_SHOW), 0);
		UpdateFlags();
	}
}
//--------------------------------------------------------------------------
void WindowsWindow::Hide() noexcept
{
	if (IsValid() && IsVisible())
	{
		VE_ASSERT_NE(ShowWindow(m_hHandle, SW_HIDE), 0);
		UpdateFlags();
	}
}
//--------------------------------------------------------------------------
void WindowsWindow::SetTitle(const char* pcTitle) noexcept
{
	int32_t i32Num = MultiByteToWideChar(CP_UTF8, 0, pcTitle, -1, nullptr, 0);
	VE_ASSERT(i32Num >= 0);
	LPWSTR lpwstrBuf = VeStackAlloc(WCHAR, i32Num + 1);
	lpwstrBuf[i32Num] = 0;
	MultiByteToWideChar(CP_UTF8, 0, pcTitle, -1, lpwstrBuf, i32Num);
	SetWindowTextW(m_hHandle, lpwstrBuf);
	VeStackFree(lpwstrBuf);
}
//--------------------------------------------------------------------------
void WindowsWindow::UpdateFlags() noexcept
{
	if (m_hHandle)
	{
		m_u32Flags &= (VE_WINDOW_ALLOW_HIGHDPI | VE_WINDOW_FOREIGN);
		LONG dwStyle = GetWindowLongW(m_hHandle, GWL_STYLE);
		VE_MASK_CONDITION(dwStyle, WS_MINIMIZE, VE_MASK_ADD(m_u32Flags, VE_WINDOW_MINIMIZED));
		VE_MASK_CONDITION(dwStyle, WS_MAXIMIZE, VE_MASK_ADD(m_u32Flags, VE_WINDOW_MAXIMIZED));
		VE_MASK_CONDITION(dwStyle, WS_VISIBLE, VE_MASK_ADD(m_u32Flags, VE_WINDOW_SHOWN));
		VE_MASK_CONDITION(dwStyle, WS_POPUP, VE_MASK_ADD(m_u32Flags, VE_WINDOW_BORDERLESS));
		VE_MASK_CONDITION(dwStyle, WS_THICKFRAME, VE_MASK_ADD(m_u32Flags, VE_WINDOW_RESIZABLE));
		VE_MASK_ADD(m_u32Flags, VE_WINDOW_VALID);
	}
	else
	{
		m_u32Flags = 0;
	}
}
//--------------------------------------------------------------------------
LRESULT WindowsWindow::WindowProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_DESTROY:
		if(m_kNode.is_attach())
		{
			VE_TRY_CALL(Term());
		}
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
//--------------------------------------------------------------------------
DWORD WindowsWindow::FlagsToWindowStyle(uint32_t u32Flags) noexcept
{
	DWORD dwStyle = 0;
	if (VE_MASK_HAS_ALL(u32Flags, VE_WINDOW_BORDERLESS))
	{
		dwStyle |= (WS_POPUP);
	}
	else
	{
		dwStyle |= (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
		if (u32Flags & VE_WINDOW_RESIZABLE)
		{
			dwStyle |= (WS_THICKFRAME | WS_MAXIMIZEBOX);
		}
	}
	return dwStyle;
}
//--------------------------------------------------------------------------
