////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeVector4.inl
//  Created:     2015/08/18 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeVector4::operator VE_FLOAT4A* () noexcept
{
	return (VE_FLOAT4A*)this;
}
//--------------------------------------------------------------------------
inline VeVector4::operator const VE_FLOAT4A* () const noexcept
{
	return (const VE_FLOAT4A*)this;
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector4::operator [] (Coord eCoord) noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector4::operator [] (Coord eCoord) const noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector4::operator () (Coord eCoord) noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector4::operator () (Coord eCoord) const noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeVector4& VeVector4::operator = (const VeVector4& kVector) noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(kVector);
	VeStoreFloat4A(*this, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector4& VeVector4::operator += (const VeVector4& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(*this);
	VE_VECTOR vec1 = VeLoadFloat4A(kVector);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat4A(*this, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector4& VeVector4::operator -= (const VeVector4& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(*this);
	VE_VECTOR vec1 = VeLoadFloat4A(kVector);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat4A(*this, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector4& VeVector4::operator *= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(*this);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat4A(*this, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector4& VeVector4::operator /= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(*this);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat4A(*this, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator + () const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(*this);
	VeStoreFloat4A(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator - () const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(*this);
	vec = VeVectorNegate(vec);
	VeStoreFloat4A(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator + (const VeVector4& kVector) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(*this);
	VE_VECTOR vec1 = VeLoadFloat4A(kVector);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat4A(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator - (const VeVector4& kVector) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(*this);
	VE_VECTOR vec1 = VeLoadFloat4A(kVector);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat4A(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator * (VeFloat32 f32Scale) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(*this);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat4A(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator / (VeFloat32 f32Scale) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(*this);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat4A(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 operator * (VeFloat32 f32Scale,
	const VeVector4& kVector) noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(kVector);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat4A(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeVector4::operator == (const VeVector4& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(*this);
	VE_VECTOR vec1 = VeLoadFloat4A(kVector);
	return VeVector4Equal(vec0, vec1);
}
//--------------------------------------------------------------------------
inline bool VeVector4::operator != (const VeVector4& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(*this);
	VE_VECTOR vec1 = VeLoadFloat4A(kVector);
	return VeVector4NotEqual(vec0, vec1);
}
//--------------------------------------------------------------------------
inline void VeVector4::SetZero() noexcept
{
	VE_VECTOR vec = VeVectorZero();
	VeStoreFloat4A(*this, vec);
}
//--------------------------------------------------------------------------
inline void VeVector4::Set(VeFloat32 f32X, VeFloat32 f32Y,
	VeFloat32 f32Z, VeFloat32 f32W) noexcept
{
	VE_VECTOR vec = VeVectorSet(f32X, f32Y, f32Z, f32W);
	VeStoreFloat4A(*this, vec);
}
//--------------------------------------------------------------------------
inline void VeVector4::Scale(const VeVector4& kVector,
	VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(kVector);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat4A(*this, vec);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector4::Length() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat4A(*this);
	vec = VeVector4Length(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector4::LengthSquared() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat4A(*this);
	vec = VeVector4LengthSq(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector4::Normalise() noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(*this);
	vec = VeVector4Normalize(vec);
	VeStoreFloat4A(*this, vec);
}
//--------------------------------------------------------------------------
inline void VeVector4::ParaMul(const VeVector4& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(*this);
	VE_VECTOR vec1 = VeLoadFloat4A(kVector);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat4A(*this, vec0);
}
//--------------------------------------------------------------------------
inline void VeVector4::ParaMul(const VeVector4& kVector0,
	const VeVector4& kVector1) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(kVector0);
	VE_VECTOR vec1 = VeLoadFloat4A(kVector1);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat4A(*this, vec0);
}
//--------------------------------------------------------------------------
inline void VeVector4::Lerp(const VeVector4& kVector0,
	const VeVector4& kVector1, VeFloat32 f32Alpha) noexcept
{
	*this = VeLerp(f32Alpha, kVector0, kVector1);
}
//--------------------------------------------------------------------------
inline void VeVector4::Clamp(const VeVector4& kLower,
	const VeVector4& kUpper) noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(*this);
	VE_VECTOR min = VeLoadFloat4A(kLower);
	VE_VECTOR max = VeLoadFloat4A(kUpper);
	vec = VeVectorClamp(vec, min, max);
	VeStoreFloat4A(*this, vec);
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::UnitVector() const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(*this);
	vec = VeVector4Normalize(vec);
	VeStoreFloat4A(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector4::Dot(const VeVector4& kVector) const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec0 = VeLoadFloat4A(*this);
	VE_VECTOR vec1 = VeLoadFloat4A(kVector);
	vec0 = VeVector4Dot(vec0, vec1);
	VeStoreFloat(&f32Res, vec0);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector4::Cross(const VeVector4& kVector0,
	const VeVector4& kVector1) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(kVector0);
	VE_VECTOR vec1 = VeLoadFloat4A(kVector1);
	vec0 = VeVector3Cross(vec0, vec1);
	VeStoreFloat4A(*this, vec0);
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::Cross(const VeVector4& kVector) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(*this);
	VE_VECTOR vec1 = VeLoadFloat4A(kVector);
	vec0 = VeVector3Cross(vec0, vec1);
	VeStoreFloat4A(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
