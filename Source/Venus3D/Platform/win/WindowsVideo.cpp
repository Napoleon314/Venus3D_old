////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Video
//  File name:   WindowsVideo.cpp
//  Created:     2016/07/24 by Albert
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

//--------------------------------------------------------------------------
#ifdef THROW
#	undef THROW
#endif
#define THROW(...) VE_THROW("WindowsVideo Error", __VA_ARGS__)
//--------------------------------------------------------------------------
VeRTTIImpl(WindowsVideo, VeDesktopVideo);
//--------------------------------------------------------------------------
WindowsVideo::WindowsVideo(const VeInitData& kInitData) noexcept
	: VeDesktopVideo("WINAPI")
{
	m_hInstance = kInitData.m_hInstance ? kInitData.m_hInstance : GetModuleHandle(NULL);
	m_hPrevInstance = kInitData.m_hPrevInstance;
	m_i32CmdShow = kInitData.m_i32CmdShow;
}
//--------------------------------------------------------------------------
WindowsVideo::~WindowsVideo() noexcept
{
	Term();
}
//--------------------------------------------------------------------------
void WindowsVideo::Init()
{
	if (!m_wstrClassName)
	{
		m_wstrClassName = UTF8ToWSTR(Venus3D::Ref().GetPakName());
		WNDCLASSEXW windowClass = { 0 };
		windowClass.cbSize = sizeof(WNDCLASSEXW);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = WindowProc;
		windowClass.hInstance = GetModuleHandle(NULL);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.lpszClassName = m_wstrClassName;
		if (!RegisterClassExW(&windowClass))
		{
			THROW("Couldn't register application class");
		}
	}
}
//--------------------------------------------------------------------------
void WindowsVideo::Term()
{
	if (m_wstrClassName)
	{
		if (!UnregisterClassW(m_wstrClassName, m_hInstance))
		{
			THROW("Couldn't unregister application class");
		}
		VeFree(m_wstrClassName);
		m_wstrClassName = nullptr;
	}
}
//--------------------------------------------------------------------------
void WindowsVideo::PumpEvents() noexcept
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
//--------------------------------------------------------------------------
int32_t WindowsVideo::MessageBoxSync(const char* pcCaption,
	const char* pcText, uint32_t u32Flags) noexcept
{
	LPWSTR lpwstrCaption = UTF8ToWSTR(pcCaption);
	LPWSTR lpwstrText = UTF8ToWSTR(pcText);
	int32_t i32Ret = MessageBoxW(NULL, lpwstrText, lpwstrCaption, u32Flags);
	VeFree(lpwstrCaption);
	VeFree(lpwstrText);
	return i32Ret;
}
//--------------------------------------------------------------------------
VeDesktopWindowPtr WindowsVideo::Create(const char* pcTitle, int32_t x,
	int32_t y, int32_t w, int32_t h, uint32_t u32Flags) noexcept
{
	WindowsWindowPtr spWindow = VE_NEW WindowsWindow();
	VE_TRY_CALL(spWindow->Init(*this, pcTitle, x, y, w, h, u32Flags));
	return spWindow;
}
//--------------------------------------------------------------------------
LPWSTR WindowsVideo::UTF8ToWSTR(const char* pcStr) noexcept
{
	VE_ASSERT(pcStr);
	int32_t i32Num = MultiByteToWideChar(CP_UTF8, 0, pcStr, -1, nullptr, 0);
	VE_ASSERT(i32Num >= 0);
	LPWSTR lpwstrBuf = VeAlloc(WCHAR, i32Num + 1);
	lpwstrBuf[i32Num] = 0;
	MultiByteToWideChar(CP_UTF8, 0, pcStr, -1, lpwstrBuf, i32Num);
	return lpwstrBuf;
}
//--------------------------------------------------------------------------
LRESULT WindowsVideo::WindowProc(HWND hwnd, UINT msg, WPARAM wParam,
	LPARAM lParam) noexcept
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------------
VeVideoPtr CreateWindowsVideo(const VeInitData& kInitData) noexcept
{
	return VE_NEW WindowsVideo(kInitData);
}
//--------------------------------------------------------------------------
