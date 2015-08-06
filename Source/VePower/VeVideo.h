////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeVideo.h
//  Created:     2015/07/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

enum VeWindowFlags
{
	VE_WINDOW_FULLSCREEN = 0x00000001,
	VE_WINDOW_OPENGL = 0x00000002,
	VE_WINDOW_SHOWN = 0x00000004,
	VE_WINDOW_HIDDEN = 0x00000008,
	VE_WINDOW_BORDERLESS = 0x00000010,
	VE_WINDOW_RESIZABLE = 0x00000020,
	VE_WINDOW_MINIMIZED = 0x00000040,
	VE_WINDOW_MAXIMIZED = 0x00000080,
	VE_WINDOW_INPUT_GRABBED = 0x00000100,
	VE_WINDOW_INPUT_FOCUS = 0x00000200,
	VE_WINDOW_MOUSE_FOCUS = 0x00000400,
	VE_WINDOW_FULLSCREEN_DESKTOP = (VE_WINDOW_FULLSCREEN | 0x00001000),
	VE_WINDOW_FOREIGN = 0x00000800,
	VE_WINDOW_ALLOW_HIGHDPI = 0x00002000
};

#define VE_WINDOWPOS_UNDEFINED_MASK			0x1FFF0000
#define VE_WINDOWPOS_UNDEFINED_DISPLAY(X)	(VE_WINDOWPOS_UNDEFINED_MASK|(X))
#define VE_WINDOWPOS_UNDEFINED				VE_WINDOWPOS_UNDEFINED_DISPLAY(0)
#define VE_WINDOWPOS_ISUNDEFINED(X)			(((X)&0xFFFF0000) == VE_WINDOWPOS_UNDEFINED_MASK)


#define VE_WINDOWPOS_CENTERED_MASK			0x2FFF0000
#define VE_WINDOWPOS_CENTERED_DISPLAY(X)	(VE_WINDOWPOS_CENTERED_MASK|(X))
#define VE_WINDOWPOS_CENTERED				VE_WINDOWPOS_CENTERED_DISPLAY(0)
#define VE_WINDOWPOS_ISCENTERED(X)			(((X)&0xFFFF0000) == VE_WINDOWPOS_CENTERED_MASK)

enum VeWindowEventID
{
	VE_WINDOWEVENT_NONE,
	VE_WINDOWEVENT_SHOWN,
	VE_WINDOWEVENT_HIDDEN,
	VE_WINDOWEVENT_EXPOSED,
	VE_WINDOWEVENT_MOVED,
	VE_WINDOWEVENT_RESIZED,
	VE_WINDOWEVENT_SIZE_CHANGED,
	VE_WINDOWEVENT_MINIMIZED,
	VE_WINDOWEVENT_MAXIMIZED,
	VE_WINDOWEVENT_RESTORED,
	VE_WINDOWEVENT_ENTER,
	VE_WINDOWEVENT_LEAVE,
	VE_WINDOWEVENT_FOCUS_GAINED,
	VE_WINDOWEVENT_FOCUS_LOST,
	VE_WINDOWEVENT_CLOSE
};

class VE_POWER_API VeVideoDevice : public VeRefObject
{
public:
	inline const VeChar8* GetDriverName() const noexcept;

	inline const VeChar8* GetDriverDesc() const noexcept;

	void Init() noexcept;

	void Term() noexcept;
	
	void GetDisplayBounds(VeInt32 i32DisplayIndex, VeRect* pkRect) noexcept;

	bool CreateWindowBy(VeWindow::Data* pkWindow, const VeChar8* pcTitle,
		VeInt32 x, VeInt32 y, VeInt32 w, VeInt32 h, VeUInt32 u32Flags) noexcept;

	void DestroyWindow(VeWindow::Data* pkWindow) noexcept;

	void ShowWindow(VeWindow::Data* pkWindow) noexcept;

	void HideWindow(VeWindow::Data* pkWindow) noexcept;

	void RaiseWindow(VeWindow::Data* pkWindow) noexcept;

	void SetWindowTitle(VeWindow::Data* pkWindow, const VeChar8* pcTitle) noexcept;

	const VeChar8* GetWindowTitle(VeWindow::Data* pkWindow) noexcept;

	void PeekEvents(VeVector<VeEvent*>& kOutput) noexcept;

protected:
	virtual void _Init() noexcept {}

	virtual void _Term() noexcept {}

	virtual bool _GetDisplayBounds(VeVideoDisplay* pkDisplay, VeRect* pkRect) noexcept { return false; }

	virtual void _GetDisplayModes(VeVideoDisplay* pkDisplay) noexcept {}

	virtual bool _SetDisplayMode(VeVideoDisplay* pkDisplay, VeDisplayMode* pkMode) noexcept { return false; }

	virtual void _PumpEvents() noexcept {}

	virtual bool _CreateWindow(VeWindow::Data* pkWindow) noexcept { return false; }

	virtual bool _CreateWindowFrom(VeWindow::Data* pkWindow, const void* pvData) noexcept { return false; }

	virtual void _SetWindowTitle(VeWindow::Data* pkWindow) noexcept {}

	//void(*SetWindowIcon) (_THIS, SDL_Window * window, SDL_Surface * icon);

	virtual void _SetWindowPosition(VeWindow::Data* pkWindow) noexcept {}

	virtual void _SetWindowSize(VeWindow::Data* pkWindow) noexcept {}

	virtual void _SetWindowMinimumSize(VeWindow::Data* pkWindow) noexcept {}

	virtual void _SetWindowMaximumSize(VeWindow::Data* pkWindow) noexcept {}

	virtual void _ShowWindow(VeWindow::Data* pkWindow) noexcept {}

	virtual void _HideWindow(VeWindow::Data* pkWindow) noexcept {}

	virtual void _RaiseWindow(VeWindow::Data* pkWindow) noexcept {}

	virtual void _MaximizeWindow(VeWindow::Data* pkWindow) noexcept {}

	virtual void _MinimizeWindow(VeWindow::Data* pkWindow) noexcept {}

	virtual void _RestoreWindow(VeWindow::Data* pkWindow) noexcept {}

	virtual void _SetWindowBordered(VeWindow::Data* pkWindow, VE_BOOL bBordered) noexcept {}

	virtual void _SetWindowFullscreen(VeWindow::Data* pkWindow, VeVideoDisplay* pkDisplay, VE_BOOL bFullscreen) noexcept {}

	virtual bool _SetWindowGammaRamp(VeWindow::Data* pkWindow, const VeUInt16* pu16Ramp) noexcept { return false; }

	virtual bool _GetWindowGammaRamp(VeWindow::Data* pkWindow, VeUInt16* pu16Ramp) noexcept { return false; }

	virtual void _SetWindowGrab(VeWindow::Data* pkWindow, VE_BOOL bGrabbed) noexcept {};

	virtual void _DestroyWindow(VeWindow::Data* pkWindow) noexcept {}

protected:
	VeInt32 GetWindowDisplayIndex(VeWindow::Data* pkWindow) noexcept;

	VeVideoDisplay* GetDisplayForWindow(VeWindow::Data* pkWindow) noexcept;

	void UpdateFullscreenMode(VeWindow::Data* pkWindow, VE_BOOL bFullscreen) noexcept;

	void FinishWindowCreation(VeWindow::Data* pkWindow, VeUInt32 u32Flags) noexcept;

	void SendWindowEvent(VeWindow::Data* pkWindow, VeUInt8 u8Event, VeInt32 i32Data1, VeInt32 i32Data2) noexcept;

	void OnWindowShown(VeWindow::Data* pkWindow) noexcept;

	void OnWindowHidden(VeWindow::Data* pkWindow) noexcept;

	void OnWindowResized(VeWindow::Data* pkWindow) noexcept;

	void OnWindowMinimized(VeWindow::Data* pkWindow) noexcept;

	void OnWindowRestored(VeWindow::Data* pkWindow) noexcept;

	void OnWindowEnter(VeWindow::Data* pkWindow) noexcept;

	void OnWindowLeave(VeWindow::Data* pkWindow) noexcept;

	void OnWindowFocusGained(VeWindow::Data* pkWindow) noexcept;

	void OnWindowFocusLost(VeWindow::Data* pkWindow) noexcept;

	VeVideoDevice() noexcept {}

	virtual ~VeVideoDevice() noexcept {}

	VeFixedString m_kName;
	VeFixedString m_kDesc;
	VeVector<VeVideoDisplay> m_kDisplayList;
	VeRefList<VeWindow::Data*> m_kWindowList;
	VeUInt32 m_u32NextObjectID;

};

VeSmartPointer(VeVideoDevice);

#include "VeVideo.inl"
