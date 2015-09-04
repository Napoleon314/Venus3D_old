////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeVector3.inl
//  Created:     2015/08/18 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeVector3::operator VE_FLOAT3* () noexcept
{
	return (VE_FLOAT3*)this;
}
//--------------------------------------------------------------------------
inline VeVector3::operator const VE_FLOAT3* () const noexcept
{
	return (const VE_FLOAT3*)this;
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector3::operator [] (Coord eCoord) noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3::operator [] (Coord eCoord) const noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector3::operator () (Coord eCoord) noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3::operator () (Coord eCoord) const noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeVector3& VeVector3::operator = (const VeVector3& kVector) noexcept
{
	VE_VECTOR vec = VeLoadFloat3(kVector);
	VeStoreFloat3(*this, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3& VeVector3::operator += (const VeVector3& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3(*this);
	VE_VECTOR vec1 = VeLoadFloat3(kVector);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat3(*this, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3& VeVector3::operator -= (const VeVector3& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3(*this);
	VE_VECTOR vec1 = VeLoadFloat3(kVector);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat3(*this, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3& VeVector3::operator *= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat3(*this);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat3(*this, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3& VeVector3::operator /= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3(*this);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat3(*this, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator + () const noexcept
{
	VeVector3 kRes;
	VE_VECTOR vec = VeLoadFloat3(*this);
	VeStoreFloat3(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator - () const noexcept
{
	VeVector3 kRes;
	VE_VECTOR vec = VeLoadFloat3(*this);
	vec = VeVectorNegate(vec);
	VeStoreFloat3(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator + (const VeVector3& kVector) const noexcept
{
	VeVector3 kRes;
	VE_VECTOR vec0 = VeLoadFloat3(*this);
	VE_VECTOR vec1 = VeLoadFloat3(kVector);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat3(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator - (const VeVector3& kVector) const noexcept
{
	VeVector3 kRes;
	VE_VECTOR vec0 = VeLoadFloat3(*this);
	VE_VECTOR vec1 = VeLoadFloat3(kVector);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat3(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator * (VeFloat32 f32Scale) const noexcept
{
	VeVector3 kRes;
	VE_VECTOR vec = VeLoadFloat3(*this);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat3(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator / (VeFloat32 f32Scale) const noexcept
{
	VeVector3 kRes;
	VE_VECTOR vec0 = VeLoadFloat3(*this);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat3(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3 operator * (VeFloat32 f32Scale,
	const VeVector3& kVector) noexcept
{
	VeVector3 kRes;
	VE_VECTOR vec = VeLoadFloat3(kVector);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat3(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeVector3::operator == (const VeVector3& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3(*this);
	VE_VECTOR vec1 = VeLoadFloat3(kVector);
	return VeVector3Equal(vec0, vec1);
}
//--------------------------------------------------------------------------
inline bool VeVector3::operator != (const VeVector3& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3(*this);
	VE_VECTOR vec1 = VeLoadFloat3(kVector);
	return VeVector3NotEqual(vec0, vec1);
}
//--------------------------------------------------------------------------
inline void VeVector3::SetZero() noexcept
{
	VE_VECTOR vec = VeVectorZero();
	VeStoreFloat3(*this, vec);
}
//--------------------------------------------------------------------------
inline void VeVector3::Set(VeFloat32 f32X, VeFloat32 f32Y,
	VeFloat32 f32Z) noexcept
{
	VE_VECTOR vec = VeVectorSet(f32X, f32Y, f32Z, 0);
	VeStoreFloat3(*this, vec);
}
//--------------------------------------------------------------------------
inline void VeVector3::Scale(const VeVector3& kVector,
	VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat3(kVector);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat3(*this, vec);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3::Length() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat3(*this);
	vec = VeVector3Length(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3::LengthSquared() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat3(*this);
	vec = VeVector3LengthSq(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector3::Normalise() noexcept
{
	VE_VECTOR vec = VeLoadFloat3(*this);
	vec = VeVector3Normalize(vec);
	VeStoreFloat3(*this, vec);
}
//--------------------------------------------------------------------------
inline void VeVector3::ParaMul(const VeVector3& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3(*this);
	VE_VECTOR vec1 = VeLoadFloat3(kVector);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat3(*this, vec0);
}
//--------------------------------------------------------------------------
inline void VeVector3::ParaMul(const VeVector3& kVector0,
	const VeVector3& kVector1) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3(kVector0);
	VE_VECTOR vec1 = VeLoadFloat3(kVector1);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat3(*this, vec0);
}
//--------------------------------------------------------------------------
inline void VeVector3::Lerp(const VeVector3& kVector0,
	const VeVector3& kVector1, VeFloat32 f32Alpha) noexcept
{
	*this = VeLerp(f32Alpha, kVector0, kVector1);
}
//--------------------------------------------------------------------------
inline void VeVector3::Clamp(const VeVector3& kLower,
	const VeVector3& kUpper) noexcept
{
	VE_VECTOR vec = VeLoadFloat3(*this);
	VE_VECTOR min = VeLoadFloat3(kLower);
	VE_VECTOR max = VeLoadFloat3(kUpper);
	vec = VeVectorClamp(vec, min, max);
	VeStoreFloat3(*this, vec);
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::UnitVector() const noexcept
{
	VeVector3 kRes;
	VE_VECTOR vec = VeLoadFloat3(*this);
	vec = VeVector3Normalize(vec);
	VeStoreFloat3(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline void VeVector3::SetPitchYaw(VeFloat32 f32PitchInRadians,
	VeFloat32 f32YawInRadians) noexcept
{
	VeFloat32 f32CosPitch = VeCosf(f32PitchInRadians);
	VeFloat32 f32SinPitch = VeSinf(-f32PitchInRadians);
	VeFloat32 f32CosYaw = VeCosf(f32YawInRadians);
	VeFloat32 f32SinYaw = VeSinf(f32YawInRadians);
	x = f32CosPitch * f32SinYaw;
	y = f32CosPitch * f32CosYaw;
	z = f32SinPitch;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3::GetYaw() const noexcept
{
	return VeAtan2f(x, y);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3::GetPitch() const noexcept
{
	return -VeAtan2f(z, VeSqrtf(x * x + y * y));
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3::Dot(const VeVector3& kVector) const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec0 = VeLoadFloat3(*this);
	VE_VECTOR vec1 = VeLoadFloat3(kVector);
	vec0 = VeVector3Dot(vec0, vec1);
	VeStoreFloat(&f32Res, vec0);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector3::Cross(const VeVector3& kVector0,
	const VeVector3& kVector1) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3(kVector0);
	VE_VECTOR vec1 = VeLoadFloat3(kVector1);
	vec0 = VeVector3Cross(vec0, vec1);
	VeStoreFloat3(*this, vec0);
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::Cross(const VeVector3& kVector) const noexcept
{
	VeVector3 kRes;
	VE_VECTOR vec0 = VeLoadFloat3(*this);
	VE_VECTOR vec1 = VeLoadFloat3(kVector);
	vec0 = VeVector3Cross(vec0, vec1);
	VeStoreFloat3(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline void VeVector3::ProjectOnto(const VeVector3& kVector0,
	const VeVector3& kVector1) noexcept
{
	*this = kVector1 * (kVector0.Dot(kVector1)
		/ kVector1.LengthSquared());
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::ProjectOnto(
	const VeVector3& kVector) const noexcept
{
	VeVector3 kRes;
	kRes.ProjectOnto(*this, kVector);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeAlmostEqual(const VeVector3& kVector0,
	const VeVector3& kVector1, const VeFloat32 f32Epsilon) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3(kVector0);
	VE_VECTOR vec1 = VeLoadFloat3(kVector1);
	VE_VECTOR e = VeVectorReplicate(f32Epsilon);
	return VeVector3NearEqual(vec0, vec1, e);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeDistance(const VeVector3& kVector0,
	const VeVector3& kVector1) noexcept
{
	VeVector3 kDelta = kVector0 - kVector1;
	return kDelta.Length();
}
//--------------------------------------------------------------------------
inline VeFloat32 VeDistanceSquared(const VeVector3& kVector0,
	const VeVector3& kVector1) noexcept
{
	VeVector3 kDelta = kVector0 - kVector1;
	return kDelta.LengthSquared();
}
//--------------------------------------------------------------------------
inline VeVector3A::operator VE_FLOAT3A* () noexcept
{
	return (VE_FLOAT3A*)this;
}
//--------------------------------------------------------------------------
inline VeVector3A::operator const VE_FLOAT3A* () const noexcept
{
	return (const VE_FLOAT3A*)this;
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector3A::operator [] (Coord eCoord) noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3A::operator [] (Coord eCoord) const noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector3A::operator () (Coord eCoord) noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3A::operator () (Coord eCoord) const noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeVector3A& VeVector3A::operator = (const VeVector3A& kVector) noexcept
{
	VE_VECTOR vec = VeLoadFloat3A(kVector);
	VeStoreFloat3A(*this, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3A& VeVector3A::operator += (const VeVector3A& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3A(*this);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat3A(*this, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3A& VeVector3A::operator -= (const VeVector3A& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3A(*this);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat3A(*this, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3A& VeVector3A::operator *= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat3A(*this);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat3A(*this, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3A& VeVector3A::operator /= (VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3A(*this);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat3A(*this, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3A VeVector3A::operator + () const noexcept
{
	VeVector3A kRes;
	VE_VECTOR vec = VeLoadFloat3A(*this);
	VeStoreFloat3A(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3A VeVector3A::operator - () const noexcept
{
	VeVector3A kRes;
	VE_VECTOR vec = VeLoadFloat3A(*this);
	vec = VeVectorNegate(vec);
	VeStoreFloat3A(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3A VeVector3A::operator + (const VeVector3A& kVector) const noexcept
{
	VeVector3A kRes;
	VE_VECTOR vec0 = VeLoadFloat3A(*this);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat3A(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3A VeVector3A::operator - (const VeVector3A& kVector) const noexcept
{
	VeVector3A kRes;
	VE_VECTOR vec0 = VeLoadFloat3A(*this);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat3A(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3A VeVector3A::operator * (VeFloat32 f32Scale) const noexcept
{
	VeVector3A kRes;
	VE_VECTOR vec = VeLoadFloat3A(*this);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat3A(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3A VeVector3A::operator / (VeFloat32 f32Scale) const noexcept
{
	VeVector3A kRes;
	VE_VECTOR vec0 = VeLoadFloat3A(*this);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat3A(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3A operator * (VeFloat32 f32Scale,
	const VeVector3A& kVector) noexcept
{
	VeVector3A kRes;
	VE_VECTOR vec = VeLoadFloat3A(kVector);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat3A(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeVector3A::operator == (const VeVector3A& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3A(*this);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector);
	return VeVector3Equal(vec0, vec1);
}
//--------------------------------------------------------------------------
inline bool VeVector3A::operator != (const VeVector3A& kVector) const noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3A(*this);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector);
	return VeVector3NotEqual(vec0, vec1);
}
//--------------------------------------------------------------------------
inline void VeVector3A::SetZero() noexcept
{
	VE_VECTOR vec = VeVectorZero();
	VeStoreFloat3A(*this, vec);
}
//--------------------------------------------------------------------------
inline void VeVector3A::Set(VeFloat32 f32X, VeFloat32 f32Y,
	VeFloat32 f32Z) noexcept
{
	VE_VECTOR vec = VeVectorSet(f32X, f32Y, f32Z, 0);
	VeStoreFloat3A(*this, vec);
}
//--------------------------------------------------------------------------
inline void VeVector3A::Scale(const VeVector3A& kVector,
	VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat3A(kVector);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat3A(*this, vec);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3A::Length() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat3A(*this);
	vec = VeVector3Length(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3A::LengthSquared() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec = VeLoadFloat3A(*this);
	vec = VeVector3LengthSq(vec);
	VeStoreFloat(&f32Res, vec);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector3A::Normalise() noexcept
{
	VE_VECTOR vec = VeLoadFloat3A(*this);
	vec = VeVector3Normalize(vec);
	VeStoreFloat3A(*this, vec);
}
//--------------------------------------------------------------------------
inline void VeVector3A::ParaMul(const VeVector3A& kVector) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3A(*this);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat3A(*this, vec0);
}
//--------------------------------------------------------------------------
inline void VeVector3A::ParaMul(const VeVector3A& kVector0,
	const VeVector3A& kVector1) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3A(kVector0);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector1);
	vec0 = VeVectorMultiply(vec0, vec1);
	VeStoreFloat3A(*this, vec0);
}
//--------------------------------------------------------------------------
inline void VeVector3A::Lerp(const VeVector3A& kVector0,
	const VeVector3A& kVector1, VeFloat32 f32Alpha) noexcept
{
	*this = VeLerp(f32Alpha, kVector0, kVector1);
}
//--------------------------------------------------------------------------
inline void VeVector3A::Clamp(const VeVector3A& kLower,
	const VeVector3A& kUpper) noexcept
{
	VE_VECTOR vec = VeLoadFloat3A(*this);
	VE_VECTOR min = VeLoadFloat3A(kLower);
	VE_VECTOR max = VeLoadFloat3A(kUpper);
	vec = VeVectorClamp(vec, min, max);
	VeStoreFloat3A(*this, vec);
}
//--------------------------------------------------------------------------
inline VeVector3A VeVector3A::UnitVector() const noexcept
{
	VeVector3A kRes;
	VE_VECTOR vec = VeLoadFloat3A(*this);
	vec = VeVector3Normalize(vec);
	VeStoreFloat3A(kRes, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline void VeVector3A::SetPitchYaw(VeFloat32 f32PitchInRadians,
	VeFloat32 f32YawInRadians) noexcept
{
	VeFloat32 f32CosPitch = VeCosf(f32PitchInRadians);
	VeFloat32 f32SinPitch = VeSinf(-f32PitchInRadians);
	VeFloat32 f32CosYaw = VeCosf(f32YawInRadians);
	VeFloat32 f32SinYaw = VeSinf(f32YawInRadians);
	x = f32CosPitch * f32SinYaw;
	y = f32CosPitch * f32CosYaw;
	z = f32SinPitch;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3A::GetYaw() const noexcept
{
	return VeAtan2f(x, y);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3A::GetPitch() const noexcept
{
	return -VeAtan2f(z, VeSqrtf(x * x + y * y));
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3A::Dot(const VeVector3A& kVector) const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR vec0 = VeLoadFloat3A(*this);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector);
	vec0 = VeVector3Dot(vec0, vec1);
	VeStoreFloat(&f32Res, vec0);
	return f32Res;
}
//--------------------------------------------------------------------------
inline void VeVector3A::Cross(const VeVector3A& kVector0,
	const VeVector3A& kVector1) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3A(kVector0);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector1);
	vec0 = VeVector3Cross(vec0, vec1);
	VeStoreFloat3A(*this, vec0);
}
//--------------------------------------------------------------------------
inline VeVector3A VeVector3A::Cross(const VeVector3A& kVector) const noexcept
{
	VeVector3A kRes;
	VE_VECTOR vec0 = VeLoadFloat3A(*this);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector);
	vec0 = VeVector3Cross(vec0, vec1);
	VeStoreFloat3A(kRes, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline void VeVector3A::ProjectOnto(const VeVector3A& kVector0,
	const VeVector3A& kVector1) noexcept
{
	*this = kVector1 * (kVector0.Dot(kVector1)
		/ kVector1.LengthSquared());
}
//--------------------------------------------------------------------------
inline VeVector3A VeVector3A::ProjectOnto(
	const VeVector3A& kVector) const noexcept
{
	VeVector3A kRes;
	kRes.ProjectOnto(*this, kVector);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeAlmostEqual(const VeVector3A& kVector0,
	const VeVector3A& kVector1, const VeFloat32 f32Epsilon) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat3A(kVector0);
	VE_VECTOR vec1 = VeLoadFloat3A(kVector1);
	VE_VECTOR e = VeVectorReplicate(f32Epsilon);
	return VeVector3NearEqual(vec0, vec1, e);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeDistance(const VeVector3A& kVector0,
	const VeVector3A& kVector1) noexcept
{
	VeVector3A kDelta = kVector0 - kVector1;
	return kDelta.Length();
}
//--------------------------------------------------------------------------
inline VeFloat32 VeDistanceSquared(const VeVector3A& kVector0,
	const VeVector3A& kVector1) noexcept
{
	VeVector3A kDelta = kVector0 - kVector1;
	return kDelta.LengthSquared();
}
//--------------------------------------------------------------------------
