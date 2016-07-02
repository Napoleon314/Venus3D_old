////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Venus3D
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   VeSIMDMathConvert.inl
//  Created:     2016/07/03 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VMVECTOR VE_VECTORCALL VeConvertVectorIntToFloat(
	FVMVECTOR    VInt,
	uint32_t     DivExponent
) noexcept
{
	assert(DivExponent < 32);
#if defined(VE_NO_INTRINSICS)
	float fScale = 1.0f / (float)(1U << DivExponent);
	uint32_t ElementIndex = 0;
	VMVECTOR Result;
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
	VMVECTOR vResult = _mm_cvtepi32_ps(_mm_castps_si128(VInt));
	// Convert DivExponent into 1.0f/(1<<DivExponent)
	uint32_t uScale = 0x3F800000U - (DivExponent << 23);
	// Splat the scalar value
	__m128i vScale = _mm_set1_epi32(uScale);
	vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(vScale));
	return vResult;
#endif
}
//--------------------------------------------------------------------------

