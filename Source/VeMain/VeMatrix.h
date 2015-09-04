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
		VeFloat32 m[3][4];
		VE_FLOAT4A v[3];
	};

	VeMatrix4X3() noexcept
	{
		SetIdentity();
	}

	VeMatrix4X3(const VeFloat32* pf32Matrix) noexcept
	{
		VE_MATRIX4X3 mat = VeLoadFloat4x3((const VE_FLOAT4X3*)pf32Matrix);
		VeStoreFloat4x3A(*this, mat);
	}

	VeMatrix4X3(const VeMatrix4X3& kMatrix) noexcept
	{
		VE_MATRIX4X3 mat = VeLoadFloat4x3A(kMatrix);
		VeStoreFloat4x3A(*this, mat);
	}

	VeMatrix4X3(
		VeFloat32 _11, VeFloat32 _21, VeFloat32 _31, VeFloat32 _41,
		VeFloat32 _12, VeFloat32 _22, VeFloat32 _32, VeFloat32 _42,
		VeFloat32 _13, VeFloat32 _23, VeFloat32 _33, VeFloat32 _43) noexcept
	{
		VE_MATRIX4X3 mat;
		mat.r[0] = VeVectorSet(_11, _21, _31, _41);
		mat.r[1] = VeVectorSet(_12, _22, _32, _42);
		mat.r[2] = VeVectorSet(_13, _23, _33, _43);
		VeStoreFloat4x3A(*this, mat);
	}

	VeMatrix4X3(std::initializer_list<VeFloat32> kInitList) noexcept
	{
		VE_MATRIX4X3 mat = VeLoadFloat4x3((const VE_FLOAT4X3*)kInitList.begin());
		VeStoreFloat4x3A(*this, mat);
	}

	VeMatrix4X3(std::initializer_list<VeVector4> kInitList) noexcept
	{
		VE_MATRIX4X3 mat = VeLoadFloat4x3A((const VE_FLOAT4X3A*)kInitList.begin());
		VeStoreFloat4x3A(*this, mat);
	}

	inline operator VE_FLOAT4X3A* () noexcept;

	inline operator const VE_FLOAT4X3A* () const noexcept;

	inline VeVector4& Row(VeUInt32 i) noexcept;

	inline const VeVector4& Row(VeUInt32 i) const noexcept;

	inline void Row(VeUInt32 i, const VeVector4& kVec) noexcept;

	inline VeVector4 Col(VeUInt32 i) const noexcept;

	inline void Col(VeUInt32 i, const VeVector4& kVec) noexcept;

	inline void SetZero() noexcept;

	inline void SetIdentity() noexcept;

};

#include "VeMatrix.inl"
