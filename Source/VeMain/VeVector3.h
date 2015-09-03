////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeVector3.h
//  Created:     2015/08/18 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeVector3 : public VeMemObject
{
public:
	VeFloat32 x, y, z;

	enum Coord
	{
		X = 0,
		Y = 1,
		Z = 2
	};

	VeVector3() noexcept
	{
		SetZero();
	}

	VeVector3(VeFloat32 f32X, VeFloat32 f32Y, VeFloat32 f32Z) noexcept
	{
		Set(f32X, f32Y, f32Z);
	}

	VeVector3(const VeFloat32* pf32Vector) noexcept
	{
		VE_VECTOR vec = VeLoadFloat3((const VE_FLOAT3*)pf32Vector);
		VeStoreFloat3(*this, vec);
	}

	VeVector3(const VeVector3& kVector) noexcept
	{
		VE_VECTOR vec = VeLoadFloat3(kVector);
		VeStoreFloat3(*this, vec);
	}

	VeVector3(std::initializer_list<VeFloat32> kInitList) noexcept
	{
		VE_VECTOR vec = VeLoadFloat3((const VE_FLOAT3*)kInitList.begin());
		VeStoreFloat3(*this, vec);
	}

	inline operator VE_FLOAT3* () noexcept;

	inline operator const VE_FLOAT3* () const noexcept;

	inline VeFloat32& operator [] (Coord eCoord) noexcept;

	inline VeFloat32 operator [] (Coord eCoord) const noexcept;

	inline VeFloat32& operator () (Coord eCoord) noexcept;

	inline VeFloat32 operator () (Coord eCoord) const noexcept;

	inline VeVector3& operator = (const VeVector3& kVector) noexcept;

	inline VeVector3& operator += (const VeVector3& kVector) noexcept;

	inline VeVector3& operator -= (const VeVector3& kVector) noexcept;

	inline VeVector3& operator *= (VeFloat32 f32Scale) noexcept;

	inline VeVector3& operator /= (VeFloat32 f32Scale) noexcept;

	inline VeVector3 operator + () const noexcept;

	inline VeVector3 operator - () const noexcept;

	inline VeVector3 operator + (const VeVector3& kVector) const noexcept;

	inline VeVector3 operator - (const VeVector3& kVector) const noexcept;

	inline VeVector3 operator * (VeFloat32 f32Scale) const noexcept;

	inline VeVector3 operator / (VeFloat32 f32Scale) const noexcept;

	friend inline VeVector3 operator * (VeFloat32 f32Scale, const VeVector3& kVector) noexcept;

	inline bool operator == (const VeVector3& kVector) const noexcept;

	inline bool operator != (const VeVector3& kVector) const noexcept;

	inline void SetZero() noexcept;

	inline void Set(VeFloat32 f32X, VeFloat32 f32Y,	VeFloat32 f32Z) noexcept;

	inline void Scale(const VeVector3& kVector, VeFloat32 f32Scale) noexcept;

	inline VeFloat32 Length() const noexcept;

	inline VeFloat32 LengthSquared() const noexcept;

	inline void Normalise() noexcept;

	inline void ParaMul(const VeVector3& kVector) noexcept;

	inline void ParaMul(const VeVector3& kVector0, const VeVector3& kVector1) noexcept;

	inline void Lerp(const VeVector3& kVector0, const VeVector3& kVector1, VeFloat32 f32Alpha) noexcept;

	inline void Clamp(const VeVector3& kLower, const VeVector3& kUpper) noexcept;

	inline VeVector3 UnitVector() const noexcept;

	inline void SetPitchYaw(VeFloat32 f32PitchInRadians, VeFloat32 f32YawInRadians) noexcept;

	inline VeFloat32 GetYaw() const noexcept;

	inline VeFloat32 GetPitch() const noexcept;

	inline VeFloat32 Dot(const VeVector3& kVector) const noexcept;

	inline void Cross(const VeVector3& kVector0, const VeVector3& kVector1) noexcept;

	inline VeVector3 Cross(const VeVector3& kVector) const noexcept;

	inline void ProjectOnto(const VeVector3& kVector0, const VeVector3& kVector1) noexcept;

	inline VeVector3 ProjectOnto(const VeVector3& kVector) const noexcept;

	static const VeVector3 ZERO;

	static const VeVector3 UNIT_X;

	static const VeVector3 UNIT_Y;

	static const VeVector3 UNIT_Z;

};

inline bool VeAlmostEqual(const VeVector3& kVector0, const VeVector3& kVector1, const VeFloat32 f32Epsilon = VE_MATH_EPSILON_F) noexcept;

inline VeFloat32 VeDistance(const VeVector3& kVector0, const VeVector3& kVector1) noexcept;

inline VeFloat32 VeDistanceSquared(const VeVector3& kVector0, const VeVector3& kVector1) noexcept;

#include "VeVector3.inl"
