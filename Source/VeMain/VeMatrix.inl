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
	const VeMatrix4X3& kMatrix) noexcept
{
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMatrix.m);
	mat0 = VeMatrixMultiply(mat0, mat1);
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
	const VeMatrix4X3& kMatrix) const noexcept
{
	VeMatrix4X3 kRes;
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMatrix.m);
	mat0 = VeMatrixMultiply(mat0, mat1);
	VeStoreFloat4x3A(&kRes.m, mat0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeMatrix4X3::operator * (
	const VeVector4& kVector) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(&kVector.v);
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&m);
	vec = VeVector4Transform(vec, mat);
	VeStoreFloat4A(&kRes.v, vec);
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
inline VeVector4 operator * (const VeVector4& kVector,
	const VeMatrix4X3& kMatrix) noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(&kVector.v);
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&kMatrix.m);
	vec = VeVector4Transform(vec, mat);
	VeStoreFloat4A(&kRes.v, vec);
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
inline VeVector4& VeMatrix4X3::Col(VeUInt32 i) noexcept
{
	VE_ASSERT(i >= 0 && i < 3);
	return *(VeVector4*)&v[i];
}
//--------------------------------------------------------------------------
inline const VeVector4& VeMatrix4X3::Col(VeUInt32 i) const noexcept
{
	VE_ASSERT(i >= 0 && i < 3);
	return *(const VeVector4*)&v[i];
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::Col(VeUInt32 i, const VeVector4& kVec) noexcept
{
	VE_ASSERT(i >= 0 && i < 3);
	VE_VECTOR ve = VeLoadFloat4A(&kVec.v);
	VeStoreFloat4A(&v[i], ve);
}
//--------------------------------------------------------------------------
inline VeVector4 VeMatrix4X3::Row(VeUInt32 i) const noexcept
{
	VE_ASSERT(0 <= i && i < 4);
	return VeVector4(f[0][i], f[1][i], f[2][i], f[3][i]);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::Row(VeUInt32 i, const VeVector4& kVec) noexcept
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
	VE_VECTOR quat = VeLoadFloat4A(&kQuat.v);
	VE_MATRIX4X3 mat = VeMatrixRotationQuaternion(quat);
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::SetRotate(VeFloat32 f32Pitch,
	VeFloat32 f32Yaw, VeFloat32 f32Roll) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixRotationRollPitchYaw(f32Pitch, f32Yaw, f32Roll);
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
void VeMatrix4X3::SetRotate(const VeVector3& kEulerianAngles) noexcept
{
	VE_VECTOR vec = VeLoadFloat3(&kEulerianAngles.v);
	VE_MATRIX4X3 mat = VeMatrixRotationRollPitchYawFromVector(vec);
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::SetTranslation(VeFloat32 x, VeFloat32 y,
	VeFloat32 z) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixTranslation(x, y, z);
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::Mul(const VeMatrix4X3& kMat0,
	const VeMatrix4X3& kMat1) noexcept
{
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&kMat0.m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMat1.m);
	mat0 = VeMatrixMultiply(mat0, mat1);
	VeStoreFloat4x3A(&m, mat0);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::PreMul(const VeMatrix4X3& kMat) noexcept
{
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&kMat.m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&m);
	mat0 = VeMatrixMultiply(mat0, mat1);
	VeStoreFloat4x3A(&m, mat0);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::PostMul(const VeMatrix4X3& kMat) noexcept
{
	VE_MATRIX4X3 mat0 = VeLoadFloat4x3A(&m);
	VE_MATRIX4X3 mat1 = VeLoadFloat4x3A(&kMat.m);
	mat0 = VeMatrixMultiply(mat0, mat1);
	VeStoreFloat4x3A(&m, mat0);
}
//--------------------------------------------------------------------------
inline VeMatrix4X3 VeMatrix4X3::GetInverse() const noexcept
{
	VeMatrix4X3 kRes;
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&m);
	mat = VeMatrixInverse(mat);
	VeStoreFloat4x3A(&kRes.m, mat);
	return kRes;
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::Inverse() noexcept
{
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&m);
	mat = VeMatrixInverse(mat);
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::Inverse(const VeMatrix4X3& kMat) noexcept
{
	VeMatrix4X3 kRes;
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&kMat.m);
	mat = VeMatrixInverse(mat);
	VeStoreFloat4x3A(&m, mat);
}
//--------------------------------------------------------------------------
inline VeVector4 VeMatrix4X3::ApplyVector(
	const VeVector4& kVector) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(&kVector.v);
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&m);
	vec = VeVector4Transform(vec, mat);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::ApplyVector(VeVector4& kOut,
	const VeVector4& kIn) const noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(&kIn.v);
	VE_MATRIX4X3 mat = VeLoadFloat4x3A(&m);
	vec = VeVector4Transform(vec, mat);
	VeStoreFloat4A(&kOut.v, vec);
}
//--------------------------------------------------------------------------
inline VeFloat32& VeMatrix::operator () (VeUInt32 u32Col,
	VeUInt32 u32Row) noexcept
{
	VE_ASSERT(u32Col >= 0 && u32Col < 4 && u32Row >= 0 && u32Row < 4);
	return f[u32Col][u32Row];
}
//--------------------------------------------------------------------------
inline const VeFloat32& VeMatrix::operator () (VeUInt32 u32Col,
	VeUInt32 u32Row) const noexcept
{
	VE_ASSERT(u32Col >= 0 && u32Col < 4 && u32Row >= 0 && u32Row < 4);
	return f[u32Col][u32Row];
}
//--------------------------------------------------------------------------
inline VeVector4& VeMatrix::operator [] (VeUInt32 i) noexcept
{
	VE_ASSERT(i >= 0 && i < 4);
	return vec[i];
}
//--------------------------------------------------------------------------
inline const VeVector4& VeMatrix::operator [] (VeUInt32 i) const noexcept
{
	VE_ASSERT(i >= 0 && i < 4);
	return vec[i];
}
//--------------------------------------------------------------------------
inline VeMatrix& VeMatrix::operator = (const VeMatrix& kMatrix) noexcept
{
	VE_MATRIX mat = VeLoadFloat4x4A(&kMatrix.m);
	VeStoreFloat4x4A(&m, mat);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix& VeMatrix::operator += (const VeMatrix& kMatrix) noexcept
{
	VE_MATRIX mat0 = VeLoadFloat4x4A(&m);
	VE_MATRIX mat1 = VeLoadFloat4x4A(&kMatrix.m);
	mat0.r[0] = VeVectorAdd(mat0.r[0], mat1.r[0]);
	mat0.r[1] = VeVectorAdd(mat0.r[1], mat1.r[1]);
	mat0.r[2] = VeVectorAdd(mat0.r[2], mat1.r[2]);
	mat0.r[3] = VeVectorAdd(mat0.r[3], mat1.r[3]);
	VeStoreFloat4x4A(&m, mat0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix& VeMatrix::operator -= (const VeMatrix& kMatrix) noexcept
{
	VE_MATRIX mat0 = VeLoadFloat4x4A(&m);
	VE_MATRIX mat1 = VeLoadFloat4x4A(&kMatrix.m);
	mat0.r[0] = VeVectorSubtract(mat0.r[0], mat1.r[0]);
	mat0.r[1] = VeVectorSubtract(mat0.r[1], mat1.r[1]);
	mat0.r[2] = VeVectorSubtract(mat0.r[2], mat1.r[2]);
	mat0.r[3] = VeVectorSubtract(mat0.r[3], mat1.r[3]);
	VeStoreFloat4x4A(&m, mat0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix& VeMatrix::operator *= (const VeMatrix& kMatrix) noexcept
{
	VE_MATRIX mat0 = VeLoadFloat4x4A(&m);
	VE_MATRIX mat1 = VeLoadFloat4x4A(&kMatrix.m);
	mat0 = VeMatrixMultiply(mat0, mat1);
	VeStoreFloat4x4A(&m, mat0);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix& VeMatrix::operator *= (VeFloat32 f32Scale) noexcept
{
	VE_MATRIX mat = VeLoadFloat4x4A(&m);
	mat.r[0] = VeVectorScale(mat.r[0], f32Scale);
	mat.r[1] = VeVectorScale(mat.r[1], f32Scale);
	mat.r[2] = VeVectorScale(mat.r[2], f32Scale);
	mat.r[3] = VeVectorScale(mat.r[3], f32Scale);
	VeStoreFloat4x4A(&m, mat);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix& VeMatrix::operator /= (VeFloat32 f32Scale) noexcept
{
	VE_MATRIX mat = VeLoadFloat4x4A(&m);
	VE_VECTOR vec = VeVectorReplicate(f32Scale);
	vec = VeVectorReciprocal(vec);
	mat.r[0] = VeVectorMultiply(mat.r[0], vec);
	mat.r[1] = VeVectorMultiply(mat.r[1], vec);
	mat.r[2] = VeVectorMultiply(mat.r[2], vec);
	mat.r[3] = VeVectorMultiply(mat.r[3], vec);
	VeStoreFloat4x4A(&m, mat);
	return *this;
}
//--------------------------------------------------------------------------
inline VeMatrix VeMatrix::operator + () const noexcept
{
	VeMatrix kRes;
	VE_MATRIX mat = VeLoadFloat4x4A(&m);
	VeStoreFloat4x4A(&kRes.m, mat);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix VeMatrix::operator - () const noexcept
{
	VeMatrix kRes;
	VE_MATRIX mat = VeLoadFloat4x4A(&m);
	mat.r[0] = VeVectorNegate(mat.r[0]);
	mat.r[1] = VeVectorNegate(mat.r[1]);
	mat.r[2] = VeVectorNegate(mat.r[2]);
	mat.r[3] = VeVectorNegate(mat.r[3]);
	VeStoreFloat4x4A(&kRes.m, mat);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix VeMatrix::operator + (const VeMatrix& kMatrix) const noexcept
{
	VeMatrix kRes;
	VE_MATRIX mat0 = VeLoadFloat4x4A(&m);
	VE_MATRIX mat1 = VeLoadFloat4x4A(&kMatrix.m);
	mat0.r[0] = VeVectorAdd(mat0.r[0], mat1.r[0]);
	mat0.r[1] = VeVectorAdd(mat0.r[1], mat1.r[1]);
	mat0.r[2] = VeVectorAdd(mat0.r[2], mat1.r[2]);
	mat0.r[3] = VeVectorAdd(mat0.r[3], mat1.r[3]);
	VeStoreFloat4x4A(&kRes.m, mat0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix VeMatrix::operator - (const VeMatrix& kMatrix) const noexcept
{
	VeMatrix kRes;
	VE_MATRIX mat0 = VeLoadFloat4x4A(&m);
	VE_MATRIX mat1 = VeLoadFloat4x4A(&kMatrix.m);
	mat0.r[0] = VeVectorSubtract(mat0.r[0], mat1.r[0]);
	mat0.r[1] = VeVectorSubtract(mat0.r[1], mat1.r[1]);
	mat0.r[2] = VeVectorSubtract(mat0.r[2], mat1.r[2]);
	mat0.r[3] = VeVectorSubtract(mat0.r[3], mat1.r[3]);
	VeStoreFloat4x4A(&kRes.m, mat0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix VeMatrix::operator * (const VeMatrix& kMatrix) const noexcept
{
	VeMatrix kRes;
	VE_MATRIX mat0 = VeLoadFloat4x4A(&m);
	VE_MATRIX mat1 = VeLoadFloat4x4A(&kMatrix.m);
	mat0 = VeMatrixMultiply(mat0, mat1);
	VeStoreFloat4x4A(&kRes.m, mat0);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 VeMatrix::operator * (const VeVector4& kVector) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(&kVector.v);
	VE_MATRIX mat = VeLoadFloat4x4A(&m);
	vec = VeVector4Transform(vec, mat);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix VeMatrix::operator * (VeFloat32 f32Scale) const noexcept
{
	VeMatrix kRes;
	VE_MATRIX mat = VeLoadFloat4x4A(&m);
	mat.r[0] = VeVectorScale(mat.r[0], f32Scale);
	mat.r[1] = VeVectorScale(mat.r[1], f32Scale);
	mat.r[2] = VeVectorScale(mat.r[2], f32Scale);
	mat.r[3] = VeVectorScale(mat.r[3], f32Scale);
	VeStoreFloat4x4A(&kRes.m, mat);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix VeMatrix::operator / (VeFloat32 f32Scale) const noexcept
{
	VeMatrix kRes;
	VE_MATRIX mat = VeLoadFloat4x4A(&m);
	VE_VECTOR vec = VeVectorReplicate(f32Scale);
	vec = VeVectorReciprocal(vec);
	mat.r[0] = VeVectorMultiply(mat.r[0], vec);
	mat.r[1] = VeVectorMultiply(mat.r[1], vec);
	mat.r[2] = VeVectorMultiply(mat.r[2], vec);
	mat.r[3] = VeVectorMultiply(mat.r[3], vec);
	VeStoreFloat4x4A(&kRes.m, mat);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeMatrix operator * (VeFloat32 f32Scale,
	const VeMatrix& kMatrix) noexcept
{
	VeMatrix kRes;
	VE_MATRIX mat = VeLoadFloat4x4A(&kMatrix.m);
	mat.r[0] = VeVectorScale(mat.r[0], f32Scale);
	mat.r[1] = VeVectorScale(mat.r[1], f32Scale);
	mat.r[2] = VeVectorScale(mat.r[2], f32Scale);
	mat.r[3] = VeVectorScale(mat.r[3], f32Scale);
	VeStoreFloat4x4A(&kRes.m, mat);
	return kRes;
}
//--------------------------------------------------------------------------
inline VeVector4 operator * (const VeVector4& kVector,
	const VeMatrix& kMatrix) noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(&kVector.v);
	VE_MATRIX mat = VeLoadFloat4x4A(&kMatrix.m);
	vec = VeVector4Transform(vec, mat);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline bool VeMatrix::operator == (const VeMatrix& kMatrix) const noexcept
{
	VE_MATRIX mat0 = VeLoadFloat4x4A(&m);
	VE_MATRIX mat1 = VeLoadFloat4x4A(&kMatrix.m);
	return VeVector4Equal(mat0.r[0], mat1.r[0])
		&& VeVector4Equal(mat0.r[1], mat1.r[1])
		&& VeVector4Equal(mat0.r[2], mat1.r[2])
		&& VeVector4Equal(mat0.r[3], mat1.r[3]);
}
//--------------------------------------------------------------------------
inline bool VeMatrix::operator != (const VeMatrix& kMatrix) const noexcept
{
	VE_MATRIX mat0 = VeLoadFloat4x4A(&m);
	VE_MATRIX mat1 = VeLoadFloat4x4A(&kMatrix.m);
	return VeVector4NotEqual(mat0.r[0], mat1.r[0])
		|| VeVector4NotEqual(mat0.r[1], mat1.r[1])
		|| VeVector4NotEqual(mat0.r[2], mat1.r[2])
		|| VeVector4NotEqual(mat0.r[3], mat1.r[3]);
}
//--------------------------------------------------------------------------
inline VeVector4& VeMatrix::Col(VeUInt32 i) noexcept
{
	VE_ASSERT(i >= 0 && i < 4);
	return *(VeVector4*)&v[i];
}
//--------------------------------------------------------------------------
inline const VeVector4& VeMatrix::Col(VeUInt32 i) const noexcept
{
	VE_ASSERT(i >= 0 && i < 4);
	return *(const VeVector4*)&v[i];
}
//--------------------------------------------------------------------------
inline void VeMatrix::Col(VeUInt32 i, const VeVector4& kVec) noexcept
{
	VE_ASSERT(i >= 0 && i < 4);
	VE_VECTOR ve = VeLoadFloat4A(&kVec.v);
	VeStoreFloat4A(&v[i], ve);
}
//--------------------------------------------------------------------------
inline VeVector4 VeMatrix::Row(VeUInt32 i) const noexcept
{
	VE_ASSERT(0 <= i && i < 4);
	return VeVector4(f[0][i], f[1][i], f[2][i], f[3][i]);
}
//--------------------------------------------------------------------------
inline void VeMatrix::Row(VeUInt32 i, const VeVector4& kVec) noexcept
{
	VE_ASSERT(0 <= i && i < 4);
	f[0][i] = kVec.x;
	f[1][i] = kVec.y;
	f[2][i] = kVec.z;
	f[3][i] = kVec.w;
}
//--------------------------------------------------------------------------
inline void VeMatrix::SetZero() noexcept
{
	VE_VECTOR vec = VeVectorZero();
	VeStoreFloat4A(&v[0], vec);
	VeStoreFloat4A(&v[1], vec);
	VeStoreFloat4A(&v[2], vec);
	VeStoreFloat4A(&v[3], vec);
}
//--------------------------------------------------------------------------
inline void VeMatrix::SetIdentity() noexcept
{
	VE_MATRIX mat = VeMatrixIdentity();
	VeStoreFloat4x4A(&m, mat);
}
//--------------------------------------------------------------------------
inline void VeMatrix::SetScale(VeFloat32 x, VeFloat32 y,
	VeFloat32 z) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixScaling(x, y, z);
	VeStoreFloat4A(&v[0], mat.r[0]);
	VeStoreFloat4A(&v[1], mat.r[1]);
	VeStoreFloat4A(&v[2], mat.r[2]);
	VeStoreFloat4A(&v[3], g_MathIdentityR3);
}
//--------------------------------------------------------------------------
inline void VeMatrix::SetRotateX(VeFloat32 f32Angle) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixRotationX(f32Angle);
	VeStoreFloat4A(&v[0], mat.r[0]);
	VeStoreFloat4A(&v[1], mat.r[1]);
	VeStoreFloat4A(&v[2], mat.r[2]);
	VeStoreFloat4A(&v[3], g_MathIdentityR3);
}
//--------------------------------------------------------------------------
inline void VeMatrix::SetRotateY(VeFloat32 f32Angle) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixRotationY(f32Angle);
	VeStoreFloat4A(&v[0], mat.r[0]);
	VeStoreFloat4A(&v[1], mat.r[1]);
	VeStoreFloat4A(&v[2], mat.r[2]);
	VeStoreFloat4A(&v[3], g_MathIdentityR3);
}
//--------------------------------------------------------------------------
inline void VeMatrix::SetRotateZ(VeFloat32 f32Angle) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixRotationZ(f32Angle);
	VeStoreFloat4A(&v[0], mat.r[0]);
	VeStoreFloat4A(&v[1], mat.r[1]);
	VeStoreFloat4A(&v[2], mat.r[2]);
	VeStoreFloat4A(&v[3], g_MathIdentityR3);
}
//--------------------------------------------------------------------------
inline void VeMatrix::SetRotate(const VeQuaternion& kQuat) noexcept
{
	VE_VECTOR quat = VeLoadFloat4A(&kQuat.v);
	VE_MATRIX4X3 mat = VeMatrixRotationQuaternion(quat);
	VeStoreFloat4A(&v[0], mat.r[0]);
	VeStoreFloat4A(&v[1], mat.r[1]);
	VeStoreFloat4A(&v[2], mat.r[2]);
	VeStoreFloat4A(&v[3], g_MathIdentityR3);
}
//--------------------------------------------------------------------------
inline void VeMatrix::SetRotate(VeFloat32 f32Pitch, VeFloat32 f32Yaw,
	VeFloat32 f32Roll) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixRotationRollPitchYaw(f32Pitch, f32Yaw, f32Roll);
	VeStoreFloat4A(&v[0], mat.r[0]);
	VeStoreFloat4A(&v[1], mat.r[1]);
	VeStoreFloat4A(&v[2], mat.r[2]);
	VeStoreFloat4A(&v[3], g_MathIdentityR3);
}
//--------------------------------------------------------------------------
inline void VeMatrix::SetRotate(const VeVector3& kEulerianAngles) noexcept
{
	VE_VECTOR vec = VeLoadFloat3(&kEulerianAngles.v);
	VE_MATRIX4X3 mat = VeMatrixRotationRollPitchYawFromVector(vec);
	VeStoreFloat4A(&v[0], mat.r[0]);
	VeStoreFloat4A(&v[1], mat.r[1]);
	VeStoreFloat4A(&v[2], mat.r[2]);
	VeStoreFloat4A(&v[3], g_MathIdentityR3);
}
//--------------------------------------------------------------------------
inline void VeMatrix::SetTranslation(VeFloat32 x, VeFloat32 y,
	VeFloat32 z) noexcept
{
	VE_MATRIX4X3 mat = VeMatrixTranslation(x, y, z);
	VeStoreFloat4A(&v[0], mat.r[0]);
	VeStoreFloat4A(&v[1], mat.r[1]);
	VeStoreFloat4A(&v[2], mat.r[2]);
	VeStoreFloat4A(&v[3], g_MathIdentityR3);
}
//--------------------------------------------------------------------------
inline void VeMatrix::Mul(const VeMatrix& kMat0,
	const VeMatrix& kMat1) noexcept
{
	VE_MATRIX mat0 = VeLoadFloat4x4A(&kMat0.m);
	VE_MATRIX mat1 = VeLoadFloat4x4A(&kMat1.m);
	mat0 = VeMatrixMultiply(mat0, mat1);
	VeStoreFloat4x4A(&m, mat0);
}
//--------------------------------------------------------------------------
inline void VeMatrix::PreMul(const VeMatrix& kMat) noexcept
{
	VE_MATRIX mat0 = VeLoadFloat4x4A(&kMat.m);
	VE_MATRIX mat1 = VeLoadFloat4x4A(&m);
	mat0 = VeMatrixMultiply(mat0, mat1);
	VeStoreFloat4x4A(&m, mat0);
}
//--------------------------------------------------------------------------
inline void VeMatrix::PostMul(const VeMatrix& kMat) noexcept
{
	VE_MATRIX mat0 = VeLoadFloat4x4A(&m);
	VE_MATRIX mat1 = VeLoadFloat4x4A(&kMat.m);
	mat0 = VeMatrixMultiply(mat0, mat1);
	VeStoreFloat4x4A(&m, mat0);
}
//--------------------------------------------------------------------------
inline VeVector4 VeMatrix::ApplyVector(
	const VeVector4& kVector) const noexcept
{
	VeVector4 kRes;
	VE_VECTOR vec = VeLoadFloat4A(&kVector.v);
	VE_MATRIX mat = VeLoadFloat4x4A(&m);
	vec = VeVector4Transform(vec, mat);
	VeStoreFloat4A(&kRes.v, vec);
	return kRes;
}
//--------------------------------------------------------------------------
inline void VeMatrix::ApplyVector(VeVector4& kOut,
	const VeVector4& kIn) const noexcept
{
	VE_VECTOR vec = VeLoadFloat4A(&kIn.v);
	VE_MATRIX mat = VeLoadFloat4x4A(&m);
	vec = VeVector4Transform(vec, mat);
	VeStoreFloat4A(&kOut.v, vec);	
}
//--------------------------------------------------------------------------
