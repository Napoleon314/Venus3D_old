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

#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(_XM_NO_INTRINSICS_) && !defined(_XM_VECTORCALL_)
#define _XM_VECTORCALL_ 1
#endif

#if _XM_VECTORCALL_
#define XM_CALLCONV __vectorcall
#else
#define XM_CALLCONV __fastcall
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

// C4514/4820: Off by default noise
#include <math.h>
#include <float.h>
//#include <malloc.h>

#ifndef _XM_NO_INTRINSICS_
//#pragma warning(push)
//#pragma warning(disable : 4987)
// C4987: Off by default noise
//#include <intrin.h>
//#pragma warning(pop)

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

//#include <sal.h>
#include <assert.h>

#ifndef _XM_NO_ROUNDF_
#ifdef _MSC_VER
#include <yvals.h>
#if defined(_CPPLIB_VER) && ( _CPPLIB_VER < 610 )
#define _XM_NO_ROUNDF_
#endif
#endif
#endif

// C4005/4668: Old header issue
#include <stdint.h>

