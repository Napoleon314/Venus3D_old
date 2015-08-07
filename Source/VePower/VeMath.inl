////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMath.inl
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
	p[0] = w;
	p[1] = w;
	p[2] = w;
	p[3] = w;
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
	dst[0] = a[0] + b[0];
	dst[1] = a[1] + b[1];
	dst[2] = a[2] + b[2];
	dst[3] = a[3] + b[3];
}
//--------------------------------------------------------------------------
inline void VeSubv4f(VeFloat32* dst, const VeFloat32* a,
	const VeFloat32* b) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(a) & 0xf));
	VE_ASSERT(!(VeSizeT(b) & 0xf));
	dst[0] = a[0] - b[0];
	dst[1] = a[1] - b[1];
	dst[2] = a[2] - b[2];
	dst[3] = a[3] - b[3];
}
//--------------------------------------------------------------------------
inline void VeMulv4f(VeFloat32* dst, const VeFloat32* v,
	VeFloat32 s) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(v) & 0xf));
	dst[0] = v[0] * s;
	dst[1] = v[1] * s;
	dst[2] = v[2] * s;
	dst[3] = v[3] * s;
}
//--------------------------------------------------------------------------
inline void VeMulv4f(VeFloat32* dst, const VeFloat32* a,
	const VeFloat32* b) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(a) & 0xf));
	VE_ASSERT(!(VeSizeT(b) & 0xf));
	dst[0] = a[0] * b[0];
	dst[1] = a[1] * b[1];
	dst[2] = a[2] * b[2];
	dst[3] = a[3] * b[3];
}
//--------------------------------------------------------------------------
inline void VeDivv4f(VeFloat32* dst, const VeFloat32* v,
	VeFloat32 s) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(v) & 0xf));
	s = 1.0f / s;
	dst[0] = v[0] * s;
	dst[1] = v[1] * s;
	dst[2] = v[2] * s;
	dst[3] = v[3] * s;
}
//--------------------------------------------------------------------------
inline void VeDivv4f(VeFloat32* dst, const VeFloat32* a,
	const VeFloat32* b) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(a) & 0xf));
	VE_ASSERT(!(VeSizeT(b) & 0xf));
	dst[0] = a[0] / b[0];
	dst[1] = a[1] / b[1];
	dst[2] = a[2] / b[2];
	dst[3] = a[3] / b[3];
}
//--------------------------------------------------------------------------
inline void VeClampv4f(VeFloat32* dst, const VeFloat32* src) noexcept
{
	VE_ASSERT(!(VeSizeT(dst) & 0xf));
	VE_ASSERT(!(VeSizeT(src) & 0xf));
	dst[0] = VeClampEx(0, src[0], 1);
	dst[1] = VeClampEx(0, src[1], 1);
	dst[2] = VeClampEx(0, src[2], 1);
	dst[3] = VeClampEx(0, src[3], 1);
}
//--------------------------------------------------------------------------
