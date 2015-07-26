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

struct VeDisplayMode
{
	VeUInt32 m_u32Format;
	VeInt32 m_i32Width;
	VeInt32 m_i32Height;
	VeInt32 m_i32RefreshRate;
	void* m_pvDriverData;
};

//struct SDL_Window
//{
//	const void *magic;
//	Uint32 id;
//	char *title;
//	SDL_Surface *icon;
//	int x, y;
//	int w, h;
//	int min_w, min_h;
//	int max_w, max_h;
//	Uint32 flags;
//	Uint32 last_fullscreen_flags;
//
//	/* Stored position and size for windowed mode */
//	SDL_Rect windowed;
//
//	SDL_DisplayMode fullscreen_mode;
//
//	float brightness;
//	Uint16 *gamma;
//	Uint16 *saved_gamma;        /* (just offset into gamma) */
//
//	SDL_Surface *surface;
//	SDL_bool surface_valid;
//
//	SDL_bool is_destroying;
//
//	SDL_WindowShaper *shaper;
//
//	SDL_WindowUserData *data;
//
//	void *driverdata;
//
//	SDL_Window *prev;
//	SDL_Window *next;
//};

class VE_POWER_API VeVideoDevice : public VeRefObject
{
public:
	

	/*struct Display
	{
		VeChar8* m_pcName;
		VeInt32 m_i32MaxDisplayModes;
		VeInt32 m_i32NumDisplayModes;
		SDL_DisplayMode *display_modes;
		SDL_DisplayMode desktop_mode;
		SDL_DisplayMode current_mode;

		SDL_Window *fullscreen_window;

		SDL_VideoDevice *device;

		void *driverdata;
	};*/


	virtual bool Init() = 0;

	virtual void Term() = 0;




protected:
	VeVideoDevice() {}

	virtual ~VeVideoDevice() {}

	VeFixedString m_kName;


};

VeSmartPointer(VeVideoDevice);
