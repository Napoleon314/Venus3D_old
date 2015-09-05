////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMath.h
//  Created:     2015/07/27 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math.h>
#include <float.h>

#define VE_MAXFLOAT_F			(3.402823466e+38F)
#define VE_MINFLOAT_F			(0.00000001F)
#define VE_MATH_E_F				(2.7182818284590452354f)
#define VE_MATH_LOG2E_F			(1.4426950408889634074f)
#define VE_MATH_LOG10E_F		(0.43429448190325182765f)
#define VE_MATH_LN2_F			(0.69314718055994530942f)
#define VE_MATH_LN10_F			(2.30258509299404568402f)
#define VE_MATH_PI_F			(3.14159265358979323846f)
#define VE_MATH_2PI_F			(6.28318530717958647692f)
#define VE_MATH_3PI_F			(9.42477796076937971538f)
#define VE_MATH_PI_2_F			(1.57079632679489661923f)
#define VE_MATH_PI_4_F			(0.78539816339744830962f)
#define VE_MATH_3PI_4_F			(2.3561944901923448370E0f)
#define VE_MATH_SQRTPI_F		(1.77245385090551602792981f)
#define VE_MATH_1_PI_F			(0.31830988618379067154f)
#define VE_MATH_1_2PI_F			(0.159154943f)
#define VE_MATH_2_PI_F			(0.63661977236758134308f)
#define VE_MATH_2_SQRTPI_F		(1.12837916709551257390f)
#define VE_MATH_SQRT2_F			(1.41421356237309504880f)
#define VE_MATH_SQRT1_2_F		(0.70710678118654752440f)
#define VE_MATH_LN2LO_F			(1.9082149292705877000E-10f)
#define VE_MATH_LN2HI_F			(6.9314718036912381649E-1f)
#define VE_MATH_SQRT3_F		   	(1.73205080756887719000f)
#define VE_MATH_IVLN10_F		(0.43429448190325182765f)
#define VE_MATH_LOG2_E_F		(0.693147180559945309417f)
#define VE_MATH_INVLN2_F		(1.4426950408889633870E0f)
#define VE_MATH_EPSILON_F		(0.0004f)

#define VeAtanf atanf
#define VeCosf cosf
#define VeSinf sinf
#define VeTanf tanf
#define VeTanhf tanhf
#define VeFrexpf frexpf
#define VeModff modff
#define VeCeilf ceilf
#define VeFabsf fabsf
#define VeFloorf floorf

#define VeAcosf acosf
#define VeAsinf asinf
#define VeAtan2f atan2f
#define VeCoshf coshf
#define VeSinhf sinhf
#define VeExpf expf
#define VeLdexpf ldexpf
#define VeLogf logf
#define VeLog10f log10f
#define VePowf powf
#define VeFmodf fmodf

#define VeAtan atan
#define VeCos cos
#define VeSin sin
#define VeTan tan
#define VeTanh tanh
#define VeFrexp frexp
#define VeModf modf
#define VeCeil ceil
#define VeFabs fabs
#define VeFloor floor

#define VeAcos acos
#define VeAsin asin
#define VeAtan2 atan2
#define VeCosh cosh
#define VeSinh sinh
#define VeExp exp
#define VeLdexp ldexp
#define VeLog10 log10
#define VePow pow
#define VeFmod fmod

#define VeRoundf roundf

#define VeDegreeToRadiantf(D)	((D) * 0.01745329251994329576923690768f)
#define VeRadiantToDegreef(R)	((R) * 57.2957795130823208767981548f)
#define VeFracf(number)			((number)-(VeFloat32)((VeInt32)(number)))

#define VeClampEx(min,val,max)	((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))
#define VeClamp(mag,val)		(VeClampEx(-(mag),(val),(mag)))
#define VeSat(a) VeClampEx(0.0f, a, 1.0f)

#define VeAbs(x) ((x) < 0 ? -(x) : (x))

#define VeIntervalMapping(x,src_a,src_b,dst_a,dst_b) \
	(((dst_b) - (dst_a)) * ((x) - (src_a)) / ((src_b) - (src_a)) + dst_a)

#define VeLerp(t,a,b) ((a) + ((b) - (a)) * (t))

#ifdef VE_USE_FAST_SQRT
#	define VeSqrtf VeFastSqrtf
#	define VeInvSqrtf VeFastInvSqrtf
#else
#	define VeSqrtf sqrtf
#	define VeInvSqrtf(v) (1.0f / sqrtf((v)))
#endif

#define VE_FLOAT_POINT(p) ((VeFloat32*)p)
#define VE_FLOAT_POINT_CONST(p) ((const VeFloat32*)p)
#define VE_FLOAT_POINT_THIS ((VeFloat32*)this)

inline void VeSinCosf(VeFloat32& f32Sin, VeFloat32& f32Cos, VeFloat32 f32Angle) noexcept
{
	f32Sin = VeSinf(f32Angle);
	f32Cos = VeCosf(f32Angle);
}

inline VeFloat32 VeSignf(VeFloat32 f32Value) noexcept
{
	return f32Value > 0 ? 1.0f : (f32Value < 0 ? -1.0f : 0.0f);
}

inline VeFloat32 VeFastInvSqrtf(VeFloat32 f32Value) noexcept
{
	union VeFloatBin
	{
		VeFloat32 f;
		VeInt32 i;
	};
	VeFloat32 f32Half = 0.5f * f32Value;
	VeFloatBin kValue;
	kValue.f = f32Value;
	kValue.i = 0x5f3759df - (kValue.i >> 1);				//What the fuck!
	f32Value = kValue.f;
	f32Value = f32Value * (1.5f - f32Half * f32Value * f32Value);
	return f32Value;
}

inline VeFloat32 VeFastSqrtf(VeFloat32 f32Value) noexcept
{
	return VeFastInvSqrtf(f32Value) * f32Value;
}

inline bool VeAlmostEqual(const VeFloat32 f1, const VeFloat32 f2,
	const VeFloat32 f32Epsilon) noexcept
{
	return VeFabsf(f1 - f2) < f32Epsilon;
}

inline bool VeAlmostEqual(const VeFloat64 f1, const VeFloat64 f2,
	const VeFloat64 f64Epsilon) noexcept
{
	return VeFabs(f1 - f2) < f64Epsilon;
}

inline bool VeAlmostZero(const VeFloat32 f, const VeFloat32 f32Epsilon) noexcept
{
	return f < f32Epsilon && f > -f32Epsilon;
}

inline bool VeAlmostZero(const VeFloat64 f, const VeFloat64 f64Epsilon) noexcept
{
	return f < f64Epsilon && f > -f64Epsilon;
}

inline bool VeIsValid(VeFloat32 f32Value) noexcept
{
	VeUInt32 u32Value = *(VeUInt32*)&f32Value;
	return (u32Value & 0x7f800000) != 0x7f800000;
}

VE_MAIN_API void VeSrand(VeUInt32 u32Seed) noexcept;

VE_MAIN_API VeInt32 VeRand() noexcept;

#include "VeMathSIMD.inl"
#include "VeMathConvert.inl"
#include "VeMathVector.inl"
#include "VeMathMatrix.inl"
#include "VeMathMisc.inl"
