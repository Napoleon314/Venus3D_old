////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeVector2.inl
//  Created:     2015/08/18 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeFloat32& VeVector2::operator [] (Coord eCoord) noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::operator [] (Coord eCoord) const noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector2::operator () (Coord eCoord) noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::operator () (Coord eCoord) const noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeVector2& VeVector2::operator = (const VeVector2& kVector) noexcept
{
	VE_VECTOR vec = VeLoadFloat2(&kVector.v);
	VeStoreFloat2(&v, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2& VeVector2::operator += (const VeVector2& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2(&v);
	VE_VECTOR vec1 = VeLoadFloat2(&kVector.v);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat2(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2& VeVector2::operator -= (const VeVector2& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2(&v);
	VE_VECTOR vec1 = VeLoadFloat2(&kVector.v);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat2(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2& VeVector2::operator *= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat2(&v);	
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat2(&v, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2& VeVector2::operator /= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2(&v);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat2(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator + () const noexcept
{
	VeVector2 kRes;
	VE_VECTOR vec = VeLoadFloat2(&v);
	VeStoreFloat2(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator - () const noexcept
{
	VeVector2 kRes;
	VE_VECTOR vec = VeLoadFloat2(&v);
	vec = VeVectorNegate(vec);
	VeStoreFloat2(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator + (
	const VeVector2& kVector) const noexcept
{
	VeVector2 kRes;
	VE_VECTOR vec0 = VeLoadFloat2(&v);
	VE_VECTOR vec1 = VeLoadFloat2(&kVector.v);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat2(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator - (
	const VeVector2& kVector) const noexcept
{
	VeVector2 kRes;
	VE_VECTOR vec0 = VeLoadFloat2(&v);
	VE_VECTOR vec1 = VeLoadFloat2(&kVector.v);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat2(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator * (
	VeFloat32 f32Scale) const noexcept
{
	VeVector2 kRes;
	VE_VECTOR vec = VeLoadFloat2(&v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat2(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator / (
	VeFloat32 f32Scale) const noexcept
{
	VeVector2 kRes;
	VE_VECTOR vec0 = VeLoadFloat2(&v);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat2(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2 operator * (VeFloat32 f32Scale,
	const VeVector2& kVector) noexcept
{
	VeVector2 kRes;
	VE_VECTOR vec = VeLoadFloat2(&kVector.v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat2(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeVector2::operator == (
	const VeVector2& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2(&v);
	VE_VECTOR vec1 = VeLoadFloat2(&kVector.v);
	return VeVector2Equal(vec0, vec1);
}
//--------------------------------------------------------------------------
inline bool VeVector2::operator != (
	const VeVector2& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2(&v);
	VE_VECTOR vec1 = VeLoadFloat2(&kVector.v);
	return VeVector2NotEqual(vec0, vec1);
}
//--------------------------------------------------------------------------
inline void VeVector2::SetZero() noexcept
{
	VE_VECTOR vec = VeVectorZero();
	VeStoreFloat2(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeVector2::Set(VeFloat32 f32X, VeFloat32 f32Y) noexcept
{
	VE_VECTOR vec = VeVectorSet(f32X, f32Y, 0, 0);
	VeStoreFloat2(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeVector2::Scale(const VeVector2& kVector,
	VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat2(&kVector.v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat2(&v, vec);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::Length() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat2(&v);
	vec = VeVector2Length(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::LengthSquared() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat2(&v);
	vec = VeVector2LengthSq(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector2::Normalise() noexcept
{
	VE_VECTOR vec = VeLoadFloat2(&v);
	vec = VeVector2Normalize(vec);
	VeStoreFloat2(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeVector2::ParaMul(const VeVector2& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2(&v);
	VE_VECTOR vec1 = VeLoadFloat2(&kVector.v);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat2(&v, vec0);
}
//--------------------------------------------------------------------------
inline void VeVector2::ParaMul(const VeVector2& kVector0,
	const VeVector2& kVector1) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2(&kVector0.v);
	VE_VECTOR vec1 = VeLoadFloat2(&kVector1.v);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat2(&v, vec0);
}
//--------------------------------------------------------------------------
inline void VeVector2::Lerp(const VeVector2& kVector0,
	const VeVector2& kVector1, VeFloat32 f32Alpha) noexcept
{
	*this = VeLerp(f32Alpha, kVector0, kVector1);
}
//--------------------------------------------------------------------------
inline void VeVector2::Clamp(const VeVector2& kLower,
	const VeVector2& kUpper) noexcept
{
	VE_VECTOR vec = VeLoadFloat2(&v);
	VE_VECTOR min = VeLoadFloat2(&kLower.v);
	VE_VECTOR max = VeLoadFloat2(&kUpper.v);
	vec = VeVectorClamp(vec, min, max);
	VeStoreFloat2(&v, vec);
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::UnitVector() const noexcept
{
	VeVector2 kRes;
	VE_VECTOR vec = VeLoadFloat2(&v);
	vec = VeVector2Normalize(vec);
	VeStoreFloat2(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::Dot(const VeVector2& kVector) const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec0 = VeLoadFloat2(&v);
	VE_VECTOR vec1 = VeLoadFloat2(&kVector.v);
	vec0 = VeVector2Dot(vec0, vec1);
	VeStoreFloat(&f32Res, vec0);
	return f32Res;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::Cross(const VeVector2& kVector) const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec0 = VeLoadFloat2(&v);
	VE_VECTOR vec1 = VeLoadFloat2(&kVector.v);
	vec0 = VeVector2Cross(vec0, vec1);
	VeStoreFloat(&f32Res, vec0);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector2::ProjectOnto(const VeVector2& kVector0,
	const VeVector2& kVector1) noexcept
{
	*this = kVector1 * (kVector0.Dot(kVector1) / kVector1.LengthSquared());
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::ProjectOnto(
	const VeVector2& kVector) const noexcept
{
	VeVector2 kRes;
	kRes.ProjectOnto(*this, kVector);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeAlmostEqual(const VeVector2& kVector0,
	const VeVector2& kVector1, const VeFloat32 f32Epsilon) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2(&kVector0.v);
	VE_VECTOR vec1 = VeLoadFloat2(&kVector1.v);
	VE_VECTOR e = VeVectorReplicate(f32Epsilon);
	return VeVector2NearEqual(vec0, vec1, e);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeDistance(const VeVector2& kVector0,
	const VeVector2& kVector1) noexcept
{
	VeVector2 kDelta = kVector0 - kVector1;
	return kDelta.Length();
}
//--------------------------------------------------------------------------
inline VeFloat32 VeDistanceSquared(const VeVector2& kVector0,
	const VeVector2& kVector1) noexcept
{
	VeVector2 kDelta = kVector0 - kVector1;
	return kDelta.LengthSquared();
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector2A::operator [] (Coord eCoord) noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2A::operator [] (Coord eCoord) const noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector2A::operator () (Coord eCoord) noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2A::operator () (Coord eCoord) const noexcept
{
	return f[eCoord];
}
//--------------------------------------------------------------------------
inline VeVector2A& VeVector2A::operator = (const VeVector2A& kVector) noexcept
{
	VE_VECTOR vec = VeLoadFloat2A(&kVector.v);
	VeStoreFloat2A(&v, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2A& VeVector2A::operator += (const VeVector2A& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2A(&v);
	VE_VECTOR vec1 = VeLoadFloat2A(&kVector.v);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat2A(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2A& VeVector2A::operator -= (const VeVector2A& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2A(&v);
	VE_VECTOR vec1 = VeLoadFloat2A(&kVector.v);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat2A(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2A& VeVector2A::operator *= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat2A(&v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat2A(&v, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2A& VeVector2A::operator /= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2A(&v);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat2A(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2A VeVector2A::operator + () const noexcept
{
	VeVector2A kRes;
	VE_VECTOR vec = VeLoadFloat2A(&v);
	VeStoreFloat2A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2A VeVector2A::operator - () const noexcept
{
	VeVector2A kRes;
	VE_VECTOR vec = VeLoadFloat2A(&v);
	vec = VeVectorNegate(vec);
	VeStoreFloat2A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2A VeVector2A::operator + (
	const VeVector2A& kVector) const noexcept
{
	VeVector2A kRes;
	VE_VECTOR vec0 = VeLoadFloat2A(&v);
	VE_VECTOR vec1 = VeLoadFloat2A(&kVector.v);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat2A(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2A VeVector2A::operator - (
	const VeVector2A& kVector) const noexcept
{
	VeVector2A kRes;
	VE_VECTOR vec0 = VeLoadFloat2A(&v);
	VE_VECTOR vec1 = VeLoadFloat2A(&kVector.v);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat2A(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2A VeVector2A::operator * (
	VeFloat32 f32Scale) const noexcept
{
	VeVector2A kRes;
	VE_VECTOR vec = VeLoadFloat2A(&v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat2A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2A VeVector2A::operator / (
	VeFloat32 f32Scale) const noexcept
{
	VeVector2A kRes;
	VE_VECTOR vec0 = VeLoadFloat2A(&v);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat2A(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector2A operator * (VeFloat32 f32Scale,
	const VeVector2A& kVector) noexcept
{
	VeVector2A kRes;
	VE_VECTOR vec = VeLoadFloat2A(&kVector.v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat2A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeVector2A::operator == (
	const VeVector2A& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2A(&v);
	VE_VECTOR vec1 = VeLoadFloat2A(&kVector.v);
	return VeVector2Equal(vec0, vec1);
}
//--------------------------------------------------------------------------
inline bool VeVector2A::operator != (
	const VeVector2A& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2A(&v);
	VE_VECTOR vec1 = VeLoadFloat2A(&kVector.v);
	return VeVector2NotEqual(vec0, vec1);
}
//--------------------------------------------------------------------------
inline void VeVector2A::SetZero() noexcept
{
	VE_VECTOR vec = VeVectorZero();
	VeStoreFloat2A(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeVector2A::Set(VeFloat32 f32X, VeFloat32 f32Y) noexcept
{
	VE_VECTOR vec = VeVectorSet(f32X, f32Y, 0, 0);
	VeStoreFloat2A(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeVector2A::Scale(const VeVector2A& kVector,
	VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat2A(&kVector.v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat2A(&v, vec);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2A::Length() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat2A(&v);
	vec = VeVector2Length(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2A::LengthSquared() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat2A(&v);
	vec = VeVector2LengthSq(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector2A::Normalise() noexcept
{
	VE_VECTOR vec = VeLoadFloat2A(&v);
	vec = VeVector2Normalize(vec);
	VeStoreFloat2A(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeVector2A::ParaMul(const VeVector2A& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2A(&v);
	VE_VECTOR vec1 = VeLoadFloat2A(&kVector.v);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat2A(&v, vec0);
}
//--------------------------------------------------------------------------
inline void VeVector2A::ParaMul(const VeVector2A& kVector0,
	const VeVector2A& kVector1) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2A(&kVector0.v);
	VE_VECTOR vec1 = VeLoadFloat2A(&kVector1.v);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat2A(&v, vec0);
}
//--------------------------------------------------------------------------
inline void VeVector2A::Lerp(const VeVector2A& kVector0,
	const VeVector2A& kVector1, VeFloat32 f32Alpha) noexcept
{
	*this = VeLerp(f32Alpha, kVector0, kVector1);
}
//--------------------------------------------------------------------------
inline void VeVector2A::Clamp(const VeVector2A& kLower,
	const VeVector2A& kUpper) noexcept
{
	VE_VECTOR vec = VeLoadFloat2A(&v);
	VE_VECTOR min = VeLoadFloat2A(&kLower.v);
	VE_VECTOR max = VeLoadFloat2A(&kUpper.v);
	vec = VeVectorClamp(vec, min, max);
	VeStoreFloat2A(&v, vec);
}
//--------------------------------------------------------------------------
inline VeVector2A VeVector2A::UnitVector() const noexcept
{
	VeVector2A kRes;
	VE_VECTOR vec = VeLoadFloat2A(&v);
	vec = VeVector2Normalize(vec);
	VeStoreFloat2A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2A::Dot(const VeVector2A& kVector) const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec0 = VeLoadFloat2A(&v);
	VE_VECTOR vec1 = VeLoadFloat2A(&kVector.v);
	vec0 = VeVector2Dot(vec0, vec1);
	VeStoreFloat(&f32Res, vec0);
	return f32Res;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2A::Cross(const VeVector2A& kVector) const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec0 = VeLoadFloat2A(&v);
	VE_VECTOR vec1 = VeLoadFloat2A(&kVector.v);
	vec0 = VeVector2Cross(vec0, vec1);
	VeStoreFloat(&f32Res, vec0);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector2A::ProjectOnto(const VeVector2A& kVector0,
	const VeVector2A& kVector1) noexcept
{
	*this = kVector1 * (kVector0.Dot(kVector1) / kVector1.LengthSquared());
}
//--------------------------------------------------------------------------
inline VeVector2A VeVector2A::ProjectOnto(
	const VeVector2A& kVector) const noexcept
{
	VeVector2A kRes;
	kRes.ProjectOnto(*this, kVector);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeAlmostEqual(const VeVector2A& kVector0,
	const VeVector2A& kVector1, const VeFloat32 f32Epsilon) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat2A(&kVector0.v);
	VE_VECTOR vec1 = VeLoadFloat2A(&kVector1.v);
	VE_VECTOR e = VeVectorReplicate(f32Epsilon);
	return VeVector2NearEqual(vec0, vec1, e);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeDistance(const VeVector2A& kVector0,
	const VeVector2A& kVector1) noexcept
{
	VeVector2A kDelta = kVector0 - kVector1;
	return kDelta.Length();
}
//--------------------------------------------------------------------------
inline VeFloat32 VeDistanceSquared(const VeVector2A& kVector0,
	const VeVector2A& kVector1) noexcept
{
	VeVector2A kDelta = kVector0 - kVector1;
	return kDelta.LengthSquared();
}
//--------------------------------------------------------------------------
