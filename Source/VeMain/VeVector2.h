////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeVector2.h
//  Created:     2015/08/18 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeVector2 : public VeMemObject
{
public:
	VeFloat32 x, y;

	enum Coord
	{
		X = 0,
		Y = 1
	};

	VeVector2() noexcept
	{
		SetZero();
	}

	VeVector2(VeFloat32 f32X, VeFloat32 f32Y) noexcept
	{
		Set(f32X, f32Y);
	}

	VeVector2(const VeFloat32* pf32Vector) noexcept
	{
		VE_VECTOR vec = VeLoadFloat2((const VE_FLOAT2*)pf32Vector);
		VeStoreFloat2(*this, vec);
	}

	VeVector2(const VeVector2& kVector) noexcept
	{
		VE_VECTOR vec = VeLoadFloat2(kVector);
		VeStoreFloat2(*this, vec);
	}

	VeVector2(std::initializer_list<VeFloat32> kInitList) noexcept
	{
		VE_VECTOR vec = VeLoadFloat2((const VE_FLOAT2*)kInitList.begin());
		VeStoreFloat2(*this, vec);
	}

	inline operator VE_FLOAT2* () noexcept;

	inline operator const VE_FLOAT2* () const noexcept;

	inline VeFloat32& operator [] (Coord eCoord) noexcept;

	inline VeFloat32 operator [] (Coord eCoord) const noexcept;

	inline VeFloat32& operator () (Coord eCoord) noexcept;

	inline VeFloat32 operator () (Coord eCoord) const noexcept;

	inline VeVector2& operator = (const VeVector2& kVector) noexcept;

	inline VeVector2& operator += (const VeVector2& kVector) noexcept;

	inline VeVector2& operator -= (const VeVector2& kVector) noexcept;

	inline VeVector2& operator *= (VeFloat32 f32Scale) noexcept;

	inline VeVector2& operator /= (VeFloat32 f32Scale) noexcept;

	inline VeVector2 operator + () const noexcept;

	inline VeVector2 operator - () const noexcept;

	inline VeVector2 operator + (const VeVector2& kVector) const noexcept;

	inline VeVector2 operator - (const VeVector2& kVector) const noexcept;

	inline VeVector2 operator * (VeFloat32 f32Scale) const noexcept;

	inline VeVector2 operator / (VeFloat32 f32Scale) const noexcept;

	friend inline VeVector2 operator * (VeFloat32 f32Scale, const VeVector2& kVector) noexcept;

	inline bool operator == (const VeVector2& kVector) const noexcept;

	inline bool operator != (const VeVector2& kVector) const noexcept;

	inline void SetZero() noexcept;

	inline void Set(VeFloat32 f32X, VeFloat32 f32Y) noexcept;

	inline void Scale(const VeVector2& kVector, VeFloat32 f32Scale) noexcept;

	inline VeFloat32 Length() const noexcept;

	inline VeFloat32 LengthSquared() const noexcept;

	inline void Normalise() noexcept;

	inline void ParaMul(const VeVector2& kVector) noexcept;

	inline void ParaMul(const VeVector2& kVector0, const VeVector2& kVector1) noexcept;

	inline void Lerp(const VeVector2& kVector0, const VeVector2& kVector1, VeFloat32 f32Alpha) noexcept;

	inline void Clamp(const VeVector2& kLower, const VeVector2& kUpper) noexcept;

	inline VeVector2 UnitVector() const noexcept;

	inline VeFloat32 Dot(const VeVector2& kVector) const noexcept;

	inline VeFloat32 Cross(const VeVector2& kVector) const noexcept;

	inline void ProjectOnto(const VeVector2& kVector0, const VeVector2& kVector1) noexcept;

	inline VeVector2 ProjectOnto(const VeVector2& kVector) const noexcept;

	static const VeVector2 ZERO;

	static const VeVector2 UNIT_X;

	static const VeVector2 UNIT_Y;

};

inline bool VeAlmostEqual(const VeVector2& kVector0, const VeVector2& kVector1, const VeFloat32 f32Epsilon = VE_MATH_EPSILON_F) noexcept;

inline VeFloat32 VeDistance(const VeVector2& kVector0, const VeVector2& kVector1) noexcept;

inline VeFloat32 VeDistanceSquared(const VeVector2& kVector0, const VeVector2& kVector1) noexcept;

#include "VeVector2.inl"
