////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      MeshConvertor
//  File name:   FBXConvertor.cpp
//  Created:     2016/08/06 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#include "FBXConvertor.h"
#include "Mesh.h"
#include <fbxsdk.h>

//--------------------------------------------------------------------------
using namespace DirectX;
using namespace vtd;
//--------------------------------------------------------------------------
void SaveMesh(FbxNode* pNode, const VeDirectoryPtr& spDest) noexcept
{
	Mesh kMesh;
	FbxMesh* pMesh = (FbxMesh*)pNode->GetNodeAttribute();
	
	kMesh.m_kName = pNode->GetName();
	kMesh.m_stFaces = pMesh->GetPolygonCount();
	kMesh.m_stVerts = kMesh.m_stFaces * 3;

	kMesh.m_kIndices.resize(kMesh.m_stVerts);
	kMesh.m_kPosition.resize(kMesh.m_stVerts);

	kMesh.m_kNormals.resize(pMesh->GetElementNormalCount());
	for (auto& v : kMesh.m_kNormals)
	{
		v.resize(kMesh.m_stVerts);
	}
	kMesh.m_kTexcoords.resize(pMesh->GetElementUVCount());
	for (auto& v : kMesh.m_kTexcoords)
	{
		v.resize(kMesh.m_stVerts);
	}
	kMesh.m_kColors.resize(pMesh->GetElementVertexColorCount());
	for (auto& v : kMesh.m_kColors)
	{
		v.resize(kMesh.m_stVerts);
	}	

	int element_mat = -1;
	for (int i(0); i < pMesh->GetElementMaterialCount(); ++i)
	{
		FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(i);
		if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			element_mat = i;
			break;
		}
	}
	if (element_mat >= 0)
	{
		kMesh.m_kAttributes.resize(kMesh.m_stFaces);
	}

	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	for (int i(0); i < (int)(kMesh.m_stFaces); ++i)
	{
		int lPolygonSize = pMesh->GetPolygonSize(i);
		VE_ASSERT_ALWAYS(lPolygonSize == 3);
		for (int j(0); j < lPolygonSize; ++j)
		{
			uint32_t u32Index = i * 3 + j;
			kMesh.m_kIndices[u32Index] = u32Index;
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
			auto& pos = kMesh.m_kPosition[u32Index];
			pos.x = (float)lControlPoints[lControlPointIndex][0];
			pos.y = (float)lControlPoints[lControlPointIndex][1];
			pos.z = (float)lControlPoints[lControlPointIndex][2];
			
			for (int k(0); k < (int)(kMesh.m_kColors.size()); ++k)
			{
				FbxColor c;
				FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(k);
				switch (leVtxc->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						c = leVtxc->GetDirectArray().GetAt(lControlPointIndex);
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
						c = leVtxc->GetDirectArray().GetAt(id);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						c = leVtxc->GetDirectArray().GetAt(u32Index);
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(u32Index);
						c = leVtxc->GetDirectArray().GetAt(id);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;
				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
				auto& color = kMesh.m_kColors[k][u32Index];
				color.x = (float)c[0];
				color.y = (float)c[1];
				color.z = (float)c[2];
				color.w = (float)c[3];
			}

			for (int k(0); k < (int)(kMesh.m_kTexcoords.size()); ++k)
			{
				FbxVector2 uv;
				FbxGeometryElementUV* leUV = pMesh->GetElementUV(k);
				switch (leUV->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						uv = leUV->GetDirectArray().GetAt(lControlPointIndex);
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
						uv = leUV->GetDirectArray().GetAt(id);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						uv = leUV->GetDirectArray().GetAt(lTextureUVIndex);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}

				auto& texcoord = kMesh.m_kTexcoords[k][u32Index];
				texcoord.x = (float)uv[0];
				texcoord.y = (float)uv[1];
			}
			
			for (int k(0); k < (int)(kMesh.m_kNormals.size()); ++k)
			{
				FbxVector4 n;
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(k);
				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						n = leNormal->GetDirectArray().GetAt(u32Index);
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(u32Index);
						n = leNormal->GetDirectArray().GetAt(id);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

				auto& normal = kMesh.m_kNormals[k][u32Index];
				normal.x = (float)n[0];
				normal.y = (float)n[1];
				normal.z = (float)n[2];
			}

			if (element_mat >= 0)
			{
				FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(element_mat);
				FbxSurfaceMaterial* lMaterial = NULL;
				int lMatId = -1;
				lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
				lMatId = lMaterialElement->GetIndexArray().GetAt(i);
				kMesh.m_kAttributes[i] = lMatId;
			}
		}
	}
	kMesh.Process();
	kMesh.Save(spDest);
}
//--------------------------------------------------------------------------
void SaveContent(FbxNode* pNode, const VeDirectoryPtr& spDest) noexcept
{
	if (!pNode->GetNodeAttribute())
	{
		VeUserLogI("NULL Node Attribute");
	}
	else
	{
		FbxNodeAttribute::EType lAttributeType = pNode->GetNodeAttribute()->GetAttributeType();
		switch (lAttributeType)
		{
		case FbxNodeAttribute::eMarker:
			VeUserLogI("eMarker");
			break;

		case FbxNodeAttribute::eSkeleton:
			VeUserLogI("eSkeleton");
			break;

		case FbxNodeAttribute::eMesh:
			SaveMesh(pNode, spDest);
			break;

		case FbxNodeAttribute::eNurbs:
			VeUserLogI("eNurbs");
			break;

		case FbxNodeAttribute::ePatch:
			VeUserLogI("ePatch");
			break;

		case FbxNodeAttribute::eCamera:
			VeUserLogI("eCamera");
			break;

		case FbxNodeAttribute::eLight:
			VeUserLogI("eLight");
			break;

		case FbxNodeAttribute::eLODGroup:
			VeUserLogI("eLODGroup");
			break;
		default:
			break;
		}
	}
}
//--------------------------------------------------------------------------
void SaveContent(FbxScene* pScene, const VeDirectoryPtr& spDest) noexcept
{
	FbxNode* lNode = pScene->GetRootNode();
	if (lNode)
	{
		for (int i = 0; i < lNode->GetChildCount(); i++)
		{
			SaveContent(lNode->GetChild(i), spDest);
		}
	}
}
//--------------------------------------------------------------------------
void ConvertorFBX(const char* lFilename,
	const VeDirectoryPtr& spDest) noexcept
{
	FbxManager* lSdkManager = FbxManager::Create();
	VE_ASSERT_ALWAYS(lSdkManager);
	VeUserLogI("Autodesk FBX SDK version ", lSdkManager->GetVersion());
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	VE_ASSERT_ALWAYS(ios);
	lSdkManager->SetIOSettings(ios);
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "Import for Venus3D");
	VE_ASSERT_ALWAYS(lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings()));
	FbxScene* lScene = FbxScene::Create(lSdkManager, "ImportedScnee");
	VE_ASSERT_ALWAYS(lImporter->Import(lScene));
	SaveContent(lScene, spDest);
	lSdkManager->Destroy();
}
//--------------------------------------------------------------------------
