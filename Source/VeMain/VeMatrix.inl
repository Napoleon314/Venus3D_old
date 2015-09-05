////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMatrix.inl
//  Created:     2015/09/04 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeFloat32& VeMatrix4X3::operator () (VeUInt32 u32Col,
	VeUInt32 u32Row) noexcept
{
	VE_ASSERT(u32Col >= 0 && u32Col < 3 && u32Row >= 0 && u32Row < 4);
	return f[u32Col][u32Row];
}
//--------------------------------------------------------------------------
inline const VeFloat32& VeMatrix4X3::operator () (VeUInt32 u32Col,
	VeUInt32 u32Row) const noexcept
{
	VE_ASSERT(u32Col >= 0 && u32Col < 3 && u32Row >= 0 && u32Row < 4);
	return f[u32Col][u32Row];
}
//--------------------------------------------------------------------------
inline VeVector4& VeMatrix4X3::operator [] (VeUInt32 i) noexcept
{
	VE_ASSERT(i >= 0 && i < 3);
	return vec[i];
}
//--------------------------------------------------------------------------
inline const VeVector4& VeMatrix4X3::operator [] (VeUInt32 i) const noexcept
{
	VE_ASSERT(i >= 0 && i < 3);
	return vec[i];
}
//--------------------------------------------------------------------------
inline VeMatrix4X3& VeMatrix4X3::operator = (
	const VeMatrix4X3& kMatrix) noexcept
{
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&kMatrix.m);
	VeStoreFloat4x3A(&m, mat);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3& VeMatrix4X3::operator += (
	const VeMatrix4X3& kMatrix) noexcept
{
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMatrix.m);
	mat0.r[0] = VeVectorAdd(mat0.r[0], mat1.r[0]);
	mat0.r[1] = VeVectorAdd(mat0.r[1], mat1.r[1]);
	mat0.r[2] = VeVectorAdd(mat0.r[2], mat1.r[2]);
	VeStoreFloat4x3A(&m, mat0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3& VeMatrix4X3::operator -= (
	const VeMatrix4X3& kMatrix) noexcept
{
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMatrix.m);
	mat0.r[0] = VeVectorSubtract(mat0.r[0], mat1.r[0]);
	mat0.r[1] = VeVectorSubtract(mat0.r[1], mat1.r[1]);
	mat0.r[2] = VeVectorSubtract(mat0.r[2], mat1.r[2]);
	VeStoreFloat4x3A(&m, mat0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3& VeMatrix4X3::operator *= (
	VeFloat32 f32Scale) noexcept
{
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&m);
	mat.r[0] = VeVectorScale(mat.r[0], f32Scale);
	mat.r[1] = VeVectorScale(mat.r[1], f32Scale);
	mat.r[2] = VeVectorScale(mat.r[2], f32Scale);
	VeStoreFloat4x3A(&m, mat);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3& VeMatrix4X3::operator /= (
	VeFloat32 f32Scale) noexcept
{
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&m);
	VE_VECTOR vec = VeVectorReplicate(f32Scale);
	vec = VeVectorReciprocal(vec);
	mat.r[0] = VeVectorMultiply(mat.r[0], vec);
	mat.r[1] = VeVectorMultiply(mat.r[1], vec);
	mat.r[2] = VeVectorMultiply(mat.r[2], vec);
	VeStoreFloat4x3A(&m, mat);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3& VeMatrix4X3::operator *= (
	const VeMatrix4X3& kMatrix) noexcept
{
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMatrix.m);
	mat0 = VeMatrix4X3Multiply(mat0, mat1);
	VeStoreFloat4x3A(&m, mat0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3 VeMatrix4X3::operator + () const noexcept
{
	VeMatrix4X3 kRes;
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&m);
	VeStoreFloat4x3A(&kRes.m, mat);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3 VeMatrix4X3::operator - () const noexcept
{
	VeMatrix4X3 kRes;
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&m);
	mat.r[0] = VeVectorNegate(mat.r[0]);
	mat.r[1] = VeVectorNegate(mat.r[1]);
	mat.r[2] = VeVectorNegate(mat.r[2]);
	VeStoreFloat4x3A(&kRes.m, mat);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3 VeMatrix4X3::operator * (
	const VeMatrix4X3& kMatrix) const noexcept
{
	VeMatrix4X3 kRes;
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMatrix.m);
	mat0 = VeMatrix4X3Multiply(mat0, mat1);
	VeStoreFloat4x3A(&kRes.m, mat0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3 VeMatrix4X3::operator + (
	const VeMatrix4X3& kMatrix) const noexcept
{
	VeMatrix4X3 kRes;
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMatrix.m);
	mat0.r[0] = VeVectorAdd(mat0.r[0], mat1.r[0]);
	mat0.r[1] = VeVectorAdd(mat0.r[1], mat1.r[1]);
	mat0.r[2] = VeVectorAdd(mat0.r[2], mat1.r[2]);
	VeStoreFloat4x3A(&kRes.m, mat0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3 VeMatrix4X3::operator - (
	const VeMatrix4X3& kMatrix) const noexcept
{
	VeMatrix4X3 kRes;
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMatrix.m);
	mat0.r[0] = VeVectorSubtract(mat0.r[0], mat1.r[0]);
	mat0.r[1] = VeVectorSubtract(mat0.r[1], mat1.r[1]);
	mat0.r[2] = VeVectorSubtract(mat0.r[2], mat1.r[2]);
	VeStoreFloat4x3A(&kRes.m, mat0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3 VeMatrix4X3::operator * (
	VeFloat32 f32Scale) const noexcept
{
	VeMatrix4X3 kRes;
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&m);
	mat.r[0] = VeVectorScale(mat.r[0], f32Scale);
	mat.r[1] = VeVectorScale(mat.r[1], f32Scale);
	mat.r[2] = VeVectorScale(mat.r[2], f32Scale);
	VeStoreFloat4x3A(&kRes.m, mat);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3 VeMatrix4X3::operator / (
	VeFloat32 f32Scale) const noexcept
{
	VeMatrix4X3 kRes;
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&m);
	VE_VECTOR vec = VeVectorReplicate(f32Scale);
	vec = VeVectorReciprocal(vec);
	mat.r[0] = VeVectorMultiply(mat.r[0], vec);
	mat.r[1] = VeVectorMultiply(mat.r[1], vec);
	mat.r[2] = VeVectorMultiply(mat.r[2], vec);
	VeStoreFloat4x3A(&kRes.m, mat);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3 operator * (VeFloat32 f32Scale,
	const VeMatrix4X3& kMatrix) noexcept
{
	VeMatrix4X3 kRes;
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&kMatrix.m);
	mat.r[0] = VeVectorScale(mat.r[0], f32Scale);
	mat.r[1] = VeVectorScale(mat.r[1], f32Scale);
	mat.r[2] = VeVectorScale(mat.r[2], f32Scale);
	VeStoreFloat4x3A(&kRes.m, mat);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeMatrix4X3::operator == (
	const VeMatrix4X3& kMatrix) const noexcept
{
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMatrix.m);
	return VeVector4Equal(mat0.r[0], mat1.r[0])
		&& VeVector4Equal(mat0.r[1], mat1.r[1])
		&& VeVector4Equal(mat0.r[2], mat1.r[2]);
}
//--------------------------------------------------------------------------
inline bool VeMatrix4X3::operator != (
	const VeMatrix4X3& kMatrix) const noexcept
{
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMatrix.m);
	return VeVector4NotEqual(mat0.r[0], mat1.r[0])
		|| VeVector4NotEqual(mat0.r[1], mat1.r[1])
		|| VeVector4NotEqual(mat0.r[2], mat1.r[2]);
}
//--------------------------------------------------------------------------
inline VeVector4& VeMatrix4X3::Row(VeUInt32 i) noexcept
{
	VE_ASSERT(i >= 0 && i < 3);
	return *(VeVector4*)&v[i];
}
//--------------------------------------------------------------------------
inline const VeVector4& VeMatrix4X3::Row(VeUInt32 i) const noexcept
{
	VE_ASSERT(i >= 0 && i < 3);
	return *(const VeVector4*)&v[i];
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::Row(VeUInt32 i, const VeVector4& kVec) noexcept
{
	VE_ASSERT(i >= 0 && i < 3);
	VE_VECTOR ve = VeLoadFloat4A(&kVec.v);
	VeStoreFloat4A(&v[i], ve);
}
//--------------------------------------------------------------------------
inline VeVector4 VeMatrix4X3::Col(VeUInt32 i) const noexcept
{
	VE_ASSERT(0 <= i && i < 4);
	return VeVector4(f[0][i], f[1][i], f[2][i], f[3][i]);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::Col(VeUInt32 i, const VeVector4& kVec) noexcept
{
	VE_ASSERT(0 <= i && i < 4);
	f[0][i] = kVec.x;
	f[1][i] = kVec.y;
	f[2][i] = kVec.z;
	f[3][i] = kVec.w;
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::SetZero() noexcept
{
	VE_VECTOR vec = VeVectorZero();
	VeStoreFloat4A(&v[0], vec);
	VeStoreFloat4A(&v[1], vec);
	VeStoreFloat4A(&v[2], vec);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::SetIdentity() noexcept
{
	VE_MATRIX4X3 mat = VeMatrix4X3Identity();
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::SetScale(VeFloat32 x, VeFloat32 y,
	VeFloat32 z) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixScaling(x, y, z);
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::SetRotateX(VeFloat32 f32Angle) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixRotationX(f32Angle);
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::SetRotateY(VeFloat32 f32Angle) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixRotationY(f32Angle);
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::SetRotateZ(VeFloat32 f32Angle) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixRotationZ(f32Angle);
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::SetRotate(const VeQuaternion& kQuat) noexcept
{

}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::SetTranslation(VeFloat32 x, VeFloat32 y,
	VeFloat32 z) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixTranslation(x, y, z);
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
