//-------------------------------------------------------------------------------------
// XMathConvert.inl -- SIMD C++ Math library
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//  
// Copyright (c) Microsoft Corporation. All rights reserved.
//-------------------------------------------------------------------------------------

/****************************************************************************
*
* Data conversion
*
****************************************************************************/

//------------------------------------------------------------------------------

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4701)
//	C4701: false positives
#endif

inline XMVECTOR XM_CALLCONV XMConvertVectorIntToFloat
(
	FXMVECTOR    VInt,
	uint32_t     DivExponent
)
{
	assert(DivExponent < 32);
#if defined(_XM_NO_INTRINSICS_)
	float fScale = 1.0f / (float)(1U << DivExponent);
	uint32_t ElementIndex = 0;
	XMVECTOR Result;
	do {
		int32_t iTemp = (int32_t)VInt.vector4_u32[ElementIndex];
		Result.vector4_f32[ElementIndex] = ((float)iTemp) * fScale;
	} while (++ElementIndex < 4);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float fScale = 1.0f / (float)(1U << DivExponent);
	float32x4_t vResult = vcvtq_f32_s32(VInt);
	return vmulq_n_f32(vResult, fScale);
#else // _XM_SSE_INTRINSICS_
	// Convert to floats
	XMVECTOR vResult = _mm_cvtepi32_ps(_mm_castps_si128(VInt));
	// Convert DivExponent into 1.0f/(1<<DivExponent)
	uint32_t uScale = 0x3F800000U - (DivExponent << 23);
	// Splat the scalar value
	__m128i vScale = _mm_set1_epi32(uScale);
	vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(vScale));
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMConvertVectorFloatToInt
(
	FXMVECTOR    VFloat,
	uint32_t     MulExponent
)
{
	assert(MulExponent < 32);
#if defined(_XM_NO_INTRINSICS_)
	// Get the scalar factor.
	float fScale = (float)(1U << MulExponent);
	uint32_t ElementIndex = 0;
	XMVECTOR Result;
	do {
		int32_t iResult;
		float fTemp = VFloat.vector4_f32[ElementIndex] * fScale;
		if (fTemp <= -(65536.0f*32768.0f)) {
			iResult = (-0x7FFFFFFF) - 1;
		}
		else if (fTemp > (65536.0f*32768.0f) - 128.0f) {
			iResult = 0x7FFFFFFF;
		}
		else {
			iResult = (int32_t)fTemp;
		}
		Result.vector4_u32[ElementIndex] = (uint32_t)iResult;
	} while (++ElementIndex < 4);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float32x4_t vResult = vmulq_n_f32(VFloat, (float)(1U << MulExponent));
	// In case of positive overflow, detect it
	uint32x4_t vOverflow = vcgtq_f32(vResult, g_XMMaxInt);
	// Float to int conversion
	int32x4_t vResulti = vcvtq_s32_f32(vResult);
	// If there was positive overflow, set to 0x7FFFFFFF
	vResult = vandq_u32(vOverflow, g_XMAbsMask);
	vOverflow = vbicq_u32(vResulti, vOverflow);
	vOverflow = vorrq_u32(vOverflow, vResult);
	return vOverflow;
#else // _XM_SSE_INTRINSICS_
	XMVECTOR vResult = _mm_set_ps1((float)(1U << MulExponent));
	vResult = _mm_mul_ps(vResult, VFloat);
	// In case of positive overflow, detect it
	XMVECTOR vOverflow = _mm_cmpgt_ps(vResult, g_XMMaxInt);
	// Float to int conversion
	__m128i vResulti = _mm_cvttps_epi32(vResult);
	// If there was positive overflow, set to 0x7FFFFFFF
	vResult = _mm_and_ps(vOverflow, g_XMAbsMask);
	vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
	vOverflow = _mm_or_ps(vOverflow, vResult);
	return vOverflow;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMConvertVectorUIntToFloat
(
	FXMVECTOR     VUInt,
	uint32_t      DivExponent
)
{
	assert(DivExponent < 32);
#if defined(_XM_NO_INTRINSICS_)
	float fScale = 1.0f / (float)(1U << DivExponent);
	uint32_t ElementIndex = 0;
	XMVECTOR Result;
	do {
		Result.vector4_f32[ElementIndex] = (float)VUInt.vector4_u32[ElementIndex] * fScale;
	} while (++ElementIndex < 4);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float fScale = 1.0f / (float)(1U << DivExponent);
	float32x4_t vResult = vcvtq_f32_u32(VUInt);
	return vmulq_n_f32(vResult, fScale);
#else // _XM_SSE_INTRINSICS_
	// For the values that are higher than 0x7FFFFFFF, a fixup is needed
	// Determine which ones need the fix.
	XMVECTOR vMask = _mm_and_ps(VUInt, g_XMNegativeZero);
	// Force all values positive
	XMVECTOR vResult = _mm_xor_ps(VUInt, vMask);
	// Convert to floats
	vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
	// Convert 0x80000000 -> 0xFFFFFFFF
	__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
	// For only the ones that are too big, add the fixup
	vMask = _mm_and_ps(_mm_castsi128_ps(iMask), g_XMFixUnsigned);
	vResult = _mm_add_ps(vResult, vMask);
	// Convert DivExponent into 1.0f/(1<<DivExponent)
	uint32_t uScale = 0x3F800000U - (DivExponent << 23);
	// Splat
	iMask = _mm_set1_epi32(uScale);
	vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(iMask));
	return vResult;
#endif
}

//------------------------------------------------------------------------------

inline XMVECTOR XM_CALLCONV XMConvertVectorFloatToUInt
(
	FXMVECTOR     VFloat,
	uint32_t      MulExponent
)
{
	assert(MulExponent < 32);
#if defined(_XM_NO_INTRINSICS_)
	// Get the scalar factor.
	float fScale = (float)(1U << MulExponent);
	uint32_t ElementIndex = 0;
	XMVECTOR Result;
	do {
		uint32_t uResult;
		float fTemp = VFloat.vector4_f32[ElementIndex] * fScale;
		if (fTemp <= 0.0f) {
			uResult = 0;
		}
		else if (fTemp >= (65536.0f*65536.0f)) {
			uResult = 0xFFFFFFFFU;
		}
		else {
			uResult = (uint32_t)fTemp;
		}
		Result.vector4_u32[ElementIndex] = uResult;
	} while (++ElementIndex < 4);
	return Result;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
	float32x4_t vResult = vmulq_n_f32(VFloat, (float)(1U << MulExponent));
	// In case of overflow, detect it
	uint32x4_t vOverflow = vcgtq_f32(vResult, g_XMMaxUInt);
	// Float to int conversion
	uint32x4_t vResulti = vcvtq_u32_f32(vResult);
	// If there was overflow, set to 0xFFFFFFFFU
	vResult = vbicq_u32(vResulti, vOverflow);
	vOverflow = vorrq_u32(vOverflow, vResult);
	return vOverflow;
#else // _XM_SSE_INTRINSICS_
	XMVECTOR vResult = _mm_set_ps1(static_cast<float>(1U << MulExponent));
	vResult = _mm_mul_ps(vResult, VFloat);
	// Clamp to >=0
	vResult = _mm_max_ps(vResult, g_XMZero);
	// Any numbers that are too big, set to 0xFFFFFFFFU
	XMVECTOR vOverflow = _mm_cmpgt_ps(vResult, g_XMMaxUInt);
	XMVECTOR vValue = g_XMUnsignedFix;
	// Too large for a signed integer?
	XMVECTOR vMask = _mm_cmpge_ps(vResult, vValue);
	// Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
	vValue = _mm_and_ps(vValue, vMask);
	// Perform fixup only on numbers too large (Keeps low bit precision)
	vResult = _mm_sub_ps(vResult, vValue);
	__m128i vResulti = _mm_cvttps_epi32(vResult);
	// Convert from signed to unsigned pnly if greater than 0x80000000
	vMask = _mm_and_ps(vMask, g_XMNegativeZero);
	vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
	// On those that are too large, set to 0xFFFFFFFF
	vResult = _mm_or_ps(vResult, vOverflow);
	return vResult;
#endif
}

#ifdef _MSC_VER
#	pragma warning(pop)
#endif
