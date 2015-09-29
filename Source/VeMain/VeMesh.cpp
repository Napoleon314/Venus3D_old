////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMesh.cpp
//  Created:     2015/09/29 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeMesh, VeResource);
//--------------------------------------------------------------------------
VeMesh::VeMesh(const VeChar8* pcName) noexcept
	: VeResource(pcName)
{

}
//--------------------------------------------------------------------------
VeMesh::~VeMesh() noexcept
{

}
//--------------------------------------------------------------------------
const VeChar8* VeMesh::GetTypeName() const noexcept
{
	return TypeName();
}
//--------------------------------------------------------------------------
void VeMesh::LoadImpl(VeResourceManager::FileCachePtr spCache) noexcept
{
	VE_ASSERT(ve_renderer_ptr);
	VeMemoryIStream kIn(spCache->GetData());
	if (kIn.Get<VeUInt32>() == VE_FOURCC('M', 'E', 'S', 'H'))
	{
		VeUInt32 u32Num = kIn.Get<VeUInt32>();
		m_kVertexDataList.resize(u32Num);
		for (VeUInt32 i(0); i < u32Num; ++i)
		{
			VertexData& kData = m_kVertexDataList[i];
			kIn >> kData.m_u16Flags;
			kIn >> kData.m_u16Stride;
			kIn >> kData.m_u32VertexNum;
			VeUInt32 u32Size = kData.m_u16Stride * kData.m_u32VertexNum;
			kData.m_spVBuffer = ve_renderer_ptr->CreateBuffer(
				VeRenderBuffer::TYPE_STATIC, VeRenderBuffer::USEAGE_VB, u32Size);
			kData.m_spVBuffer->UpdateSync(kIn.Skip(u32Size), u32Size);
		}
		u32Num = kIn.Get<VeUInt32>();
		m_kSubMeshList.resize(u32Num);
		for (VeUInt32 i(0); i < u32Num; ++i)
		{
			SubMesh& kData = m_kSubMeshList[i];
			kIn.GetStringAligned4(kData.m_kName);
			kIn >> kData.m_u16GeoIndex;
			kIn >> kData.m_u16UnitSize;
			kIn >> kData.m_u32IndexSize;
			VeUInt32 u32Size = kData.m_u16UnitSize * kData.m_u32IndexSize;
			kData.m_spIBuffer = ve_renderer_ptr->CreateBuffer(
				VeRenderBuffer::TYPE_STATIC, VeRenderBuffer::USEAGE_IB, u32Size);
			kData.m_spIBuffer->UpdateSync(kIn.Skip(u32Size), u32Size);
		}
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
void VeMesh::UnloadImpl() noexcept
{
	m_kVertexDataList.clear();
	m_kSubMeshList.clear();
	OnResUnloaded();
}
//--------------------------------------------------------------------------
void VeMesh::Regist() noexcept
{
	VE_ENUM(FileType,
	{
		{ FILE_VMESH, "vmesh" }
	});

	ve_res_mgr.RegistResCreator(TypeName(),
		[](const VeChar8* pcName) noexcept
	{
		return VE_NEW VeMesh(pcName);
	});
	for (VeUInt32 i(0); i < FILE_MAX; ++i)
	{
		const VeChar8* pcExt = ve_parser.EnumToStr((FileType)i);
		ve_res_mgr.RegistExt(pcExt, TypeName());
	}
}
//--------------------------------------------------------------------------
void VeMesh::Unregist() noexcept
{
	ve_res_mgr.UnregistResCreator(TypeName());
	for (VeUInt32 i(0); i < FILE_MAX; ++i)
	{
		const VeChar8* pcExt = ve_parser.EnumToStr((FileType)i);
		ve_res_mgr.UnregistExt(pcExt);
	}
}
//--------------------------------------------------------------------------
