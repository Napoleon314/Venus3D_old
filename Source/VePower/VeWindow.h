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
	VeUInt32 m_u32Format;
	VeInt32 m_i32Width;
	VeInt32 m_i32Height;
	VeInt32 m_i32RefreshRate;
	void* m_pvDriverData;
};

struct VeWindowUserData
{
	VeChar8* m_pcName;
	void* m_pvData;
	VeWindowUserData* m_pkNext;
};

class VE_POWER_API VeWindow : public VeRefObject
{
public:
	virtual ~VeWindow() noexcept {}

protected:
	VeWindow() noexcept {}

	VeRefNode<VeWindow*> m_kNode;

	const void* m_pvMagic;
	VeUInt32 m_u32Id;
	VeChar8* m_pcTitle;
	void* icon;
	VeInt32 x, y;
	VeInt32 w, h;
	VeInt32 min_w, min_h;
	VeInt32 max_w, max_h;
	VeUInt32 m_u32Flags;
	VeUInt32 m_u32LastFullscreenFlags;
	VeRect m_kWindowed;
	VeDisplayMode m_kFullscreenMode;
	VeFloat32 m_f32Brightness;
	VeUInt16 m_u16Gamma;
	VeUInt16 m_u16SavedGamma;
	void* surface;
	VE_BOOL m_bSurfaceValid;
	VE_BOOL m_bIsDestorying;
	void* shaper;
	VeWindowUserData* m_pkData;
	void* m_pvDriverdata;

};

VeSmartPointer(VeWindow);
