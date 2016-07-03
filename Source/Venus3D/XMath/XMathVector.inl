//-------------------------------------------------------------------------------------
// XMathVector.inl -- SIMD C++ Math library
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//  
// Copyright (c) Microsoft Corporation. All rights reserved.
//-------------------------------------------------------------------------------------

#pragma once

#if defined(_XM_NO_INTRINSICS_)
#define XMISNAN(x)  ((*(uint32_t*)&(x) & 0x7F800000) == 0x7F800000 && (*(uint32_t*)&(x) & 0x7FFFFF) != 0)
#define XMISINF(x)  ((*(uint32_t*)&(x) & 0x7FFFFFFF) == 0x7F800000)
#endif

#if defined(_XM_SSE_INTRINSICS_)

#define XM3UNPACK3INTO4(l1,l2,l3) \
    XMVECTOR V3 = _mm_shuffle_ps(l2,l3,_MM_SHUFFLE(0,0,3,2));\
    XMVECTOR V2 = _mm_shuffle_ps(l2,l1,_MM_SHUFFLE(3,3,1,0));\
    V2 = XM_PERMUTE_PS(V2,_MM_SHUFFLE(1,1,0,2));\
    XMVECTOR V4 = _mm_castsi128_ps( _mm_srli_si128(_mm_castps_si128(L3),32/8) );

#define XM3PACK4INTO3(v2x) \
    v2x = _mm_shuffle_ps(V2,V3,_MM_SHUFFLE(1,0,2,1));\
    V2 = _mm_shuffle_ps(V2,V1,_MM_SHUFFLE(2,2,0,0));\
    V1 = _mm_shuffle_ps(V1,V2,_MM_SHUFFLE(0,2,1,0));\
    V3 = _mm_shuffle_ps(V3,V4,_MM_SHUFFLE(0,0,2,2));\
    V3 = _mm_shuffle_ps(V3,V4,_MM_SHUFFLE(2,1,2,0));\

#endif

/****************************************************************************
*
* General Vector
*
****************************************************************************/

//------------------------------------------------------------------------------
// Assignment operations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Return a vector with all elements equaling zero
inline XMVECTOR XM_CALLCONV XMVectorZero()
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult = { 0.0f,0.0f,0.0f,0.0f };
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_n_f32(0);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_setzero_ps();
#endif
}

//------------------------------------------------------------------------------
// Initialize a vector with four floating point values
inline XMVECTOR XM_CALLCONV XMVectorSet
(
	float x,
	float y,
	float z,
	float w
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTORF32 vResult = { x,y,z,w };
	return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float32x2_t V0 = vcreate_f32(((uint64_t)*(const uint32_t *)&x) | ((uint64_t)(*(const uint32_t *)&y) << 32));
	float32x2_t V1 = vcreate_f32(((uint64_t)*(const uint32_t *)&z) | ((uint64_t)(*(const uint32_t *)&w) << 32));
	return vcombine_f32(V0, V1);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_set_ps(w, z, y, x);
#endif
}

//------------------------------------------------------------------------------
// Initialize a vector with four integer values
inline XMVECTOR XM_CALLCONV XMVectorSetInt
(
	uint32_t x,
	uint32_t y,
	uint32_t z,
	uint32_t w
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTORU32 vResult = { x,y,z,w };
	return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	uint32x2_t V0 = vcreate_u32(((uint64_t)x) | ((uint64_t)y << 32));
	uint32x2_t V1 = vcreate_u32(((uint64_t)z) | ((uint64_t)w << 32));
	return vcombine_u32(V0, V1);
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i V = _mm_set_epi32(w, z, y, x);
	return _mm_castsi128_ps(V);
#endif
}

//------------------------------------------------------------------------------
// Initialize a vector with a replicated floating point value
inline XMVECTOR XM_CALLCONV XMVectorReplicate
(
	float Value
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult;
	vResult.vector4_f32[0] =
		vResult.vector4_f32[1] =
		vResult.vector4_f32[2] =
		vResult.vector4_f32[3] = Value;
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_n_f32(Value);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_set_ps1(Value);
#endif
}

//------------------------------------------------------------------------------
// Initialize a vector with a replicated floating point value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorReplicatePtr
(
	const float *pValue
)
{
#if defined(_XM_NO_INTRINSICS_)
	float Value = pValue[0];
	XMVECTOR vResult;
	vResult.vector4_f32[0] =
		vResult.vector4_f32[1] =
		vResult.vector4_f32[2] =
		vResult.vector4_f32[3] = Value;
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vld1q_dup_f32(pValue);
#elif defined(_XM_AVX_INTRINSICS_)
	return _mm_broadcast_ss(pValue);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_load_ps1(pValue);
#endif
}

//------------------------------------------------------------------------------
// Initialize a vector with a replicated integer value
inline XMVECTOR XM_CALLCONV XMVectorReplicateInt
(
	uint32_t Value
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTORU32 vResult;
	vResult.u[0] =
		vResult.u[1] =
		vResult.u[2] =
		vResult.u[3] = Value;
	return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_n_u32(Value);
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i vTemp = _mm_set1_epi32(Value);
	return _mm_castsi128_ps(vTemp);
#endif
}

//------------------------------------------------------------------------------
// Initialize a vector with a replicated integer value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorReplicateIntPtr
(
	const uint32_t *pValue
)
{
#if defined(_XM_NO_INTRINSICS_)
	uint32_t Value = pValue[0];
	XMVECTORU32 vResult;
	vResult.u[0] =
		vResult.u[1] =
		vResult.u[2] =
		vResult.u[3] = Value;
	return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vld1q_dup_u32(pValue);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_load_ps1(reinterpret_cast<const float *>(pValue));
#endif
}

//------------------------------------------------------------------------------
// Initialize a vector with all bits set (true mask)
inline XMVECTOR XM_CALLCONV XMVectorTrueInt()
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTORU32 vResult = { 0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU };
	return vResult.v;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_n_s32(-1);
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i V = _mm_set1_epi32(-1);
	return _mm_castsi128_ps(V);
#endif
}

//------------------------------------------------------------------------------
// Initialize a vector with all bits clear (false mask)
inline XMVECTOR XM_CALLCONV XMVectorFalseInt()
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult = { 0.0f,0.0f,0.0f,0.0f };
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_n_u32(0);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_setzero_ps();
#endif
}

//------------------------------------------------------------------------------
// Replicate the x component of the vector
inline XMVECTOR XM_CALLCONV XMVectorSplatX
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult;
	vResult.vector4_f32[0] =
		vResult.vector4_f32[1] =
		vResult.vector4_f32[2] =
		vResult.vector4_f32[3] = V.vector4_f32[0];
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_lane_f32(vget_low_f32(V), 0);
#elif defined(_XM_SSE_INTRINSICS_)
	return XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
#endif
}

//------------------------------------------------------------------------------
// Replicate the y component of the vector
inline XMVECTOR XM_CALLCONV XMVectorSplatY
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult;
	vResult.vector4_f32[0] =
		vResult.vector4_f32[1] =
		vResult.vector4_f32[2] =
		vResult.vector4_f32[3] = V.vector4_f32[1];
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_lane_f32(vget_low_f32(V), 1);
#elif defined(_XM_SSE_INTRINSICS_)
	return XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
#endif
}

//------------------------------------------------------------------------------
// Replicate the z component of the vector
inline XMVECTOR XM_CALLCONV XMVectorSplatZ
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult;
	vResult.vector4_f32[0] =
		vResult.vector4_f32[1] =
		vResult.vector4_f32[2] =
		vResult.vector4_f32[3] = V.vector4_f32[2];
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_lane_f32(vget_high_f32(V), 0);
#elif defined(_XM_SSE_INTRINSICS_)
	return XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
#endif
}

//------------------------------------------------------------------------------
// Replicate the w component of the vector
inline XMVECTOR XM_CALLCONV XMVectorSplatW
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult;
	vResult.vector4_f32[0] =
		vResult.vector4_f32[1] =
		vResult.vector4_f32[2] =
		vResult.vector4_f32[3] = V.vector4_f32[3];
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_lane_f32(vget_high_f32(V), 1);
#elif defined(_XM_SSE_INTRINSICS_)
	return XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
#endif
}

//------------------------------------------------------------------------------
// Return a vector of 1.0f,1.0f,1.0f,1.0f
inline XMVECTOR XM_CALLCONV XMVectorSplatOne()
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult;
	vResult.vector4_f32[0] =
		vResult.vector4_f32[1] =
		vResult.vector4_f32[2] =
		vResult.vector4_f32[3] = 1.0f;
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_n_f32(1.0f);
#elif defined(_XM_SSE_INTRINSICS_)
	return g_XMOne;
#endif
}

//------------------------------------------------------------------------------
// Return a vector of INF,INF,INF,INF
inline XMVECTOR XM_CALLCONV XMVectorSplatInfinity()
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult;
	vResult.vector4_u32[0] =
		vResult.vector4_u32[1] =
		vResult.vector4_u32[2] =
		vResult.vector4_u32[3] = 0x7F800000;
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_n_u32(0x7F800000);
#elif defined(_XM_SSE_INTRINSICS_)
	return g_XMInfinity;
#endif
}

//------------------------------------------------------------------------------
// Return a vector of Q_NAN,Q_NAN,Q_NAN,Q_NAN
inline XMVECTOR XM_CALLCONV XMVectorSplatQNaN()
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult;
	vResult.vector4_u32[0] =
		vResult.vector4_u32[1] =
		vResult.vector4_u32[2] =
		vResult.vector4_u32[3] = 0x7FC00000;
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_n_u32(0x7FC00000);
#elif defined(_XM_SSE_INTRINSICS_)
	return g_XMQNaN;
#endif
}

//------------------------------------------------------------------------------
// Return a vector of 1.192092896e-7f,1.192092896e-7f,1.192092896e-7f,1.192092896e-7f
inline XMVECTOR XM_CALLCONV XMVectorSplatEpsilon()
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult;
	vResult.vector4_u32[0] =
		vResult.vector4_u32[1] =
		vResult.vector4_u32[2] =
		vResult.vector4_u32[3] = 0x34000000;
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_n_u32(0x34000000);
#elif defined(_XM_SSE_INTRINSICS_)
	return g_XMEpsilon;
#endif
}

//------------------------------------------------------------------------------
// Return a vector of -0.0f (0x80000000),-0.0f,-0.0f,-0.0f
inline XMVECTOR XM_CALLCONV XMVectorSplatSignMask()
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult;
	vResult.vector4_u32[0] =
		vResult.vector4_u32[1] =
		vResult.vector4_u32[2] =
		vResult.vector4_u32[3] = 0x80000000U;
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vdupq_n_u32(0x80000000U);
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i V = _mm_set1_epi32(0x80000000);
	return _mm_castsi128_ps(V);
#endif
}

//------------------------------------------------------------------------------
// Return a floating point value via an index. This is not a recommended
// function to use due to performance loss.
inline float XM_CALLCONV XMVectorGetByIndex(FXMVECTOR V, size_t i)
{
	assert(i < 4);
	_Analysis_assume_(i < 4);
	switch (i)
	{
	case 0:
		return XMVectorGetX(V);
	case 1:
		return XMVectorGetY(V);
	case 2:
		return XMVectorGetZ(V);
	case 3:
		return XMVectorGetW(V);
	default:
		return NAN;
	}
}

//------------------------------------------------------------------------------
// Return the X component in an FPU register. 
inline float XM_CALLCONV XMVectorGetX(FXMVECTOR V)
{
#if defined(_XM_NO_INTRINSICS_)
	return V.vector4_f32[0];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vgetq_lane_f32(V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cvtss_f32(V);
#endif
}

// Return the Y component in an FPU register. 
inline float XM_CALLCONV XMVectorGetY(FXMVECTOR V)
{
#if defined(_XM_NO_INTRINSICS_)
	return V.vector4_f32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vgetq_lane_f32(V, 1);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	return _mm_cvtss_f32(vTemp);
#endif
}

// Return the Z component in an FPU register. 
inline float XM_CALLCONV XMVectorGetZ(FXMVECTOR V)
{
#if defined(_XM_NO_INTRINSICS_)
	return V.vector4_f32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vgetq_lane_f32(V, 2);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	return _mm_cvtss_f32(vTemp);
#endif
}

// Return the W component in an FPU register. 
inline float XM_CALLCONV XMVectorGetW(FXMVECTOR V)
{
#if defined(_XM_NO_INTRINSICS_)
	return V.vector4_f32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vgetq_lane_f32(V, 3);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
	return _mm_cvtss_f32(vTemp);
#endif
}

//------------------------------------------------------------------------------

// Store a component indexed by i into a 32 bit float location in memory.
_Use_decl_annotations_
inline void XM_CALLCONV XMVectorGetByIndexPtr(float *f, FXMVECTOR V, size_t i)
{
	assert(f != nullptr);
	assert(i < 4);
	_Analysis_assume_(i < 4);
	switch (i)
	{
	case 0:
		XMVectorGetXPtr(f, V);
		break;
	case 1:
		XMVectorGetXPtr(f, V);
		break;
	case 2:
		XMVectorGetXPtr(f, V);
		break;
	case 3:
		XMVectorGetXPtr(f, V);
		break;
	default:
		(*f) = NAN;
		break;
	}
}

//------------------------------------------------------------------------------

// Store the X component into a 32 bit float location in memory.
_Use_decl_annotations_
inline void XM_CALLCONV XMVectorGetXPtr(float *x, FXMVECTOR V)
{
	assert(x != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	*x = V.vector4_f32[0];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	vst1q_lane_f32(x, V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
	_mm_store_ss(x, V);
#endif
}

// Store the Y component into a 32 bit float location in memory.
_Use_decl_annotations_
inline void XM_CALLCONV XMVectorGetYPtr(float *y, FXMVECTOR V)
{
	assert(y != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	*y = V.vector4_f32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	vst1q_lane_f32(y, V, 1);
#elif defined(_XM_SSE4_INTRINSICS_)
	*((int*)y) = _mm_extract_ps(V, 1);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	_mm_store_ss(y, vResult);
#endif
}

// Store the Z component into a 32 bit float location in memory.
_Use_decl_annotations_
inline void XM_CALLCONV XMVectorGetZPtr(float *z, FXMVECTOR V)
{
	assert(z != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	*z = V.vector4_f32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	vst1q_lane_f32(z, V, 2);
#elif defined(_XM_SSE4_INTRINSICS_)
	*((int*)z) = _mm_extract_ps(V, 2);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(z, vResult);
#endif
}

// Store the W component into a 32 bit float location in memory.
_Use_decl_annotations_
inline void XM_CALLCONV XMVectorGetWPtr(float *w, FXMVECTOR V)
{
	assert(w != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	*w = V.vector4_f32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	vst1q_lane_f32(w, V, 3);
#elif defined(_XM_SSE4_INTRINSICS_)
	*((int*)w) = _mm_extract_ps(V, 3);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
	_mm_store_ss(w, vResult);
#endif
}

//------------------------------------------------------------------------------

// Return an integer value via an index. This is not a recommended
// function to use due to performance loss.
inline uint32_t XM_CALLCONV XMVectorGetIntByIndex(FXMVECTOR V, size_t i)
{
	assert(i < 4);
	_Analysis_assume_(i < 4);
	switch (i)
	{
	case 0:
		return XMVectorGetIntX(V);
	case 1:
		return XMVectorGetIntY(V);
	case 2:
		return XMVectorGetIntZ(V);
	case 3:
		return XMVectorGetIntW(V);
	default:
		return UINT32_MAX;
	}
}

//------------------------------------------------------------------------------

// Return the X component in an integer register. 
inline uint32_t XM_CALLCONV XMVectorGetIntX(FXMVECTOR V)
{
#if defined(_XM_NO_INTRINSICS_)
	return V.vector4_u32[0];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vgetq_lane_u32(V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
	return static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(V)));
#endif
}

// Return the Y component in an integer register. 
inline uint32_t XM_CALLCONV XMVectorGetIntY(FXMVECTOR V)
{
#if defined(_XM_NO_INTRINSICS_)
	return V.vector4_u32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vgetq_lane_u32(V, 1);
#elif defined(_XM_SSE4_INTRINSICS_)
	__m128i V1 = _mm_castps_si128(V);
	return static_cast<uint32_t>(_mm_extract_epi32(V1, 1));
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(1, 1, 1, 1));
	return static_cast<uint32_t>(_mm_cvtsi128_si32(vResulti));
#endif
}

// Return the Z component in an integer register. 
inline uint32_t XM_CALLCONV XMVectorGetIntZ(FXMVECTOR V)
{
#if defined(_XM_NO_INTRINSICS_)
	return V.vector4_u32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vgetq_lane_u32(V, 2);
#elif defined(_XM_SSE4_INTRINSICS_)
	__m128i V1 = _mm_castps_si128(V);
	return static_cast<uint32_t>(_mm_extract_epi32(V1, 2));
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(2, 2, 2, 2));
	return static_cast<uint32_t>(_mm_cvtsi128_si32(vResulti));
#endif
}

// Return the W component in an integer register. 
inline uint32_t XM_CALLCONV XMVectorGetIntW(FXMVECTOR V)
{
#if defined(_XM_NO_INTRINSICS_)
	return V.vector4_u32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vgetq_lane_u32(V, 3);
#elif defined(_XM_SSE4_INTRINSICS_)
	__m128i V1 = _mm_castps_si128(V);
	return static_cast<uint32_t>(_mm_extract_epi32(V1, 3));
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(3, 3, 3, 3));
	return static_cast<uint32_t>(_mm_cvtsi128_si32(vResulti));
#endif
}

//------------------------------------------------------------------------------

// Store a component indexed by i into a 32 bit integer location in memory.
_Use_decl_annotations_
inline void XM_CALLCONV XMVectorGetIntByIndexPtr(uint32_t *x, FXMVECTOR V, size_t i)
{
	assert(x != nullptr);
	assert(i < 4);
	_Analysis_assume_(i < 4);
	switch (i)
	{
	case 0:
		XMVectorGetIntXPtr(x, V);
		break;
	case 1:
		XMVectorGetIntYPtr(x, V);
		break;
	case 2:
		XMVectorGetIntZPtr(x, V);
		break;
	case 3:
		XMVectorGetIntWPtr(x, V);
		break;
	default:
		(*x) = UINT32_MAX;
		break;
}
}

//------------------------------------------------------------------------------

// Store the X component into a 32 bit integer location in memory.
_Use_decl_annotations_
inline void XM_CALLCONV XMVectorGetIntXPtr(uint32_t *x, FXMVECTOR V)
{
	assert(x != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	*x = V.vector4_u32[0];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	vst1q_lane_u32(x, *reinterpret_cast<const uint32x4_t*>(&V), 0);
#elif defined(_XM_SSE_INTRINSICS_)
	_mm_store_ss(reinterpret_cast<float *>(x), V);
#endif
}

// Store the Y component into a 32 bit integer location in memory.
_Use_decl_annotations_
inline void XM_CALLCONV XMVectorGetIntYPtr(uint32_t *y, FXMVECTOR V)
{
	assert(y != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	*y = V.vector4_u32[1];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	vst1q_lane_u32(y, *reinterpret_cast<const uint32x4_t*>(&V), 1);
#elif defined(_XM_SSE4_INTRINSICS_)
	__m128i V1 = _mm_castps_si128(V);
	*y = static_cast<uint32_t>(_mm_extract_epi32(V1, 1));
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	_mm_store_ss(reinterpret_cast<float *>(y), vResult);
#endif
}

// Store the Z component into a 32 bit integer locaCantion in memory.
_Use_decl_annotations_
inline void XM_CALLCONV XMVectorGetIntZPtr(uint32_t *z, FXMVECTOR V)
{
	assert(z != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	*z = V.vector4_u32[2];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	vst1q_lane_u32(z, *reinterpret_cast<const uint32x4_t*>(&V), 2);
#elif defined(_XM_SSE4_INTRINSICS_)
	__m128i V1 = _mm_castps_si128(V);
	*z = static_cast<uint32_t>(_mm_extract_epi32(V1, 2));
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(reinterpret_cast<float *>(z), vResult);
#endif
}

// Store the W component into a 32 bit integer location in memory.
_Use_decl_annotations_
inline void XM_CALLCONV XMVectorGetIntWPtr(uint32_t *w, FXMVECTOR V)
{
	assert(w != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	*w = V.vector4_u32[3];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	vst1q_lane_u32(w, *reinterpret_cast<const uint32x4_t*>(&V), 3);
#elif defined(_XM_SSE4_INTRINSICS_)
	__m128i V1 = _mm_castps_si128(V);
	*w = static_cast<uint32_t>(_mm_extract_epi32(V1, 3));
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
	_mm_store_ss(reinterpret_cast<float *>(w), vResult);
#endif
}

//------------------------------------------------------------------------------

// Set a single indexed floating point component
inline XMVECTOR XM_CALLCONV XMVectorSetByIndex(FXMVECTOR V, float f, size_t i)
{
	assert(i < 4);
	_Analysis_assume_(i < 4);
	switch (i)
	{
	case 0:
		return XMVectorSetX(V, f);
	case 1:
		return XMVectorSetY(V, f);
	case 2:
		return XMVectorSetZ(V, f);
	case 3:
		return XMVectorSetW(V, f);
	default:
		return g_XMNAN;
	}
}

//------------------------------------------------------------------------------

// Sets the X component of a vector to a passed floating point value
inline XMVECTOR XM_CALLCONV XMVectorSetX(FXMVECTOR V, float x)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_f32[0] = x;
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vsetq_lane_f32(x, V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = _mm_set_ss(x);
	vResult = _mm_move_ss(V, vResult);
	return vResult;
#endif
}

// Sets the Y component of a vector to a passed floating point value
inline XMVECTOR XM_CALLCONV XMVectorSetY(FXMVECTOR V, float y)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = y;
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vsetq_lane_f32(y, V, 1);
#elif defined(_XM_SSE4_INTRINSICS_)
	XMVECTOR vResult = _mm_set_ss(y);
	vResult = _mm_insert_ps(V, vResult, 0x10);
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap y and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
	// Convert input to vector
	XMVECTOR vTemp = _mm_set_ss(y);
	// Replace the x component
	vResult = _mm_move_ss(vResult, vTemp);
	// Swap y and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
	return vResult;
#endif
}
// Sets the Z component of a vector to a passed floating point value
inline XMVECTOR XM_CALLCONV XMVectorSetZ(FXMVECTOR V, float z)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = z;
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vsetq_lane_f32(z, V, 2);
#elif defined(_XM_SSE4_INTRINSICS_)
	XMVECTOR vResult = _mm_set_ss(z);
	vResult = _mm_insert_ps(V, vResult, 0x20);
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap z and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
	// Convert input to vector
	XMVECTOR vTemp = _mm_set_ss(z);
	// Replace the x component
	vResult = _mm_move_ss(vResult, vTemp);
	// Swap z and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
	return vResult;
#endif
}

// Sets the W component of a vector to a passed floating point value
inline XMVECTOR XM_CALLCONV XMVectorSetW(FXMVECTOR V, float w)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = w;
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vsetq_lane_f32(w, V, 3);
#elif defined(_XM_SSE4_INTRINSICS_)
	XMVECTOR vResult = _mm_set_ss(w);
	vResult = _mm_insert_ps(V, vResult, 0x30);
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap w and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
	// Convert input to vector
	XMVECTOR vTemp = _mm_set_ss(w);
	// Replace the x component
	vResult = _mm_move_ss(vResult, vTemp);
	// Swap w and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
	return vResult;
#endif
}

//------------------------------------------------------------------------------

// Sets a component of a vector to a floating point value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorSetByIndexPtr(FXMVECTOR V, const float *f, size_t i)
{
	assert(f != nullptr);
	assert(i < 4);
	_Analysis_assume_(i < 4);
	switch (i)
	{
	case 0:
		return XMVectorSetXPtr(V, f);
	case 1:
		return XMVectorSetYPtr(V, f);
	case 2:
		return XMVectorSetZPtr(V, f);
	case 3:
		return XMVectorSetWPtr(V, f);
	default:
		return g_XMNAN;
	}
}

//------------------------------------------------------------------------------

// Sets the X component of a vector to a floating point value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorSetXPtr(FXMVECTOR V, const float *x)
{
	assert(x != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_f32[0] = *x;
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vld1q_lane_f32(x, V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = _mm_load_ss(x);
	vResult = _mm_move_ss(V, vResult);
	return vResult;
#endif
}

// Sets the Y component of a vector to a floating point value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorSetYPtr(FXMVECTOR V, const float *y)
{
	assert(y != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = *y;
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vld1q_lane_f32(y, V, 1);
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap y and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
	// Convert input to vector
	XMVECTOR vTemp = _mm_load_ss(y);
	// Replace the x component
	vResult = _mm_move_ss(vResult, vTemp);
	// Swap y and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
	return vResult;
#endif
}

// Sets the Z component of a vector to a floating point value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorSetZPtr(FXMVECTOR V, const float *z)
{
	assert(z != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = *z;
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vld1q_lane_f32(z, V, 2);
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap z and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
	// Convert input to vector
	XMVECTOR vTemp = _mm_load_ss(z);
	// Replace the x component
	vResult = _mm_move_ss(vResult, vTemp);
	// Swap z and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
	return vResult;
#endif
}

// Sets the W component of a vector to a floating point value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorSetWPtr(FXMVECTOR V, const float *w)
{
	assert(w != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = *w;
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vld1q_lane_f32(w, V, 3);
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap w and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
	// Convert input to vector
	XMVECTOR vTemp = _mm_load_ss(w);
	// Replace the x component
	vResult = _mm_move_ss(vResult, vTemp);
	// Swap w and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
	return vResult;
#endif
}

//------------------------------------------------------------------------------

// Sets a component of a vector to an integer passed by value
inline XMVECTOR XM_CALLCONV XMVectorSetIntByIndex(FXMVECTOR V, uint32_t x, size_t i)
{
	assert(i < 4);
	_Analysis_assume_(i < 4);
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U = V;
	U.vector4_u32[i] = x;
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	XMVECTORU32 tmp;
	tmp.v = V;
	tmp.u[i] = x;
	return tmp;
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTORU32 tmp;
	tmp.v = V;
	tmp.u[i] = x;
	return tmp;
#endif
}

//------------------------------------------------------------------------------

// Sets the X component of a vector to an integer passed by value
inline XMVECTOR XM_CALLCONV XMVectorSetIntX(FXMVECTOR V, uint32_t x)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_u32[0] = x;
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vsetq_lane_u32(x, V, 0);
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i vTemp = _mm_cvtsi32_si128(x);
	XMVECTOR vResult = _mm_move_ss(V, _mm_castsi128_ps(vTemp));
	return vResult;
#endif
}

// Sets the Y component of a vector to an integer passed by value
inline XMVECTOR XM_CALLCONV XMVectorSetIntY(FXMVECTOR V, uint32_t y)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = y;
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vsetq_lane_u32(y, V, 1);
#elif defined(_XM_SSE4_INTRINSICS_)
	__m128i vResult = _mm_castps_si128(V);
	vResult = _mm_insert_epi32(vResult, static_cast<int>(y), 1);
	return _mm_castsi128_ps(vResult);
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap y and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
	// Convert input to vector
	__m128i vTemp = _mm_cvtsi32_si128(y);
	// Replace the x component
	vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
	// Swap y and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
	return vResult;
#endif
}

// Sets the Z component of a vector to an integer passed by value
inline XMVECTOR XM_CALLCONV XMVectorSetIntZ(FXMVECTOR V, uint32_t z)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = z;
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vsetq_lane_u32(z, V, 2);
#elif defined(_XM_SSE4_INTRINSICS_)
	__m128i vResult = _mm_castps_si128(V);
	vResult = _mm_insert_epi32(vResult, static_cast<int>(z), 2);
	return _mm_castsi128_ps(vResult);
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap z and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
	// Convert input to vector
	__m128i vTemp = _mm_cvtsi32_si128(z);
	// Replace the x component
	vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
	// Swap z and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
	return vResult;
#endif
}

// Sets the W component of a vector to an integer passed by value
inline XMVECTOR XM_CALLCONV XMVectorSetIntW(FXMVECTOR V, uint32_t w)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = w;
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vsetq_lane_u32(w, V, 3);
#elif defined(_XM_SSE4_INTRINSICS_)
	__m128i vResult = _mm_castps_si128(V);
	vResult = _mm_insert_epi32(vResult, static_cast<int>(w), 3);
	return _mm_castsi128_ps(vResult);
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap w and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
	// Convert input to vector
	__m128i vTemp = _mm_cvtsi32_si128(w);
	// Replace the x component
	vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
	// Swap w and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
	return vResult;
#endif
}

//------------------------------------------------------------------------------

// Sets a component of a vector to an integer value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorSetIntByIndexPtr(FXMVECTOR V, const uint32_t *x, size_t i)
{
	assert(x != nullptr);
	assert(i < 4);
	_Analysis_assume_(i < 4);
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U = V;
	U.vector4_u32[i] = *x;
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	XMVECTORU32 tmp;
	tmp.v = V;
	tmp.u[i] = *x;
	return tmp;
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTORU32 tmp;
	tmp.v = V;
	tmp.u[i] = *x;
	return tmp;
#endif
}

//------------------------------------------------------------------------------

// Sets the X component of a vector to an integer value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorSetIntXPtr(FXMVECTOR V, const uint32_t *x)
{
	assert(x != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_u32[0] = *x;
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vld1q_lane_u32(x, *reinterpret_cast<const uint32x4_t *>(&V), 0);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vTemp = _mm_load_ss(reinterpret_cast<const float *>(x));
	XMVECTOR vResult = _mm_move_ss(V, vTemp);
	return vResult;
#endif
}

// Sets the Y component of a vector to an integer value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorSetIntYPtr(FXMVECTOR V, const uint32_t *y)
{
	assert(y != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = *y;
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vld1q_lane_u32(y, *reinterpret_cast<const uint32x4_t *>(&V), 1);
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap y and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
	// Convert input to vector
	XMVECTOR vTemp = _mm_load_ss(reinterpret_cast<const float *>(y));
	// Replace the x component
	vResult = _mm_move_ss(vResult, vTemp);
	// Swap y and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
	return vResult;
#endif
}

// Sets the Z component of a vector to an integer value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorSetIntZPtr(FXMVECTOR V, const uint32_t *z)
{
	assert(z != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = *z;
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vld1q_lane_u32(z, *reinterpret_cast<const uint32x4_t *>(&V), 2);
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap z and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
	// Convert input to vector
	XMVECTOR vTemp = _mm_load_ss(reinterpret_cast<const float *>(z));
	// Replace the x component
	vResult = _mm_move_ss(vResult, vTemp);
	// Swap z and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
	return vResult;
#endif
}

// Sets the W component of a vector to an integer value passed by pointer
_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorSetIntWPtr(FXMVECTOR V, const uint32_t *w)
{
	assert(w != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = *w;
	return U;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vld1q_lane_u32(w, *reinterpret_cast<const uint32x4_t *>(&V), 3);
#elif defined(_XM_SSE_INTRINSICS_)
	// Swap w and x
	XMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
	// Convert input to vector
	XMVECTOR vTemp = _mm_load_ss(reinterpret_cast<const float *>(w));
	// Replace the x component
	vResult = _mm_move_ss(vResult, vTemp);
	// Swap w and x again
	vResult = XM_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorSwizzle
(
	FXMVECTOR V,
	uint32_t E0,
	uint32_t E1,
	uint32_t E2,
	uint32_t E3
)
{
	assert((E0 < 4) && (E1 < 4) && (E2 < 4) && (E3 < 4));
	_Analysis_assume_((E0 < 4) && (E1 < 4) && (E2 < 4) && (E3 < 4));
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result = { V.vector4_f32[E0],
		V.vector4_f32[E1],
		V.vector4_f32[E2],
		V.vector4_f32[E3] };
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	static const uint32_t ControlElement[4] =
	{
		0x03020100, // XM_SWIZZLE_X
		0x07060504, // XM_SWIZZLE_Y
		0x0B0A0908, // XM_SWIZZLE_Z
		0x0F0E0D0C, // XM_SWIZZLE_W
	};

	uint8x8x2_t tbl;
	tbl.val[0] = vget_low_f32(V);
	tbl.val[1] = vget_high_f32(V);

	uint32x2_t idx = vcreate_u32(((uint64_t)ControlElement[E0]) | (((uint64_t)ControlElement[E1]) << 32));
	const uint8x8_t rL = vtbl2_u8(tbl, idx);

	idx = vcreate_u32(((uint64_t)ControlElement[E2]) | (((uint64_t)ControlElement[E3]) << 32));
	const uint8x8_t rH = vtbl2_u8(tbl, idx);

	return vcombine_f32(rL, rH);
#elif defined(_XM_AVX_INTRINSICS_)
	unsigned int elem[4] = { E0, E1, E2, E3 };
	__m128i vControl = _mm_loadu_si128(reinterpret_cast<const __m128i *>(&elem[0]));
	return _mm_permutevar_ps(V, vControl);
#else
	const uint32_t *aPtr = (const uint32_t*)(&V);

	XMVECTOR Result;
	uint32_t *pWork = (uint32_t*)(&Result);

	pWork[0] = aPtr[E0];
	pWork[1] = aPtr[E1];
	pWork[2] = aPtr[E2];
	pWork[3] = aPtr[E3];

	return Result;
#endif
}

//------------------------------------------------------------------------------
inline XMVECTOR XM_CALLCONV XMVectorPermute
(
	FXMVECTOR V1,
	FXMVECTOR V2,
	uint32_t PermuteX,
	uint32_t PermuteY,
	uint32_t PermuteZ,
	uint32_t PermuteW
)
{
	assert(PermuteX <= 7 && PermuteY <= 7 && PermuteZ <= 7 && PermuteW <= 7);
	_Analysis_assume_(PermuteX <= 7 && PermuteY <= 7 && PermuteZ <= 7 && PermuteW <= 7);

#if defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
	static const uint32_t ControlElement[8] =
	{
		0x03020100, // XM_PERMUTE_0X
		0x07060504, // XM_PERMUTE_0Y
		0x0B0A0908, // XM_PERMUTE_0Z
		0x0F0E0D0C, // XM_PERMUTE_0W
		0x13121110, // XM_PERMUTE_1X
		0x17161514, // XM_PERMUTE_1Y
		0x1B1A1918, // XM_PERMUTE_1Z
		0x1F1E1D1C, // XM_PERMUTE_1W
	};

	uint8x8x4_t tbl;
	tbl.val[0] = vget_low_f32(V1);
	tbl.val[1] = vget_high_f32(V1);
	tbl.val[2] = vget_low_f32(V2);
	tbl.val[3] = vget_high_f32(V2);

	uint32x2_t idx = vcreate_u32(((uint64_t)ControlElement[PermuteX]) | (((uint64_t)ControlElement[PermuteY]) << 32));
	const uint8x8_t rL = vtbl4_u8(tbl, idx);

	idx = vcreate_u32(((uint64_t)ControlElement[PermuteZ]) | (((uint64_t)ControlElement[PermuteW]) << 32));
	const uint8x8_t rH = vtbl4_u8(tbl, idx);

	return vcombine_f32(rL, rH);
#elif defined(_XM_AVX_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
	static const XMVECTORU32 three = { 3, 3, 3, 3 };

	alignas(16) unsigned int elem[4] = { PermuteX, PermuteY, PermuteZ, PermuteW };
	__m128i vControl = _mm_load_si128(reinterpret_cast<const __m128i *>(&elem[0]));

	__m128i vSelect = _mm_cmpgt_epi32(vControl, three);
	vControl = _mm_castps_si128(_mm_and_ps(_mm_castsi128_ps(vControl), three));

	__m128 shuffled1 = _mm_permutevar_ps(V1, vControl);
	__m128 shuffled2 = _mm_permutevar_ps(V2, vControl);

	__m128 masked1 = _mm_andnot_ps(_mm_castsi128_ps(vSelect), shuffled1);
	__m128 masked2 = _mm_and_ps(_mm_castsi128_ps(vSelect), shuffled2);

	return _mm_or_ps(masked1, masked2);
#else

	const uint32_t *aPtr[2];
	aPtr[0] = (const uint32_t*)(&V1);
	aPtr[1] = (const uint32_t*)(&V2);

	XMVECTOR Result;
	uint32_t *pWork = (uint32_t*)(&Result);

	const uint32_t i0 = PermuteX & 3;
	const uint32_t vi0 = PermuteX >> 2;
	pWork[0] = aPtr[vi0][i0];

	const uint32_t i1 = PermuteY & 3;
	const uint32_t vi1 = PermuteY >> 2;
	pWork[1] = aPtr[vi1][i1];

	const uint32_t i2 = PermuteZ & 3;
	const uint32_t vi2 = PermuteZ >> 2;
	pWork[2] = aPtr[vi2][i2];

	const uint32_t i3 = PermuteW & 3;
	const uint32_t vi3 = PermuteW >> 2;
	pWork[3] = aPtr[vi3][i3];

	return Result;
#endif
}


