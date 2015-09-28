////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   VeMaxSceneLoader.cpp
//  Created:     2015/09/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//--------------------------------------------------------------------------
VeMaxSceneLoader::VeMaxSceneLoader() noexcept
{

}
//--------------------------------------------------------------------------
VeMaxSceneLoader::~VeMaxSceneLoader() noexcept
{

}
//--------------------------------------------------------------------------
bool VeMaxSceneLoader::Load(const VeDirectoryPtr& spDir,
	bool bSelected) noexcept
{
	bool bRes = false;
	Clear();
	IGameScene* gameScene = GetIGameInterface();
	GetConversionManager()->SetCoordSystem(IGameConversionManager::IGAME_USER);
	GetConversionManager()->SetUserCoordSystem({
		0,	//Left Handed
		0,	//X axis goes left
		4,	//Y Axis goes in
		2,	//Z Axis goes up.
		1,	//U Tex axis is right
		1,  //V Tex axis is Down
	});
	if (gameScene->InitialiseIGame(bSelected))
	{
		bRes = true;
		VeInt32 nodeCount = gameScene->GetTopLevelNodeCount();
		for (VeInt32 nodeIdx = 0; nodeIdx < nodeCount; ++nodeIdx)
		{
			IGameNode* node = gameScene->GetTopLevelNode(nodeIdx);
			DumpNodes(node);
		}		
		VeMaxNodeContainer meshNodes, physxNodes, tagNodes;
		for (auto& sceneNodeItor : m_kSceneNodeMap)
		{
			IGameNode* gameNode = sceneNodeItor.first;
			VeMaxSceneNode& sceneNode = sceneNodeItor.second;
			ProcessNodeUserDefinedProperties(gameNode, sceneNode);
			VeFixedString gameNodeName = ToStr(gameNode->GetName());
			if (m_kSceneNodeNameSet.count(gameNodeName))
			{
				gameNodeName = "";
			}
			else
			{
				m_kSceneNodeNameSet.insert(gameNodeName);
			}
			INode* maxNode = gameNode->GetMaxNode();
			sceneNode.m_kName = sceneNode.m_kMeshName = gameNodeName;
			sceneNode.m_pkNodeReference = &sceneNode;
			sceneNode.m_bHasUV = VE_FALSE;
			sceneNode.m_bHasSkeleton = IsNodeSkinned(maxNode);
			sceneNode.m_bHasMorph = IsNodeMorphed(maxNode);
			Object* objectRef = maxNode->GetObjOrWSMRef();
			auto result = m_kSceneNodeInstanceMap.insert(std::make_pair(objectRef, &sceneNode));
			if (!result.second)
			{
				sceneNode.m_pkNodeReference = result.first->second;
				sceneNode.m_kMeshName = sceneNode.m_pkNodeReference->m_kMeshName;
				if (sceneNode.m_pkNodeReference->m_kMaterialFileName == sceneNode.m_kMaterialFileName)
					sceneNode.m_kMaterialFileName = "";
				else
				{
					IGameMaterial* gameMaterial = gameNode->GetNodeMaterial();
					if (gameMaterial)
					{
						sceneNode.m_kMaterialFileName = ToStrReplaceIegality(gameMaterial->GetMaterialName());
					}
				}
			}
			ProcessAnimation(sceneNode);

			if (sceneNode.m_eType == ST_Geometry)
				meshNodes.push_back(gameNode);
			if (sceneNode.m_eType == ST_PhysX)
				physxNodes.push_back(gameNode);

			if (sceneNode.m_eType == ST_Helper)
			{
				IGameNode* rootNode = GetRootNode(gameNode);
				if (rootNode)
				{
					VE_ASSERT(m_kSceneNodeMap.count(gameNode));
					const VeMaxSceneNode& sceneNode = m_kSceneNodeMap[gameNode];
					if (sceneNode.m_eType == ST_Bone)
					{
						auto tagItor = sceneNode.m_kUserDefinedPropertyMap.find("tag");
						if (tagItor != sceneNode.m_kUserDefinedPropertyMap.end() &&
							tagItor->second[0] == "true")
						{
							tagNodes.push_back(gameNode);
						}
					}
				}
			}
		}
		
		for (auto& gameNode : meshNodes)
		{
			IGameMaterial* gameRootMaterial = gameNode->GetNodeMaterial();
			if (gameRootMaterial)
			{
				auto rootMaterialName = ToStr(gameRootMaterial->GetMaterialName());
				ProcessMaterial(gameRootMaterial, rootMaterialName);
			}
			else
			{
				ve_sys.USER.E.LogFormat("Node material is null: \"%s\" .", gameNode->GetName());
			}
		}

		for (auto& gameNode : tagNodes)
			ProcessTag(gameNode);

		for (auto& gameNode : meshNodes)
		{
			VE_ASSERT(m_kSceneNodeMap.count(gameNode));
			VeMaxSceneNode* sceneNode = &m_kSceneNodeMap[gameNode];
			if (sceneNode->m_pkNodeReference == sceneNode)
			{
				ProcessGeometry(gameNode, m_kSceneNodeMap);
				VeMaxWeldVertices(m_kMesh);				
				//VeMaxOptimizeMesh(m_kMesh);
				//VeMaxWriteToDir(spDir, m_kMesh);
				break;
			}
			else
			{
				sceneNode->m_f32PositionCenterX = sceneNode->m_pkNodeReference->m_f32PositionCenterX;
				sceneNode->m_f32PositionCenterY = sceneNode->m_pkNodeReference->m_f32PositionCenterY;
				sceneNode->m_f32PositionCenterZ = sceneNode->m_pkNodeReference->m_f32PositionCenterZ;
				sceneNode->m_f32PositionExtentX = sceneNode->m_pkNodeReference->m_f32PositionExtentX;
				sceneNode->m_f32PositionExtentY = sceneNode->m_pkNodeReference->m_f32PositionExtentY;
				sceneNode->m_f32PositionExtentZ = sceneNode->m_pkNodeReference->m_f32PositionExtentZ;

				sceneNode->m_bHasUV = sceneNode->m_pkNodeReference->m_bHasUV;
				sceneNode->m_f32CenterU = sceneNode->m_pkNodeReference->m_f32CenterU;
				sceneNode->m_f32CenterV = sceneNode->m_pkNodeReference->m_f32CenterV;
				sceneNode->m_f32ExtentU = sceneNode->m_pkNodeReference->m_f32ExtentU;
				sceneNode->m_f32ExtentV = sceneNode->m_pkNodeReference->m_f32ExtentV;
			}
		}


	}
	gameScene->ReleaseIGame();
	return bRes;
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::Clear() noexcept
{
	m_kSceneNodeMap.clear();
	m_kSceneNodeNameSet.clear();
	m_kSceneNodeInstanceMap.clear();
	m_kMaterialMap.clear();
	m_kMaterials.clear();
	m_kMesh.clear();
	m_kSkeletonMap.clear();
}
//--------------------------------------------------------------------------
VeMaxSceneNodeType VeMaxSceneLoader::GetSceneNodeType(INode* node) noexcept
{
	auto name = node->GetName();
	ObjectState os = node->EvalWorldState(0);
	if (os.obj)
	{
		SClass_ID scid = os.obj->SuperClassID();
		switch (scid)
		{
		case CAMERA_CLASS_ID:
			return ST_Camera;

		case LIGHT_CLASS_ID:
			return ST_Light;

		case HELPER_CLASS_ID:
			return ST_Helper;

		case SPLINESHAPE_CLASS_ID:
			return ST_Spline;

		case GEOMOBJECT_CLASS_ID:
			if (node->IsTarget())
			{
				return ST_Target;
			}				
			else
			{
				auto classid = os.obj->ClassID();
				if (classid == BONE_OBJ_CLASSID)
				{
					return ST_Bone;
				}					
				else
				{
					Control* ctrl = node->GetTMController();
					if (ctrl)
					{
						Class_ID cid = ctrl->ClassID();
						if (cid == BIPSLAVE_CONTROL_CLASS_ID ||
							cid == BIPBODY_CONTROL_CLASS_ID)
							return ST_Bone;
					}

					if (os.obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
					{
						auto type = ST_UnSupported;
						TriObject* tri = (TriObject *)os.obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
						VeInt32 numFaces = tri->mesh.numFaces;
						if (numFaces > 0)
						{
							MSTR mstr;
							node->GetUserPropBuffer(mstr);
							VeFixedString str = ToStr(mstr);
							if (VeStrstr<VeChar8>(str, "physx") != nullptr)
							{
								type = ST_PhysX;
							}
							else
							{
								Mtl* mtl = node->GetMtl();
								if (mtl)
								{
									Class_ID classID = mtl->ClassID();
									if (classID == Class_ID(DMTL_CLASS_ID, 0))
									{
										type = ST_Geometry;
									}
									else
									{
										if (classID == Class_ID(BAKE_SHELL_CLASS_ID, 0))
										{
											type = ST_Geometry;
										}
										else
										{
											if (classID == Class_ID(MULTI_CLASS_ID, 0) && mtl->IsMultiMtl())
											{
												VeSet<VeInt32> matIDs;
												for (VeInt32 i = 0; i < numFaces; ++i)
													matIDs.insert(tri->mesh.faces[i].getMatID());
												for (auto matID : matIDs)
												{
													Mtl* subMtl = mtl->GetSubMtl(matID);
													if (subMtl)
													{
														type = ST_Geometry;
														break;
													}
												}
											}
										}
									}																	
								}
							}
						}
						if (tri != os.obj)
							tri->DeleteMe();

						return type;
					}
				}
			}
			break;
		}
	}

	return ST_UnSupported;
}
//--------------------------------------------------------------------------
bool VeMaxSceneLoader::IsNodeSkinned(INode* node) noexcept
{
	Object *pObj = node->GetObjectRef();
	if (pObj)
	{
		while (pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
		{
			IDerivedObject *pDerivedObj = static_cast<IDerivedObject*>(pObj);
			VeInt32 ModStackIndex = 0;
			while (ModStackIndex < pDerivedObj->NumModifiers())
			{
				Modifier* pMod = pDerivedObj->GetModifier(ModStackIndex);
				Class_ID classID = pMod->ClassID();
				if (classID == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
				{
					bool isSkinned = false;
					IPhysiqueExport* pPhysique = (IPhysiqueExport *)pMod->GetInterface(I_PHYINTERFACE);
					if (pPhysique)
					{
						IPhyContextExport* pPhysiqueContext = (IPhyContextExport *)pPhysique->GetContextInterface(node);
						if (pPhysiqueContext)
						{
							VeInt32 numVertices = pPhysiqueContext->GetNumberVertices();
							isSkinned = numVertices > 0;

							pPhysique->ReleaseContextInterface(pPhysiqueContext);
						}
						pMod->ReleaseInterface(I_PHYINTERFACE, pPhysique);
					}

					return isSkinned;
				}
				else if (classID == SKIN_CLASSID)
				{
					bool isSkinned = false;
					ISkin* pSkin = (ISkin*)pMod->GetInterface(I_SKIN);
					if (pSkin)
					{
						ISkinContextData* pSkinContext = pSkin->GetContextInterface(node);
						if (pSkinContext)
						{
							int numAssignedBones = pSkinContext->GetNumAssignedBones(0);
							isSkinned = numAssignedBones > 0;
						}
						pMod->ReleaseInterface(I_SKIN, pSkin);
					}
					return isSkinned;
				}
				ModStackIndex++;
			}
			pObj = pDerivedObj->GetObjRef();
		}
	}
	return false;
}
//--------------------------------------------------------------------------
bool VeMaxSceneLoader::IsNodeMorphed(Animatable* node) noexcept
{
	SClass_ID sid = node->SuperClassID();
	if (node->IsAnimated() && (sid == CTRL_MORPH_CLASS_ID))
		return true;
	return false;
}
//--------------------------------------------------------------------------
bool VeMaxSceneLoader::IsNodeHidden(IGameNode* gameNode) noexcept
{
	bool isHidden = gameNode->IsNodeHidden();
	if (isHidden)
	{
		int count = gameNode->GetChildCount();
		for (int n = 0; n < count; ++n)
		{
			IGameNode* childNode = gameNode->GetNodeChild(n);
			if (!IsNodeHidden(childNode))
			{
				isHidden = false;
				break;
			}
		}
	}
	return isHidden;
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::DumpNodes(IGameNode* gameNode) noexcept
{
	bool isHidden = IsNodeHidden(gameNode);
	if (!isHidden)
	{
		auto type = GetSceneNodeType(gameNode->GetMaxNode());
		if (type < ST_UnSupported)
		{
			VeMaxSceneNode& sceneNode = m_kSceneNodeMap[gameNode];
			sceneNode.m_eType = type;
		}
		VeInt32 count = gameNode->GetChildCount();
		for (VeInt32 n = 0; n < count; ++n)
		{
			IGameNode* childNode = gameNode->GetNodeChild(n);
			DumpNodes(childNode);
		}
	}
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::ProcessNodeUserDefinedProperties(IGameNode* gameNode,
	VeMaxSceneNode& sceneNode) noexcept
{
	MSTR str;
	gameNode->GetMaxNode()->GetUserPropBuffer(str);
	VeChar8* pcBuffer = nullptr;
	VeInt32 i32Size;
#	ifdef _UNICODE
	i32Size = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, FALSE);
	if (i32Size > 0)
	{
		pcBuffer = VeStackAlloc(VeChar8, i32Size + 1);
		WideCharToMultiByte(CP_UTF8, 0, str,
			-1, pcBuffer, i32Size, 0, FALSE);
	}
#	else
	i32Size = (VeInt32)VeStrlen(str);
	if (i32Size > 0)
	{
		pcBuffer = VeStackAlloc(VeChar8, i32Size + 1);
		VeStrcpy(pcBuffer, i32Size + 1, str);
	}
#	endif	
	if (pcBuffer)
	{
		if (*pcBuffer)
		{
			VeChar8* pcLineContext;
			VeChar8* pcLine = VeStrline(pcBuffer, &pcLineContext);
			while (pcLine)
			{
				VeChar8* pcContext;
				VeChar8* pcTemp = VeStrtok(pcLine, "=", &pcContext);
				if (pcTemp && (*pcTemp) && pcContext && (*pcContext))
				{
					pcTemp = VeTrim(pcTemp);
					pcContext = VeTrim(pcContext);
					auto& list = sceneNode.m_kUserDefinedPropertyMap[pcTemp];
					list.push_back(pcContext);
				}
				pcLine = VeStrline<VeChar8>(nullptr, &pcLineContext);
			}

		}
		VeStackFree(pcBuffer);
		pcBuffer = nullptr;
	}
#	if defined(MAX_RELEASE_R13_ALPHA) && MAX_RELEASE >= MAX_RELEASE_R13_ALPHA
	VeStringMap<VeVector<VeFixedString>>::iterator LastPoseItor = sceneNode.m_kUserDefinedPropertyMap.find("LastPose");
	if (LastPoseItor != sceneNode.m_kUserDefinedPropertyMap.end())
	{
		for (auto it = LastPoseItor->second.begin(); it != LastPoseItor->second.end(); ++it)
		{
			if (*it == "undefined")
			{
				LastPoseItor->second.erase(it);
			}
		}
		if (LastPoseItor->second.empty())
		{
			sceneNode.m_kUserDefinedPropertyMap.erase(LastPoseItor);
		}		
	}
#	endif
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::ProcessAnimation(VeMaxSceneNode& sceneNode) noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	auto range = sceneNode.m_kUserDefinedPropertyMap.find("anim");
	if (range != sceneNode.m_kUserDefinedPropertyMap.end())
	{
		for (auto value : range->second)
		{
			sceneNode.m_kAnimationDescs.resize(sceneNode.m_kAnimationDescs.size() + 1);
			VeStrcpy(acBuffer, value);
			VeChar8* pcContext;
			VeChar8* pcTemp = VeStrtok(acBuffer, ",", &pcContext);
			sceneNode.m_kAnimationDescs.back().m_Name = pcTemp;
			pcTemp = VeStrtok<VeChar8>(nullptr, ",", &pcContext);
			sceneNode.m_kAnimationDescs.back().m_i32Start = VeAtoi(pcTemp);
			pcTemp = VeStrtok<VeChar8>(nullptr, ",", &pcContext);
			sceneNode.m_kAnimationDescs.back().m_i32End = VeAtoi(pcTemp);
		}
	}
	if (sceneNode.m_kAnimationDescs.empty() &&
		(sceneNode.m_bHasMorph || sceneNode.m_bHasSkeleton))
	{
		sceneNode.m_kAnimationDescs.resize(sceneNode.m_kAnimationDescs.size() + 1);
		sceneNode.m_kAnimationDescs.back().m_Name = "_auto_";
		sceneNode.m_kAnimationDescs.back().m_i32Start = GetIGameInterface()->GetSceneStartTime() / GetIGameInterface()->GetSceneTicks();
		sceneNode.m_kAnimationDescs.back().m_i32End = GetIGameInterface()->GetSceneEndTime() / GetIGameInterface()->GetSceneTicks();
	}
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::ProcessMaterial(IGameMaterial* gameMaterial,
	const VeFixedString& rootMaterialName) noexcept
{
	if (gameMaterial->IsMultiType())
	{
		const MCHAR* materialClass = gameMaterial->GetMaterialClass();
		if (_tcscmp(materialClass, _T("Shell Material")) == 0)
		{
			if (gameMaterial->GetSubMaterialCount() > 1)
			{
				IGameMaterial* gameOrigMaterial = gameMaterial->GetSubMaterial(0);
				IGameMaterial* gameBakedMaterial = gameMaterial->GetSubMaterial(1);

				if (gameOrigMaterial->IsMultiType() &&
					gameBakedMaterial->IsMultiType())
				{
					VeInt32 origSubCount = gameOrigMaterial->GetSubMaterialCount(),
						bakedSubCount = gameBakedMaterial->GetSubMaterialCount();
					VE_ASSERT(origSubCount >= bakedSubCount);
					for (VeInt32 n = 0; n < origSubCount; ++n)
					{
						IGameMaterial* gameOrigSubMaterial = gameOrigMaterial->GetSubMaterial(n);
						int bakedIndex = VE_MIN(n, bakedSubCount - 1);
						IGameMaterial* gameBakedSubMaterial = gameBakedMaterial->GetSubMaterial(bakedIndex);
						auto origSubMaterial = DumpMaterialProperty(gameOrigSubMaterial, rootMaterialName),
							bakedSubMaterial = DumpMaterialProperty(gameBakedSubMaterial, rootMaterialName);

						auto textureSlotItor = std::find_if(bakedSubMaterial->m_kTextureSlots.begin(), bakedSubMaterial->m_kTextureSlots.end(),
							[](const VeMaxMaterial::TextureSlotContainer::value_type& textureSlot) {
							return textureSlot->m_Name == "diffuseMap"; });
						if (textureSlotItor != bakedSubMaterial->m_kTextureSlots.end())
						{
							auto& textureSlot = *textureSlotItor;
							textureSlot->m_Name = "lightMap";
							textureSlot->m_eTexUnit = VeMaxMaterial::TU_LightMap;
							textureSlot->m_eAlphaOpEx.operation = VeMaxMaterial::Op_Source1;
							textureSlot->m_eAlphaOpEx.source1 = VeMaxMaterial::Src_Current;
							textureSlot->m_eAlphaOpEx.source2 = VeMaxMaterial::Src_Texture;

							if (origSubMaterial->m_kUVCoordinateMap.count(textureSlot->m_i32MapChannel))
							{
								VeDebugOutput("Diffuse map and lightmap, using the same channel : (%d).", textureSlot->m_i32MapChannel);
							}								

							int bakedMapChannel = textureSlot->m_i32MapChannel;
							origSubMaterial->m_kUVCoordinateMap.insert(
								std::make_pair(bakedMapChannel, std::move(bakedSubMaterial->m_kUVCoordinateMap[bakedMapChannel])));

							origSubMaterial->m_kTextureSlots.push_back(std::move(textureSlot));

							if (origSubMaterial->m_kUVCoordinateMap.size() > 1)
								origSubMaterial->m_bPackedTexcoords = VE_TRUE;
						}
						AddMaterial(gameOrigSubMaterial, origSubMaterial);
					}
				}
				else
				{
					auto	origMaterial = DumpMaterialProperty(gameOrigMaterial, rootMaterialName),
						bakedMaterial = DumpMaterialProperty(gameBakedMaterial, rootMaterialName);

					auto textureSlotItor = std::find_if(bakedMaterial->m_kTextureSlots.begin(), bakedMaterial->m_kTextureSlots.end(),
						[](const VeMaxMaterial::TextureSlotContainer::value_type& textureSlot) {
						return textureSlot->m_Name == "diffuseMap"; });
					if (textureSlotItor != bakedMaterial->m_kTextureSlots.end())
					{
						auto& textureSlot = *textureSlotItor;
						textureSlot->m_Name = "lightMap";
						textureSlot->m_eTexUnit = VeMaxMaterial::TU_LightMap;
						textureSlot->m_eAlphaOpEx.operation = VeMaxMaterial::Op_Source1;
						textureSlot->m_eAlphaOpEx.source1 = VeMaxMaterial::Src_Current;
						textureSlot->m_eAlphaOpEx.source2 = VeMaxMaterial::Src_Texture;

						int bakedMapChannel = textureSlot->m_i32MapChannel;
						origMaterial->m_kUVCoordinateMap.insert(
							std::make_pair(bakedMapChannel, std::move(bakedMaterial->m_kUVCoordinateMap[bakedMapChannel])));

						origMaterial->m_kTextureSlots.push_back(std::move(textureSlot));

						if (origMaterial->m_kUVCoordinateMap.size() > 1)
							origMaterial->m_bPackedTexcoords = VE_TRUE;
					}
					AddMaterial(gameOrigMaterial, origMaterial);
				}
			}
			else
			{
				auto origMaterial = DumpMaterialProperty(gameMaterial->GetSubMaterial(0), rootMaterialName);
				AddMaterial(gameMaterial, origMaterial);
			}
		}
		else
		{
			for (int n = 0; n < gameMaterial->GetSubMaterialCount(); ++n)
				ProcessMaterial(gameMaterial->GetSubMaterial(n), rootMaterialName);
		}
	}
	else
	{
		auto material = DumpMaterialProperty(gameMaterial, rootMaterialName);
		AddMaterial(gameMaterial, material);
	}
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::ProcessGeometry(IGameNode* gameNode,
	const VeMaxSceneNodeMap& sceneNodeMap) noexcept
{
	VE_ASSERT(sceneNodeMap.count(gameNode));
	const VeMaxSceneNode* sceneNode = &sceneNodeMap.find(gameNode)->second;
	GMatrix nodeInvWorldTM;
	//GMatrix localTM = gameNode->GetLocalTM();
	bool flipNormal = false; //localTM.Parity() == -1;

	IGameObject* gameObject = gameNode->GetIGameObject();
	IGameMesh* gameMesh = static_cast<IGameMesh*>(gameObject);

	m_kMesh.clear();
	m_kMesh.m_kName = sceneNode->m_kMeshName;

	m_kMesh.m_bHasSkeleton = sceneNode->m_bHasSkeleton;
	IGameSkin* skin = nullptr;
	if (m_kMesh.m_bHasSkeleton && gameObject->IsObjectSkinned())
	{
		skin = gameObject->GetIGameSkin();
		skin->GetInitSkinTM(m_kInitSkinTM);

		nodeInvWorldTM = m_kInitSkinTM.Inverse();

		int numSkinBones = skin->GetTotalBoneCount();
		VE_ASSERT(numSkinBones > 0);
		if (numSkinBones > 0)
		{
			IGameNode* rootNode = GetRootNode(skin->GetIGameBone(0, true));
			VE_ASSERT(rootNode);
			if (rootNode)
			{
				m_kMesh.m_kSkeletonName = ToStrReplaceIegality(rootNode->GetName());				
			}
		}
	}
	else
	{
		nodeInvWorldTM = gameNode->GetWorldTM().Inverse();
	}

	VeVector<TimeValue> morphKeyFrameTimes;
	if (sceneNode->m_bHasMorph)
		ProcessVertexAnimation(gameNode, sceneNode, morphKeyFrameTimes);

	bool ret = gameMesh->InitializeData();

	bool hasNormal = false;
	Mesh* mesh = gameMesh->GetMaxMesh();
	if (mesh && mesh->normalCount > 0)
		hasNormal = true;

	int tangentMapChannel = -1;
	for (int mapChannel = -2; mapChannel <= 99; ++mapChannel)
	{
		if (gameMesh->GetNumberOfTangents(mapChannel) > 0)
		{
			tangentMapChannel = mapChannel;
			break;
		}
	}

	struct SubMeshDesc
	{
		int matID;
		VeMaxVertexDeclaration vertexDeclaration;
		VeSet<int> mapChannelSet;
		VeVector<FaceEx*> faces;
		size_t numVertices;
		VeMaxGeometry geometry;
	};
	typedef std::unique_ptr<SubMeshDesc> SubMeshDescPtr;

	VeMap<VeMaxMaterialPtr, SubMeshDescPtr> subMeshDescMap;
	VeMultiSet<VeMaxVertexDeclaration> vertexDeclSet;
	int faceTotalCount = 0;
	Tab<int> matIDs = gameMesh->GetActiveMatIDs();
	int matIDCount = matIDs.Count();
	for (int matIdx = 0; matIdx < matIDCount; ++matIdx)
	{
		int matID = matIDs[matIdx];
		auto faces = gameMesh->GetFacesFromMatID(matID);
		int numFaces = faces.Count();
		faceTotalCount += numFaces;
		VE_ASSERT(numFaces > 0);

		FaceEx* face = faces[0];
		IGameMaterial* gameMaterial = gameMesh->GetMaterialFromFace(face);
		if (gameMaterial == nullptr)
		{
			ve_sys.USER.E.LogFormat("Ignore Submesh(matID: %d) with no material.", matID);
			continue;
		}
		if (gameMaterial->IsMultiType())
		{
			const MCHAR* materialClass = gameMaterial->GetMaterialClass();
			if (_tcscmp(_T("Shell Material"), materialClass) == 0)
			{
				if (gameMaterial->GetSubMaterialCount() > 1)
				{
					gameMaterial = gameMaterial->GetSubMaterial(0);
					if (gameMaterial->IsMultiType())
					{
						int origSubCount = gameMaterial->GetSubMaterialCount();
						int index = 0;
						for (; index < origSubCount; ++index)
						{
							int subMtlID = gameMaterial->GetMaterialID(index);
							if (subMtlID == matID)
								break;
						}
						if (index == origSubCount)
							index = 0;

						gameMaterial = gameMaterial->GetSubMaterial(index);
					}
				}
			}
		}

		VE_ASSERT(m_kMaterialMap.count(gameMaterial));
		auto& material = m_kMaterialMap[gameMaterial];

		auto subMeshDescItor = subMeshDescMap.find(material);
		if (subMeshDescItor == subMeshDescMap.end())
		{
			material->m_bIsUsed = VE_TRUE;
			SubMeshDescPtr subMeshDesc(new SubMeshDesc);
			subMeshDesc->matID = matID;
			subMeshDesc->faces.reserve(numFaces);
			std::copy(faces.Addr(0), faces.Addr(0) + numFaces, std::back_inserter(subMeshDesc->faces));
			auto& vertexDeclaration = subMeshDesc->vertexDeclaration;
			auto& mapChannelSet = subMeshDesc->mapChannelSet;
			vertexDeclaration = Ves_Position;
			if (hasNormal)
				vertexDeclaration |= Ves_Normal;
			{
				Point3 rgb(1.0f, 1.0f, 1.0f);
				if (gameMesh->GetColorVertex(face->color[0], rgb) && !rgb.Equals(Point3(-1.0f, -1.0f, -1.0f)))
					vertexDeclaration |= Ves_Diffuse;
			}
			{
				for (auto& textureSlot : material->m_kTextureSlots)
				{
					if (textureSlot->m_eTexUnit != VeMaxMaterial::TU_Operation)
					{
						int mapChannel = textureSlot->m_i32MapChannel;
						if (gameMesh->GetMaxMesh()->mapSupport(mapChannel))
							mapChannelSet.insert(mapChannel);
						else
							VeDebugOutput("Unsupported map channel : (%d).", mapChannel);
					}
				}
				const size_t MaxTexUnits = 8;
				VE_ASSERT(mapChannelSet.size() <= MaxTexUnits);
				size_t maxTexUnitNum = VE_MIN(MaxTexUnits, mapChannelSet.size());
				for (size_t n = 0; n < maxTexUnitNum; ++n)
					vertexDeclaration |= Ves_Texture_Coordinate0 << n;
			}

			{
				if (tangentMapChannel != -1 &&
					gameMesh->GetFaceVertexTangentBinormal(face->meshFaceIndex, 0, tangentMapChannel) != -1)
				{
					bool hasNormalMap = false;
					for (auto& textureSlot : material->m_kTextureSlots)
					{
						if (textureSlot->m_eTexUnit == VeMaxMaterial::TU_NormalMap)
						{
							hasNormalMap = true;
							break;
						}
					}

					if (hasNormalMap)
						vertexDeclaration |= Ves_Binormal | Ves_Tangent;
				}
			}

			{
				if (m_kMesh.m_bHasSkeleton)
					vertexDeclaration |= Ves_Blend_Weights | Ves_Blend_Indices;
			}

			{
				if (m_kMesh.m_bHasMorph)
					vertexDeclaration |= Ves_VertexAnimationIndex;
			}

			subMeshDescMap.insert(std::make_pair(material, std::move(subMeshDesc)));
			vertexDeclSet.insert(vertexDeclaration);

		}
		else
		{
			auto& subMeshDesc = subMeshDescItor->second;
			vertexDeclSet.insert(subMeshDesc->vertexDeclaration);
			std::copy(faces.Addr(0), faces.Addr(0) + numFaces, std::back_inserter(subMeshDesc->faces));
		}		
	}
	VE_ASSERT(faceTotalCount > 0 && !vertexDeclSet.empty());
	size_t sharedVertexDeclarationUsed = 0;
	VeMaxVertexDeclaration sharedVertexDeclaration = 0;
	for (auto& itor = vertexDeclSet.begin(); itor != vertexDeclSet.end(); itor = vertexDeclSet.upper_bound(*itor))
	{
		auto& vertexDeclaration = *itor;
		size_t vertexDeclUsed = vertexDeclSet.count(vertexDeclaration);
		if (vertexDeclUsed > sharedVertexDeclarationUsed)
		{
			sharedVertexDeclarationUsed = vertexDeclUsed;
			sharedVertexDeclaration = vertexDeclaration;
		}
	}

	VE_ASSERT(sharedVertexDeclaration != 0);
	m_kMesh.m_spSharedGeometry.reset(new VeMaxGeometryData);
	size_t sharedVertexCount = 0;
	for (auto& desc : subMeshDescMap)
	{
		auto& material = desc.first;
		auto& subMeshDesc = desc.second;
		size_t numVertices = subMeshDesc->faces.size() * 3;
		subMeshDesc->numVertices = numVertices;
		auto& geometry = subMeshDesc->geometry;
		if (sharedVertexDeclaration == subMeshDesc->vertexDeclaration)
		{
			geometry = m_kMesh.m_spSharedGeometry;
			sharedVertexCount += numVertices;
		}
		else
		{
			geometry.reset(new VeMaxGeometryData);
			geometry->m_kVertexBuffer.reserve(numVertices);
		}
		geometry->m_flagVertexDeclaration = subMeshDesc->vertexDeclaration;
		geometry->m_spMaterial = material;
	}
	if (m_kMesh.m_spSharedGeometry)
	{
		m_kMesh.m_spSharedGeometry->m_kVertexBuffer.reserve(sharedVertexCount);
	}		

	int faceProgress = 0;
	Interface* coreInterface = GetCOREInterface();
	TimeValue sceneStartTime = GetIGameInterface()->GetSceneStartTime();
	for (auto& desc : subMeshDescMap)
	{
		auto& material = desc.first;
		auto& subMeshDesc = desc.second;

		auto matID = subMeshDesc->matID;
		auto& mapChannelSet = subMeshDesc->mapChannelSet;
		auto& geometry = subMeshDesc->geometry;
		auto& vertexDeclaration = geometry->m_flagVertexDeclaration;
		auto& faces = subMeshDesc->faces;
		auto& numVertices = subMeshDesc->numVertices;

		std::string materialName = (const VeChar8*)material->m_kName;
		VE_ASSERT(!materialName.empty());		
		{
			std::string extended;
			if (m_kMesh.m_bHasSkeleton)
				extended = "_skeleton";
			else if (m_kMesh.m_bHasMorph)
				extended = "_morph";
			else
				extended = "_static";
			material->m_kExtended.insert(extended.c_str());
			if (!materialName.empty())
				materialName += extended;
		}

		std::unique_ptr<VeMaxSubMesh> subMesh(new VeMaxSubMesh);
		subMesh->m_kMaterialName = materialName.c_str();
		subMesh->m_i32MatID = matID;
		subMesh->m_spGeometry = geometry;

		size_t numVertex = 3;
		std::vector<VeMaxVertex> vertices(3);
		std::vector<std::vector<VeMaxMorphVertex>> morphTrackVertices;
		if (m_kMesh.m_bHasMorph)
		{
			morphTrackVertices.resize(numVertex);
			std::for_each(morphTrackVertices.begin(), morphTrackVertices.end(),
				std::bind2nd(std::mem_fun_ref(&std::vector<VeMaxMorphVertex>::resize), morphKeyFrameTimes.size()));
		}

		VeSet<VeUInt16> boneAssignments;

		size_t numFaces = faces.size();
		for (size_t f = 0; f < numFaces; ++f, ++faceProgress)
		{
			FaceEx* face = faces[f];
			for (int v = 0; v < 3; ++v)
			{
				auto& vertex = vertices[v];
				int vertexIndex = face->vert[v];				
				if (m_kMesh.m_bHasMorph)
					coreInterface->SetTime(sceneStartTime, FALSE);

				if (vertexDeclaration & Ves_Position)
				{
					Point3 position = gameMesh->GetVertex(vertexIndex, false);
					position = position * nodeInvWorldTM;
					vertex.m_v3Position.x = position.x;
					vertex.m_v3Position.y = position.y;
					vertex.m_v3Position.z = position.z;
					UpdateVertexBounds(vertex.m_v3Position);
				}

				if (vertexDeclaration & Ves_Normal)
				{
					Point3 normal;
					gameMesh->GetNormal(face->norm[v], normal, true);
					if (flipNormal)
						normal *= -1.0f;

					vertex.m_v3Normal.x = normal.x;
					vertex.m_v3Normal.y = normal.y;
					vertex.m_v3Normal.z = normal.z;
				}

				if (vertexDeclaration & Ves_Diffuse)
				{
					Point3 rgb(1, 1, 1);
					gameMesh->GetColorVertex(face->color[v], rgb);
					std::swap(rgb.y, rgb.z);
					float a = 1.0;
					gameMesh->GetAlphaVertex(face->alpha[v], a);
					unsigned char argb[4] = { static_cast<unsigned char>(fabs(a) * 255),
						static_cast<unsigned char>(fabs(rgb.x) * 255),
						static_cast<unsigned char>(fabs(rgb.y) * 255),
						static_cast<unsigned char>(fabs(rgb.z) * 255) };
					vertex.m_kDiffuse = argb[0] << 24 | argb[1] << 16 | argb[2] << 8 | argb[3];
				}

				if (vertexDeclaration & Ves_Texture_Coordinate0)
				{
					vertex.m_kTexcoords.clear();
					for (auto& mapChannel : mapChannelSet)
					{
						int mapVertexIndex = gameMesh->GetFaceTextureVertex(face->meshFaceIndex, v, mapChannel);
						Point3 mapVertex = gameMesh->GetMapVertex(mapChannel, mapVertexIndex);
						{
							const float TexcoordRange = 500.0f;
							mapVertex.x = VE_MIN(TexcoordRange, VE_MAX(mapVertex.x, -TexcoordRange));
							mapVertex.y = VE_MIN(TexcoordRange, VE_MAX(mapVertex.y, -TexcoordRange));
							if (fzero(mapVertex.x))
								mapVertex.x = 0;
							if (fzero(mapVertex.y))
								mapVertex.y = 0;
						}

						VE_ASSERT(material->m_kUVCoordinateMap.count(mapChannel));
						auto& uvCoordinate = material->m_kUVCoordinateMap[mapChannel];
						GMatrix uvTransform = ToGMatrix(uvCoordinate->m_m4Transform);
						mapVertex = mapVertex * uvTransform;
						if (uvCoordinate->m_eUMode == VeMaxMaterial::TM_mirror)
							mapVertex.x *= 2;
						if (uvCoordinate->m_eVMode == VeMaxMaterial::TM_mirror)
							mapVertex.y *= 2;

						VeVector2 uv(mapVertex.x, 1.0f - mapVertex.y);
						vertex.m_kTexcoords.push_back(uv);
						UpdateUVBounds(uv);
					}
				}

				if (vertexDeclaration & Ves_Tangent)
				{
					int tangentIndex = gameMesh->GetFaceVertexTangentBinormal(face->meshFaceIndex, v, tangentMapChannel);
					Point3	tangent = gameMesh->GetTangent(tangentIndex, tangentMapChannel),
						binormal = gameMesh->GetBinormal(tangentIndex, tangentMapChannel),
						normal(vertex.m_v3Normal.x, vertex.m_v3Normal.y, vertex.m_v3Normal.z);

					tangent = (tangent - normal * DotProd(normal, tangent)).Normalize();
					float handedness = (DotProd(CrossProd(normal, tangent), binormal) < 0.0f) ? -1.0f : 1.0f;

					vertex.m_v4Tangent.Set(tangent.x, tangent.y, tangent.z, handedness);
					vertex.m_v3Binormal.Set(binormal.x, binormal.y, binormal.z);
				}

				if (vertexDeclaration & Ves_Blend_Weights)
				{
					float weight = 0.0f;
					int numBones = skin->GetNumberOfBones(vertexIndex);
					if (numBones > 0)
					{
						struct SkinBone {
							IGameNode*	boneNode;
							float		weight;
							bool operator < (const SkinBone& rhs)const { return weight > rhs.weight; }
						};

						const float weightEpsilon = 0.002f;
						std::vector<SkinBone> skinBones;
						skinBones.reserve(numBones);
						for (int idx = 0; idx < numBones; ++idx)
						{
							IGameNode* boneNode = skin->GetIGameBone(vertexIndex, idx);
							if (boneNode)
							{
								float weight = skin->GetWeight(vertexIndex, idx);
								if (weight > weightEpsilon)
									skinBones.push_back({ boneNode, weight });
							}
						}
						numBones = static_cast<int>(skinBones.size());
						if (numBones > 4)
						{
							std::sort(skinBones.begin(), skinBones.end());
							skinBones.resize(4);
							numBones = 4;
						}

						if (numBones > 0)
						{
							float weightTotal = 0.0f;
							for (auto& skinBone : skinBones)
								weightTotal += skinBone.weight;

							if (!fequal(weightTotal, 1.0f))
							{
								float weightFactor = 1.0f / weightTotal;
								int idx = 0;
								weightTotal = 0.0f;
								for (; idx < numBones - 1; ++idx)
								{
									float& weight = skinBones[idx].weight;
									weight *= weightFactor;
									weightTotal += weight;
								}
								skinBones[idx].weight = 1.0f - weightTotal;
							}
						}

						Point4 weights(1, 1, 1, 1);
						unsigned long indices = 0xffffffff;
						unsigned char* subIndices = reinterpret_cast<unsigned char*>(&indices);
						int boneIndex = 0;
						for (; boneIndex < numBones; ++boneIndex)
						{
							const SkinBone& bone = skinBones[boneIndex];
							weights[boneIndex] = bone.weight;
							unsigned short boneHandle = DumpBone(bone.boneNode);
							*subIndices++ = static_cast<unsigned char>(boneHandle);
							boneAssignments.insert(boneHandle);
						}

						vertex.m_v4BlendWeight.x = weights.x;
						vertex.m_v4BlendWeight.y = weights.y;
						vertex.m_v4BlendWeight.z = weights.z;
						vertex.m_v4BlendWeight.w = weights.w;
						vertex.m_u32BlendIndex = indices;
					}
				}

				if (m_kMesh.m_bHasMorph)
				{
					for (size_t k = 0; k < morphKeyFrameTimes.size(); ++k)
					{
						TimeValue t = morphKeyFrameTimes[k];
						coreInterface->SetTime(t, FALSE);
						auto& morphVertex = morphTrackVertices[v][k];

						{
							Point3 position = gameMesh->GetVertex(vertexIndex, false);
							position = position * nodeInvWorldTM;

							morphVertex.m_v3Position.x = position.x;
							morphVertex.m_v3Position.y = position.y;
							morphVertex.m_v3Position.z = position.z;
							UpdateVertexBounds(morphVertex.m_v3Position);
						}

						{
							Point3 normal;
							gameMesh->GetNormal(face->norm[v], normal, true);
							if (flipNormal)
								normal *= -1.0f;

							morphVertex.m_v3Normal.x = normal.x;
							morphVertex.m_v3Normal.y = normal.y;
							morphVertex.m_v3Normal.z = normal.z;
						}
					}
				}
			}

			for (size_t v = 0; v < numVertex; ++v)
			{
				VeMaxVertex& vertex = vertices[v];

				size_t index = geometry->m_kVertexBuffer.size();
				if (m_kMesh.m_bHasMorph)
				{
					const auto& morphTrack = morphTrackVertices[v];
					for (size_t k = 0; k < morphKeyFrameTimes.size(); ++k)
					{
						const auto& t = morphKeyFrameTimes[k];
						const auto& morphVertex = morphTrack[k];
						auto& morphKeyFrame = geometry->m_kMorphAnimationTrack[t];
						morphKeyFrame.push_back(morphVertex);
					}
					vertex.m_u32AnimationIndex = static_cast<unsigned long>(index);
				}

				geometry->m_kVertexBuffer.push_back(vertex);
			}
		}

		if (m_kMesh.m_bHasSkeleton)
			ve_sys.USER.I.LogFormat("Bone assignments size = %d.", boneAssignments.size());

		size_t numIndices = geometry->m_kVertexBuffer.size();
		VE_ASSERT(numIndices > 0);
		subMesh->m_kIndexBuffer.m_bUse32BitIndices = numIndices > 65535;
		if (subMesh->m_kIndexBuffer.m_bUse32BitIndices)
			subMesh->m_kIndexBuffer.m_kIndex32Buffer.resize(numVertices);
		else
			subMesh->m_kIndexBuffer.m_kIndex16Buffer.resize(numVertices);

		size_t indexStart = numIndices - numVertices;
		for (size_t idx = 0; idx < numVertices; ++idx)
		{
			if (subMesh->m_kIndexBuffer.m_bUse32BitIndices)
				subMesh->m_kIndexBuffer.m_kIndex32Buffer[idx] = static_cast<unsigned int>(idx + indexStart);
			else
				subMesh->m_kIndexBuffer.m_kIndex16Buffer[idx] = static_cast<unsigned short>(idx + indexStart);
		}

		m_kMesh.m_kSubMeshes.push_back(std::move(subMesh));
	}
	gameNode->ReleaseIGameObject();
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::ProcessTag(IGameNode* tagNode) noexcept
{
	IGameNode* rootNode = GetRootNode(tagNode);
	auto& skeleton = m_kSkeletonMap[rootNode];
	skeleton->m_kTagArray.push_back(tagNode);
}
//--------------------------------------------------------------------------
IGameNode* VeMaxSceneLoader::GetRootNode(IGameNode* gameNode) noexcept
{
	if (gameNode)
	{
		IGameNode* parentNode = gameNode->GetNodeParent();
		if (parentNode == 0)
			return gameNode;
		else
			return GetRootNode(parentNode);
	}

	return 0;
}
//--------------------------------------------------------------------------
bool VeMaxSceneLoader::MaterialComp(const VeMaxMaterial& lhs,
	const VeMaxMaterial& rhs) noexcept
{
	bool result = true;
	result = result && (lhs.m_bTwoSided == rhs.m_bTwoSided);
	result = result && (lhs.m_bPhongShading == rhs.m_bPhongShading);
	result = result && (lhs.m_bAlphaBlend == rhs.m_bAlphaBlend);
	result = result && (lhs.m_bAlphaTest == rhs.m_bAlphaTest);
	result = result && (lhs.m_bAddBlend == rhs.m_bAddBlend);

	result = result && (lhs.m_kDiffuseColour == (rhs.m_kDiffuseColour));
	result = result && (lhs.m_bDiffuseColourEnable == rhs.m_bDiffuseColourEnable);
	result = result && (lhs.m_kEmissiveColour == (rhs.m_kEmissiveColour));
	result = result && (lhs.m_bEmissiveColourEnable == rhs.m_bEmissiveColourEnable);
	result = result && (lhs.m_kSpecularColour == (rhs.m_kSpecularColour));
	result = result && (lhs.m_bSpecularColourEnable == rhs.m_bSpecularColourEnable);

	result = result && (lhs.m_kTextureSlots.size() == rhs.m_kTextureSlots.size());
	if (result)
	{
		for (size_t t = 0; t < lhs.m_kTextureSlots.size(); ++t)
		{
			auto& x = lhs.m_kTextureSlots[t];
			auto& y = rhs.m_kTextureSlots[t];

			result = result && (x->m_Name == y->m_Name);
			result = result && (x->m_eTexUnit == y->m_eTexUnit);
			result = result && (x->m_i32MapChannel == y->m_i32MapChannel);
			result = result && (x->m_eColourOpEx.operation == y->m_eColourOpEx.operation);
			result = result && (x->m_eColourOpEx.source1 == y->m_eColourOpEx.source1);
			result = result && (x->m_eColourOpEx.source2 == y->m_eColourOpEx.source2);
			result = result && (x->m_eColourOpEx.operation == y->m_eColourOpEx.operation);
			result = result && (x->m_eAlphaOpEx.source1 == y->m_eAlphaOpEx.source1);
			result = result && (x->m_eAlphaOpEx.source2 == y->m_eAlphaOpEx.source2);
			result = result && (x->m_kFrames.size() == y->m_kFrames.size());
			if (result)
			{
				for (size_t f = 0; f < x->m_kFrames.size(); ++f)
				{
					auto& a = x->m_kFrames[f];
					auto& b = y->m_kFrames[f];
					result = result && (a == b);
				}
			}
			result = result && (fequal(x->m_f32FrameTime, y->m_f32FrameTime));
		}
	}

	result = result && (lhs.m_kUVCoordinateMap.size() == rhs.m_kUVCoordinateMap.size());
	if (result)
	{
		for (auto& uvCoordinateItor : lhs.m_kUVCoordinateMap)
		{
			auto& channel = uvCoordinateItor.first;
			auto& coordinate = uvCoordinateItor.second;

			auto findItor = rhs.m_kUVCoordinateMap.find(channel);
			result = result && (findItor != rhs.m_kUVCoordinateMap.end());
			if (result)
			{
				result = result && (coordinate->m_eUMode == findItor->second->m_eUMode);
				result = result && (coordinate->m_eVMode == findItor->second->m_eVMode);
			}
		}
	}

	return result;
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::CreateDefaultMaterial() noexcept
{
	std::shared_ptr<VeMaxMaterial> defaultMaterial(new VeMaxMaterial);
	defaultMaterial->m_kRootName = defaultMaterial->m_kName = "";
	defaultMaterial->m_bTwoSided = false;
	defaultMaterial->m_bPhongShading = false;
	defaultMaterial->m_bAlphaBlend = false;
	defaultMaterial->m_bAlphaTest = false;
	defaultMaterial->m_bAddBlend = false;
	defaultMaterial->m_bDiffuseColourEnable = false;
	defaultMaterial->m_bEmissiveColourEnable = false;
	defaultMaterial->m_bSpecularColourEnable = false;
	m_kMaterialMap.insert(std::make_pair(nullptr, defaultMaterial));
}
//--------------------------------------------------------------------------
bool VeMaxSceneLoader::GetMapPath(MSTR& mapPath) noexcept
{
#	if defined(MAX_RELEASE_R12_ALPHA) && MAX_RELEASE >= MAX_RELEASE_R12_ALPHA
	MSTR mstrFullFilePath = IFileResolutionManager::GetInstance()->GetFullFilePath(mapPath, MaxSDK::AssetManagement::kBitmapAsset);
#	else
	MSTR mstrFullFilePath = IPathConfigMgr::GetPathConfigMgr()->GetFullFilePath(mapPath, false);
#	endif
	bool result = !mstrFullFilePath.isNull();
	if (result)
		mapPath = mstrFullFilePath;
	else
		ve_sys.USER.E.LogFormat("Cannot find map: \"%s\".", mapPath);
	return result;
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::AddMaterial(IGameMaterial* gameMaterial,
	const std::shared_ptr<VeMaxMaterial>& oiramMaterial) noexcept
{
	for (auto& materialPair : m_kMaterialMap)
	{
		auto& storageMaterial = materialPair.second;
		if (MaterialComp(*storageMaterial, *oiramMaterial))
		{
			m_kMaterialMap.insert(std::make_pair(gameMaterial, storageMaterial));
			return;
		}
	}
	m_kMaterialMap.insert(std::make_pair(gameMaterial, oiramMaterial));
}
//--------------------------------------------------------------------------
struct BitmapSlot
{
	VeFixedString m_kName;
	BitmapTex* m_pkBitmapTex;
	VeMaxMaterial::ColourOpEx m_kColourOpEx;
	VeMaxMaterial::AlphaOpEx m_kAlphaOpEx;
};
//--------------------------------------------------------------------------
std::shared_ptr<VeMaxMaterial> VeMaxSceneLoader::DumpMaterialProperty(
	IGameMaterial* gameMaterial, const VeFixedString& rootMaterialName) noexcept
{
	std::shared_ptr<VeMaxMaterial> material(new VeMaxMaterial);

	material->m_kRootName = rootMaterialName;

	material->m_kName = ToStr(gameMaterial->GetMaterialName());

	IGameProperty* p = gameMaterial->GetDiffuseData();
	if (p && p->GetType() == IGAME_POINT3_PROP)
	{
		Point3 diffuse;
		p->GetPropertyValue(diffuse);
		if (diffuse.Equals(Point3(1, 1, 1)))
			material->m_bDiffuseColourEnable = false;
		else
			material->m_kDiffuseColour.Set(diffuse.x, diffuse.y, diffuse.z, 1.0f);
	}
	p = gameMaterial->GetOpacityData();
	if (p && p->GetType() == IGAME_FLOAT_PROP)
	{
		VeFloat32 opacity = 0.0f;
		p->GetPropertyValue(opacity);

		if (material->m_bDiffuseColourEnable)
			material->m_kDiffuseColour.a = opacity;
	}
	p = gameMaterial->GetSpecularData();
	if (p && p->GetType() == IGAME_POINT3_PROP)
	{
		Point3 specular;
		p->GetPropertyValue(specular);
		if (specular.Equals(Point3(0, 0, 0)))
			material->m_bSpecularColourEnable = false;
		else
			material->m_kSpecularColour.Set(specular.x, specular.y, specular.z, 100.0f);
	}
	p = gameMaterial->GetGlossinessData();
	if (p && p->GetType() == IGAME_FLOAT_PROP)
	{
		float glossiness = 0.0f;
		p->GetPropertyValue(glossiness);

		if (material->m_bSpecularColourEnable)
			material->m_kSpecularColour.a = glossiness * 255.0f;
	}
	p = gameMaterial->GetEmissiveData();
	if (p && p->GetType() == IGAME_POINT3_PROP)
	{
		Point3 emissive;
		p->GetPropertyValue(emissive);
		if (emissive.Equals(Point3(0, 0, 0)))
			material->m_bEmissiveColourEnable = false;
		else
			material->m_kEmissiveColour.Set(emissive.x, emissive.y, emissive.z, 1.0f);
	}
	Mtl* maxMaterial = gameMaterial->GetMaxMaterial();
	Class_ID mtlClassID = maxMaterial->ClassID();
	if (mtlClassID == Class_ID(DMTL_CLASS_ID, 0))
	{
		StdMat2* maxStdMaterial = static_cast<StdMat2*>(maxMaterial);
		material->m_bTwoSided = maxStdMaterial->GetTwoSided() == TRUE;
		material->m_bPhongShading = !maxStdMaterial->IsFaceted();
		VeInt32 numTexMaps = gameMaterial->GetNumberOfTextureMaps();
		for (VeInt32 texMapIdx = 0; texMapIdx < numTexMaps; ++texMapIdx)
		{
			IGameTextureMap* gameTexMap = gameMaterial->GetIGameTextureMap(texMapIdx);
			VeInt32 mapSlot = gameTexMap->GetStdMapSlot();
			/*
			- ID_AM - Ambient (value 0)
			- ID_DI - Diffuse (value 1)
			- ID_SP - Specular (value 2)
			- ID_SH - Shininess (value 3). In R3 and later this is called Glossiness.
			- ID_SS - Shininess strength (value 4). In R3 and later this is called Specular Level.
			- ID_SI - Self-illumination (value 5)
			- ID_OP - Opacity (value 6)
			- ID_FI - Filter color (value 7)
			- ID_BU - Bump (value 8)
			- ID_RL - Reflection (value 9)
			- ID_RR - Refraction (value 10)
			- ID_DP - Displacement (value 11)
			*/

			if (gameTexMap &&
				(mapSlot == ID_SS || maxStdMaterial->MapEnabled(mapSlot)))
			{
				Texmap* texMap = gameTexMap->GetMaxTexmap();
				Class_ID texClassID = texMap->ClassID();
				VeVector<BitmapSlot> szBitmapSlots;
				if (texClassID == Class_ID(MIX_CLASS_ID, 0))
				{
					Mix* mix = static_cast<Mix*>(texMap);
					Texmap* layerMap0 = mix->GetSubTexmap(0);
					Texmap* layerMap1 = mix->GetSubTexmap(1);
					Texmap* mixMap = mix->GetSubTexmap(2);
					assert(layerMap0 && layerMap1 && mixMap);
					IParamBlock2* paramBlock = mix->GetParamBlock(0);
					if (paramBlock &&
						paramBlock->GetInt(Mix::mix_map1_on) && layerMap0 &&
						paramBlock->GetInt(Mix::mix_map2_on) && layerMap1 &&
						paramBlock->GetInt(Mix::mix_mask_on) && mixMap)
					{
						BitmapTex* bitmapLayer0 = static_cast<BitmapTex*>(layerMap0);
						BitmapTex* bitmapLayer1 = static_cast<BitmapTex*>(layerMap1);
						BitmapTex* bitmapMix = static_cast<BitmapTex*>(mixMap);

						bool alphaAsMono = bitmapMix->GetAlphaAsMono(TRUE) == TRUE;
						if (alphaAsMono)
						{
							bool sameBitmap = (MSTR(bitmapMix->GetMapName()) == MSTR(bitmapLayer1->GetMapName())) != 0;
							if (sameBitmap)
							{
								BitmapSlot mixLayer0Map = { "mixLayer0Map", bitmapLayer0,
								{ VeMaxMaterial::Op_Source1, VeMaxMaterial::Src_Texture, VeMaxMaterial::Src_Current },
								{ VeMaxMaterial::Op_Default, VeMaxMaterial::Src_Texture, VeMaxMaterial::Src_Current } };
								szBitmapSlots.push_back(mixLayer0Map);

								BitmapSlot mixLayer1Map = { "mixLayer1Map", bitmapLayer1,
								{ VeMaxMaterial::Op_Blend_Texture_Alpha, VeMaxMaterial::Src_Texture, VeMaxMaterial::Src_Current },
								{ VeMaxMaterial::Op_Default, VeMaxMaterial::Src_Texture, VeMaxMaterial::Src_Current } };
								szBitmapSlots.push_back(mixLayer1Map);

								BitmapSlot mixOperationMap = { "mixOperation", nullptr,
								{ VeMaxMaterial::Op_Modulate, VeMaxMaterial::Src_Current, VeMaxMaterial::Src_Diffuse },
								{ VeMaxMaterial::Op_Modulate, VeMaxMaterial::Src_Current, VeMaxMaterial::Src_Diffuse } };
								szBitmapSlots.push_back(mixOperationMap);
							}
							else
							{
								BitmapSlot mixLayer0Map = { "mixLayer0Map", bitmapLayer0,
								{ VeMaxMaterial::Op_Source1, VeMaxMaterial::Src_Texture, VeMaxMaterial::Src_Current },
								{ VeMaxMaterial::Op_Default, VeMaxMaterial::Src_Texture, VeMaxMaterial::Src_Current } };
								szBitmapSlots.push_back(mixLayer0Map);

								BitmapSlot mixMap = { "mixMap", bitmapMix,
								{ VeMaxMaterial::Op_Blend_Texture_Alpha, VeMaxMaterial::Src_Texture, VeMaxMaterial::Src_Current },
								{ VeMaxMaterial::Op_Source1, VeMaxMaterial::Src_Texture, VeMaxMaterial::Src_Current } };
								szBitmapSlots.push_back(mixMap);

								BitmapSlot mixLayer1Map = { "mixLayer1Map", bitmapLayer1,
								{ VeMaxMaterial::Op_Blend_Current_Alpha, VeMaxMaterial::Src_Texture, VeMaxMaterial::Src_Current },
								{ VeMaxMaterial::Op_Default, VeMaxMaterial::Src_Current, VeMaxMaterial::Src_Current } };
								szBitmapSlots.push_back(mixLayer1Map);

								BitmapSlot mixOperationMap = { "mixOperation", nullptr,
								{ VeMaxMaterial::Op_Modulate, VeMaxMaterial::Src_Current, VeMaxMaterial::Src_Diffuse },
								{ VeMaxMaterial::Op_Modulate, VeMaxMaterial::Src_Current, VeMaxMaterial::Src_Diffuse } };
								szBitmapSlots.push_back(mixOperationMap);
							}
						}
						else
						{
							ve_sys.USER.W.LogFormat("warning: mono channel of mix amount map need output as alpha.");
						}
					}
				}
				else
					if (texClassID == GNORMAL_CLASS_ID)
					{
						Gnormal* gnormal = static_cast<Gnormal*>(texMap);
						IParamBlock2* paramBlock = gnormal->GetParamBlock(0);
						if (paramBlock && paramBlock->GetInt(Gnormal::gn_map1on))
						{
							Texmap* normalMap = gnormal->GetSubTexmap(0);
							assert(normalMap);
							if (normalMap)
							{
								BitmapTex* bitmapTex = static_cast<BitmapTex*>(normalMap);
								assert(bitmapTex);
								if (bitmapTex)
								{
									BitmapSlot bitmapSlot = { "normalMap", bitmapTex,
									{ VeMaxMaterial::Op_Default, VeMaxMaterial::Src_Current, VeMaxMaterial::Src_Texture },
									{ VeMaxMaterial::Op_Default, VeMaxMaterial::Src_Current, VeMaxMaterial::Src_Texture } };
									szBitmapSlots.push_back(bitmapSlot);
								}
							}
						}
					}
					else
						if (texClassID == Class_ID(BMTEX_CLASS_ID, 0))
						{
							BitmapTex* bitmapTex = static_cast<BitmapTex*>(texMap);
							assert(bitmapTex);
							if (bitmapTex)
							{
								BitmapSlot bitmapSlot = { "diffuseMap", bitmapTex,
								{ VeMaxMaterial::Op_Default, VeMaxMaterial::Src_Current, VeMaxMaterial::Src_Texture },
								{ VeMaxMaterial::Op_Default, VeMaxMaterial::Src_Current, VeMaxMaterial::Src_Texture } };
								szBitmapSlots.push_back(bitmapSlot);
							}
						}

				for (auto& bitmapSlot : szBitmapSlots)
				{
					if (bitmapSlot.m_pkBitmapTex == nullptr)
					{
						std::unique_ptr<VeMaxMaterial::TextureSlot> textureSlot(new VeMaxMaterial::TextureSlot);
						textureSlot->m_Name = bitmapSlot.m_kName;
						textureSlot->m_eTexUnit = VeMaxMaterial::TU_Operation;
						textureSlot->m_eColourOpEx = bitmapSlot.m_kColourOpEx;
						textureSlot->m_eAlphaOpEx = bitmapSlot.m_kAlphaOpEx;
						textureSlot->m_kFrames.clear();
						textureSlot->m_f32FrameTime = 1.0f;
						material->m_kTextureSlots.push_back(std::move(textureSlot));
					}
					else
					{
						MSTR mstrTexturePath = bitmapSlot.m_pkBitmapTex->GetMapName();
						if (GetMapPath(mstrTexturePath))
						{
							std::unique_ptr<VeMaxMaterial::TextureSlot> textureSlot(new VeMaxMaterial::TextureSlot);
							textureSlot->m_f32FrameTime = 1.0f;
							VeVector<VeFixedString> frames;
							BitmapInfo bi;
							TheManager->GetImageInfo(&bi, mstrTexturePath);
							int idx = mstrTexturePath.first('.');
							const MCHAR* ext = nullptr;
							if (idx != -1)
								ext = mstrTexturePath.data() + idx + 1;
							/*if (bi.NumberFrames() > 0 &&
								ext && _tcscmp(ext, _T("ifl")) == 0)
							{
								textureSlot->m_f32FrameTime = 30 / bitmapSlot.m_pkBitmapTex->GetPlaybackRate();							
								MSTR path = mstrTexturePath.Substr(0, mstrTexturePath.last(_T('\\')) + 1);								
								std::ifstream fs("");
								std::string line;
								while (std::getline(fs, line))
								{
									// 要求查找程序也在指定目录中搜索
									MSTR mstrFrame = Ansi2Mstr(line);
									GetMapPath(mstrFrame);
									textureSlot->m_kFrames.push_back(ToStr(mstrFrame));
								}
							}
							else
								textureSlot->m_kFrames.push_back(ToStr(mstrTexturePath));*/
							textureSlot->m_kFrames.push_back(ToStr(mstrTexturePath));

							textureSlot->m_eTexUnit = VeMaxMaterial::TU_Unknow;
							textureSlot->m_i32MapChannel = bitmapSlot.m_pkBitmapTex->GetMapChannel();
							textureSlot->m_eColourOpEx = bitmapSlot.m_kColourOpEx;
							textureSlot->m_eAlphaOpEx = bitmapSlot.m_kAlphaOpEx;

							// 记录uv变换矩阵
							{
								Texmap* texmap = gameTexMap->GetMaxTexmap();

								std::unique_ptr<VeMaxMaterial::UVCoordinate> uvCoordinate(new VeMaxMaterial::UVCoordinate);

								// UV镜像
								int textureTiling = texmap->GetTextureTiling();
								if (textureTiling & U_MIRROR)
									uvCoordinate->m_eUMode = VeMaxMaterial::TM_mirror;
								else
									if (textureTiling & U_WRAP)
										uvCoordinate->m_eUMode = VeMaxMaterial::TM_wrap;
									else
										uvCoordinate->m_eUMode = VeMaxMaterial::TM_clamp;

								if (textureTiling & V_MIRROR)
									uvCoordinate->m_eVMode = VeMaxMaterial::TM_mirror;
								else
									if (textureTiling & V_WRAP)
										uvCoordinate->m_eVMode = VeMaxMaterial::TM_wrap;
									else
										uvCoordinate->m_eVMode = VeMaxMaterial::TM_clamp;

								// 经验证IGameUVGen::GetUVTransform()有bug, 估计是做了swap(y, z); z = -z;的操作
								// 但实际上uvgen并不是像max坐标系那样z朝上和y朝里, 所以是不需要做上述变换的
								// 所以直接用sdk中的Texmap::GetUVTransform反而是正确的
								Matrix3 uvtrans(TRUE);
								texmap->GetUVTransform(uvtrans);
								uvCoordinate->m_m4Transform = FromGMatrix(GMatrix(uvtrans));
								material->m_kUVCoordinateMap.insert(std::make_pair(textureSlot->m_i32MapChannel, std::move(uvCoordinate)));
							}

							int alphaSource = bitmapSlot.m_pkBitmapTex->GetAlphaSource();
							switch (mapSlot)
							{
							case ID_DI: // Diffuse Map
								textureSlot->m_Name = bitmapSlot.m_kName;
								textureSlot->m_eTexUnit = VeMaxMaterial::TU_DiffuseMap;
								material->m_bDiffuseColourEnable = false;
								break;

							case ID_SP: // Specular Level Map/Specular Map, 因为specular = specular map color * specular level, 所以一视同仁
							case ID_SS:
								textureSlot->m_Name = "specularMap";
								textureSlot->m_eTexUnit = VeMaxMaterial::TU_SpecularMap;
								material->m_bSpecularColourEnable = false;
								break;

							case ID_SI: // Self Illumination Map
								textureSlot->m_Name = "emissiveMap";
								textureSlot->m_eTexUnit = VeMaxMaterial::TU_EmissiveMap;
								material->m_bEmissiveColourEnable = false;
								break;

							case ID_OP: // Opacity Map
										// 透明贴图只设置渲染状态, 不必导出贴图(与diffuseMap重复了), 贴图必须有Alpha通道
								if (alphaSource == ALPHA_FILE)
								{
									// 移动平台上没有alphaTest, 而且使用alpha blend效率也远高于discard
									//if (config.mobilePlatform)
									//	material->alphaBlend = true;
									//else
									{
										// 根据"预乘Alpha"决定是AlphaBlend还是AlphaTest
										if (bitmapSlot.m_pkBitmapTex->GetPremultAlpha(TRUE))
										{
											material->m_bAlphaTest = true;
										}
										else
										{
											material->m_bAlphaBlend = true;
										}
									}
								}
								else if (alphaSource == ALPHA_RGB)
								{
									// 没有alpha通道, 认为是以RGB方式Add
									material->m_bAddBlend = true;
								}
								continue;

							case ID_BU:	// Bump/Normal Map
								textureSlot->m_Name = "normalMap";
								textureSlot->m_eTexUnit = VeMaxMaterial::TU_NormalMap;
								break;

							case ID_RL:	// Reflection Map
								textureSlot->m_Name = "reflectionMap";
								textureSlot->m_eTexUnit = VeMaxMaterial::TU_ReflectionMap;
								break;

							default:
								continue;
							}

							bool dumpTextureSlot = true;
							// 固定管线只导出diffuseMap
							//if (config.renderingType == RT_FixedFunction)
							//	dumpTextureSlot = mapSlot == ID_DI;

							if (dumpTextureSlot)
								material->m_kTextureSlots.push_back(std::move(textureSlot));
						}
					}
				}
			}
		}
	}

	return material;
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::UpdateVertexBounds(const VeVector3& position) noexcept
{
	m_kMesh.m_kVertexBoundingBox.pmax.x = VE_MAX(m_kMesh.m_kVertexBoundingBox.pmax.x, position.x);
	m_kMesh.m_kVertexBoundingBox.pmax.y = VE_MAX(m_kMesh.m_kVertexBoundingBox.pmax.y, position.y);
	m_kMesh.m_kVertexBoundingBox.pmax.z = VE_MAX(m_kMesh.m_kVertexBoundingBox.pmax.z, position.z);

	m_kMesh.m_kVertexBoundingBox.pmin.x = VE_MIN(m_kMesh.m_kVertexBoundingBox.pmin.x, position.x);
	m_kMesh.m_kVertexBoundingBox.pmin.y = VE_MIN(m_kMesh.m_kVertexBoundingBox.pmin.y, position.y);
	m_kMesh.m_kVertexBoundingBox.pmin.z = VE_MIN(m_kMesh.m_kVertexBoundingBox.pmin.z, position.z);
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::UpdateUVBounds(const VeVector2& uv) noexcept
{
	m_kMesh.m_kUVBoundingBox.pmax.x = VE_MAX(m_kMesh.m_kUVBoundingBox.pmax.x, uv.x);
	m_kMesh.m_kUVBoundingBox.pmax.y = VE_MAX(m_kMesh.m_kUVBoundingBox.pmax.y, uv.y);

	m_kMesh.m_kUVBoundingBox.pmin.x = VE_MIN(m_kMesh.m_kUVBoundingBox.pmin.x, uv.x);
	m_kMesh.m_kUVBoundingBox.pmin.y = VE_MIN(m_kMesh.m_kUVBoundingBox.pmin.y, uv.y);
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::ProcessVertexAnimation(IGameNode* gameNode,
	const VeMaxSceneNode* sceneNode,
	VeVector<TimeValue>& morphKeyFrameTimes) noexcept
{
	IGameControl* gameControl = gameNode->GetIGameControl();
	if (gameControl &&
		gameControl->IsAnimated(IGAME_POINT3))
	{
		IGameKeyTab samples;
		if (gameControl->GetQuickSampledKeys(samples, IGAME_POINT3))
		{
			int keyFrameCount = samples.Count();
			if (keyFrameCount > 0)
			{
				m_kMesh.m_bHasMorph = VE_TRUE;

				size_t animationCount = sceneNode->m_kAnimationDescs.size();
				m_kMesh.m_kMorphAnimations.resize(animationCount);
				for (size_t n = 0; n < animationCount; ++n)
				{
					const auto& animDesc = sceneNode->m_kAnimationDescs[n];
					TimeValue timeStart = animDesc.m_i32Start;
					TimeValue timeEnd = animDesc.m_i32End;

					auto& morphAnimations = m_kMesh.m_kMorphAnimations[n];
					morphAnimations.push_back(timeStart);
					morphKeyFrameTimes.push_back(timeStart);
					for (int i = 0; i < keyFrameCount; ++i)
					{
						TimeValue t = samples[i].t;
						if (t >= timeStart && t <= timeEnd)
						{
							morphAnimations.push_back(t);
							morphKeyFrameTimes.push_back(t);
						}
					}
					morphAnimations.push_back(timeEnd);
					morphKeyFrameTimes.push_back(timeEnd);

					std::sort(morphAnimations.begin(), morphAnimations.end());
					morphAnimations.erase(std::unique(morphAnimations.begin(), morphAnimations.end()), morphAnimations.end());
				}
				std::sort(morphKeyFrameTimes.begin(), morphKeyFrameTimes.end());
				morphKeyFrameTimes.erase(std::unique(morphKeyFrameTimes.begin(), morphKeyFrameTimes.end()), morphKeyFrameTimes.end());
			}
		}
	}
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::DumpSkeletal(IGameNode* gameNode) noexcept
{
	DumpBone(gameNode);
	int count = gameNode->GetChildCount();
	for (int n = 0; n < count; ++n)
	{
		IGameNode* childNode = gameNode->GetNodeChild(n);
		DumpSkeletal(childNode);
	}
}
//--------------------------------------------------------------------------
VeUInt16 VeMaxSceneLoader::DumpBone(IGameNode* pBone, bool isTag) noexcept
{
	IGameNode* rootNode = GetRootNode(pBone);
	auto& skeleton = m_kSkeletonMap[rootNode];
	if (!skeleton)
		skeleton.reset(new VeMaxSkeleton);

	auto& bone = skeleton->m_kBoneMap[pBone];
	if (!bone)
	{
		bone.reset(new VeMaxBone);
		bone->m_bIsTag = isTag;
		bone->m_kName = ToStr(pBone->GetName());
		bone->m_u16Handle = static_cast<VeUInt16>(skeleton->m_kBoneMap.size()) - 1;
		bone->m_u16ParentHandle = -1;
		GMatrix initTM = GetNodeTransform(pBone, *bone, 0);
		bone->m_m4InitMatrix = FromGMatrix(initTM);
		IGameNode* pParentBone = pBone->GetNodeParent();
		if (pParentBone)
			bone->m_u16ParentHandle = DumpBone(pParentBone);
	}
	return bone->m_u16Handle;
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::CheckMirroredMatrix(GMatrix& tm) noexcept
{
	if (tm.Parity() == -1)
		tm[2] *= -1.0f;
}
//--------------------------------------------------------------------------
GMatrix VeMaxSceneLoader::GetNodeTransform(IGameNode* pNode,
	VeMaxBone& bone, TimeValue t) noexcept
{
	auto transItor = bone.m_kWorldTM.find(t);
	if (transItor == bone.m_kWorldTM.end())
	{
		GMatrix tm = pNode->GetWorldTM(t);
		tm = tm * m_kInitSkinTM.Inverse();
		CheckMirroredMatrix(tm);
		transItor = bone.m_kWorldTM.insert(std::make_pair(t, FromGMatrix(tm))).first;
	}
	return ToGMatrix(transItor->second);
}
//--------------------------------------------------------------------------
void VeMaxSceneLoader::DecomposeNodeMatrix(IGameNode* pBone,
	IGameNode* rootNode, Point3& nodePos, Point3& nodeScale,
	Quat& nodeOriet, TimeValue t, bool exportInitMatrix, bool isTag) noexcept
{
	auto& boneMap = m_kSkeletonMap[rootNode]->m_kBoneMap;
	auto& bone = boneMap[pBone];
	GMatrix tm;
	if (exportInitMatrix)
	{
		tm = ToGMatrix(bone->m_m4InitMatrix);

		IGameNode* parentNode = pBone->GetNodeParent();
		if (parentNode)
		{
			GMatrix ptm = ToGMatrix(boneMap[parentNode]->m_m4InitMatrix);
			tm *= ptm.Inverse();
		}
	}
	else
	{
		tm = GetNodeTransform(pBone, *bone, t);

		IGameNode* parentNode = pBone->GetNodeParent();
		if (parentNode)
		{
			auto& parentBone = boneMap[parentNode];
			GMatrix ptm = GetNodeTransform(parentNode, *parentBone, t);

			tm *= ptm.Inverse();
		}
	}

	nodePos = tm.Translation();

	if (isTag)
	{
		nodeScale.Set(1, 1, 1);
		bone->m_bNonuniformScaleChecked = true;
	}
	else
	{
		nodeScale = tm.Scaling();

		if (!bone->m_bNonuniformScaleChecked)
		{
			const float epsilon = 1e-5f;
			if (nodeScale.x < epsilon) nodeScale.x = epsilon;
			if (nodeScale.y < epsilon) nodeScale.y = epsilon;
			if (nodeScale.z < epsilon) nodeScale.z = epsilon;

			const float tolerance = 1e-3f;
			float	xyScale = nodeScale.x / nodeScale.y,
				xzScale = nodeScale.x / nodeScale.z,
				yzScale = nodeScale.y / nodeScale.z;
			if (!fequal(xyScale, xzScale, tolerance) ||
				!fequal(xyScale, yzScale, tolerance) ||
				!fequal(xyScale, yzScale, tolerance))
			{
				bone->m_bNonuniformScaleChecked = true;
				VeDebugOutput("Nonuniform scale found: \"%s\".", pBone->GetName());
			}
		}
	}

	nodeOriet = tm.Rotation();
	nodeOriet.w = -nodeOriet.w;
}
//--------------------------------------------------------------------------
box3 VeMaxSceneLoader::FromBox3(Box3 box) noexcept
{
	VE_ASSERT(!box.IsEmpty());

	std::swap(box.pmin.y, box.pmin.z);
	box.pmin.z = -box.pmin.z;
	std::swap(box.pmax.y, box.pmax.z);
	box.pmax.z = -box.pmax.z;

	if (box.pmin.x > box.pmax.x)
		std::swap(box.pmin.x, box.pmax.x);
	if (box.pmin.y > box.pmax.y)
		std::swap(box.pmin.y, box.pmax.y);
	if (box.pmin.z > box.pmax.z)
		std::swap(box.pmin.z, box.pmax.z);

	return{ { box.pmin.x, box.pmin.y, box.pmin.z },{ box.pmax.x, box.pmax.y, box.pmax.z } };
}
//--------------------------------------------------------------------------
GMatrix VeMaxSceneLoader::ToGMatrix(matrix& m) noexcept
{
	GMatrix gm;
	memcpy(&(gm[0][0]), &m, sizeof(m));
	return gm;
}
//--------------------------------------------------------------------------
matrix VeMaxSceneLoader::FromGMatrix(const GMatrix& gm) noexcept
{
	matrix m;
	memcpy(&m, &(gm[0][0]), sizeof(m));
	return m;
}
//--------------------------------------------------------------------------
