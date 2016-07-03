//-------------------------------------------------------------------------------------
// XMath.h -- SIMD C++ Math library
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//  
// Copyright (c) Microsoft Corporation. All rights reserved.
//-------------------------------------------------------------------------------------

#pragma once

#ifndef __cplusplus
#error DirectX Math requires C++
#endif

#define DIRECTX_MATH_VERSION 309

#if defined(_MSC_VER) && (_MSC_VER < 1800)
#error DirectX Math Visual C++ 2013 or later.
#endif

#if !defined(__ARM_ARCH) && !defined(_M_ARM) && !defined(_M_ARM64) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(_XM_NO_INTRINSICS_) && !defined(_XM_VECTORCALL_)
#define _XM_VECTORCALL_ 1
#endif

#ifdef _XM_NO_CALL_CONVENTION_
#	define XM_CALLCONV
#else
#	ifdef _MSC_VER
#		if _XM_VECTORCALL_
#			define XM_CALLCONV __vectorcall
#		else
#			define XM_CALLCONV __fastcall
#		endif
#	else
#		if _XM_VECTORCALL_
#			define XM_CALLCONV __attribute__((vectorcall))
#		else
#			define XM_CALLCONV __attribute__((fastcall))
#		endif
#	endif
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1800)
#define XM_CTOR_DEFAULT {}
#else
#define XM_CTOR_DEFAULT =default;
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1900)
#define XM_CONSTEXPR
#else
#define XM_CONSTEXPR constexpr
#endif

#ifndef XM_DEPRECATED
#define XM_DEPRECATED __declspec(deprecated("This is deprecated and will be removed in a future version."))
#endif

#if !defined(_XM_F16C_INTRINSICS_) && defined(__AVX2__) && !defined(_XM_NO_INTRINSICS_)
#define _XM_F16C_INTRINSICS_
#endif

#if defined(_XM_F16C_INTRINSICS_) && !defined(_XM_AVX_INTRINSICS_)
#define _XM_AVX_INTRINSICS_
#endif

#if !defined(_XM_AVX_INTRINSICS_) && defined(__AVX__) && !defined(_XM_NO_INTRINSICS_)
#define _XM_AVX_INTRINSICS_
#endif

#if defined(_XM_AVX_INTRINSICS_) && !defined(_XM_SSE4_INTRINSICS_)
#define _XM_SSE4_INTRINSICS_
#endif

#if defined(_XM_SSE4_INTRINSICS_) && !defined(_XM_SSE3_INTRINSICS_)
#define _XM_SSE3_INTRINSICS_
#endif

#if defined(_XM_SSE3_INTRINSICS_) && !defined(_XM_SSE_INTRINSICS_)
#define _XM_SSE_INTRINSICS_
#endif

#if !defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
#if defined(_M_IX86) || defined(_M_X64)
#define _XM_SSE_INTRINSICS_
#elif defined(_M_ARM) || defined(_M_ARM64)
#define _XM_ARM_NEON_INTRINSICS_
#elif !defined(_XM_NO_INTRINSICS_)
#error X Math does not support this target
#endif
#endif // !_XM_ARM_NEON_INTRINSICS_ && !_XM_SSE_INTRINSICS_ && !_XM_NO_INTRINSICS_

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4514 4820)
#endif
// C4514/4820: Off by default noise
#include <math.h>
#include <float.h>
#ifndef __APPLE__
#	include <malloc.h>
#endif
#ifdef _MSC_VER
#	pragma warning(pop)
#endif

#ifndef _XM_NO_INTRINSICS_
#ifdef _MSC_VER
#	include <intrin.h>
#endif

#ifdef _XM_SSE_INTRINSICS_
#include <xmmintrin.h>
#include <emmintrin.h>

#ifdef _XM_SSE3_INTRINSICS_
#include <pmmintrin.h>
#endif

#ifdef _XM_SSE4_INTRINSICS_
#include <smmintrin.h>
#endif

#ifdef _XM_AVX_INTRINSICS_
#include <immintrin.h>
#endif

#elif defined(_XM_ARM_NEON_INTRINSICS_)
#ifdef _M_ARM64
#include <arm64_neon.h>
#else
#include <arm_neon.h>
#endif
#endif
#endif // !_XM_NO_INTRINSICS_

#ifdef _MSC_VER
#	include <sal.h>
#else
#	define _In_
#	define _In_reads_(n)
#	define _Out_
#	define _Out_writes_(n)
#endif

#include <assert.h>

#ifndef _XM_NO_ROUNDF_
#ifdef _MSC_VER
#include <yvals.h>
#if defined(_CPPLIB_VER) && ( _CPPLIB_VER < 610 )
#define _XM_NO_ROUNDF_
#endif
#endif
#endif

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable : 4005 4668)
#endif
// C4005/4668: Old header issue
#include <stdint.h>
#ifdef _MSC_VER
#	pragma warning(pop)
#endif

/****************************************************************************
*
* Conditional intrinsics
*
****************************************************************************/

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)

#if defined(_XM_NO_MOVNT_)
#define XM_STREAM_PS( p, a ) _mm_store_ps( p, a )
#define XM_SFENCE()
#else
#define XM_STREAM_PS( p, a ) _mm_stream_ps( p, a )
#define XM_SFENCE() _mm_sfence()
#endif

#if defined(_XM_AVX_INTRINSICS_)
#define XM_PERMUTE_PS( v, c ) _mm_permute_ps( v, c )
#else
#define XM_PERMUTE_PS( v, c ) _mm_shuffle_ps( v, v, c )
#endif

#endif // _XM_SSE_INTRINSICS_ && !_XM_NO_INTRINSICS_

/****************************************************************************
*
* Constant definitions
*
****************************************************************************/

constexpr float XM_PI = 3.141592654f;
constexpr float XM_2PI = 6.283185307f;
constexpr float XM_1DIVPI = 0.318309886f;
constexpr float XM_1DIV2PI = 0.159154943f;
constexpr float XM_PIDIV2 = 1.570796327f;
constexpr float XM_PIDIV4 = 0.785398163f;

constexpr uint32_t XM_SELECT_0 = 0x00000000;
constexpr uint32_t XM_SELECT_1 = 0xFFFFFFFF;

constexpr uint32_t XM_PERMUTE_0X = 0;
constexpr uint32_t XM_PERMUTE_0Y = 1;
constexpr uint32_t XM_PERMUTE_0Z = 2;
constexpr uint32_t XM_PERMUTE_0W = 3;
constexpr uint32_t XM_PERMUTE_1X = 4;
constexpr uint32_t XM_PERMUTE_1Y = 5;
constexpr uint32_t XM_PERMUTE_1Z = 6;
constexpr uint32_t XM_PERMUTE_1W = 7;

constexpr uint32_t XM_SWIZZLE_X = 0;
constexpr uint32_t XM_SWIZZLE_Y = 1;
constexpr uint32_t XM_SWIZZLE_Z = 2;
constexpr uint32_t XM_SWIZZLE_W = 3;

constexpr uint32_t XM_CRMASK_CR6 = 0x000000F0;
constexpr uint32_t XM_CRMASK_CR6TRUE = 0x00000080;
constexpr uint32_t XM_CRMASK_CR6FALSE = 0x00000020;
constexpr uint32_t XM_CRMASK_CR6BOUNDS = XM_CRMASK_CR6FALSE;

constexpr size_t XM_CACHE_LINE_SIZE = 64;

/****************************************************************************
*
* Macros
*
****************************************************************************/

// Unit conversion

inline XM_CONSTEXPR float XMConvertToRadians(float fDegrees) { return fDegrees * (XM_PI / 180.0f); }
inline XM_CONSTEXPR float XMConvertToDegrees(float fRadians) { return fRadians * (180.0f / XM_PI); }

// Condition register evaluation proceeding a recording (R) comparison

inline bool XMComparisonAllTrue(uint32_t CR) { return (((CR)& XM_CRMASK_CR6TRUE) == XM_CRMASK_CR6TRUE); }
inline bool XMComparisonAnyTrue(uint32_t CR) { return (((CR)& XM_CRMASK_CR6FALSE) != XM_CRMASK_CR6FALSE); }
inline bool XMComparisonAllFalse(uint32_t CR) { return (((CR)& XM_CRMASK_CR6FALSE) == XM_CRMASK_CR6FALSE); }
inline bool XMComparisonAnyFalse(uint32_t CR) { return (((CR)& XM_CRMASK_CR6TRUE) != XM_CRMASK_CR6TRUE); }
inline bool XMComparisonMixed(uint32_t CR) { return (((CR)& XM_CRMASK_CR6) == 0); }
inline bool XMComparisonAllInBounds(uint32_t CR) { return (((CR)& XM_CRMASK_CR6BOUNDS) == XM_CRMASK_CR6BOUNDS); }
inline bool XMComparisonAnyOutOfBounds(uint32_t CR) { return (((CR)& XM_CRMASK_CR6BOUNDS) != XM_CRMASK_CR6BOUNDS); }

/****************************************************************************
*
* Data types
*
****************************************************************************/

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4068 4201 4365 4324 4820)
//	C4068: ignore unknown pragmas
//	C4201: nonstandard extension used : nameless struct/union
//	C4365: Off by default noise
//	C4324/4820: padding warnings
#	pragma prefast(push)
#	pragma prefast(disable : 25000, "FXMVECTOR is 16 bytes")
#endif

//------------------------------------------------------------------------------
#if defined(_XM_NO_INTRINSICS_)
struct __vector4
{
	union
	{
		float       vector4_f32[4];
		uint32_t    vector4_u32[4];
	};
};
#endif // _XM_NO_INTRINSICS_

//------------------------------------------------------------------------------
// Vector intrinsic: Four 32 bit floating point components aligned on a 16 byte 
// boundary and mapped to hardware vector registers
#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
typedef __m128 XMVECTOR;
#elif defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
typedef float32x4_t XMVECTOR;
#else
typedef __vector4 XMVECTOR;
#endif

// Fix-up for (1st-3rd) XMVECTOR parameters that are pass-in-register for x86, ARM, ARM64, and vector call; by reference otherwise
#if ( defined(_M_IX86) || defined(_M_ARM) || defined(_M_ARM64) || defined(__ARM_ARCH) || _XM_VECTORCALL_ ) && !defined(_XM_NO_INTRINSICS_)
typedef const XMVECTOR FXMVECTOR;
#else
typedef const XMVECTOR& FXMVECTOR;
#endif

// Fix-up for (4th) XMVECTOR parameter to pass in-register for ARM, ARM64, and x64 vector call; by reference otherwise
#if ( defined(_M_ARM) || defined(_M_ARM64) || defined(__ARM_ARCH) || (_XM_VECTORCALL_ && !defined(_M_IX86) ) ) && !defined(_XM_NO_INTRINSICS_)
typedef const XMVECTOR GXMVECTOR;
#else
typedef const XMVECTOR& GXMVECTOR;
#endif

// Fix-up for (5th & 6th) XMVECTOR parameter to pass in-register for ARM64 and vector call; by reference otherwise
#if ( defined(_M_ARM64) || _XM_VECTORCALL_ ) && !defined(_XM_NO_INTRINSICS_)
typedef const XMVECTOR HXMVECTOR;
#else
typedef const XMVECTOR& HXMVECTOR;
#endif

// Fix-up for (7th+) XMVECTOR parameters to pass by reference
typedef const XMVECTOR& CXMVECTOR;

//------------------------------------------------------------------------------
// Conversion types for constants
struct alignas(16) XMVECTORF32
{
	union
	{
		float f[4];
		XMVECTOR v;
	};

	inline operator XMVECTOR() const { return v; }
	inline operator const float*() const { return f; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
	inline operator __m128i() const { return _mm_castps_si128(v); }
	inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
};

struct alignas(16) XMVECTORI32
{
	union
	{
		int32_t i[4];
		XMVECTOR v;
	};

	inline operator XMVECTOR() const { return v; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
	inline operator __m128i() const { return _mm_castps_si128(v); }
	inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
};

struct alignas(16) XMVECTORU8
{
	union
	{
		uint8_t u[16];
		XMVECTOR v;
	};

	inline operator XMVECTOR() const { return v; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
	inline operator __m128i() const { return _mm_castps_si128(v); }
	inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
};

struct alignas(16) XMVECTORU32
{
	union
	{
		uint32_t u[4];
		XMVECTOR v;
	};

	inline operator XMVECTOR() const { return v; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
	inline operator __m128i() const { return _mm_castps_si128(v); }
	inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
};

//------------------------------------------------------------------------------
// Vector operators
/*XMVECTOR    XM_CALLCONV     operator+ (FXMVECTOR V);
XMVECTOR    XM_CALLCONV     operator- (FXMVECTOR V);

XMVECTOR&   XM_CALLCONV     operator+= (XMVECTOR& V1, FXMVECTOR V2);
XMVECTOR&   XM_CALLCONV     operator-= (XMVECTOR& V1, FXMVECTOR V2);
XMVECTOR&   XM_CALLCONV     operator*= (XMVECTOR& V1, FXMVECTOR V2);
XMVECTOR&   XM_CALLCONV     operator/= (XMVECTOR& V1, FXMVECTOR V2);

XMVECTOR&   operator*= (XMVECTOR& V, float S);
XMVECTOR&   operator/= (XMVECTOR& V, float S);

XMVECTOR    XM_CALLCONV     operator+ (FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     operator- (FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     operator* (FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     operator/ (FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     operator* (FXMVECTOR V, float S);
XMVECTOR    XM_CALLCONV     operator* (float S, FXMVECTOR V);
XMVECTOR    XM_CALLCONV     operator/ (FXMVECTOR V, float S);*/

//------------------------------------------------------------------------------
// Matrix type: Sixteen 32 bit floating point components aligned on a
// 16 byte boundary and mapped to four hardware vector registers

struct XMMATRIX;

// Fix-up for (1st) XMMATRIX parameter to pass in-register for ARM64 and vector call; by reference otherwise
#if ( defined(_M_ARM64) || _XM_VECTORCALL_ ) && !defined(_XM_NO_INTRINSICS_)
typedef const XMMATRIX FXMMATRIX;
#else
typedef const XMMATRIX& FXMMATRIX;
#endif

// Fix-up for (2nd+) XMMATRIX parameters to pass by reference
typedef const XMMATRIX& CXMMATRIX;

#ifdef _XM_NO_INTRINSICS_
struct XMMATRIX
#else
struct alignas(16) XMMATRIX
#endif
{
#ifdef _XM_NO_INTRINSICS_
	union
	{
		XMVECTOR c[4];
		struct
		{
			float _11, _21, _31, _41;
			float _12, _22, _32, _42;
			float _13, _23, _33, _43;
			float _14, _24, _34, _44;
		};
		float m[4][4];
	};
#else
	XMVECTOR c[4];
#endif

	XMMATRIX() XM_CTOR_DEFAULT
#if defined(_MSC_VER) && _MSC_VER >= 1900
		constexpr XMMATRIX(FXMVECTOR C0, FXMVECTOR C1, FXMVECTOR C2, CXMVECTOR C3) : c{ C0,C1,C2,C3 } {}
#else
		XMMATRIX(FXMVECTOR C0, FXMVECTOR C1, FXMVECTOR C2, CXMVECTOR C3) { c[0] = C0; c[1] = C1; c[2] = C2; c[3] = C3; }
#endif
	XMMATRIX(float m00, float m10, float m20, float m30,
		float m01, float m11, float m21, float m31,
		float m02, float m12, float m22, float m32,
		float m03, float m13, float m23, float m33);
	explicit XMMATRIX(_In_reads_(16) const float *pArray);

#ifdef _XM_NO_INTRINSICS_
	float       operator() (size_t Row, size_t Column) const { return m[Column][Row]; }
	float&      operator() (size_t Row, size_t Column) { return m[Column][Row]; }
#endif

	XMMATRIX&   operator= (const XMMATRIX& M) { c[0] = M.c[0]; c[1] = M.c[1]; c[2] = M.c[2]; c[3] = M.c[3]; return *this; }

	XMMATRIX    operator+ () const { return *this; }
	XMMATRIX    operator- () const;

	XMMATRIX&   XM_CALLCONV     operator+= (FXMMATRIX M);
	XMMATRIX&   XM_CALLCONV     operator-= (FXMMATRIX M);
	XMMATRIX&   XM_CALLCONV     operator*= (FXMMATRIX M);
	XMMATRIX&   operator*= (float S);
	XMMATRIX&   operator/= (float S);

	XMMATRIX    XM_CALLCONV     operator+ (FXMMATRIX M) const;
	XMMATRIX    XM_CALLCONV     operator- (FXMMATRIX M) const;
	XMMATRIX    XM_CALLCONV     operator* (FXMMATRIX M) const;
	XMMATRIX    operator* (float S) const;
	XMMATRIX    operator/ (float S) const;

	friend XMMATRIX     XM_CALLCONV     operator* (float S, FXMMATRIX M);
};

//------------------------------------------------------------------------------
// 2D Vector; 32 bit floating point components
struct XMFLOAT2
{
	float x;
	float y;

	XMFLOAT2() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMFLOAT2(float _x, float _y) : x(_x), y(_y) {}
	explicit XMFLOAT2(_In_reads_(2) const float *pArray) : x(pArray[0]), y(pArray[1]) {}

	XMFLOAT2& operator= (const XMFLOAT2& Float2) { x = Float2.x; y = Float2.y; return *this; }
};

// 2D Vector; 32 bit floating point components aligned on a 16 byte boundary
struct alignas(16) XMFLOAT2A : public XMFLOAT2
{
	XMFLOAT2A() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMFLOAT2A(float _x, float _y) : XMFLOAT2(_x, _y) {}
	explicit XMFLOAT2A(_In_reads_(2) const float *pArray) : XMFLOAT2(pArray) {}

	XMFLOAT2A& operator= (const XMFLOAT2A& Float2) { x = Float2.x; y = Float2.y; return *this; }
};

//------------------------------------------------------------------------------
// 2D Vector; 32 bit signed integer components
struct XMINT2
{
	int32_t x;
	int32_t y;

	XMINT2() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMINT2(int32_t _x, int32_t _y) : x(_x), y(_y) {}
	explicit XMINT2(_In_reads_(2) const int32_t *pArray) : x(pArray[0]), y(pArray[1]) {}

	XMINT2& operator= (const XMINT2& Int2) { x = Int2.x; y = Int2.y; return *this; }
};

// 2D Vector; 32 bit unsigned integer components
struct XMUINT2
{
	uint32_t x;
	uint32_t y;

	XMUINT2() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMUINT2(uint32_t _x, uint32_t _y) : x(_x), y(_y) {}
	explicit XMUINT2(_In_reads_(2) const uint32_t *pArray) : x(pArray[0]), y(pArray[1]) {}

	XMUINT2& operator= (const XMUINT2& UInt2) { x = UInt2.x; y = UInt2.y; return *this; }
};

//------------------------------------------------------------------------------
// 3D Vector; 32 bit floating point components
struct XMFLOAT3
{
	float x;
	float y;
	float z;

	XMFLOAT3() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMFLOAT3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	explicit XMFLOAT3(_In_reads_(3) const float *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

	XMFLOAT3& operator= (const XMFLOAT3& Float3) { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
};

// 3D Vector; 32 bit floating point components aligned on a 16 byte boundary
struct alignas(16) XMFLOAT3A : public XMFLOAT3
{
	XMFLOAT3A() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMFLOAT3A(float _x, float _y, float _z) : XMFLOAT3(_x, _y, _z) {}
	explicit XMFLOAT3A(_In_reads_(3) const float *pArray) : XMFLOAT3(pArray) {}

	XMFLOAT3A& operator= (const XMFLOAT3A& Float3) { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
};

//------------------------------------------------------------------------------
// 3D Vector; 32 bit signed integer components
struct XMINT3
{
	int32_t x;
	int32_t y;
	int32_t z;

	XMINT3() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMINT3(int32_t _x, int32_t _y, int32_t _z) : x(_x), y(_y), z(_z) {}
	explicit XMINT3(_In_reads_(3) const int32_t *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

	XMINT3& operator= (const XMINT3& i3) { x = i3.x; y = i3.y; z = i3.z; return *this; }
};

// 3D Vector; 32 bit unsigned integer components
struct XMUINT3
{
	uint32_t x;
	uint32_t y;
	uint32_t z;

	XMUINT3() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMUINT3(uint32_t _x, uint32_t _y, uint32_t _z) : x(_x), y(_y), z(_z) {}
	explicit XMUINT3(_In_reads_(3) const uint32_t *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

	XMUINT3& operator= (const XMUINT3& u3) { x = u3.x; y = u3.y; z = u3.z; return *this; }
};

//------------------------------------------------------------------------------
// 4D Vector; 32 bit floating point components
struct XMFLOAT4
{
	float x;
	float y;
	float z;
	float w;

	XMFLOAT4() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMFLOAT4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	explicit XMFLOAT4(_In_reads_(4) const float *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

	XMFLOAT4& operator= (const XMFLOAT4& Float4) { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
};

// 4D Vector; 32 bit floating point components aligned on a 16 byte boundary
struct alignas(16) XMFLOAT4A : public XMFLOAT4
{
	XMFLOAT4A() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMFLOAT4A(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
	explicit XMFLOAT4A(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}

	XMFLOAT4A& operator= (const XMFLOAT4A& Float4) { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
};

//------------------------------------------------------------------------------
// 4D Vector; 32 bit signed integer components
struct XMINT4
{
	int32_t x;
	int32_t y;
	int32_t z;
	int32_t w;

	XMINT4() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMINT4(int32_t _x, int32_t _y, int32_t _z, int32_t _w) : x(_x), y(_y), z(_z), w(_w) {}
	explicit XMINT4(_In_reads_(4) const int32_t *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

	XMINT4& operator= (const XMINT4& Int4) { x = Int4.x; y = Int4.y; z = Int4.z; w = Int4.w; return *this; }
};

// 4D Vector; 32 bit unsigned integer components
struct XMUINT4
{
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint32_t w;

	XMUINT4() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMUINT4(uint32_t _x, uint32_t _y, uint32_t _z, uint32_t _w) : x(_x), y(_y), z(_z), w(_w) {}
	explicit XMUINT4(_In_reads_(4) const uint32_t *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

	XMUINT4& operator= (const XMUINT4& UInt4) { x = UInt4.x; y = UInt4.y; z = UInt4.z; w = UInt4.w; return *this; }
};

//------------------------------------------------------------------------------
// 3x3 Matrix: 32 bit floating point components
struct XMFLOAT3X3
{
	union
	{
		struct
		{
			float _11, _21, _31;
			float _12, _22, _32;
			float _13, _23, _33;
		};
		float m[3][3];
	};

	XMFLOAT3X3() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMFLOAT3X3(float m00, float m10, float m20,
			float m01, float m11, float m21,
			float m02, float m12, float m22)
		: _11(m00), _21(m10), _31(m20),
		_12(m01), _22(m11), _32(m21),
		_13(m02), _23(m12), _33(m22) {}
	explicit XMFLOAT3X3(_In_reads_(9) const float *pArray);

	float       operator() (size_t Row, size_t Column) const { return m[Column][Row]; }
	float&      operator() (size_t Row, size_t Column) { return m[Column][Row]; }

	XMFLOAT3X3& operator= (const XMFLOAT3X3& Float3x3);
};

//------------------------------------------------------------------------------
// 4x3 Matrix: 32 bit floating point components
struct XMFLOAT4X3
{
	union
	{
		struct
		{
			float _11, _21, _31, _41;
			float _12, _22, _32, _42;
			float _13, _23, _33, _43;
		};
		float m[3][4];
	};

	XMFLOAT4X3() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMFLOAT4X3(float m00, float m10, float m20, float m30,
			float m01, float m11, float m21, float m31,
			float m02, float m12, float m22, float m32)
		: _11(m00), _21(m10), _31(m20), _41(m30),
		_12(m01), _22(m11), _32(m21), _42(m31),
		_13(m02), _23(m12), _33(m22), _43(m32) {}
	explicit XMFLOAT4X3(_In_reads_(12) const float *pArray);

	float       operator() (size_t Row, size_t Column) const { return m[Column][Row]; }
	float&      operator() (size_t Row, size_t Column) { return m[Column][Row]; }

	XMFLOAT4X3& operator= (const XMFLOAT4X3& Float4x3);

};

// 4x3 Matrix: 32 bit floating point components aligned on a 16 byte boundary
struct alignas(16) XMFLOAT4X3A : public XMFLOAT4X3
{
	XMFLOAT4X3A() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMFLOAT4X3A(float m00, float m10, float m20, float m30,
			float m01, float m11, float m21, float m31,
			float m02, float m12, float m22, float m32) :
		XMFLOAT4X3(m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32) {}
	explicit XMFLOAT4X3A(_In_reads_(12) const float *pArray) : XMFLOAT4X3(pArray) {}

	float       operator() (size_t Row, size_t Column) const { return m[Column][Row]; }
	float&      operator() (size_t Row, size_t Column) { return m[Column][Row]; }

	XMFLOAT4X3A& operator= (const XMFLOAT4X3A& Float4x3);
};

//------------------------------------------------------------------------------
// 4x4 Matrix: 32 bit floating point components
struct XMFLOAT4X4
{
	union
	{
		struct
		{
			float _11, _21, _31, _41;
			float _12, _22, _32, _42;
			float _13, _23, _33, _43;
			float _14, _24, _34, _44;
		};
		float m[4][4];
	};

	XMFLOAT4X4() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMFLOAT4X4(float m00, float m10, float m20, float m30,
			float m01, float m11, float m21, float m31,
			float m02, float m12, float m22, float m32,
			float m03, float m13, float m23, float m33)
		: _11(m00), _21(m10), _31(m20), _41(m30),
		_12(m01), _22(m11), _32(m21), _42(m31),
		_13(m02), _23(m12), _33(m22), _43(m32),
		_14(m03), _24(m13), _34(m23), _44(m33) {}
	explicit XMFLOAT4X4(_In_reads_(16) const float *pArray);

	float       operator() (size_t Row, size_t Column) const { return m[Column][Row]; }
	float&      operator() (size_t Row, size_t Column) { return m[Column][Row]; }

	XMFLOAT4X4& operator= (const XMFLOAT4X4& Float4x4);
};

// 4x4 Matrix: 32 bit floating point components aligned on a 16 byte boundary
struct alignas(16) XMFLOAT4X4A : public XMFLOAT4X4
{
	XMFLOAT4X4A() XM_CTOR_DEFAULT
		XM_CONSTEXPR XMFLOAT4X4A(float m00, float m10, float m20, float m30,
			float m01, float m11, float m21, float m31,
			float m02, float m12, float m22, float m32,
			float m03, float m13, float m23, float m33)
		: XMFLOAT4X4(m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33) {}
	explicit XMFLOAT4X4A(_In_reads_(16) const float *pArray) : XMFLOAT4X4(pArray) {}

	float       operator() (size_t Row, size_t Column) const { return m[Column][Row]; }
	float&      operator() (size_t Row, size_t Column) { return m[Column][Row]; }

	XMFLOAT4X4A& operator= (const XMFLOAT4X4A& Float4x4);
};

////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#	pragma prefast(pop)
#	pragma warning(pop)
#endif

/****************************************************************************
*
* Data conversion operations
*
****************************************************************************/

XMVECTOR    XM_CALLCONV     XMConvertVectorIntToFloat(FXMVECTOR VInt, uint32_t DivExponent);
XMVECTOR    XM_CALLCONV     XMConvertVectorFloatToInt(FXMVECTOR VFloat, uint32_t MulExponent);
XMVECTOR    XM_CALLCONV     XMConvertVectorUIntToFloat(FXMVECTOR VUInt, uint32_t DivExponent);
XMVECTOR    XM_CALLCONV     XMConvertVectorFloatToUInt(FXMVECTOR VFloat, uint32_t MulExponent);

XMVECTOR    XM_CALLCONV     XMVectorSetBinaryConstant(uint32_t C0, uint32_t C1, uint32_t C2, uint32_t C3);
XMVECTOR    XM_CALLCONV     XMVectorSplatConstant(int32_t IntConstant, uint32_t DivExponent);
XMVECTOR    XM_CALLCONV     XMVectorSplatConstantInt(int32_t IntConstant);

/****************************************************************************
*
* Load operations
*
****************************************************************************/

XMVECTOR    XM_CALLCONV     XMLoadInt(_In_ const uint32_t* pSource);
XMVECTOR    XM_CALLCONV     XMLoadFloat(_In_ const float* pSource);

XMVECTOR    XM_CALLCONV     XMLoadInt2(_In_reads_(2) const uint32_t* pSource);
XMVECTOR    XM_CALLCONV     XMLoadInt2A(_In_reads_(2) const uint32_t* PSource);
XMVECTOR    XM_CALLCONV     XMLoadFloat2(_In_ const XMFLOAT2* pSource);
XMVECTOR    XM_CALLCONV     XMLoadFloat2A(_In_ const XMFLOAT2A* pSource);
XMVECTOR    XM_CALLCONV     XMLoadSInt2(_In_ const XMINT2* pSource);
XMVECTOR    XM_CALLCONV     XMLoadUInt2(_In_ const XMUINT2* pSource);

XMVECTOR    XM_CALLCONV     XMLoadInt3(_In_reads_(3) const uint32_t* pSource);
XMVECTOR    XM_CALLCONV     XMLoadInt3A(_In_reads_(3) const uint32_t* pSource);
XMVECTOR    XM_CALLCONV     XMLoadFloat3(_In_ const XMFLOAT3* pSource);
XMVECTOR    XM_CALLCONV     XMLoadFloat3A(_In_ const XMFLOAT3A* pSource);
XMVECTOR    XM_CALLCONV     XMLoadSInt3(_In_ const XMINT3* pSource);
XMVECTOR    XM_CALLCONV     XMLoadUInt3(_In_ const XMUINT3* pSource);

XMVECTOR    XM_CALLCONV     XMLoadInt4(_In_reads_(4) const uint32_t* pSource);
XMVECTOR    XM_CALLCONV     XMLoadInt4A(_In_reads_(4) const uint32_t* pSource);
XMVECTOR    XM_CALLCONV     XMLoadFloat4(_In_ const XMFLOAT4* pSource);
XMVECTOR    XM_CALLCONV     XMLoadFloat4A(_In_ const XMFLOAT4A* pSource);
XMVECTOR    XM_CALLCONV     XMLoadSInt4(_In_ const XMINT4* pSource);
XMVECTOR    XM_CALLCONV     XMLoadUInt4(_In_ const XMUINT4* pSource);

XMMATRIX    XM_CALLCONV     XMLoadFloat3x3(_In_ const XMFLOAT3X3* pSource);
XMMATRIX    XM_CALLCONV     XMLoadFloat4x3(_In_ const XMFLOAT4X3* pSource);
XMMATRIX    XM_CALLCONV     XMLoadFloat4x3A(_In_ const XMFLOAT4X3A* pSource);
XMMATRIX    XM_CALLCONV     XMLoadFloat4x4(_In_ const XMFLOAT4X4* pSource);
XMMATRIX    XM_CALLCONV     XMLoadFloat4x4A(_In_ const XMFLOAT4X4A* pSource);

/****************************************************************************
*
* Store operations
*
****************************************************************************/

void        XM_CALLCONV     XMStoreInt(_Out_ uint32_t* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreFloat(_Out_ float* pDestination, _In_ FXMVECTOR V);

void        XM_CALLCONV     XMStoreInt2(_Out_writes_(2) uint32_t* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreInt2A(_Out_writes_(2) uint32_t* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreFloat2(_Out_ XMFLOAT2* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreFloat2A(_Out_ XMFLOAT2A* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreSInt2(_Out_ XMINT2* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreUInt2(_Out_ XMUINT2* pDestination, _In_ FXMVECTOR V);

void        XM_CALLCONV     XMStoreInt3(_Out_writes_(3) uint32_t* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreInt3A(_Out_writes_(3) uint32_t* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreFloat3(_Out_ XMFLOAT3* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreFloat3A(_Out_ XMFLOAT3A* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreSInt3(_Out_ XMINT3* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreUInt3(_Out_ XMUINT3* pDestination, _In_ FXMVECTOR V);

void        XM_CALLCONV     XMStoreInt4(_Out_writes_(4) uint32_t* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreInt4A(_Out_writes_(4) uint32_t* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreFloat4(_Out_ XMFLOAT4* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreFloat4A(_Out_ XMFLOAT4A* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreSInt4(_Out_ XMINT4* pDestination, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMStoreUInt4(_Out_ XMUINT4* pDestination, _In_ FXMVECTOR V);

void        XM_CALLCONV     XMStoreFloat3x3(_Out_ XMFLOAT3X3* pDestination, _In_ FXMMATRIX M);
void        XM_CALLCONV     XMStoreFloat4x3(_Out_ XMFLOAT4X3* pDestination, _In_ FXMMATRIX M);
void        XM_CALLCONV     XMStoreFloat4x3A(_Out_ XMFLOAT4X3A* pDestination, _In_ FXMMATRIX M);
void        XM_CALLCONV     XMStoreFloat4x4(_Out_ XMFLOAT4X4* pDestination, _In_ FXMMATRIX M);
void        XM_CALLCONV     XMStoreFloat4x4A(_Out_ XMFLOAT4X4A* pDestination, _In_ FXMMATRIX M);

/****************************************************************************
*
* General vector operations
*
****************************************************************************/

XMVECTOR    XM_CALLCONV     XMVectorZero();
XMVECTOR    XM_CALLCONV     XMVectorSet(float x, float y, float z, float w);
XMVECTOR    XM_CALLCONV     XMVectorSetInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w);
XMVECTOR    XM_CALLCONV     XMVectorReplicate(float Value);
XMVECTOR    XM_CALLCONV     XMVectorReplicatePtr(_In_ const float *pValue);
XMVECTOR    XM_CALLCONV     XMVectorReplicateInt(uint32_t Value);
XMVECTOR    XM_CALLCONV     XMVectorReplicateIntPtr(_In_ const uint32_t *pValue);
XMVECTOR    XM_CALLCONV     XMVectorTrueInt();
XMVECTOR    XM_CALLCONV     XMVectorFalseInt();
XMVECTOR    XM_CALLCONV     XMVectorSplatX(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorSplatY(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorSplatZ(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorSplatW(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorSplatOne();
XMVECTOR    XM_CALLCONV     XMVectorSplatInfinity();
XMVECTOR    XM_CALLCONV     XMVectorSplatQNaN();
XMVECTOR    XM_CALLCONV     XMVectorSplatEpsilon();
XMVECTOR    XM_CALLCONV     XMVectorSplatSignMask();

float       XM_CALLCONV     XMVectorGetByIndex(FXMVECTOR V, size_t i);
float       XM_CALLCONV     XMVectorGetX(FXMVECTOR V);
float       XM_CALLCONV     XMVectorGetY(FXMVECTOR V);
float       XM_CALLCONV     XMVectorGetZ(FXMVECTOR V);
float       XM_CALLCONV     XMVectorGetW(FXMVECTOR V);

void        XM_CALLCONV     XMVectorGetByIndexPtr(_Out_ float *f, _In_ FXMVECTOR V, _In_ size_t i);
void        XM_CALLCONV     XMVectorGetXPtr(_Out_ float *x, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMVectorGetYPtr(_Out_ float *y, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMVectorGetZPtr(_Out_ float *z, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMVectorGetWPtr(_Out_ float *w, _In_ FXMVECTOR V);

uint32_t    XM_CALLCONV     XMVectorGetIntByIndex(FXMVECTOR V, size_t i);
uint32_t    XM_CALLCONV     XMVectorGetIntX(FXMVECTOR V);
uint32_t    XM_CALLCONV     XMVectorGetIntY(FXMVECTOR V);
uint32_t    XM_CALLCONV     XMVectorGetIntZ(FXMVECTOR V);
uint32_t    XM_CALLCONV     XMVectorGetIntW(FXMVECTOR V);

void        XM_CALLCONV     XMVectorGetIntByIndexPtr(_Out_ uint32_t *x, _In_ FXMVECTOR V, _In_ size_t i);
void        XM_CALLCONV     XMVectorGetIntXPtr(_Out_ uint32_t *x, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMVectorGetIntYPtr(_Out_ uint32_t *y, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMVectorGetIntZPtr(_Out_ uint32_t *z, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMVectorGetIntWPtr(_Out_ uint32_t *w, _In_ FXMVECTOR V);

XMVECTOR    XM_CALLCONV     XMVectorSetByIndex(FXMVECTOR V, float f, size_t i);
XMVECTOR    XM_CALLCONV     XMVectorSetX(FXMVECTOR V, float x);
XMVECTOR    XM_CALLCONV     XMVectorSetY(FXMVECTOR V, float y);
XMVECTOR    XM_CALLCONV     XMVectorSetZ(FXMVECTOR V, float z);
XMVECTOR    XM_CALLCONV     XMVectorSetW(FXMVECTOR V, float w);

XMVECTOR    XM_CALLCONV     XMVectorSetByIndexPtr(_In_ FXMVECTOR V, _In_ const float *f, _In_ size_t i);
XMVECTOR    XM_CALLCONV     XMVectorSetXPtr(_In_ FXMVECTOR V, _In_ const float *x);
XMVECTOR    XM_CALLCONV     XMVectorSetYPtr(_In_ FXMVECTOR V, _In_ const float *y);
XMVECTOR    XM_CALLCONV     XMVectorSetZPtr(_In_ FXMVECTOR V, _In_ const float *z);
XMVECTOR    XM_CALLCONV     XMVectorSetWPtr(_In_ FXMVECTOR V, _In_ const float *w);

XMVECTOR    XM_CALLCONV     XMVectorSetIntByIndex(FXMVECTOR V, uint32_t x, size_t i);
XMVECTOR    XM_CALLCONV     XMVectorSetIntX(FXMVECTOR V, uint32_t x);
XMVECTOR    XM_CALLCONV     XMVectorSetIntY(FXMVECTOR V, uint32_t y);
XMVECTOR    XM_CALLCONV     XMVectorSetIntZ(FXMVECTOR V, uint32_t z);
XMVECTOR    XM_CALLCONV     XMVectorSetIntW(FXMVECTOR V, uint32_t w);

XMVECTOR    XM_CALLCONV     XMVectorSetIntByIndexPtr(_In_ FXMVECTOR V, _In_ const uint32_t *x, _In_ size_t i);
XMVECTOR    XM_CALLCONV     XMVectorSetIntXPtr(_In_ FXMVECTOR V, _In_ const uint32_t *x);
XMVECTOR    XM_CALLCONV     XMVectorSetIntYPtr(_In_ FXMVECTOR V, _In_ const uint32_t *y);
XMVECTOR    XM_CALLCONV     XMVectorSetIntZPtr(_In_ FXMVECTOR V, _In_ const uint32_t *z);
XMVECTOR    XM_CALLCONV     XMVectorSetIntWPtr(_In_ FXMVECTOR V, _In_ const uint32_t *w);

#if defined(__XNAMATH_H__) && defined(XMVectorSwizzle)
#undef XMVectorSwizzle
#endif

XMVECTOR    XM_CALLCONV     XMVectorSwizzle(FXMVECTOR V, uint32_t E0, uint32_t E1, uint32_t E2, uint32_t E3);
XMVECTOR    XM_CALLCONV     XMVectorPermute(FXMVECTOR V1, FXMVECTOR V2, uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW);
XMVECTOR    XM_CALLCONV     XMVectorSelectControl(uint32_t VectorIndex0, uint32_t VectorIndex1, uint32_t VectorIndex2, uint32_t VectorIndex3);
XMVECTOR    XM_CALLCONV     XMVectorSelect(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR Control);
XMVECTOR    XM_CALLCONV     XMVectorMergeXY(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorMergeZW(FXMVECTOR V1, FXMVECTOR V2);

#if defined(__XNAMATH_H__) && defined(XMVectorShiftLeft)
#undef XMVectorShiftLeft
#undef XMVectorRotateLeft
#undef XMVectorRotateRight
#undef XMVectorInsert
#endif

XMVECTOR    XM_CALLCONV     XMVectorShiftLeft(FXMVECTOR V1, FXMVECTOR V2, uint32_t Elements);
XMVECTOR    XM_CALLCONV     XMVectorRotateLeft(FXMVECTOR V, uint32_t Elements);
XMVECTOR    XM_CALLCONV     XMVectorRotateRight(FXMVECTOR V, uint32_t Elements);
XMVECTOR    XM_CALLCONV     XMVectorInsert(FXMVECTOR VD, FXMVECTOR VS, uint32_t VSLeftRotateElements,
	uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3);

XMVECTOR    XM_CALLCONV     XMVectorEqual(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorEqualR(_Out_ uint32_t* pCR, _In_ FXMVECTOR V1, _In_ FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorEqualInt(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorEqualIntR(_Out_ uint32_t* pCR, _In_ FXMVECTOR V, _In_ FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorNearEqual(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR Epsilon);
XMVECTOR    XM_CALLCONV     XMVectorNotEqual(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorNotEqualInt(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorGreater(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorGreaterR(_Out_ uint32_t* pCR, _In_ FXMVECTOR V1, _In_ FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorGreaterOrEqual(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorGreaterOrEqualR(_Out_ uint32_t* pCR, _In_ FXMVECTOR V1, _In_ FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorLess(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorLessOrEqual(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorInBounds(FXMVECTOR V, FXMVECTOR Bounds);
XMVECTOR    XM_CALLCONV     XMVectorInBoundsR(_Out_ uint32_t* pCR, _In_ FXMVECTOR V, _In_ FXMVECTOR Bounds);

XMVECTOR    XM_CALLCONV     XMVectorIsNaN(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorIsInfinite(FXMVECTOR V);

XMVECTOR    XM_CALLCONV     XMVectorMin(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorMax(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorRound(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorTruncate(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorFloor(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorCeiling(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorClamp(FXMVECTOR V, FXMVECTOR Min, FXMVECTOR Max);
XMVECTOR    XM_CALLCONV     XMVectorSaturate(FXMVECTOR V);

XMVECTOR    XM_CALLCONV     XMVectorAndInt(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorAndCInt(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorOrInt(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorNorInt(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorXorInt(FXMVECTOR V1, FXMVECTOR V2);

XMVECTOR    XM_CALLCONV     XMVectorNegate(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorAdd(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorSum(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorAddAngles(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorSubtract(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorSubtractAngles(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorMultiply(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorMultiplyAdd(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR V3);
XMVECTOR    XM_CALLCONV     XMVectorDivide(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorNegativeMultiplySubtract(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR V3);
XMVECTOR    XM_CALLCONV     XMVectorScale(FXMVECTOR V, float ScaleFactor);
XMVECTOR    XM_CALLCONV     XMVectorReciprocalEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorReciprocal(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorSqrtEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorSqrt(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorReciprocalSqrtEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorReciprocalSqrt(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorExp2(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorExpE(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorExp(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorLog2(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorLogE(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorLog(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorPow(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorAbs(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorMod(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVectorModAngles(FXMVECTOR Angles);
XMVECTOR    XM_CALLCONV     XMVectorSin(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorSinEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorCos(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorCosEst(FXMVECTOR V);
void        XM_CALLCONV     XMVectorSinCos(_Out_ XMVECTOR* pSin, _Out_ XMVECTOR* pCos, _In_ FXMVECTOR V);
void        XM_CALLCONV     XMVectorSinCosEst(_Out_ XMVECTOR* pSin, _Out_ XMVECTOR* pCos, _In_ FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorTan(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorTanEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorSinH(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorCosH(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorTanH(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorASin(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorASinEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorACos(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorACosEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorATan(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorATanEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVectorATan2(FXMVECTOR Y, FXMVECTOR X);
XMVECTOR    XM_CALLCONV     XMVectorATan2Est(FXMVECTOR Y, FXMVECTOR X);
XMVECTOR    XM_CALLCONV     XMVectorLerp(FXMVECTOR V0, FXMVECTOR V1, float t);
XMVECTOR    XM_CALLCONV     XMVectorLerpV(FXMVECTOR V0, FXMVECTOR V1, FXMVECTOR T);
XMVECTOR    XM_CALLCONV     XMVectorHermite(FXMVECTOR Position0, FXMVECTOR Tangent0, FXMVECTOR Position1, GXMVECTOR Tangent1, float t);
XMVECTOR    XM_CALLCONV     XMVectorHermiteV(FXMVECTOR Position0, FXMVECTOR Tangent0, FXMVECTOR Position1, GXMVECTOR Tangent1, HXMVECTOR T);
XMVECTOR    XM_CALLCONV     XMVectorCatmullRom(FXMVECTOR Position0, FXMVECTOR Position1, FXMVECTOR Position2, GXMVECTOR Position3, float t);
XMVECTOR    XM_CALLCONV     XMVectorCatmullRomV(FXMVECTOR Position0, FXMVECTOR Position1, FXMVECTOR Position2, GXMVECTOR Position3, HXMVECTOR T);
XMVECTOR    XM_CALLCONV     XMVectorBaryCentric(FXMVECTOR Position0, FXMVECTOR Position1, FXMVECTOR Position2, float f, float g);
XMVECTOR    XM_CALLCONV     XMVectorBaryCentricV(FXMVECTOR Position0, FXMVECTOR Position1, FXMVECTOR Position2, GXMVECTOR F, HXMVECTOR G);

/****************************************************************************
*
* 2D vector operations
*
****************************************************************************/

bool        XM_CALLCONV     XMVector2Equal(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector2EqualR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector2EqualInt(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector2EqualIntR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector2NearEqual(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR Epsilon);
bool        XM_CALLCONV     XMVector2NotEqual(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector2NotEqualInt(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector2Greater(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector2GreaterR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector2GreaterOrEqual(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector2GreaterOrEqualR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector2Less(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector2LessOrEqual(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector2InBounds(FXMVECTOR V, FXMVECTOR Bounds);

bool        XM_CALLCONV     XMVector2IsNaN(FXMVECTOR V);
bool        XM_CALLCONV     XMVector2IsInfinite(FXMVECTOR V);

XMVECTOR    XM_CALLCONV     XMVector2Dot(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVector2Cross(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVector2LengthSq(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector2ReciprocalLengthEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector2ReciprocalLength(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector2LengthEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector2Length(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector2NormalizeEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector2Normalize(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector2ClampLength(FXMVECTOR V, float LengthMin, float LengthMax);
XMVECTOR    XM_CALLCONV     XMVector2ClampLengthV(FXMVECTOR V, FXMVECTOR LengthMin, FXMVECTOR LengthMax);
XMVECTOR    XM_CALLCONV     XMVector2Reflect(FXMVECTOR Incident, FXMVECTOR Normal);
XMVECTOR    XM_CALLCONV     XMVector2Refract(FXMVECTOR Incident, FXMVECTOR Normal, float RefractionIndex);
XMVECTOR    XM_CALLCONV     XMVector2RefractV(FXMVECTOR Incident, FXMVECTOR Normal, FXMVECTOR RefractionIndex);
XMVECTOR    XM_CALLCONV     XMVector2Orthogonal(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector2AngleBetweenNormalsEst(FXMVECTOR N1, FXMVECTOR N2);
XMVECTOR    XM_CALLCONV     XMVector2AngleBetweenNormals(FXMVECTOR N1, FXMVECTOR N2);
XMVECTOR    XM_CALLCONV     XMVector2AngleBetweenVectors(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVector2LinePointDistance(FXMVECTOR LinePoint1, FXMVECTOR LinePoint2, FXMVECTOR Point);
XMVECTOR    XM_CALLCONV     XMVector2IntersectLine(FXMVECTOR Line1Point1, FXMVECTOR Line1Point2, FXMVECTOR Line2Point1, GXMVECTOR Line2Point2);
XMVECTOR    XM_CALLCONV     XMVector2Transform(FXMVECTOR V, FXMMATRIX M);
XMFLOAT4*   XM_CALLCONV     XMVector2TransformStream(_Out_writes_bytes_(sizeof(XMFLOAT4) + OutputStride*(VectorCount - 1)) XMFLOAT4* pOutputStream,
	_In_ size_t OutputStride,
	_In_reads_bytes_(sizeof(XMFLOAT2) + InputStride*(VectorCount - 1)) const XMFLOAT2* pInputStream,
	_In_ size_t InputStride, _In_ size_t VectorCount, _In_ FXMMATRIX M);
XMVECTOR    XM_CALLCONV     XMVector2TransformCoord(FXMVECTOR V, FXMMATRIX M);
XMFLOAT2*   XM_CALLCONV     XMVector2TransformCoordStream(_Out_writes_bytes_(sizeof(XMFLOAT2) + OutputStride*(VectorCount - 1)) XMFLOAT2* pOutputStream,
	_In_ size_t OutputStride,
	_In_reads_bytes_(sizeof(XMFLOAT2) + InputStride*(VectorCount - 1)) const XMFLOAT2* pInputStream,
	_In_ size_t InputStride, _In_ size_t VectorCount, _In_ FXMMATRIX M);
XMVECTOR    XM_CALLCONV     XMVector2TransformNormal(FXMVECTOR V, FXMMATRIX M);
XMFLOAT2*   XM_CALLCONV     XMVector2TransformNormalStream(_Out_writes_bytes_(sizeof(XMFLOAT2) + OutputStride*(VectorCount - 1)) XMFLOAT2* pOutputStream,
	_In_ size_t OutputStride,
	_In_reads_bytes_(sizeof(XMFLOAT2) + InputStride*(VectorCount - 1)) const XMFLOAT2* pInputStream,
	_In_ size_t InputStride, _In_ size_t VectorCount, _In_ FXMMATRIX M);

/****************************************************************************
*
* 3D vector operations
*
****************************************************************************/

bool        XM_CALLCONV     XMVector3Equal(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector3EqualR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector3EqualInt(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector3EqualIntR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector3NearEqual(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR Epsilon);
bool        XM_CALLCONV     XMVector3NotEqual(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector3NotEqualInt(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector3Greater(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector3GreaterR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector3GreaterOrEqual(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector3GreaterOrEqualR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector3Less(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector3LessOrEqual(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector3InBounds(FXMVECTOR V, FXMVECTOR Bounds);

bool        XM_CALLCONV     XMVector3IsNaN(FXMVECTOR V);
bool        XM_CALLCONV     XMVector3IsInfinite(FXMVECTOR V);

XMVECTOR    XM_CALLCONV     XMVector3Dot(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVector3Cross(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVector3LengthSq(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector3ReciprocalLengthEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector3ReciprocalLength(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector3LengthEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector3Length(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector3NormalizeEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector3Normalize(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector3ClampLength(FXMVECTOR V, float LengthMin, float LengthMax);
XMVECTOR    XM_CALLCONV     XMVector3ClampLengthV(FXMVECTOR V, FXMVECTOR LengthMin, FXMVECTOR LengthMax);
XMVECTOR    XM_CALLCONV     XMVector3Reflect(FXMVECTOR Incident, FXMVECTOR Normal);
XMVECTOR    XM_CALLCONV     XMVector3Refract(FXMVECTOR Incident, FXMVECTOR Normal, float RefractionIndex);
XMVECTOR    XM_CALLCONV     XMVector3RefractV(FXMVECTOR Incident, FXMVECTOR Normal, FXMVECTOR RefractionIndex);
XMVECTOR    XM_CALLCONV     XMVector3Orthogonal(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector3AngleBetweenNormalsEst(FXMVECTOR N1, FXMVECTOR N2);
XMVECTOR    XM_CALLCONV     XMVector3AngleBetweenNormals(FXMVECTOR N1, FXMVECTOR N2);
XMVECTOR    XM_CALLCONV     XMVector3AngleBetweenVectors(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVector3LinePointDistance(FXMVECTOR LinePoint1, FXMVECTOR LinePoint2, FXMVECTOR Point);
void        XM_CALLCONV     XMVector3ComponentsFromNormal(_Out_ XMVECTOR* pParallel, _Out_ XMVECTOR* pPerpendicular, _In_ FXMVECTOR V, _In_ FXMVECTOR Normal);
XMVECTOR    XM_CALLCONV     XMVector3Rotate(FXMVECTOR V, FXMVECTOR RotationQuaternion);
XMVECTOR    XM_CALLCONV     XMVector3InverseRotate(FXMVECTOR V, FXMVECTOR RotationQuaternion);
XMVECTOR    XM_CALLCONV     XMVector3Transform(FXMVECTOR V, FXMMATRIX M);
XMFLOAT4*   XM_CALLCONV     XMVector3TransformStream(_Out_writes_bytes_(sizeof(XMFLOAT4) + OutputStride*(VectorCount - 1)) XMFLOAT4* pOutputStream,
	_In_ size_t OutputStride,
	_In_reads_bytes_(sizeof(XMFLOAT3) + InputStride*(VectorCount - 1)) const XMFLOAT3* pInputStream,
	_In_ size_t InputStride, _In_ size_t VectorCount, _In_ FXMMATRIX M);
XMVECTOR    XM_CALLCONV     XMVector3TransformCoord(FXMVECTOR V, FXMMATRIX M);
XMFLOAT3*   XM_CALLCONV     XMVector3TransformCoordStream(_Out_writes_bytes_(sizeof(XMFLOAT3) + OutputStride*(VectorCount - 1)) XMFLOAT3* pOutputStream,
	_In_ size_t OutputStride,
	_In_reads_bytes_(sizeof(XMFLOAT3) + InputStride*(VectorCount - 1)) const XMFLOAT3* pInputStream,
	_In_ size_t InputStride, _In_ size_t VectorCount, _In_ FXMMATRIX M);
XMVECTOR    XM_CALLCONV     XMVector3TransformNormal(FXMVECTOR V, FXMMATRIX M);
XMFLOAT3*   XM_CALLCONV     XMVector3TransformNormalStream(_Out_writes_bytes_(sizeof(XMFLOAT3) + OutputStride*(VectorCount - 1)) XMFLOAT3* pOutputStream,
	_In_ size_t OutputStride,
	_In_reads_bytes_(sizeof(XMFLOAT3) + InputStride*(VectorCount - 1)) const XMFLOAT3* pInputStream,
	_In_ size_t InputStride, _In_ size_t VectorCount, _In_ FXMMATRIX M);
XMVECTOR    XM_CALLCONV     XMVector3Project(FXMVECTOR V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ,
	FXMMATRIX Projection, CXMMATRIX View, CXMMATRIX World);
XMFLOAT3*   XM_CALLCONV     XMVector3ProjectStream(_Out_writes_bytes_(sizeof(XMFLOAT3) + OutputStride*(VectorCount - 1)) XMFLOAT3* pOutputStream,
	_In_ size_t OutputStride,
	_In_reads_bytes_(sizeof(XMFLOAT3) + InputStride*(VectorCount - 1)) const XMFLOAT3* pInputStream,
	_In_ size_t InputStride, _In_ size_t VectorCount,
	_In_ float ViewportX, _In_ float ViewportY, _In_ float ViewportWidth, _In_ float ViewportHeight, _In_ float ViewportMinZ, _In_ float ViewportMaxZ,
	_In_ FXMMATRIX Projection, _In_ CXMMATRIX View, _In_ CXMMATRIX World);
XMVECTOR    XM_CALLCONV     XMVector3Unproject(FXMVECTOR V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ,
	FXMMATRIX Projection, CXMMATRIX View, CXMMATRIX World);
XMFLOAT3*   XM_CALLCONV     XMVector3UnprojectStream(_Out_writes_bytes_(sizeof(XMFLOAT3) + OutputStride*(VectorCount - 1)) XMFLOAT3* pOutputStream,
	_In_ size_t OutputStride,
	_In_reads_bytes_(sizeof(XMFLOAT3) + InputStride*(VectorCount - 1)) const XMFLOAT3* pInputStream,
	_In_ size_t InputStride, _In_ size_t VectorCount,
	_In_ float ViewportX, _In_ float ViewportY, _In_ float ViewportWidth, _In_ float ViewportHeight, _In_ float ViewportMinZ, _In_ float ViewportMaxZ,
	_In_ FXMMATRIX Projection, _In_ CXMMATRIX View, _In_ CXMMATRIX World);

/****************************************************************************
*
* 4D vector operations
*
****************************************************************************/

bool        XM_CALLCONV     XMVector4Equal(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector4EqualR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector4EqualInt(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector4EqualIntR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector4NearEqual(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR Epsilon);
bool        XM_CALLCONV     XMVector4NotEqual(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector4NotEqualInt(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector4Greater(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector4GreaterR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector4GreaterOrEqual(FXMVECTOR V1, FXMVECTOR V2);
uint32_t    XM_CALLCONV     XMVector4GreaterOrEqualR(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector4Less(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector4LessOrEqual(FXMVECTOR V1, FXMVECTOR V2);
bool        XM_CALLCONV     XMVector4InBounds(FXMVECTOR V, FXMVECTOR Bounds);

bool        XM_CALLCONV     XMVector4IsNaN(FXMVECTOR V);
bool        XM_CALLCONV     XMVector4IsInfinite(FXMVECTOR V);

XMVECTOR    XM_CALLCONV     XMVector4Dot(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVector4Cross(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR V3);
XMVECTOR    XM_CALLCONV     XMVector4LengthSq(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector4ReciprocalLengthEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector4ReciprocalLength(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector4LengthEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector4Length(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector4NormalizeEst(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector4Normalize(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector4ClampLength(FXMVECTOR V, float LengthMin, float LengthMax);
XMVECTOR    XM_CALLCONV     XMVector4ClampLengthV(FXMVECTOR V, FXMVECTOR LengthMin, FXMVECTOR LengthMax);
XMVECTOR    XM_CALLCONV     XMVector4Reflect(FXMVECTOR Incident, FXMVECTOR Normal);
XMVECTOR    XM_CALLCONV     XMVector4Refract(FXMVECTOR Incident, FXMVECTOR Normal, float RefractionIndex);
XMVECTOR    XM_CALLCONV     XMVector4RefractV(FXMVECTOR Incident, FXMVECTOR Normal, FXMVECTOR RefractionIndex);
XMVECTOR    XM_CALLCONV     XMVector4Orthogonal(FXMVECTOR V);
XMVECTOR    XM_CALLCONV     XMVector4AngleBetweenNormalsEst(FXMVECTOR N1, FXMVECTOR N2);
XMVECTOR    XM_CALLCONV     XMVector4AngleBetweenNormals(FXMVECTOR N1, FXMVECTOR N2);
XMVECTOR    XM_CALLCONV     XMVector4AngleBetweenVectors(FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR    XM_CALLCONV     XMVector4Transform(FXMVECTOR V, FXMMATRIX M);
XMFLOAT4*   XM_CALLCONV     XMVector4TransformStream(_Out_writes_bytes_(sizeof(XMFLOAT4) + OutputStride*(VectorCount - 1)) XMFLOAT4* pOutputStream,
	_In_ size_t OutputStride,
	_In_reads_bytes_(sizeof(XMFLOAT4) + InputStride*(VectorCount - 1)) const XMFLOAT4* pInputStream,
	_In_ size_t InputStride, _In_ size_t VectorCount, _In_ FXMMATRIX M);



