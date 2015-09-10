////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeQuaternion.h
//  Created:     2015/09/04 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API alignas(16) VeQuaternion : public VeMemObject
{
public:
	union
	{
		struct
		{
			VeFloat32 x, y, z, w;
		};
		VeFloat32 f[4];
		VE_FLOAT3A v3;
		VE_FLOAT4A v;
	};

	enum Coord
	{
		X = 0,
		Y = 1,
		Z = 2,
		W = 3
	};

	VeQuaternion() noexcept
	{
		SetZero();
	}

	VeQuaternion(VeFloat32 f32X, VeFloat32 f32Y, VeFloat32 f32Z, VeFloat32 f32W) noexcept
	{
		Set(f32X, f32Y, f32Z, f32W);
	}

	VeQuaternion(const VeFloat32* pf32Quat) noexcept
	{
		VE_VECTOR vec = VeLoadFloat4((const VE_FLOAT4*)pf32Quat);
		VeStoreFloat4A(&v, vec);
	}

	VeQuaternion(const VeQuaternion& kQuat) noexcept
	{
		VE_VECTOR vec = VeLoadFloat4A(&kQuat.v);
		VeStoreFloat4A(&v, vec);
	}

	VeQuaternion(const VeVector3& kVec, VeFloat32 f32Angle) noexcept
	{
		FromAngleAxis(kVec, f32Angle);
	}

	VeQuaternion(std::initializer_list<VeFloat32> kInitList) noexcept
	{
		VE_VECTOR vec = VeLoadFloat4((const VE_FLOAT4*)kInitList.begin());
		VeStoreFloat4A(&v, vec);
	}

	inline VeQuaternion& operator = (const VeQuaternion& kQuat) noexcept;

	inline VeQuaternion& operator += (const VeQuaternion& kQuat) noexcept;

	inline VeQuaternion& operator -= (const VeQuaternion& kQuat) noexcept;

	inline VeQuaternion& operator *= (const VeQuaternion& kQuat) noexcept;

	inline VeQuaternion& operator *= (VeFloat32 f32Scale) noexcept;

	inline VeQuaternion& operator /= (VeFloat32 f32Scale) noexcept;

	inline VeQuaternion operator + () const noexcept;

	inline VeQuaternion operator - () const noexcept;

	inline VeQuaternion operator + (const VeQuaternion& kQuat) const noexcept;

	inline VeQuaternion operator - (const VeQuaternion& kQuat) const noexcept;

	inline VeQuaternion operator * (const VeQuaternion& kQuat) const noexcept;

	inline VeQuaternion operator * (VeFloat32 f32Scale) const noexcept;

	inline VeQuaternion operator / (VeFloat32 f32Scale) const noexcept;
	
	friend inline VeQuaternion operator * (VeFloat32 f32Scale, const VeQuaternion& kQuat) noexcept;

	inline VeVector3 operator * (const VeVector3& kVec) const noexcept;

	inline bool operator == (const VeQuaternion& kQuat) const;

	inline bool operator != (const VeQuaternion& kQuat) const;

	inline void SetZero() noexcept;

	inline void Set(VeFloat32 f32X, VeFloat32 f32Y, VeFloat32 f32Z, VeFloat32 f32W) noexcept;

	inline void FromAngleAxis(const VeVector3& kAxis, VeFloat32 f32Angle) noexcept;

	//inline void FromMatrix(const VeMatrix& kMat) noexcept;

	inline void Normalise() noexcept;

	inline void Invert() noexcept;

	inline void Minimise() noexcept;

	inline void Slerp(const VeQuaternion& kStart, const VeQuaternion &kEnd, VeFloat32 t) noexcept;

	inline void Mul(const VeQuaternion& kQuat0, const VeQuaternion& kQuat1) noexcept;

	inline void PreMul(const VeQuaternion& kQuat) noexcept;

	inline void PostMul(const VeQuaternion& kQuat) noexcept;

	inline VeFloat32 Dot(const VeQuaternion& kQuat) const noexcept;

	inline VeFloat32 Length() const noexcept;

	inline VeFloat32 LengthSquared() const noexcept;

	static const VeQuaternion IDENTITY;

};

#include "VeQuaternion.inl"
