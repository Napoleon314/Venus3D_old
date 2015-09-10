////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeVector4.h
//  Created:     2015/08/18 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API alignas(16) VeVector4 : public VeMemObject
{
public:
	union
	{
		struct
		{
			VeFloat32 x, y, z, w;
		};
		VeFloat32 f[4];
		VE_FLOAT2A v2;
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

	VeVector4() noexcept
	{
		SetZero();
	}

	VeVector4(VeFloat32 f32X, VeFloat32 f32Y, VeFloat32 f32Z, VeFloat32 f32W) noexcept
	{
		Set(f32X, f32Y, f32Z, f32W);
	}

	VeVector4(const VE_FLOAT4& kVector) noexcept
	{
		VE_VECTOR vec = VeLoadFloat4(&kVector);
		VeStoreFloat4A(&v, vec);
	}

	VeVector4(const VE_FLOAT4A& kVector) noexcept
	{
		VE_VECTOR vec = VeLoadFloat4A(&kVector);
		VeStoreFloat4A(&v, vec);
	}

	VeVector4(const VeVector4& kVector) noexcept
	{
		VE_VECTOR vec = VeLoadFloat4A(&kVector.v);
		VeStoreFloat4A(&v, vec);
	}

	VeVector4(const VeVector3& kVector, VeFloat32 f32W) noexcept
	{
		Set(kVector.x, kVector.y, kVector.z, f32W);
	}

	VeVector4(std::initializer_list<VeFloat32> kInitList) noexcept
	{
		VE_VECTOR vec = VeLoadFloat4((const VE_FLOAT4*)kInitList.begin());
		VeStoreFloat4A(&v, vec);
	}

	inline VeFloat32& operator [] (Coord eCoord) noexcept;

	inline VeFloat32 operator [] (Coord eCoord) const noexcept;

	inline VeFloat32& operator () (Coord eCoord) noexcept;

	inline VeFloat32 operator () (Coord eCoord) const noexcept;

	inline VeVector4& operator = (const VeVector4& kVector) noexcept;

	inline VeVector4& operator += (const VeVector4& kVector) noexcept;

	inline VeVector4& operator -= (const VeVector4& kVector) noexcept;

	inline VeVector4& operator *= (VeFloat32 f32Scale) noexcept;

	inline VeVector4& operator /= (VeFloat32 f32Scale) noexcept;

	inline VeVector4 operator + () const noexcept;

	inline VeVector4 operator - () const noexcept;

	inline VeVector4 operator + (const VeVector4& kVector) const noexcept;

	inline VeVector4 operator - (const VeVector4& kVector) const noexcept;

	inline VeVector4 operator * (VeFloat32 f32Scale) const noexcept;

	inline VeVector4 operator / (VeFloat32 f32Scale) const noexcept;

	friend inline VeVector4 operator * (VeFloat32 f32Scale, const VeVector4& kVector) noexcept;

	inline bool operator == (const VeVector4& kVector) const noexcept;

	inline bool operator != (const VeVector4& kVector) const noexcept;

	inline void SetZero() noexcept;

	inline void Set(VeFloat32 f32X, VeFloat32 f32Y, VeFloat32 f32Z, VeFloat32 f32W) noexcept;

	inline void Scale(const VeVector4& kVector, VeFloat32 f32Scale) noexcept;

	inline VeFloat32 Length() const noexcept;

	inline VeFloat32 LengthSquared() const noexcept;

	inline void Normalise() noexcept;

	inline void ParaMul(const VeVector4& kVector) noexcept;

	inline void ParaMul(const VeVector4& kVector0, const VeVector4& kVector1) noexcept;

	inline void Lerp(const VeVector4& kVector0, const VeVector4& kVector1, VeFloat32 f32Alpha) noexcept;

	inline void Clamp(const VeVector4& kLower, const VeVector4& kUpper) noexcept;

	inline VeVector4 UnitVector() const noexcept;

	inline VeFloat32 Dot(const VeVector4& kVector) const noexcept;

	inline void Cross(const VeVector4& kVector0, const VeVector4& kVector1) noexcept;

	inline VeVector4 Cross(const VeVector4& kVector) const noexcept;	

	static const VeVector4 ZERO;

	static const VeVector4 UNIT_X;

	static const VeVector4 UNIT_Y;

	static const VeVector4 UNIT_Z;

	static const VeVector4 ZERO_POINT;

	static const VeVector4 UNIT_X_POINT;

	static const VeVector4 UNIT_Y_POINT;

	static const VeVector4 UNIT_Z_POINT;
};

#include "VeVector4.inl"
