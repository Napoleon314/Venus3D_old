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
inline VeMatrix4X3::operator VE_FLOAT4X3A* () noexcept
{
	return (VE_FLOAT4X3A*)this;
}
//--------------------------------------------------------------------------
inline VeMatrix4X3::operator const VE_FLOAT4X3A* () const noexcept
{
	return (const VE_FLOAT4X3A*)this;
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
	return VeVector4(m[0][i], m[1][i], m[2][i], m[3][i]);
}
//--------------------------------------------------------------------------
inline void VeMatrix4X3::Col(VeUInt32 i, const VeVector4& kVec) noexcept
{
	VE_ASSERT(0 <= i && i < 4);
	m[0][i] = kVec.x;
	m[1][i] = kVec.y;
	m[2][i] = kVec.z;
	m[3][i] = kVec.w;
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
	VeStoreFloat4x3A(*this, mat);
}
//--------------------------------------------------------------------------
