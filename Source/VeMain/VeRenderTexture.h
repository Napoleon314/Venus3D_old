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
		USEAGE_CUBE = 0x100
	};

	struct SubResDesc
	{
		VeUInt32 m_u32RowPitch;
		VeUInt32 m_u32RowNum;
		VeUInt32 m_u32SlicePitch;
	};

	inline Useage GetUseage() noexcept;

	inline Format GetFormat() noexcept;

	inline VeUInt32 GetWidth() noexcept;

	inline VeUInt32 GetHeight() noexcept;

	inline VeUInt16 GetDepth() noexcept;

	inline VeUInt16 GetMipLevels() noexcept;

	inline std::pair<VeUInt16,VeUInt16> GetSampleDesc() noexcept;

	inline bool CanUse(Useage eUse) noexcept;

	VeSizeT GetSubResDesc(SubResDesc* pkDesc) noexcept;
	
protected:
	VeRenderTexture(Dimension eDim, Useage eUse, Format eFormat,
		VeUInt32 u32Width, VeUInt32 u32Height, VeUInt16 u16Depth,
		VeUInt16 u16MipLevels, VeUInt16 u16Count, VeUInt16 u16Quality) noexcept
		: VeRenderResource(eDim), m_eUseage(eUse), m_eFormat(eFormat)
		, m_u32Width(u32Width), m_u32Height(u32Height), m_u16Depth(u16Depth)
		, m_u16MipLevels(u16MipLevels), m_u16Count(u16Count), m_u16Quality(u16Quality) {}

	Useage m_eUseage;
	Format m_eFormat;
	VeUInt32 m_u32Width;
	VeUInt32 m_u32Height;
	VeUInt16 m_u16Depth;
	VeUInt16 m_u16MipLevels;
	VeUInt16 m_u16Count;
	VeUInt16 m_u16Quality;

};

VeSmartPointer(VeRenderTexture);

#include "VeRenderTexture.inl"
