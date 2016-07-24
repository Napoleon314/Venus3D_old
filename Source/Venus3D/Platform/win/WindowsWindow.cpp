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

	//x = GetSystemMetrics(SM_CMONITORS);

	m_hHandle = CreateWindowW(
		kVideo.m_wstrClassName,
		lpwstrTitle,
		dwStyle,
		flag_x == 1 ? CW_USEDEFAULT : windowRect.left,
		flag_y == 1 ? CW_USEDEFAULT : windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		kVideo.m_hInstance,
		this);

	VeFree(lpwstrTitle);

	if (!m_hHandle)
	{
		THROW("Couldn't create window");
	}

	ShowWindow(m_hHandle, kVideo.m_i32CmdShow);

	kVideo.m_kWindowList.attach_back(m_kNode);
}
//--------------------------------------------------------------------------
void WindowsWindow::Term()
{
	if (m_hHandle)
	{
		if (!DestroyWindow(m_hHandle))
		{
			THROW("Couldn't destory window");
		}
		m_hHandle = nullptr;
	}
}
//--------------------------------------------------------------------------
#define STYLE_BASIC         (WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define STYLE_FULLSCREEN    (WS_POPUP)
#define STYLE_BORDERLESS    (WS_POPUP)
#define STYLE_NORMAL        (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)
#define STYLE_RESIZABLE     (WS_THICKFRAME | WS_MAXIMIZEBOX)
#define STYLE_MASK          (STYLE_FULLSCREEN | STYLE_BORDERLESS | STYLE_NORMAL | STYLE_RESIZABLE)
//--------------------------------------------------------------------------
DWORD WindowsWindow::FlagsToWindowStyle(uint32_t u32Flags) noexcept
{
	DWORD dwStyle = 0;
	if (VE_MASK_HAS_ALL(u32Flags, VE_WINDOW_BORDERLESS))
	{
		dwStyle |= STYLE_BORDERLESS;
	}
	else
	{
		dwStyle |= STYLE_NORMAL;
		if (u32Flags & VE_WINDOW_RESIZABLE)
		{
			dwStyle |= STYLE_RESIZABLE;
		}
	}
	return dwStyle;
}
//--------------------------------------------------------------------------
