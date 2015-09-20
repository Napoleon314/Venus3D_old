////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeTexture.h
//  Created:     2015/09/19 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeTexture : public VeResource
{
	VeNoCopy(VeTexture);
	VeRTTIDecl(VeTexture, VeResource);
public:
	enum FileType
	{
		FILE_DDS,
		FILE_MAX
	};

	struct FileInfo
	{
		VeRenderTexture::Dimension m_eDimension;
		VeRenderTexture::Format m_eFormat;
		VeUInt32 m_u32Width;
		VeUInt32 m_u32Height;
		VeUInt16 m_u16Depth;
		VeUInt16 m_u16MipLevels;
		bool m_bIsCube;
	};

	VeTexture(const VeChar8* pcName) noexcept;

	virtual ~VeTexture() noexcept;

	virtual const VeChar8* GetTypeName() const noexcept override;

	virtual void LoadImpl(VeResourceManager::FileCachePtr spCache) noexcept;

	virtual void UnloadImpl() noexcept;

	inline static const VeChar8* TypeName() noexcept;

	static void Regist() noexcept;

	static void Unregist() noexcept;

protected:
	VeRenderTexturePtr m_spTexture;
	
};

#include "VeTexture.inl"
