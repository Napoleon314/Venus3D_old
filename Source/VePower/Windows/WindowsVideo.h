////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   WindowsVideo.h
//  Created:     2015/07/27 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

struct VeDisplayData : public VeRefObject
{
	VeDisplayData() noexcept
	{
		VeZero(DeviceName);
	}

	CHAR DeviceName[32];
} ;

struct VeDisplayModeData : public VeRefObject
{
	DEVMODEA DeviceMode;
	VeFloat32 ScaleX;
	VeFloat32 ScaleY;
};

class WindowsVideoDevice;

struct VeWindowData : public VeRefObject
{
	VeWindow::Data* m_pkWindow = nullptr;
	HWND m_hWnd = nullptr;
	HDC m_hDc = nullptr;
	HDC m_mDc = nullptr;
	HBITMAP m_hBitMap = nullptr;
	WNDPROC m_pfuncWndProc = nullptr;
	VE_BOOL m_bCreated = VE_FALSE;
	WPARAM m_u32MouseButtonFlags = 0;
	BOOL m_bExpectedResize = FALSE;
	VE_BOOL m_bInTitleClick = VE_FALSE;
	VE_BOOL m_bInModalLoop = VE_FALSE;
};

class WindowsVideoDevice : public VeVideoDevice
{
public:
	WindowsVideoDevice() noexcept;

	virtual ~WindowsVideoDevice() noexcept;

	virtual void Init() noexcept;

	virtual void Term() noexcept;

	virtual bool GetDisplayBounds(VeRect* pkRect, VeVideoDisplay* pkDisplay) noexcept;

	virtual void GetDisplayModes(VeVideoDisplay* pkDisplay) noexcept;

	virtual bool SetDisplayMode(VeVideoDisplay* pkDisplay, VeDisplayMode* pkMode) noexcept;

	virtual void PumpEvents() noexcept;

	virtual bool _CreateWindow(VeWindow::Data* pkWindow) noexcept;

protected:
	bool RegisterApp(const VeChar8* pcName, VeUInt32 u32Style, HINSTANCE hInst) noexcept;

	void UnregisterApp() noexcept;

	bool AddDisplay(const VeChar8* pcDeviceName) noexcept;

	void InitModes() noexcept;

	void TermModes() noexcept;

	void SetWindowPositionInternal(VeWindow::Data* pkWindow, VeUInt32 u32Flags) noexcept;

	bool SetupWindowData(VeWindow::Data* pkWindow, HWND hWnd, VE_BOOL bCreated) noexcept;

	VeInt32 m_i32AppRegistered = 0;
	VeFixedString m_kAppName;
	VeUInt32 m_u32AppStyle = 0;
	HINSTANCE m_hInstance = nullptr;

};
