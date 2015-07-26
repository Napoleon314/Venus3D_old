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

class VeVideoDevice;

struct VeVideoDisplay
{
	VeChar8* m_pcName;
	VeInt32 m_i32MaxDisplayModes;
	VeVector<VeDisplayMode> m_kDisplayModes;
	VeDisplayMode m_kDesktopMode;
	VeDisplayMode m_kCurrentMode;
	VeWindow* m_pkFullscreenWindow;
	VeVideoDevice* m_pkDevice;

	void *driverdata;
};

class VE_POWER_API VeVideoDevice : public VeRefObject
{
public:

	virtual bool Init() noexcept = 0;

	virtual void Term() noexcept = 0;

	virtual VeInt32 GetDisplayBounds(VeVideoDisplay* pkDisplay, VeRect* rect) = 0;

	///*
	//* Get a list of the available display modes for a display.
	//*/
	//void(*GetDisplayModes) (_THIS, SDL_VideoDisplay * display);

	///*
	//* Setting the display mode is independent of creating windows, so
	//* when the display mode is changed, all existing windows should have
	//* their data updated accordingly, including the display surfaces
	//* associated with them.
	//*/
	//int(*SetDisplayMode) (_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode);


protected:
	VeVideoDevice() noexcept {}

	virtual ~VeVideoDevice() noexcept {}

	VeFixedString m_kName;


};

VeSmartPointer(VeVideoDevice);
