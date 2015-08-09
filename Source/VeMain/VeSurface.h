////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeSurface.h
//  Created:     2015/08/08 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#define VE_SWSURFACE       0
#define VE_PREALLOC        0x00000001
#define VE_RLEACCEL        0x00000002
#define VE_DONTFREE        0x00000004

#define VE_MUSTLOCK(S) (((S)->flags & VE_RLEACCEL) != 0)

VeSmartPointer(VeSurface);
VeSmartPointer(VeBlob);

class VE_MAIN_API VeSurface : public VeRefObject
{
public:
	inline bool SetPalette(const VePalettePtr& spPalette) noexcept;

	inline VeInt32 GetWidth() noexcept;

	inline VeInt32 GetHeight() noexcept;

	inline VeInt32 GetPitch() noexcept;

	inline VePixelFormatPtr GetFormat() noexcept;

	void* GetBuffer() noexcept;
	
	static VeSurfacePtr CreateRGBSurface(VeUInt32 u32Flags,
		VeInt32 i32Width, VeInt32 i32Height, VeInt32 i32Depth,
		VeUInt32 u32Rmask, VeUInt32 u32Gmask, VeUInt32 u32Bmask,
		VeUInt32 u32Amask) noexcept;

protected:
	VeSurface() noexcept;

	virtual ~VeSurface() noexcept;

	VeInt32 CalculatePitch() noexcept;

	VeUInt32 m_u32Flags = 0;
	VePixelFormatPtr m_spFormat;
	VeInt32 m_i32Width = 0;
	VeInt32 m_i32Height = 0;
	VeInt32 m_i32Pitch = 0;
	VeBlobPtr m_spPixels;

};

#include "VeSurface.inl"
