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

	TCHAR DeviceName[32];
} ;

struct VeDisplayModeData : public VeRefObject
{
	DEVMODE DeviceMode;
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

	virtual void _Init() noexcept override;

	virtual void _Term() noexcept override;

	virtual bool _GetDisplayBounds(VeVideoDisplay* pkDisplay, VeRect* pkRect) noexcept override;

	virtual void _GetDisplayModes(VeVideoDisplay* pkDisplay) noexcept override;

	virtual bool _SetDisplayMode(VeVideoDisplay* pkDisplay, VeDisplayMode* pkMode) noexcept override;

	virtual void _PumpEvents() noexcept override;

	virtual bool _CreateWindow(VeWindow::Data* pkWindow) noexcept override;

	virtual bool _CreateWindowFrom(VeWindow::Data* pkWindow, const void* pvData) noexcept override;

	virtual void _SetWindowTitle(VeWindow::Data* pkWindow) noexcept override;

	virtual void _SetWindowIcon(VeWindow::Data* pkWindow, const VeSurfacePtr& spIcon) noexcept override;

	virtual void _SetWindowPosition(VeWindow::Data* pkWindow) noexcept override;

	virtual void _SetWindowSize(VeWindow::Data* pkWindow) noexcept override;

	virtual void _ShowWindow(VeWindow::Data* pkWindow) noexcept override;

	virtual void _HideWindow(VeWindow::Data* pkWindow) noexcept override;

	virtual void _RaiseWindow(VeWindow::Data* pkWindow) noexcept override;

	virtual void _MaximizeWindow(VeWindow::Data* pkWindow) noexcept override;

	virtual void _MinimizeWindow(VeWindow::Data* pkWindow) noexcept override;

	virtual void _RestoreWindow(VeWindow::Data* pkWindow) noexcept override;

	virtual void _SetWindowBordered(VeWindow::Data* pkWindow, VE_BOOL bBordered) noexcept override;

	virtual void _SetWindowFullscreen(VeWindow::Data* pkWindow, VeVideoDisplay* pkDisplay, VE_BOOL bFullscreen) noexcept override;

	virtual bool _SetWindowGammaRamp(VeWindow::Data* pkWindow, const VeUInt16* pu16Ramp) noexcept override;
	
	virtual bool _GetWindowGammaRamp(VeWindow::Data* pkWindow, VeUInt16* pu16Ramp) noexcept override;

	virtual void _SetWindowGrab(VeWindow::Data* pkWindow, VE_BOOL bGrabbed) noexcept override;

	virtual void _DestroyWindow(VeWindow::Data* pkWindow) noexcept override;

	virtual void _OnWindowEnter(VeWindow::Data* pkWindow) noexcept override;

	virtual void _GetWindowWMInfo(VeWindow::Data* pkWindow, VeSysWMInfo* pkInfo) noexcept override;

protected:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	bool RegisterApp(const VeChar8* pcName, VeUInt32 u32Style, HINSTANCE hInst) noexcept;

	void UnregisterApp() noexcept;

	bool AddDisplay(LPCTSTR DeviceName) noexcept;

	void InitModes() noexcept;

	void TermModes() noexcept;

	void SetWindowPositionInternal(VeWindow::Data* pkWindow, VeUInt32 u32Flags) noexcept;

	bool SetupWindowData(VeWindow::Data* pkWindow, HWND hWnd, VE_BOOL bCreated) noexcept;

	VeInt32 m_i32AppRegistered = 0;
	VeFixedString m_kAppName;
	VeUInt32 m_u32AppStyle = 0;
	HINSTANCE m_hInstance = nullptr;

};
