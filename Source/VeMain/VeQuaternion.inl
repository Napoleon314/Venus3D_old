////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeQuaternion.inl
//  Created:     2015/09/04 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
VeQuaternion& VeQuaternion::operator = (const VeQuaternion& kQuat) noexcept
{
	VE_VECTOR quat = VeLoadFloat4A(&kQuat.v);
	VeStoreFloat4A(&v, quat);
	return *this;
}
//--------------------------------------------------------------------------
inline VeQuaternion& VeQuaternion::operator += (
	const VeQuaternion& kQuat) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kQuat.v);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat4A(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeQuaternion& VeQuaternion::operator -= (
	const VeQuaternion& kQuat) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kQuat.v);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat4A(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeQuaternion& VeQuaternion::operator *= (
	const VeQuaternion& kQuat) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kQuat.v);
	vec0 = VeQuaternionMultiply(vec0, vec1);
	VeStoreFloat4A(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeQuaternion& VeQuaternion::operator *= (
	VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(&v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat4A(&v, vec);
	return *this;
}
//--------------------------------------------------------------------------
inline VeQuaternion& VeQuaternion::operator /= (
	VeFloat32 f32Scale) noexcept
{
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat4A(&v, vec0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeQuaternion VeQuaternion::operator + () const noexcept
{
	VeQuaternion kRes;
	VE_VECTOR vec = VeLoadFloat4A(&v);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeQuaternion VeQuaternion::operator - () const noexcept
{
	VeQuaternion kRes;
	VE_VECTOR vec = VeLoadFloat4A(&v);
	vec = VeVectorNegate(vec);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeQuaternion VeQuaternion::operator + (
	const VeQuaternion& kQuat) const noexcept
{
	VeQuaternion kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kQuat.v);
	vec0 = VeVectorAdd(vec0, vec1);
	VeStoreFloat4A(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeQuaternion VeQuaternion::operator - (
	const VeQuaternion& kQuat) const noexcept
{
	VeQuaternion kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kQuat.v);
	vec0 = VeVectorSubtract(vec0, vec1);
	VeStoreFloat4A(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeQuaternion VeQuaternion::operator * (
	const VeQuaternion& kQuat) const noexcept
{
	VeQuaternion kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeLoadFloat4A(&kQuat.v);
	vec0 = VeQuaternionMultiply(vec0, vec1);
	VeStoreFloat4A(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeQuaternion VeQuaternion::operator * (
	VeFloat32 f32Scale) const noexcept
{
	VeQuaternion kRes;
	VE_VECTOR vec = VeLoadFloat4A(&v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeQuaternion VeQuaternion::operator / (
	VeFloat32 f32Scale) const noexcept
{
	VeQuaternion kRes;
	VE_VECTOR vec0 = VeLoadFloat4A(&v);
	VE_VECTOR vec1 = VeVectorReplicate(f32Scale);
	vec0 = VeVectorDivide(vec0, vec1);
	VeStoreFloat4A(&kRes.v, vec0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeQuaternion operator * (VeFloat32 f32Scale,
	const VeQuaternion& kQuat) noexcept
{
	VeQuaternion kRes;
	VE_VECTOR vec = VeLoadFloat4A(&kQuat.v);
	vec = VeVectorScale(vec, f32Scale);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector3 VeQuaternion::operator * (
	const VeVector3& kVec) const noexcept
{
	VeVector3 kRes;
	VE_VECTOR vec = VeLoadFloat3(&kVec.v);
	VE_VECTOR quat = VeLoadFloat4A(&v);
	vec = VeVector3Rotate(vec, quat);
	VeStoreFloat3(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeQuaternion::operator == (const VeQuaternion& kQuat) const
{
	VE_VECTOR quat0 = VeLoadFloat4A(&v);
	VE_VECTOR quat1 = VeLoadFloat4A(&kQuat.v);
	return VeQuaternionEqual(quat0, quat1);
}
//--------------------------------------------------------------------------
inline bool VeQuaternion::operator != (const VeQuaternion& kQuat) const
{
	VE_VECTOR quat0 = VeLoadFloat4A(&v);
	VE_VECTOR quat1 = VeLoadFloat4A(&kQuat.v);
	return VeQuaternionNotEqual(quat0, quat1);
}
//--------------------------------------------------------------------------
inline void VeQuaternion::SetZero() noexcept
{
	VE_VECTOR vec = VeVectorZero();
	VeStoreFloat4A(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeQuaternion::Set(VeFloat32 f32X, VeFloat32 f32Y,
	VeFloat32 f32Z, VeFloat32 f32W) noexcept
{
	VE_VECTOR vec = VeVectorSet(f32X, f32Y, f32Z, f32W);
	VeStoreFloat4A(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeQuaternion::FromAngleAxis(const VeVector3& kAxis,
	VeFloat32 f32Angle) noexcept
{
	VE_VECTOR vec = VeLoadFloat3(&kAxis.v);
	vec = VeQuaternionRotationAxis(vec, f32Angle);
	VeStoreFloat4A(&v, vec);
}
//--------------------------------------------------------------------------
inline void VeQuaternion::Normalise() noexcept
{
	VE_VECTOR quat = VeLoadFloat4A(&v);
	quat = VeQuaternionNormalize(quat);
	VeStoreFloat4A(&v, quat);
}
//--------------------------------------------------------------------------
inline void VeQuaternion::Invert() noexcept
{
	VE_VECTOR quat = VeLoadFloat4A(&v);
	quat = VeQuaternionInverse(quat);
	VeStoreFloat4A(&v, quat);
}
//--------------------------------------------------------------------------
inline void VeQuaternion::Minimise() noexcept
{
	if (w < 0)
	{
		*this = -(*this);
	}
}
//--------------------------------------------------------------------------
inline void VeQuaternion::Slerp(const VeQuaternion& kStart,
	const VeQuaternion &kEnd, VeFloat32 t) noexcept
{
	VE_VECTOR quat0 = VeLoadFloat4A(&kStart.v);
	VE_VECTOR quat1 = VeLoadFloat4A(&kEnd.v);
	quat0 = VeQuaternionSlerp(quat0, quat1, t);
	VeStoreFloat4A(&v, quat0);
}
//--------------------------------------------------------------------------
inline void VeQuaternion::Mul(const VeQuaternion& kQuat0,
	const VeQuaternion& kQuat1) noexcept
{
	VE_VECTOR quat0 = VeLoadFloat4A(&kQuat0.v);
	VE_VECTOR quat1 = VeLoadFloat4A(&kQuat1.v);
	quat0 = VeQuaternionMultiply(quat0, quat1);
	VeStoreFloat4A(&v, quat0);
}
//--------------------------------------------------------------------------
inline void VeQuaternion::PreMul(const VeQuaternion& kQuat) noexcept
{
	VE_VECTOR quat0 = VeLoadFloat4A(&kQuat.v);
	VE_VECTOR quat1 = VeLoadFloat4A(&v);
	quat0 = VeQuaternionMultiply(quat0, quat1);
	VeStoreFloat4A(&v, quat0);
}
//--------------------------------------------------------------------------
inline void VeQuaternion::PostMul(const VeQuaternion& kQuat) noexcept
{
	VE_VECTOR quat0 = VeLoadFloat4A(&v);
	VE_VECTOR quat1 = VeLoadFloat4A(&kQuat.v);
	quat0 = VeQuaternionMultiply(quat0, quat1);
	VeStoreFloat4A(&v, quat0);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeQuaternion::Dot(const VeQuaternion& kQuat) const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR quat0 = VeLoadFloat4A(&v);
	VE_VECTOR quat1 = VeLoadFloat4A(&kQuat.v);
	quat0 = VeQuaternionDot(quat0, quat1);
	VeStoreFloat(&f32Res, quat0);
	return f32Res;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeQuaternion::Length() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR quat = VeLoadFloat4A(&v);
	quat = VeQuaternionLength(quat);
	VeStoreFloat(&f32Res, quat);
	return f32Res;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeQuaternion::LengthSquared() const noexcept
{
	VeFloat32 f32Res;
	VE_VECTOR quat = VeLoadFloat4A(&v);
	quat = VeQuaternionLengthSq(quat);
	VeStoreFloat(&f32Res, quat);
	return f32Res;
}
//--------------------------------------------------------------------------
