////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   WindowsVideo.cpp
//  Created:     2015/07/27 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "../VePowerPch.h"
#include "WindowsVideo.h"
#include <shellapi.h>

//--------------------------------------------------------------------------
VeVideoDevicePtr CreateVideoDevice() noexcept
{
	return VE_NEW WindowsVideoDevice();
}
//--------------------------------------------------------------------------
WindowsVideoDevice::WindowsVideoDevice() noexcept
{
	m_kName = "windows";
	m_kDesc = "Venus3D Windows Video Driver";
	VE_ASSERT_EQ(RegisterApp(nullptr, 0, nullptr), true);
}
//--------------------------------------------------------------------------
WindowsVideoDevice::~WindowsVideoDevice() noexcept
{
	UnregisterApp();
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::Init() noexcept
{
	InitModes();
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::Term() noexcept
{
	TermModes();
}
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) noexcept
{
	return CallWindowProc(DefWindowProc, hwnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::RegisterApp(const VeChar8* pcName,
	VeUInt32 u32Style, HINSTANCE hInst) noexcept
{
	if (m_i32AppRegistered)
	{
		++m_i32AppRegistered;
		return true;
	}

	if (!pcName)
	{
		pcName = ve_sys.GetPakName();
#if defined(CS_BYTEALIGNCLIENT) || defined(CS_OWNDC)
		m_u32AppStyle = (CS_BYTEALIGNCLIENT | CS_OWNDC);
#endif
	}

	VE_ASSERT(pcName);
	m_kAppName = pcName;
	m_u32AppStyle = u32Style;
	m_hInstance = hInst ? hInst : GetModuleHandle(nullptr);

	WNDCLASSA kClass;
	kClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	kClass.hIcon = nullptr;
	kClass.lpszMenuName = nullptr;
	kClass.lpszClassName = m_kAppName;
	kClass.hbrBackground = nullptr;
	kClass.hInstance = m_hInstance;
	kClass.style = m_u32AppStyle;
	kClass.lpfnWndProc = WindowProc;
	kClass.cbWndExtra = 0;
	kClass.cbClsExtra = 0;
	if (!RegisterClassA(&kClass))
	{
		VeDebugOutputCore("Couldn't register application class");
		return false;
	}
	m_i32AppRegistered = 1;
	return true;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::UnregisterApp() noexcept
{
	if (!m_i32AppRegistered)
		return;
	--m_i32AppRegistered;
	if (m_i32AppRegistered <= 0)
	{
		WNDCLASSA kClass;
		if (GetClassInfoA(m_hInstance, m_kAppName, &kClass))
		{
			UnregisterClassA(m_kAppName, m_hInstance);
		}
		m_i32AppRegistered = 0;
		m_kAppName = "";
		m_u32AppStyle = 0;
		m_hInstance = nullptr;
	}
}
//--------------------------------------------------------------------------
static bool GetDisplayMode(LPCSTR deviceName, DWORD index,
	VeDisplayMode* mode) noexcept
{
	VeDisplayModeData* data;
	DEVMODEA devmode;
	HDC hdc;

	devmode.dmSize = sizeof(devmode);
	devmode.dmDriverExtra = 0;
	if (!EnumDisplaySettingsA(deviceName, index, &devmode))
	{
		return false;
	}

	data = VE_NEW VeDisplayModeData;
	VE_ASSERT(data);
	data->DeviceMode = devmode;
	data->DeviceMode.dmFields =
		(DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY |
			DM_DISPLAYFLAGS);
	data->ScaleX = 1.0f;
	data->ScaleY = 1.0f;

	mode->m_u32Format = VE_PIXELFORMAT_UNKNOWN;
	mode->m_i32Width = devmode.dmPelsWidth;
	mode->m_i32Height = devmode.dmPelsHeight;
	mode->m_i32RefreshRate = devmode.dmDisplayFrequency;
	mode->m_spDriverData = data;

	if (index == ENUM_CURRENT_SETTINGS
		&& (hdc = CreateDCA(deviceName, nullptr, nullptr, nullptr)) != nullptr)
	{
		char bmi_data[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
		LPBITMAPINFO bmi;
		HBITMAP hbm;
		int logical_width = GetDeviceCaps(hdc, HORZRES);
		int logical_height = GetDeviceCaps(hdc, VERTRES);

		data->ScaleX = (float)logical_width / devmode.dmPelsWidth;
		data->ScaleY = (float)logical_height / devmode.dmPelsHeight;
		mode->m_i32Width = logical_width;
		mode->m_i32Height = logical_height;

		VeZeroMemory(bmi_data, sizeof(bmi_data));
		bmi = (LPBITMAPINFO)bmi_data;
		bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		hbm = CreateCompatibleBitmap(hdc, 1, 1);
		GetDIBits(hdc, hbm, 0, 1, nullptr, bmi, DIB_RGB_COLORS);
		GetDIBits(hdc, hbm, 0, 1, nullptr, bmi, DIB_RGB_COLORS);
		DeleteObject(hbm);
		DeleteDC(hdc);
		if (bmi->bmiHeader.biCompression == BI_BITFIELDS)
		{
			switch (*(VeUInt32 *)bmi->bmiColors)
			{
			case 0x00FF0000:
				mode->m_u32Format = VE_PIXELFORMAT_RGB888;
				break;
			case 0x000000FF:
				mode->m_u32Format = VE_PIXELFORMAT_BGR888;
				break;
			case 0xF800:
				mode->m_u32Format = VE_PIXELFORMAT_RGB565;
				break;
			case 0x7C00:
				mode->m_u32Format = VE_PIXELFORMAT_RGB555;
				break;
			}
		}
		else if (bmi->bmiHeader.biBitCount == 8)
		{
			mode->m_u32Format = VE_PIXELFORMAT_INDEX8;
		}
		else if (bmi->bmiHeader.biBitCount == 4)
		{
			mode->m_u32Format = VE_PIXELFORMAT_INDEX4LSB;
		}
	}
	else
	{
		if ((devmode.dmFields & DM_BITSPERPEL) == DM_BITSPERPEL)
		{
			switch (devmode.dmBitsPerPel)
			{
			case 32:
				mode->m_u32Format = VE_PIXELFORMAT_RGB888;
				break;
			case 24:
				mode->m_u32Format = VE_PIXELFORMAT_RGB24;
				break;
			case 16:
				mode->m_u32Format = VE_PIXELFORMAT_RGB565;
				break;
			case 15:
				mode->m_u32Format = VE_PIXELFORMAT_RGB555;
				break;
			case 8:
				mode->m_u32Format = VE_PIXELFORMAT_INDEX8;
				break;
			case 4:
				mode->m_u32Format = VE_PIXELFORMAT_INDEX4LSB;
				break;
			}
		}
	}
	return true;
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::AddDisplay(const VeChar8* pcDeviceName) noexcept
{
	VeDebugOutputCore("Display: %s\n", pcDeviceName);

	VeDisplayMode kMode;
	
	if (!GetDisplayMode(pcDeviceName, ENUM_CURRENT_SETTINGS, &kMode))
	{
		return false;
	}

	m_kDisplayList.resize(m_kDisplayList.size() + 1);
	VeVideoDisplay& kDisplay = m_kDisplayList.back();

	VeDisplayData* pkDisplayData = VE_NEW VeDisplayData;
	VE_ASSERT(pkDisplayData);
	DISPLAY_DEVICEA device;
	device.cb = sizeof(device);
	if (EnumDisplayDevicesA(pcDeviceName, 0, &device, 0))
	{
		kDisplay.m_kName = device.DeviceString;
	}

	kDisplay.m_kDesktopMode = kMode;
	kDisplay.m_kCurrentMode = kMode;
	kDisplay.m_spDriverData = pkDisplayData;
	return true;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::InitModes() noexcept
{
	VeInt32 pass;
	DWORD i, j, count;
	DISPLAY_DEVICEA device;

	device.cb = sizeof(device);

	for (pass = 0; pass < 2; ++pass)
	{
		for (i = 0;; ++i)
		{
			CHAR DeviceName[32];

			if (!EnumDisplayDevicesA(nullptr, i, &device, 0))
			{
				break;
			}
			if (!(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
			{
				continue;
			}
			if (pass == 0)
			{
				if (!(device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE))
				{
					continue;
				}
			}
			else
			{
				if (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
				{
					continue;
				}
			}
			VeMemoryCopy(DeviceName, device.DeviceName, sizeof(DeviceName));

			VeDebugOutputCore("Device: %s\n", DeviceName);

			count = 0;
			for (j = 0;; ++j)
			{
				if (!EnumDisplayDevicesA(DeviceName, j, &device, 0))
				{
					break;
				}
				if (!(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
				{
					continue;
				}
				if (pass == 0)
				{
					if (!(device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE))
					{
						continue;
					}
				}
				else {
					if (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
					{
						continue;
					}
				}
				count += AddDisplay(device.DeviceName);
			}
			if (count == 0)
			{
				AddDisplay(DeviceName);
			}
		}
	}

	VE_ASSERT(m_kDisplayList.size());
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::TermModes() noexcept
{
	m_kDisplayList.clear();
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::GetDisplayBounds(VeRect* pkRect,
	VeVideoDisplay* pkDisplay) noexcept
{
	VE_ASSERT(pkDisplay);
	VeDisplayModeData* pkData = (VeDisplayModeData*)pkDisplay->m_kCurrentMode.m_spDriverData;

	pkRect->x = (VeInt32)VeCeilf(pkData->DeviceMode.dmPosition.x * pkData->ScaleX);
	pkRect->y = (VeInt32)VeCeilf(pkData->DeviceMode.dmPosition.y * pkData->ScaleY);
	pkRect->w = (VeInt32)VeCeilf(pkData->DeviceMode.dmPelsWidth * pkData->ScaleX);
	pkRect->h = (VeInt32)VeCeilf(pkData->DeviceMode.dmPelsHeight * pkData->ScaleY);
	return true;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::GetDisplayModes(VeVideoDisplay* pkDisplay) noexcept
{
	VeDisplayData* data = (VeDisplayData*)pkDisplay->m_spDriverData;
	VeDisplayMode mode;

	for (VeUInt32 i = 0;; ++i)
	{
		if (!GetDisplayMode(data->DeviceName, i, &mode))
		{
			break;
		}
		if (VE_ISPIXELFORMAT_INDEXED(mode.m_u32Format))
		{
			continue;
		}
		if (mode.m_u32Format != VE_PIXELFORMAT_UNKNOWN)
		{
			pkDisplay->m_kDisplayModes.push_back(mode);
		}
	}
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::SetDisplayMode(VeVideoDisplay* pkDisplay,
	VeDisplayMode* pkMode) noexcept
{
	VeDisplayData* pkDisplayData = (VeDisplayData*)pkDisplay->m_spDriverData;
	VeDisplayModeData* pkData = (VeDisplayModeData*)pkMode->m_spDriverData;
	LONG status;

	status = ChangeDisplaySettingsExA(pkDisplayData->DeviceName,
		&pkData->DeviceMode, nullptr, CDS_FULLSCREEN, nullptr);
	if (status != DISP_CHANGE_SUCCESSFUL)
	{
		const VeChar8* pcReason = "Unknown reason";
		switch (status) {
		case DISP_CHANGE_BADFLAGS:
			pcReason = "DISP_CHANGE_BADFLAGS";
			break;
		case DISP_CHANGE_BADMODE:
			pcReason = "DISP_CHANGE_BADMODE";
			break;
		case DISP_CHANGE_BADPARAM:
			pcReason = "DISP_CHANGE_BADPARAM";
			break;
		case DISP_CHANGE_FAILED:
			pcReason = "DISP_CHANGE_FAILED";
			break;
		}
		VeDebugOutputCore("ChangeDisplaySettingsEx() failed: %s", pcReason);
		return false;
	}
	EnumDisplaySettingsA(pkDisplayData->DeviceName, ENUM_CURRENT_SETTINGS, &pkData->DeviceMode);
	return true;
}
//--------------------------------------------------------------------------
#define TICKS_PASSED(A, B)  ((VeInt32)((B) - (A)) <= 0)
//--------------------------------------------------------------------------
void WindowsVideoDevice::PumpEvents() noexcept
{
	MSG msg;
	DWORD dwStartTicks = GetTickCount();
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (TICKS_PASSED(msg.time, dwStartTicks))
		{
			break;
		}
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
static DWORD GetWindowStyle(VeWindow::Data* pkWindow) noexcept
{
	DWORD style = 0;

	if (pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN)
	{
		style |= STYLE_FULLSCREEN;
	}
	else
	{
		if (pkWindow->m_u32Flags & VE_WINDOW_BORDERLESS)
		{
			style |= STYLE_BORDERLESS;
		}
		else {
			style |= STYLE_NORMAL;
		}
		if (pkWindow->m_u32Flags & VE_WINDOW_RESIZABLE)
		{
			style |= STYLE_RESIZABLE;
		}
	}
	return style;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::SetWindowPositionInternal(
	VeWindow::Data* pkWindow, VeUInt32 u32Flags) noexcept
{
	VeWindowData* pkData = (VeWindowData*)pkWindow->m_spDriverdata;
	HWND hwnd = pkData->m_hWnd;
	RECT rect;
	DWORD style;
	HWND top;
	BOOL menu;
	VeInt32 x, y;
	VeInt32 w, h;

	if ((pkWindow->m_u32Flags & (VE_WINDOW_FULLSCREEN | VE_WINDOW_INPUT_FOCUS))
		== (VE_WINDOW_FULLSCREEN | VE_WINDOW_INPUT_FOCUS))
	{
		top = HWND_TOPMOST;
	}
	else
	{
		top = HWND_NOTOPMOST;
	}

	style = GetWindowLong(hwnd, GWL_STYLE);
	rect.left = 0;
	rect.top = 0;
	rect.right = pkWindow->w;
	rect.bottom = pkWindow->h;
	menu = (style & WS_CHILDWINDOW) ? FALSE : (GetMenu(hwnd) != NULL);
	AdjustWindowRectEx(&rect, style, menu, 0);
	w = (rect.right - rect.left);
	h = (rect.bottom - rect.top);
	x = pkWindow->x + rect.left;
	y = pkWindow->y + rect.top;

	pkData->m_bExpectedResize = TRUE;
	SetWindowPos(hwnd, top, x, y, w, h, u32Flags);
	pkData->m_bExpectedResize = FALSE;
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::SetupWindowData(VeWindow::Data* pkWindow,
	HWND hWnd, VE_BOOL bCreated) noexcept
{
	VE_ASSERT(pkWindow);
	VeWindowData* pkData = VE_NEW VeWindowData;
	VE_ASSERT(pkData);
	pkData->m_pkWindow = pkWindow;
	pkData->m_hWnd = hWnd;
	pkData->m_hDc = GetDC(hWnd);
	pkData->m_bCreated = bCreated;
	pkData->m_u32MouseButtonFlags = 0;
	pkWindow->m_spDriverdata = pkData;

	if (!SetProp(hWnd, TEXT("VeWindowData"), pkData))
	{
		ReleaseDC(hWnd, pkData->m_hDc);
		pkWindow->m_spDriverdata = nullptr;
		pkData = nullptr;
		return false;
	}

	pkData->m_pfuncWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
	if (pkData->m_pfuncWndProc == WindowProc)
	{
		pkData->m_pfuncWndProc = nullptr;
	}
	else
	{
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
	}

	{
		RECT rect;
		if (GetClientRect(hWnd, &rect))
		{
			VeInt32 w = rect.right;
			VeInt32 h = rect.bottom;
			if ((pkWindow->w && pkWindow->w != w) || (pkWindow->h && pkWindow->h != h))
			{
				SetWindowPositionInternal(pkWindow,
					SWP_NOCOPYBITS | SWP_NOZORDER | SWP_NOACTIVATE);
			}
			else
			{
				pkWindow->w = w;
				pkWindow->h = h;
			}
		}
	}

	{
		POINT point;
		point.x = 0;
		point.y = 0;
		if (ClientToScreen(hWnd, &point))
		{
			pkWindow->x = point.x;
			pkWindow->y = point.y;
		}
	}

	{
		DWORD style = GetWindowLong(hWnd, GWL_STYLE);
		if (style & WS_VISIBLE)
		{
			pkWindow->m_u32Flags |= VE_WINDOW_SHOWN;
		}
		else
		{
			pkWindow->m_u32Flags &= ~VE_WINDOW_SHOWN;
		}
		if (style & (WS_BORDER | WS_THICKFRAME))
		{
			pkWindow->m_u32Flags &= ~VE_WINDOW_BORDERLESS;
		}
		else
		{
			pkWindow->m_u32Flags |= VE_WINDOW_BORDERLESS;
		}
		if (style & WS_THICKFRAME)
		{
			pkWindow->m_u32Flags |= VE_WINDOW_RESIZABLE;
		}
		else
		{
			pkWindow->m_u32Flags &= ~VE_WINDOW_RESIZABLE;
		}
#		ifdef WS_MAXIMIZE
		if (style & WS_MAXIMIZE)
		{
			pkWindow->m_u32Flags |= VE_WINDOW_MAXIMIZED;
		}
		else
#		endif
		{
			pkWindow->m_u32Flags &= ~VE_WINDOW_MAXIMIZED;
		}
#		ifdef WS_MINIMIZE
		if (style & WS_MINIMIZE)
		{
			pkWindow->m_u32Flags |= VE_WINDOW_MINIMIZED;
		}
		else
#		endif
		{
			pkWindow->m_u32Flags &= ~VE_WINDOW_MINIMIZED;
		}
	}

	if (GetFocus() == hWnd)
	{
		pkWindow->m_u32Flags |= VE_WINDOW_INPUT_FOCUS;
		//SDL_SetKeyboardFocus(data->window);

		if (pkWindow->m_u32Flags & VE_WINDOW_INPUT_GRABBED)
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			ClientToScreen(hWnd, (LPPOINT)& rect);
			ClientToScreen(hWnd, (LPPOINT)& rect + 1);
			ClipCursor(&rect);
		}
	}

	/*if (videodata->RegisterTouchWindow)
	{
		videodata->RegisterTouchWindow(hwnd, (TWF_FINETOUCH | TWF_WANTPALM));
	}*/

	DragAcceptFiles(hWnd, TRUE);

	return true;
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::_CreateWindow(VeWindow::Data* pkWindow) noexcept
{
	HWND hwnd;
	RECT rect;
	DWORD style = STYLE_BASIC;
	VeInt32 x, y;
	VeInt32 w, h;

	style |= GetWindowStyle(pkWindow);

	rect.left = pkWindow->x;
	rect.top = pkWindow->y;
	rect.right = pkWindow->x + pkWindow->w;
	rect.bottom = pkWindow->y + pkWindow->h;
	AdjustWindowRectEx(&rect, style, FALSE, 0);
	x = rect.left;
	y = rect.top;
	w = (rect.right - rect.left);
	h = (rect.bottom - rect.top);

	hwnd = CreateWindowA(m_kAppName, "", style, x, y, w, h,
		NULL, NULL, m_hInstance, NULL);
	if (!hwnd)
	{
		return false;
	}

	PumpEvents();

	if (!SetupWindowData(pkWindow, hwnd, VE_TRUE))
	{
		DestroyWindow(hwnd);
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------
