////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMatrix.h
//  Created:     2015/09/04 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API alignas(16) VeMatrix4X3 : public VeMemA16Object
{
public:
	union
	{
		struct
		{
			VeFloat32 _11, _21, _31, _41;
			VeFloat32 _12, _22, _32, _42;
			VeFloat32 _13, _23, _33, _43;
		};
		VeFloat32 f[3][4];
		VE_FLOAT4A v[3];
		VeVector4 vec[3];
		VE_FLOAT4X3A m;
	};

	VeMatrix4X3() noexcept
	{
		SetIdentity();
	}

	VeMatrix4X3(const VE_FLOAT4X3& kMatrix) noexcept
	{
		VE_MATRIX4X3 mat = VeLoadFloat4x3(&kMatrix);
		VeStoreFloat4x3A(&m, mat);
	}

	VeMatrix4X3(const VE_FLOAT4X3A& kMatrix) noexcept
	{
		VE_MATRIX4X3 mat = VeLoadFloat4x3A(&kMatrix);
		VeStoreFloat4x3A(&m, mat);
	}

	VeMatrix4X3(const VeMatrix4X3& kMatrix) noexcept
	{
		VE_MATRIX4X3 mat = VeLoadFloat4x3A(&kMatrix.m);
		VeStoreFloat4x3A(&m, mat);
	}

	VeMatrix4X3(
		VeFloat32 _11, VeFloat32 _21, VeFloat32 _31, VeFloat32 _41,
		VeFloat32 _12, VeFloat32 _22, VeFloat32 _32, VeFloat32 _42,
		VeFloat32 _13, VeFloat32 _23, VeFloat32 _33, VeFloat32 _43) noexcept
	{
		VE_MATRIX4X3 mat = VeMatrix4X3Set(_11, _21, _31, _41,
			_12, _22, _32, _42, _13, _23, _33, _43);
		VeStoreFloat4x3A(&m, mat);
	}

	VeMatrix4X3(std::initializer_list<VeFloat32> kInitList) noexcept
	{
		VE_MATRIX4X3 mat = VeLoadFloat4x3((const VE_FLOAT4X3*)kInitList.begin());
		VeStoreFloat4x3A(&m, mat);
	}

	VeMatrix4X3(std::initializer_list<VeVector4> kInitList) noexcept
	{
		VE_MATRIX4X3 mat = VeLoadFloat4x3A((const VE_FLOAT4X3A*)kInitList.begin());
		VeStoreFloat4x3A(&m, mat);
	}

	inline VeFloat32& operator () (VeUInt32 u32Col, VeUInt32 u32Row) noexcept;

	inline const VeFloat32& operator () (VeUInt32 u32Col, VeUInt32 u32Row) const noexcept;

	inline VeVector4& operator [] (VeUInt32 i) noexcept;

	inline const VeVector4& operator [] (VeUInt32 i) const noexcept;

	inline VeMatrix4X3& operator = (const VeMatrix4X3& kMatrix) noexcept;

	inline VeMatrix4X3& operator += (const VeMatrix4X3& kMatrix) noexcept;

	inline VeMatrix4X3& operator -= (const VeMatrix4X3& kMatrix) noexcept;

	inline VeMatrix4X3& operator *= (const VeMatrix4X3& kMatrix) noexcept;

	inline VeMatrix4X3& operator *= (VeFloat32 f32Scale) noexcept;

	inline VeMatrix4X3& operator /= (VeFloat32 f32Scale) noexcept;	

	inline VeMatrix4X3 operator + () const noexcept;

	inline VeMatrix4X3 operator - () const noexcept;

	inline VeMatrix4X3 operator + (const VeMatrix4X3& kMatrix) const noexcept;

	inline VeMatrix4X3 operator - (const VeMatrix4X3& kMatrix) const noexcept;

	inline VeMatrix4X3 operator * (const VeMatrix4X3& kMatrix) const noexcept;

	inline VeVector4 operator * (const VeVector4& kVector) const noexcept;

	inline VeMatrix4X3 operator * (VeFloat32 f32Scale) const noexcept;

	inline VeMatrix4X3 operator / (VeFloat32 f32Scale) const noexcept;

	friend inline VeMatrix4X3 operator * (VeFloat32 f32Scale, const VeMatrix4X3& kMatrix) noexcept;

	friend inline VeVector4 operator * (const VeVector4& kVector, const VeMatrix4X3& kMatrix) noexcept;

	inline bool operator == (const VeMatrix4X3& kMatrix) const noexcept;

	inline bool operator != (const VeMatrix4X3& kMatrix) const noexcept;

	inline VeVector4& Col(VeUInt32 i) noexcept;

	inline const VeVector4& Col(VeUInt32 i) const noexcept;

	inline void Col(VeUInt32 i, const VeVector4& kVec) noexcept;

	inline VeVector4 Row(VeUInt32 i) const noexcept;

	inline void Row(VeUInt32 i, const VeVector4& kVec) noexcept;

	inline void SetZero() noexcept;

	inline void SetIdentity() noexcept;

	inline void SetScale(VeFloat32 x, VeFloat32 y, VeFloat32 z) noexcept;

	inline void SetRotateX(VeFloat32 f32Angle) noexcept;

	inline void SetRotateY(VeFloat32 f32Angle) noexcept;

	inline void SetRotateZ(VeFloat32 f32Angle) noexcept;

	inline void SetRotate(const VeQuaternion& kQuat) noexcept;

	inline void SetRotate(VeFloat32 f32Pitch, VeFloat32 f32Yaw, VeFloat32 f32Roll) noexcept;

	inline void SetRotate(const VeVector3& kEulerianAngles) noexcept;

	inline void SetTranslation(VeFloat32 x, VeFloat32 y, VeFloat32 z) noexcept;

	inline void Mul(const VeMatrix4X3& kMat0, const VeMatrix4X3& kMat1) noexcept;

	inline void PreMul(const VeMatrix4X3& kMat) noexcept;

	inline void PostMul(const VeMatrix4X3& kMat) noexcept;

	inline VeMatrix4X3 GetInverse() const noexcept;

	inline void Inverse() noexcept;

	inline void Inverse(const VeMatrix4X3& kMat) noexcept;

	inline VeVector4 ApplyVector(const VeVector4& kVector) const noexcept;

	inline void ApplyVector(VeVector4& kOut, const VeVector4& kIn) const noexcept;

};

class VE_MAIN_API alignas(16) VeMatrix : public VeMemA16Object
{
public:
	union
	{
		struct
		{
			VeFloat32 _11, _21, _31, _41;
			VeFloat32 _12, _22, _32, _42;
			VeFloat32 _13, _23, _33, _43;
			VeFloat32 _14, _24, _34, _44;
		};
		VeFloat32 f[4][4];
		VE_FLOAT4A v[4];
		VeVector4 vec[4];
		VE_FLOAT4X4A m;
	};

	VeMatrix() noexcept
	{
		SetIdentity();
	}

	VeMatrix(const VE_FLOAT4X4& kMatrix) noexcept
	{
		VE_MATRIX mat = VeLoadFloat4x4(&kMatrix);
		VeStoreFloat4x4A(&m, mat);
	}

	VeMatrix(const VE_FLOAT4X4A& kMatrix) noexcept
	{
		VE_MATRIX mat = VeLoadFloat4x4A(&kMatrix);
		VeStoreFloat4x4A(&m, mat);
	}

	VeMatrix(const VeMatrix& kMatrix) noexcept
	{
		VE_MATRIX mat = VeLoadFloat4x4A(&kMatrix.m);
		VeStoreFloat4x4A(&m, mat);
	}

	VeMatrix(
		VeFloat32 _11, VeFloat32 _21, VeFloat32 _31, VeFloat32 _41,
		VeFloat32 _12, VeFloat32 _22, VeFloat32 _32, VeFloat32 _42,
		VeFloat32 _13, VeFloat32 _23, VeFloat32 _33, VeFloat32 _43,
		VeFloat32 _14, VeFloat32 _24, VeFloat32 _34, VeFloat32 _44) noexcept
	{
		VE_MATRIX mat = VeMatrixSet(
			_11, _21, _31, _41,
			_12, _22, _32, _42,
			_13, _23, _33, _43,
			_14, _24, _34, _44);
		VeStoreFloat4x4A(&m, mat);
	}

	VeMatrix(std::initializer_list<VeFloat32> kInitList) noexcept
	{
		VE_MATRIX mat = VeLoadFloat4x4((const VE_FLOAT4X4*)kInitList.begin());
		VeStoreFloat4x4A(&m, mat);
	}

	VeMatrix(std::initializer_list<VeVector4> kInitList) noexcept
	{
		VE_MATRIX mat = VeLoadFloat4x4A((const VE_FLOAT4X4A*)kInitList.begin());
		VeStoreFloat4x4A(&m, mat);
	}

	inline VeFloat32& operator () (VeUInt32 u32Col, VeUInt32 u32Row) noexcept;

	inline const VeFloat32& operator () (VeUInt32 u32Col, VeUInt32 u32Row) const noexcept;

	inline VeVector4& operator [] (VeUInt32 i) noexcept;

	inline const VeVector4& operator [] (VeUInt32 i) const noexcept;

	inline VeMatrix& operator = (const VeMatrix& kMatrix) noexcept;

	inline VeMatrix& operator += (const VeMatrix& kMatrix) noexcept;

	inline VeMatrix& operator -= (const VeMatrix& kMatrix) noexcept;

	inline VeMatrix& operator *= (const VeMatrix& kMatrix) noexcept;

	inline VeMatrix& operator *= (VeFloat32 f32Scale) noexcept;

	inline VeMatrix& operator /= (VeFloat32 f32Scale) noexcept;

	inline VeMatrix operator + () const noexcept;

	inline VeMatrix operator - () const noexcept;

	inline VeMatrix operator + (const VeMatrix& kMatrix) const noexcept;

	inline VeMatrix operator - (const VeMatrix& kMatrix) const noexcept;

	inline VeMatrix operator * (const VeMatrix& kMatrix) const noexcept;

	inline VeVector4 operator * (const VeVector4& kVector) const noexcept;

	inline VeMatrix operator * (VeFloat32 f32Scale) const noexcept;

	inline VeMatrix operator / (VeFloat32 f32Scale) const noexcept;

	friend inline VeMatrix operator * (VeFloat32 f32Scale, const VeMatrix& kMatrix) noexcept;

	friend inline VeVector4 operator * (const VeVector4& kVector, const VeMatrix& kMatrix) noexcept;

	inline bool operator == (const VeMatrix& kMatrix) const noexcept;

	inline bool operator != (const VeMatrix& kMatrix) const noexcept;

	inline VeVector4& Col(VeUInt32 i) noexcept;

	inline const VeVector4& Col(VeUInt32 i) const noexcept;

	inline void Col(VeUInt32 i, const VeVector4& kVec) noexcept;

	inline VeVector4 Row(VeUInt32 i) const noexcept;

	inline void Row(VeUInt32 i, const VeVector4& kVec) noexcept;

	inline void SetZero() noexcept;

	inline void SetIdentity() noexcept;

	inline void SetScale(VeFloat32 x, VeFloat32 y, VeFloat32 z) noexcept;

	inline void SetRotateX(VeFloat32 f32Angle) noexcept;

	inline void SetRotateY(VeFloat32 f32Angle) noexcept;

	inline void SetRotateZ(VeFloat32 f32Angle) noexcept;

	inline void SetRotate(const VeQuaternion& kQuat) noexcept;

	inline void SetRotate(VeFloat32 f32Pitch, VeFloat32 f32Yaw, VeFloat32 f32Roll) noexcept;

	inline void SetRotate(const VeVector3& kEulerianAngles) noexcept;

	inline void SetTranslation(VeFloat32 x, VeFloat32 y, VeFloat32 z) noexcept;

	inline void Mul(const VeMatrix& kMat0, const VeMatrix& kMat1) noexcept;

	inline void PreMul(const VeMatrix& kMat) noexcept;

	inline void PostMul(const VeMatrix& kMat) noexcept;

	inline VeVector4 ApplyVector(const VeVector4& kVector) const noexcept;

	inline void ApplyVector(VeVector4& kOut, const VeVector4& kIn) const noexcept;

};

#include "VeMatrix.inl"
