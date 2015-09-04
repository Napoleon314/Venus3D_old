////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeSurface.cpp
//  Created:     2015/08/08 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeSurface::VeSurface() noexcept
{

}
//--------------------------------------------------------------------------
VeSurface::~VeSurface() noexcept
{

}
//--------------------------------------------------------------------------
void* VeSurface::GetBuffer() noexcept
{
	VE_ASSERT(m_spPixels);
	return m_spPixels->GetBuffer();
}
//--------------------------------------------------------------------------
VeSurfacePtr VeSurface::CreateRGBSurface(VeUInt32 u32Flags,
	VeInt32 i32Width, VeInt32 i32Height, VeInt32 i32Depth,
	VeUInt32 u32Rmask, VeUInt32 u32Gmask, VeUInt32 u32Bmask,
	VeUInt32 u32Amask) noexcept
{
	VeUInt32 u32Format = VeMasksToPixelFormatEnum(i32Depth, u32Rmask,
		u32Gmask, u32Bmask, u32Amask);
	if (u32Format == VE_PIXELFORMAT_UNKNOWN) return nullptr;

	VeSurfacePtr spSurface = VE_NEW VeSurface();
	VE_ASSERT(spSurface);
	spSurface->m_spFormat = VePixelFormat::Create(u32Format);
	if (!spSurface->m_spFormat) return nullptr;
	
	spSurface->m_i32Width = i32Width;
	spSurface->m_i32Height = i32Height;
	spSurface->m_i32Pitch = spSurface->CalculatePitch();

	if (VE_ISPIXELFORMAT_INDEXED(spSurface->m_spFormat->m_u32Format))
	{
		VePalettePtr spPattle = VE_NEW VePalette((1 << spSurface->m_spFormat->m_u8BitsPerPixel));
		VE_ASSERT(spPattle);
		if (spPattle->m_kColors.size() == 2)
		{
			spPattle->m_kColors[0].r = 0xFF;
			spPattle->m_kColors[0].g = 0xFF;
			spPattle->m_kColors[0].b = 0xFF;
			spPattle->m_kColors[1].r = 0x00;
			spPattle->m_kColors[1].g = 0x00;
			spPattle->m_kColors[1].b = 0x00;
		}
		spSurface->SetPalette(spPattle);
	}

	if (spSurface->m_i32Width && spSurface->m_i32Height)
	{
		VeSizeT stSize = spSurface->m_i32Height * spSurface->m_i32Pitch;
		spSurface->m_spPixels = VE_NEW VeBlob(stSize);
		VeZeroMemory(*spSurface->m_spPixels, stSize);
	}

	return spSurface;
}
//--------------------------------------------------------------------------
VeInt32 VeSurface::CalculatePitch() noexcept
{
	VeInt32 i32Pitch;
	i32Pitch = m_i32Width * m_spFormat->m_u8BytesPerPixel;
	switch (m_spFormat->m_u8BitsPerPixel)
	{
	case 1:
		i32Pitch = (i32Pitch + 7) >> 3;
		break;
	case 4:
		i32Pitch = (i32Pitch + 1) >> 1;
		break;
	default:
		break;
	}
	i32Pitch = (i32Pitch + 3) & ~3;
	return (i32Pitch);
}
//--------------------------------------------------------------------------
