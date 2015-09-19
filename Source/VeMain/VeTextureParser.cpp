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
	VeUInt32 m_u32caps;
	VeUInt32 m_u32caps2;
	VeUInt32 m_u32caps3;
	VeUInt32 m_u32caps4;
	VeUInt32 m_u32reserved2;
};
//--------------------------------------------------------------------------
struct DDS_HEADER_DXT10
{
	VeRenderResource::Format m_eFormat;
	VeUInt32 m_u32ResourceDimension;
	VeUInt32 m_u32MiscFlag; // see D3D11_RESOURCE_MISC_FLAG
	VeUInt32 m_u32ArraySize;
	VeUInt32 m_u32MiscFlags2;
};
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
	


	return nullptr;
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
