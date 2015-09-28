////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   VeMaxOptimizer.cpp
//  Created:     2015/09/27 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <DirectXMesh/DirectXMesh.h>

//--------------------------------------------------------------------------
using namespace DirectX;
//--------------------------------------------------------------------------
bool IsSame(VeMaxVertexDeclaration eFlag,
	VeMaxVertex& vert0, VeMaxVertex& vert1, VeFloat32 f32Epsilon) noexcept
{
	VE_ASSERT(VE_MASK_HAS_ALL(eFlag, Ves_Position));
	if (!VeAlmostEqual(vert0.m_v3Position, vert1.m_v3Position, f32Epsilon))
		return false;

	if (VE_MASK_HAS_ALL(eFlag, Ves_Normal) && (!VeAlmostEqual(vert0.m_v3Normal, vert1.m_v3Normal, f32Epsilon)))
		return false;

	if (VE_MASK_HAS_ALL(eFlag, Ves_Diffuse) && vert0.m_kDiffuse != vert1.m_kDiffuse)
		return false;

	for (VeUInt32 i(0); i < vert0.m_kTexcoords.size(); ++i)
	{
		if (!VeAlmostEqual(vert0.m_kTexcoords[i], vert1.m_kTexcoords[i], f32Epsilon))
		{
			return false;
		}
	}
	return true;
}
//--------------------------------------------------------------------------
void VeMaxWeldVertices(VeMaxMesh& mesh, VeFloat32 f32Epsilon) noexcept
{
	std::set<VeMaxGeometry> geometrySet;
	if (mesh.m_spSharedGeometry)
		geometrySet.insert(mesh.m_spSharedGeometry);

	for (auto& subMesh : mesh.m_kSubMeshes)
		geometrySet.insert(subMesh->m_spGeometry);

	VeVector<VeUInt32> kRemap;
	VeVector<VeMaxVertex> kNewBuffer;
	VeVector<VeUInt32> kIndexs32;
	VeVector<VeUInt16> kIndexs16;

	for (auto& geometry : geometrySet)
	{
		kRemap.clear();
		kNewBuffer.clear();
		kIndexs32.clear();
		kIndexs16.clear();
		for (VeUInt32 i(0); i < geometry->m_kVertexBuffer.size(); ++i)
		{
			VeUInt32 u32Remap = (VeUInt32)kNewBuffer.size();
			for (VeUInt32 j(0); j < kNewBuffer.size(); ++j)
			{
				if (IsSame(geometry->m_flagVertexDeclaration,
					geometry->m_kVertexBuffer[i], kNewBuffer[j], f32Epsilon))
				{
					u32Remap = j;
					break;
				}
			}
			kRemap.push_back(u32Remap);
			if (u32Remap == kNewBuffer.size())
			{				
				kNewBuffer.push_back(geometry->m_kVertexBuffer[i]);
			}
		}		

		bool bUse32Bit = kNewBuffer.size() > 65535;

		for (auto& subMesh : mesh.m_kSubMeshes)
		{
			if (geometry == subMesh->m_spGeometry)
			{
				if (subMesh->m_kIndexBuffer.m_bUse32BitIndices)
				{
					for (auto idx : subMesh->m_kIndexBuffer.m_kIndex32Buffer)
					{
						if (bUse32Bit)
						{
							kIndexs32.push_back(kRemap[idx]);
						}
						else
						{
							kIndexs16.push_back((VeUInt16)kRemap[idx]);
						}
					}
				}
				else
				{
					VE_ASSERT(!bUse32Bit);
					for (auto idx : subMesh->m_kIndexBuffer.m_kIndex16Buffer)
					{
						kIndexs16.push_back((VeUInt16)kRemap[idx]);
					}
				}

				if (bUse32Bit)
				{
					std::swap(subMesh->m_kIndexBuffer.m_kIndex32Buffer, kIndexs32);
					subMesh->m_kIndexBuffer.m_kIndex16Buffer.clear();
				}
				else
				{
					std::swap(subMesh->m_kIndexBuffer.m_kIndex16Buffer, kIndexs16);
					subMesh->m_kIndexBuffer.m_kIndex32Buffer.clear();
				}

				subMesh->m_kIndexBuffer.m_bUse32BitIndices = bUse32Bit ? VE_TRUE : VE_FALSE;
			}
		}
		std::swap(geometry->m_kVertexBuffer, kNewBuffer);
	}
}
//--------------------------------------------------------------------------
void VeMaxOptimizeMesh(VeMaxMesh& mesh) noexcept
{
	std::set<VeMaxGeometry> geometrySet;
	if (mesh.m_spSharedGeometry)
		geometrySet.insert(mesh.m_spSharedGeometry);

	for (auto& subMesh : mesh.m_kSubMeshes)
		geometrySet.insert(subMesh->m_spGeometry);

	VeVector<VeUInt32> kIndices;
	VeVector<VeUInt32> kAttributes;
	VeVector<VeUInt32> kAdjacency;
	VeVector<VE_FLOAT3> kPositions;

	for (auto& geometry : geometrySet)
	{
		kIndices.clear();
		kAttributes.clear();
		kAdjacency.clear();
		kPositions.clear();

		for (auto& subMesh : mesh.m_kSubMeshes)
		{
			if (geometry == subMesh->m_spGeometry)
			{
				if (subMesh->m_kIndexBuffer.m_bUse32BitIndices)
				{
					for (auto idx : subMesh->m_kIndexBuffer.m_kIndex32Buffer)
					{
						kIndices.push_back(idx);
					}
				}
				else
				{
					for (auto idx : subMesh->m_kIndexBuffer.m_kIndex16Buffer)
					{
						kIndices.push_back(idx);
					}
				}
			}
		}
		for (auto& vert : geometry->m_kVertexBuffer)
		{
			kPositions.push_back(vert.m_v3Position.v);
		}

		

		if (geometry == mesh.m_spSharedGeometry)
		{
			for (auto& subMesh : mesh.m_kSubMeshes)
			{
				if (geometry == subMesh->m_spGeometry)
				{
					VeSizeT faceCount = subMesh->m_kIndexBuffer.m_bUse32BitIndices ?
						subMesh->m_kIndexBuffer.m_kIndex32Buffer.size() / 3 :
						subMesh->m_kIndexBuffer.m_kIndex16Buffer.size() / 3;
					VeUInt32 matID = subMesh->m_i32MatID;
					for (VeSizeT attr = 0; attr < faceCount; ++attr)
					{
						kAttributes.push_back(matID);
					}
				}
			}
		}
		else
		{
			VeSizeT faceCount = geometry->m_kVertexBuffer.size() / 3;
			VeUInt32 matID = 0;
			for (auto& subMesh : mesh.m_kSubMeshes)
			{
				if (geometry == subMesh->m_spGeometry)
				{
					matID = subMesh->m_i32MatID;
					break;
				}
			}
			for (VeSizeT attr = 0; attr < faceCount; ++attr)
			{
				kAttributes.push_back(matID);
			}
		}
		
		VeSizeT stFaces = kIndices.size() / 3;
		VeSizeT stVerts = kPositions.size();
		kAdjacency.resize(kIndices.size());
		VE_ASSERT_EQ(GenerateAdjacencyAndPointReps(&kIndices.front(), stFaces,
			(XMFLOAT3*)&kPositions.front(), stVerts, 0, nullptr, &kAdjacency.front()), S_OK);

		{
			VeVector<VeUInt32> remap;
			remap.resize(stFaces);

			VE_ASSERT_EQ(AttributeSort(stFaces, &kAttributes.front(), &remap.front()), S_OK);

			VE_ASSERT_EQ(ReorderIBAndAdjacency(&kIndices.front(),
				stFaces, &kAdjacency.front(), &remap.front()), S_OK);

			VE_ASSERT_EQ(OptimizeFacesEx(&kIndices.front(), stFaces,
				&kAdjacency.front(), &kAttributes.front(), &remap.front()), S_OK);

			VE_ASSERT_EQ(ReorderIBAndAdjacency(&kIndices.front(), stFaces,
				&kAdjacency.front(), &remap.front()), S_OK);			
		}

		VeVector<VeUInt32> remap;
		remap.resize(stVerts);

		VE_ASSERT_EQ(OptimizeVertices(&kIndices.front(), stFaces,
			stVerts, &remap.front()), S_OK);

		int a = 0;


	}

	std::sort(mesh.m_kSubMeshes.begin(), mesh.m_kSubMeshes.end(),
		[](const VeMaxSubMeshContainer::value_type& a,
			const VeMaxSubMeshContainer::value_type& b) noexcept
	{
		return VeStrcmp<VeChar8>(a->m_kMaterialName, b->m_kMaterialName) > 0;
	});
}
//--------------------------------------------------------------------------
