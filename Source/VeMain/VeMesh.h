////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMesh.h
//  Created:     2015/09/29 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeMesh : public VeResource
{
	VeNoCopy(VeMesh);
	VeRTTIDecl(VeMesh, VeResource);
public:
	enum FileType
	{
		FILE_VMESH,
		FILE_MAX
	};

	enum VertexFlag
	{
		FLAG_POSITION = 0x1,
		FLAG_NORMAL = 0x2,
		FLAG_TANGENT = 0x4,
		FLAG_TEXTURE = 0x8
	};

	struct VertexData
	{
		VeUInt16 m_u16Flags = 0;
		VeUInt16 m_u16Stride = 0;
		VeUInt32 m_u32VertexNum = 0;
		VeRenderBufferPtr m_spVBuffer;
	};

	struct SubMesh
	{
		VeFixedString m_kName;
		VeUInt16 m_u16GeoIndex = 0;
		VeUInt16 m_u16UnitSize = 0;
		VeUInt32 m_u32IndexSize = 0;
		VeRenderBufferPtr m_spIBuffer;
	};

	VeMesh(const VeChar8* pcName) noexcept;

	virtual ~VeMesh() noexcept;

	virtual const VeChar8* GetTypeName() const noexcept override;

	virtual void LoadImpl(VeResourceManager::FileCachePtr spCache) noexcept override;

	virtual void UnloadImpl() noexcept override;

	inline static const VeChar8* TypeName() noexcept;

	static void Regist() noexcept;

	static void Unregist() noexcept;
protected:
	VeVector<VertexData> m_kVertexDataList;
	VeVector<SubMesh> m_kSubMeshList;

};

#include "VeMesh.inl"
