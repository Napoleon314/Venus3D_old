////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderResource.h
//  Created:     2015/09/14 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeRenderTexture : public VeRenderResource
{
	VeNoCopy(VeRenderTexture);
	VeRTTIDecl(VeRenderTexture, VeRenderResource);
public:
	enum Useage
	{
		USEAGE_SRV = 0x1,
		USEAGE_RTV = 0x2,
		USEAGE_DSV = 0x4,
		USEAGE_UAV = 0x8,
		USEAGE_MASK = 0xff
	};

	struct SampleDesc
	{
		VeUInt16 m_u16Count = 0;
		VeUInt16 m_u16Qulity = 0;
	};

	inline Format GetFormat() noexcept;

	inline VeUInt32 GetWidth() noexcept;

	inline VeUInt32 GetHeight() noexcept;

	inline VeUInt16 GetDepth() noexcept;

	inline VeUInt16 GetMipLevels() noexcept;

	inline SampleDesc GetSampleDesc() noexcept;
	
protected:
	VeRenderTexture(Dimension eDim, Format eFormat,
		VeUInt32 u32Width, VeUInt32 u32Height, VeUInt16 u16Depth,
		VeUInt16 u16MipLevels, VeUInt16 u16Count, VeUInt16 u16Qulity) noexcept;

	Format m_eFormat;
	VeUInt32 m_u32Width;
	VeUInt32 m_u32Height;
	VeUInt16 m_u16Depth;
	VeUInt16 m_u16MipLevels;
	SampleDesc m_kSampleDesc;

};

#include "VeRenderTexture.inl"
