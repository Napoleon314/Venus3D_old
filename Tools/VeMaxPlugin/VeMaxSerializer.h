////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   VeMaxSerializer.h
//  Created:     2015/09/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

const float EPSILON = 1e-5f;

inline bool fequal(float a, float b, float epsilon = EPSILON) { return fabs(b - a) <= epsilon; }

inline bool fzero(float a, float epsilon = EPSILON) { return fabs(a) <= epsilon; }

struct box2
{
	VeVector2 pmin, pmax;
	void Init() { pmin.Set(FLT_MAX, FLT_MAX); pmax.Set(FLT_MIN, FLT_MIN); }
	VeVector2 center()const { return VeVector2((pmax.x + pmin.x) * 0.5f, (pmax.y + pmin.y) * 0.5f); }
	VeVector2 halfSize()const { return VeVector2((pmax.x - pmin.x) * 0.5f, (pmax.y - pmin.y) * 0.5f); }
};

struct box3
{
	VeVector3 pmin, pmax;
	void Init() { pmin.Set(FLT_MAX, FLT_MAX, FLT_MAX); pmax.Set(FLT_MIN, FLT_MIN, FLT_MIN); }
	VeVector3 center()const { return VeVector3((pmax.x + pmin.x) * 0.5f, (pmax.y + pmin.y) * 0.5f, (pmax.z + pmin.z) * 0.5f); }
	VeVector3 halfSize()const { return VeVector3((pmax.x - pmin.x) * 0.5f, (pmax.y - pmin.y) * 0.5f, (pmax.z - pmin.z) * 0.5f); }
};

struct matrix
{
	float m[4][4];
	float* operator & () { return &m[0][0]; }
	const float* operator & ()const { return &m[0][0]; }
	void init() {
		m[0][0] = 1; m[0][0] = 0; m[0][0] = 0; m[0][0] = 0;
		m[0][0] = 0; m[0][0] = 1; m[0][0] = 0; m[0][0] = 0;
		m[0][0] = 0; m[0][0] = 0; m[0][0] = 1; m[0][0] = 0;
		m[0][0] = 0; m[0][0] = 0; m[0][0] = 0; m[0][0] = 1;
	}
};

struct VeMaxMaterial
{
	enum Operation
	{
		Op_Source1,
		Op_Source2,
		Op_Modulate,
		Op_Modulate_x2,
		Op_Modulate_x4,
		Op_Add,
		Op_Add_Signed,
		Op_Add_Smooth,
		Op_Substract,
		Op_Blend_Diffuse_Alpha,
		Op_Blend_Texture_Alpha,
		Op_Blend_Current_Alpha,
		Op_Manual,
		Op_Dotproduct,
		Op_Diffuse_Colour,
		Op_Default
	};

	enum Source
	{
		Src_Current,
		Src_Texture,
		Src_Diffuse,
		Src_Specular,
		Src_Manual
	};

	enum TextureUnit
	{
		TU_Unknow,
		TU_Operation,
		TU_DiffuseMap,
		TU_LightMap,
		TU_NormalMap,
		TU_SpecularMap,
		TU_EmissiveMap,
		TU_ReflectionMap
	};

	struct ColourOpEx
	{
		Operation operation;
		Source source1;
		Source source2;
	};

	struct AlphaOpEx
	{
		Operation operation;
		Source source1;
		Source source2;
	};

	struct TextureSlot
	{
		VeFixedString m_Name;
		TextureUnit m_eTexUnit;
		VeInt32 m_i32MapChannel;
		ColourOpEx m_eColourOpEx;
		AlphaOpEx m_eAlphaOpEx;
		VeVector<VeFixedString> m_kFrames;
		VeFloat32 m_f32FrameTime;
	};
	typedef VeVector<std::unique_ptr<TextureSlot>> TextureSlotContainer;
	TextureSlotContainer m_kTextureSlots;

	enum TextureAdressMode
	{
		TM_wrap,
		TM_clamp,
		TM_mirror
	};

	struct UVCoordinate
	{
		TextureAdressMode m_eUMode;
		TextureAdressMode m_eVMode;
		matrix m_m4Transform;
	};
	typedef VeMap<VeInt32, std::unique_ptr<UVCoordinate>> UVCoordinateMap;
	UVCoordinateMap m_kUVCoordinateMap;

	VeFixedString m_kRootName;
	VeFixedString m_kName;
	VE_BOOL m_bTwoSided = VE_FALSE;
	VE_BOOL m_bPhongShading = VE_TRUE;
	VE_BOOL m_bAlphaBlend = VE_FALSE;
	VE_BOOL m_bAlphaTest = VE_FALSE;
	VE_BOOL m_bAddBlend = VE_FALSE;

	VeColor m_kDiffuseColour;
	VE_BOOL m_bDiffuseColourEnable = VE_TRUE;
	VeColor m_kEmissiveColour;
	VE_BOOL m_bEmissiveColourEnable = VE_TRUE;
	VeColor m_kSpecularColour;
	VE_BOOL m_bSpecularColourEnable = VE_TRUE;
	VE_BOOL	m_bPackedTexcoords = VE_FALSE;
	VeSet<VeFixedString> m_kExtended;
	VE_BOOL m_bIsUsed = VE_FALSE;

};

typedef std::shared_ptr<VeMaxMaterial> VeMaxMaterialPtr;
typedef VeMap<void*, VeMaxMaterialPtr> VeMaxMaterialMap;
typedef VeVector<VeMaxMaterialPtr> VeMaxMaterialContainer;

enum VeMaxVertexElementSemantic
{
	Ves_Position = 1 << 1,
	Ves_Normal = 1 << 2,
	Ves_Diffuse = 1 << 3,
	Ves_Texture_Coordinate0 = 1 << 4,
	Ves_Texture_Coordinate1 = 1 << 5,
	Ves_Texture_Coordinate2 = 1 << 6,
	Ves_Texture_Coordinate3 = 1 << 7,
	Ves_Texture_Coordinate4 = 1 << 8,
	Ves_Texture_Coordinate5 = 1 << 9,
	Ves_Texture_Coordinate6 = 1 << 10,
	Ves_Texture_Coordinate7 = 1 << 11,
	Ves_Binormal = 1 << 12,
	Ves_Tangent = 1 << 13,
	Ves_Blend_Weights = 1 << 14,
	Ves_Blend_Indices = 1 << 15,

	Ves_VertexAnimationIndex = 1 << 16,
};

struct VeMaxVertex
{
	VeVector3 m_v3Position;
	VeInt16 m_ai16Short4Position[4];
	VeRGBA m_kDiffuse;
	VeVector3 m_v3Normal;
	VeByte m_abyNormal[4];
	VeVector<VeVector2> m_kTexcoords;
	VeVector<std::pair<VeInt16, VeInt16>>	m_kShort2Texcoords;
	VeInt16 m_ai16Texcoord[4];
	VeVector4 m_v4Tangent;
	VeByte m_abyTangent[4];
	VeVector3 m_v3Binormal;
	VeByte m_abyBinormal[4];
	VeVector4 m_v4BlendWeight;
	VeByte m_abyBlendWeight[4];
	VeUInt32 m_u32BlendIndex;
	VeUInt32 m_u32AnimationIndex;
};

struct VeMaxMorphVertex
{
	VeVector3 m_v3Position;
	VeInt16 m_ai16Position[4];
	VeVector3 m_v3Normal;
	VeByte m_abyNormal[4];
};

typedef VeInt32 VeMaxMorphKeyTick;
typedef VeVector<VeMaxMorphVertex> VeMaxMorphKeyFrame;
typedef VeMap<VeMaxMorphKeyTick, VeMaxMorphKeyFrame> VeMaxMorphAnimationTrack;
typedef VeVector<VeVector<VeMaxMorphKeyTick>> VeMaxMorphAnimations;

struct VeMaxLOD
{
	VeUInt32 m_u32IndexCount = 0;
	void* m_pvIndexBuffer = nullptr;

	~VeMaxLOD() noexcept
	{
		if (m_pvIndexBuffer)
		{
			VeFree(m_pvIndexBuffer);
			m_pvIndexBuffer = nullptr;
		}
	}
};

typedef VeUInt32 VeMaxVertexDeclaration;
typedef VeVector<VeMaxVertex> VeMaxVertexBuffer;

struct VeMaxIndexBuffer
{
	VE_BOOL m_bUse32BitIndices = VE_FALSE;
	VeVector<VeUInt32> m_kIndex32Buffer;
	VeVector<VeUInt16> m_kIndex16Buffer;
};

struct VeMaxGeometryData
{
	VeMaxMaterialPtr m_spMaterial;
	VeMaxVertexDeclaration m_flagVertexDeclaration = 0;
	VeMaxVertexBuffer m_kVertexBuffer;
	VeMaxMorphAnimationTrack m_kMorphAnimationTrack;
	VeVector<VeMaxLOD> m_kIndexLODs;
};

typedef std::shared_ptr<VeMaxGeometryData> VeMaxGeometry;

struct VeMaxSubMesh
{
	VeFixedString m_kMaterialName;
	VeInt32 m_i32MatID = -1;
	VeMaxGeometry m_spGeometry;
	VeMaxIndexBuffer m_kIndexBuffer;
};
typedef VeVector<std::unique_ptr<VeMaxSubMesh>> VeMaxSubMeshContainer;


struct VeMaxMesh
{
	VeFixedString m_kName;
	VE_BOOL m_bHasSkeleton = VE_FALSE;
	VeFixedString m_kSkeletonName;
	VE_BOOL m_bHasMorph = VE_FALSE;
	box3 m_kVertexBoundingBox;
	box2 m_kUVBoundingBox;

	VeMaxGeometry m_spSharedGeometry;
	VeMaxSubMeshContainer m_kSubMeshes;
	VeMaxMorphAnimations m_kMorphAnimations;

	void clear() noexcept
	{
		m_kName = "";
		m_bHasSkeleton = VE_FALSE;
		m_kSkeletonName = "";
		m_bHasMorph = VE_FALSE;
		m_kVertexBoundingBox.Init();
		m_kVertexBoundingBox.Init();
		m_spSharedGeometry = nullptr;
		m_kSubMeshes.clear();
		m_kMorphAnimations.clear();
	}
};

struct VeMaxKeyFrame
{
	VeVector4 m_v4Rotation;
	VeVector3 m_v3Translation;	
	VeVector3 m_v3Scale;
	VeFloat32 m_f32FOV;
	VeInt32 m_i32FrameTime;
	VeFloat32 m_f32KeyTime;
};

struct VeMaxAnimation
{
	VeInt32 m_i32Start;
	VeInt32 m_i32End;
	VeVector<VeMaxKeyFrame>	m_kKeyFrames;
};

struct VeMaxBone
{
	VE_BOOL m_bIsTag = VE_FALSE;
	VE_BOOL m_bNonuniformScaleChecked = VE_FALSE;
	VeFixedString m_kName;
	VeUInt16 m_u16Handle = 0;
	VeUInt16 m_u16ParentHandle = 0;
	VeVector3 m_v3InitTranslation;
	VeQuaternion m_qInitRotation;
	VeVector3 m_v3InitScale;
	matrix m_m4InitMatrix;
	typedef VeMap<VeInt32, matrix> TransformMap;
	TransformMap m_kWorldTM;
	VeVector<VeMaxAnimation> m_kAnimations;
};
typedef VeMap<void*, std::unique_ptr<VeMaxBone>> VeMaxBoneMap;

struct VeMaxSkeleton
{
	VeFixedString m_kSkeletonName;
	VeInt32 m_i32Start;
	VeInt32 m_i32End;
	VeMaxBoneMap m_kBoneMap;
	VeVector<void*> m_kTagArray;
};
typedef VeMap<void*, std::unique_ptr<VeMaxSkeleton>> VeMaxSkeletonMap;
