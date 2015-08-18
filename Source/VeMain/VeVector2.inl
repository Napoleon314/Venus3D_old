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
inline VeVector2::operator VeFloat32* () noexcept
{
	return VE_FLOAT_POINT_THIS;
}
//--------------------------------------------------------------------------
inline VeVector2::operator const VeFloat32* () const noexcept
{
	return VE_FLOAT_POINT_THIS;
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector2::operator [] (Coord eCoord) noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::operator [] (Coord eCoord) const noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32& VeVector2::operator () (Coord eCoord) noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::operator () (Coord eCoord) const noexcept
{
	return VE_FLOAT_POINT_THIS[eCoord];
}
//--------------------------------------------------------------------------
inline VeVector2& VeVector2::operator = (const VeVector2& kVector) noexcept
{
	x = kVector.x;
	y = kVector.y;
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2& VeVector2::operator += (const VeVector2& kVector) noexcept
{
	x += kVector.x;
	y += kVector.y;
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2& VeVector2::operator -= (const VeVector2& kVector) noexcept
{
	x -= kVector.x;
	y -= kVector.y;
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2& VeVector2::operator *= (VeFloat32 f32Scale) noexcept
{
	x *= f32Scale;
	y *= f32Scale;
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2& VeVector2::operator /= (VeFloat32 f32Scale) noexcept
{
	f32Scale = 1.0f / f32Scale;
	x *= f32Scale;
	y *= f32Scale;
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator + () const noexcept
{
	return *this;
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator - () const noexcept
{
	return VeVector2(-x, -y);
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator + (
	const VeVector2& kVector) const noexcept
{
	return VeVector2(x + kVector.x, y + kVector.y);
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator - (
	const VeVector2& kVector) const noexcept
{
	return VeVector2(x - kVector.x, y - kVector.y);
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator * (
	VeFloat32 f32Scale) const noexcept
{
	return VeVector2(x * f32Scale, y * f32Scale);
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::operator / (
	VeFloat32 f32Scale) const noexcept
{
	f32Scale = 1.0f / f32Scale;
	return VeVector2(x * f32Scale, y * f32Scale);
}
//--------------------------------------------------------------------------
inline VeVector2 operator * (VeFloat32 f32Scale,
	const VeVector2& kVector) noexcept
{
	return kVector * f32Scale;
}
//--------------------------------------------------------------------------
inline bool VeVector2::operator == (
	const VeVector2& kVector) const noexcept
{
	return (x == kVector.x) && (y == kVector.y);
}
//--------------------------------------------------------------------------
inline bool VeVector2::operator != (
	const VeVector2& kVector) const noexcept
{
	return (x != kVector.x) || (y != kVector.y);
}
//--------------------------------------------------------------------------
inline void VeVector2::SetZero() noexcept
{
	x = 0;
	y = 0;
}
//--------------------------------------------------------------------------
inline void VeVector2::Set(VeFloat32 f32X, VeFloat32 f32Y) noexcept
{
	x = f32X;
	y = f32Y;
}
//--------------------------------------------------------------------------
inline void VeVector2::Scale(const VeVector2& kVector,
	VeFloat32 f32Scale) noexcept
{
	*this = kVector * f32Scale;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::Length() const noexcept
{
	return VeSqrtf(LengthSquared());
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::LengthSquared() const noexcept
{
	return x * x + y * y;
}
//--------------------------------------------------------------------------
inline void VeVector2::Normalise() noexcept
{
	VeFloat32 f32LengthSquared = LengthSquared();
	if (f32LengthSquared)
	{
		*this *= VeInvSqrtf(f32LengthSquared);
	}
}
//--------------------------------------------------------------------------
inline void VeVector2::ParaMul(const VeVector2& kVector) noexcept
{
	x *= kVector.x;
	y *= kVector.y;
}
//--------------------------------------------------------------------------
inline void VeVector2::ParaMul(const VeVector2& kVector0,
	const VeVector2& kVector1) noexcept
{
	x = kVector0.x * kVector1.x;
	y = kVector0.y * kVector1.y;
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
	*this = VeClampEx(kLower, *this, kUpper);
}
//--------------------------------------------------------------------------
inline VeVector2 VeVector2::UnitVector() const noexcept
{
	VeVector2 kResVector(*this);
	kResVector.Normalise();
	return kResVector;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::Dot(const VeVector2& kVector) const noexcept
{
	return x * kVector.x + y * kVector.y;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeVector2::Cross(const VeVector2& kVector) const noexcept
{
	return x * y - y * x;
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
	return VeAlmostEqual(kVector0.x, kVector1.x, f32Epsilon)
		&& VeAlmostEqual(kVector0.y, kVector1.y, f32Epsilon);
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
