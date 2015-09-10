////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   VeMaxScene.h
//  Created:     2015/09/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class IGameNode;
class Object;

typedef VeStringMap<VeVector<VeFixedString>> VeMaxUserDefinedPropertyMap;

struct VeMaxSceneAnimation
{
	VeFixedString m_Name;
	VeInt32 m_i32Start;
	VeInt32 m_i32End;
};

typedef VeVector<VeMaxSceneAnimation> VeMaxAnimationDesc;

enum VeMaxSceneNodeType
{
	ST_Geometry,
	ST_Camera,
	ST_Light,
	ST_Bone,
	ST_Helper,
	ST_Spline,
	ST_Target,
	ST_PhysX,
	ST_Apex,
	ST_UnSupported
};

struct VeMaxSceneNode
{
	VeMaxSceneNodeType m_eType;
	VeFixedString m_kName;
	VeMaxSceneNode* m_pkNodeReference;
	VeFixedString m_kMeshName;
	VeFixedString m_kMaterialFileName;
	VE_BOOL m_bHasUV;
	VeFloat32 m_f32CenterU;
	VeFloat32 m_f32CenterV;
	VeFloat32 m_f32ExtentU;
	VeFloat32 m_f32ExtentV;
	VeFloat32 m_f32PositionCenterX;
	VeFloat32 m_f32PositionCenterY;
	VeFloat32 m_f32PositionCenterZ;
	VeFloat32 m_f32PositionExtentX;
	VeFloat32 m_f32PositionExtentY;
	VeFloat32 m_f32PositionExtentZ;	
	VE_BOOL m_bHasSkeleton;
	VE_BOOL m_bHasMorph;
	VeMaxUserDefinedPropertyMap m_kUserDefinedPropertyMap;
	VeMaxAnimationDesc m_kAnimationDescs;

};

typedef VeMap<IGameNode*, VeMaxSceneNode> VeMaxSceneNodeMap;
typedef VeStringSet<> VeMaxSceneNodeNameSet;
typedef VeMap<Object*, VeMaxSceneNode*> VeMaxSceneNodeInstanceMap;
typedef VeVector<IGameNode*> VeMaxNodeContainer;
