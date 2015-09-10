////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   VeMaxSceneLoader.h
//  Created:     2015/09/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VeMaxSceneLoader : public VeSingleton<VeMaxSceneLoader>
{
public:
	VeMaxSceneLoader() noexcept;

	~VeMaxSceneLoader() noexcept;

	bool Load(bool bSelected) noexcept;

	void Clear() noexcept;

private:
	VeMaxSceneNodeType GetSceneNodeType(INode* node) noexcept;

	bool IsNodeSkinned(INode* node) noexcept;

	bool IsNodeMorphed(Animatable* node) noexcept;

	bool IsNodeHidden(IGameNode* gameNode) noexcept;

	void DumpNodes(IGameNode* gameNode) noexcept;

	void ProcessNodeUserDefinedProperties(IGameNode* gameNode, VeMaxSceneNode& sceneNode) noexcept;

	void ProcessAnimation(VeMaxSceneNode& sceneNode) noexcept;

	void ProcessMaterial(IGameMaterial* gameMaterial, const VeFixedString& rootMaterialName) noexcept;

	void ProcessGeometry(IGameNode* gameNode, const VeMaxSceneNodeMap& sceneNodeMap) noexcept;

	void ProcessTag(IGameNode* tagNode) noexcept;

private:
	IGameNode* GetRootNode(IGameNode* gameNode) noexcept;

	bool MaterialComp(const VeMaxMaterial& lhs, const VeMaxMaterial& rhs) noexcept;

	void CreateDefaultMaterial() noexcept;

	bool GetMapPath(MSTR& mapPath) noexcept;

	void AddMaterial(IGameMaterial* gameMaterial, const std::shared_ptr<VeMaxMaterial>& oiramMaterial) noexcept;

	std::shared_ptr<VeMaxMaterial> DumpMaterialProperty(IGameMaterial* gameMaterial, const VeFixedString& rootMaterialName) noexcept;

	void UpdateVertexBounds(const VeVector3& position) noexcept;

	void UpdateUVBounds(const VeVector2& uv) noexcept;

	void ProcessVertexAnimation(IGameNode* gameNode, const VeMaxSceneNode* sceneNode, VeVector<TimeValue>& morphKeyFrameTimes) noexcept;

	void DumpSkeletal(IGameNode* gameNode) noexcept;

	VeUInt16 DumpBone(IGameNode* pBone, bool isTag = false) noexcept;

	void CheckMirroredMatrix(GMatrix& tm) noexcept;

	GMatrix GetNodeTransform(IGameNode* pNode, VeMaxBone& bone, TimeValue t) noexcept;

	void DecomposeNodeMatrix(IGameNode* pBone, IGameNode* rootNode, Point3& nodePos, Point3& nodeScale, Quat& nodeOriet, TimeValue t, bool exportInitMatrix = false, bool isTag = false) noexcept;

	box3 FromBox3(Box3 box) noexcept;

	GMatrix ToGMatrix(matrix& m) noexcept;

	matrix FromGMatrix(const GMatrix& gm) noexcept;

private:
	VeMaxSceneNodeMap m_kSceneNodeMap;
	VeMaxSceneNodeNameSet m_kSceneNodeNameSet;
	VeMaxSceneNodeInstanceMap m_kSceneNodeInstanceMap;
	VeMaxMaterialMap m_kMaterialMap;
	VeMaxMaterialContainer m_kMaterials;
	VeMaxMesh m_kMesh;
	VeMaxSkeletonMap m_kSkeletonMap;
	GMatrix m_kInitSkinTM;

};

#define scence_loader VeMaxSceneLoader::GetSingleton()
