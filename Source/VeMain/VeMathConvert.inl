////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMathConvert.inl
//  Created:     2015/08/18 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeConvertVectorIntToFloat(
	VE_FVECTOR VInt, VeUInt32 DivExponent) noexcept
{
	VE_ASSERT(DivExponent < 32);
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fScale = 1.0f / (VeFloat32)(1U << DivExponent);
	VeUInt32 ElementIndex = 0;
	VE_VECTOR Result;
	do
	{
		VeInt32 iTemp = (VeInt32)VInt.vector4_u32[ElementIndex];
		Result.vector4_f32[ElementIndex] = ((VeFloat32)iTemp) * fScale;
	}
	while (++ElementIndex < 4);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_cvtepi32_ps(_mm_castps_si128(VInt));
	VeUInt32 uScale = 0x3F800000U - (DivExponent << 23);
	__m128i vScale = _mm_set1_epi32(uScale);
	vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(vScale));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeConvertVectorFloatToInt(
	VE_FVECTOR VFloat, VeUInt32 MulExponent) noexcept
{
	VE_ASSERT(MulExponent<32);
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fScale = (VeFloat32)(1U << MulExponent);
	VeUInt32 ElementIndex = 0;
	VE_VECTOR Result;
	do
	{
		VeInt32 iResult;
		VeFloat32 fTemp = VFloat.vector4_f32[ElementIndex] * fScale;
		if (fTemp <= -(65536.0f*32768.0f))
		{
			iResult = (-0x7FFFFFFF) - 1;
		}
		else if (fTemp > (65536.0f*32768.0f) - 128.0f)
		{
			iResult = 0x7FFFFFFF;
		}
		else
		{
			iResult = (int32_t)fTemp;
		}
		Result.vector4_u32[ElementIndex] = (uint32_t)iResult;
	}
	while (++ElementIndex < 4);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_set_ps1((VeFloat32)(1U << MulExponent));
	vResult = _mm_mul_ps(vResult, VFloat);
	VE_VECTOR vOverflow = _mm_cmpgt_ps(vResult, g_MathMaxInt);
	__m128i vResulti = _mm_cvttps_epi32(vResult);
	vResult = _mm_and_ps(vOverflow, g_MathAbsMask);
	vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
	vOverflow = _mm_or_ps(vOverflow, vResult);
	return vOverflow;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeConvertVectorUIntToFloat(
	VE_FVECTOR VUInt, VeUInt32 DivExponent) noexcept
{
	VE_ASSERT(DivExponent < 32);
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fScale = 1.0f / (VeFloat32)(1U << DivExponent);
	VeUInt32 ElementIndex = 0;
	VE_VECTOR Result;
	do
	{
		Result.vector4_f32[ElementIndex] = (VeFloat32)VUInt.vector4_u32[ElementIndex] * fScale;
	}
	while (++ElementIndex < 4);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vMask = _mm_and_ps(VUInt, g_MathNegativeZero);
	VE_VECTOR vResult = _mm_xor_ps(VUInt, vMask);
	vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
	__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
	vMask = _mm_and_ps(_mm_castsi128_ps(iMask), g_MathFixUnsigned);
	vResult = _mm_add_ps(vResult, vMask);
	VeUInt32 uScale = 0x3F800000U - (DivExponent << 23);
	iMask = _mm_set1_epi32(uScale);
	vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(iMask));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeConvertVectorFloatToUInt(
	VE_FVECTOR VFloat, VeUInt32 MulExponent) noexcept
{
	VE_ASSERT(MulExponent < 32);
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fScale = (VeFloat32)(1U << MulExponent);
	uint32_t ElementIndex = 0;
	VE_VECTOR Result;
	do
	{
		VeUInt32 uResult;
		VeFloat32 fTemp = VFloat.vector4_f32[ElementIndex] * fScale;
		if (fTemp <= 0.0f)
		{
			uResult = 0;
		}
		else if (fTemp >= (65536.0f*65536.0f))
		{
			uResult = 0xFFFFFFFFU;
		}
		else
		{
			uResult = (VeUInt32)fTemp;
		}
		Result.vector4_u32[ElementIndex] = uResult;
	}
	while (++ElementIndex < 4);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_set_ps1(static_cast<VeFloat32>(1U << MulExponent));
	vResult = _mm_mul_ps(vResult, VFloat);
	vResult = _mm_max_ps(vResult, g_MathZero);
	VE_VECTOR vOverflow = _mm_cmpgt_ps(vResult, g_MathMaxUInt);
	VE_VECTOR vValue = g_MathUnsignedFix;
	VE_VECTOR vMask = _mm_cmpge_ps(vResult, vValue);
	vValue = _mm_and_ps(vValue, vMask);
	vResult = _mm_sub_ps(vResult, vValue);
	__m128i vResulti = _mm_cvttps_epi32(vResult);
	vMask = _mm_and_ps(vMask, g_MathNegativeZero);
	vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
	vResult = _mm_or_ps(vResult, vOverflow);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadInt(
	const VeUInt32* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_u32[0] = *pSource;
	V.vector4_u32[1] = 0;
	V.vector4_u32[2] = 0;
	V.vector4_u32[3] = 0;
	return V;
#	elif defined(VE_ENABLE_SSE)
	return _mm_load_ss(reinterpret_cast<const VeFloat32*>(pSource));
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadFloat(
	const float* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = *pSource;
	V.vector4_f32[1] = 0.f;
	V.vector4_f32[2] = 0.f;
	V.vector4_f32[3] = 0.f;
	return V;
#	elif defined(VE_ENABLE_SSE)
	return _mm_load_ss(pSource);
#	endif
}
//--------------------------------------------------------------------------
