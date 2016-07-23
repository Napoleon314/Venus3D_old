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
		memset(DeviceName, 0, sizeof(DeviceName));
	}

	TCHAR DeviceName[32];
} ;

struct VeDisplayModeData : public VeRefObject
{
	DEVMODE DeviceMode;
	float ScaleX;
	float ScaleY;
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
	VeNoCopy(WindowsVideoDevice);
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

	virtual bool _SetWindowGammaRamp(VeWindow::Data* pkWindow, const uint16_t* pu16Ramp) noexcept override;
	
	virtual bool _GetWindowGammaRamp(VeWindow::Data* pkWindow, uint16_t* pu16Ramp) noexcept override;

	virtual void _SetWindowGrab(VeWindow::Data* pkWindow, VE_BOOL bGrabbed) noexcept override;

	virtual void _DestroyWindow(VeWindow::Data* pkWindow) noexcept override;

	virtual void _OnWindowEnter(VeWindow::Data* pkWindow) noexcept override;

	virtual void _GetWindowWMInfo(VeWindow::Data* pkWindow, VeSysWMInfo* pkInfo) noexcept override;

protected:
	static void UpdateClipCursor(VeWindow::Data* pkWindow) noexcept;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	bool RegisterApp(const char* pcName, uint32_t u32Style, HINSTANCE hInst) noexcept;

	void UnregisterApp() noexcept;

	bool AddDisplay(LPCTSTR DeviceName) noexcept;

	void InitModes() noexcept;

	void TermModes() noexcept;

	void SetWindowPositionInternal(VeWindow::Data* pkWindow, uint32_t u32Flags) noexcept;

	bool SetupWindowData(VeWindow::Data* pkWindow, HWND hWnd, VE_BOOL bCreated) noexcept;

	void CheckWParamMouseButton(VE_BOOL bwParamMousePressed, VE_BOOL bMousePressed,
		VeWindowData* pkData, uint8_t u8Button) noexcept;

	void CheckWParamMouseButtons(WPARAM wParam, VeWindowData* pkData) noexcept;

	void CheckRawMouseButtons(ULONG rawButtons, VeWindowData* pkData) noexcept;

	void CheckAsyncMouseRelease(VeWindowData* pkData) noexcept;

	int32_t m_i32AppRegistered = 0;
	vtd::string m_kAppName;
	uint32_t m_u32AppStyle = 0;
	HINSTANCE m_hInstance = nullptr;

};
