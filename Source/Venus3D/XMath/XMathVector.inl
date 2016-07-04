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

//------------------------------------------------------------------------------
// Define a control vector to be used in XMVectorSelect 
// operations.  The four integers specified in XMVectorSelectControl
// serve as indices to select between components in two vectors.
// The first index controls selection for the first component of 
// the vectors involved in a select operation, the second index 
// controls selection for the second component etc.  A value of
// zero for an index causes the corresponding component from the first 
// vector to be selected whereas a one causes the component from the
// second vector to be selected instead.

inline XMVECTOR XM_CALLCONV XMVectorSelectControl
(
	uint32_t VectorIndex0,
	uint32_t VectorIndex1,
	uint32_t VectorIndex2,
	uint32_t VectorIndex3
)
{
#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
	// x=Index0,y=Index1,z=Index2,w=Index3
	__m128i vTemp = _mm_set_epi32(VectorIndex3, VectorIndex2, VectorIndex1, VectorIndex0);
	// Any non-zero entries become 0xFFFFFFFF else 0
	vTemp = _mm_cmpgt_epi32(vTemp, g_XMZero);
	return _mm_castsi128_ps(vTemp);
#elif defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
	int32x2_t V0 = vcreate_s32(((uint64_t)VectorIndex0) | ((uint64_t)VectorIndex1 << 32));
	int32x2_t V1 = vcreate_s32(((uint64_t)VectorIndex2) | ((uint64_t)VectorIndex3 << 32));
	int32x4_t vTemp = vcombine_s32(V0, V1);
	// Any non-zero entries become 0xFFFFFFFF else 0
	return vcgtq_s32(vTemp, g_XMZero);
#else
	XMVECTOR    ControlVector;
	const uint32_t  ControlElement[] =
	{
		XM_SELECT_0,
		XM_SELECT_1
	};

	assert(VectorIndex0 < 2);
	assert(VectorIndex1 < 2);
	assert(VectorIndex2 < 2);
	assert(VectorIndex3 < 2);
	_Analysis_assume_(VectorIndex0 < 2);
	_Analysis_assume_(VectorIndex1 < 2);
	_Analysis_assume_(VectorIndex2 < 2);
	_Analysis_assume_(VectorIndex3 < 2);

	ControlVector.vector4_u32[0] = ControlElement[VectorIndex0];
	ControlVector.vector4_u32[1] = ControlElement[VectorIndex1];
	ControlVector.vector4_u32[2] = ControlElement[VectorIndex2];
	ControlVector.vector4_u32[3] = ControlElement[VectorIndex3];

	return ControlVector;

#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorSelect
(
	FXMVECTOR V1,
	FXMVECTOR V2,
	FXMVECTOR Control
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_u32[0] = (V1.vector4_u32[0] & ~Control.vector4_u32[0]) | (V2.vector4_u32[0] & Control.vector4_u32[0]);
	Result.vector4_u32[1] = (V1.vector4_u32[1] & ~Control.vector4_u32[1]) | (V2.vector4_u32[1] & Control.vector4_u32[1]);
	Result.vector4_u32[2] = (V1.vector4_u32[2] & ~Control.vector4_u32[2]) | (V2.vector4_u32[2] & Control.vector4_u32[2]);
	Result.vector4_u32[3] = (V1.vector4_u32[3] & ~Control.vector4_u32[3]) | (V2.vector4_u32[3] & Control.vector4_u32[3]);
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vbslq_f32(Control, V2, V1);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vTemp1 = _mm_andnot_ps(Control, V1);
	XMVECTOR vTemp2 = _mm_and_ps(V2, Control);
	return _mm_or_ps(vTemp1, vTemp2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorMergeXY
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[0];
	Result.vector4_u32[1] = V2.vector4_u32[0];
	Result.vector4_u32[2] = V1.vector4_u32[1];
	Result.vector4_u32[3] = V2.vector4_u32[1];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vzipq_f32(V1, V2).val[0];
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_unpacklo_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorMergeZW
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[2];
	Result.vector4_u32[1] = V2.vector4_u32[2];
	Result.vector4_u32[2] = V1.vector4_u32[3];
	Result.vector4_u32[3] = V2.vector4_u32[3];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vzipq_f32(V1, V2).val[1];
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_unpackhi_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorShiftLeft(FXMVECTOR V1, FXMVECTOR V2, uint32_t Elements)
{
	assert(Elements < 4);
	_Analysis_assume_(Elements < 4);
	return XMVectorPermute(V1, V2, Elements, ((Elements)+1), ((Elements)+2), ((Elements)+3));
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorRotateLeft(FXMVECTOR V, uint32_t Elements)
{
	assert(Elements < 4);
	_Analysis_assume_(Elements < 4);
	return XMVectorSwizzle(V, Elements & 3, (Elements + 1) & 3, (Elements + 2) & 3, (Elements + 3) & 3);
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorRotateRight(FXMVECTOR V, uint32_t Elements)
{
	assert(Elements < 4);
	_Analysis_assume_(Elements < 4);
	return XMVectorSwizzle(V, (4 - (Elements)) & 3, (5 - (Elements)) & 3, (6 - (Elements)) & 3, (7 - (Elements)) & 3);
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorInsert(FXMVECTOR VD, FXMVECTOR VS, uint32_t VSLeftRotateElements,
	uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3)
{
	XMVECTOR Control = XMVectorSelectControl(Select0 & 1, Select1 & 1, Select2 & 1, Select3 & 1);
	return XMVectorSelect(VD, XMVectorRotateLeft(VS, VSLeftRotateElements), Control);
}

//------------------------------------------------------------------------------
// Comparison operations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorEqual
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] == V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] == V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] == V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] == V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vceqq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cmpeq_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorEqualR
(
	uint32_t*    pCR,
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
	assert(pCR != nullptr);
#if defined(_XM_NO_INTRINSICS_)
	uint32_t ux = (V1.vector4_f32[0] == V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	uint32_t uy = (V1.vector4_f32[1] == V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	uint32_t uz = (V1.vector4_f32[2] == V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	uint32_t uw = (V1.vector4_f32[3] == V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	uint32_t CR = 0;
	if (ux&uy&uz&uw)
	{
		// All elements are greater
		CR = XM_CRMASK_CR6TRUE;
	}
	else if (!(ux | uy | uz | uw))
	{
		// All elements are not greater
		CR = XM_CRMASK_CR6FALSE;
	}
	*pCR = CR;

	XMVECTOR Control;
	Control.vector4_u32[0] = ux;
	Control.vector4_u32[1] = uy;
	Control.vector4_u32[2] = uz;
	Control.vector4_u32[3] = uw;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	uint32x4_t vResult = vceqq_f32(V1, V2);
	uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));	
	uint32_t r = vget_lane_u32(vzip_u16(vTemp.val[0], vTemp.val[1]).val[1], 1);
	uint32_t CR = 0;
	if (r == 0xFFFFFFFFU)
	{
		// All elements are equal
		CR = XM_CRMASK_CR6TRUE;
	}
	else if (!r)
	{
		// All elements are not equal
		CR = XM_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vTemp = _mm_cmpeq_ps(V1, V2);
	uint32_t CR = 0;
	int iTest = _mm_movemask_ps(vTemp);
	if (iTest == 0xf)
	{
		CR = XM_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		// All elements are not greater
		CR = XM_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return vTemp;
#endif
}

//------------------------------------------------------------------------------
// Treat the components of the vectors as unsigned integers and
// compare individual bits between the two.  This is useful for
// comparing control vectors and result vectors returned from
// other comparison operations.

inline XMVECTOR XM_CALLCONV XMVectorEqualInt
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_u32[0] == V2.vector4_u32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_u32[1] == V2.vector4_u32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_u32[2] == V2.vector4_u32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_u32[3] == V2.vector4_u32[3]) ? 0xFFFFFFFF : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vceqq_u32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return _mm_castsi128_ps(V);
#endif
}

//------------------------------------------------------------------------------

_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorEqualIntR
(
	uint32_t*    pCR,
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
	assert(pCR != nullptr);
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control = XMVectorEqualInt(V1, V2);

	*pCR = 0;
	if (XMVector4EqualInt(Control, XMVectorTrueInt()))
	{
		// All elements are equal
		*pCR |= XM_CRMASK_CR6TRUE;
	}
	else if (XMVector4EqualInt(Control, XMVectorFalseInt()))
	{
		// All elements are not equal
		*pCR |= XM_CRMASK_CR6FALSE;
	}
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	uint32x4_t vResult = vceqq_u32(V1, V2);
	uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
	uint32_t r = vget_lane_u32(vzip_u16(vTemp.val[0], vTemp.val[1]).val[1], 1);
	uint32_t CR = 0;
	if (r == 0xFFFFFFFFU)
	{
		// All elements are equal
		CR = XM_CRMASK_CR6TRUE;
	}
	else if (!r)
	{
		// All elements are not equal
		CR = XM_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	int iTemp = _mm_movemask_ps(_mm_castsi128_ps(V));
	uint32_t CR = 0;
	if (iTemp == 0x0F)
	{
		CR = XM_CRMASK_CR6TRUE;
	}
	else if (!iTemp)
	{
		CR = XM_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return _mm_castsi128_ps(V);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorNearEqual
(
	FXMVECTOR V1,
	FXMVECTOR V2,
	FXMVECTOR Epsilon
)
{
#if defined(_XM_NO_INTRINSICS_)

	float fDeltax = V1.vector4_f32[0] - V2.vector4_f32[0];
	float fDeltay = V1.vector4_f32[1] - V2.vector4_f32[1];
	float fDeltaz = V1.vector4_f32[2] - V2.vector4_f32[2];
	float fDeltaw = V1.vector4_f32[3] - V2.vector4_f32[3];

	fDeltax = fabsf(fDeltax);
	fDeltay = fabsf(fDeltay);
	fDeltaz = fabsf(fDeltaz);
	fDeltaw = fabsf(fDeltaw);

	XMVECTOR Control;
	Control.vector4_u32[0] = (fDeltax <= Epsilon.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[1] = (fDeltay <= Epsilon.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[2] = (fDeltaz <= Epsilon.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[3] = (fDeltaw <= Epsilon.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	XMVECTOR vDelta = vsubq_f32(V1, V2);
	return vcaleq_f32(vDelta, Epsilon);
#elif defined(_XM_SSE_INTRINSICS_)
	// Get the difference
	XMVECTOR vDelta = _mm_sub_ps(V1, V2);
	// Get the absolute value of the difference
	XMVECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_sub_ps(vTemp, vDelta);
	vTemp = _mm_max_ps(vTemp, vDelta);
	vTemp = _mm_cmple_ps(vTemp, Epsilon);
	return vTemp;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorNotEqual
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] != V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] != V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] != V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] != V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vmvnq_u32(vceqq_f32(V1, V2));
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cmpneq_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorNotEqualInt
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_u32[0] != V2.vector4_u32[0]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[1] = (V1.vector4_u32[1] != V2.vector4_u32[1]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[2] = (V1.vector4_u32[2] != V2.vector4_u32[2]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[3] = (V1.vector4_u32[3] != V2.vector4_u32[3]) ? 0xFFFFFFFFU : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vmvnq_u32(vceqq_u32(V1, V2));
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return _mm_xor_ps(_mm_castsi128_ps(V), g_XMNegOneMask);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorGreater
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] > V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] > V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] > V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] > V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vcgtq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cmpgt_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorGreaterR
(
	uint32_t*    pCR,
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
	assert(pCR != nullptr);
#if defined(_XM_NO_INTRINSICS_)

	uint32_t ux = (V1.vector4_f32[0] > V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	uint32_t uy = (V1.vector4_f32[1] > V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	uint32_t uz = (V1.vector4_f32[2] > V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	uint32_t uw = (V1.vector4_f32[3] > V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	uint32_t CR = 0;
	if (ux&uy&uz&uw)
	{
		// All elements are greater
		CR = XM_CRMASK_CR6TRUE;
	}
	else if (!(ux | uy | uz | uw))
	{
		// All elements are not greater
		CR = XM_CRMASK_CR6FALSE;
	}
	*pCR = CR;

	XMVECTOR Control;
	Control.vector4_u32[0] = ux;
	Control.vector4_u32[1] = uy;
	Control.vector4_u32[2] = uz;
	Control.vector4_u32[3] = uw;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	uint32x4_t vResult = vcgtq_f32(V1, V2);
	uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));	
	uint32_t r = vget_lane_u32(vzip_u16(vTemp.val[0], vTemp.val[1]).val[1], 1);
	uint32_t CR = 0;
	if (r == 0xFFFFFFFFU)
	{
		// All elements are greater
		CR = XM_CRMASK_CR6TRUE;
	}
	else if (!r)
	{
		// All elements are not greater
		CR = XM_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vTemp = _mm_cmpgt_ps(V1, V2);
	uint32_t CR = 0;
	int iTest = _mm_movemask_ps(vTemp);
	if (iTest == 0xf)
	{
		CR = XM_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		// All elements are not greater
		CR = XM_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return vTemp;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorGreaterOrEqual
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] >= V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] >= V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] >= V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] >= V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vcgeq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cmpge_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorGreaterOrEqualR
(
	uint32_t*    pCR,
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
	assert(pCR != nullptr);
#if defined(_XM_NO_INTRINSICS_)

	uint32_t ux = (V1.vector4_f32[0] >= V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	uint32_t uy = (V1.vector4_f32[1] >= V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	uint32_t uz = (V1.vector4_f32[2] >= V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	uint32_t uw = (V1.vector4_f32[3] >= V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	uint32_t CR = 0;
	if (ux&uy&uz&uw)
	{
		// All elements are greater
		CR = XM_CRMASK_CR6TRUE;
	}
	else if (!(ux | uy | uz | uw))
	{
		// All elements are not greater
		CR = XM_CRMASK_CR6FALSE;
	}
	*pCR = CR;

	XMVECTOR Control;
	Control.vector4_u32[0] = ux;
	Control.vector4_u32[1] = uy;
	Control.vector4_u32[2] = uz;
	Control.vector4_u32[3] = uw;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	uint32x4_t vResult = vcgeq_f32(V1, V2);
	uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
	uint32_t r = vget_lane_u32(vzip_u16(vTemp.val[0], vTemp.val[1]).val[1], 1);
	uint32_t CR = 0;
	if (r == 0xFFFFFFFFU)
	{
		// All elements are greater or equal
		CR = XM_CRMASK_CR6TRUE;
	}
	else if (!r)
	{
		// All elements are not greater or equal
		CR = XM_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vTemp = _mm_cmpge_ps(V1, V2);
	uint32_t CR = 0;
	int iTest = _mm_movemask_ps(vTemp);
	if (iTest == 0xf)
	{
		CR = XM_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		// All elements are not greater
		CR = XM_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return vTemp;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorLess
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] < V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] < V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] < V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] < V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vcltq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cmplt_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorLessOrEqual
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] <= V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] <= V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] <= V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] <= V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vcleq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_cmple_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorInBounds
(
	FXMVECTOR V,
	FXMVECTOR Bounds
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control;
	Control.vector4_u32[0] = (V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V.vector4_f32[3] <= Bounds.vector4_f32[3] && V.vector4_f32[3] >= -Bounds.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Test if less than or equal
	XMVECTOR vTemp1 = vcleq_f32(V, Bounds);
	// Negate the bounds
	XMVECTOR vTemp2 = vnegq_f32(Bounds);
	// Test if greater or equal (Reversed)
	vTemp2 = vcleq_f32(vTemp2, V);
	// Blend answers
	vTemp1 = vandq_u32(vTemp1, vTemp2);
	return vTemp1;
#elif defined(_XM_SSE_INTRINSICS_)
	// Test if less than or equal
	XMVECTOR vTemp1 = _mm_cmple_ps(V, Bounds);
	// Negate the bounds
	XMVECTOR vTemp2 = _mm_mul_ps(Bounds, g_XMNegativeOne);
	// Test if greater or equal (Reversed)
	vTemp2 = _mm_cmple_ps(vTemp2, V);
	// Blend answers
	vTemp1 = _mm_and_ps(vTemp1, vTemp2);
	return vTemp1;
#endif
}

//------------------------------------------------------------------------------

_Use_decl_annotations_
inline XMVECTOR XM_CALLCONV XMVectorInBoundsR
(
	uint32_t*    pCR,
	FXMVECTOR V,
	FXMVECTOR Bounds
)
{
	assert(pCR != nullptr);
#if defined(_XM_NO_INTRINSICS_)

	uint32_t ux = (V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	uint32_t uy = (V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	uint32_t uz = (V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	uint32_t uw = (V.vector4_f32[3] <= Bounds.vector4_f32[3] && V.vector4_f32[3] >= -Bounds.vector4_f32[3]) ? 0xFFFFFFFFU : 0;

	uint32_t CR = 0;
	if (ux&uy&uz&uw)
	{
		// All elements are in bounds
		CR = XM_CRMASK_CR6BOUNDS;
	}
	*pCR = CR;

	XMVECTOR Control;
	Control.vector4_u32[0] = ux;
	Control.vector4_u32[1] = uy;
	Control.vector4_u32[2] = uz;
	Control.vector4_u32[3] = uw;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Test if less than or equal
	XMVECTOR vTemp1 = vcleq_f32(V, Bounds);
	// Negate the bounds
	XMVECTOR vTemp2 = vnegq_f32(Bounds);
	// Test if greater or equal (Reversed)
	vTemp2 = vcleq_f32(vTemp2, V);
	// Blend answers
	vTemp1 = vandq_u32(vTemp1, vTemp2);
	uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
	uint32_t r = vget_lane_u32(vzip_u16(vTemp.val[0], vTemp.val[1]).val[1], 1);
	uint32_t CR = 0;
	if (r == 0xFFFFFFFFU)
	{
		// All elements are in bounds
		CR = XM_CRMASK_CR6BOUNDS;
	}
	*pCR = CR;
	return vTemp1;
#elif defined(_XM_SSE_INTRINSICS_)
	// Test if less than or equal
	XMVECTOR vTemp1 = _mm_cmple_ps(V, Bounds);
	// Negate the bounds
	XMVECTOR vTemp2 = _mm_mul_ps(Bounds, g_XMNegativeOne);
	// Test if greater or equal (Reversed)
	vTemp2 = _mm_cmple_ps(vTemp2, V);
	// Blend answers
	vTemp1 = _mm_and_ps(vTemp1, vTemp2);

	uint32_t CR = 0;
	if (_mm_movemask_ps(vTemp1) == 0xf) {
		// All elements are in bounds
		CR = XM_CRMASK_CR6BOUNDS;
	}
	*pCR = CR;
	return vTemp1;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorIsNaN
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control;
	Control.vector4_u32[0] = XMISNAN(V.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[1] = XMISNAN(V.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[2] = XMISNAN(V.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[3] = XMISNAN(V.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Test against itself. NaN is always not equal
	uint32x4_t vTempNan = vceqq_f32(V, V);
	// Flip results
	return vmvnq_u32(vTempNan);
#elif defined(_XM_SSE_INTRINSICS_)
	// Test against itself. NaN is always not equal
	return _mm_cmpneq_ps(V, V);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorIsInfinite
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Control;
	Control.vector4_u32[0] = XMISINF(V.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[1] = XMISINF(V.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[2] = XMISINF(V.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[3] = XMISINF(V.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	return Control;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Mask off the sign bit
	uint32x4_t vTemp = vandq_u32(V, g_XMAbsMask);
	// Compare to infinity
	vTemp = vceqq_f32(vTemp, g_XMInfinity);
	// If any are infinity, the signs are true.
	return vTemp;
#elif defined(_XM_SSE_INTRINSICS_)
	// Mask off the sign bit
	__m128 vTemp = _mm_and_ps(V, g_XMAbsMask);
	// Compare to infinity
	vTemp = _mm_cmpeq_ps(vTemp, g_XMInfinity);
	// If any are infinity, the signs are true.
	return vTemp;
#endif
}

//------------------------------------------------------------------------------
// Rounding and clamping operations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorMin
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_f32[0] = (V1.vector4_f32[0] < V2.vector4_f32[0]) ? V1.vector4_f32[0] : V2.vector4_f32[0];
	Result.vector4_f32[1] = (V1.vector4_f32[1] < V2.vector4_f32[1]) ? V1.vector4_f32[1] : V2.vector4_f32[1];
	Result.vector4_f32[2] = (V1.vector4_f32[2] < V2.vector4_f32[2]) ? V1.vector4_f32[2] : V2.vector4_f32[2];
	Result.vector4_f32[3] = (V1.vector4_f32[3] < V2.vector4_f32[3]) ? V1.vector4_f32[3] : V2.vector4_f32[3];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vminq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_min_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorMax
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_f32[0] = (V1.vector4_f32[0] > V2.vector4_f32[0]) ? V1.vector4_f32[0] : V2.vector4_f32[0];
	Result.vector4_f32[1] = (V1.vector4_f32[1] > V2.vector4_f32[1]) ? V1.vector4_f32[1] : V2.vector4_f32[1];
	Result.vector4_f32[2] = (V1.vector4_f32[2] > V2.vector4_f32[2]) ? V1.vector4_f32[2] : V2.vector4_f32[2];
	Result.vector4_f32[3] = (V1.vector4_f32[3] > V2.vector4_f32[3]) ? V1.vector4_f32[3] : V2.vector4_f32[3];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vmaxq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_max_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

#ifdef _XM_NO_ROUNDF_

namespace Internal
{
	inline float round_to_nearest(float x)
	{
		float i = floorf(x);
		x -= i;
		if (x < 0.5f)
			return i;
		if (x > 0.5f)
			return i + 1.f;

		float int_part;
		modff(i / 2.f, &int_part);
		if ((2.f*int_part) == i)
		{
			return i;
		}

		return i + 1.f;
	}
};

#endif

#if !defined(_XM_NO_INTRINSICS_) && defined(_MSC_VER)
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

inline XMVECTOR XM_CALLCONV XMVectorRound
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

#ifdef _XM_NO_ROUNDF_
	XMVECTOR Result;
	Result.vector4_f32[0] = Internal::round_to_nearest(V.vector4_f32[0]);
	Result.vector4_f32[1] = Internal::round_to_nearest(V.vector4_f32[1]);
	Result.vector4_f32[2] = Internal::round_to_nearest(V.vector4_f32[2]);
	Result.vector4_f32[3] = Internal::round_to_nearest(V.vector4_f32[3]);
	return Result;
#else
	XMVECTOR Result;
	Result.vector4_f32[0] = roundf(V.vector4_f32[0]);
	Result.vector4_f32[1] = roundf(V.vector4_f32[1]);
	Result.vector4_f32[2] = roundf(V.vector4_f32[2]);
	Result.vector4_f32[3] = roundf(V.vector4_f32[3]);
	return Result;
#endif

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	uint32x4_t sign = vandq_u32(V, g_XMNegativeZero);
	uint32x4_t sMagic = vorrq_u32(g_XMNoFraction, sign);
	float32x4_t R1 = vaddq_f32(V, sMagic);
	R1 = vsubq_f32(R1, sMagic);
	float32x4_t R2 = vabsq_f32(V);
	uint32x4_t mask = vcleq_f32(R2, g_XMNoFraction);
	XMVECTOR vResult = vbslq_f32(mask, R1, V);
	return vResult;
#elif defined(_XM_SSE4_INTRINSICS_)
	return _mm_round_ps(V, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
#elif defined(_XM_SSE_INTRINSICS_)
	__m128 sign = _mm_and_ps(V, g_XMNegativeZero);
	__m128 sMagic = _mm_or_ps(g_XMNoFraction, sign);
	__m128 R1 = _mm_add_ps(V, sMagic);
	R1 = _mm_sub_ps(R1, sMagic);
	__m128 R2 = _mm_and_ps(V, g_XMAbsMask);
	__m128 mask = _mm_cmple_ps(R2, g_XMNoFraction);
	R2 = _mm_andnot_ps(mask, V);
	R1 = _mm_and_ps(R1, mask);
	XMVECTOR vResult = _mm_xor_ps(R1, R2);
	return vResult;
#endif
}

#if !defined(_XM_NO_INTRINSICS_) && defined(_MSC_VER)
#pragma float_control(pop)
#endif

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorTruncate
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	uint32_t     i;

	// Avoid C4701
	Result.vector4_f32[0] = 0.0f;

	for (i = 0; i < 4; i++)
	{
		if (XMISNAN(V.vector4_f32[i]))
		{
			Result.vector4_u32[i] = 0x7FC00000;
		}
		else if (fabsf(V.vector4_f32[i]) < 8388608.0f)
		{
			Result.vector4_f32[i] = (float)((int32_t)V.vector4_f32[i]);
		}
		else
		{
			Result.vector4_f32[i] = V.vector4_f32[i];
		}
	}
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float32x4_t vTest = vabsq_f32(V);
	vTest = vcltq_f32(vTest, g_XMNoFraction);

	int32x4_t vInt = vcvtq_s32_f32(V);
	XMVECTOR vResult = vcvtq_f32_s32(vInt);

	// All numbers less than 8388608 will use the round to int
	// All others, use the ORIGINAL value
	return vbslq_f32(vTest, vResult, V);
#elif defined(_XM_SSE4_INTRINSICS_)
	return _mm_round_ps(V, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
#elif defined(_XM_SSE_INTRINSICS_)
	// To handle NAN, INF and numbers greater than 8388608, use masking
	// Get the abs value
	__m128i vTest = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
	// Test for greater than 8388608 (All floats with NO fractionals, NAN and INF
	vTest = _mm_cmplt_epi32(vTest, g_XMNoFraction);
	// Convert to int and back to float for rounding with truncation
	__m128i vInt = _mm_cvttps_epi32(V);
	// Convert back to floats
	XMVECTOR vResult = _mm_cvtepi32_ps(vInt);
	// All numbers less than 8388608 will use the round to int
	vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
	// All others, use the ORIGINAL value
	vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
	vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorFloor
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = floorf(V.vector4_f32[0]);
	Result.vector4_f32[1] = floorf(V.vector4_f32[1]);
	Result.vector4_f32[2] = floorf(V.vector4_f32[2]);
	Result.vector4_f32[3] = floorf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float32x4_t vTest = vabsq_f32(V);
	vTest = vcltq_f32(vTest, g_XMNoFraction);
	// Truncate
	int32x4_t vInt = vcvtq_s32_f32(V);
	XMVECTOR vResult = vcvtq_f32_s32(vInt);
	XMVECTOR vLarger = vcgtq_f32(vResult, V);
	// 0 -> 0, 0xffffffff -> -1.0f
	vLarger = vcvtq_f32_s32(vLarger);
	vResult = vaddq_f32(vResult, vLarger);
	// All numbers less than 8388608 will use the round to int
	// All others, use the ORIGINAL value
	return vbslq_f32(vTest, vResult, V);
#elif defined(_XM_SSE4_INTRINSICS_)
	return _mm_floor_ps(V);
#elif defined(_XM_SSE_INTRINSICS_)
	// To handle NAN, INF and numbers greater than 8388608, use masking
	__m128i vTest = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
	vTest = _mm_cmplt_epi32(vTest, g_XMNoFraction);
	// Truncate
	__m128i vInt = _mm_cvttps_epi32(V);
	XMVECTOR vResult = _mm_cvtepi32_ps(vInt);
	__m128 vLarger = _mm_cmpgt_ps(vResult, V);
	// 0 -> 0, 0xffffffff -> -1.0f
	vLarger = _mm_cvtepi32_ps(_mm_castps_si128(vLarger));
	vResult = _mm_add_ps(vResult, vLarger);
	// All numbers less than 8388608 will use the round to int
	vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
	// All others, use the ORIGINAL value
	vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
	vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorCeiling
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = ceilf(V.vector4_f32[0]);
	Result.vector4_f32[1] = ceilf(V.vector4_f32[1]);
	Result.vector4_f32[2] = ceilf(V.vector4_f32[2]);
	Result.vector4_f32[3] = ceilf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float32x4_t vTest = vabsq_f32(V);
	vTest = vcltq_f32(vTest, g_XMNoFraction);
	// Truncate
	int32x4_t vInt = vcvtq_s32_f32(V);
	XMVECTOR vResult = vcvtq_f32_s32(vInt);
	XMVECTOR vSmaller = vcltq_f32(vResult, V);
	// 0 -> 0, 0xffffffff -> -1.0f
	vSmaller = vcvtq_f32_s32(vSmaller);
	vResult = vsubq_f32(vResult, vSmaller);
	// All numbers less than 8388608 will use the round to int
	// All others, use the ORIGINAL value
	return vbslq_f32(vTest, vResult, V);
#elif defined(_XM_SSE4_INTRINSICS_)
	return _mm_ceil_ps(V);
#elif defined(_XM_SSE_INTRINSICS_)
	// To handle NAN, INF and numbers greater than 8388608, use masking
	__m128i vTest = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
	vTest = _mm_cmplt_epi32(vTest, g_XMNoFraction);
	// Truncate
	__m128i vInt = _mm_cvttps_epi32(V);
	XMVECTOR vResult = _mm_cvtepi32_ps(vInt);
	__m128 vSmaller = _mm_cmplt_ps(vResult, V);
	// 0 -> 0, 0xffffffff -> -1.0f
	vSmaller = _mm_cvtepi32_ps(_mm_castps_si128(vSmaller));
	vResult = _mm_sub_ps(vResult, vSmaller);
	// All numbers less than 8388608 will use the round to int
	vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
	// All others, use the ORIGINAL value
	vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
	vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorClamp
(
	FXMVECTOR V,
	FXMVECTOR Min,
	FXMVECTOR Max
)
{
	assert(XMVector4LessOrEqual(Min, Max));

#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result = XMVectorMax(Min, V);
	Result = XMVectorMin(Max, Result);
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	XMVECTOR vResult;
	vResult = vmaxq_f32(Min, V);
	vResult = vminq_f32(vResult, Max);
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult;
	vResult = _mm_max_ps(Min, V);
	vResult = _mm_min_ps(vResult, Max);
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorSaturate
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

	const XMVECTOR Zero = XMVectorZero();

	return XMVectorClamp(V, Zero, g_XMOne.v);

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Set <0 to 0
	XMVECTOR vResult = vmaxq_f32(V, vdupq_n_f32(0));
	// Set>1 to 1
	return vminq_f32(vResult, vdupq_n_f32(1.0f));
#elif defined(_XM_SSE_INTRINSICS_)
	// Set <0 to 0
	XMVECTOR vResult = _mm_max_ps(V, g_XMZero);
	// Set>1 to 1
	return _mm_min_ps(vResult, g_XMOne);
#endif
}

//------------------------------------------------------------------------------
// Bitwise logical operations
//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorAndInt
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[0] & V2.vector4_u32[0];
	Result.vector4_u32[1] = V1.vector4_u32[1] & V2.vector4_u32[1];
	Result.vector4_u32[2] = V1.vector4_u32[2] & V2.vector4_u32[2];
	Result.vector4_u32[3] = V1.vector4_u32[3] & V2.vector4_u32[3];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vandq_u32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_and_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorAndCInt
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[0] & ~V2.vector4_u32[0];
	Result.vector4_u32[1] = V1.vector4_u32[1] & ~V2.vector4_u32[1];
	Result.vector4_u32[2] = V1.vector4_u32[2] & ~V2.vector4_u32[2];
	Result.vector4_u32[3] = V1.vector4_u32[3] & ~V2.vector4_u32[3];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vbicq_u32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i V = _mm_andnot_si128(_mm_castps_si128(V2), _mm_castps_si128(V1));
	return _mm_castsi128_ps(V);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorOrInt
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[0] | V2.vector4_u32[0];
	Result.vector4_u32[1] = V1.vector4_u32[1] | V2.vector4_u32[1];
	Result.vector4_u32[2] = V1.vector4_u32[2] | V2.vector4_u32[2];
	Result.vector4_u32[3] = V1.vector4_u32[3] | V2.vector4_u32[3];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vorrq_u32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i V = _mm_or_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return _mm_castsi128_ps(V);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorNorInt
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_u32[0] = ~(V1.vector4_u32[0] | V2.vector4_u32[0]);
	Result.vector4_u32[1] = ~(V1.vector4_u32[1] | V2.vector4_u32[1]);
	Result.vector4_u32[2] = ~(V1.vector4_u32[2] | V2.vector4_u32[2]);
	Result.vector4_u32[3] = ~(V1.vector4_u32[3] | V2.vector4_u32[3]);
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	uint32x4_t Result = vorrq_u32(V1, V2);
	return vbicq_u32(g_XMNegOneMask, Result);
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i Result;
	Result = _mm_or_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
	Result = _mm_andnot_si128(Result, g_XMNegOneMask);
	return _mm_castsi128_ps(Result);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorXorInt
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[0] ^ V2.vector4_u32[0];
	Result.vector4_u32[1] = V1.vector4_u32[1] ^ V2.vector4_u32[1];
	Result.vector4_u32[2] = V1.vector4_u32[2] ^ V2.vector4_u32[2];
	Result.vector4_u32[3] = V1.vector4_u32[3] ^ V2.vector4_u32[3];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return veorq_u32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i V = _mm_xor_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return _mm_castsi128_ps(V);
#endif
}

//------------------------------------------------------------------------------
// Computation operations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorNegate
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_f32[0] = -V.vector4_f32[0];
	Result.vector4_f32[1] = -V.vector4_f32[1];
	Result.vector4_f32[2] = -V.vector4_f32[2];
	Result.vector4_f32[3] = -V.vector4_f32[3];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vnegq_f32(V);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR Z;

	Z = _mm_setzero_ps();

	return _mm_sub_ps(Z, V);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorAdd
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_f32[0] = V1.vector4_f32[0] + V2.vector4_f32[0];
	Result.vector4_f32[1] = V1.vector4_f32[1] + V2.vector4_f32[1];
	Result.vector4_f32[2] = V1.vector4_f32[2] + V2.vector4_f32[2];
	Result.vector4_f32[3] = V1.vector4_f32[3] + V2.vector4_f32[3];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vaddq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_add_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorSum
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_f32[0] =
		Result.vector4_f32[1] =
		Result.vector4_f32[2] =
		Result.vector4_f32[3] = V.vector4_f32[0] + V.vector4_f32[1] + V.vector4_f32[2] + V.vector4_f32[3];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float32x2_t v1 = vget_low_f32(V);
	float32x2_t v2 = vget_high_f32(V);
	v1 = vadd_f32(v1, v2);
	v1 = vpadd_f32(v1, v1);
	return vcombine_f32(v1, v1);
#elif defined(_XM_SSE3_INTRINSICS_)
	XMVECTOR vTemp = _mm_hadd_ps(V, V);
	return _mm_hadd_ps(vTemp, vTemp);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 3, 0, 1));
	XMVECTOR vTemp2 = _mm_add_ps(V, vTemp);
	vTemp = XM_PERMUTE_PS(vTemp2, _MM_SHUFFLE(1, 0, 3, 2));
	return _mm_add_ps(vTemp, vTemp2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorAddAngles
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	const XMVECTOR Zero = XMVectorZero();

	// Add the given angles together.  If the range of V1 is such
	// that -Pi <= V1 < Pi and the range of V2 is such that
	// -2Pi <= V2 <= 2Pi, then the range of the resulting angle
	// will be -Pi <= Result < Pi.
	XMVECTOR Result = XMVectorAdd(V1, V2);

	XMVECTOR Mask = XMVectorLess(Result, g_XMNegativePi.v);
	XMVECTOR Offset = XMVectorSelect(Zero, g_XMTwoPi.v, Mask);

	Mask = XMVectorGreaterOrEqual(Result, g_XMPi.v);
	Offset = XMVectorSelect(Offset, g_XMNegativeTwoPi.v, Mask);

	Result = XMVectorAdd(Result, Offset);

	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Adjust the angles
	XMVECTOR vResult = vaddq_f32(V1, V2);
	// Less than Pi?
	uint32x4_t vOffset = vcltq_f32(vResult, g_XMNegativePi);
	vOffset = vandq_u32(vOffset, g_XMTwoPi);
	// Add 2Pi to all entries less than -Pi
	vResult = vaddq_f32(vResult, vOffset);
	// Greater than or equal to Pi?
	vOffset = vcgeq_f32(vResult, g_XMPi);
	vOffset = vandq_u32(vOffset, g_XMTwoPi);
	// Sub 2Pi to all entries greater than Pi
	vResult = vsubq_f32(vResult, vOffset);
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	// Adjust the angles
	XMVECTOR vResult = _mm_add_ps(V1, V2);
	// Less than Pi?
	XMVECTOR vOffset = _mm_cmplt_ps(vResult, g_XMNegativePi);
	vOffset = _mm_and_ps(vOffset, g_XMTwoPi);
	// Add 2Pi to all entries less than -Pi
	vResult = _mm_add_ps(vResult, vOffset);
	// Greater than or equal to Pi?
	vOffset = _mm_cmpge_ps(vResult, g_XMPi);
	vOffset = _mm_and_ps(vOffset, g_XMTwoPi);
	// Sub 2Pi to all entries greater than Pi
	vResult = _mm_sub_ps(vResult, vOffset);
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorSubtract
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_f32[0] = V1.vector4_f32[0] - V2.vector4_f32[0];
	Result.vector4_f32[1] = V1.vector4_f32[1] - V2.vector4_f32[1];
	Result.vector4_f32[2] = V1.vector4_f32[2] - V2.vector4_f32[2];
	Result.vector4_f32[3] = V1.vector4_f32[3] - V2.vector4_f32[3];
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vsubq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_sub_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorSubtractAngles
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	const XMVECTOR Zero = XMVectorZero();

	// Subtract the given angles.  If the range of V1 is such
	// that -Pi <= V1 < Pi and the range of V2 is such that
	// -2Pi <= V2 <= 2Pi, then the range of the resulting angle
	// will be -Pi <= Result < Pi.
	XMVECTOR Result = XMVectorSubtract(V1, V2);

	XMVECTOR Mask = XMVectorLess(Result, g_XMNegativePi.v);
	XMVECTOR Offset = XMVectorSelect(Zero, g_XMTwoPi.v, Mask);

	Mask = XMVectorGreaterOrEqual(Result, g_XMPi.v);
	Offset = XMVectorSelect(Offset, g_XMNegativeTwoPi.v, Mask);

	Result = XMVectorAdd(Result, Offset);

	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Adjust the angles
	XMVECTOR vResult = vsubq_f32(V1, V2);
	// Less than Pi?
	uint32x4_t vOffset = vcltq_f32(vResult, g_XMNegativePi);
	vOffset = vandq_u32(vOffset, g_XMTwoPi);
	// Add 2Pi to all entries less than -Pi
	vResult = vaddq_f32(vResult, vOffset);
	// Greater than or equal to Pi?
	vOffset = vcgeq_f32(vResult, g_XMPi);
	vOffset = vandq_u32(vOffset, g_XMTwoPi);
	// Sub 2Pi to all entries greater than Pi
	vResult = vsubq_f32(vResult, vOffset);
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	// Adjust the angles
	XMVECTOR vResult = _mm_sub_ps(V1, V2);
	// Less than Pi?
	XMVECTOR vOffset = _mm_cmplt_ps(vResult, g_XMNegativePi);
	vOffset = _mm_and_ps(vOffset, g_XMTwoPi);
	// Add 2Pi to all entries less than -Pi
	vResult = _mm_add_ps(vResult, vOffset);
	// Greater than or equal to Pi?
	vOffset = _mm_cmpge_ps(vResult, g_XMPi);
	vOffset = _mm_and_ps(vOffset, g_XMTwoPi);
	// Sub 2Pi to all entries greater than Pi
	vResult = _mm_sub_ps(vResult, vOffset);
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorMultiply
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = V1.vector4_f32[0] * V2.vector4_f32[0];
	Result.vector4_f32[1] = V1.vector4_f32[1] * V2.vector4_f32[1];
	Result.vector4_f32[2] = V1.vector4_f32[2] * V2.vector4_f32[2];
	Result.vector4_f32[3] = V1.vector4_f32[3] * V2.vector4_f32[3];
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vmulq_f32(V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_mul_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorMultiplyAdd
(
	FXMVECTOR V1,
	FXMVECTOR V2,
	FXMVECTOR V3
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = V1.vector4_f32[0] * V2.vector4_f32[0] + V3.vector4_f32[0];
	Result.vector4_f32[1] = V1.vector4_f32[1] * V2.vector4_f32[1] + V3.vector4_f32[1];
	Result.vector4_f32[2] = V1.vector4_f32[2] * V2.vector4_f32[2] + V3.vector4_f32[2];
	Result.vector4_f32[3] = V1.vector4_f32[3] * V2.vector4_f32[3] + V3.vector4_f32[3];
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vmlaq_f32(V3, V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = _mm_mul_ps(V1, V2);
	return _mm_add_ps(vResult, V3);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorDivide
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = V1.vector4_f32[0] / V2.vector4_f32[0];
	Result.vector4_f32[1] = V1.vector4_f32[1] / V2.vector4_f32[1];
	Result.vector4_f32[2] = V1.vector4_f32[2] / V2.vector4_f32[2];
	Result.vector4_f32[3] = V1.vector4_f32[3] / V2.vector4_f32[3];
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// 2 iterations of Newton-Raphson refinement of reciprocal
	float32x4_t Reciprocal = vrecpeq_f32(V2);
	float32x4_t S = vrecpsq_f32(Reciprocal, V2);
	Reciprocal = vmulq_f32(S, Reciprocal);
	S = vrecpsq_f32(Reciprocal, V2);
	Reciprocal = vmulq_f32(S, Reciprocal);
	return vmulq_f32(V1, Reciprocal);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_div_ps(V1, V2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorNegativeMultiplySubtract
(
	FXMVECTOR V1,
	FXMVECTOR V2,
	FXMVECTOR V3
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = V3.vector4_f32[0] - (V1.vector4_f32[0] * V2.vector4_f32[0]);
	Result.vector4_f32[1] = V3.vector4_f32[1] - (V1.vector4_f32[1] * V2.vector4_f32[1]);
	Result.vector4_f32[2] = V3.vector4_f32[2] - (V1.vector4_f32[2] * V2.vector4_f32[2]);
	Result.vector4_f32[3] = V3.vector4_f32[3] - (V1.vector4_f32[3] * V2.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vmlsq_f32(V3, V1, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR R = _mm_mul_ps(V1, V2);
	return _mm_sub_ps(V3, R);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorScale
(
	FXMVECTOR V,
	float    ScaleFactor
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = V.vector4_f32[0] * ScaleFactor;
	Result.vector4_f32[1] = V.vector4_f32[1] * ScaleFactor;
	Result.vector4_f32[2] = V.vector4_f32[2] * ScaleFactor;
	Result.vector4_f32[3] = V.vector4_f32[3] * ScaleFactor;
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vmulq_n_f32(V, ScaleFactor);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = _mm_set_ps1(ScaleFactor);
	return _mm_mul_ps(vResult, V);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorReciprocalEst
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = 1.f / V.vector4_f32[0];
	Result.vector4_f32[1] = 1.f / V.vector4_f32[1];
	Result.vector4_f32[2] = 1.f / V.vector4_f32[2];
	Result.vector4_f32[3] = 1.f / V.vector4_f32[3];
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vrecpeq_f32(V);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_rcp_ps(V);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorReciprocal
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = 1.f / V.vector4_f32[0];
	Result.vector4_f32[1] = 1.f / V.vector4_f32[1];
	Result.vector4_f32[2] = 1.f / V.vector4_f32[2];
	Result.vector4_f32[3] = 1.f / V.vector4_f32[3];
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// 2 iterations of Newton-Raphson refinement
	float32x4_t Reciprocal = vrecpeq_f32(V);
	float32x4_t S = vrecpsq_f32(Reciprocal, V);
	Reciprocal = vmulq_f32(S, Reciprocal);
	S = vrecpsq_f32(Reciprocal, V);
	return vmulq_f32(S, Reciprocal);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_div_ps(g_XMOne, V);
#endif
}

//------------------------------------------------------------------------------
// Return an estimated square root
inline XMVECTOR XM_CALLCONV XMVectorSqrtEst
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = sqrtf(V.vector4_f32[0]);
	Result.vector4_f32[1] = sqrtf(V.vector4_f32[1]);
	Result.vector4_f32[2] = sqrtf(V.vector4_f32[2]);
	Result.vector4_f32[3] = sqrtf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// 1 iteration of Newton-Raphson refinment of sqrt
	float32x4_t S0 = vrsqrteq_f32(V);
	float32x4_t P0 = vmulq_f32(V, S0);
	float32x4_t R0 = vrsqrtsq_f32(P0, S0);
	float32x4_t S1 = vmulq_f32(S0, R0);

	XMVECTOR VEqualsInfinity = XMVectorEqualInt(V, g_XMInfinity.v);
	XMVECTOR VEqualsZero = XMVectorEqual(V, vdupq_n_f32(0));
	XMVECTOR Result = vmulq_f32(V, S1);
	XMVECTOR Select = XMVectorEqualInt(VEqualsInfinity, VEqualsZero);
	return XMVectorSelect(V, Result, Select);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_sqrt_ps(V);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorSqrt
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = sqrtf(V.vector4_f32[0]);
	Result.vector4_f32[1] = sqrtf(V.vector4_f32[1]);
	Result.vector4_f32[2] = sqrtf(V.vector4_f32[2]);
	Result.vector4_f32[3] = sqrtf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// 3 iterations of Newton-Raphson refinment of sqrt
	float32x4_t S0 = vrsqrteq_f32(V);
	float32x4_t P0 = vmulq_f32(V, S0);
	float32x4_t R0 = vrsqrtsq_f32(P0, S0);
	float32x4_t S1 = vmulq_f32(S0, R0);
	float32x4_t P1 = vmulq_f32(V, S1);
	float32x4_t R1 = vrsqrtsq_f32(P1, S1);
	float32x4_t S2 = vmulq_f32(S1, R1);
	float32x4_t P2 = vmulq_f32(V, S2);
	float32x4_t R2 = vrsqrtsq_f32(P2, S2);
	float32x4_t S3 = vmulq_f32(S2, R2);

	XMVECTOR VEqualsInfinity = XMVectorEqualInt(V, g_XMInfinity.v);
	XMVECTOR VEqualsZero = XMVectorEqual(V, vdupq_n_f32(0));
	XMVECTOR Result = vmulq_f32(V, S3);
	XMVECTOR Select = XMVectorEqualInt(VEqualsInfinity, VEqualsZero);
	return XMVectorSelect(V, Result, Select);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_sqrt_ps(V);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorReciprocalSqrtEst
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = 1.f / sqrtf(V.vector4_f32[0]);
	Result.vector4_f32[1] = 1.f / sqrtf(V.vector4_f32[1]);
	Result.vector4_f32[2] = 1.f / sqrtf(V.vector4_f32[2]);
	Result.vector4_f32[3] = 1.f / sqrtf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vrsqrteq_f32(V);
#elif defined(_XM_SSE_INTRINSICS_)
	return _mm_rsqrt_ps(V);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorReciprocalSqrt
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = 1.f / sqrtf(V.vector4_f32[0]);
	Result.vector4_f32[1] = 1.f / sqrtf(V.vector4_f32[1]);
	Result.vector4_f32[2] = 1.f / sqrtf(V.vector4_f32[2]);
	Result.vector4_f32[3] = 1.f / sqrtf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// 2 iterations of Newton-Raphson refinement of reciprocal
	float32x4_t S0 = vrsqrteq_f32(V);

	float32x4_t P0 = vmulq_f32(V, S0);
	float32x4_t R0 = vrsqrtsq_f32(P0, S0);

	float32x4_t S1 = vmulq_f32(S0, R0);
	float32x4_t P1 = vmulq_f32(V, S1);
	float32x4_t R1 = vrsqrtsq_f32(P1, S1);

	return vmulq_f32(S1, R1);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = _mm_sqrt_ps(V);
	vResult = _mm_div_ps(g_XMOne, vResult);
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorExp2
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_f32[0] = powf(2.0f, V.vector4_f32[0]);
	Result.vector4_f32[1] = powf(2.0f, V.vector4_f32[1]);
	Result.vector4_f32[2] = powf(2.0f, V.vector4_f32[2]);
	Result.vector4_f32[3] = powf(2.0f, V.vector4_f32[3]);
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	int32x4_t itrunc = vcvtq_s32_f32(V);
	float32x4_t ftrunc = vcvtq_f32_s32(itrunc);
	float32x4_t y = vsubq_f32(V, ftrunc);

	float32x4_t poly = vmlaq_f32(g_XMExpEst6, g_XMExpEst7, y);
	poly = vmlaq_f32(g_XMExpEst5, poly, y);
	poly = vmlaq_f32(g_XMExpEst4, poly, y);
	poly = vmlaq_f32(g_XMExpEst3, poly, y);
	poly = vmlaq_f32(g_XMExpEst2, poly, y);
	poly = vmlaq_f32(g_XMExpEst1, poly, y);
	poly = vmlaq_f32(g_XMOne, poly, y);

	int32x4_t biased = vaddq_s32(itrunc, g_XMExponentBias);
	biased = vshlq_n_s32(biased, 23);
	float32x4_t result0 = XMVectorDivide(biased, poly);

	biased = vaddq_s32(itrunc, g_XM253);
	biased = vshlq_n_s32(biased, 23);
	float32x4_t result1 = XMVectorDivide(biased, poly);
	result1 = vmulq_f32(g_XMMinNormal.v, result1);

	// Use selection to handle the cases
	//  if (V is NaN) -> QNaN;
	//  else if (V sign bit set)
	//      if (V > -150)
	//         if (V.exponent < -126) -> result1
	//         else -> result0
	//      else -> +0
	//  else
	//      if (V < 128) -> result0
	//      else -> +inf

	int32x4_t comp = vcltq_s32(V, g_XMBin128);
	float32x4_t result2 = vbslq_f32(comp, result0, g_XMInfinity);

	comp = vcltq_s32(itrunc, g_XMSubnormalExponent);
	float32x4_t result3 = vbslq_f32(comp, result1, result0);

	comp = vcltq_s32(V, g_XMBinNeg150);
	float32x4_t result4 = vbslq_f32(comp, result3, g_XMZero);

	int32x4_t sign = vandq_s32(V, g_XMNegativeZero);
	comp = vceqq_s32(sign, g_XMNegativeZero);
	float32x4_t result5 = vbslq_f32(comp, result4, result2);

	int32x4_t t0 = vandq_s32(V, g_XMQNaNTest);
	int32x4_t t1 = vandq_s32(V, g_XMInfinity);
	t0 = vceqq_s32(t0, g_XMZero);
	t1 = vceqq_s32(t1, g_XMInfinity);
	int32x4_t isNaN = vbicq_s32(t1, t0);

	float32x4_t vResult = vbslq_f32(isNaN, g_XMQNaN, result5);
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i itrunc = _mm_cvttps_epi32(V);
	__m128 ftrunc = _mm_cvtepi32_ps(itrunc);
	__m128 y = _mm_sub_ps(V, ftrunc);
	__m128 poly = _mm_mul_ps(g_XMExpEst7, y);
	poly = _mm_add_ps(g_XMExpEst6, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMExpEst5, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMExpEst4, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMExpEst3, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMExpEst2, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMExpEst1, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMOne, poly);

	__m128i biased = _mm_add_epi32(itrunc, g_XMExponentBias);
	biased = _mm_slli_epi32(biased, 23);
	__m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);

	biased = _mm_add_epi32(itrunc, g_XM253);
	biased = _mm_slli_epi32(biased, 23);
	__m128 result1 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
	result1 = _mm_mul_ps(g_XMMinNormal.v, result1);

	// Use selection to handle the cases
	//  if (V is NaN) -> QNaN;
	//  else if (V sign bit set)
	//      if (V > -150)
	//         if (V.exponent < -126) -> result1
	//         else -> result0
	//      else -> +0
	//  else
	//      if (V < 128) -> result0
	//      else -> +inf

	__m128i comp = _mm_cmplt_epi32(_mm_castps_si128(V), g_XMBin128);
	__m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
	__m128i select1 = _mm_andnot_si128(comp, g_XMInfinity);
	__m128i result2 = _mm_or_si128(select0, select1);

	comp = _mm_cmplt_epi32(itrunc, g_XMSubnormalExponent);
	select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
	select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
	__m128i result3 = _mm_or_si128(select0, select1);

	comp = _mm_cmplt_epi32(_mm_castps_si128(V), g_XMBinNeg150);
	select0 = _mm_and_si128(comp, result3);
	select1 = _mm_andnot_si128(comp, g_XMZero);
	__m128i result4 = _mm_or_si128(select0, select1);

	__m128i sign = _mm_and_si128(_mm_castps_si128(V), g_XMNegativeZero);
	comp = _mm_cmpeq_epi32(sign, g_XMNegativeZero);
	select0 = _mm_and_si128(comp, result4);
	select1 = _mm_andnot_si128(comp, result2);
	__m128i result5 = _mm_or_si128(select0, select1);

	__m128i t0 = _mm_and_si128(_mm_castps_si128(V), g_XMQNaNTest);
	__m128i t1 = _mm_and_si128(_mm_castps_si128(V), g_XMInfinity);
	t0 = _mm_cmpeq_epi32(t0, g_XMZero);
	t1 = _mm_cmpeq_epi32(t1, g_XMInfinity);
	__m128i isNaN = _mm_andnot_si128(t0, t1);

	select0 = _mm_and_si128(isNaN, g_XMQNaN);
	select1 = _mm_andnot_si128(isNaN, result5);
	__m128i vResult = _mm_or_si128(select0, select1);

	return _mm_castsi128_ps(vResult);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorExpE
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_f32[0] = expf(V.vector4_f32[0]);
	Result.vector4_f32[1] = expf(V.vector4_f32[1]);
	Result.vector4_f32[2] = expf(V.vector4_f32[2]);
	Result.vector4_f32[3] = expf(V.vector4_f32[3]);
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// expE(V) = exp2(vin*log2(e))
	float32x4_t Ve = vmulq_f32(g_XMLgE, V);

	int32x4_t itrunc = vcvtq_s32_f32(Ve);
	float32x4_t ftrunc = vcvtq_f32_s32(itrunc);
	float32x4_t y = vsubq_f32(Ve, ftrunc);


	float32x4_t poly = vmlaq_f32(g_XMExpEst6, g_XMExpEst7, y);
	poly = vmlaq_f32(g_XMExpEst5, poly, y);
	poly = vmlaq_f32(g_XMExpEst4, poly, y);
	poly = vmlaq_f32(g_XMExpEst3, poly, y);
	poly = vmlaq_f32(g_XMExpEst2, poly, y);
	poly = vmlaq_f32(g_XMExpEst1, poly, y);
	poly = vmlaq_f32(g_XMOne, poly, y);

	int32x4_t biased = vaddq_s32(itrunc, g_XMExponentBias);
	biased = vshlq_n_s32(biased, 23);
	float32x4_t result0 = XMVectorDivide(biased, poly);

	biased = vaddq_s32(itrunc, g_XM253);
	biased = vshlq_n_s32(biased, 23);
	float32x4_t result1 = XMVectorDivide(biased, poly);
	result1 = vmulq_f32(g_XMMinNormal.v, result1);

	// Use selection to handle the cases
	//  if (V is NaN) -> QNaN;
	//  else if (V sign bit set)
	//      if (V > -150)
	//         if (V.exponent < -126) -> result1
	//         else -> result0
	//      else -> +0
	//  else
	//      if (V < 128) -> result0
	//      else -> +inf

	int32x4_t comp = vcltq_s32(Ve, g_XMBin128);
	float32x4_t result2 = vbslq_f32(comp, result0, g_XMInfinity);

	comp = vcltq_s32(itrunc, g_XMSubnormalExponent);
	float32x4_t result3 = vbslq_f32(comp, result1, result0);

	comp = vcltq_s32(Ve, g_XMBinNeg150);
	float32x4_t result4 = vbslq_f32(comp, result3, g_XMZero);

	int32x4_t sign = vandq_s32(Ve, g_XMNegativeZero);
	comp = vceqq_s32(sign, g_XMNegativeZero);
	float32x4_t result5 = vbslq_f32(comp, result4, result2);

	int32x4_t t0 = vandq_s32(Ve, g_XMQNaNTest);
	int32x4_t t1 = vandq_s32(Ve, g_XMInfinity);
	t0 = vceqq_s32(t0, g_XMZero);
	t1 = vceqq_s32(t1, g_XMInfinity);
	int32x4_t isNaN = vbicq_s32(t1, t0);

	float32x4_t vResult = vbslq_f32(isNaN, g_XMQNaN, result5);
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	// expE(V) = exp2(vin*log2(e))
	__m128 Ve = _mm_mul_ps(g_XMLgE, V);

	__m128i itrunc = _mm_cvttps_epi32(Ve);
	__m128 ftrunc = _mm_cvtepi32_ps(itrunc);
	__m128 y = _mm_sub_ps(Ve, ftrunc);
	__m128 poly = _mm_mul_ps(g_XMExpEst7, y);
	poly = _mm_add_ps(g_XMExpEst6, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMExpEst5, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMExpEst4, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMExpEst3, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMExpEst2, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMExpEst1, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_XMOne, poly);

	__m128i biased = _mm_add_epi32(itrunc, g_XMExponentBias);
	biased = _mm_slli_epi32(biased, 23);
	__m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);

	biased = _mm_add_epi32(itrunc, g_XM253);
	biased = _mm_slli_epi32(biased, 23);
	__m128 result1 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
	result1 = _mm_mul_ps(g_XMMinNormal.v, result1);

	// Use selection to handle the cases
	//  if (V is NaN) -> QNaN;
	//  else if (V sign bit set)
	//      if (V > -150)
	//         if (V.exponent < -126) -> result1
	//         else -> result0
	//      else -> +0
	//  else
	//      if (V < 128) -> result0
	//      else -> +inf

	__m128i comp = _mm_cmplt_epi32(_mm_castps_si128(Ve), g_XMBin128);
	__m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
	__m128i select1 = _mm_andnot_si128(comp, g_XMInfinity);
	__m128i result2 = _mm_or_si128(select0, select1);

	comp = _mm_cmplt_epi32(itrunc, g_XMSubnormalExponent);
	select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
	select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
	__m128i result3 = _mm_or_si128(select0, select1);

	comp = _mm_cmplt_epi32(_mm_castps_si128(Ve), g_XMBinNeg150);
	select0 = _mm_and_si128(comp, result3);
	select1 = _mm_andnot_si128(comp, g_XMZero);
	__m128i result4 = _mm_or_si128(select0, select1);

	__m128i sign = _mm_and_si128(_mm_castps_si128(Ve), g_XMNegativeZero);
	comp = _mm_cmpeq_epi32(sign, g_XMNegativeZero);
	select0 = _mm_and_si128(comp, result4);
	select1 = _mm_andnot_si128(comp, result2);
	__m128i result5 = _mm_or_si128(select0, select1);

	__m128i t0 = _mm_and_si128(_mm_castps_si128(Ve), g_XMQNaNTest);
	__m128i t1 = _mm_and_si128(_mm_castps_si128(Ve), g_XMInfinity);
	t0 = _mm_cmpeq_epi32(t0, g_XMZero);
	t1 = _mm_cmpeq_epi32(t1, g_XMInfinity);
	__m128i isNaN = _mm_andnot_si128(t0, t1);

	select0 = _mm_and_si128(isNaN, g_XMQNaN);
	select1 = _mm_andnot_si128(isNaN, result5);
	__m128i vResult = _mm_or_si128(select0, select1);

	return _mm_castsi128_ps(vResult);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorExp
(
	FXMVECTOR V
)
{
	return XMVectorExp2(V);
}

//------------------------------------------------------------------------------

#if defined(_XM_SSE_INTRINSICS_)

namespace Internal
{
	inline __m128i multi_sll_epi32(__m128i value, __m128i count)
	{
		__m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
		__m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
		c = _mm_and_si128(c, g_XMMaskX);
		__m128i r0 = _mm_sll_epi32(v, c);

		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
		c = _mm_and_si128(c, g_XMMaskX);
		__m128i r1 = _mm_sll_epi32(v, c);

		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
		c = _mm_and_si128(c, g_XMMaskX);
		__m128i r2 = _mm_sll_epi32(v, c);

		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
		c = _mm_and_si128(c, g_XMMaskX);
		__m128i r3 = _mm_sll_epi32(v, c);

		// (r0,r0,r1,r1)
		__m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
		// (r2,r2,r3,r3)
		__m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
		// (r0,r1,r2,r3)
		__m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
		return _mm_castps_si128(result);
	}

	inline __m128i multi_srl_epi32(__m128i value, __m128i count)
	{
		__m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
		__m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
		c = _mm_and_si128(c, g_XMMaskX);
		__m128i r0 = _mm_srl_epi32(v, c);

		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
		c = _mm_and_si128(c, g_XMMaskX);
		__m128i r1 = _mm_srl_epi32(v, c);

		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
		c = _mm_and_si128(c, g_XMMaskX);
		__m128i r2 = _mm_srl_epi32(v, c);

		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
		c = _mm_and_si128(c, g_XMMaskX);
		__m128i r3 = _mm_srl_epi32(v, c);

		// (r0,r0,r1,r1)
		__m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
		// (r2,r2,r3,r3)
		__m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
		// (r0,r1,r2,r3)
		__m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
		return _mm_castps_si128(result);
	}

	inline __m128i GetLeadingBit(const __m128i value)
	{
		static const XMVECTORI32 g_XM0000FFFF = { 0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF };
		static const XMVECTORI32 g_XM000000FF = { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF };
		static const XMVECTORI32 g_XM0000000F = { 0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F };
		static const XMVECTORI32 g_XM00000003 = { 0x00000003, 0x00000003, 0x00000003, 0x00000003 };

		__m128i v = value, r, c, b, s;

		c = _mm_cmpgt_epi32(v, g_XM0000FFFF);   // c = (v > 0xFFFF)
		b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
		r = _mm_slli_epi32(b, 4);               // r = (b << 4)
		v = multi_srl_epi32(v, r);              // v = (v >> r)

		c = _mm_cmpgt_epi32(v, g_XM000000FF);   // c = (v > 0xFF)
		b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
		s = _mm_slli_epi32(b, 3);               // s = (b << 3)
		v = multi_srl_epi32(v, s);              // v = (v >> s)
		r = _mm_or_si128(r, s);                 // r = (r | s)

		c = _mm_cmpgt_epi32(v, g_XM0000000F);   // c = (v > 0xF)
		b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
		s = _mm_slli_epi32(b, 2);               // s = (b << 2)
		v = multi_srl_epi32(v, s);              // v = (v >> s)
		r = _mm_or_si128(r, s);                 // r = (r | s)

		c = _mm_cmpgt_epi32(v, g_XM00000003);   // c = (v > 0x3)
		b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
		s = _mm_slli_epi32(b, 1);               // s = (b << 1)
		v = multi_srl_epi32(v, s);              // v = (v >> s)
		r = _mm_or_si128(r, s);                 // r = (r | s)

		s = _mm_srli_epi32(v, 1);
		r = _mm_or_si128(r, s);
		return r;
	}
} // namespace Internal

#endif // _XM_SSE_INTRINSICS_

#if defined(_XM_ARM_NEON_INTRINSICS_)

namespace Internal
{
	inline int32x4_t GetLeadingBit(const int32x4_t value)
	{
		static const XMVECTORI32 g_XM0000FFFF = { 0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF };
		static const XMVECTORI32 g_XM000000FF = { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF };
		static const XMVECTORI32 g_XM0000000F = { 0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F };
		static const XMVECTORI32 g_XM00000003 = { 0x00000003, 0x00000003, 0x00000003, 0x00000003 };

		int32x4_t v = value, r, c, b, s;

		c = vcgtq_s32(v, g_XM0000FFFF);     // c = (v > 0xFFFF)
		b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
		r = vshlq_n_s32(b, 4);              // r = (b << 4)
		r = vnegq_s32(r);
		v = vshlq_u32(v, r);              // v = (v >> r)

		c = vcgtq_s32(v, g_XM000000FF);     // c = (v > 0xFF)
		b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
		s = vshlq_n_s32(b, 3);              // s = (b << 3)
		s = vnegq_s32(s);
		v = vshlq_u32(v, s);                // v = (v >> s)
		r = vorrq_s32(r, s);                // r = (r | s)

		c = vcgtq_s32(v, g_XM0000000F);     // c = (v > 0xF)
		b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
		s = vshlq_n_s32(b, 2);              // s = (b << 2)
		s = vnegq_s32(s);
		v = vshlq_u32(v, s);                // v = (v >> s)
		r = vorrq_s32(r, s);                // r = (r | s)

		c = vcgtq_s32(v, g_XM00000003);     // c = (v > 0x3)
		b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
		s = vshlq_n_s32(b, 1);              // s = (b << 1)
		s = vnegq_s32(s);
		v = vshlq_u32(v, s);                // v = (v >> s)
		r = vorrq_s32(r, s);                // r = (r | s)

		s = vshrq_n_u32(v, 1);
		r = vorrq_s32(r, s);
		return r;
	}

} // namespace Internal

#endif

  //------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorLog2
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

	const float fScale = 1.4426950f; // (1.0f / logf(2.0f));

	XMVECTOR Result;
	Result.vector4_f32[0] = logf(V.vector4_f32[0])*fScale;
	Result.vector4_f32[1] = logf(V.vector4_f32[1])*fScale;
	Result.vector4_f32[2] = logf(V.vector4_f32[2])*fScale;
	Result.vector4_f32[3] = logf(V.vector4_f32[3])*fScale;
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	int32x4_t rawBiased = vandq_s32(V, g_XMInfinity);
	int32x4_t trailing = vandq_s32(V, g_XMQNaNTest);
	int32x4_t isExponentZero = vceqq_s32(g_XMZero, rawBiased);

	// Compute exponent and significand for normals.
	int32x4_t biased = vshrq_n_u32(rawBiased, 23);
	int32x4_t exponentNor = vsubq_s32(biased, g_XMExponentBias);
	int32x4_t trailingNor = trailing;

	// Compute exponent and significand for subnormals.
	int32x4_t leading = Internal::GetLeadingBit(trailing);
	int32x4_t shift = vsubq_s32(g_XMNumTrailing, leading);
	int32x4_t exponentSub = vsubq_s32(g_XMSubnormalExponent, shift);
	int32x4_t trailingSub = vshlq_u32(trailing, shift);
	trailingSub = vandq_s32(trailingSub, g_XMQNaNTest);
	int32x4_t e = vbslq_f32(isExponentZero, exponentSub, exponentNor);
	int32x4_t t = vbslq_f32(isExponentZero, trailingSub, trailingNor);

	// Compute the approximation.
	int32x4_t tmp = vorrq_s32(g_XMOne, t);
	float32x4_t y = vsubq_f32(tmp, g_XMOne);

	float32x4_t log2 = vmlaq_f32(g_XMLogEst6, g_XMLogEst7, y);
	log2 = vmlaq_f32(g_XMLogEst5, log2, y);
	log2 = vmlaq_f32(g_XMLogEst4, log2, y);
	log2 = vmlaq_f32(g_XMLogEst3, log2, y);
	log2 = vmlaq_f32(g_XMLogEst2, log2, y);
	log2 = vmlaq_f32(g_XMLogEst1, log2, y);
	log2 = vmlaq_f32(g_XMLogEst0, log2, y);
	log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

	//  if (x is NaN) -> QNaN
	//  else if (V is positive)
	//      if (V is infinite) -> +inf
	//      else -> log2(V)
	//  else
	//      if (V is zero) -> -inf
	//      else -> -QNaN

	int32x4_t isInfinite = vandq_s32((V), g_XMAbsMask);
	isInfinite = vceqq_s32(isInfinite, g_XMInfinity);

	int32x4_t isGreaterZero = vcgtq_s32((V), g_XMZero);
	int32x4_t isNotFinite = vcgtq_s32((V), g_XMInfinity);
	int32x4_t isPositive = vbicq_s32(isGreaterZero, isNotFinite);

	int32x4_t isZero = vandq_s32((V), g_XMAbsMask);
	isZero = vceqq_s32(isZero, g_XMZero);

	int32x4_t t0 = vandq_s32((V), g_XMQNaNTest);
	int32x4_t t1 = vandq_s32((V), g_XMInfinity);
	t0 = vceqq_s32(t0, g_XMZero);
	t1 = vceqq_s32(t1, g_XMInfinity);
	int32x4_t isNaN = vbicq_s32(t1, t0);

	float32x4_t result = vbslq_f32(isInfinite, g_XMInfinity, log2);
	tmp = vbslq_f32(isZero, g_XMNegInfinity, g_XMNegQNaN);
	result = vbslq_f32(isPositive, result, tmp);
	result = vbslq_f32(isNaN, g_XMQNaN, result);
	return result;
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i rawBiased = _mm_and_si128(_mm_castps_si128(V), g_XMInfinity);
	__m128i trailing = _mm_and_si128(_mm_castps_si128(V), g_XMQNaNTest);
	__m128i isExponentZero = _mm_cmpeq_epi32(g_XMZero, rawBiased);

	// Compute exponent and significand for normals.
	__m128i biased = _mm_srli_epi32(rawBiased, 23);
	__m128i exponentNor = _mm_sub_epi32(biased, g_XMExponentBias);
	__m128i trailingNor = trailing;

	// Compute exponent and significand for subnormals.
	__m128i leading = Internal::GetLeadingBit(trailing);
	__m128i shift = _mm_sub_epi32(g_XMNumTrailing, leading);
	__m128i exponentSub = _mm_sub_epi32(g_XMSubnormalExponent, shift);
	__m128i trailingSub = Internal::multi_sll_epi32(trailing, shift);
	trailingSub = _mm_and_si128(trailingSub, g_XMQNaNTest);

	__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
	__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
	__m128i e = _mm_or_si128(select0, select1);

	select0 = _mm_and_si128(isExponentZero, trailingSub);
	select1 = _mm_andnot_si128(isExponentZero, trailingNor);
	__m128i t = _mm_or_si128(select0, select1);

	// Compute the approximation.
	__m128i tmp = _mm_or_si128(g_XMOne, t);
	__m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), g_XMOne);

	__m128 log2 = _mm_mul_ps(g_XMLogEst7, y);
	log2 = _mm_add_ps(g_XMLogEst6, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst5, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst4, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst3, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst2, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst1, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst0, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(log2, _mm_cvtepi32_ps(e));

	//  if (x is NaN) -> QNaN
	//  else if (V is positive)
	//      if (V is infinite) -> +inf
	//      else -> log2(V)
	//  else
	//      if (V is zero) -> -inf
	//      else -> -QNaN

	__m128i isInfinite = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
	isInfinite = _mm_cmpeq_epi32(isInfinite, g_XMInfinity);

	__m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(V), g_XMZero);
	__m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(V), g_XMInfinity);
	__m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

	__m128i isZero = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
	isZero = _mm_cmpeq_epi32(isZero, g_XMZero);

	__m128i t0 = _mm_and_si128(_mm_castps_si128(V), g_XMQNaNTest);
	__m128i t1 = _mm_and_si128(_mm_castps_si128(V), g_XMInfinity);
	t0 = _mm_cmpeq_epi32(t0, g_XMZero);
	t1 = _mm_cmpeq_epi32(t1, g_XMInfinity);
	__m128i isNaN = _mm_andnot_si128(t0, t1);

	select0 = _mm_and_si128(isInfinite, g_XMInfinity);
	select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
	__m128i result = _mm_or_si128(select0, select1);

	select0 = _mm_and_si128(isZero, g_XMNegInfinity);
	select1 = _mm_andnot_si128(isZero, g_XMNegQNaN);
	tmp = _mm_or_si128(select0, select1);

	select0 = _mm_and_si128(isPositive, result);
	select1 = _mm_andnot_si128(isPositive, tmp);
	result = _mm_or_si128(select0, select1);

	select0 = _mm_and_si128(isNaN, g_XMQNaN);
	select1 = _mm_andnot_si128(isNaN, result);
	result = _mm_or_si128(select0, select1);

	return _mm_castsi128_ps(result);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorLogE
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_f32[0] = logf(V.vector4_f32[0]);
	Result.vector4_f32[1] = logf(V.vector4_f32[1]);
	Result.vector4_f32[2] = logf(V.vector4_f32[2]);
	Result.vector4_f32[3] = logf(V.vector4_f32[3]);
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	int32x4_t rawBiased = vandq_s32(V, g_XMInfinity);
	int32x4_t trailing = vandq_s32(V, g_XMQNaNTest);
	int32x4_t isExponentZero = vceqq_s32(g_XMZero, rawBiased);

	// Compute exponent and significand for normals.
	int32x4_t biased = vshrq_n_u32(rawBiased, 23);
	int32x4_t exponentNor = vsubq_s32(biased, g_XMExponentBias);
	int32x4_t trailingNor = trailing;

	// Compute exponent and significand for subnormals.
	int32x4_t leading = Internal::GetLeadingBit(trailing);
	int32x4_t shift = vsubq_s32(g_XMNumTrailing, leading);
	int32x4_t exponentSub = vsubq_s32(g_XMSubnormalExponent, shift);
	int32x4_t trailingSub = vshlq_u32(trailing, shift);
	trailingSub = vandq_s32(trailingSub, g_XMQNaNTest);
	int32x4_t e = vbslq_f32(isExponentZero, exponentSub, exponentNor);
	int32x4_t t = vbslq_f32(isExponentZero, trailingSub, trailingNor);

	// Compute the approximation.
	int32x4_t tmp = vorrq_s32(g_XMOne, t);
	float32x4_t y = vsubq_f32(tmp, g_XMOne);

	float32x4_t log2 = vmlaq_f32(g_XMLogEst6, g_XMLogEst7, y);
	log2 = vmlaq_f32(g_XMLogEst5, log2, y);
	log2 = vmlaq_f32(g_XMLogEst4, log2, y);
	log2 = vmlaq_f32(g_XMLogEst3, log2, y);
	log2 = vmlaq_f32(g_XMLogEst2, log2, y);
	log2 = vmlaq_f32(g_XMLogEst1, log2, y);
	log2 = vmlaq_f32(g_XMLogEst0, log2, y);
	log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

	log2 = vmulq_f32(g_XMInvLgE, log2);

	//  if (x is NaN) -> QNaN
	//  else if (V is positive)
	//      if (V is infinite) -> +inf
	//      else -> log2(V)
	//  else
	//      if (V is zero) -> -inf
	//      else -> -QNaN

	int32x4_t isInfinite = vandq_s32((V), g_XMAbsMask);
	isInfinite = vceqq_s32(isInfinite, g_XMInfinity);

	int32x4_t isGreaterZero = vcgtq_s32((V), g_XMZero);
	int32x4_t isNotFinite = vcgtq_s32((V), g_XMInfinity);
	int32x4_t isPositive = vbicq_s32(isGreaterZero, isNotFinite);

	int32x4_t isZero = vandq_s32((V), g_XMAbsMask);
	isZero = vceqq_s32(isZero, g_XMZero);

	int32x4_t t0 = vandq_s32((V), g_XMQNaNTest);
	int32x4_t t1 = vandq_s32((V), g_XMInfinity);
	t0 = vceqq_s32(t0, g_XMZero);
	t1 = vceqq_s32(t1, g_XMInfinity);
	int32x4_t isNaN = vbicq_s32(t1, t0);

	float32x4_t result = vbslq_f32(isInfinite, g_XMInfinity, log2);
	tmp = vbslq_f32(isZero, g_XMNegInfinity, g_XMNegQNaN);
	result = vbslq_f32(isPositive, result, tmp);
	result = vbslq_f32(isNaN, g_XMQNaN, result);
	return result;
#elif defined(_XM_SSE_INTRINSICS_)
	__m128i rawBiased = _mm_and_si128(_mm_castps_si128(V), g_XMInfinity);
	__m128i trailing = _mm_and_si128(_mm_castps_si128(V), g_XMQNaNTest);
	__m128i isExponentZero = _mm_cmpeq_epi32(g_XMZero, rawBiased);

	// Compute exponent and significand for normals.
	__m128i biased = _mm_srli_epi32(rawBiased, 23);
	__m128i exponentNor = _mm_sub_epi32(biased, g_XMExponentBias);
	__m128i trailingNor = trailing;

	// Compute exponent and significand for subnormals.
	__m128i leading = Internal::GetLeadingBit(trailing);
	__m128i shift = _mm_sub_epi32(g_XMNumTrailing, leading);
	__m128i exponentSub = _mm_sub_epi32(g_XMSubnormalExponent, shift);
	__m128i trailingSub = Internal::multi_sll_epi32(trailing, shift);
	trailingSub = _mm_and_si128(trailingSub, g_XMQNaNTest);

	__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
	__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
	__m128i e = _mm_or_si128(select0, select1);

	select0 = _mm_and_si128(isExponentZero, trailingSub);
	select1 = _mm_andnot_si128(isExponentZero, trailingNor);
	__m128i t = _mm_or_si128(select0, select1);

	// Compute the approximation.
	__m128i tmp = _mm_or_si128(g_XMOne, t);
	__m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), g_XMOne);

	__m128 log2 = _mm_mul_ps(g_XMLogEst7, y);
	log2 = _mm_add_ps(g_XMLogEst6, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst5, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst4, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst3, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst2, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst1, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_XMLogEst0, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(log2, _mm_cvtepi32_ps(e));

	log2 = _mm_mul_ps(g_XMInvLgE, log2);

	//  if (x is NaN) -> QNaN
	//  else if (V is positive)
	//      if (V is infinite) -> +inf
	//      else -> log2(V)
	//  else
	//      if (V is zero) -> -inf
	//      else -> -QNaN

	__m128i isInfinite = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
	isInfinite = _mm_cmpeq_epi32(isInfinite, g_XMInfinity);

	__m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(V), g_XMZero);
	__m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(V), g_XMInfinity);
	__m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

	__m128i isZero = _mm_and_si128(_mm_castps_si128(V), g_XMAbsMask);
	isZero = _mm_cmpeq_epi32(isZero, g_XMZero);

	__m128i t0 = _mm_and_si128(_mm_castps_si128(V), g_XMQNaNTest);
	__m128i t1 = _mm_and_si128(_mm_castps_si128(V), g_XMInfinity);
	t0 = _mm_cmpeq_epi32(t0, g_XMZero);
	t1 = _mm_cmpeq_epi32(t1, g_XMInfinity);
	__m128i isNaN = _mm_andnot_si128(t0, t1);

	select0 = _mm_and_si128(isInfinite, g_XMInfinity);
	select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
	__m128i result = _mm_or_si128(select0, select1);

	select0 = _mm_and_si128(isZero, g_XMNegInfinity);
	select1 = _mm_andnot_si128(isZero, g_XMNegQNaN);
	tmp = _mm_or_si128(select0, select1);

	select0 = _mm_and_si128(isPositive, result);
	select1 = _mm_andnot_si128(isPositive, tmp);
	result = _mm_or_si128(select0, select1);

	select0 = _mm_and_si128(isNaN, g_XMQNaN);
	select1 = _mm_andnot_si128(isNaN, result);
	result = _mm_or_si128(select0, select1);

	return _mm_castsi128_ps(result);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorLog
(
	FXMVECTOR V
)
{
	return XMVectorLog2(V);
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorPow
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Result;
	Result.vector4_f32[0] = powf(V1.vector4_f32[0], V2.vector4_f32[0]);
	Result.vector4_f32[1] = powf(V1.vector4_f32[1], V2.vector4_f32[1]);
	Result.vector4_f32[2] = powf(V1.vector4_f32[2], V2.vector4_f32[2]);
	Result.vector4_f32[3] = powf(V1.vector4_f32[3], V2.vector4_f32[3]);
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	XMVECTORF32 vResult = {
		powf(vgetq_lane_f32(V1, 0), vgetq_lane_f32(V2, 0)),
		powf(vgetq_lane_f32(V1, 1), vgetq_lane_f32(V2, 1)),
		powf(vgetq_lane_f32(V1, 2), vgetq_lane_f32(V2, 2)),
		powf(vgetq_lane_f32(V1, 3), vgetq_lane_f32(V2, 3))
	};
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	alignas(16) float a[4];
	alignas(16) float b[4];
	_mm_store_ps(a, V1);
	_mm_store_ps(b, V2);
	XMVECTOR vResult = _mm_setr_ps(
		powf(a[0], b[0]),
		powf(a[1], b[1]),
		powf(a[2], b[2]),
		powf(a[3], b[3]));
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorAbs
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR vResult;
	vResult.vector4_f32[0] = fabsf(V.vector4_f32[0]);
	vResult.vector4_f32[1] = fabsf(V.vector4_f32[1]);
	vResult.vector4_f32[2] = fabsf(V.vector4_f32[2]);
	vResult.vector4_f32[3] = fabsf(V.vector4_f32[3]);
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	return vabsq_f32(V);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = _mm_setzero_ps();
	vResult = _mm_sub_ps(vResult, V);
	vResult = _mm_max_ps(vResult, V);
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorMod
(
	FXMVECTOR V1,
	FXMVECTOR V2
)
{
	// V1 % V2 = V1 - V2 * truncate(V1 / V2)

#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Quotient = XMVectorDivide(V1, V2);
	Quotient = XMVectorTruncate(Quotient);
	XMVECTOR Result = XMVectorNegativeMultiplySubtract(V2, Quotient, V1);
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	XMVECTOR vResult = XMVectorDivide(V1, V2);
	vResult = XMVectorTruncate(vResult);
	return vmlsq_f32(V1, vResult, V2);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR vResult = _mm_div_ps(V1, V2);
	vResult = XMVectorTruncate(vResult);
	vResult = _mm_mul_ps(vResult, V2);
	vResult = _mm_sub_ps(V1, vResult);
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorModAngles
(
	FXMVECTOR Angles
)
{
#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR V;
	XMVECTOR Result;

	// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
	V = XMVectorMultiply(Angles, g_XMReciprocalTwoPi.v);
	V = XMVectorRound(V);
	Result = XMVectorNegativeMultiplySubtract(g_XMTwoPi.v, V, Angles);
	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
	XMVECTOR vResult = vmulq_f32(Angles, g_XMReciprocalTwoPi);
	// Use the inline function due to complexity for rounding
	vResult = XMVectorRound(vResult);
	return vmlsq_f32(Angles, vResult, g_XMTwoPi);
#elif defined(_XM_SSE_INTRINSICS_)
	// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
	XMVECTOR vResult = _mm_mul_ps(Angles, g_XMReciprocalTwoPi);
	// Use the inline function due to complexity for rounding
	vResult = XMVectorRound(vResult);
	vResult = _mm_mul_ps(vResult, g_XMTwoPi);
	vResult = _mm_sub_ps(Angles, vResult);
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorSin
(
	FXMVECTOR V
)
{
	// 11-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = sinf(V.vector4_f32[0]);
	Result.vector4_f32[1] = sinf(V.vector4_f32[1]);
	Result.vector4_f32[2] = sinf(V.vector4_f32[2]);
	Result.vector4_f32[3] = sinf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Force the value within the bounds of pi
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with sin(y) = sin(x).
	uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
	uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	float32x4_t absx = vabsq_f32(x);
	float32x4_t rflx = vsubq_f32(c, x);
	uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
	x = vbslq_f32(comp, x, rflx);

	float32x4_t x2 = vmulq_f32(x, x);

	// Compute polynomial approximation
	const XMVECTOR SC1 = g_XMSinCoefficients1;
	const XMVECTOR SC0 = g_XMSinCoefficients0;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(SC0), 1);
	XMVECTOR Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(SC1), 0);

	vConstants = vdupq_lane_f32(vget_high_f32(SC0), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(SC0), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(SC0), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	Result = vmlaq_f32(g_XMOne, Result, x2);
	Result = vmulq_f32(Result, x);
	return Result;
#elif defined(_XM_SSE_INTRINSICS_)
	// Force the value within the bounds of pi
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with sin(y) = sin(x).
	__m128 sign = _mm_and_ps(x, g_XMNegativeZero);
	__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	__m128 absx = _mm_andnot_ps(sign, x);  // |x|
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);

	__m128 x2 = _mm_mul_ps(x, x);

	// Compute polynomial approximation
	const XMVECTOR SC1 = g_XMSinCoefficients1;
	XMVECTOR vConstants = XM_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 Result = _mm_mul_ps(vConstants, x2);

	const XMVECTOR SC0 = g_XMSinCoefficients0;
	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_XMOne);
	Result = _mm_mul_ps(Result, x);
	return Result;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorCos
(
	FXMVECTOR V
)
{
	// 10-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = cosf(V.vector4_f32[0]);
	Result.vector4_f32[1] = cosf(V.vector4_f32[1]);
	Result.vector4_f32[2] = cosf(V.vector4_f32[2]);
	Result.vector4_f32[3] = cosf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Map V to x in [-pi,pi].
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
	uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
	uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	float32x4_t absx = vabsq_f32(x);
	float32x4_t rflx = vsubq_f32(c, x);
	uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
	x = vbslq_f32(comp, x, rflx);
	sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);

	float32x4_t x2 = vmulq_f32(x, x);

	// Compute polynomial approximation
	const XMVECTOR CC1 = g_XMCosCoefficients1;
	const XMVECTOR CC0 = g_XMCosCoefficients0;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(CC0), 1);
	XMVECTOR Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(CC1), 0);

	vConstants = vdupq_lane_f32(vget_high_f32(CC0), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(CC0), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(CC0), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	Result = vmlaq_f32(g_XMOne, Result, x2);
	Result = vmulq_f32(Result, sign);
	return Result;
#elif defined(_XM_SSE_INTRINSICS_)
	// Map V to x in [-pi,pi].
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
	XMVECTOR sign = _mm_and_ps(x, g_XMNegativeZero);
	__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	__m128 absx = _mm_andnot_ps(sign, x);  // |x|
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, g_XMOne);
	select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
	sign = _mm_or_ps(select0, select1);

	__m128 x2 = _mm_mul_ps(x, x);

	// Compute polynomial approximation
	const XMVECTOR CC1 = g_XMCosCoefficients1;
	XMVECTOR vConstants = XM_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 Result = _mm_mul_ps(vConstants, x2);

	const XMVECTOR CC0 = g_XMCosCoefficients0;
	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_XMOne);
	Result = _mm_mul_ps(Result, sign);
	return Result;
#endif
}

//------------------------------------------------------------------------------

_Use_decl_annotations_
inline void XM_CALLCONV XMVectorSinCos
(
	XMVECTOR* pSin,
	XMVECTOR* pCos,
	FXMVECTOR V
)
{
	assert(pSin != nullptr);
	assert(pCos != nullptr);

	// 11/10-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Sin;
	Sin.vector4_f32[0] = sinf(V.vector4_f32[0]);
	Sin.vector4_f32[1] = sinf(V.vector4_f32[1]);
	Sin.vector4_f32[2] = sinf(V.vector4_f32[2]);
	Sin.vector4_f32[3] = sinf(V.vector4_f32[3]);

	XMVECTOR Cos;
	Cos.vector4_f32[0] = cosf(V.vector4_f32[0]);
	Cos.vector4_f32[1] = cosf(V.vector4_f32[1]);
	Cos.vector4_f32[2] = cosf(V.vector4_f32[2]);
	Cos.vector4_f32[3] = cosf(V.vector4_f32[3]);

	*pSin = Sin;
	*pCos = Cos;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Force the value within the bounds of pi
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
	uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
	uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	float32x4_t absx = vabsq_f32(x);
	float32x4_t  rflx = vsubq_f32(c, x);
	uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
	x = vbslq_f32(comp, x, rflx);
	sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);

	float32x4_t x2 = vmulq_f32(x, x);

	// Compute polynomial approximation for sine
	const XMVECTOR SC1 = g_XMSinCoefficients1;
	const XMVECTOR SC0 = g_XMSinCoefficients0;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(SC0), 1);
	XMVECTOR Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(SC1), 0);

	vConstants = vdupq_lane_f32(vget_high_f32(SC0), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(SC0), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(SC0), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	Result = vmlaq_f32(g_XMOne, Result, x2);
	*pSin = vmulq_f32(Result, x);

	// Compute polynomial approximation for cosine
	const XMVECTOR CC1 = g_XMCosCoefficients1;
	const XMVECTOR CC0 = g_XMCosCoefficients0;
	vConstants = vdupq_lane_f32(vget_high_f32(CC0), 1);
	Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(CC1), 0);

	vConstants = vdupq_lane_f32(vget_high_f32(CC0), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(CC0), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(CC0), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	Result = vmlaq_f32(g_XMOne, Result, x2);
	*pCos = vmulq_f32(Result, sign);
#elif defined(_XM_SSE_INTRINSICS_)
	// Force the value within the bounds of pi
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
	XMVECTOR sign = _mm_and_ps(x, g_XMNegativeZero);
	__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	__m128 absx = _mm_andnot_ps(sign, x);  // |x|
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, g_XMOne);
	select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
	sign = _mm_or_ps(select0, select1);

	__m128 x2 = _mm_mul_ps(x, x);

	// Compute polynomial approximation of sine
	const XMVECTOR SC1 = g_XMSinCoefficients1;
	XMVECTOR vConstants = XM_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 Result = _mm_mul_ps(vConstants, x2);

	const XMVECTOR SC0 = g_XMSinCoefficients0;
	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_XMOne);
	Result = _mm_mul_ps(Result, x);
	*pSin = Result;

	// Compute polynomial approximation of cosine
	const XMVECTOR CC1 = g_XMCosCoefficients1;
	vConstants = XM_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_mul_ps(vConstants, x2);

	const XMVECTOR CC0 = g_XMCosCoefficients0;
	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_XMOne);
	Result = _mm_mul_ps(Result, sign);
	*pCos = Result;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorTan
(
	FXMVECTOR V
)
{
	// Cody and Waite algorithm to compute tangent.

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = tanf(V.vector4_f32[0]);
	Result.vector4_f32[1] = tanf(V.vector4_f32[1]);
	Result.vector4_f32[2] = tanf(V.vector4_f32[2]);
	Result.vector4_f32[3] = tanf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_) 

	static const XMVECTORF32 TanCoefficients0 = { 1.0f, -4.667168334e-1f, 2.566383229e-2f, -3.118153191e-4f };
	static const XMVECTORF32 TanCoefficients1 = { 4.981943399e-7f, -1.333835001e-1f, 3.424887824e-3f, -1.786170734e-5f };
	static const XMVECTORF32 TanConstants = { 1.570796371f, 6.077100628e-11f, 0.000244140625f, 0.63661977228f /*2 / Pi*/ };
	static const XMVECTORU32 Mask = { 0x1, 0x1, 0x1, 0x1 };

	XMVECTOR TwoDivPi = XMVectorSplatW(TanConstants.v);

	XMVECTOR Zero = XMVectorZero();

	XMVECTOR C0 = XMVectorSplatX(TanConstants.v);
	XMVECTOR C1 = XMVectorSplatY(TanConstants.v);
	XMVECTOR Epsilon = XMVectorSplatZ(TanConstants.v);

	XMVECTOR VA = XMVectorMultiply(V, TwoDivPi);

	VA = XMVectorRound(VA);

	XMVECTOR VC = XMVectorNegativeMultiplySubtract(VA, C0, V);

	XMVECTOR VB = XMVectorAbs(VA);

	VC = XMVectorNegativeMultiplySubtract(VA, C1, VC);

#if defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
	VB = vcvtq_u32_f32(VB);
#elif defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
	reinterpret_cast<__m128i *>(&VB)[0] = _mm_cvttps_epi32(VB);
#else
	for (size_t i = 0; i < 4; i++)
	{
		VB.vector4_u32[i] = (uint32_t)VB.vector4_f32[i];
	}
#endif

	XMVECTOR VC2 = XMVectorMultiply(VC, VC);

	XMVECTOR T7 = XMVectorSplatW(TanCoefficients1.v);
	XMVECTOR T6 = XMVectorSplatZ(TanCoefficients1.v);
	XMVECTOR T4 = XMVectorSplatX(TanCoefficients1.v);
	XMVECTOR T3 = XMVectorSplatW(TanCoefficients0.v);
	XMVECTOR T5 = XMVectorSplatY(TanCoefficients1.v);
	XMVECTOR T2 = XMVectorSplatZ(TanCoefficients0.v);
	XMVECTOR T1 = XMVectorSplatY(TanCoefficients0.v);
	XMVECTOR T0 = XMVectorSplatX(TanCoefficients0.v);

	XMVECTOR VBIsEven = XMVectorAndInt(VB, Mask.v);
	VBIsEven = XMVectorEqualInt(VBIsEven, Zero);

	XMVECTOR N = XMVectorMultiplyAdd(VC2, T7, T6);
	XMVECTOR D = XMVectorMultiplyAdd(VC2, T4, T3);
	N = XMVectorMultiplyAdd(VC2, N, T5);
	D = XMVectorMultiplyAdd(VC2, D, T2);
	N = XMVectorMultiply(VC2, N);
	D = XMVectorMultiplyAdd(VC2, D, T1);
	N = XMVectorMultiplyAdd(VC, N, VC);
	XMVECTOR VCNearZero = XMVectorInBounds(VC, Epsilon);
	D = XMVectorMultiplyAdd(VC2, D, T0);

	N = XMVectorSelect(N, VC, VCNearZero);
	D = XMVectorSelect(D, g_XMOne.v, VCNearZero);

	XMVECTOR R0 = XMVectorNegate(N);
	XMVECTOR R1 = XMVectorDivide(N, D);
	R0 = XMVectorDivide(D, R0);

	XMVECTOR VIsZero = XMVectorEqual(V, Zero);

	XMVECTOR Result = XMVectorSelect(R0, R1, VBIsEven);

	Result = XMVectorSelect(Result, Zero, VIsZero);

	return Result;

#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorSinH
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = sinhf(V.vector4_f32[0]);
	Result.vector4_f32[1] = sinhf(V.vector4_f32[1]);
	Result.vector4_f32[2] = sinhf(V.vector4_f32[2]);
	Result.vector4_f32[3] = sinhf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	static const XMVECTORF32 Scale = { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f }; // 1.0f / ln(2.0f)

	XMVECTOR V1 = vmlaq_f32(g_XMNegativeOne.v, V, Scale.v);
	XMVECTOR V2 = vmlsq_f32(g_XMNegativeOne.v, V, Scale.v);
	XMVECTOR E1 = XMVectorExp(V1);
	XMVECTOR E2 = XMVectorExp(V2);

	return vsubq_f32(E1, E2);
#elif defined(_XM_SSE_INTRINSICS_)
	static const XMVECTORF32 Scale = { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f }; // 1.0f / ln(2.0f)

	XMVECTOR V1 = _mm_mul_ps(V, Scale);
	V1 = _mm_add_ps(V1, g_XMNegativeOne);
	XMVECTOR V2 = _mm_mul_ps(V, Scale);
	V2 = _mm_sub_ps(g_XMNegativeOne, V2);
	XMVECTOR E1 = XMVectorExp(V1);
	XMVECTOR E2 = XMVectorExp(V2);

	return _mm_sub_ps(E1, E2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorCosH
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = coshf(V.vector4_f32[0]);
	Result.vector4_f32[1] = coshf(V.vector4_f32[1]);
	Result.vector4_f32[2] = coshf(V.vector4_f32[2]);
	Result.vector4_f32[3] = coshf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	static const XMVECTORF32 Scale = { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f }; // 1.0f / ln(2.0f)

	XMVECTOR V1 = vmlaq_f32(g_XMNegativeOne.v, V, Scale.v);
	XMVECTOR V2 = vmlsq_f32(g_XMNegativeOne.v, V, Scale.v);
	XMVECTOR E1 = XMVectorExp(V1);
	XMVECTOR E2 = XMVectorExp(V2);
	return vaddq_f32(E1, E2);
#elif defined(_XM_SSE_INTRINSICS_)
	static const XMVECTORF32 Scale = { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f }; // 1.0f / ln(2.0f)

	XMVECTOR V1 = _mm_mul_ps(V, Scale.v);
	V1 = _mm_add_ps(V1, g_XMNegativeOne.v);
	XMVECTOR V2 = _mm_mul_ps(V, Scale.v);
	V2 = _mm_sub_ps(g_XMNegativeOne.v, V2);
	XMVECTOR E1 = XMVectorExp(V1);
	XMVECTOR E2 = XMVectorExp(V2);
	return _mm_add_ps(E1, E2);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorTanH
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = tanhf(V.vector4_f32[0]);
	Result.vector4_f32[1] = tanhf(V.vector4_f32[1]);
	Result.vector4_f32[2] = tanhf(V.vector4_f32[2]);
	Result.vector4_f32[3] = tanhf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	static const XMVECTORF32 Scale = { 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f }; // 2.0f / ln(2.0f)

	XMVECTOR E = vmulq_f32(V, Scale.v);
	E = XMVectorExp(E);
	E = vmlaq_f32(g_XMOneHalf.v, E, g_XMOneHalf.v);
	E = XMVectorReciprocal(E);
	return vsubq_f32(g_XMOne.v, E);
#elif defined(_XM_SSE_INTRINSICS_)
	static const XMVECTORF32 Scale = { 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f }; // 2.0f / ln(2.0f)

	XMVECTOR E = _mm_mul_ps(V, Scale.v);
	E = XMVectorExp(E);
	E = _mm_mul_ps(E, g_XMOneHalf.v);
	E = _mm_add_ps(E, g_XMOneHalf.v);
	E = _mm_div_ps(g_XMOne.v, E);
	return _mm_sub_ps(g_XMOne.v, E);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorASin
(
	FXMVECTOR V
)
{
	// 7-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = asinf(V.vector4_f32[0]);
	Result.vector4_f32[1] = asinf(V.vector4_f32[1]);
	Result.vector4_f32[2] = asinf(V.vector4_f32[2]);
	Result.vector4_f32[3] = asinf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	uint32x4_t nonnegative = vcgeq_f32(V, g_XMZero);
	float32x4_t x = vabsq_f32(V);

	// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
	float32x4_t oneMValue = vsubq_f32(g_XMOne, x);
	float32x4_t clampOneMValue = vmaxq_f32(g_XMZero, oneMValue);
	float32x4_t root = XMVectorSqrt(clampOneMValue);

	// Compute polynomial approximation
	const XMVECTOR AC1 = g_XMArcCoefficients1;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(AC1), 0);
	XMVECTOR t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AC1), 1);

	vConstants = vdupq_lane_f32(vget_low_f32(AC1), 1);
	t0 = vmlaq_f32(vConstants, t0, x);

	vConstants = vdupq_lane_f32(vget_low_f32(AC1), 0);
	t0 = vmlaq_f32(vConstants, t0, x);

	const XMVECTOR AC0 = g_XMArcCoefficients0;
	vConstants = vdupq_lane_f32(vget_high_f32(AC0), 1);
	t0 = vmlaq_f32(vConstants, t0, x);

	vConstants = vdupq_lane_f32(vget_high_f32(AC0), 0);
	t0 = vmlaq_f32(vConstants, t0, x);

	vConstants = vdupq_lane_f32(vget_low_f32(AC0), 1);
	t0 = vmlaq_f32(vConstants, t0, x);

	vConstants = vdupq_lane_f32(vget_low_f32(AC0), 0);
	t0 = vmlaq_f32(vConstants, t0, x);
	t0 = vmulq_f32(t0, root);

	float32x4_t t1 = vsubq_f32(g_XMPi, t0);
	t0 = vbslq_f32(nonnegative, t0, t1);
	t0 = vsubq_f32(g_XMHalfPi, t0);
	return t0;
#elif defined(_XM_SSE_INTRINSICS_)
	__m128 nonnegative = _mm_cmpge_ps(V, g_XMZero);
	__m128 mvalue = _mm_sub_ps(g_XMZero, V);
	__m128 x = _mm_max_ps(V, mvalue);  // |V|

									   // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
	__m128 oneMValue = _mm_sub_ps(g_XMOne, x);
	__m128 clampOneMValue = _mm_max_ps(g_XMZero, oneMValue);
	__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

												// Compute polynomial approximation
	const XMVECTOR AC1 = g_XMArcCoefficients1;
	XMVECTOR vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 t0 = _mm_mul_ps(vConstants, x);

	vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	const XMVECTOR AC0 = g_XMArcCoefficients0;
	vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(3, 3, 3, 3));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, root);

	__m128 t1 = _mm_sub_ps(g_XMPi, t0);
	t0 = _mm_and_ps(nonnegative, t0);
	t1 = _mm_andnot_ps(nonnegative, t1);
	t0 = _mm_or_ps(t0, t1);
	t0 = _mm_sub_ps(g_XMHalfPi, t0);
	return t0;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorACos
(
	FXMVECTOR V
)
{
	// 7-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = acosf(V.vector4_f32[0]);
	Result.vector4_f32[1] = acosf(V.vector4_f32[1]);
	Result.vector4_f32[2] = acosf(V.vector4_f32[2]);
	Result.vector4_f32[3] = acosf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	uint32x4_t nonnegative = vcgeq_f32(V, g_XMZero);
	float32x4_t x = vabsq_f32(V);

	// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
	float32x4_t oneMValue = vsubq_f32(g_XMOne, x);
	float32x4_t clampOneMValue = vmaxq_f32(g_XMZero, oneMValue);
	float32x4_t root = XMVectorSqrt(clampOneMValue);

	// Compute polynomial approximation
	const XMVECTOR AC1 = g_XMArcCoefficients1;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(AC1), 0);
	XMVECTOR t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AC1), 1);

	vConstants = vdupq_lane_f32(vget_low_f32(AC1), 1);
	t0 = vmlaq_f32(vConstants, t0, x);

	vConstants = vdupq_lane_f32(vget_low_f32(AC1), 0);
	t0 = vmlaq_f32(vConstants, t0, x);

	const XMVECTOR AC0 = g_XMArcCoefficients0;
	vConstants = vdupq_lane_f32(vget_high_f32(AC0), 1);
	t0 = vmlaq_f32(vConstants, t0, x);

	vConstants = vdupq_lane_f32(vget_high_f32(AC0), 0);
	t0 = vmlaq_f32(vConstants, t0, x);

	vConstants = vdupq_lane_f32(vget_low_f32(AC0), 1);
	t0 = vmlaq_f32(vConstants, t0, x);

	vConstants = vdupq_lane_f32(vget_low_f32(AC0), 0);
	t0 = vmlaq_f32(vConstants, t0, x);
	t0 = vmulq_f32(t0, root);

	float32x4_t t1 = vsubq_f32(g_XMPi, t0);
	t0 = vbslq_f32(nonnegative, t0, t1);
	return t0;
#elif defined(_XM_SSE_INTRINSICS_)
	__m128 nonnegative = _mm_cmpge_ps(V, g_XMZero);
	__m128 mvalue = _mm_sub_ps(g_XMZero, V);
	__m128 x = _mm_max_ps(V, mvalue);  // |V|

									   // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
	__m128 oneMValue = _mm_sub_ps(g_XMOne, x);
	__m128 clampOneMValue = _mm_max_ps(g_XMZero, oneMValue);
	__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

												// Compute polynomial approximation
	const XMVECTOR AC1 = g_XMArcCoefficients1;
	XMVECTOR vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 t0 = _mm_mul_ps(vConstants, x);

	vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AC1, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	const XMVECTOR AC0 = g_XMArcCoefficients0;
	vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(3, 3, 3, 3));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AC0, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, root);

	__m128 t1 = _mm_sub_ps(g_XMPi, t0);
	t0 = _mm_and_ps(nonnegative, t0);
	t1 = _mm_andnot_ps(nonnegative, t1);
	t0 = _mm_or_ps(t0, t1);
	return t0;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorATan
(
	FXMVECTOR V
)
{
	// 17-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = atanf(V.vector4_f32[0]);
	Result.vector4_f32[1] = atanf(V.vector4_f32[1]);
	Result.vector4_f32[2] = atanf(V.vector4_f32[2]);
	Result.vector4_f32[3] = atanf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float32x4_t absV = vabsq_f32(V);
	float32x4_t invV = XMVectorReciprocal(V);
	uint32x4_t comp = vcgtq_f32(V, g_XMOne);
	uint32x4_t sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);
	comp = vcleq_f32(absV, g_XMOne);
	sign = vbslq_f32(comp, g_XMZero, sign);
	uint32x4_t x = vbslq_f32(comp, V, invV);

	float32x4_t x2 = vmulq_f32(x, x);

	// Compute polynomial approximation
	const XMVECTOR TC1 = g_XMATanCoefficients1;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(TC1), 0);
	XMVECTOR Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(TC1), 1);

	vConstants = vdupq_lane_f32(vget_low_f32(TC1), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(TC1), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	const XMVECTOR TC0 = g_XMATanCoefficients0;
	vConstants = vdupq_lane_f32(vget_high_f32(TC0), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_high_f32(TC0), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(TC0), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(TC0), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	Result = vmlaq_f32(g_XMOne, Result, x2);
	Result = vmulq_f32(Result, x);

	float32x4_t result1 = vmulq_f32(sign, g_XMHalfPi);
	result1 = vsubq_f32(result1, Result);

	comp = vceqq_f32(sign, g_XMZero);
	Result = vbslq_f32(comp, Result, result1);
	return Result;
#elif defined(_XM_SSE_INTRINSICS_)
	__m128 absV = XMVectorAbs(V);
	__m128 invV = _mm_div_ps(g_XMOne, V);
	__m128 comp = _mm_cmpgt_ps(V, g_XMOne);
	__m128 select0 = _mm_and_ps(comp, g_XMOne);
	__m128 select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
	__m128 sign = _mm_or_ps(select0, select1);
	comp = _mm_cmple_ps(absV, g_XMOne);
	select0 = _mm_and_ps(comp, g_XMZero);
	select1 = _mm_andnot_ps(comp, sign);
	sign = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, V);
	select1 = _mm_andnot_ps(comp, invV);
	__m128 x = _mm_or_ps(select0, select1);

	__m128 x2 = _mm_mul_ps(x, x);

	// Compute polynomial approximation
	const XMVECTOR TC1 = g_XMATanCoefficients1;
	XMVECTOR vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 Result = _mm_mul_ps(vConstants, x2);

	vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(TC1, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	const XMVECTOR TC0 = g_XMATanCoefficients0;
	vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(TC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_XMOne);
	Result = _mm_mul_ps(Result, x);
	__m128 result1 = _mm_mul_ps(sign, g_XMHalfPi);
	result1 = _mm_sub_ps(result1, Result);

	comp = _mm_cmpeq_ps(sign, g_XMZero);
	select0 = _mm_and_ps(comp, Result);
	select1 = _mm_andnot_ps(comp, result1);
	Result = _mm_or_ps(select0, select1);
	return Result;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorATan2
(
	FXMVECTOR Y,
	FXMVECTOR X
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = atan2f(Y.vector4_f32[0], X.vector4_f32[0]);
	Result.vector4_f32[1] = atan2f(Y.vector4_f32[1], X.vector4_f32[1]);
	Result.vector4_f32[2] = atan2f(Y.vector4_f32[2], X.vector4_f32[2]);
	Result.vector4_f32[3] = atan2f(Y.vector4_f32[3], X.vector4_f32[3]);
	return Result;
#else

	// Return the inverse tangent of Y / X in the range of -Pi to Pi with the following exceptions:

	//     Y == 0 and X is Negative         -> Pi with the sign of Y
	//     y == 0 and x is positive         -> 0 with the sign of y
	//     Y != 0 and X == 0                -> Pi / 2 with the sign of Y
	//     Y != 0 and X is Negative         -> atan(y/x) + (PI with the sign of Y)
	//     X == -Infinity and Finite Y      -> Pi with the sign of Y
	//     X == +Infinity and Finite Y      -> 0 with the sign of Y
	//     Y == Infinity and X is Finite    -> Pi / 2 with the sign of Y
	//     Y == Infinity and X == -Infinity -> 3Pi / 4 with the sign of Y
	//     Y == Infinity and X == +Infinity -> Pi / 4 with the sign of Y

	static const XMVECTORF32 ATan2Constants = { XM_PI, XM_PIDIV2, XM_PIDIV4, XM_PI * 3.0f / 4.0f };

	XMVECTOR Zero = XMVectorZero();
	XMVECTOR ATanResultValid = XMVectorTrueInt();

	XMVECTOR Pi = XMVectorSplatX(ATan2Constants);
	XMVECTOR PiOverTwo = XMVectorSplatY(ATan2Constants);
	XMVECTOR PiOverFour = XMVectorSplatZ(ATan2Constants);
	XMVECTOR ThreePiOverFour = XMVectorSplatW(ATan2Constants);

	XMVECTOR YEqualsZero = XMVectorEqual(Y, Zero);
	XMVECTOR XEqualsZero = XMVectorEqual(X, Zero);
	XMVECTOR XIsPositive = XMVectorAndInt(X, g_XMNegativeZero.v);
	XIsPositive = XMVectorEqualInt(XIsPositive, Zero);
	XMVECTOR YEqualsInfinity = XMVectorIsInfinite(Y);
	XMVECTOR XEqualsInfinity = XMVectorIsInfinite(X);

	XMVECTOR YSign = XMVectorAndInt(Y, g_XMNegativeZero.v);
	Pi = XMVectorOrInt(Pi, YSign);
	PiOverTwo = XMVectorOrInt(PiOverTwo, YSign);
	PiOverFour = XMVectorOrInt(PiOverFour, YSign);
	ThreePiOverFour = XMVectorOrInt(ThreePiOverFour, YSign);

	XMVECTOR R1 = XMVectorSelect(Pi, YSign, XIsPositive);
	XMVECTOR R2 = XMVectorSelect(ATanResultValid, PiOverTwo, XEqualsZero);
	XMVECTOR R3 = XMVectorSelect(R2, R1, YEqualsZero);
	XMVECTOR R4 = XMVectorSelect(ThreePiOverFour, PiOverFour, XIsPositive);
	XMVECTOR R5 = XMVectorSelect(PiOverTwo, R4, XEqualsInfinity);
	XMVECTOR Result = XMVectorSelect(R3, R5, YEqualsInfinity);
	ATanResultValid = XMVectorEqualInt(Result, ATanResultValid);

	XMVECTOR V = XMVectorDivide(Y, X);

	XMVECTOR R0 = XMVectorATan(V);

	R1 = XMVectorSelect(Pi, g_XMNegativeZero, XIsPositive);
	R2 = XMVectorAdd(R0, R1);

	return XMVectorSelect(Result, R2, ATanResultValid);

#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorSinEst
(
	FXMVECTOR V
)
{
	// 7-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = sinf(V.vector4_f32[0]);
	Result.vector4_f32[1] = sinf(V.vector4_f32[1]);
	Result.vector4_f32[2] = sinf(V.vector4_f32[2]);
	Result.vector4_f32[3] = sinf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Force the value within the bounds of pi
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with sin(y) = sin(x).
	uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
	uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	float32x4_t absx = vabsq_f32(x);
	float32x4_t rflx = vsubq_f32(c, x);
	uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
	x = vbslq_f32(comp, x, rflx);

	float32x4_t x2 = vmulq_f32(x, x);

	// Compute polynomial approximation
	const XMVECTOR SEC = g_XMSinCoefficients1;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(SEC), 0);
	XMVECTOR Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(SEC), 1);

	vConstants = vdupq_lane_f32(vget_low_f32(SEC), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	Result = vmlaq_f32(g_XMOne, Result, x2);
	Result = vmulq_f32(Result, x);
	return Result;
#elif defined(_XM_SSE_INTRINSICS_)
	// Force the value within the bounds of pi
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with sin(y) = sin(x).
	__m128 sign = _mm_and_ps(x, g_XMNegativeZero);
	__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	__m128 absx = _mm_andnot_ps(sign, x);  // |x|
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);

	__m128 x2 = _mm_mul_ps(x, x);

	// Compute polynomial approximation
	const XMVECTOR SEC = g_XMSinCoefficients1;
	XMVECTOR vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 Result = _mm_mul_ps(vConstants, x2);

	vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	Result = _mm_add_ps(Result, g_XMOne);
	Result = _mm_mul_ps(Result, x);
	return Result;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorCosEst
(
	FXMVECTOR V
)
{
	// 6-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = cosf(V.vector4_f32[0]);
	Result.vector4_f32[1] = cosf(V.vector4_f32[1]);
	Result.vector4_f32[2] = cosf(V.vector4_f32[2]);
	Result.vector4_f32[3] = cosf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Map V to x in [-pi,pi].
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
	uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
	uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	float32x4_t absx = vabsq_f32(x);
	float32x4_t rflx = vsubq_f32(c, x);
	uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
	x = vbslq_f32(comp, x, rflx);
	sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);

	float32x4_t x2 = vmulq_f32(x, x);

	// Compute polynomial approximation
	const XMVECTOR CEC = g_XMCosCoefficients1;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(CEC), 0);
	XMVECTOR Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(CEC), 1);

	vConstants = vdupq_lane_f32(vget_low_f32(CEC), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	Result = vmlaq_f32(g_XMOne, Result, x2);
	Result = vmulq_f32(Result, sign);
	return Result;
#elif defined(_XM_SSE_INTRINSICS_)
	// Map V to x in [-pi,pi].
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
	XMVECTOR sign = _mm_and_ps(x, g_XMNegativeZero);
	__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	__m128 absx = _mm_andnot_ps(sign, x);  // |x|
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, g_XMOne);
	select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
	sign = _mm_or_ps(select0, select1);

	__m128 x2 = _mm_mul_ps(x, x);

	// Compute polynomial approximation
	const XMVECTOR CEC = g_XMCosCoefficients1;
	XMVECTOR vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 Result = _mm_mul_ps(vConstants, x2);

	vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	Result = _mm_add_ps(Result, g_XMOne);
	Result = _mm_mul_ps(Result, sign);
	return Result;
#endif
}

//------------------------------------------------------------------------------

_Use_decl_annotations_
inline void XM_CALLCONV XMVectorSinCosEst
(
	XMVECTOR* pSin,
	XMVECTOR* pCos,
	FXMVECTOR  V
)
{
	assert(pSin != nullptr);
	assert(pCos != nullptr);

	// 7/6-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Sin;
	Sin.vector4_f32[0] = sinf(V.vector4_f32[0]);
	Sin.vector4_f32[1] = sinf(V.vector4_f32[1]);
	Sin.vector4_f32[2] = sinf(V.vector4_f32[2]);
	Sin.vector4_f32[3] = sinf(V.vector4_f32[3]);

	XMVECTOR Cos;
	Cos.vector4_f32[0] = cosf(V.vector4_f32[0]);
	Cos.vector4_f32[1] = cosf(V.vector4_f32[1]);
	Cos.vector4_f32[2] = cosf(V.vector4_f32[2]);
	Cos.vector4_f32[3] = cosf(V.vector4_f32[3]);

	*pSin = Sin;
	*pCos = Cos;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	// Force the value within the bounds of pi
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
	uint32x4_t sign = vandq_u32(x, g_XMNegativeZero);
	uint32x4_t c = vorrq_u32(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	float32x4_t absx = vabsq_f32(x);
	float32x4_t rflx = vsubq_f32(c, x);
	uint32x4_t comp = vcleq_f32(absx, g_XMHalfPi);
	x = vbslq_f32(comp, x, rflx);
	sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);

	float32x4_t x2 = vmulq_f32(x, x);

	// Compute polynomial approximation for sine
	const XMVECTOR SEC = g_XMSinCoefficients1;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(SEC), 0);
	XMVECTOR Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(SEC), 1);

	vConstants = vdupq_lane_f32(vget_low_f32(SEC), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	Result = vmlaq_f32(g_XMOne, Result, x2);
	*pSin = vmulq_f32(Result, x);

	// Compute polynomial approximation
	const XMVECTOR CEC = g_XMCosCoefficients1;
	vConstants = vdupq_lane_f32(vget_high_f32(CEC), 0);
	Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(CEC), 1);

	vConstants = vdupq_lane_f32(vget_low_f32(CEC), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	Result = vmlaq_f32(g_XMOne, Result, x2);
	*pCos = vmulq_f32(Result, sign);
#elif defined(_XM_SSE_INTRINSICS_)
	// Force the value within the bounds of pi
	XMVECTOR x = XMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
	XMVECTOR sign = _mm_and_ps(x, g_XMNegativeZero);
	__m128 c = _mm_or_ps(g_XMPi, sign);  // pi when x >= 0, -pi when x < 0
	__m128 absx = _mm_andnot_ps(sign, x);  // |x|
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, g_XMOne);
	select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
	sign = _mm_or_ps(select0, select1);

	__m128 x2 = _mm_mul_ps(x, x);

	// Compute polynomial approximation for sine
	const XMVECTOR SEC = g_XMSinCoefficients1;
	XMVECTOR vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 Result = _mm_mul_ps(vConstants, x2);

	vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(SEC, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	Result = _mm_add_ps(Result, g_XMOne);
	Result = _mm_mul_ps(Result, x);
	*pSin = Result;

	// Compute polynomial approximation for cosine
	const XMVECTOR CEC = g_XMCosCoefficients1;
	vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_mul_ps(vConstants, x2);

	vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(CEC, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	Result = _mm_add_ps(Result, g_XMOne);
	Result = _mm_mul_ps(Result, sign);
	*pCos = Result;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorTanEst
(
	FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = tanf(V.vector4_f32[0]);
	Result.vector4_f32[1] = tanf(V.vector4_f32[1]);
	Result.vector4_f32[2] = tanf(V.vector4_f32[2]);
	Result.vector4_f32[3] = tanf(V.vector4_f32[3]);
	return Result;
#else

	XMVECTOR OneOverPi = XMVectorSplatW(g_XMTanEstCoefficients.v);

	XMVECTOR V1 = XMVectorMultiply(V, OneOverPi);
	V1 = XMVectorRound(V1);

	V1 = XMVectorNegativeMultiplySubtract(g_XMPi.v, V1, V);

	XMVECTOR T0 = XMVectorSplatX(g_XMTanEstCoefficients.v);
	XMVECTOR T1 = XMVectorSplatY(g_XMTanEstCoefficients.v);
	XMVECTOR T2 = XMVectorSplatZ(g_XMTanEstCoefficients.v);

	XMVECTOR V2T2 = XMVectorNegativeMultiplySubtract(V1, V1, T2);
	XMVECTOR V2 = XMVectorMultiply(V1, V1);
	XMVECTOR V1T0 = XMVectorMultiply(V1, T0);
	XMVECTOR V1T1 = XMVectorMultiply(V1, T1);

	XMVECTOR D = XMVectorReciprocalEst(V2T2);
	XMVECTOR N = XMVectorMultiplyAdd(V2, V1T1, V1T0);

	return XMVectorMultiply(N, D);

#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorASinEst
(
	FXMVECTOR V
)
{
	// 3-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = asinf(V.vector4_f32[0]);
	Result.vector4_f32[1] = asinf(V.vector4_f32[1]);
	Result.vector4_f32[2] = asinf(V.vector4_f32[2]);
	Result.vector4_f32[3] = asinf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	uint32x4_t nonnegative = vcgeq_f32(V, g_XMZero);
	float32x4_t x = vabsq_f32(V);

	// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
	float32x4_t oneMValue = vsubq_f32(g_XMOne, x);
	float32x4_t clampOneMValue = vmaxq_f32(g_XMZero, oneMValue);
	float32x4_t root = XMVectorSqrt(clampOneMValue);

	// Compute polynomial approximation
	const XMVECTOR AEC = g_XMArcEstCoefficients;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
	XMVECTOR t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AEC), 1);

	vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
	t0 = vmlaq_f32(vConstants, t0, x);

	vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
	t0 = vmlaq_f32(vConstants, t0, x);
	t0 = vmulq_f32(t0, root);

	float32x4_t t1 = vsubq_f32(g_XMPi, t0);
	t0 = vbslq_f32(nonnegative, t0, t1);
	t0 = vsubq_f32(g_XMHalfPi, t0);
	return t0;
#elif defined(_XM_SSE_INTRINSICS_)
	__m128 nonnegative = _mm_cmpge_ps(V, g_XMZero);
	__m128 mvalue = _mm_sub_ps(g_XMZero, V);
	__m128 x = _mm_max_ps(V, mvalue);  // |V|

									   // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
	__m128 oneMValue = _mm_sub_ps(g_XMOne, x);
	__m128 clampOneMValue = _mm_max_ps(g_XMZero, oneMValue);
	__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

												// Compute polynomial approximation
	const XMVECTOR AEC = g_XMArcEstCoefficients;
	XMVECTOR vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 t0 = _mm_mul_ps(vConstants, x);

	vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, root);

	__m128 t1 = _mm_sub_ps(g_XMPi, t0);
	t0 = _mm_and_ps(nonnegative, t0);
	t1 = _mm_andnot_ps(nonnegative, t1);
	t0 = _mm_or_ps(t0, t1);
	t0 = _mm_sub_ps(g_XMHalfPi, t0);
	return t0;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorACosEst
(
	FXMVECTOR V
)
{
	// 3-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = acosf(V.vector4_f32[0]);
	Result.vector4_f32[1] = acosf(V.vector4_f32[1]);
	Result.vector4_f32[2] = acosf(V.vector4_f32[2]);
	Result.vector4_f32[3] = acosf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	uint32x4_t nonnegative = vcgeq_f32(V, g_XMZero);
	float32x4_t x = vabsq_f32(V);

	// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
	float32x4_t oneMValue = vsubq_f32(g_XMOne, x);
	float32x4_t clampOneMValue = vmaxq_f32(g_XMZero, oneMValue);
	float32x4_t root = XMVectorSqrt(clampOneMValue);

	// Compute polynomial approximation
	const XMVECTOR AEC = g_XMArcEstCoefficients;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
	XMVECTOR t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AEC), 1);

	vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
	t0 = vmlaq_f32(vConstants, t0, x);

	vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
	t0 = vmlaq_f32(vConstants, t0, x);
	t0 = vmulq_f32(t0, root);

	float32x4_t t1 = vsubq_f32(g_XMPi, t0);
	t0 = vbslq_f32(nonnegative, t0, t1);
	return t0;
#elif defined(_XM_SSE_INTRINSICS_)
	__m128 nonnegative = _mm_cmpge_ps(V, g_XMZero);
	__m128 mvalue = _mm_sub_ps(g_XMZero, V);
	__m128 x = _mm_max_ps(V, mvalue);  // |V|

									   // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
	__m128 oneMValue = _mm_sub_ps(g_XMOne, x);
	__m128 clampOneMValue = _mm_max_ps(g_XMZero, oneMValue);
	__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

												// Compute polynomial approximation
	const XMVECTOR AEC = g_XMArcEstCoefficients;
	XMVECTOR vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 t0 = _mm_mul_ps(vConstants, x);

	vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);

	vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, root);

	__m128 t1 = _mm_sub_ps(g_XMPi, t0);
	t0 = _mm_and_ps(nonnegative, t0);
	t1 = _mm_andnot_ps(nonnegative, t1);
	t0 = _mm_or_ps(t0, t1);
	return t0;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorATanEst
(
	FXMVECTOR V
)
{
	// 9-degree minimax approximation

#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = atanf(V.vector4_f32[0]);
	Result.vector4_f32[1] = atanf(V.vector4_f32[1]);
	Result.vector4_f32[2] = atanf(V.vector4_f32[2]);
	Result.vector4_f32[3] = atanf(V.vector4_f32[3]);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float32x4_t absV = vabsq_f32(V);
	float32x4_t invV = XMVectorReciprocalEst(V);
	uint32x4_t comp = vcgtq_f32(V, g_XMOne);
	uint32x4_t sign = vbslq_f32(comp, g_XMOne, g_XMNegativeOne);
	comp = vcleq_f32(absV, g_XMOne);
	sign = vbslq_f32(comp, g_XMZero, sign);
	uint32x4_t x = vbslq_f32(comp, V, invV);

	float32x4_t x2 = vmulq_f32(x, x);

	// Compute polynomial approximation
	const XMVECTOR AEC = g_XMATanEstCoefficients1;
	XMVECTOR vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
	XMVECTOR Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(AEC), 1);

	vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
	Result = vmlaq_f32(vConstants, Result, x2);

	vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
	Result = vmlaq_f32(vConstants, Result, x2);

	// ATanEstCoefficients0 is already splatted
	Result = vmlaq_f32(g_XMATanEstCoefficients0, Result, x2);
	Result = vmulq_f32(Result, x);

	float32x4_t result1 = vmulq_f32(sign, g_XMHalfPi);
	result1 = vsubq_f32(result1, Result);

	comp = vceqq_f32(sign, g_XMZero);
	Result = vbslq_f32(comp, Result, result1);
	return Result;
#elif defined(_XM_SSE_INTRINSICS_)
	__m128 absV = XMVectorAbs(V);
	__m128 invV = _mm_div_ps(g_XMOne, V);
	__m128 comp = _mm_cmpgt_ps(V, g_XMOne);
	__m128 select0 = _mm_and_ps(comp, g_XMOne);
	__m128 select1 = _mm_andnot_ps(comp, g_XMNegativeOne);
	__m128 sign = _mm_or_ps(select0, select1);
	comp = _mm_cmple_ps(absV, g_XMOne);
	select0 = _mm_and_ps(comp, g_XMZero);
	select1 = _mm_andnot_ps(comp, sign);
	sign = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, V);
	select1 = _mm_andnot_ps(comp, invV);
	__m128 x = _mm_or_ps(select0, select1);

	__m128 x2 = _mm_mul_ps(x, x);

	// Compute polynomial approximation
	const XMVECTOR AEC = g_XMATanEstCoefficients1;
	XMVECTOR vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 Result = _mm_mul_ps(vConstants, x2);

	vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	// ATanEstCoefficients0 is already splatted
	Result = _mm_add_ps(Result, g_XMATanEstCoefficients0);
	Result = _mm_mul_ps(Result, x);
	__m128 result1 = _mm_mul_ps(sign, g_XMHalfPi);
	result1 = _mm_sub_ps(result1, Result);

	comp = _mm_cmpeq_ps(sign, g_XMZero);
	select0 = _mm_and_ps(comp, Result);
	select1 = _mm_andnot_ps(comp, result1);
	Result = _mm_or_ps(select0, select1);
	return Result;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorATan2Est
(
	FXMVECTOR Y,
	FXMVECTOR X
)
{
#if defined(_XM_NO_INTRINSICS_)
	XMVECTOR Result;
	Result.vector4_f32[0] = atan2f(Y.vector4_f32[0], X.vector4_f32[0]);
	Result.vector4_f32[1] = atan2f(Y.vector4_f32[1], X.vector4_f32[1]);
	Result.vector4_f32[2] = atan2f(Y.vector4_f32[2], X.vector4_f32[2]);
	Result.vector4_f32[3] = atan2f(Y.vector4_f32[3], X.vector4_f32[3]);
	return Result;
#else

	static const XMVECTORF32 ATan2Constants = { XM_PI, XM_PIDIV2, XM_PIDIV4, 2.3561944905f /* Pi*3/4 */ };

	const XMVECTOR Zero = XMVectorZero();
	XMVECTOR ATanResultValid = XMVectorTrueInt();

	XMVECTOR Pi = XMVectorSplatX(ATan2Constants);
	XMVECTOR PiOverTwo = XMVectorSplatY(ATan2Constants);
	XMVECTOR PiOverFour = XMVectorSplatZ(ATan2Constants);
	XMVECTOR ThreePiOverFour = XMVectorSplatW(ATan2Constants);

	XMVECTOR YEqualsZero = XMVectorEqual(Y, Zero);
	XMVECTOR XEqualsZero = XMVectorEqual(X, Zero);
	XMVECTOR XIsPositive = XMVectorAndInt(X, g_XMNegativeZero.v);
	XIsPositive = XMVectorEqualInt(XIsPositive, Zero);
	XMVECTOR YEqualsInfinity = XMVectorIsInfinite(Y);
	XMVECTOR XEqualsInfinity = XMVectorIsInfinite(X);

	XMVECTOR YSign = XMVectorAndInt(Y, g_XMNegativeZero.v);
	Pi = XMVectorOrInt(Pi, YSign);
	PiOverTwo = XMVectorOrInt(PiOverTwo, YSign);
	PiOverFour = XMVectorOrInt(PiOverFour, YSign);
	ThreePiOverFour = XMVectorOrInt(ThreePiOverFour, YSign);

	XMVECTOR R1 = XMVectorSelect(Pi, YSign, XIsPositive);
	XMVECTOR R2 = XMVectorSelect(ATanResultValid, PiOverTwo, XEqualsZero);
	XMVECTOR R3 = XMVectorSelect(R2, R1, YEqualsZero);
	XMVECTOR R4 = XMVectorSelect(ThreePiOverFour, PiOverFour, XIsPositive);
	XMVECTOR R5 = XMVectorSelect(PiOverTwo, R4, XEqualsInfinity);
	XMVECTOR Result = XMVectorSelect(R3, R5, YEqualsInfinity);
	ATanResultValid = XMVectorEqualInt(Result, ATanResultValid);

	XMVECTOR Reciprocal = XMVectorReciprocalEst(X);
	XMVECTOR V = XMVectorMultiply(Y, Reciprocal);
	XMVECTOR R0 = XMVectorATanEst(V);

	R1 = XMVectorSelect(Pi, g_XMNegativeZero, XIsPositive);
	R2 = XMVectorAdd(R0, R1);

	Result = XMVectorSelect(Result, R2, ATanResultValid);

	return Result;

#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorLerp
(
	FXMVECTOR V0,
	FXMVECTOR V1,
	float    t
)
{
	// V0 + t * (V1 - V0)

#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Scale = XMVectorReplicate(t);
	XMVECTOR Length = XMVectorSubtract(V1, V0);
	return XMVectorMultiplyAdd(Length, Scale, V0);

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	XMVECTOR L = vsubq_f32(V1, V0);
	return vmlaq_n_f32(V0, L, t);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR L = _mm_sub_ps(V1, V0);
	XMVECTOR S = _mm_set_ps1(t);
	XMVECTOR Result = _mm_mul_ps(L, S);
	return _mm_add_ps(Result, V0);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorLerpV
(
	FXMVECTOR V0,
	FXMVECTOR V1,
	FXMVECTOR T
)
{
	// V0 + T * (V1 - V0)

#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR Length = XMVectorSubtract(V1, V0);
	return XMVectorMultiplyAdd(Length, T, V0);

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	XMVECTOR L = vsubq_f32(V1, V0);
	return vmlaq_f32(V0, L, T);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR Length = _mm_sub_ps(V1, V0);
	XMVECTOR Result = _mm_mul_ps(Length, T);
	return _mm_add_ps(Result, V0);
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorHermite
(
	FXMVECTOR Position0,
	FXMVECTOR Tangent0,
	FXMVECTOR Position1,
	GXMVECTOR Tangent1,
	float    t
)
{
	// Result = (2 * t^3 - 3 * t^2 + 1) * Position0 +
	//          (t^3 - 2 * t^2 + t) * Tangent0 +
	//          (-2 * t^3 + 3 * t^2) * Position1 +
	//          (t^3 - t^2) * Tangent1

#if defined(_XM_NO_INTRINSICS_)

	float t2 = t * t;
	float t3 = t * t2;

	XMVECTOR P0 = XMVectorReplicate(2.0f * t3 - 3.0f * t2 + 1.0f);
	XMVECTOR T0 = XMVectorReplicate(t3 - 2.0f * t2 + t);
	XMVECTOR P1 = XMVectorReplicate(-2.0f * t3 + 3.0f * t2);
	XMVECTOR T1 = XMVectorReplicate(t3 - t2);

	XMVECTOR Result = XMVectorMultiply(P0, Position0);
	Result = XMVectorMultiplyAdd(T0, Tangent0, Result);
	Result = XMVectorMultiplyAdd(P1, Position1, Result);
	Result = XMVectorMultiplyAdd(T1, Tangent1, Result);

	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float t2 = t * t;
	float t3 = t * t2;

	float p0 = 2.0f * t3 - 3.0f * t2 + 1.0f;
	float t0 = t3 - 2.0f * t2 + t;
	float p1 = -2.0f * t3 + 3.0f * t2;
	float t1 = t3 - t2;

	XMVECTOR vResult = vmulq_n_f32(Position0, p0);
	vResult = vmlaq_n_f32(vResult, Tangent0, t0);
	vResult = vmlaq_n_f32(vResult, Position1, p1);
	vResult = vmlaq_n_f32(vResult, Tangent1, t1);
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	float t2 = t * t;
	float t3 = t * t2;

	XMVECTOR P0 = _mm_set_ps1(2.0f * t3 - 3.0f * t2 + 1.0f);
	XMVECTOR T0 = _mm_set_ps1(t3 - 2.0f * t2 + t);
	XMVECTOR P1 = _mm_set_ps1(-2.0f * t3 + 3.0f * t2);
	XMVECTOR T1 = _mm_set_ps1(t3 - t2);

	XMVECTOR vResult = _mm_mul_ps(P0, Position0);
	XMVECTOR vTemp = _mm_mul_ps(T0, Tangent0);
	vResult = _mm_add_ps(vResult, vTemp);
	vTemp = _mm_mul_ps(P1, Position1);
	vResult = _mm_add_ps(vResult, vTemp);
	vTemp = _mm_mul_ps(T1, Tangent1);
	vResult = _mm_add_ps(vResult, vTemp);
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorHermiteV
(
	FXMVECTOR Position0,
	FXMVECTOR Tangent0,
	FXMVECTOR Position1,
	GXMVECTOR Tangent1,
	HXMVECTOR T
)
{
	// Result = (2 * t^3 - 3 * t^2 + 1) * Position0 +
	//          (t^3 - 2 * t^2 + t) * Tangent0 +
	//          (-2 * t^3 + 3 * t^2) * Position1 +
	//          (t^3 - t^2) * Tangent1

#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR T2 = XMVectorMultiply(T, T);
	XMVECTOR T3 = XMVectorMultiply(T, T2);

	XMVECTOR P0 = XMVectorReplicate(2.0f * T3.vector4_f32[0] - 3.0f * T2.vector4_f32[0] + 1.0f);
	XMVECTOR T0 = XMVectorReplicate(T3.vector4_f32[1] - 2.0f * T2.vector4_f32[1] + T.vector4_f32[1]);
	XMVECTOR P1 = XMVectorReplicate(-2.0f * T3.vector4_f32[2] + 3.0f * T2.vector4_f32[2]);
	XMVECTOR T1 = XMVectorReplicate(T3.vector4_f32[3] - T2.vector4_f32[3]);

	XMVECTOR Result = XMVectorMultiply(P0, Position0);
	Result = XMVectorMultiplyAdd(T0, Tangent0, Result);
	Result = XMVectorMultiplyAdd(P1, Position1, Result);
	Result = XMVectorMultiplyAdd(T1, Tangent1, Result);

	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	static const XMVECTORF32 CatMulT2 = { -3.0f,-2.0f,3.0f,-1.0f };
	static const XMVECTORF32 CatMulT3 = { 2.0f,1.0f,-2.0f,1.0f };

	XMVECTOR T2 = vmulq_f32(T, T);
	XMVECTOR T3 = vmulq_f32(T, T2);
	// Mul by the constants against t^2
	T2 = vmulq_f32(T2, CatMulT2);
	// Mul by the constants against t^3
	T3 = vmlaq_f32(T2, T3, CatMulT3);
	// T3 now has the pre-result.
	// I need to add t.y only
	T2 = vandq_u32(T, g_XMMaskY);
	T3 = vaddq_f32(T3, T2);
	// Add 1.0f to x
	T3 = vaddq_f32(T3, g_XMIdentityR0);
	// Now, I have the constants created
	// Mul the x constant to Position0
	XMVECTOR vResult = vmulq_lane_f32(Position0, vget_low_f32(T3), 0); // T3[0]
																	   // Mul the y constant to Tangent0
	vResult = vmlaq_lane_f32(vResult, Tangent0, vget_low_f32(T3), 1); // T3[1]
																	  // Mul the z constant to Position1
	vResult = vmlaq_lane_f32(vResult, Position1, vget_high_f32(T3), 0); // T3[2]
																		// Mul the w constant to Tangent1
	vResult = vmlaq_lane_f32(vResult, Tangent1, vget_high_f32(T3), 1); // T3[3]
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	static const XMVECTORF32 CatMulT2 = { -3.0f,-2.0f,3.0f,-1.0f };
	static const XMVECTORF32 CatMulT3 = { 2.0f,1.0f,-2.0f,1.0f };

	XMVECTOR T2 = _mm_mul_ps(T, T);
	XMVECTOR T3 = _mm_mul_ps(T, T2);
	// Mul by the constants against t^2
	T2 = _mm_mul_ps(T2, CatMulT2);
	// Mul by the constants against t^3
	T3 = _mm_mul_ps(T3, CatMulT3);
	// T3 now has the pre-result.
	T3 = _mm_add_ps(T3, T2);
	// I need to add t.y only
	T2 = _mm_and_ps(T, g_XMMaskY);
	T3 = _mm_add_ps(T3, T2);
	// Add 1.0f to x
	T3 = _mm_add_ps(T3, g_XMIdentityR0);
	// Now, I have the constants created
	// Mul the x constant to Position0
	XMVECTOR vResult = XM_PERMUTE_PS(T3, _MM_SHUFFLE(0, 0, 0, 0));
	vResult = _mm_mul_ps(vResult, Position0);
	// Mul the y constant to Tangent0
	T2 = XM_PERMUTE_PS(T3, _MM_SHUFFLE(1, 1, 1, 1));
	T2 = _mm_mul_ps(T2, Tangent0);
	vResult = _mm_add_ps(vResult, T2);
	// Mul the z constant to Position1
	T2 = XM_PERMUTE_PS(T3, _MM_SHUFFLE(2, 2, 2, 2));
	T2 = _mm_mul_ps(T2, Position1);
	vResult = _mm_add_ps(vResult, T2);
	// Mul the w constant to Tangent1
	T3 = XM_PERMUTE_PS(T3, _MM_SHUFFLE(3, 3, 3, 3));
	T3 = _mm_mul_ps(T3, Tangent1);
	vResult = _mm_add_ps(vResult, T3);
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorCatmullRom
(
	FXMVECTOR Position0,
	FXMVECTOR Position1,
	FXMVECTOR Position2,
	GXMVECTOR Position3,
	float    t
)
{
	// Result = ((-t^3 + 2 * t^2 - t) * Position0 +
	//           (3 * t^3 - 5 * t^2 + 2) * Position1 +
	//           (-3 * t^3 + 4 * t^2 + t) * Position2 +
	//           (t^3 - t^2) * Position3) * 0.5

#if defined(_XM_NO_INTRINSICS_)

	float t2 = t * t;
	float t3 = t * t2;

	XMVECTOR P0 = XMVectorReplicate((-t3 + 2.0f * t2 - t) * 0.5f);
	XMVECTOR P1 = XMVectorReplicate((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
	XMVECTOR P2 = XMVectorReplicate((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
	XMVECTOR P3 = XMVectorReplicate((t3 - t2) * 0.5f);

	XMVECTOR Result = XMVectorMultiply(P0, Position0);
	Result = XMVectorMultiplyAdd(P1, Position1, Result);
	Result = XMVectorMultiplyAdd(P2, Position2, Result);
	Result = XMVectorMultiplyAdd(P3, Position3, Result);

	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float t2 = t * t;
	float t3 = t * t2;

	float p0 = (-t3 + 2.0f * t2 - t) * 0.5f;
	float p1 = (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f;
	float p2 = (-3.0f * t3 + 4.0f * t2 + t) * 0.5f;
	float p3 = (t3 - t2) * 0.5f;

	XMVECTOR P1 = vmulq_n_f32(Position1, p1);
	XMVECTOR P0 = vmlaq_n_f32(P1, Position0, p0);
	XMVECTOR P3 = vmulq_n_f32(Position3, p3);
	XMVECTOR P2 = vmlaq_n_f32(P3, Position2, p2);
	P0 = vaddq_f32(P0, P2);
	return P0;
#elif defined(_XM_SSE_INTRINSICS_)
	float t2 = t * t;
	float t3 = t * t2;

	XMVECTOR P0 = _mm_set_ps1((-t3 + 2.0f * t2 - t) * 0.5f);
	XMVECTOR P1 = _mm_set_ps1((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
	XMVECTOR P2 = _mm_set_ps1((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
	XMVECTOR P3 = _mm_set_ps1((t3 - t2) * 0.5f);

	P0 = _mm_mul_ps(P0, Position0);
	P1 = _mm_mul_ps(P1, Position1);
	P2 = _mm_mul_ps(P2, Position2);
	P3 = _mm_mul_ps(P3, Position3);
	P0 = _mm_add_ps(P0, P1);
	P2 = _mm_add_ps(P2, P3);
	P0 = _mm_add_ps(P0, P2);
	return P0;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorCatmullRomV
(
	FXMVECTOR Position0,
	FXMVECTOR Position1,
	FXMVECTOR Position2,
	GXMVECTOR Position3,
	HXMVECTOR T
)
{
#if defined(_XM_NO_INTRINSICS_)
	float fx = T.vector4_f32[0];
	float fy = T.vector4_f32[1];
	float fz = T.vector4_f32[2];
	float fw = T.vector4_f32[3];
	XMVECTOR vResult;
	vResult.vector4_f32[0] = 0.5f*((-fx*fx*fx + 2 * fx*fx - fx)*Position0.vector4_f32[0]
		+ (3 * fx*fx*fx - 5 * fx*fx + 2)*Position1.vector4_f32[0]
		+ (-3 * fx*fx*fx + 4 * fx*fx + fx)*Position2.vector4_f32[0]
		+ (fx*fx*fx - fx*fx)*Position3.vector4_f32[0]);
	vResult.vector4_f32[1] = 0.5f*((-fy*fy*fy + 2 * fy*fy - fy)*Position0.vector4_f32[1]
		+ (3 * fy*fy*fy - 5 * fy*fy + 2)*Position1.vector4_f32[1]
		+ (-3 * fy*fy*fy + 4 * fy*fy + fy)*Position2.vector4_f32[1]
		+ (fy*fy*fy - fy*fy)*Position3.vector4_f32[1]);
	vResult.vector4_f32[2] = 0.5f*((-fz*fz*fz + 2 * fz*fz - fz)*Position0.vector4_f32[2]
		+ (3 * fz*fz*fz - 5 * fz*fz + 2)*Position1.vector4_f32[2]
		+ (-3 * fz*fz*fz + 4 * fz*fz + fz)*Position2.vector4_f32[2]
		+ (fz*fz*fz - fz*fz)*Position3.vector4_f32[2]);
	vResult.vector4_f32[3] = 0.5f*((-fw*fw*fw + 2 * fw*fw - fw)*Position0.vector4_f32[3]
		+ (3 * fw*fw*fw - 5 * fw*fw + 2)*Position1.vector4_f32[3]
		+ (-3 * fw*fw*fw + 4 * fw*fw + fw)*Position2.vector4_f32[3]
		+ (fw*fw*fw - fw*fw)*Position3.vector4_f32[3]);
	return vResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	static const XMVECTORF32 Catmul2 = { 2.0f,2.0f,2.0f,2.0f };
	static const XMVECTORF32 Catmul3 = { 3.0f,3.0f,3.0f,3.0f };
	static const XMVECTORF32 Catmul4 = { 4.0f,4.0f,4.0f,4.0f };
	static const XMVECTORF32 Catmul5 = { 5.0f,5.0f,5.0f,5.0f };
	// Cache T^2 and T^3
	XMVECTOR T2 = vmulq_f32(T, T);
	XMVECTOR T3 = vmulq_f32(T, T2);
	// Perform the Position0 term
	XMVECTOR vResult = vaddq_f32(T2, T2);
	vResult = vsubq_f32(vResult, T);
	vResult = vsubq_f32(vResult, T3);
	vResult = vmulq_f32(vResult, Position0);
	// Perform the Position1 term and add
	XMVECTOR vTemp = vmulq_f32(T3, Catmul3);
	vTemp = vmlsq_f32(vTemp, T2, Catmul5);
	vTemp = vaddq_f32(vTemp, Catmul2);
	vResult = vmlaq_f32(vResult, vTemp, Position1);
	// Perform the Position2 term and add
	vTemp = vmulq_f32(T2, Catmul4);
	vTemp = vmlsq_f32(vTemp, T3, Catmul3);
	vTemp = vaddq_f32(vTemp, T);
	vResult = vmlaq_f32(vResult, vTemp, Position2);
	// Position3 is the last term
	T3 = vsubq_f32(T3, T2);
	vResult = vmlaq_f32(vResult, T3, Position3);
	// Multiply by 0.5f and exit
	vResult = vmulq_f32(vResult, g_XMOneHalf);
	return vResult;
#elif defined(_XM_SSE_INTRINSICS_)
	static const XMVECTORF32 Catmul2 = { 2.0f,2.0f,2.0f,2.0f };
	static const XMVECTORF32 Catmul3 = { 3.0f,3.0f,3.0f,3.0f };
	static const XMVECTORF32 Catmul4 = { 4.0f,4.0f,4.0f,4.0f };
	static const XMVECTORF32 Catmul5 = { 5.0f,5.0f,5.0f,5.0f };
	// Cache T^2 and T^3
	XMVECTOR T2 = _mm_mul_ps(T, T);
	XMVECTOR T3 = _mm_mul_ps(T, T2);
	// Perform the Position0 term
	XMVECTOR vResult = _mm_add_ps(T2, T2);
	vResult = _mm_sub_ps(vResult, T);
	vResult = _mm_sub_ps(vResult, T3);
	vResult = _mm_mul_ps(vResult, Position0);
	// Perform the Position1 term and add
	XMVECTOR vTemp = _mm_mul_ps(T3, Catmul3);
	XMVECTOR vTemp2 = _mm_mul_ps(T2, Catmul5);
	vTemp = _mm_sub_ps(vTemp, vTemp2);
	vTemp = _mm_add_ps(vTemp, Catmul2);
	vTemp = _mm_mul_ps(vTemp, Position1);
	vResult = _mm_add_ps(vResult, vTemp);
	// Perform the Position2 term and add
	vTemp = _mm_mul_ps(T2, Catmul4);
	vTemp2 = _mm_mul_ps(T3, Catmul3);
	vTemp = _mm_sub_ps(vTemp, vTemp2);
	vTemp = _mm_add_ps(vTemp, T);
	vTemp = _mm_mul_ps(vTemp, Position2);
	vResult = _mm_add_ps(vResult, vTemp);
	// Position3 is the last term
	T3 = _mm_sub_ps(T3, T2);
	T3 = _mm_mul_ps(T3, Position3);
	vResult = _mm_add_ps(vResult, T3);
	// Multiply by 0.5f and exit
	vResult = _mm_mul_ps(vResult, g_XMOneHalf);
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorBaryCentric
(
	FXMVECTOR Position0,
	FXMVECTOR Position1,
	FXMVECTOR Position2,
	float    f,
	float    g
)
{
	// Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)

#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR P10 = XMVectorSubtract(Position1, Position0);
	XMVECTOR ScaleF = XMVectorReplicate(f);

	XMVECTOR P20 = XMVectorSubtract(Position2, Position0);
	XMVECTOR ScaleG = XMVectorReplicate(g);

	XMVECTOR Result = XMVectorMultiplyAdd(P10, ScaleF, Position0);
	Result = XMVectorMultiplyAdd(P20, ScaleG, Result);

	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	XMVECTOR R1 = vsubq_f32(Position1, Position0);
	XMVECTOR R2 = vsubq_f32(Position2, Position0);
	R1 = vmlaq_n_f32(Position0, R1, f);
	return vmlaq_n_f32(R1, R2, g);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR R1 = _mm_sub_ps(Position1, Position0);
	XMVECTOR SF = _mm_set_ps1(f);
	XMVECTOR R2 = _mm_sub_ps(Position2, Position0);
	XMVECTOR SG = _mm_set_ps1(g);
	R1 = _mm_mul_ps(R1, SF);
	R2 = _mm_mul_ps(R2, SG);
	R1 = _mm_add_ps(R1, Position0);
	R1 = _mm_add_ps(R1, R2);
	return R1;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMVectorBaryCentricV
(
	FXMVECTOR Position0,
	FXMVECTOR Position1,
	FXMVECTOR Position2,
	GXMVECTOR F,
	HXMVECTOR G
)
{
	// Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)

#if defined(_XM_NO_INTRINSICS_)

	XMVECTOR P10 = XMVectorSubtract(Position1, Position0);
	XMVECTOR P20 = XMVectorSubtract(Position2, Position0);

	XMVECTOR Result = XMVectorMultiplyAdd(P10, F, Position0);
	Result = XMVectorMultiplyAdd(P20, G, Result);

	return Result;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
	XMVECTOR R1 = vsubq_f32(Position1, Position0);
	XMVECTOR R2 = vsubq_f32(Position2, Position0);
	R1 = vmlaq_f32(Position0, R1, F);
	return vmlaq_f32(R1, R2, G);
#elif defined(_XM_SSE_INTRINSICS_)
	XMVECTOR R1 = _mm_sub_ps(Position1, Position0);
	XMVECTOR R2 = _mm_sub_ps(Position2, Position0);
	R1 = _mm_mul_ps(R1, F);
	R2 = _mm_mul_ps(R2, G);
	R1 = _mm_add_ps(R1, Position0);
	R1 = _mm_add_ps(R1, R2);
	return R1;
#endif
}