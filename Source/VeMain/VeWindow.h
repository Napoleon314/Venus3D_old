////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeWindow.h
//  Created:     2015/07/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

struct VeDisplayMode
{
	VeUInt32 m_u32Format = 0;
	VeInt32 m_i32Width = 0;
	VeInt32 m_i32Height = 0;
	VeInt32 m_i32RefreshRate = 0;
	VeRefObjectPtr m_spDriverData;
};

struct VeVideoDisplay
{
	VeFixedString m_kName;
	VeVector<VeDisplayMode> m_kDisplayModes;
	VeDisplayMode m_kDesktopMode;
	VeDisplayMode m_kCurrentMode;
	void* m_pvFullscreenWindow = nullptr;
	VeRefObjectPtr m_spDriverData;
};

struct VeWindowUserData
{
	VeChar8* m_pcName = nullptr;
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

#ifdef VE_PLATFORM_WIN
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
		VeInt32 dummy = 0;
	};
};

class VE_MAIN_API VeWindow : public VeRefObject
{
	VeNoCopy(VeWindow);
public:
	struct Data
	{
		VeRefNode<Data*> m_kNode;
		VeUInt32 m_u32Id = 0;
		VeFixedString m_kTitle;
		VeSurfacePtr m_spIcon;
		VeInt32 x = 0, y = 0;
		VeInt32 w = 0, h = 0;
		VeInt32 min_w = 0, min_h = 0;
		VeInt32 max_w = 0, max_h = 0;
		VeUInt32 m_u32Flags = 0;
		VeUInt32 m_u32LastFullscreenFlags = 0;
		VeRect m_kWindowed;
		VeDisplayMode m_kFullscreenMode;
		VeFloat32 m_f32Brightness = 0;
		VeUInt16* m_pu16Gamma = nullptr;
		VeUInt16* m_pu16SavedGamma = nullptr;
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

	inline VeUInt32 GetID() const noexcept;

	inline VeUInt32 GetFlags() const noexcept;

	inline const VeChar8* GetTitle() const noexcept;

	inline VeInt32 GetPosX() const noexcept;

	inline VeInt32 GetPosY() const noexcept;

	inline VeInt32 GetWidth() const noexcept;

	inline VeInt32 GetHeight() const noexcept;

	inline VeInt32 GetMinimumWidth() const noexcept;

	inline VeInt32 GetMinimumHeight() const noexcept;

	inline VeInt32 GetMaximumWidth() const noexcept;

	inline VeInt32 GetMaximumHeight() const noexcept;

	inline VeFloat32 GetBrightness() const noexcept;

	bool Create(const VeChar8* pcTitle, VeInt32 x, VeInt32 y,
		VeInt32 w, VeInt32 h, VeUInt32 u32Flags) noexcept;

	bool Create(const void* pvData) noexcept;

	void SetTitle(const VeChar8* pcTitle) noexcept;

	void SetPosition(VeInt32 x, VeInt32 y) noexcept;

	void SetSize(VeInt32 w, VeInt32 h) noexcept;

	void SetMinimumSize(VeInt32 min_w, VeInt32 min_h) noexcept;

	void SetMaximumSize(VeInt32 max_w, VeInt32 max_h) noexcept;

	void Show() noexcept;

	void Hide() noexcept;

	void Raise() noexcept;

	void Maximize() noexcept;

	void Minimize() noexcept;

	void Restore() noexcept;

	void SetBordered(bool bBordered) noexcept;

	void SetFullscreen(VeUInt32 u32Flags) noexcept;

	bool SetBrightness(VeFloat32 f32Brightness) noexcept;

	void SetGrab(bool bGrabbed) noexcept;

	void Destory() noexcept;

	VeSysWMInfo GetWMInfo() noexcept;

	static VeWindow* Cast(VeWindow::Data* pkData) noexcept;

protected:
	Data m_kData;

};

VeSmartPointer(VeWindow);

#include "VeWindow.inl"
