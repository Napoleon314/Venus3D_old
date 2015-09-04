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
inline VeFloat32& VeVector4::operator [] (Coord eCoord) noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector4::operator [] (Coord eCoord) const noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector4::operator () (Coord eCoord) noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector4::operator () (Coord eCoord) const noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeVector4& VeVector4::operator = (const VeVector4& kVector) noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(&kVector.v);
	VeStoreFloat4A(&v, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector4& VeVector4::operator += (const VeVector4& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kVector.v);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat4A(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector4& VeVector4::operator -= (const VeVector4& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kVector.v);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat4A(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector4& VeVector4::operator *= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(&v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat4A(&v, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector4& VeVector4::operator /= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat4A(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator + () const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(&v);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator - () const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(&v);
	vec = VeVectorNegate(vec);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator + (const VeVector4& kVector) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kVector.v);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat4A(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator - (const VeVector4& kVector) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kVector.v);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat4A(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator * (VeFloat32 f32Scale) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(&v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::operator / (VeFloat32 f32Scale) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat4A(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 operator * (VeFloat32 f32Scale,
	const VeVector4& kVector) noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(&kVector.v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeVector4::operator == (const VeVector4& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kVector.v);
	return VeVector4Equal(vec0, vec1);
}
//--------------------------------------------------------------------------
inline bool VeVector4::operator != (const VeVector4& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kVector.v);
	return VeVector4NotEqual(vec0, vec1);
}
//--------------------------------------------------------------------------
inline void VeVector4::SetZero() noexcept
{
	VE_VECTOR vec = VeVectorZero();
	VeStoreFloat4A(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeVector4::Set(VeFloat32 f32X, VeFloat32 f32Y,
	VeFloat32 f32Z, VeFloat32 f32W) noexcept
{
	VE_VECTOR vec = VeVectorSet(f32X, f32Y, f32Z, f32W);
	VeStoreFloat4A(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeVector4::Scale(const VeVector4& kVector,
	VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(&kVector.v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat4A(&v, vec);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector4::Length() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat4A(&v);
	vec = VeVector4Length(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector4::LengthSquared() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat4A(&v);
	vec = VeVector4LengthSq(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector4::Normalise() noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(&v);
	vec = VeVector4Normalize(vec);
	VeStoreFloat4A(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeVector4::ParaMul(const VeVector4& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kVector.v);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat4A(&v, vec0);
}
//--------------------------------------------------------------------------
inline void VeVector4::ParaMul(const VeVector4& kVector0,
	const VeVector4& kVector1) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&kVector0.v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kVector1.v);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat4A(&v, vec0);
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
	VE_VECTOR vec = VeLoadFloat4A(&v);
	VE_VECTOR min = VeLoadFloat4A(&kLower.v);
	VE_VECTOR max = VeLoadFloat4A(&kUpper.v);
	vec = VeVectorClamp(vec, min, max);
	VeStoreFloat4A(&v, vec);
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::UnitVector() const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(&v);
	vec = VeVector4Normalize(vec);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector4::Dot(const VeVector4& kVector) const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kVector.v);
	vec0 = VeVector4Dot(vec0, vec1);
	VeStoreFloat(&f32Res, vec0);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector4::Cross(const VeVector4& kVector0,
	const VeVector4& kVector1) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&kVector0.v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kVector1.v);
	vec0 = VeVector3Cross(vec0, vec1);
	VeStoreFloat4A(&v, vec0);
}
//--------------------------------------------------------------------------
inline VeVector4 VeVector4::Cross(const VeVector4& kVector) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kVector.v);
	vec0 = VeVector3Cross(vec0, vec1);
	VeStoreFloat4A(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
