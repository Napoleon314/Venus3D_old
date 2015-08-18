////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMathSIMD.inl
//  Created:     2015/08/18 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#ifdef VE_PLATFORM_WIN
#	define VE_MATH_CALLCONV __vectorcall
#else
#	define VE_MATH_CALLCONV
#endif

#if defined(VE_CPU_X86) || defined(VE_CPU_X64)
#	define VE_ENABLE_SSE
#elif defined(VE_CPU_ARM) || defined(VE_CPU_ARM64)
#	define VE_ENABLE_NEON
#else
#	define VE_NO_INTRINSICS
#endif

#ifdef VE_ENABLE_SSE
#	include <xmmintrin.h>
#	include <emmintrin.h>
#endif

#define VE_SELECT_0				(0x00000000)
#define VE_SELECT_1				(0xFFFFFFFF)

#define VE_PERMUTE_0X			(0)
#define VE_PERMUTE_0Y			(1)
#define VE_PERMUTE_0Z			(2)
#define VE_PERMUTE_0W			(3)
#define VE_PERMUTE_1X			(4)
#define VE_PERMUTE_1Y			(5)
#define VE_PERMUTE_1Z			(6)
#define VE_PERMUTE_1W			(7)

#define VE_SWIZZLE_X			(0)
#define VE_SWIZZLE_Y			(1)
#define VE_SWIZZLE_Z			(2)
#define VE_SWIZZLE_W			(3)

#define VE_CRMASK_CR6			(0x000000F0)
#define VE_CRMASK_CR6TRUE		(0x00000080)
#define VE_CRMASK_CR6FALSE		(0x00000020)
#define VE_CRMASK_CR6BOUNDS		VE_CRMASK_CR6FALSE
#define VE_CACHE_LINE_SIZE		(64)

inline bool VeComparisonAllTrue(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6TRUE) == VE_CRMASK_CR6TRUE); }
inline bool VeComparisonAnyTrue(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6FALSE) != VE_CRMASK_CR6FALSE); }
inline bool VeComparisonAllFalse(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6FALSE) == VE_CRMASK_CR6FALSE); }
inline bool VeComparisonAnyFalse(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6TRUE) != VE_CRMASK_CR6TRUE); }
inline bool VeComparisonMixed(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6) == 0); }
inline bool VeComparisonAllInBounds(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6BOUNDS) == VE_CRMASK_CR6BOUNDS); }
inline bool VeComparisonAnyOutOfBounds(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6BOUNDS) != VE_CRMASK_CR6BOUNDS); }

#if defined(VE_NO_INTRINSICS)

struct __vector4
{
	union
	{
		VeFloat32		vector4_f32[4];
		VeUInt32		vector4_u32[4];
	};
};

typedef VeUInt32 __vector4i[4];

#endif

#if defined(VE_ENABLE_SSE)

typedef __m128 VE_VECTOR;

#else

typedef __vector4 VE_VECTOR;

#endif

#ifndef VE_NO_INTRINSICS

typedef const VE_VECTOR VE_FVECTOR;
typedef const VE_VECTOR VE_GVECTOR;
typedef const VE_VECTOR VE_HVECTOR;

#else

typedef const VE_VECTOR& VE_FVECTOR;
typedef const VE_VECTOR& VE_GVECTOR;
typedef const VE_VECTOR& VE_HVECTOR;

#endif

typedef const VE_VECTOR& VE_CVECTOR;

struct alignas(16) VE_VECTORF32
{
	union
	{
		VeFloat32 f[4];
		VE_VECTOR v;
	};

	inline operator VE_VECTOR() const noexcept { return v; }
	inline operator const VeFloat32*() const noexcept { return f; }
#	if defined(VE_ENABLE_SSE)
	inline operator __m128i() const noexcept { return _mm_castps_si128(v); }
	inline operator __m128d() const noexcept { return _mm_castps_pd(v); }
#	endif
};

struct alignas(16) VE_VECTORI32
{
	union
	{
		VeInt32 i[4];
		VE_VECTOR v;
	};

	inline operator VE_VECTOR() const noexcept { return v; }
#	if defined(VE_ENABLE_SSE)
	inline operator __m128i() const noexcept { return _mm_castps_si128(v); }
	inline operator __m128d() const noexcept { return _mm_castps_pd(v); }
#	endif
};

struct alignas(16) VE_VECTORU8
{
	union
	{
		VeUInt8 u[16];
		VE_VECTOR v;
	};

	inline operator VE_VECTOR() const noexcept { return v; }
#	if defined(VE_ENABLE_SSE)
	inline operator __m128i() const noexcept { return _mm_castps_si128(v); }
	inline operator __m128d() const noexcept { return _mm_castps_pd(v); }
#	endif
};

struct alignas(16) VE_VECTORU32
{
	union
	{
		VeUInt32 u[4];
		VE_VECTOR v;
	};

	inline operator VE_VECTOR() const noexcept { return v; }
#	if defined(VE_ENABLE_SSE)
	inline operator __m128i() const noexcept { return _mm_castps_si128(v); }
	inline operator __m128d() const noexcept { return _mm_castps_pd(v); }
#	endif
};

constexpr VE_VECTORF32 g_MathSinCoefficients0 = { -0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f };
constexpr VE_VECTORF32 g_MathSinCoefficients1 = { -2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/ };
constexpr VE_VECTORF32 g_MathCosCoefficients0 = { -0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f };
constexpr VE_VECTORF32 g_MathCosCoefficients1 = { -2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/ };
constexpr VE_VECTORF32 g_MathTanCoefficients0 = { 1.0f, 0.333333333f, 0.133333333f, 5.396825397e-2f };
constexpr VE_VECTORF32 g_MathTanCoefficients1 = { 2.186948854e-2f, 8.863235530e-3f, 3.592128167e-3f, 1.455834485e-3f };
constexpr VE_VECTORF32 g_MathTanCoefficients2 = { 5.900274264e-4f, 2.391290764e-4f, 9.691537707e-5f, 3.927832950e-5f };
constexpr VE_VECTORF32 g_MathArcCoefficients0 = { +1.5707963050f, -0.2145988016f, +0.0889789874f, -0.0501743046f };
constexpr VE_VECTORF32 g_MathArcCoefficients1 = { +0.0308918810f, -0.0170881256f, +0.0066700901f, -0.0012624911f };
constexpr VE_VECTORF32 g_MathATanCoefficients0 = { -0.3333314528f, +0.1999355085f, -0.1420889944f, +0.1065626393f };
constexpr VE_VECTORF32 g_MathATanCoefficients1 = { -0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f };
constexpr VE_VECTORF32 g_MathATanEstCoefficients0 = { +0.999866f, +0.999866f, +0.999866f, +0.999866f };
constexpr VE_VECTORF32 g_MathATanEstCoefficients1 = { -0.3302995f, +0.180141f, -0.085133f, +0.0208351f };
constexpr VE_VECTORF32 g_MathTanEstCoefficients = { 2.484f, -1.954923183e-1f, 2.467401101f, VE_MATH_1_PI_F };
constexpr VE_VECTORF32 g_MathArcEstCoefficients = { +1.5707288f,-0.2121144f,+0.0742610f,-0.0187293f };
constexpr VE_VECTORF32 g_MathPiConstants0 = { VE_MATH_PI_F, VE_MATH_2PI_F, VE_MATH_1_PI_F, VE_MATH_1_2PI_F };
constexpr VE_VECTORF32 g_MathIdentityR0 = { 1.0f, 0.0f, 0.0f, 0.0f };
constexpr VE_VECTORF32 g_MathIdentityR1 = { 0.0f, 1.0f, 0.0f, 0.0f };
constexpr VE_VECTORF32 g_MathIdentityR2 = { 0.0f, 0.0f, 1.0f, 0.0f };
constexpr VE_VECTORF32 g_MathIdentityR3 = { 0.0f, 0.0f, 0.0f, 1.0f };
constexpr VE_VECTORF32 g_MathNegIdentityR0 = { -1.0f,0.0f, 0.0f, 0.0f };
constexpr VE_VECTORF32 g_MathNegIdentityR1 = { 0.0f,-1.0f, 0.0f, 0.0f };
constexpr VE_VECTORF32 g_MathNegIdentityR2 = { 0.0f, 0.0f,-1.0f, 0.0f };
constexpr VE_VECTORF32 g_MathNegIdentityR3 = { 0.0f, 0.0f, 0.0f,-1.0f };
constexpr VE_VECTORU32 g_MathNegativeZero = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
constexpr VE_VECTORU32 g_MathNegate3 = { 0x80000000, 0x80000000, 0x80000000, 0x00000000 };
constexpr VE_VECTORU32 g_MathMask3 = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };
constexpr VE_VECTORU32 g_MathMaskX = { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 };
constexpr VE_VECTORU32 g_MathMaskY = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 };
constexpr VE_VECTORU32 g_MathMaskZ = { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 };
constexpr VE_VECTORU32 g_MathMaskW = { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF };
constexpr VE_VECTORF32 g_MathOne = { 1.0f, 1.0f, 1.0f, 1.0f };
constexpr VE_VECTORF32 g_MathOne3 = { 1.0f, 1.0f, 1.0f, 0.0f };
constexpr VE_VECTORF32 g_MathZero = { 0.0f, 0.0f, 0.0f, 0.0f };
constexpr VE_VECTORF32 g_MathTwo = { 2.f, 2.f, 2.f, 2.f };
constexpr VE_VECTORF32 g_MathFour = { 4.f, 4.f, 4.f, 4.f };
constexpr VE_VECTORF32 g_MathSix = { 6.f, 6.f, 6.f, 6.f };
constexpr VE_VECTORF32 g_MathNegativeOne = { -1.0f,-1.0f,-1.0f,-1.0f };
constexpr VE_VECTORF32 g_MathOneHalf = { 0.5f, 0.5f, 0.5f, 0.5f };
constexpr VE_VECTORF32 g_MathNegativeOneHalf = { -0.5f,-0.5f,-0.5f,-0.5f };
constexpr VE_VECTORF32 g_MathNegativeTwoPi = { -VE_MATH_2PI_F, -VE_MATH_2PI_F, -VE_MATH_2PI_F, -VE_MATH_2PI_F };
constexpr VE_VECTORF32 g_MathNegativePi = { -VE_MATH_PI_F, -VE_MATH_PI_F, -VE_MATH_PI_F, -VE_MATH_PI_F };
constexpr VE_VECTORF32 g_MathHalfPi = { VE_MATH_PI_2_F, VE_MATH_PI_2_F, VE_MATH_PI_2_F, VE_MATH_PI_2_F };
constexpr VE_VECTORF32 g_MathPi = { VE_MATH_PI_F, VE_MATH_PI_F, VE_MATH_PI_F, VE_MATH_PI_F };
constexpr VE_VECTORF32 g_MathReciprocalPi = { VE_MATH_1_PI_F, VE_MATH_1_PI_F, VE_MATH_1_PI_F, VE_MATH_1_PI_F };
constexpr VE_VECTORF32 g_MathTwoPi = { VE_MATH_2PI_F, VE_MATH_2PI_F, VE_MATH_2PI_F, VE_MATH_2PI_F };
constexpr VE_VECTORF32 g_MathReciprocalTwoPi = { VE_MATH_1_2PI_F, VE_MATH_1_2PI_F, VE_MATH_1_2PI_F, VE_MATH_1_2PI_F };
constexpr VE_VECTORF32 g_MathEpsilon = { 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f };
constexpr VE_VECTORI32 g_MathInfinity = { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 };
constexpr VE_VECTORI32 g_MathQNaN = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };
constexpr VE_VECTORI32 g_MathQNaNTest = { 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF };
constexpr VE_VECTORI32 g_MathAbsMask = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF };
constexpr VE_VECTORI32 g_MathFltMin = { 0x00800000, 0x00800000, 0x00800000, 0x00800000 };
constexpr VE_VECTORI32 g_MathFltMax = { 0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF };
constexpr VE_VECTORU32 g_MathNegOneMask = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
constexpr VE_VECTORU32 g_MathMaskA8R8G8B8 = { 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000 };
constexpr VE_VECTORU32 g_MathFlipA8R8G8B8 = { 0x00000000, 0x00000000, 0x00000000, 0x80000000 };
constexpr VE_VECTORF32 g_MathFixAA8R8G8B8 = { 0.0f,0.0f,0.0f,(VeFloat32)(0x80000000U) };
constexpr VE_VECTORF32 g_MathNormalizeA8R8G8B8 = { 1.0f / (255.0f*(VeFloat32)(0x10000)),1.0f / (255.0f*(VeFloat32)(0x100)),1.0f / 255.0f,1.0f / (255.0f*(VeFloat32)(0x1000000)) };
constexpr VE_VECTORU32 g_MathMaskA2B10G10R10 = { 0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000 };
constexpr VE_VECTORU32 g_MathFlipA2B10G10R10 = { 0x00000200, 0x00080000, 0x20000000, 0x80000000 };
constexpr VE_VECTORF32 g_MathFixAA2B10G10R10 = { -512.0f,-512.0f*(VeFloat32)(0x400),-512.0f*(VeFloat32)(0x100000),(VeFloat32)(0x80000000U) };
constexpr VE_VECTORF32 g_MathNormalizeA2B10G10R10 = { 1.0f / 511.0f,1.0f / (511.0f*(VeFloat32)(0x400)),1.0f / (511.0f*(VeFloat32)(0x100000)),1.0f / (3.0f*(VeFloat32)(0x40000000)) };
constexpr VE_VECTORU32 g_MathMaskX16Y16 = { 0x0000FFFF, 0xFFFF0000, 0x00000000, 0x00000000 };
constexpr VE_VECTORI32 g_MathFlipX16Y16 = { 0x00008000, 0x00000000, 0x00000000, 0x00000000 };
constexpr VE_VECTORF32 g_MathFixX16Y16 = { -32768.0f,0.0f,0.0f,0.0f };
constexpr VE_VECTORF32 g_MathNormalizeX16Y16 = { 1.0f / 32767.0f,1.0f / (32767.0f*65536.0f),0.0f,0.0f };
constexpr VE_VECTORU32 g_MathMaskX16Y16Z16W16 = { 0x0000FFFF, 0x0000FFFF, 0xFFFF0000, 0xFFFF0000 };
constexpr VE_VECTORI32 g_MathFlipX16Y16Z16W16 = { 0x00008000, 0x00008000, 0x00000000, 0x00000000 };
constexpr VE_VECTORF32 g_MathFixX16Y16Z16W16 = { -32768.0f,-32768.0f,0.0f,0.0f };
constexpr VE_VECTORF32 g_MathNormalizeX16Y16Z16W16 = { 1.0f / 32767.0f,1.0f / 32767.0f,1.0f / (32767.0f*65536.0f),1.0f / (32767.0f*65536.0f) };
constexpr VE_VECTORF32 g_MathNoFraction = { 8388608.0f,8388608.0f,8388608.0f,8388608.0f };
constexpr VE_VECTORI32 g_MathMaskByte = { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF };
constexpr VE_VECTORF32 g_MathNegateX = { -1.0f, 1.0f, 1.0f, 1.0f };
constexpr VE_VECTORF32 g_MathNegateY = { 1.0f,-1.0f, 1.0f, 1.0f };
constexpr VE_VECTORF32 g_MathNegateZ = { 1.0f, 1.0f,-1.0f, 1.0f };
constexpr VE_VECTORF32 g_MathNegateW = { 1.0f, 1.0f, 1.0f,-1.0f };
constexpr VE_VECTORU32 g_MathSelect0101 = { VE_SELECT_0, VE_SELECT_1, VE_SELECT_0, VE_SELECT_1 };
constexpr VE_VECTORU32 g_MathSelect1010 = { VE_SELECT_1, VE_SELECT_0, VE_SELECT_1, VE_SELECT_0 };
constexpr VE_VECTORI32 g_MathOneHalfMinusEpsilon = { 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD };
constexpr VE_VECTORU32 g_MathSelect1000 = { VE_SELECT_1, VE_SELECT_0, VE_SELECT_0, VE_SELECT_0 };
constexpr VE_VECTORU32 g_MathSelect1100 = { VE_SELECT_1, VE_SELECT_1, VE_SELECT_0, VE_SELECT_0 };
constexpr VE_VECTORU32 g_MathSelect1110 = { VE_SELECT_1, VE_SELECT_1, VE_SELECT_1, VE_SELECT_0 };
constexpr VE_VECTORU32 g_MathSelect1011 = { VE_SELECT_1, VE_SELECT_0, VE_SELECT_1, VE_SELECT_1 };
constexpr VE_VECTORF32 g_MathFixupY16 = { 1.0f,1.0f / 65536.0f,0.0f,0.0f };
constexpr VE_VECTORF32 g_MathFixupY16W16 = { 1.0f,1.0f,1.0f / 65536.0f,1.0f / 65536.0f };
constexpr VE_VECTORU32 g_MathFlipY = { 0,0x80000000,0,0 };
constexpr VE_VECTORU32 g_MathFlipZ = { 0,0,0x80000000,0 };
constexpr VE_VECTORU32 g_MathFlipW = { 0,0,0,0x80000000 };
constexpr VE_VECTORU32 g_MathFlipYZ = { 0,0x80000000,0x80000000,0 };
constexpr VE_VECTORU32 g_MathFlipZW = { 0,0,0x80000000,0x80000000 };
constexpr VE_VECTORU32 g_MathFlipYW = { 0,0x80000000,0,0x80000000 };
constexpr VE_VECTORI32 g_MathMaskDec4 = { 0x3FF,0x3FF << 10,0x3FF << 20,0x3 << 30 };
constexpr VE_VECTORI32 g_MathXorDec4 = { 0x200,0x200 << 10,0x200 << 20,0 };
constexpr VE_VECTORF32 g_MathAddUDec4 = { 0,0,0,32768.0f*65536.0f };
constexpr VE_VECTORF32 g_MathAddDec4 = { -512.0f,-512.0f*1024.0f,-512.0f*1024.0f*1024.0f,0 };
constexpr VE_VECTORF32 g_MathMulDec4 = { 1.0f,1.0f / 1024.0f,1.0f / (1024.0f*1024.0f),1.0f / (1024.0f*1024.0f*1024.0f) };
constexpr VE_VECTORU32 g_MathMaskByte4 = { 0xFF,0xFF00,0xFF0000,0xFF000000 };
constexpr VE_VECTORI32 g_MathXorByte4 = { 0x80,0x8000,0x800000,0x00000000 };
constexpr VE_VECTORF32 g_MathAddByte4 = { -128.0f,-128.0f*256.0f,-128.0f*65536.0f,0 };
constexpr VE_VECTORF32 g_MathFixUnsigned = { 32768.0f*65536.0f,32768.0f*65536.0f,32768.0f*65536.0f,32768.0f*65536.0f };
constexpr VE_VECTORF32 g_MathMaxInt = { 65536.0f*32768.0f - 128.0f,65536.0f*32768.0f - 128.0f,65536.0f*32768.0f - 128.0f,65536.0f*32768.0f - 128.0f };
constexpr VE_VECTORF32 g_MathMaxUInt = { 65536.0f*65536.0f - 256.0f,65536.0f*65536.0f - 256.0f,65536.0f*65536.0f - 256.0f,65536.0f*65536.0f - 256.0f };
constexpr VE_VECTORF32 g_MathUnsignedFix = { 32768.0f*65536.0f,32768.0f*65536.0f,32768.0f*65536.0f,32768.0f*65536.0f };
constexpr VE_VECTORF32 g_MathsrgbScale = { 12.92f, 12.92f, 12.92f, 1.0f };
constexpr VE_VECTORF32 g_MathsrgbA = { 0.055f, 0.055f, 0.055f, 0.0f };
constexpr VE_VECTORF32 g_MathsrgbA1 = { 1.055f, 1.055f, 1.055f, 1.0f };
constexpr VE_VECTORI32 g_MathExponentBias = { 127, 127, 127, 127 };
constexpr VE_VECTORI32 g_MathSubnormalExponent = { -126, -126, -126, -126 };
constexpr VE_VECTORI32 g_MathNumTrailing = { 23, 23, 23, 23 };
constexpr VE_VECTORI32 g_MathMinNormal = { 0x00800000, 0x00800000, 0x00800000, 0x00800000 };
constexpr VE_VECTORU32 g_MathNegInfinity = { 0xFF800000, 0xFF800000, 0xFF800000, 0xFF800000 };
constexpr VE_VECTORU32 g_MathNegQNaN = { 0xFFC00000, 0xFFC00000, 0xFFC00000, 0xFFC00000 };
constexpr VE_VECTORI32 g_MathBin128 = { 0x43000000, 0x43000000, 0x43000000, 0x43000000 };
constexpr VE_VECTORU32 g_MathBinNeg150 = { 0xC3160000, 0xC3160000, 0xC3160000, 0xC3160000 };
constexpr VE_VECTORI32 g_Math253 = { 253, 253, 253, 253 };
constexpr VE_VECTORF32 g_MathExpEst1 = { -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f };
constexpr VE_VECTORF32 g_MathExpEst2 = { +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f };
constexpr VE_VECTORF32 g_MathExpEst3 = { -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f };
constexpr VE_VECTORF32 g_MathExpEst4 = { +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f };
constexpr VE_VECTORF32 g_MathExpEst5 = { -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f };
constexpr VE_VECTORF32 g_MathExpEst6 = { +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f };
constexpr VE_VECTORF32 g_MathExpEst7 = { -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f };
constexpr VE_VECTORF32 g_MathLogEst0 = { +1.442693f, +1.442693f, +1.442693f, +1.442693f };
constexpr VE_VECTORF32 g_MathLogEst1 = { -0.721242f, -0.721242f, -0.721242f, -0.721242f };
constexpr VE_VECTORF32 g_MathLogEst2 = { +0.479384f, +0.479384f, +0.479384f, +0.479384f };
constexpr VE_VECTORF32 g_MathLogEst3 = { -0.350295f, -0.350295f, -0.350295f, -0.350295f };
constexpr VE_VECTORF32 g_MathLogEst4 = { +0.248590f, +0.248590f, +0.248590f, +0.248590f };
constexpr VE_VECTORF32 g_MathLogEst5 = { -0.145700f, -0.145700f, -0.145700f, -0.145700f };
constexpr VE_VECTORF32 g_MathLogEst6 = { +0.057148f, +0.057148f, +0.057148f, +0.057148f };
constexpr VE_VECTORF32 g_MathLogEst7 = { -0.010578f, -0.010578f, -0.010578f, -0.010578f };
constexpr VE_VECTORF32 g_MathLgE = { +1.442695f, +1.442695f, +1.442695f, +1.442695f };
constexpr VE_VECTORF32 g_MathInvLgE = { +6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f };

#include "VeMathConvert.inl"
