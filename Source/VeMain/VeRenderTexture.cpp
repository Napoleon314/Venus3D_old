////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderResource.cpp
//  Created:     2015/09/14 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderTexture, VeRenderResource);
//--------------------------------------------------------------------------
VeSizeT VeRenderTexture::GetSubResDesc(SubResDesc* pkDesc) noexcept
{
	VeUInt32 u32UnitWidth, u32UnitHeight, u32UnitSize;
	switch (m_eFormat)
	{
	case FORMAT_R32G32B32A32_TYPELESS:
	case FORMAT_R32G32B32A32_FLOAT:
	case FORMAT_R32G32B32A32_UINT:
	case FORMAT_R32G32B32A32_SINT:
	{
		u32UnitWidth = m_u32Width;
		u32UnitHeight = m_u32Height;
		u32UnitSize = 16;
	}
	break;
	case FORMAT_R32G32B32_TYPELESS:
	case FORMAT_R32G32B32_FLOAT:
	case FORMAT_R32G32B32_UINT:
	case FORMAT_R32G32B32_SINT:
	{
		u32UnitWidth = m_u32Width;
		u32UnitHeight = m_u32Height;
		u32UnitSize = 12;
	}
	break;
	case FORMAT_R16G16B16A16_TYPELESS:
	case FORMAT_R16G16B16A16_FLOAT:
	case FORMAT_R16G16B16A16_UNORM:
	case FORMAT_R16G16B16A16_UINT:
	case FORMAT_R16G16B16A16_SNORM:
	case FORMAT_R16G16B16A16_SINT:
	case FORMAT_R32G32_TYPELESS:
	case FORMAT_R32G32_FLOAT:
	case FORMAT_R32G32_UINT:
	case FORMAT_R32G32_SINT:
	case FORMAT_R32G8X24_TYPELESS:
	case FORMAT_D32_FLOAT_S8X24_UINT:
	case FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case FORMAT_X32_TYPELESS_G8X24_UINT:
	case FORMAT_Y416:
	case FORMAT_Y210:
	case FORMAT_Y216:
	{
		u32UnitWidth = m_u32Width;
		u32UnitHeight = m_u32Height;
		u32UnitSize = 8;
	}
	break;
	case FORMAT_R10G10B10A2_TYPELESS:
	case FORMAT_R10G10B10A2_UNORM:
	case FORMAT_R10G10B10A2_UINT:
	case FORMAT_R11G11B10_FLOAT:
	case FORMAT_R8G8B8A8_TYPELESS:
	case FORMAT_R8G8B8A8_UNORM:
	case FORMAT_R8G8B8A8_UNORM_SRGB:
	case FORMAT_R8G8B8A8_UINT:
	case FORMAT_R8G8B8A8_SNORM:
	case FORMAT_R8G8B8A8_SINT:
	case FORMAT_R16G16_TYPELESS:
	case FORMAT_R16G16_FLOAT:
	case FORMAT_R16G16_UNORM:
	case FORMAT_R16G16_UINT:
	case FORMAT_R16G16_SNORM:
	case FORMAT_R16G16_SINT:
	case FORMAT_R32_TYPELESS:
	case FORMAT_D32_FLOAT:
	case FORMAT_R32_FLOAT:
	case FORMAT_R32_UINT:
	case FORMAT_R32_SINT:
	case FORMAT_R24G8_TYPELESS:
	case FORMAT_D24_UNORM_S8_UINT:
	case FORMAT_R24_UNORM_X8_TYPELESS:
	case FORMAT_X24_TYPELESS_G8_UINT:
	case FORMAT_R9G9B9E5_SHAREDEXP:
	case FORMAT_R8G8_B8G8_UNORM:
	case FORMAT_G8R8_G8B8_UNORM:
	case FORMAT_B8G8R8A8_UNORM:
	case FORMAT_B8G8R8X8_UNORM:
	case FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
	case FORMAT_B8G8R8A8_TYPELESS:
	case FORMAT_B8G8R8A8_UNORM_SRGB:
	case FORMAT_B8G8R8X8_TYPELESS:
	case FORMAT_B8G8R8X8_UNORM_SRGB:
	case FORMAT_AYUV:
	case FORMAT_Y410:
	case FORMAT_YUY2:
	{
		u32UnitWidth = m_u32Width;
		u32UnitHeight = m_u32Height;
		u32UnitSize = 4;
	}
	break;
	case FORMAT_P010:
	case FORMAT_P016:
	{
		u32UnitWidth = m_u32Width;
		u32UnitHeight = m_u32Height;
		u32UnitSize = 3;
	}
	break;
	case FORMAT_R8G8_TYPELESS:
	case FORMAT_R8G8_UNORM:
	case FORMAT_R8G8_UINT:
	case FORMAT_R8G8_SNORM:
	case FORMAT_R8G8_SINT:
	case FORMAT_R16_TYPELESS:
	case FORMAT_R16_FLOAT:
	case FORMAT_D16_UNORM:
	case FORMAT_R16_UNORM:
	case FORMAT_R16_UINT:
	case FORMAT_R16_SNORM:
	case FORMAT_R16_SINT:
	case FORMAT_B5G6R5_UNORM:
	case FORMAT_B5G5R5A1_UNORM:
	case FORMAT_A8P8:
	case FORMAT_B4G4R4A4_UNORM:
	{
		u32UnitWidth = m_u32Width;
		u32UnitHeight = m_u32Height;
		u32UnitSize = 2;
	}
	break;
	case FORMAT_R8_TYPELESS:
	case FORMAT_R8_UNORM:
	case FORMAT_R8_UINT:
	case FORMAT_R8_SNORM:
	case FORMAT_R8_SINT:
	case FORMAT_A8_UNORM:
	case FORMAT_AI44:
	case FORMAT_IA44:
	case FORMAT_P8:
	{
		u32UnitWidth = m_u32Width;
		u32UnitHeight = m_u32Height;
		u32UnitSize = 1;
	}
	break;
	case FORMAT_BC1_TYPELESS:
	case FORMAT_BC1_UNORM:
	case FORMAT_BC1_UNORM_SRGB:
	case FORMAT_BC4_TYPELESS:
	case FORMAT_BC4_UNORM:
	case FORMAT_BC4_SNORM:
	{
		u32UnitWidth = m_u32Width >> 2;
		u32UnitHeight = m_u32Height >> 2;
		u32UnitSize = 8;
	}
	break;
	case FORMAT_BC2_TYPELESS:
	case FORMAT_BC2_UNORM:
	case FORMAT_BC2_UNORM_SRGB:
	case FORMAT_BC3_TYPELESS:
	case FORMAT_BC3_UNORM:
	case FORMAT_BC3_UNORM_SRGB:
	case FORMAT_BC5_TYPELESS:
	case FORMAT_BC5_UNORM:
	case FORMAT_BC5_SNORM:
	case FORMAT_BC6H_TYPELESS:
	case FORMAT_BC6H_UF16:
	case FORMAT_BC6H_SF16:
	case FORMAT_BC7_TYPELESS:
	case FORMAT_BC7_UNORM:
	case FORMAT_BC7_UNORM_SRGB:
	{
		u32UnitWidth = m_u32Width >> 2;
		u32UnitHeight = m_u32Height >> 2;
		u32UnitSize = 16;
	}
	break;
	default:
		return 0;
	}
	VeSizeT stSize(0);

	for (VeUInt32 i(0); i < m_u16MipLevels; ++i)
	{
		VeUInt32 w = u32UnitWidth >> i;
		w = w ? w : 1;
		VeUInt32 h = u32UnitHeight >> i;
		h = h ? h : 1;
		pkDesc[i].m_u32RowPitch = w * u32UnitSize;
		pkDesc[i].m_u32RowNum = h;
		pkDesc[i].m_u32SlicePitch = h * pkDesc[i].m_u32RowPitch;
		stSize += pkDesc[i].m_u32SlicePitch;
	}
	return stSize * m_u16Depth;
}
//--------------------------------------------------------------------------
