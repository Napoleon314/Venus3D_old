////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   VeMaxWriter.cpp
//  Created:     2015/09/28 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//--------------------------------------------------------------------------
enum VertexFlag
{
	FLAG_POSITION = 0x1,
	FLAG_NORMAL = 0x2,
	FLAG_TANGENT = 0x4,
	FLAG_TEXTURE = 0x8
};
//--------------------------------------------------------------------------
std::pair<VeUInt32, VeUInt32> GetVertexMisc(
	VeMaxVertexDeclaration flags) noexcept
{
	VeUInt32 u32Flags = FLAG_POSITION;
	VeUInt32 u32Stride = sizeof(VE_FLOAT3);
	VE_ASSERT(VE_MASK_HAS_ALL(flags, Ves_Position));
	if (VE_MASK_HAS_ALL(flags, Ves_Normal))
	{
		u32Flags |= FLAG_NORMAL;
		u32Stride += sizeof(VE_FLOAT3);
	}
	if (VE_MASK_HAS_ALL(flags, Ves_Tangent))
	{
		u32Flags |= FLAG_TANGENT;
		u32Stride += sizeof(VE_FLOAT4);
	}
	if (VE_MASK_HAS_ALL(flags, Ves_Texture_Coordinate0))
	{
		u32Flags |= FLAG_TEXTURE;
		u32Stride += sizeof(VE_FLOAT2);
	}
	return std::make_pair(u32Flags, u32Stride);
}
//--------------------------------------------------------------------------
void VeMaxWriteToStream(VeBinaryOStream& out, VeMaxMesh& mesh) noexcept
{
	VeSet<VeMaxGeometry> geometrySet;
	if (mesh.m_spSharedGeometry)
		geometrySet.insert(mesh.m_spSharedGeometry);

	for (auto& subMesh : mesh.m_kSubMeshes)
		geometrySet.insert(subMesh->m_spGeometry);

	out << VE_FOURCC('M', 'E', 'S', 'H');
	out << VeUInt32(geometrySet.size());
	for (auto& geometry : geometrySet)
	{
		auto type = GetVertexMisc(geometry->m_flagVertexDeclaration);
		out << VeUInt16(type.first);
		out << VeUInt16(type.second);
		out << VeUInt32(geometry->m_kVertexBuffer.size());
		for (auto& vertex : geometry->m_kVertexBuffer)
		{
			if (VE_MASK_HAS_ALL(type.first, FLAG_POSITION))
			{
				out << vertex.m_v3Position;
			}
			if (VE_MASK_HAS_ALL(type.first, FLAG_NORMAL))
			{
				out << vertex.m_v3Normal;
			}
			if (VE_MASK_HAS_ALL(type.first, FLAG_TANGENT))
			{
				out << vertex.m_v4Tangent;
			}
			if (VE_MASK_HAS_ALL(type.first, FLAG_TEXTURE))
			{
				out << vertex.m_kTexcoords[0];
			}
		}
	}
	out << VeUInt32(mesh.m_kSubMeshes.size());
	for (auto& subMesh : mesh.m_kSubMeshes)
	{
		out.AppendStringAligned4(subMesh->m_kMaterialName, subMesh->m_kMaterialName.GetLength());
		VeUInt16 iGeo(0);
		for (auto& geometry : geometrySet)
		{
			if (geometry == subMesh->m_spGeometry)
				break;
			++iGeo;
		}
		out << iGeo;
		if (subMesh->m_kIndexBuffer.m_bUse32BitIndices)
		{
			VeUInt16 u16UnitSize = sizeof(VeUInt32);
			out << u16UnitSize;
			out << VeUInt32(subMesh->m_kIndexBuffer.m_kIndex32Buffer.size());
			out.Write(&subMesh->m_kIndexBuffer.m_kIndex32Buffer.front(),
				u16UnitSize * subMesh->m_kIndexBuffer.m_kIndex32Buffer.size());
		}
		else
		{
			VeUInt16 u16UnitSize = sizeof(VeUInt16);
			out << u16UnitSize;
			out << VeUInt32(subMesh->m_kIndexBuffer.m_kIndex16Buffer.size());
			out.Write(&subMesh->m_kIndexBuffer.m_kIndex16Buffer.front(),
				u16UnitSize * subMesh->m_kIndexBuffer.m_kIndex16Buffer.size());
		}		
	}
}
//--------------------------------------------------------------------------
