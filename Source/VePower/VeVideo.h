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

	virtual void Init() noexcept = 0;

	virtual void Term() noexcept = 0;

	virtual bool GetDisplayBounds(VeRect* pkRect, VeVideoDisplay* pkDisplay) noexcept = 0;

	virtual void GetDisplayModes(VeVideoDisplay* pkDisplay) noexcept = 0;

	virtual bool SetDisplayMode(VeVideoDisplay* pkDisplay, VeDisplayMode* pkMode) noexcept = 0;

	virtual void PumpEvents() noexcept = 0;

	virtual bool _CreateWindow(VeWindow::Data* pkWindow) noexcept = 0;

protected:
	VeVideoDevice() noexcept {}

	virtual ~VeVideoDevice() noexcept {}

	VeFixedString m_kName;
	VeFixedString m_kDesc;
	VeVector<VeVideoDisplay> m_kDisplayList;


};

VeSmartPointer(VeVideoDevice);

#include "VeVideo.inl"
