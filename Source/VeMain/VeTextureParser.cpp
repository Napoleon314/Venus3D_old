////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeTextureParser.cpp
//  Created:     2015/09/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
#define	REQ_BLEND_OBJECT_COUNT_PER_DEVICE	( 4096 )
#define	REQ_BUFFER_RESOURCE_TEXEL_COUNT_2_TO_EXP	( 27 )
#define	REQ_CONSTANT_BUFFER_ELEMENT_COUNT	( 4096 )
#define	REQ_DEPTH_STENCIL_OBJECT_COUNT_PER_DEVICE	( 4096 )
#define	REQ_DRAWINDEXED_INDEX_COUNT_2_TO_EXP	( 32 )
#define	REQ_DRAW_VERTEX_COUNT_2_TO_EXP	( 32 )
#define	REQ_FILTERING_HW_ADDRESSABLE_RESOURCE_DIMENSION	( 16384 )
#define	REQ_GS_INVOCATION_32BIT_OUTPUT_COMPONENT_LIMIT	( 1024 )
#define	REQ_IMMEDIATE_CONSTANT_BUFFER_ELEMENT_COUNT	( 4096 )
#define	REQ_MAXANISOTROPY	( 16 )
#define	REQ_MIP_LEVELS	( 15 )
#define	REQ_MULTI_ELEMENT_STRUCTURE_SIZE_IN_BYTES	( 2048 )
#define	REQ_RASTERIZER_OBJECT_COUNT_PER_DEVICE	( 4096 )
#define	REQ_RENDER_TO_BUFFER_WINDOW_WIDTH	( 16384 )
#define	REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM	( 128 )
#define REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_B_TERM	( 0.25f )
#define	REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_C_TERM	( 2048 )
#define	REQ_RESOURCE_VIEW_COUNT_PER_DEVICE_2_TO_EXP	( 20 )
#define	REQ_SAMPLER_OBJECT_COUNT_PER_DEVICE	( 4096 )
#define	REQ_SUBRESOURCES	( 30720 )
#define	REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION	( 2048 )
#define	REQ_TEXTURE1D_U_DIMENSION	( 16384 )
#define	REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION	( 2048 )
#define	REQ_TEXTURE2D_U_OR_V_DIMENSION	( 16384 )
#define	REQ_TEXTURE3D_U_V_OR_W_DIMENSION	( 2048 )
#define	REQ_TEXTURECUBE_DIMENSION	( 16384 )
//--------------------------------------------------------------------------
const VeUInt32 DDS_MAGIC = 0x20534444;
//--------------------------------------------------------------------------
struct DDS_PIXELFORMAT
{
	VeUInt32 m_u32Size;
	VeUInt32 m_u32Flags;
	VeUInt32 m_u32FourCC;
	VeUInt32 m_u32RGBBitCount;
	VeUInt32 m_u32RBitMask;
	VeUInt32 m_u32GBitMask;
	VeUInt32 m_u32BBitMask;
	VeUInt32 m_u32ABitMask;
};
//--------------------------------------------------------------------------
#define DDS_FOURCC      0x00000004  // DDPF_FOURCC
#define DDS_RGB         0x00000040  // DDPF_RGB
#define DDS_LUMINANCE   0x00020000  // DDPF_LUMINANCE
#define DDS_ALPHA       0x00000002  // DDPF_ALPHA
//--------------------------------------------------------------------------
#define DDS_HEADER_FLAGS_VOLUME         0x00800000  // DDSD_DEPTH
//--------------------------------------------------------------------------
#define DDS_HEIGHT 0x00000002 // DDSD_HEIGHT
#define DDS_WIDTH  0x00000004 // DDSD_WIDTH
//--------------------------------------------------------------------------
#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ
//--------------------------------------------------------------------------
#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )
//--------------------------------------------------------------------------
#define DDS_CUBEMAP 0x00000200 // DDSCAPS2_CUBEMAP
//--------------------------------------------------------------------------
enum RESOURCE_MISC_FLAG
{
	RESOURCE_MISC_GENERATE_MIPS = 0x1L,
	RESOURCE_MISC_SHARED = 0x2L,
	RESOURCE_MISC_TEXTURECUBE = 0x4L,
	RESOURCE_MISC_DRAWINDIRECT_ARGS = 0x10L,
	RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS = 0x20L,
	RESOURCE_MISC_BUFFER_STRUCTURED = 0x40L,
	RESOURCE_MISC_RESOURCE_CLAMP = 0x80L,
	RESOURCE_MISC_SHARED_KEYEDMUTEX = 0x100L,
	RESOURCE_MISC_GDI_COMPATIBLE = 0x200L,
	RESOURCE_MISC_SHARED_NTHANDLE = 0x800L,
	RESOURCE_MISC_RESTRICTED_CONTENT = 0x1000L,
	RESOURCE_MISC_RESTRICT_SHARED_RESOURCE = 0x2000L,
	RESOURCE_MISC_RESTRICT_SHARED_RESOURCE_DRIVER = 0x4000L,
	RESOURCE_MISC_GUARDED = 0x8000L
};
//--------------------------------------------------------------------------
enum DDS_MISC_FLAGS2
{
	DDS_MISC_FLAGS2_ALPHA_MODE_MASK = 0x7L,
};
//--------------------------------------------------------------------------
struct DDS_HEADER
{
	VeUInt32 m_u32Size;
	VeUInt32 m_u32Flags;
	VeUInt32 m_u32Height;
	VeUInt32 m_u32Width;
	VeUInt32 m_u32PitchOrLinearSize;
	VeUInt32 m_u32Depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
	VeUInt32 m_u32MipMapCount;
	VeUInt32 m_u32Reserved1[11];
	DDS_PIXELFORMAT m_kFormat;
	VeUInt32 m_u32Caps;
	VeUInt32 m_u32Caps2;
	VeUInt32 m_u32Caps3;
	VeUInt32 m_u32Caps4;
	VeUInt32 m_u32Reserved2;
};
//--------------------------------------------------------------------------
struct DDS_HEADER_DXT10
{
	VeRenderResource::Format m_eFormat;
	VeRenderResource::Dimension m_eResourceDimension;
	VeUInt32 m_u32MiscFlag; // see D3D11_RESOURCE_MISC_FLAG
	VeUInt32 m_u32ArraySize;
	VeUInt32 m_u32MiscFlags2;
};
//--------------------------------------------------------------------------
#define ISBITMASK( r,g,b,a ) ( ddpf.m_u32RBitMask == r && ddpf.m_u32GBitMask == g && ddpf.m_u32BBitMask == b && ddpf.m_u32ABitMask == a )
//--------------------------------------------------------------------------
static VeRenderResource::Format GetDXGIFormat(
	const DDS_PIXELFORMAT& ddpf) noexcept
{
	if (ddpf.m_u32Flags & DDS_RGB)
	{
		switch (ddpf.m_u32RGBBitCount)
		{
		case 32:
			if (ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
			{
				return VeRenderResource::FORMAT_R8G8B8A8_UNORM;
			}
			if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000))
			{
				return VeRenderResource::FORMAT_B8G8R8A8_UNORM;
			}
			if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000))
			{
				return VeRenderResource::FORMAT_B8G8R8X8_UNORM;
			}
			if (ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000))
			{
				return VeRenderResource::FORMAT_R10G10B10A2_UNORM;
			}
			if (ISBITMASK(0x0000ffff, 0xffff0000, 0x00000000, 0x00000000))
			{
				return VeRenderResource::FORMAT_R16G16_UNORM;
			}
			if (ISBITMASK(0xffffffff, 0x00000000, 0x00000000, 0x00000000))
			{
				return VeRenderResource::FORMAT_R32_FLOAT;
			}
			break;
		case 24:
			break;
		case 16:
			if (ISBITMASK(0x7c00, 0x03e0, 0x001f, 0x8000))
			{
				return VeRenderResource::FORMAT_B5G5R5A1_UNORM;
			}
			if (ISBITMASK(0xf800, 0x07e0, 0x001f, 0x0000))
			{
				return VeRenderResource::FORMAT_B5G6R5_UNORM;
			}
			if (ISBITMASK(0x0f00, 0x00f0, 0x000f, 0xf000))
			{
				return VeRenderResource::FORMAT_B4G4R4A4_UNORM;
			}
			break;
		}
	}
	else if (ddpf.m_u32Flags & DDS_LUMINANCE)
	{
		if (8 == ddpf.m_u32RGBBitCount)
		{
			if (ISBITMASK(0x000000ff, 0x00000000, 0x00000000, 0x00000000))
			{
				return VeRenderResource::FORMAT_R8_UNORM;
			}
		}

		if (16 == ddpf.m_u32RGBBitCount)
		{
			if (ISBITMASK(0x0000ffff, 0x00000000, 0x00000000, 0x00000000))
			{
				return VeRenderResource::FORMAT_R16_UNORM;
			}
			if (ISBITMASK(0x000000ff, 0x00000000, 0x00000000, 0x0000ff00))
			{
				return VeRenderResource::FORMAT_R8G8_UNORM;
			}
		}
	}
	else if (ddpf.m_u32Flags & DDS_ALPHA)
	{
		if (8 == ddpf.m_u32RGBBitCount)
		{
			return VeRenderResource::FORMAT_A8_UNORM;
		}
	}
	else if (ddpf.m_u32Flags & DDS_FOURCC)
	{
		if (VE_FOURCC('D', 'X', 'T', '1') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_BC1_UNORM;
		}
		if (VE_FOURCC('D', 'X', 'T', '3') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_BC2_UNORM;
		}
		if (VE_FOURCC('D', 'X', 'T', '5') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_BC3_UNORM;
		}
		if (VE_FOURCC('D', 'X', 'T', '2') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_BC2_UNORM;
		}
		if (VE_FOURCC('D', 'X', 'T', '4') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_BC3_UNORM;
		}
		if (VE_FOURCC('A', 'T', 'I', '1') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_BC4_UNORM;
		}
		if (VE_FOURCC('B', 'C', '4', 'U') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_BC4_UNORM;
		}
		if (VE_FOURCC('B', 'C', '4', 'S') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_BC4_SNORM;
		}
		if (VE_FOURCC('A', 'T', 'I', '2') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_BC5_UNORM;
		}
		if (VE_FOURCC('B', 'C', '5', 'U') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_BC5_UNORM;
		}
		if (VE_FOURCC('B', 'C', '5', 'S') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_BC5_SNORM;
		}
		if (VE_FOURCC('R', 'G', 'B', 'G') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_R8G8_B8G8_UNORM;
		}
		if (VE_FOURCC('G', 'R', 'G', 'B') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_G8R8_G8B8_UNORM;
		}
		if (VE_FOURCC('Y', 'U', 'Y', '2') == ddpf.m_u32FourCC)
		{
			return VeRenderResource::FORMAT_YUY2;
		}
		switch (ddpf.m_u32FourCC)
		{
		case 36: // D3DFMT_A16B16G16R16
			return VeRenderResource::FORMAT_R16G16B16A16_UNORM;
		case 110: // D3DFMT_Q16W16V16U16
			return VeRenderResource::FORMAT_R16G16B16A16_SNORM;
		case 111: // D3DFMT_R16F
			return VeRenderResource::FORMAT_R16_FLOAT;
		case 112: // D3DFMT_G16R16F
			return VeRenderResource::FORMAT_R16G16_FLOAT;
		case 113: // D3DFMT_A16B16G16R16F
			return VeRenderResource::FORMAT_R16G16B16A16_FLOAT;
		case 114: // D3DFMT_R32F
			return VeRenderResource::FORMAT_R32_FLOAT;
		case 115: // D3DFMT_G32R32F
			return VeRenderResource::FORMAT_R32G32_FLOAT;
		case 116: // D3DFMT_A32B32G32R32F
			return VeRenderResource::FORMAT_R32G32B32A32_FLOAT;
		}
	}
	return VeRenderResource::FORMAT_UNKNOWN;
}
//--------------------------------------------------------------------------
VeBlobPtr ParseDDS(VeTexture::FileInfo& kOut,
	VeMemoryIStream& kIn) noexcept
{
	if (kIn.RemainingLength() < sizeof(DDS_HEADER)) return nullptr;
	DDS_HEADER* pkHeader = kIn.To<DDS_HEADER>();
	if (pkHeader->m_u32Size != sizeof(DDS_HEADER) ||
		pkHeader->m_kFormat.m_u32Size != sizeof(DDS_PIXELFORMAT)) return nullptr;

	DDS_HEADER_DXT10* pkHeader10 = nullptr;
	if ((pkHeader->m_kFormat.m_u32Flags & DDS_FOURCC)
		&& (VE_FOURCC('D', 'X', '1', '0') == pkHeader->m_kFormat.m_u32FourCC))
	{
		if (kIn.RemainingLength() < sizeof(DDS_HEADER_DXT10)) return nullptr;
		pkHeader10 = kIn.To<DDS_HEADER_DXT10>();
	}

	VeUInt32 u32Width = pkHeader->m_u32Width;
	VeUInt32 u32Height = pkHeader->m_u32Height;
	VeUInt32 u32Depth = pkHeader->m_u32Depth;
	
	VeRenderResource::Dimension eResDim = VeRenderResource::DIMENSION_UNKNOWN;
	VeUInt32 u32ArraySize = 1;
	VeRenderResource::Format eFormat = VeRenderResource::FORMAT_UNKNOWN;
	bool bIsCubeMap = false;

	VeUInt32 u32MipCount = pkHeader->m_u32MipMapCount ? pkHeader->m_u32MipMapCount : 1;
	
	if (pkHeader10)
	{
		u32ArraySize = pkHeader10->m_u32ArraySize;
		if (u32ArraySize == 0) return nullptr;
		switch (pkHeader10->m_eFormat)
		{
		case VeRenderResource::FORMAT_AI44:
		case VeRenderResource::FORMAT_IA44:
		case VeRenderResource::FORMAT_P8:
		case VeRenderResource::FORMAT_A8P8:
			return nullptr;
		default:
			if (VeRenderResource::BitsPerPixel(pkHeader10->m_eFormat) == 0)
			{
				return nullptr;
			}
		}
		eFormat = pkHeader10->m_eFormat;

		switch (pkHeader10->m_eResourceDimension)
		{
		case VeRenderResource::DIMENSION_TEXTURE1D:
			if ((pkHeader->m_u32Flags & DDS_HEIGHT) && u32Height != 1)
			{
				return nullptr;
			}
			u32Height = u32Depth = 1;
			break;

		case VeRenderResource::DIMENSION_TEXTURE2D:
			if (pkHeader10->m_u32MiscFlag & RESOURCE_MISC_TEXTURECUBE)
			{
				u32ArraySize *= 6;
				bIsCubeMap = true;
			}
			u32Depth = 1;
			break;

		case VeRenderResource::DIMENSION_TEXTURE3D:
			if (!(pkHeader->m_u32Flags & DDS_HEADER_FLAGS_VOLUME))
			{
				return nullptr;
			}

			if (u32ArraySize > 1)
			{
				return nullptr;
			}
			break;

		default:
			return nullptr;
		}

		eResDim = pkHeader10->m_eResourceDimension;
	}
	else
	{
		eFormat = GetDXGIFormat(pkHeader->m_kFormat);
		if (eFormat == VeRenderResource::FORMAT_UNKNOWN)
		{
			return nullptr;
		}
		if (pkHeader->m_u32Flags & DDS_HEADER_FLAGS_VOLUME)
		{
			eResDim = VeRenderResource::DIMENSION_TEXTURE3D;
		}
		else
		{
			if (pkHeader->m_u32Caps2 & DDS_CUBEMAP)
			{
				if ((pkHeader->m_u32Caps2 & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES)
				{
					return nullptr;
				}
				u32ArraySize = 6;
				bIsCubeMap = true;
			}
			u32Depth = 1;
			eResDim = VeRenderResource::DIMENSION_TEXTURE2D;
		}
		VE_ASSERT(VeRenderResource::BitsPerPixel(eFormat) != 0);
	}
	
	if (u32MipCount > REQ_MIP_LEVELS)
	{
		return nullptr;
	}

	switch (eResDim)
	{
	case VeRenderResource::DIMENSION_TEXTURE1D:
		if ((u32ArraySize > REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION) ||
			(u32Width > REQ_TEXTURE1D_U_DIMENSION))
		{
			return nullptr;
		}
		break;
	case VeRenderResource::DIMENSION_TEXTURE2D:
		if (bIsCubeMap)
		{
			if ((u32ArraySize > REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
				(u32Width > REQ_TEXTURECUBE_DIMENSION) ||
				(u32Height > REQ_TEXTURECUBE_DIMENSION))
			{
				return nullptr;
			}
		}
		else if ((u32ArraySize > REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
			(u32Width > REQ_TEXTURE2D_U_OR_V_DIMENSION) ||
			(u32Height > REQ_TEXTURE2D_U_OR_V_DIMENSION))
		{
			return nullptr;
		}
		break;
	case VeRenderResource::DIMENSION_TEXTURE3D:
		if ((u32ArraySize > 1) ||
			(u32Width > REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
			(u32Height > REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
			(u32Depth > REQ_TEXTURE3D_U_V_OR_W_DIMENSION))
		{
			return nullptr;
		}
		break;
	default:
		return nullptr;
	}

	if (u32Depth != 1 && u32ArraySize != 1) return nullptr;

	kOut.m_eDimension = eResDim;
	kOut.m_eFormat = eFormat;
	kOut.m_u32Width = u32Width;
	kOut.m_u32Height = u32Height;
	kOut.m_u16Depth = (VeUInt16)(u32Depth * u32ArraySize);
	kOut.m_u16MipLevels = (VeUInt16)u32MipCount;
	kOut.m_bIsCube = bIsCubeMap;
	VeSizeT stDataSize = kIn.RemainingLength();
	void* pvData = kIn.Skip(stDataSize);
	return VE_NEW VeBlob(pvData, stDataSize);
}
//--------------------------------------------------------------------------
VeBlobPtr ParseTexture(VeTexture::FileInfo& kOut,
	const VeBlobPtr& spData) noexcept
{
	VeMemoryIStream kIn(spData);
	switch (kIn.Get<VeUInt32>())
	{
	case DDS_MAGIC:
		return ParseDDS(kOut, kIn);
	default:
		return nullptr;
	}
}
//--------------------------------------------------------------------------
