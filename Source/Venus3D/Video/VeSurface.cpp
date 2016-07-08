////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Video
//  File name:   VeSurface.cpp
//  Created:     2016/07/08 by Albert
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

#include "stdafx.h"

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
	assert(m_spPixels);
	return m_spPixels->GetBuffer();
}
//--------------------------------------------------------------------------
VeSurfacePtr VeSurface::CreateRGBSurface(uint32_t,
	int32_t i32Width, int32_t i32Height, int32_t i32Depth,
	uint32_t u32Rmask, uint32_t u32Gmask, uint32_t u32Bmask,
	uint32_t u32Amask) noexcept
{
	uint32_t u32Format = VeMasksToPixelFormatEnum(i32Depth, u32Rmask,
		u32Gmask, u32Bmask, u32Amask);
	if (u32Format == VE_PIXELFORMAT_UNKNOWN) return nullptr;

	VeSurfacePtr spSurface = VE_NEW VeSurface();
	assert(spSurface);
	spSurface->m_spFormat = VePixelFormat::Create(u32Format);
	if (!spSurface->m_spFormat) return nullptr;
	
	spSurface->m_i32Width = i32Width;
	spSurface->m_i32Height = i32Height;
	spSurface->m_i32Pitch = spSurface->CalculatePitch();

	if (VE_ISPIXELFORMAT_INDEXED(spSurface->m_spFormat->m_u32Format))
	{
		VePalettePtr spPattle = VE_NEW VePalette((1 << spSurface->m_spFormat->m_u8BitsPerPixel));
		assert(spPattle);
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
		size_t stSize = spSurface->m_i32Height * spSurface->m_i32Pitch;
		spSurface->m_spPixels = VE_NEW VeBlob(stSize);
		memset(*spSurface->m_spPixels, 0, stSize);
	}

	return spSurface;
}
//--------------------------------------------------------------------------
int32_t VeSurface::CalculatePitch() noexcept
{
	int32_t i32Pitch;
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
