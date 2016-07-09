////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Video
//  File name:   VeWindow.h
//  Created:     2016/07/10 by Albert
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

struct VeDisplayMode
{
	uint32_t m_u32Format = 0;
	int32_t m_i32Width = 0;
	int32_t m_i32Height = 0;
	int32_t m_i32RefreshRate = 0;
	VeRefObjectPtr m_spDriverData;
};

struct VeVideoDisplay
{
	vtd::string m_kName;
	vtd::vector<VeDisplayMode> m_kDisplayModes;
	VeDisplayMode m_kDesktopMode;
	VeDisplayMode m_kCurrentMode;
	void* m_pvFullscreenWindow = nullptr;
	VeRefObjectPtr m_spDriverData;
};

struct VeWindowUserData
{
	char* m_pcName = nullptr;
	void* m_pvData = nullptr;
	VeWindowUserData* m_pkNext = nullptr;
};

enum VeSysWMType
{
	VE_SYSWM_UNKNOWN,
	VE_SYSWM_WINDOWS,
	VE_SYSWM_X11,
	VE_SYSWM_COCOA,
	VE_SYSWM_UIKIT,
	VE_SYSWM_ANDROID
};

#ifdef BUILD_PLATFORM_WIN
#	define VE_VIDEO_DRIVER_WINDOWS
#endif

struct VeSysWMInfo
{
	VeSysWMType m_eType = VE_SYSWM_UNKNOWN;
	union
	{
#		if defined(VE_VIDEO_DRIVER_WINDOWS)
		struct
		{
			HWND window;
		} win;
#		endif
#		if defined(VE_VIDEO_DRIVER_X11)
		struct
		{
			Display *display;
			Window window;
		} x11;
#		endif
#		if defined(VE_VIDEO_DRIVER_COCOA)
		struct
		{
			NSWindow *window;
		} cocoa;
#		endif
#		if defined(VE_VIDEO_DRIVER_UIKIT)
		struct
		{
			UIWindow *window;
		} uikit;
#		endif
		int32_t dummy = 0;
	};
};

class VENUS_API VeWindow : public VeRefObject
{
	VeNoCopy(VeWindow);
public:
	struct Data
	{
		vtd::intrusive_node<Data*> m_kNode;
		uint32_t m_u32Id = 0;
		vtd::string m_kTitle;
		VeSurfacePtr m_spIcon;
		int32_t x = 0, y = 0;
		int32_t w = 0, h = 0;
		int32_t min_w = 0, min_h = 0;
		int32_t max_w = 0, max_h = 0;
		uint32_t m_u32Flags = 0;
		uint32_t m_u32LastFullscreenFlags = 0;
		vtd::rect<int32_t> m_kWindowed;
		VeDisplayMode m_kFullscreenMode;
		float m_f32Brightness = 0;
		uint16_t* m_pu16Gamma = nullptr;
		uint16_t* m_pu16SavedGamma = nullptr;
		void* surface = nullptr;
		VE_BOOL m_bSurfaceValid = 0;
		VE_BOOL m_bIsDestorying = 0;
		void* shaper = nullptr;
		VeWindowUserData* m_pkData = nullptr;
		VeRefObjectPtr m_spDriverdata;
	};

	VeWindow() noexcept;

	virtual ~VeWindow() noexcept;

	inline bool IsValid() const noexcept;

	inline uint32_t GetID() const noexcept;

	inline uint32_t GetFlags() const noexcept;

	inline const char* GetTitle() const noexcept;

	inline int32_t GetPosX() const noexcept;

	inline int32_t GetPosY() const noexcept;

	inline int32_t GetWidth() const noexcept;

	inline int32_t GetHeight() const noexcept;

	inline int32_t GetMinimumWidth() const noexcept;

	inline int32_t GetMinimumHeight() const noexcept;

	inline int32_t GetMaximumWidth() const noexcept;

	inline int32_t GetMaximumHeight() const noexcept;

	inline float GetBrightness() const noexcept;

	bool Create(const char* pcTitle, int32_t x, int32_t y,
		int32_t w, int32_t h, uint32_t u32Flags) noexcept;

	bool Create(const void* pvData) noexcept;

	void SetTitle(const char* pcTitle) noexcept;

	void SetPosition(int32_t x, int32_t y) noexcept;

	void SetSize(int32_t w, int32_t h) noexcept;

	void SetMinimumSize(int32_t min_w, int32_t min_h) noexcept;

	void SetMaximumSize(int32_t max_w, int32_t max_h) noexcept;

	void Show() noexcept;

	void Hide() noexcept;

	void Raise() noexcept;

	void Maximize() noexcept;

	void Minimize() noexcept;

	void Restore() noexcept;

	void SetBordered(bool bBordered) noexcept;

	void SetFullscreen(uint32_t u32Flags) noexcept;

	bool SetBrightness(float f32Brightness) noexcept;

	void SetGrab(bool bGrabbed) noexcept;

	void Destory() noexcept;

	VeSysWMInfo GetWMInfo() noexcept;

	static VeWindow* Cast(VeWindow::Data* pkData) noexcept;

protected:
	Data m_kData;

};

VeSmartPointer(VeWindow);

#include "VeWindow.inl"
