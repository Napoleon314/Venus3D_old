////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMathSIMD.inl
//  Created:     2015/08/21 by Napoleon
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

//--------------------------------------------------------------------------
#define VE_SELECT_0				(0x00000000)
#define VE_SELECT_1				(0xFFFFFFFF)
//--------------------------------------------------------------------------
#define VE_PERMUTE_0X			(0)
#define VE_PERMUTE_0Y			(1)
#define VE_PERMUTE_0Z			(2)
#define VE_PERMUTE_0W			(3)
#define VE_PERMUTE_1X			(4)
#define VE_PERMUTE_1Y			(5)
#define VE_PERMUTE_1Z			(6)
#define VE_PERMUTE_1W			(7)
//--------------------------------------------------------------------------
#define VE_SWIZZLE_X			(0)
#define VE_SWIZZLE_Y			(1)
#define VE_SWIZZLE_Z			(2)
#define VE_SWIZZLE_W			(3)
//--------------------------------------------------------------------------
#define VE_CRMASK_CR6			(0x000000F0)
#define VE_CRMASK_CR6TRUE		(0x00000080)
#define VE_CRMASK_CR6FALSE		(0x00000020)
#define VE_CRMASK_CR6BOUNDS		VE_CRMASK_CR6FALSE
#define VE_CACHE_LINE_SIZE		(64)
//--------------------------------------------------------------------------
#define VE_STREAM_PS(p,a) _mm_stream_ps(p,a)
#define VE_PERMUTE_PS(v,c) _mm_shuffle_ps(v,v,c)
#define _mm_replicate_x_ps(v)  _mm_shuffle_ps(v,v,_MM_SHUFFLE(0, 0, 0, 0))
#define _mm_replicate_y_ps(v)  _mm_shuffle_ps(v,v,_MM_SHUFFLE(1, 1, 1, 1))
#define _mm_replicate_z_ps(v)  _mm_shuffle_ps(v,v,_MM_SHUFFLE(2, 2, 2, 2))
#define _mm_replicate_w_ps(v)  _mm_shuffle_ps(v,v,_MM_SHUFFLE(3, 3, 3, 3))
#define _mm_madd_ps(a,b,c) _mm_add_ps(_mm_mul_ps(a,b),c)
//--------------------------------------------------------------------------
inline bool VeComparisonAllTrue(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6TRUE) == VE_CRMASK_CR6TRUE); }
inline bool VeComparisonAnyTrue(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6FALSE) != VE_CRMASK_CR6FALSE); }
inline bool VeComparisonAllFalse(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6FALSE) == VE_CRMASK_CR6FALSE); }
inline bool VeComparisonAnyFalse(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6TRUE) != VE_CRMASK_CR6TRUE); }
inline bool VeComparisonMixed(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6) == 0); }
inline bool VeComparisonAllInBounds(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6BOUNDS) == VE_CRMASK_CR6BOUNDS); }
inline bool VeComparisonAnyOutOfBounds(VeUInt32 CR) noexcept { return (((CR)& VE_CRMASK_CR6BOUNDS) != VE_CRMASK_CR6BOUNDS); }
//--------------------------------------------------------------------------
#if defined(VE_NO_INTRINSICS)
//--------------------------------------------------------------------------
struct __vector4
{
	union
	{
		VeFloat32		vector4_f32[4];
		VeUInt32		vector4_u32[4];
	};
};
//--------------------------------------------------------------------------
typedef VeUInt32 __vector4i[4];
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
#if defined(VE_ENABLE_SSE)
typedef __m128 VE_VECTOR;
#else
typedef __vector4 VE_VECTOR;
#endif
//--------------------------------------------------------------------------
#ifndef VE_NO_INTRINSICS
typedef const VE_VECTOR VE_FVECTOR;
typedef const VE_VECTOR VE_GVECTOR;
typedef const VE_VECTOR VE_HVECTOR;
#else
typedef const VE_VECTOR& VE_FVECTOR;
typedef const VE_VECTOR& VE_GVECTOR;
typedef const VE_VECTOR& VE_HVECTOR;
#endif
//--------------------------------------------------------------------------
typedef const VE_VECTOR& VE_CVECTOR;
//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
struct VE_MATRIX4X3;
#ifndef VE_NO_INTRINSICS
typedef const VE_MATRIX4X3 VE_FMATRIX4X3;
#else
typedef const VE_MATRIX4X3& VE_FMATRIX4X3;
#endif
//--------------------------------------------------------------------------
typedef const VE_MATRIX4X3& VE_CMATRIX4X3;
//--------------------------------------------------------------------------
struct alignas(16) VE_MATRIX4X3
{
#	ifdef VE_NO_INTRINSICS
	union
	{
		VE_VECTOR r[3];
		struct
		{
			VeFloat32 _11, _21, _31, _41;
			VeFloat32 _12, _22, _32, _42;
			VeFloat32 _13, _23, _33, _43;
		};
		VeFloat32 m[3][4];
	};
#	else
	VE_VECTOR r[3];
#	endif

	VE_MATRIX4X3() noexcept = default;
	VE_MATRIX4X3(VE_FVECTOR R0, VE_FVECTOR R1, VE_FVECTOR R2) noexcept { r[0] = R0; r[1] = R1; r[2] = R2; }

};
//--------------------------------------------------------------------------
struct VE_MATRIX;
//--------------------------------------------------------------------------
#ifndef VE_NO_INTRINSICS
typedef const VE_MATRIX VE_FMATRIX;
#else
typedef const VE_MATRIX& VE_FMATRIX;
#endif
//--------------------------------------------------------------------------
typedef const VE_MATRIX& VE_CMATRIX;
//--------------------------------------------------------------------------
struct alignas(16) VE_MATRIX
{
#	ifdef VE_NO_INTRINSICS
	union
	{
		VE_VECTOR r[4];
		struct
		{
			VeFloat32 _11, _12, _13, _14;
			VeFloat32 _21, _22, _23, _24;
			VeFloat32 _31, _32, _33, _34;
			VeFloat32 _41, _42, _43, _44;
		};
		VeFloat32 m[4][4];
	};
#	else
	VE_VECTOR r[4];
#	endif

	VE_MATRIX() noexcept = default;
	VE_MATRIX(VE_FVECTOR R0, VE_FVECTOR R1, VE_FVECTOR R2, VE_CVECTOR R3) noexcept { r[0] = R0; r[1] = R1; r[2] = R2; r[3] = R3; }

};
//--------------------------------------------------------------------------
struct VE_FLOAT2
{
	VeFloat32 x;
	VeFloat32 y;

	VE_FLOAT2() noexcept = default;
	VE_FLOAT2(VeFloat32 _x, VeFloat32 _y) noexcept : x(_x), y(_y) {}
	explicit VE_FLOAT2(const VeFloat32* pArray) noexcept : x(pArray[0]), y(pArray[1]) {}

	VE_FLOAT2& operator= (const VE_FLOAT2& Float2) noexcept { x = Float2.x; y = Float2.y; return *this; }
};
//--------------------------------------------------------------------------
struct alignas(16) VE_FLOAT2A : public VE_FLOAT2
{
	VE_FLOAT2A() noexcept = default;
	VE_FLOAT2A(VeFloat32 _x, VeFloat32 _y) noexcept : VE_FLOAT2(_x, _y) {}
	explicit VE_FLOAT2A(const float *pArray) noexcept : VE_FLOAT2(pArray) {}

	VE_FLOAT2A& operator= (const VE_FLOAT2A& Float2) noexcept { x = Float2.x; y = Float2.y; return *this; }
};
//--------------------------------------------------------------------------
struct VE_INT2
{
	VeInt32 x;
	VeInt32 y;

	VE_INT2() noexcept = default;
	VE_INT2(VeInt32 _x, VeInt32 _y) noexcept : x(_x), y(_y) {}
	explicit VE_INT2(const VeInt32 *pArray) noexcept : x(pArray[0]), y(pArray[1]) {}

	VE_INT2& operator= (const VE_INT2& Int2) noexcept { x = Int2.x; y = Int2.y; return *this; }
};
//--------------------------------------------------------------------------
struct VE_UINT2
{
	VeUInt32 x;
	VeUInt32 y;

	VE_UINT2() noexcept {}
	VE_UINT2(VeUInt32 _x, VeUInt32 _y) noexcept : x(_x), y(_y) {}
	explicit VE_UINT2(const VeUInt32* pArray) noexcept : x(pArray[0]), y(pArray[1]) {}

	VE_UINT2& operator= (const VE_UINT2& UInt2) noexcept { x = UInt2.x; y = UInt2.y; return *this; }
};
//--------------------------------------------------------------------------
struct VE_FLOAT3
{
	VeFloat32 x;
	VeFloat32 y;
	VeFloat32 z;

	VE_FLOAT3() noexcept = default;
	VE_FLOAT3(VeFloat32 _x, VeFloat32 _y, VeFloat32 _z) noexcept : x(_x), y(_y), z(_z) {}
	explicit VE_FLOAT3(const VeFloat32* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

	VE_FLOAT3& operator= (const VE_FLOAT3& Float3) noexcept { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
};
//--------------------------------------------------------------------------
struct alignas(16) VE_FLOAT3A : public VE_FLOAT3
{
	VE_FLOAT3A() noexcept = default;
	VE_FLOAT3A(VeFloat32 _x, VeFloat32 _y, VeFloat32 _z) noexcept : VE_FLOAT3(_x, _y, _z) {}
	explicit VE_FLOAT3A(const VeFloat32* pArray) noexcept : VE_FLOAT3(pArray) {}

	VE_FLOAT3A& operator= (const VE_FLOAT3A& Float3) noexcept { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
};
//--------------------------------------------------------------------------
struct VE_INT3
{
	VeInt32 x;
	VeInt32 y;
	VeInt32 z;

	VE_INT3() noexcept = default;
	VE_INT3(VeInt32 _x, VeInt32 _y, VeInt32 _z) noexcept : x(_x), y(_y), z(_z) {}
	explicit VE_INT3(const VeInt32* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

	VE_INT3& operator= (const VE_INT3& i3) noexcept { x = i3.x; y = i3.y; z = i3.z; return *this; }
};
//--------------------------------------------------------------------------
struct VE_UINT3
{
	VeUInt32 x;
	VeUInt32 y;
	VeUInt32 z;

	VE_UINT3() noexcept = default;
	VE_UINT3(VeUInt32 _x, VeUInt32 _y, VeUInt32 _z) noexcept : x(_x), y(_y), z(_z) {}
	explicit VE_UINT3(const uint32_t *pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

	VE_UINT3& operator= (const VE_UINT3& u3) noexcept { x = u3.x; y = u3.y; z = u3.z; return *this; }
};
//--------------------------------------------------------------------------
struct VE_FLOAT4
{
	VeFloat32 x;
	VeFloat32 y;
	VeFloat32 z;
	VeFloat32 w;

	VE_FLOAT4() noexcept = default;
	VE_FLOAT4(VeFloat32 _x, VeFloat32 _y, VeFloat32 _z, VeFloat32 _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
	explicit VE_FLOAT4(const VeFloat32* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

	VE_FLOAT4& operator= (const VE_FLOAT4& Float4) noexcept { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
};
//--------------------------------------------------------------------------
struct alignas(16) VE_FLOAT4A : public VE_FLOAT4
{
	VE_FLOAT4A() noexcept = default;
	VE_FLOAT4A(VeFloat32 _x, VeFloat32 _y, VeFloat32 _z, VeFloat32 _w) noexcept : VE_FLOAT4(_x, _y, _z, _w) {}
	explicit VE_FLOAT4A(const VeFloat32* pArray) noexcept : VE_FLOAT4(pArray) {}

	VE_FLOAT4A& operator= (const VE_FLOAT4A& Float4) noexcept { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
};
//--------------------------------------------------------------------------
struct VE_INT4
{
	VeInt32 x;
	VeInt32 y;
	VeInt32 z;
	VeInt32 w;

	VE_INT4() noexcept = default;
	VE_INT4(VeInt32 _x, VeInt32 _y, VeInt32 _z, VeInt32 _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
	explicit VE_INT4(const VeInt32* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

	VE_INT4& operator= (const VE_INT4& Int4) noexcept { x = Int4.x; y = Int4.y; z = Int4.z; w = Int4.w; return *this; }
};
//--------------------------------------------------------------------------
struct VE_UINT4
{
	VeUInt32 x;
	VeUInt32 y;
	VeUInt32 z;
	VeUInt32 w;

	VE_UINT4() noexcept = default;
	VE_UINT4(VeUInt32 _x, VeUInt32 _y, VeUInt32 _z, VeUInt32 _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
	explicit VE_UINT4(const VeUInt32* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

	VE_UINT4& operator= (const VE_UINT4& UInt4) noexcept { x = UInt4.x; y = UInt4.y; z = UInt4.z; w = UInt4.w; return *this; }
};
//--------------------------------------------------------------------------
struct VE_FLOAT3X3
{
	union
	{
		struct
		{
			VeFloat32 _11, _21, _31;
			VeFloat32 _12, _22, _32;
			VeFloat32 _13, _23, _33;
		};
		VeFloat32 m[3][3];
	};

	VE_FLOAT3X3() noexcept = default;

};
//--------------------------------------------------------------------------
struct VE_FLOAT4X3
{
	union
	{
		struct
		{
			VeFloat32 _11, _21, _31, _41;
			VeFloat32 _12, _22, _32, _42;
			VeFloat32 _13, _23, _33, _43;
		};
		VeFloat32 m[3][4];
	};

	VE_FLOAT4X3() noexcept = default;
};
//--------------------------------------------------------------------------
struct alignas(16) VE_FLOAT4X3A : public VE_FLOAT4X3
{
	VE_FLOAT4X3A() noexcept = default;
};
//--------------------------------------------------------------------------
struct VE_FLOAT4X4
{
	union
	{
		struct
		{
			VeFloat32 _11, _21, _31, _41;
			VeFloat32 _12, _22, _32, _42;
			VeFloat32 _13, _23, _33, _43;
			VeFloat32 _14, _24, _34, _44;
		};
		VeFloat32 m[4][4];
	};

	VE_FLOAT4X4() noexcept = default;
};
//--------------------------------------------------------------------------
struct alignas(16) VE_FLOAT4X4A : public VE_FLOAT4X4
{
	VE_FLOAT4X4A() noexcept = default;
};
//--------------------------------------------------------------------------
#if defined(VE_ENABLE_SSE)
//--------------------------------------------------------------------------
namespace venus
{
	template<VeUInt32 Shuffle, bool WhichX, bool WhichY, bool WhichZ, bool WhichW> struct PermuteHelper
	{
		static VE_VECTOR VE_MATH_CALLCONV Permute(VE_FVECTOR v1, VE_FVECTOR v2) noexcept
		{
			static const VE_VECTORU32 selectMask =
			{
				WhichX ? 0xFFFFFFFF : 0,
				WhichY ? 0xFFFFFFFF : 0,
				WhichZ ? 0xFFFFFFFF : 0,
				WhichW ? 0xFFFFFFFF : 0,
			};
			VE_VECTOR shuffled1 = VE_PERMUTE_PS(v1, Shuffle);
			VE_VECTOR shuffled2 = VE_PERMUTE_PS(v2, Shuffle);
			VE_VECTOR masked1 = _mm_andnot_ps(selectMask, shuffled1);
			VE_VECTOR masked2 = _mm_and_ps(selectMask, shuffled2);
			return _mm_or_ps(masked1, masked2);
		}
	};

	template<VeUInt32 Shuffle>
	struct PermuteHelper<Shuffle, false, false, false, false>
	{
		static VE_VECTOR VE_MATH_CALLCONV Permute(VE_FVECTOR v1, VE_FVECTOR v2) noexcept { (v2); return XM_PERMUTE_PS(v1, Shuffle); }
	};

	template<VeUInt32 Shuffle>
	struct PermuteHelper<Shuffle, true, true, true, true>
	{
		static VE_VECTOR VE_MATH_CALLCONV Permute(VE_FVECTOR v1, VE_FVECTOR v2) noexcept { (v1); return XM_PERMUTE_PS(v2, Shuffle); }
	};

	template<VeUInt32 Shuffle>
	struct PermuteHelper<Shuffle, false, false, true, true>
	{
		static VE_VECTOR VE_MATH_CALLCONV Permute(VE_FVECTOR v1, VE_FVECTOR v2) noexcept { return _mm_shuffle_ps(v1, v2, Shuffle); }
	};

	template<uint32_t Shuffle>
	struct PermuteHelper<Shuffle, true, true, false, false>
	{
		static VE_VECTOR VE_MATH_CALLCONV Permute(VE_FVECTOR v1, VE_FVECTOR v2) noexcept { return _mm_shuffle_ps(v2, v1, Shuffle); }
	};
};
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
template<VeUInt32 PermuteX, VeUInt32 PermuteY, VeUInt32 PermuteZ, VeUInt32 PermuteW>
inline VE_VECTOR VE_MATH_CALLCONV VeVectorPermute(VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	static_assert(PermuteX <= 7, "PermuteX template parameter out of range");
	static_assert(PermuteY <= 7, "PermuteY template parameter out of range");
	static_assert(PermuteZ <= 7, "PermuteZ template parameter out of range");
	static_assert(PermuteW <= 7, "PermuteW template parameter out of range");
#	if defined(VE_ENABLE_SSE)
	const VeUInt32 Shuffle = _MM_SHUFFLE(PermuteW & 3, PermuteZ & 3, PermuteY & 3, PermuteX & 3);
	const bool WhichX = PermuteX > 3;
	const bool WhichY = PermuteY > 3;
	const bool WhichZ = PermuteZ > 3;
	const bool WhichW = PermuteW > 3;
	return venus::PermuteHelper<Shuffle, WhichX, WhichY, WhichZ, WhichW>::Permute(V1, V2);
#	else
	return VeVectorPermute(V1, V2, PermuteX, PermuteY, PermuteZ, PermuteW);
#	endif
}
//--------------------------------------------------------------------------
template<VeUInt32 SwizzleX, VeUInt32 SwizzleY, VeUInt32 SwizzleZ, VeUInt32 SwizzleW>
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSwizzle(VE_FVECTOR V) noexcept
{
	static_assert(SwizzleX <= 3, "SwizzleX template parameter out of range");
	static_assert(SwizzleY <= 3, "SwizzleY template parameter out of range");
	static_assert(SwizzleZ <= 3, "SwizzleZ template parameter out of range");
	static_assert(SwizzleW <= 3, "SwizzleW template parameter out of range");
#	if defined(VE_ENABLE_SSE)
	return VE_PERMUTE_PS(V, _MM_SHUFFLE(SwizzleW, SwizzleZ, SwizzleY, SwizzleX));
#	else
	return VeVectorSwizzle(V, SwizzleX, SwizzleY, SwizzleZ, SwizzleW);
#	endif
}
//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetBinaryConstant(
	VeUInt32 C0, VeUInt32 C1, VeUInt32 C2, VeUInt32 C3) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTORU32 vResult;
	vResult.u[0] = (0 - (C0 & 1)) & 0x3F800000;
	vResult.u[1] = (0 - (C1 & 1)) & 0x3F800000;
	vResult.u[2] = (0 - (C2 & 1)) & 0x3F800000;
	vResult.u[3] = (0 - (C3 & 1)) & 0x3F800000;
	return vResult.v;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORU32 g_vMask1 = { 1,1,1,1 };
	__m128i vTemp = _mm_set_epi32(C3, C2, C1, C0);
	vTemp = _mm_and_si128(vTemp, g_vMask1);
	vTemp = _mm_cmpeq_epi32(vTemp, g_vMask1);
	vTemp = _mm_and_si128(vTemp, g_MathOne);
	return _mm_castsi128_ps(vTemp);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeConvertVectorIntToFloat(
	VE_FVECTOR VInt, VeUInt32 DivExponent) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSplatConstant(
	VeInt32 IntConstant, VeUInt32 DivExponent) noexcept
{
	VE_ASSERT(IntConstant >= -16 && IntConstant <= 15);
	VE_ASSERT(DivExponent < 32);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTORI32 V = { IntConstant, IntConstant, IntConstant, IntConstant };
	return VeConvertVectorIntToFloat(V.v, DivExponent);
#	elif defined(VE_ENABLE_SSE)
	__m128i vScale = _mm_set1_epi32(IntConstant);
	VE_VECTOR vResult = _mm_cvtepi32_ps(vScale);
	VeUInt32 uScale = 0x3F800000U - (DivExponent << 23);
	vScale = _mm_set1_epi32(uScale);
	vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(vScale));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSplatConstantInt(
	VeInt32 IntConstant) noexcept
{
	VE_ASSERT(IntConstant >= -16 && IntConstant <= 15);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTORI32 V = { IntConstant, IntConstant, IntConstant, IntConstant };
	return V.v;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_set1_epi32(IntConstant);
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
