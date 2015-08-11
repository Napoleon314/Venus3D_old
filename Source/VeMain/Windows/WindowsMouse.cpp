////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   WindowsMouse.cpp
//  Created:     2015/08/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "../VeMainPch.h"
#include "WindowsMouse.h"
#include "WindowsVideo.h"

//--------------------------------------------------------------------------
#ifdef VE_ENABLE_KEYBOARD
//--------------------------------------------------------------------------
VeMousePtr CreateMouse() noexcept
{
	return VE_NEW WindowsMouse();
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
WindowsMouse::WindowsMouse() noexcept
{

}
//--------------------------------------------------------------------------
WindowsMouse::~WindowsMouse() noexcept
{

}
//--------------------------------------------------------------------------
void WindowsMouse::_Init() noexcept
{
	VeCursorPtr spDefault = VE_NEW VeCursor();
	VeCursor::Data* pkData = GetCursorData(spDefault);
	pkData->m_pvDriverdata = LoadCursor(nullptr, IDC_ARROW);
	SetDefaultCursor(pkData);
	SetDoubleClickTime(GetDoubleClickTime());
}
//--------------------------------------------------------------------------
void WindowsMouse::_Term() noexcept
{
	m_spDefCursor = nullptr;
	m_spCurCursor = nullptr;
}
//--------------------------------------------------------------------------
struct VeCursorData : public VeRefObject
{
	HICON hicon;
};
//--------------------------------------------------------------------------
bool WindowsMouse::_CreateCursor(VeCursor::Data* pkCur,
	const VeSurfacePtr& spSurface,
	VeInt32 i32HotX, VeInt32 i32HotY) noexcept
{
	VE_ASSERT(spSurface && pkCur);
	const VeSizeT pad = (sizeof(VeSizeT) * 8);
	HICON hicon;
	HDC hdc;
	BITMAPV4HEADER bmh;
	LPVOID pixels;
	LPVOID maskbits;
	size_t maskbitslen;
	ICONINFO ii;

	VeInt32 w = spSurface->GetWidth();
	VeInt32 h = spSurface->GetHeight();

	VeZeroMemory(&bmh, sizeof(bmh));
	bmh.bV4Size = sizeof(bmh);
	bmh.bV4Width = w;
	bmh.bV4Height = -h;
	bmh.bV4Planes = 1;
	bmh.bV4BitCount = 32;
	bmh.bV4V4Compression = BI_BITFIELDS;
	bmh.bV4AlphaMask = 0xFF000000;
	bmh.bV4RedMask = 0x00FF0000;
	bmh.bV4GreenMask = 0x0000FF00;
	bmh.bV4BlueMask = 0x000000FF;

	maskbitslen = ((w + (pad - (w % pad))) / 8) * h;
	maskbits = VeStackAlloc(VeUInt8, maskbitslen);
	
	VeMemorySet(maskbits, 0xFF, maskbitslen);

	hdc = GetDC(nullptr);
	VeZeroMemory(&ii, sizeof(ii));
	ii.fIcon = FALSE;
	ii.xHotspot = (DWORD)i32HotX;
	ii.yHotspot = (DWORD)i32HotY;
	ii.hbmColor = CreateDIBSection(hdc, (BITMAPINFO*)&bmh, DIB_RGB_COLORS, &pixels, nullptr, 0);
	ii.hbmMask = CreateBitmap(w, h, 1, 1, maskbits);
	ReleaseDC(nullptr, hdc);
	VeStackFree(maskbits);

	VE_ASSERT(spSurface->GetFormat()->m_u32Format == VE_PIXELFORMAT_ARGB8888);
	VE_ASSERT(spSurface->GetPitch() == w * 4);
	VeMemoryCopy(pixels, spSurface->GetBuffer(), h * spSurface->GetPitch());

	hicon = CreateIconIndirect(&ii);

	DeleteObject(ii.hbmColor);
	DeleteObject(ii.hbmMask);

	pkCur->m_pvDriverdata = hicon;

	if (!hicon)
	{
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------
bool WindowsMouse::_CreateSystemCursor(VeCursor::Data* pkCur,
	VeSystemCursor eCursor) noexcept
{
	VE_ASSERT(pkCur);
	LPCTSTR name;

	switch (eCursor)
	{
	case VE_SYSTEM_CURSOR_ARROW:     name = IDC_ARROW; break;
	case VE_SYSTEM_CURSOR_IBEAM:     name = IDC_IBEAM; break;
	case VE_SYSTEM_CURSOR_WAIT:      name = IDC_WAIT; break;
	case VE_SYSTEM_CURSOR_CROSSHAIR: name = IDC_CROSS; break;
	case VE_SYSTEM_CURSOR_WAITARROW: name = IDC_WAIT; break;
	case VE_SYSTEM_CURSOR_SIZENWSE:  name = IDC_SIZENWSE; break;
	case VE_SYSTEM_CURSOR_SIZENESW:  name = IDC_SIZENESW; break;
	case VE_SYSTEM_CURSOR_SIZEWE:    name = IDC_SIZEWE; break;
	case VE_SYSTEM_CURSOR_SIZENS:    name = IDC_SIZENS; break;
	case VE_SYSTEM_CURSOR_SIZEALL:   name = IDC_SIZEALL; break;
	case VE_SYSTEM_CURSOR_NO:        name = IDC_NO; break;
	case VE_SYSTEM_CURSOR_HAND:      name = IDC_HAND; break;
	default:
		return false;
	}

	HICON hicon;
	hicon = LoadCursor(nullptr, name);
	pkCur->m_pvDriverdata = hicon;

	return true;
}
//--------------------------------------------------------------------------
void WindowsMouse::_ShowCursor(VeCursor::Data* pkCur) noexcept
{
	if (pkCur)
	{
		m_hCursor = (HCURSOR)pkCur->m_pvDriverdata;
	}
	else
	{
		m_hCursor = nullptr;
	}

	if (m_pkFocus)
	{
		::SetCursor(m_hCursor);
	}
}
//--------------------------------------------------------------------------
void WindowsMouse::_FreeCursor(VeCursor::Data* pkCur) noexcept
{
	HICON hicon = (HICON)pkCur->m_pvDriverdata;
	DestroyIcon(hicon);
	pkCur->m_pvDriverdata = nullptr;
}
//--------------------------------------------------------------------------
void WindowsMouse::_Warp(VeWindow::Data* pkWindow,
	VeInt32 x, VeInt32 y) noexcept
{
	VeWindowData* pkData = (VeWindowData *)pkWindow->m_spDriverdata;
	HWND hwnd = pkData->m_hWnd;
	POINT pt;

	if (pkData->m_bInTitleClick || pkData->m_bInModalLoop)
	{
		return;
	}

	pt.x = x;
	pt.y = y;
	ClientToScreen(hwnd, &pt);
	SetCursorPos(pt.x, pt.y);
}
//--------------------------------------------------------------------------
bool WindowsMouse::_SetRelativeMode(bool bEnabled) noexcept
{
	RAWINPUTDEVICE rawMouse = { 0x01, 0x02, 0, nullptr };

	if (!bEnabled)
	{
		rawMouse.dwFlags |= RIDEV_REMOVE;
	}

	if (RegisterRawInputDevices(&rawMouse, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
	{
		if (bEnabled) {
			return false;
		}
	}
	return true;
}
//--------------------------------------------------------------------------
