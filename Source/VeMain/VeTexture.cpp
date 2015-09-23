////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeTexture.cpp
//  Created:     2015/09/19 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeBlobPtr ParseTexture(VeTexture::FileInfo& kOut,
	const VeBlobPtr& spData) noexcept;
//--------------------------------------------------------------------------
VeRTTIImpl(VeTexture, VeResource);
//--------------------------------------------------------------------------
VeTexture::VeTexture(const VeChar8* pcName) noexcept
	: VeResource(pcName)
{

}
//--------------------------------------------------------------------------
VeTexture::~VeTexture() noexcept
{

}
//--------------------------------------------------------------------------
const VeChar8* VeTexture::GetTypeName() const noexcept
{
	return TypeName();
}
//--------------------------------------------------------------------------
void VeTexture::LoadImpl(VeResourceManager::FileCachePtr spCache) noexcept
{
	VE_ASSERT(ve_renderer_ptr);
	auto eType = ve_parser.Parse(GetExt(), FILE_MAX);
	FileInfo kInfo;
	VeBlobPtr spData = ParseTexture(kInfo, spCache->GetData());	
	if (spData)
	{
		VeUInt32 u32Use = VeRenderTexture::USEAGE_SRV;
		if (kInfo.m_bIsCube) u32Use |= VeRenderTexture::USEAGE_CUBE;
		m_spTexture = ve_renderer_ptr->CreateTexture(kInfo.m_eDimension,
			(VeRenderTexture::Useage)u32Use, kInfo.m_eFormat,
			kInfo.m_u32Width, kInfo.m_u32Height, kInfo.m_u16Depth,
			kInfo.m_u16MipLevels);
		m_spTexture->SetSRVNum(1);
		m_spTexture->SetSRV(0, VeRenderResource::SRV_DEFAULT);
		m_spTexture->UpdateSync(spData->GetBuffer(), spData->GetSize());
		ve_renderer_ptr->RegistResource(m_kName, m_spTexture);
		if (m_u32WaitNumber == 0)
		{
			OnResLoaded();
		}
	}
	else
	{
		OnResLoadFailed(ERR_PARSE_FAILED);
	}	
}
//--------------------------------------------------------------------------
void VeTexture::UnloadImpl() noexcept
{
	if(ve_renderer_ptr) ve_renderer_ptr->UnregistResource(m_kName);
	m_spTexture = nullptr;
	OnResUnloaded();
}
//--------------------------------------------------------------------------
void VeTexture::Regist() noexcept
{
	VE_ENUM(FileType,
	{
		{ FILE_DDS, "dds" }
	});

	ve_res_mgr.RegistResCreator(TypeName(),
		[](const VeChar8* pcName) noexcept
	{
		return VE_NEW VeTexture(pcName);
	});
	for (VeUInt32 i(0); i < FILE_MAX; ++i)
	{
		const VeChar8* pcExt = ve_parser.EnumToStr((FileType)i);
		ve_res_mgr.RegistExt(pcExt, TypeName());
	}
}
//--------------------------------------------------------------------------
void VeTexture::Unregist() noexcept
{
	ve_res_mgr.UnregistResCreator(TypeName());
	for (VeUInt32 i(0); i < FILE_MAX; ++i)
	{
		const VeChar8* pcExt = ve_parser.EnumToStr((FileType)i);
		ve_res_mgr.UnregistExt(pcExt);
	}
}
//--------------------------------------------------------------------------
