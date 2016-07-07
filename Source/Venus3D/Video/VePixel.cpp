////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Video
//  File name:   VePixel.cpp
//  Created:     2016/07/07 by Albert
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
VePalette::VePalette(size_t stNumColors) noexcept
{
	assert(stNumColors > 0);
	m_kColors.resize(stNumColors);
	memset(&m_kColors[0], 0xFF, stNumColors * sizeof(VeRGBA));
	m_u32Version = 1;
}
//--------------------------------------------------------------------------
vtd::intrusive_list<VePixelFormat*> VePixelFormat::ms_kList;
//--------------------------------------------------------------------------
VePixelFormat::VePixelFormat()
{
	m_kNode.m_Content = this;
}

bool VePixelFormat::Init(uint32_t u32PixelFormat) noexcept
{
	int32_t bpp;
	uint32_t Rmask, Gmask, Bmask, Amask;
	uint32_t mask;

	if (!VePixelFormatEnumToMasks(u32PixelFormat,
		bpp, Rmask, Gmask, Bmask, Amask)) return false;

	m_u32Format = u32PixelFormat;
	m_u8BitsPerPixel = (uint8_t)bpp;
	m_u8BytesPerPixel = (uint8_t)((bpp + 7) >> 3);

	m_u32Rmask = Rmask;
	m_u8Rshift = 0;
	m_u8Rloss = 8;
	if (Rmask)
	{
		for (mask = Rmask; !(mask & 0x01); mask >>= 1)
			++m_u8Rshift;
		for (; (mask & 0x01); mask >>= 1)
			--m_u8Rloss;
	}

	m_u32Gmask = Gmask;
	m_u8Gshift = 0;
	m_u8Gloss = 8;
	if (Gmask)
	{
		for (mask = Rmask; !(mask & 0x01); mask >>= 1)
			++m_u8Gshift;
		for (; (mask & 0x01); mask >>= 1)
			--m_u8Gloss;
	}

	m_u32Bmask = Bmask;
	m_u8Bshift = 0;
	m_u8Bloss = 8;
	if (Bmask)
	{
		for (mask = Bmask; !(mask & 0x01); mask >>= 1)
			++m_u8Bshift;
		for (; (mask & 0x01); mask >>= 1)
			--m_u8Bloss;
	}

	m_u32Amask = Amask;
	m_u8Ashift = 0;
	m_u8Aloss = 8;
	if (Amask)
	{
		for (mask = Amask; !(mask & 0x01); mask >>= 1)
			++m_u8Ashift;
		for (; (mask & 0x01); mask >>= 1)
			--m_u8Aloss;
	}

	m_spPalette = nullptr;
	return true;
}
//--------------------------------------------------------------------------
VePixelFormatPtr VePixelFormat::Create(uint32_t u32PixelFormat) noexcept
{
	for (auto obj : ms_kList)
	{
		if (obj->m_u32Format == u32PixelFormat)
		{
			return obj;
		}
	}

	VePixelFormatPtr spFormat = new VePixelFormat();
	assert(spFormat);
	if (!spFormat->Init(u32PixelFormat))
	{
		return nullptr;
	}

	if (!VE_ISPIXELFORMAT_INDEXED(u32PixelFormat))
	{
		ms_kList.attach_back(spFormat->m_kNode);
	}

	return spFormat;
}
//--------------------------------------------------------------------------
const char* VeGetPixelFormatName(uint32_t u32Format) noexcept
{
	switch (u32Format)
	{
#define CASE(X) case X: return #X;
		CASE(VE_PIXELFORMAT_INDEX1LSB)
			CASE(VE_PIXELFORMAT_INDEX1MSB)
			CASE(VE_PIXELFORMAT_INDEX4LSB)
			CASE(VE_PIXELFORMAT_INDEX4MSB)
			CASE(VE_PIXELFORMAT_INDEX8)
			CASE(VE_PIXELFORMAT_RGB332)
			CASE(VE_PIXELFORMAT_RGB444)
			CASE(VE_PIXELFORMAT_RGB555)
			CASE(VE_PIXELFORMAT_BGR555)
			CASE(VE_PIXELFORMAT_ARGB4444)
			CASE(VE_PIXELFORMAT_RGBA4444)
			CASE(VE_PIXELFORMAT_ABGR4444)
			CASE(VE_PIXELFORMAT_BGRA4444)
			CASE(VE_PIXELFORMAT_ARGB1555)
			CASE(VE_PIXELFORMAT_RGBA5551)
			CASE(VE_PIXELFORMAT_ABGR1555)
			CASE(VE_PIXELFORMAT_BGRA5551)
			CASE(VE_PIXELFORMAT_RGB565)
			CASE(VE_PIXELFORMAT_BGR565)
			CASE(VE_PIXELFORMAT_RGB24)
			CASE(VE_PIXELFORMAT_BGR24)
			CASE(VE_PIXELFORMAT_RGB888)
			CASE(VE_PIXELFORMAT_RGBX8888)
			CASE(VE_PIXELFORMAT_BGR888)
			CASE(VE_PIXELFORMAT_BGRX8888)
			CASE(VE_PIXELFORMAT_ARGB8888)
			CASE(VE_PIXELFORMAT_RGBA8888)
			CASE(VE_PIXELFORMAT_ABGR8888)
			CASE(VE_PIXELFORMAT_BGRA8888)
			CASE(VE_PIXELFORMAT_ARGB2101010)
			CASE(VE_PIXELFORMAT_YV12)
			CASE(VE_PIXELFORMAT_IYUV)
			CASE(VE_PIXELFORMAT_YUY2)
			CASE(VE_PIXELFORMAT_UYVY)
			CASE(VE_PIXELFORMAT_YVYU)
#undef CASE
	default:
		return "VE_PIXELFORMAT_UNKNOWN";
	}
}
//--------------------------------------------------------------------------
bool VePixelFormatEnumToMasks(uint32_t u32Format, int32_t& i32Bpp,
	uint32_t& u32Rmask, uint32_t& u32Gmask, uint32_t& u32Bmask,
	uint32_t& u32Amask) noexcept
{
	uint32_t masks[4];

	if (VE_ISPIXELFORMAT_FOURCC(u32Format))
	{
		return false;
	}

	if (VE_BYTESPERPIXEL(u32Format) <= 2)
	{
		i32Bpp = VE_BITSPERPIXEL(u32Format);
	}
	else
	{
		i32Bpp = VE_BYTESPERPIXEL(u32Format) * 8;
	}
	u32Rmask = u32Gmask = u32Bmask = u32Amask = 0;

	if (u32Format == VE_PIXELFORMAT_RGB24)
	{
		u32Rmask = 0x000000FF;
		u32Gmask = 0x0000FF00;
		u32Bmask = 0x00FF0000;
		return true;
	}

	if (u32Format == VE_PIXELFORMAT_BGR24)
	{
		u32Rmask = 0x00FF0000;
		u32Gmask = 0x0000FF00;
		u32Bmask = 0x000000FF;
		return true;
	}

	if (VE_PIXELTYPE(u32Format) != VE_PIXELTYPE_PACKED8
		&& VE_PIXELTYPE(u32Format) != VE_PIXELTYPE_PACKED16
		&& VE_PIXELTYPE(u32Format) != VE_PIXELTYPE_PACKED32)
	{
		return true;
	}

	switch (VE_PIXELLAYOUT(u32Format))
	{
	case VE_PACKEDLAYOUT_332:
		masks[0] = 0x00000000;
		masks[1] = 0x000000E0;
		masks[2] = 0x0000001C;
		masks[3] = 0x00000003;
		break;
	case VE_PACKEDLAYOUT_4444:
		masks[0] = 0x0000F000;
		masks[1] = 0x00000F00;
		masks[2] = 0x000000F0;
		masks[3] = 0x0000000F;
		break;
	case VE_PACKEDLAYOUT_1555:
		masks[0] = 0x00008000;
		masks[1] = 0x00007C00;
		masks[2] = 0x000003E0;
		masks[3] = 0x0000001F;
		break;
	case VE_PACKEDLAYOUT_5551:
		masks[0] = 0x0000F800;
		masks[1] = 0x000007C0;
		masks[2] = 0x0000003E;
		masks[3] = 0x00000001;
		break;
	case VE_PACKEDLAYOUT_565:
		masks[0] = 0x00000000;
		masks[1] = 0x0000F800;
		masks[2] = 0x000007E0;
		masks[3] = 0x0000001F;
		break;
	case VE_PACKEDLAYOUT_8888:
		masks[0] = 0xFF000000;
		masks[1] = 0x00FF0000;
		masks[2] = 0x0000FF00;
		masks[3] = 0x000000FF;
		break;
	case VE_PACKEDLAYOUT_2101010:
		masks[0] = 0xC0000000;
		masks[1] = 0x3FF00000;
		masks[2] = 0x000FFC00;
		masks[3] = 0x000003FF;
		break;
	case VE_PACKEDLAYOUT_1010102:
		masks[0] = 0xFFC00000;
		masks[1] = 0x003FF000;
		masks[2] = 0x00000FFC;
		masks[3] = 0x00000003;
		break;
	default:
		return false;
	}

	switch (VE_PIXELORDER(u32Format))
	{
	case VE_PACKEDORDER_XRGB:
		u32Rmask = masks[1];
		u32Gmask = masks[2];
		u32Bmask = masks[3];
		break;
	case VE_PACKEDORDER_RGBX:
		u32Rmask = masks[0];
		u32Gmask = masks[1];
		u32Bmask = masks[2];
		break;
	case VE_PACKEDORDER_ARGB:
		u32Amask = masks[0];
		u32Rmask = masks[1];
		u32Gmask = masks[2];
		u32Bmask = masks[3];
		break;
	case VE_PACKEDORDER_RGBA:
		u32Rmask = masks[0];
		u32Gmask = masks[1];
		u32Bmask = masks[2];
		u32Amask = masks[3];
		break;
	case VE_PACKEDORDER_XBGR:
		u32Bmask = masks[1];
		u32Gmask = masks[2];
		u32Rmask = masks[3];
		break;
	case VE_PACKEDORDER_BGRX:
		u32Bmask = masks[0];
		u32Gmask = masks[1];
		u32Rmask = masks[2];
		break;
	case VE_PACKEDORDER_BGRA:
		u32Bmask = masks[0];
		u32Gmask = masks[1];
		u32Rmask = masks[2];
		u32Amask = masks[3];
		break;
	case VE_PACKEDORDER_ABGR:
		u32Amask = masks[0];
		u32Bmask = masks[1];
		u32Gmask = masks[2];
		u32Rmask = masks[3];
		break;
	default:
		return false;
	}
	return true;
}
//--------------------------------------------------------------------------
uint32_t VeMasksToPixelFormatEnum(int32_t i32Bpp, uint32_t u32Rmask,
	uint32_t u32Gmask, uint32_t u32Bmask, uint32_t u32Amask) noexcept
{
	switch (i32Bpp)
	{
	case 1:
		return VE_PIXELFORMAT_INDEX1MSB;
	case 4:
		return VE_PIXELFORMAT_INDEX4MSB;
	case 8:
		if (u32Rmask == 0)
		{
			return VE_PIXELFORMAT_INDEX8;
		}
		if (u32Rmask == 0xE0 &&
			u32Gmask == 0x1C &&
			u32Bmask == 0x03 &&
			u32Amask == 0x00)
		{
			return VE_PIXELFORMAT_RGB332;
		}
		break;
	case 12:
		if (u32Rmask == 0)
		{
			return VE_PIXELFORMAT_RGB444;
		}
		if (u32Rmask == 0x0F00 &&
			u32Gmask == 0x00F0 &&
			u32Bmask == 0x000F &&
			u32Amask == 0x0000)
		{
			return VE_PIXELFORMAT_RGB444;
		}
		break;
	case 15:
		if (u32Rmask == 0)
		{
			return VE_PIXELFORMAT_RGB555;
		}
	case 16:
		if (u32Rmask == 0)
		{
			return VE_PIXELFORMAT_RGB565;
		}
		if (u32Rmask == 0x7C00 &&
			u32Gmask == 0x03E0 &&
			u32Bmask == 0x001F &&
			u32Amask == 0x0000)
		{
			return VE_PIXELFORMAT_RGB555;
		}
		if (u32Rmask == 0x001F &&
			u32Gmask == 0x03E0 &&
			u32Bmask == 0x7C00 &&
			u32Amask == 0x0000)
		{
			return VE_PIXELFORMAT_BGR555;
		}
		if (u32Rmask == 0x0F00 &&
			u32Gmask == 0x00F0 &&
			u32Bmask == 0x000F &&
			u32Amask == 0xF000)
		{
			return VE_PIXELFORMAT_ARGB4444;
		}
		if (u32Rmask == 0xF000 &&
			u32Gmask == 0x0F00 &&
			u32Bmask == 0x00F0 &&
			u32Amask == 0x000F)
		{
			return VE_PIXELFORMAT_RGBA4444;
		}
		if (u32Rmask == 0x000F &&
			u32Gmask == 0x00F0 &&
			u32Bmask == 0x0F00 &&
			u32Amask == 0xF000)
		{
			return VE_PIXELFORMAT_ABGR4444;
		}
		if (u32Rmask == 0x00F0 &&
			u32Gmask == 0x0F00 &&
			u32Bmask == 0xF000 &&
			u32Amask == 0x000F)
		{
			return VE_PIXELFORMAT_BGRA4444;
		}
		if (u32Rmask == 0x7C00 &&
			u32Gmask == 0x03E0 &&
			u32Bmask == 0x001F &&
			u32Amask == 0x8000)
		{
			return VE_PIXELFORMAT_ARGB1555;
		}
		if (u32Rmask == 0xF800 &&
			u32Gmask == 0x07C0 &&
			u32Bmask == 0x003E &&
			u32Amask == 0x0001)
		{
			return VE_PIXELFORMAT_RGBA5551;
		}
		if (u32Rmask == 0x001F &&
			u32Gmask == 0x03E0 &&
			u32Bmask == 0x7C00 &&
			u32Amask == 0x8000)
		{
			return VE_PIXELFORMAT_ABGR1555;
		}
		if (u32Rmask == 0x003E &&
			u32Gmask == 0x07C0 &&
			u32Bmask == 0xF800 &&
			u32Amask == 0x0001)
		{
			return VE_PIXELFORMAT_BGRA5551;
		}
		if (u32Rmask == 0xF800 &&
			u32Gmask == 0x07E0 &&
			u32Bmask == 0x001F &&
			u32Amask == 0x0000)
		{
			return VE_PIXELFORMAT_RGB565;
		}
		if (u32Rmask == 0x001F &&
			u32Gmask == 0x07E0 &&
			u32Bmask == 0xF800 &&
			u32Amask == 0x0000)
		{
			return VE_PIXELFORMAT_BGR565;
		}
		break;
	case 24:
		switch (u32Rmask)
		{
		case 0:
		case 0x00FF0000:
			return VE_PIXELFORMAT_BGR24;
		case 0x000000FF:
			return VE_PIXELFORMAT_RGB24;
		}
	case 32:
		if (u32Rmask == 0)
		{
			return VE_PIXELFORMAT_RGB888;
		}
		if (u32Rmask == 0x00FF0000 &&
			u32Gmask == 0x0000FF00 &&
			u32Bmask == 0x000000FF &&
			u32Amask == 0x00000000)
		{
			return VE_PIXELFORMAT_RGB888;
		}
		if (u32Rmask == 0xFF000000 &&
			u32Gmask == 0x00FF0000 &&
			u32Bmask == 0x0000FF00 &&
			u32Amask == 0x00000000)
		{
			return VE_PIXELFORMAT_RGBX8888;
		}
		if (u32Rmask == 0x000000FF &&
			u32Gmask == 0x0000FF00 &&
			u32Bmask == 0x00FF0000 &&
			u32Amask == 0x00000000)
		{
			return VE_PIXELFORMAT_BGR888;
		}
		if (u32Rmask == 0x0000FF00 &&
			u32Gmask == 0x00FF0000 &&
			u32Bmask == 0xFF000000 &&
			u32Amask == 0x00000000)
		{
			return VE_PIXELFORMAT_BGRX8888;
		}
		if (u32Rmask == 0x00FF0000 &&
			u32Gmask == 0x0000FF00 &&
			u32Bmask == 0x000000FF &&
			u32Amask == 0xFF000000)
		{
			return VE_PIXELFORMAT_ARGB8888;
		}
		if (u32Rmask == 0xFF000000 &&
			u32Gmask == 0x00FF0000 &&
			u32Bmask == 0x0000FF00 &&
			u32Amask == 0x000000FF)
		{
			return VE_PIXELFORMAT_RGBA8888;
		}
		if (u32Rmask == 0x000000FF &&
			u32Gmask == 0x0000FF00 &&
			u32Bmask == 0x00FF0000 &&
			u32Amask == 0xFF000000)
		{
			return VE_PIXELFORMAT_ABGR8888;
		}
		if (u32Rmask == 0x0000FF00 &&
			u32Gmask == 0x00FF0000 &&
			u32Bmask == 0xFF000000 &&
			u32Amask == 0x000000FF)
		{
			return VE_PIXELFORMAT_BGRA8888;
		}
		if (u32Rmask == 0x3FF00000 &&
			u32Gmask == 0x000FFC00 &&
			u32Bmask == 0x000003FF &&
			u32Amask == 0xC0000000)
		{
			return VE_PIXELFORMAT_ARGB2101010;
		}
	}
	return VE_PIXELFORMAT_UNKNOWN;
}
//--------------------------------------------------------------------------
