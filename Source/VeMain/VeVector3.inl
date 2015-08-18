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
inline VeVector3::operator VeFloat32* () noexcept
{
	return VE_FLOAT_POINT_THIS;
}
//--------------------------------------------------------------------------
inline VeVector3::operator const VeFloat32* () const noexcept
{
	return VE_FLOAT_POINT_THIS;
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
	x = kVector.x;
	y = kVector.y;
	z = kVector.z;
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3& VeVector3::operator += (const VeVector3& kVector) noexcept
{
	x += kVector.x;
	y += kVector.y;
	z += kVector.z;
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3& VeVector3::operator -= (const VeVector3& kVector) noexcept
{
	x -= kVector.x;
	y -= kVector.y;
	z -= kVector.z;
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3& VeVector3::operator *= (VeFloat32 f32Scale) noexcept
{
	x *= f32Scale;
	y *= f32Scale;
	z *= f32Scale;
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3& VeVector3::operator /= (VeFloat32 f32Scale) noexcept
{
	f32Scale = 1.0f / f32Scale;
	x *= f32Scale;
	y *= f32Scale;
	z *= f32Scale;
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator + () const noexcept
{
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator - () const noexcept
{
	return VeVector3(-x, -y, -z);
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator + (const VeVector3& kVector) const noexcept
{
	return VeVector3(x + kVector.x, y + kVector.y, z + kVector.z);
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator - (const VeVector3& kVector) const noexcept
{
	return VeVector3(x - kVector.x, y - kVector.y, z - kVector.z);
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator * (VeFloat32 f32Scale) const noexcept
{
	return VeVector3(x * f32Scale, y * f32Scale, z * f32Scale);
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::operator / (VeFloat32 f32Scale) const noexcept
{
	f32Scale = 1.0f / f32Scale;
	return VeVector3(x * f32Scale, y * f32Scale, z * f32Scale);
}
//--------------------------------------------------------------------------
inline VeVector3 operator * (VeFloat32 f32Scale,
	const VeVector3& kVector) noexcept
{
	return kVector * f32Scale;
}
//--------------------------------------------------------------------------
inline bool VeVector3::operator == (const VeVector3& kVector) const noexcept
{
	return (x == kVector.x) && (y == kVector.y) && (z == kVector.z);
}
//--------------------------------------------------------------------------
inline bool VeVector3::operator != (const VeVector3& kVector) const noexcept
{
	return (x != kVector.x) || (y != kVector.y) || (z != kVector.z);
}
//--------------------------------------------------------------------------
inline void VeVector3::SetZero() noexcept
{
	x = 0;
	y = 0;
	z = 0;
}
//--------------------------------------------------------------------------
inline void VeVector3::Set(VeFloat32 f32X, VeFloat32 f32Y,
	VeFloat32 f32Z) noexcept
{
	x = f32X;
	y = f32Y;
	z = f32Z;
}
//--------------------------------------------------------------------------
inline void VeVector3::Scale(const VeVector3& kVector,
	VeFloat32 f32Scale) noexcept
{
	*this = kVector * f32Scale;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3::Length() const noexcept
{
	return VeSqrtf(LengthSquared());
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector3::LengthSquared() const noexcept
{
	return x * x + y * y + z * z;
}
//--------------------------------------------------------------------------
inline void VeVector3::Normalise() noexcept
{
	VeFloat32 f32LengthSquared = LengthSquared();
	if (f32LengthSquared)
	{
		*this *= VeInvSqrtf(f32LengthSquared);
	}
}
//--------------------------------------------------------------------------
inline void VeVector3::ParaMul(const VeVector3& kVector) noexcept
{
	x *= kVector.x;
	y *= kVector.y;
	z *= kVector.z;
}
//--------------------------------------------------------------------------
inline void VeVector3::ParaMul(const VeVector3& kVector0,
	const VeVector3& kVector1) noexcept
{
	x = kVector0.x * kVector1.x;
	y = kVector0.y * kVector1.y;
	z = kVector0.z * kVector1.z;
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
	*this = VeClampEx(kLower, *this, kUpper);
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::UnitVector() const noexcept
{
	VeVector3 kResVector(*this);
	kResVector.Normalise();
	return kResVector;
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
	return x * kVector.x + y * kVector.y + z * kVector.z;
}
//--------------------------------------------------------------------------
inline void VeVector3::Cross(const VeVector3& kVector0,
	const VeVector3& kVector1) noexcept
{
	x = (kVector0.y * kVector1.z) - (kVector0.z * kVector1.y);
	y = (kVector0.z * kVector1.x) - (kVector0.x * kVector1.z);
	z = (kVector0.x * kVector1.y) - (kVector0.y * kVector1.x);
}
//--------------------------------------------------------------------------
inline VeVector3 VeVector3::Cross(const VeVector3& kVector) const noexcept
{
	VeVector3 kRes;
	kRes.Cross(*this, kVector);
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
	return VeAlmostEqual(kVector0.x, kVector1.x, f32Epsilon) &&
		VeAlmostEqual(kVector0.y, kVector1.y, f32Epsilon) &&
		VeAlmostEqual(kVector0.z, kVector1.z, f32Epsilon);
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
