////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMathSSE.inl
//  Created:     2015/08/08 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline void VeSetv4f(VeFloat32* p, VeFloat32 w) noexcept
{
	VE_ASSERT(!(VeSizeT(p) & 0xf));
	__m128 dst = _mm_load_ps1(&w);
	_mm_store_ps(p, dst);
}
//--------------------------------------------------------------------------
inline bool VeEqualv4f(const VeFloat32* a, const VeFloat32* b) noexcept
{
	VE_ASSERT(!(VeSizeT(a) & 0xf));
	VE_ASSERT(!(VeSizeT(b) & 0xf));
	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}
//--------------------------------------------------------------------------
inline void VeAddv4f(VeFloat32* dst, const VeFloat32* a,
	const VeFloat32* b) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(a) & 0xf));
	VE_ASSERT(!(VeSizeT(b) & 0xf));
	__m128 m1 = _mm_load_ps(a);
	__m128 m2 = _mm_load_ps(b);
	m1 = _mm_add_ps(m1, m2);
	_mm_store_ps(dst, m1);
}
//--------------------------------------------------------------------------
inline void VeSubv4f(VeFloat32* dst, const VeFloat32* a,
	const VeFloat32* b) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(a) & 0xf));
	VE_ASSERT(!(VeSizeT(b) & 0xf));
	__m128 m1 = _mm_load_ps(a);
	__m128 m2 = _mm_load_ps(b);
	m1 = _mm_sub_ps(m1, m2);
	_mm_store_ps(dst, m1);
}
//--------------------------------------------------------------------------
inline void VeMulv4f(VeFloat32* dst, const VeFloat32* v,
	VeFloat32 s) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(v) & 0xf));
	__m128 m1 = _mm_load_ps(v);
	__m128 m2 = _mm_load_ps1(&s);
	m1 = _mm_mul_ps(m1, m2);
	_mm_store_ps(dst, m1);
}
//--------------------------------------------------------------------------
inline void VeMulv4f(VeFloat32* dst, const VeFloat32* a,
	const VeFloat32* b) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(a) & 0xf));
	VE_ASSERT(!(VeSizeT(b) & 0xf));
	__m128 m1 = _mm_load_ps(a);
	__m128 m2 = _mm_load_ps(b);
	m1 = _mm_mul_ps(m1, m2);
	_mm_store_ps(dst, m1);
}
//--------------------------------------------------------------------------
inline void VeDivv4f(VeFloat32* dst, const VeFloat32* v,
	VeFloat32 s) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(v) & 0xf));
	__m128 m1 = _mm_load_ps(v);
	__m128 m2 = _mm_load_ps1(&s);
	m1 = _mm_div_ps(m1, m2);
	_mm_store_ps(dst, m1);
}
//--------------------------------------------------------------------------
inline void VeDivv4f(VeFloat32* dst, const VeFloat32* a,
	const VeFloat32* b) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(a) & 0xf));
	VE_ASSERT(!(VeSizeT(b) & 0xf));
	__m128 m1 = _mm_load_ps(a);
	__m128 m2 = _mm_load_ps(b);
	m1 = _mm_div_ps(m1, m2);
	_mm_store_ps(dst, m1);
}
//--------------------------------------------------------------------------
inline void VeClampv4f(VeFloat32* dst, const VeFloat32* src) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(src) & 0xf));
	__m128 m1 = _mm_load_ps(dst);
	__m128 m2 = _mm_setzero_ps();
	m1 = _mm_max_ps(m1, m2);
	m2 = _mm_set_ps1(1.0f);
	m1 = _mm_min_ps(m1, m2);
	_mm_store_ps(dst, m1);
}
//--------------------------------------------------------------------------
