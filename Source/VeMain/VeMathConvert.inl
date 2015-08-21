////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMathConvert.inl
//  Created:     2015/08/21 by Napoleon
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
inline VE_VECTOR VE_MATH_CALLCONV VeLoadInt2(
	const VeUInt32* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_u32[0] = pSource[0];
	V.vector4_u32[1] = pSource[1];
	V.vector4_u32[2] = 0;
	V.vector4_u32[3] = 0;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128 x = _mm_load_ss(reinterpret_cast<const VeFloat32*>(pSource));
	__m128 y = _mm_load_ss(reinterpret_cast<const VeFloat32*>(pSource + 1));
	return _mm_unpacklo_ps(x, y);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadInt2A(
	const VeUInt32* pSource) noexcept
{
	VE_ASSERT(pSource);
	VE_ASSERT(((VeSizeT)pSource & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_u32[0] = pSource[0];
	V.vector4_u32[1] = pSource[1];
	V.vector4_u32[2] = 0;
	V.vector4_u32[3] = 0;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(pSource));
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadFloat2(
	const VE_FLOAT2* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = pSource->x;
	V.vector4_f32[1] = pSource->y;
	V.vector4_f32[2] = 0.f;
	V.vector4_f32[3] = 0.f;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128 x = _mm_load_ss(&pSource->x);
	__m128 y = _mm_load_ss(&pSource->y);
	return _mm_unpacklo_ps(x, y);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadFloat2A(
	const VE_FLOAT2A* pSource) noexcept
{
	VE_ASSERT(pSource);
	VE_ASSERT(((VeSizeT)pSource & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = pSource->x;
	V.vector4_f32[1] = pSource->y;
	V.vector4_f32[2] = 0.f;
	V.vector4_f32[3] = 0.f;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(pSource));
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadSInt2(
	const VE_INT2* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = (VeFloat32)pSource->x;
	V.vector4_f32[1] = (VeFloat32)pSource->y;
	V.vector4_f32[2] = 0.f;
	V.vector4_f32[3] = 0.f;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128 x = _mm_load_ss(reinterpret_cast<const VeFloat32*>(&pSource->x));
	__m128 y = _mm_load_ss(reinterpret_cast<const VeFloat32*>(&pSource->y));
	__m128 V = _mm_unpacklo_ps(x, y);
	return _mm_cvtepi32_ps(_mm_castps_si128(V));
#	endif
}
//------------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadUInt2(
	const VE_UINT2* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = (VeFloat32)pSource->x;
	V.vector4_f32[1] = (VeFloat32)pSource->y;
	V.vector4_f32[2] = 0.f;
	V.vector4_f32[3] = 0.f;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128 x = _mm_load_ss(reinterpret_cast<const VeFloat32*>(&pSource->x));
	__m128 y = _mm_load_ss(reinterpret_cast<const VeFloat32*>(&pSource->y));
	__m128 V = _mm_unpacklo_ps(x, y);
	VE_VECTOR vMask = _mm_and_ps(V, g_MathNegativeZero);
	VE_VECTOR vResult = _mm_xor_ps(V, vMask);
	vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
	__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
	vMask = _mm_and_ps(_mm_castsi128_ps(iMask), g_MathFixUnsigned);
	vResult = _mm_add_ps(vResult, vMask);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadInt3(
	const VeUInt32* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_u32[0] = pSource[0];
	V.vector4_u32[1] = pSource[1];
	V.vector4_u32[2] = pSource[2];
	V.vector4_u32[3] = 0;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128 x = _mm_load_ss(reinterpret_cast<const VeFloat32*>(pSource));
	__m128 y = _mm_load_ss(reinterpret_cast<const VeFloat32*>(pSource + 1));
	__m128 z = _mm_load_ss(reinterpret_cast<const VeFloat32*>(pSource + 2));
	__m128 xy = _mm_unpacklo_ps(x, y);
	return _mm_movelh_ps(xy, z);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadInt3A(
	const VeUInt32* pSource) noexcept
{
	VE_ASSERT(pSource);
	VE_ASSERT(((VeSizeT)pSource & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_u32[0] = pSource[0];
	V.vector4_u32[1] = pSource[1];
	V.vector4_u32[2] = pSource[2];
	V.vector4_u32[3] = 0;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_load_si128(reinterpret_cast<const __m128i*>(pSource));
	V = _mm_and_si128(V, g_MathMask3);
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadFloat3(
	const VE_FLOAT3* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = pSource->x;
	V.vector4_f32[1] = pSource->y;
	V.vector4_f32[2] = pSource->z;
	V.vector4_f32[3] = 0.f;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128 x = _mm_load_ss(&pSource->x);
	__m128 y = _mm_load_ss(&pSource->y);
	__m128 z = _mm_load_ss(&pSource->z);
	__m128 xy = _mm_unpacklo_ps(x, y);
	return _mm_movelh_ps(xy, z);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadFloat3A(
	const VE_FLOAT3A* pSource) noexcept
{
	VE_ASSERT(pSource);
	VE_ASSERT(((VeSizeT)pSource & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = pSource->x;
	V.vector4_f32[1] = pSource->y;
	V.vector4_f32[2] = pSource->z;
	V.vector4_f32[3] = 0.f;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128 V = _mm_load_ps(&pSource->x);
	return _mm_and_ps(V, g_MathMask3);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadSInt3(
	const VE_INT3* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = (VeFloat32)pSource->x;
	V.vector4_f32[1] = (VeFloat32)pSource->y;
	V.vector4_f32[2] = (VeFloat32)pSource->z;
	V.vector4_f32[3] = 0.f;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128 x = _mm_load_ss(reinterpret_cast<const VeFloat32*>(&pSource->x));
	__m128 y = _mm_load_ss(reinterpret_cast<const VeFloat32*>(&pSource->y));
	__m128 z = _mm_load_ss(reinterpret_cast<const VeFloat32*>(&pSource->z));
	__m128 xy = _mm_unpacklo_ps(x, y);
	__m128 V = _mm_movelh_ps(xy, z);
	return _mm_cvtepi32_ps(_mm_castps_si128(V));
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadUInt3(
	const VE_UINT3* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = (VeFloat32)pSource->x;
	V.vector4_f32[1] = (VeFloat32)pSource->y;
	V.vector4_f32[2] = (VeFloat32)pSource->z;
	V.vector4_f32[3] = 0.f;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128 x = _mm_load_ss(reinterpret_cast<const VeFloat32*>(&pSource->x));
	__m128 y = _mm_load_ss(reinterpret_cast<const VeFloat32*>(&pSource->y));
	__m128 z = _mm_load_ss(reinterpret_cast<const VeFloat32*>(&pSource->z));
	__m128 xy = _mm_unpacklo_ps(x, y);
	__m128 V = _mm_movelh_ps(xy, z);
	VE_VECTOR vMask = _mm_and_ps(V, g_MathNegativeZero);
	VE_VECTOR vResult = _mm_xor_ps(V, vMask);
	vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
	__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
	vMask = _mm_and_ps(_mm_castsi128_ps(iMask), g_MathFixUnsigned);
	vResult = _mm_add_ps(vResult, vMask);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadInt4(
	const VeUInt32* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_u32[0] = pSource[0];
	V.vector4_u32[1] = pSource[1];
	V.vector4_u32[2] = pSource[2];
	V.vector4_u32[3] = pSource[3];
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadInt4A(
	const VeUInt32* pSource) noexcept
{
	VE_ASSERT(pSource);
	VE_ASSERT(((VeSizeT)pSource & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_u32[0] = pSource[0];
	V.vector4_u32[1] = pSource[1];
	V.vector4_u32[2] = pSource[2];
	V.vector4_u32[3] = pSource[3];
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_load_si128(reinterpret_cast<const __m128i*>(pSource));
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadFloat4(
	const VE_FLOAT4* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = pSource->x;
	V.vector4_f32[1] = pSource->y;
	V.vector4_f32[2] = pSource->z;
	V.vector4_f32[3] = pSource->w;
	return V;
#	elif defined(VE_ENABLE_SSE)
	return _mm_loadu_ps(&pSource->x);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadFloat4A(
	const VE_FLOAT4A* pSource) noexcept
{
	VE_ASSERT(pSource);
	VE_ASSERT(((VeSizeT)pSource & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = pSource->x;
	V.vector4_f32[1] = pSource->y;
	V.vector4_f32[2] = pSource->z;
	V.vector4_f32[3] = pSource->w;
	return V;
#	elif defined(VE_ENABLE_SSE)
	return _mm_load_ps(&pSource->x);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadSInt4(
	const VE_INT4* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = (VeFloat32)pSource->x;
	V.vector4_f32[1] = (VeFloat32)pSource->y;
	V.vector4_f32[2] = (VeFloat32)pSource->z;
	V.vector4_f32[3] = (VeFloat32)pSource->w;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
	return _mm_cvtepi32_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeLoadUInt4(
	const VE_UINT4* pSource) noexcept
{
	VE_ASSERT(pSource);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	V.vector4_f32[0] = (VeFloat32)pSource->x;
	V.vector4_f32[1] = (VeFloat32)pSource->y;
	V.vector4_f32[2] = (VeFloat32)pSource->z;
	V.vector4_f32[3] = (VeFloat32)pSource->w;
	return V;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
	VE_VECTOR vMask = _mm_and_ps(_mm_castsi128_ps(V), g_MathNegativeZero);
	VE_VECTOR vResult = _mm_xor_ps(_mm_castsi128_ps(V), vMask);
	vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
	__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
	vMask = _mm_and_ps(_mm_castsi128_ps(iMask), g_MathFixUnsigned);
	vResult = _mm_add_ps(vResult, vMask);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreInt(
	VeUInt32* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	*pDestination = V.vector4_u32[0];
#	elif defined(VE_ENABLE_SSE)
	_mm_store_ss(reinterpret_cast<VeFloat32*>(pDestination), V);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreFloat(
	VeFloat32* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	*pDestination = V.vector4_f32[0];
#	elif defined(VE_ENABLE_SSE)
	_mm_store_ss(pDestination, V);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreInt2(
	VeUInt32* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination[0] = V.vector4_u32[0];
	pDestination[1] = V.vector4_u32[1];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR T = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination[0]), V);
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination[1]), T);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreInt2A(
	VeUInt32* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
	VE_ASSERT(((VeSizeT)pDestination & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	pDestination[0] = V.vector4_u32[0];
	pDestination[1] = V.vector4_u32[1];
#	elif defined(VE_ENABLE_SSE)
	_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreFloat2(
	VE_FLOAT2* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = V.vector4_f32[0];
	pDestination->y = V.vector4_f32[1];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR T = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	_mm_store_ss(&pDestination->x, V);
	_mm_store_ss(&pDestination->y, T);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreFloat2A(
	VE_FLOAT2A* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
	VE_ASSERT(((VeSizeT)pDestination & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = V.vector4_f32[0];
	pDestination->y = V.vector4_f32[1];
#	elif defined(VE_ENABLE_SSE)
	_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreSInt2(
	VE_INT2* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = (VeInt32)V.vector4_f32[0];
	pDestination->y = (VeInt32)V.vector4_f32[1];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vOverflow = _mm_cmpgt_ps(V, g_MathMaxInt);
	__m128i vResulti = _mm_cvttps_epi32(V);
	VE_VECTOR vResult = _mm_and_ps(vOverflow, g_MathAbsMask);
	vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
	vOverflow = _mm_or_ps(vOverflow, vResult);
	VE_VECTOR T = VE_PERMUTE_PS(vOverflow, _MM_SHUFFLE(1, 1, 1, 1));
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination->x), vOverflow);
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination->y), T);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreUInt2(
	VE_UINT2* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = (VeUInt32)V.vector4_f32[0];
	pDestination->y = (VeUInt32)V.vector4_f32[1];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_max_ps(V, g_MathZero);
	VE_VECTOR vOverflow = _mm_cmpgt_ps(vResult, g_MathMaxUInt);
	VE_VECTOR vValue = g_MathUnsignedFix;
	VE_VECTOR vMask = _mm_cmpge_ps(vResult, vValue);
	vValue = _mm_and_ps(vValue, vMask);
	vResult = _mm_sub_ps(vResult, vValue);
	__m128i vResulti = _mm_cvttps_epi32(vResult);
	vMask = _mm_and_ps(vMask, g_MathNegativeZero);
	vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
	vResult = _mm_or_ps(vResult, vOverflow);
	VE_VECTOR T = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(1, 1, 1, 1));
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination->x), vResult);
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination->y), T);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreInt3(
	VeUInt32* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination[0] = V.vector4_u32[0];
	pDestination[1] = V.vector4_u32[1];
	pDestination[2] = V.vector4_u32[2];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR T1 = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	VE_VECTOR T2 = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(reinterpret_cast<VeFloat32*>(pDestination), V);
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination[1]), T1);
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination[2]), T2);
#	endif
}
//------------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreInt3A(
	VeUInt32* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
	VE_ASSERT(((VeSizeT)pDestination & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	pDestination[0] = V.vector4_u32[0];
	pDestination[1] = V.vector4_u32[1];
	pDestination[2] = V.vector4_u32[2];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR T = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination[2]), T);
#	endif
}
//------------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreFloat3(
	VE_FLOAT3* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = V.vector4_f32[0];
	pDestination->y = V.vector4_f32[1];
	pDestination->z = V.vector4_f32[2];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR T1 = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	VE_VECTOR T2 = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(&pDestination->x, V);
	_mm_store_ss(&pDestination->y, T1);
	_mm_store_ss(&pDestination->z, T2);
#	endif
}
//------------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreFloat3A(
	VE_FLOAT3A* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
	VE_ASSERT(((VeSizeT)pDestination & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = V.vector4_f32[0];
	pDestination->y = V.vector4_f32[1];
	pDestination->z = V.vector4_f32[2];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR T = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
	_mm_store_ss(&pDestination->z, T);
#	endif
}
//------------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreSInt3(
	VE_INT3* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = (VeInt32)V.vector4_f32[0];
	pDestination->y = (VeInt32)V.vector4_f32[1];
	pDestination->z = (VeInt32)V.vector4_f32[2];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vOverflow = _mm_cmpgt_ps(V, g_MathMaxInt);
	__m128i vResulti = _mm_cvttps_epi32(V);
	VE_VECTOR vResult = _mm_and_ps(vOverflow, g_MathAbsMask);
	vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
	vOverflow = _mm_or_ps(vOverflow, vResult);
	VE_VECTOR T1 = VE_PERMUTE_PS(vOverflow, _MM_SHUFFLE(1, 1, 1, 1));
	VE_VECTOR T2 = VE_PERMUTE_PS(vOverflow, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(reinterpret_cast<float*>(&pDestination->x), vOverflow);
	_mm_store_ss(reinterpret_cast<float*>(&pDestination->y), T1);
	_mm_store_ss(reinterpret_cast<float*>(&pDestination->z), T2);
#	endif
}
//------------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreUInt3(
	VE_UINT3* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = (VeUInt32)V.vector4_f32[0];
	pDestination->y = (VeUInt32)V.vector4_f32[1];
	pDestination->z = (VeUInt32)V.vector4_f32[2];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_max_ps(V, g_MathZero);
	VE_VECTOR vOverflow = _mm_cmpgt_ps(vResult, g_MathMaxUInt);
	VE_VECTOR vValue = g_MathUnsignedFix;
	VE_VECTOR vMask = _mm_cmpge_ps(vResult, vValue);
	vValue = _mm_and_ps(vValue, vMask);
	vResult = _mm_sub_ps(vResult, vValue);
	__m128i vResulti = _mm_cvttps_epi32(vResult);
	vMask = _mm_and_ps(vMask, g_MathNegativeZero);
	vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
	vResult = _mm_or_ps(vResult, vOverflow);
	VE_VECTOR T1 = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(1, 1, 1, 1));
	VE_VECTOR T2 = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination->x), vResult);
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination->y), T1);
	_mm_store_ss(reinterpret_cast<VeFloat32*>(&pDestination->z), T2);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreInt4(
	VeUInt32* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination[0] = V.vector4_u32[0];
	pDestination[1] = V.vector4_u32[1];
	pDestination[2] = V.vector4_u32[2];
	pDestination[3] = V.vector4_u32[3];
#	elif defined(VE_ENABLE_SSE)
	_mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#	endif
}
//------------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreInt4A(
	VeUInt32* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
	VE_ASSERT(((VeSizeT)pDestination & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	pDestination[0] = V.vector4_u32[0];
	pDestination[1] = V.vector4_u32[1];
	pDestination[2] = V.vector4_u32[2];
	pDestination[3] = V.vector4_u32[3];
#	elif defined(VE_ENABLE_SSE)
	_mm_store_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreFloat4(
	VE_FLOAT4* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = V.vector4_f32[0];
	pDestination->y = V.vector4_f32[1];
	pDestination->z = V.vector4_f32[2];
	pDestination->w = V.vector4_f32[3];
#	elif defined(VE_ENABLE_SSE)
	_mm_storeu_ps(&pDestination->x, V);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreFloat4A(
	VE_FLOAT4A* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
	VE_ASSERT(((VeSizeT)pDestination & 0xF) == 0);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = V.vector4_f32[0];
	pDestination->y = V.vector4_f32[1];
	pDestination->z = V.vector4_f32[2];
	pDestination->w = V.vector4_f32[3];
#	elif defined(VE_ENABLE_SSE)
	_mm_store_ps(&pDestination->x, V);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreSInt4(
	VE_INT4* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = (VeInt32)V.vector4_f32[0];
	pDestination->y = (VeInt32)V.vector4_f32[1];
	pDestination->z = (VeInt32)V.vector4_f32[2];
	pDestination->w = (VeInt32)V.vector4_f32[3];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vOverflow = _mm_cmpgt_ps(V, g_MathMaxInt);
	__m128i vResulti = _mm_cvttps_epi32(V);
	VE_VECTOR vResult = _mm_and_ps(vOverflow, g_MathAbsMask);
	vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
	vOverflow = _mm_or_ps(vOverflow, vResult);
	_mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(vOverflow));
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeStoreUInt4(
	VE_UINT4* pDestination, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pDestination);
#	if defined(VE_NO_INTRINSICS)
	pDestination->x = (VeUInt32)V.vector4_f32[0];
	pDestination->y = (VeUInt32)V.vector4_f32[1];
	pDestination->z = (VeUInt32)V.vector4_f32[2];
	pDestination->w = (VeUInt32)V.vector4_f32[3];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_max_ps(V, g_MathZero);
	VE_VECTOR vOverflow = _mm_cmpgt_ps(vResult, g_MathMaxUInt);
	VE_VECTOR vValue = g_MathUnsignedFix;
	VE_VECTOR vMask = _mm_cmpge_ps(vResult, vValue);
	vValue = _mm_and_ps(vValue, vMask);
	vResult = _mm_sub_ps(vResult, vValue);
	__m128i vResulti = _mm_cvttps_epi32(vResult);
	vMask = _mm_and_ps(vMask, g_MathNegativeZero);
	vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
	vResult = _mm_or_ps(vResult, vOverflow);
	_mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(vResult));
#	endif
}
//--------------------------------------------------------------------------
