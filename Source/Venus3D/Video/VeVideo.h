////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Video
//  File name:   VeVideo.h
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

typedef vtd::point<int32_t> VePoint;

typedef vtd::rect<int32_t> VeRect;

class VENUS_API VeVideoDevice : public VeRefObject
{
	VeNoCopy(VeVideoDevice);
public:
	inline const char* GetDriverName() const noexcept;

	inline const char* GetDriverDesc() const noexcept;

	void Init() noexcept;

	void Term() noexcept;
	
	void GetDisplayBounds(int32_t i32DisplayIndex, VeRect* pkRect) noexcept;

	bool CreateWindowBy(VeWindow::Data* pkWindow, const char* pcTitle,
		int32_t x, int32_t y, int32_t w, int32_t h, uint32_t u32Flags) noexcept;

	bool CreateWindowFrom(VeWindow::Data* pkWindow, const void* pvData) noexcept;

	void SetWindowTitle(VeWindow::Data* pkWindow, const char* pcTitle) noexcept;

	void SetWindowIcon(VeWindow::Data* pkWindow, const VeSurfacePtr& spIcon) noexcept;

	void SetWindowPosition(VeWindow::Data* pkWindow, int32_t x, int32_t y) noexcept;

	void SetWindowSize(VeWindow::Data* pkWindow, int32_t w, int32_t h) noexcept;

	void SetWindowMinimumSize(VeWindow::Data* pkWindow, int32_t min_w, int32_t min_h) noexcept;

	void SetWindowMaximumSize(VeWindow::Data* pkWindow, int32_t max_w, int32_t max_h) noexcept;

	void ShowWindow(VeWindow::Data* pkWindow) noexcept;

	void HideWindow(VeWindow::Data* pkWindow) noexcept;

	void RaiseWindow(VeWindow::Data* pkWindow) noexcept;

	void MaximizeWindow(VeWindow::Data* pkWindow) noexcept;

	void MinimizeWindow(VeWindow::Data* pkWindow) noexcept;

	void RestoreWindow(VeWindow::Data* pkWindow) noexcept;

	void SetWindowBordered(VeWindow::Data* pkWindow, bool bBordered) noexcept;

	void SetWindowFullscreen(VeWindow::Data* pkWindow, uint32_t u32Flags) noexcept;

	bool SetWindowGammaRamp(VeWindow::Data* pkWindow, const uint16_t* red,
		const uint16_t* green, const uint16_t* blue) noexcept;

	bool GetWindowGammaRamp(VeWindow::Data* pkWindow, uint16_t* red,
		uint16_t* green, uint16_t* blue) noexcept;

	bool SetWindowBrightness(VeWindow::Data* pkWindow, float f32Brightness) noexcept;

	void SetWindowGrab(VeWindow::Data* pkWindow, bool bGrabbed) noexcept;

	void DestroyWindow(VeWindow::Data* pkWindow) noexcept;

	void GetWindowWMInfo(VeWindow::Data* pkWindow, VeSysWMInfo* pkInfo) noexcept;

	void PeekEvents(VeVector<VeEvent*>& kOutput) noexcept;

protected:
	virtual void _Init() noexcept {}

	virtual void _Term() noexcept {}

	virtual bool _GetDisplayBounds(VeVideoDisplay*, VeRect*) noexcept { return false; }

	virtual void _GetDisplayModes(VeVideoDisplay*) noexcept {}

	virtual bool _SetDisplayMode(VeVideoDisplay*, VeDisplayMode*) noexcept { return false; }

	virtual void _PumpEvents() noexcept {}

	virtual bool _CreateWindow(VeWindow::Data*) noexcept { return false; }

	virtual bool _CreateWindowFrom(VeWindow::Data*, const void*) noexcept { return false; }

	virtual void _SetWindowTitle(VeWindow::Data*) noexcept {}

	virtual void _SetWindowIcon(VeWindow::Data*, const VeSurfacePtr&) noexcept {}

	virtual void _SetWindowPosition(VeWindow::Data*) noexcept {}

	virtual void _SetWindowSize(VeWindow::Data*) noexcept {}

	virtual void _SetWindowMinimumSize(VeWindow::Data*) noexcept {}

	virtual void _SetWindowMaximumSize(VeWindow::Data*) noexcept {}

	virtual void _ShowWindow(VeWindow::Data*) noexcept {}

	virtual void _HideWindow(VeWindow::Data*) noexcept {}

	virtual void _RaiseWindow(VeWindow::Data*) noexcept {}

	virtual void _MaximizeWindow(VeWindow::Data*) noexcept {}

	virtual void _MinimizeWindow(VeWindow::Data*) noexcept {}

	virtual void _RestoreWindow(VeWindow::Data*) noexcept {}

	virtual void _SetWindowBordered(VeWindow::Data*, VE_BOOL) noexcept {}

	virtual void _SetWindowFullscreen(VeWindow::Data*, VeVideoDisplay*, VE_BOOL) noexcept {}

	virtual bool _SetWindowGammaRamp(VeWindow::Data*, const uint16_t*) noexcept { return false; }

	virtual bool _GetWindowGammaRamp(VeWindow::Data*, uint16_t*) noexcept;

	virtual void _SetWindowGrab(VeWindow::Data*, VE_BOOL) noexcept {}

	virtual void _DestroyWindow(VeWindow::Data*) noexcept {}

	virtual void _OnWindowEnter(VeWindow::Data*) noexcept {}

	virtual void _GetWindowWMInfo(VeWindow::Data*, VeSysWMInfo*) noexcept {}

	static void CalculateGammaRamp(float, uint16_t*) noexcept;

protected:
	friend class VeKeyboard;
	friend class VeMouse;

	int32_t GetWindowDisplayIndex(VeWindow::Data* pkWindow) noexcept;

	VeVideoDisplay* GetDisplayForWindow(VeWindow::Data* pkWindow) noexcept;

	int32_t GetIndexOfDisplay(VeVideoDisplay* pkDisplay) noexcept;

	void UpdateFullscreenMode(VeWindow::Data* pkWindow, VE_BOOL bFullscreen) noexcept;

	void FinishWindowCreation(VeWindow::Data* pkWindow, uint32_t u32Flags) noexcept;

	void SendWindowEvent(VeWindow::Data* pkWindow, uint8_t u8Event, int32_t i32Data1, int32_t i32Data2) noexcept;

	void OnWindowShown(VeWindow::Data* pkWindow) noexcept;

	void OnWindowHidden(VeWindow::Data* pkWindow) noexcept;

	void OnWindowResized(VeWindow::Data* pkWindow) noexcept;

	void OnWindowMinimized(VeWindow::Data* pkWindow) noexcept;

	void OnWindowRestored(VeWindow::Data* pkWindow) noexcept;

	void OnWindowEnter(VeWindow::Data* pkWindow) noexcept;

	void OnWindowLeave(VeWindow::Data* pkWindow) noexcept;

	void OnWindowFocusGained(VeWindow::Data* pkWindow) noexcept;

	void OnWindowFocusLost(VeWindow::Data* pkWindow) noexcept;

	void UpdateWindowGrab(VeWindow::Data* pkWindow) noexcept;

	static VeWindow::Data* GetKeyboardFocus() noexcept;

	static VeWindow::Data* GetMouseFocus() noexcept;

	VeVideoDevice() noexcept {}

	virtual ~VeVideoDevice() noexcept {}

	vtd::string m_kName;
	vtd::string m_kDesc;
	VeVector<VeVideoDisplay> m_kDisplayList;
	vtd::intrusive_list<VeWindow::Data*> m_kWindowList;
	uint32_t m_u32NextObjectID;

};

VeSmartPointer(VeVideoDevice);

#include "VeVideo.inl"
