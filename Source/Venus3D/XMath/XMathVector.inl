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
#if defined(_XM_NO_INTRINSICS_)
	return V.vector4_f32[i];
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	switch (i)
	{
	case 0:
		return vgetq_lane_f32(V, 0);
	case 1:
		return vgetq_lane_f32(V, 1);
	case 2:
		return vgetq_lane_f32(V, 2);
	case 3:
		return vgetq_lane_f32(V, 3);
	default:
		return NAN;
	}
#elif defined(_XM_SSE_INTRINSICS_)
	return V.m128_f32[i];
#endif
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


