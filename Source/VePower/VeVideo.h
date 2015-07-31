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
	VE_WINDOW_FULLSCREEN = 0x00000001,         /**< fullscreen window */
	VE_WINDOW_OPENGL = 0x00000002,             /**< window usable with OpenGL context */
	VE_WINDOW_SHOWN = 0x00000004,              /**< window is visible */
	VE_WINDOW_HIDDEN = 0x00000008,             /**< window is not visible */
	VE_WINDOW_BORDERLESS = 0x00000010,         /**< no window decoration */
	VE_WINDOW_RESIZABLE = 0x00000020,          /**< window can be resized */
	VE_WINDOW_MINIMIZED = 0x00000040,          /**< window is minimized */
	VE_WINDOW_MAXIMIZED = 0x00000080,          /**< window is maximized */
	VE_WINDOW_INPUT_GRABBED = 0x00000100,      /**< window has grabbed input focus */
	VE_WINDOW_INPUT_FOCUS = 0x00000200,        /**< window has input focus */
	VE_WINDOW_MOUSE_FOCUS = 0x00000400,        /**< window has mouse focus */
	VE_WINDOW_FULLSCREEN_DESKTOP = (VE_WINDOW_FULLSCREEN | 0x00001000),
	VE_WINDOW_FOREIGN = 0x00000800,            /**< window not created by SDL */
	VE_WINDOW_ALLOW_HIGHDPI = 0x00002000       /**< window should be created in high-DPI mode if supported */
};

class VE_POWER_API VeVideoDevice : public VeRefObject
{
public:
	inline const VeChar8* GetName() const noexcept;

	inline const VeChar8* GetDesc() const noexcept;

	virtual void Init() noexcept {}

	virtual void Term() noexcept {}

	virtual bool GetDisplayBounds(VeRect* pkRect, VeVideoDisplay* pkDisplay) noexcept { return false; }

	virtual void GetDisplayModes(VeVideoDisplay* pkDisplay) noexcept {}

	virtual bool SetDisplayMode(VeVideoDisplay* pkDisplay, VeDisplayMode* pkMode) noexcept { return false; }

	virtual void PumpEvents() noexcept {}

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

	virtual void _DestroyWindow(VeWindow::Data* pkWindow) noexcept {}

protected:
	VeVideoDevice() noexcept {}

	virtual ~VeVideoDevice() noexcept {}

	VeFixedString m_kName;
	VeFixedString m_kDesc;
	VeVector<VeVideoDisplay> m_kDisplayList;


};

VeSmartPointer(VeVideoDevice);

#include "VeVideo.inl"
