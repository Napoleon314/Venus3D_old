////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMathMatrix.inl
//  Created:     2015/08/21 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeMatrixIsNaN(
	VE_FMATRIX M) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeSizeT i = 16;
	const VeUInt32 *pWork = (const VeUInt32*)(&M.m[0][0]);
	do
	{
		VeUInt32 uTest = pWork[0];
		uTest &= 0x7FFFFFFFU;
		uTest -= 0x7F800001U;
		if (uTest<0x007FFFFFU)
		{
			break;
		}
		++pWork;
	}
	while (--i);
	return (i != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vX = M.r[0];
	VE_VECTOR vY = M.r[1];
	VE_VECTOR vZ = M.r[2];
	VE_VECTOR vW = M.r[3];
	vX = _mm_cmpneq_ps(vX, vX);
	vY = _mm_cmpneq_ps(vY, vY);
	vZ = _mm_cmpneq_ps(vZ, vZ);
	vW = _mm_cmpneq_ps(vW, vW);
	vX = _mm_or_ps(vX, vZ);
	vY = _mm_or_ps(vY, vW);
	vX = _mm_or_ps(vX, vY);
	return (_mm_movemask_ps(vX) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeMatrixIsInfinite(
	VE_FMATRIX M) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeSizeT i = 16;
	const VeUInt32 *pWork = (const VeUInt32 *)(&M.m[0][0]);
	do
	{
		VeUInt32 uTest = pWork[0];
		uTest &= 0x7FFFFFFFU;
		if (uTest == 0x7F800000U)
		{
			break;
		}
		++pWork;
	}
	while (--i);
	return (i != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp1 = _mm_and_ps(M.r[0], g_MathAbsMask);
	VE_VECTOR vTemp2 = _mm_and_ps(M.r[1], g_MathAbsMask);
	VE_VECTOR vTemp3 = _mm_and_ps(M.r[2], g_MathAbsMask);
	VE_VECTOR vTemp4 = _mm_and_ps(M.r[3], g_MathAbsMask);
	vTemp1 = _mm_cmpeq_ps(vTemp1, g_MathInfinity);
	vTemp2 = _mm_cmpeq_ps(vTemp2, g_MathInfinity);
	vTemp3 = _mm_cmpeq_ps(vTemp3, g_MathInfinity);
	vTemp4 = _mm_cmpeq_ps(vTemp4, g_MathInfinity);
	vTemp1 = _mm_or_ps(vTemp1, vTemp2);
	vTemp3 = _mm_or_ps(vTemp3, vTemp4);
	vTemp1 = _mm_or_ps(vTemp1, vTemp3);
	return (_mm_movemask_ps(vTemp1) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeMatrixIsIdentity(
	VE_FMATRIX M) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	const VeUInt32 *pWork = (const VeUInt32*)(&M.m[0][0]);
	VeUInt32 uOne = pWork[0] ^ 0x3F800000U;
	VeUInt32 uZero = pWork[1];
	uZero |= pWork[2];
	uZero |= pWork[3];
	uZero |= pWork[4];
	uOne |= pWork[5] ^ 0x3F800000U;
	uZero |= pWork[6];
	uZero |= pWork[7];
	uZero |= pWork[8];
	uZero |= pWork[9];
	uOne |= pWork[10] ^ 0x3F800000U;
	uZero |= pWork[11];
	uZero |= pWork[12];
	uZero |= pWork[13];
	uZero |= pWork[14];
	uOne |= pWork[15] ^ 0x3F800000U;
	uZero &= 0x7FFFFFFF;
	uOne |= uZero;
	return (uOne == 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp1 = _mm_cmpeq_ps(M.r[0], g_MathIdentityR0);
	VE_VECTOR vTemp2 = _mm_cmpeq_ps(M.r[1], g_MathIdentityR1);
	VE_VECTOR vTemp3 = _mm_cmpeq_ps(M.r[2], g_MathIdentityR2);
	VE_VECTOR vTemp4 = _mm_cmpeq_ps(M.r[3], g_MathIdentityR3);
	vTemp1 = _mm_and_ps(vTemp1, vTemp2);
	vTemp3 = _mm_and_ps(vTemp3, vTemp4);
	vTemp1 = _mm_and_ps(vTemp1, vTemp3);
	return (_mm_movemask_ps(vTemp1) == 0x0f);
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixMultiply(
	VE_FMATRIX M1, VE_CMATRIX M2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_MATRIX mResult;
	VeFloat32 x = M1.m[0][0];
	VeFloat32 y = M1.m[0][1];
	VeFloat32 z = M1.m[0][2];
	VeFloat32 w = M1.m[0][3];
	mResult.m[0][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
	mResult.m[0][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
	mResult.m[0][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
	mResult.m[0][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
	x = M1.m[1][0];
	y = M1.m[1][1];
	z = M1.m[1][2];
	w = M1.m[1][3];
	mResult.m[1][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
	mResult.m[1][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
	mResult.m[1][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
	mResult.m[1][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
	x = M1.m[2][0];
	y = M1.m[2][1];
	z = M1.m[2][2];
	w = M1.m[2][3];
	mResult.m[2][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
	mResult.m[2][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
	mResult.m[2][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
	mResult.m[2][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
	x = M1.m[3][0];
	y = M1.m[3][1];
	z = M1.m[3][2];
	w = M1.m[3][3];
	mResult.m[3][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
	mResult.m[3][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
	mResult.m[3][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
	mResult.m[3][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
	return mResult;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX mResult;
	VE_VECTOR vW = M1.r[0];
	VE_VECTOR vX = VE_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	VE_VECTOR vY = VE_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	VE_VECTOR vZ = VE_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = VE_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX = _mm_mul_ps(vX, M2.r[0]);
	vY = _mm_mul_ps(vY, M2.r[1]);
	vZ = _mm_mul_ps(vZ, M2.r[2]);
	vW = _mm_mul_ps(vW, M2.r[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	mResult.r[0] = vX;
	vW = M1.r[1];
	vX = VE_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = VE_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = VE_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = VE_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX = _mm_mul_ps(vX, M2.r[0]);
	vY = _mm_mul_ps(vY, M2.r[1]);
	vZ = _mm_mul_ps(vZ, M2.r[2]);
	vW = _mm_mul_ps(vW, M2.r[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	mResult.r[1] = vX;
	vW = M1.r[2];
	vX = VE_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = VE_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = VE_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = VE_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX = _mm_mul_ps(vX, M2.r[0]);
	vY = _mm_mul_ps(vY, M2.r[1]);
	vZ = _mm_mul_ps(vZ, M2.r[2]);
	vW = _mm_mul_ps(vW, M2.r[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	mResult.r[2] = vX;
	vW = M1.r[3];
	vX = VE_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = VE_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = VE_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = VE_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX = _mm_mul_ps(vX, M2.r[0]);
	vY = _mm_mul_ps(vY, M2.r[1]);
	vZ = _mm_mul_ps(vZ, M2.r[2]);
	vW = _mm_mul_ps(vW, M2.r[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	mResult.r[3] = vX;
	return mResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixMultiplyTranspose(
	VE_FMATRIX M1, VE_CMATRIX M2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_MATRIX mResult;
	VeFloat32 x = M2.m[0][0];
	VeFloat32 y = M2.m[1][0];
	VeFloat32 z = M2.m[2][0];
	VeFloat32 w = M2.m[3][0];
	mResult.m[0][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
	mResult.m[0][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
	mResult.m[0][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
	mResult.m[0][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
	x = M2.m[0][1];
	y = M2.m[1][1];
	z = M2.m[2][1];
	w = M2.m[3][1];
	mResult.m[1][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
	mResult.m[1][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
	mResult.m[1][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
	mResult.m[1][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
	x = M2.m[0][2];
	y = M2.m[1][2];
	z = M2.m[2][2];
	w = M2.m[3][2];
	mResult.m[2][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
	mResult.m[2][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
	mResult.m[2][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
	mResult.m[2][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
	x = M2.m[0][3];
	y = M2.m[1][3];
	z = M2.m[2][3];
	w = M2.m[3][3];
	mResult.m[3][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
	mResult.m[3][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
	mResult.m[3][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
	mResult.m[3][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
	return mResult;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vW = M1.r[0];
	VE_VECTOR vX = VE_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	VE_VECTOR vY = VE_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	VE_VECTOR vZ = VE_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = VE_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX = _mm_mul_ps(vX, M2.r[0]);
	vY = _mm_mul_ps(vY, M2.r[1]);
	vZ = _mm_mul_ps(vZ, M2.r[2]);
	vW = _mm_mul_ps(vW, M2.r[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	__m128 r0 = vX;
	vW = M1.r[1];
	vX = VE_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = VE_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = VE_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = VE_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX = _mm_mul_ps(vX, M2.r[0]);
	vY = _mm_mul_ps(vY, M2.r[1]);
	vZ = _mm_mul_ps(vZ, M2.r[2]);
	vW = _mm_mul_ps(vW, M2.r[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	__m128 r1 = vX;
	vW = M1.r[2];
	vX = VE_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = VE_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = VE_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = VE_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX = _mm_mul_ps(vX, M2.r[0]);
	vY = _mm_mul_ps(vY, M2.r[1]);
	vZ = _mm_mul_ps(vZ, M2.r[2]);
	vW = _mm_mul_ps(vW, M2.r[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	__m128 r2 = vX;
	vW = M1.r[3];
	vX = VE_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = VE_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = VE_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = VE_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX = _mm_mul_ps(vX, M2.r[0]);
	vY = _mm_mul_ps(vY, M2.r[1]);
	vZ = _mm_mul_ps(vZ, M2.r[2]);
	vW = _mm_mul_ps(vW, M2.r[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	__m128 r3 = vX;
	VE_VECTOR vTemp1 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 0, 1, 0));
	VE_VECTOR vTemp3 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(3, 2, 3, 2));
	VE_VECTOR vTemp2 = _mm_shuffle_ps(r2, r3, _MM_SHUFFLE(1, 0, 1, 0));
	VE_VECTOR vTemp4 = _mm_shuffle_ps(r2, r3, _MM_SHUFFLE(3, 2, 3, 2));
	VE_MATRIX mResult;
	mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
	mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
	mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
	mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
	return mResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixTranspose(
	VE_FMATRIX M) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_MATRIX P;
	P.r[0] = VeVectorMergeXY(M.r[0], M.r[2]);
	P.r[1] = VeVectorMergeXY(M.r[1], M.r[3]);
	P.r[2] = VeVectorMergeZW(M.r[0], M.r[2]);
	P.r[3] = VeVectorMergeZW(M.r[1], M.r[3]);
	VE_MATRIX MT;
	MT.r[0] = VeVectorMergeXY(P.r[0], P.r[1]);
	MT.r[1] = VeVectorMergeZW(P.r[0], P.r[1]);
	MT.r[2] = VeVectorMergeXY(P.r[2], P.r[3]);
	MT.r[3] = VeVectorMergeZW(P.r[2], P.r[3]);
	return MT;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
	VE_VECTOR vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
	VE_VECTOR vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
	VE_VECTOR vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));
	VE_MATRIX mResult;
	mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
	mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
	mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
	mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
	return mResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixInverse(
	VE_VECTOR* pDeterminant, VE_FMATRIX M) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_MATRIX MT = VeMatrixTranspose(M);
	VE_VECTOR V0[4], V1[4];
	V0[0] = VeVectorSwizzle<VE_SWIZZLE_X, VE_SWIZZLE_X, VE_SWIZZLE_Y, VE_SWIZZLE_Y>(MT.r[2]);
	V1[0] = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_W, VE_SWIZZLE_Z, VE_SWIZZLE_W>(MT.r[3]);
	V0[1] = VeVectorSwizzle<VE_SWIZZLE_X, VE_SWIZZLE_X, VE_SWIZZLE_Y, VE_SWIZZLE_Y>(MT.r[0]);
	V1[1] = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_W, VE_SWIZZLE_Z, VE_SWIZZLE_W>(MT.r[1]);
	V0[2] = VeVectorPermute<VE_PERMUTE_0X, VE_PERMUTE_0Z, VE_PERMUTE_1X, VE_PERMUTE_1Z>(MT.r[2], MT.r[0]);
	V1[2] = VeVectorPermute<VE_PERMUTE_0Y, VE_PERMUTE_0W, VE_PERMUTE_1Y, VE_PERMUTE_1W>(MT.r[3], MT.r[1]);
	VE_VECTOR D0 = VeVectorMultiply(V0[0], V1[0]);
	VE_VECTOR D1 = VeVectorMultiply(V0[1], V1[1]);
	VE_VECTOR D2 = VeVectorMultiply(V0[2], V1[2]);
	V0[0] = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_W, VE_SWIZZLE_Z, VE_SWIZZLE_W>(MT.r[2]);
	V1[0] = VeVectorSwizzle<VE_SWIZZLE_X, VE_SWIZZLE_X, VE_SWIZZLE_Y, VE_SWIZZLE_Y>(MT.r[3]);
	V0[1] = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_W, VE_SWIZZLE_Z, VE_SWIZZLE_W>(MT.r[0]);
	V1[1] = VeVectorSwizzle<VE_SWIZZLE_X, VE_SWIZZLE_X, VE_SWIZZLE_Y, VE_SWIZZLE_Y>(MT.r[1]);
	V0[2] = VeVectorPermute<VE_PERMUTE_0Y, VE_PERMUTE_0W, VE_PERMUTE_1Y, VE_PERMUTE_1W>(MT.r[2], MT.r[0]);
	V1[2] = VeVectorPermute<VE_PERMUTE_0X, VE_PERMUTE_0Z, VE_PERMUTE_1X, VE_PERMUTE_1Z>(MT.r[3], MT.r[1]);
	D0 = VeVectorNegativeMultiplySubtract(V0[0], V1[0], D0);
	D1 = VeVectorNegativeMultiplySubtract(V0[1], V1[1], D1);
	D2 = VeVectorNegativeMultiplySubtract(V0[2], V1[2], D2);
	V0[0] = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_Z, VE_SWIZZLE_X, VE_SWIZZLE_Y>(MT.r[1]);
	V1[0] = VeVectorPermute<VE_PERMUTE_1Y, VE_PERMUTE_0Y, VE_PERMUTE_0W, VE_PERMUTE_0X>(D0, D2);
	V0[1] = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_X, VE_SWIZZLE_Y, VE_SWIZZLE_X>(MT.r[0]);
	V1[1] = VeVectorPermute<VE_PERMUTE_0W, VE_PERMUTE_1Y, VE_PERMUTE_0Y, VE_PERMUTE_0Z>(D0, D2);
	V0[2] = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_Z, VE_SWIZZLE_X, VE_SWIZZLE_Y>(MT.r[3]);
	V1[2] = VeVectorPermute<VE_PERMUTE_1W, VE_PERMUTE_0Y, VE_PERMUTE_0W, VE_PERMUTE_0X>(D1, D2);
	V0[3] = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_X, VE_SWIZZLE_Y, VE_SWIZZLE_X>(MT.r[2]);
	V1[3] = VeVectorPermute<VE_PERMUTE_0W, VE_PERMUTE_1W, VE_PERMUTE_0Y, VE_PERMUTE_0Z>(D1, D2);
	VE_VECTOR C0 = VeVectorMultiply(V0[0], V1[0]);
	VE_VECTOR C2 = VeVectorMultiply(V0[1], V1[1]);
	VE_VECTOR C4 = VeVectorMultiply(V0[2], V1[2]);
	VE_VECTOR C6 = VeVectorMultiply(V0[3], V1[3]);
	V0[0] = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_W, VE_SWIZZLE_Y, VE_SWIZZLE_Z>(MT.r[1]);
	V1[0] = VeVectorPermute<VE_PERMUTE_0W, VE_PERMUTE_0X, VE_PERMUTE_0Y, VE_PERMUTE_1X>(D0, D2);
	V0[1] = VeVectorSwizzle<VE_SWIZZLE_W, VE_SWIZZLE_Z, VE_SWIZZLE_W, VE_SWIZZLE_Y>(MT.r[0]);
	V1[1] = VeVectorPermute<VE_PERMUTE_0Z, VE_PERMUTE_0Y, VE_PERMUTE_1X, VE_PERMUTE_0X>(D0, D2);
	V0[2] = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_W, VE_SWIZZLE_Y, VE_SWIZZLE_Z>(MT.r[3]);
	V1[2] = VeVectorPermute<VE_PERMUTE_0W, VE_PERMUTE_0X, VE_PERMUTE_0Y, VE_PERMUTE_1Z>(D1, D2);
	V0[3] = VeVectorSwizzle<VE_SWIZZLE_W, VE_SWIZZLE_Z, VE_SWIZZLE_W, VE_SWIZZLE_Y>(MT.r[2]);
	V1[3] = VeVectorPermute<VE_PERMUTE_0Z, VE_PERMUTE_0Y, VE_PERMUTE_1Z, VE_PERMUTE_0X>(D1, D2);
	C0 = VeVectorNegativeMultiplySubtract(V0[0], V1[0], C0);
	C2 = VeVectorNegativeMultiplySubtract(V0[1], V1[1], C2);
	C4 = VeVectorNegativeMultiplySubtract(V0[2], V1[2], C4);
	C6 = VeVectorNegativeMultiplySubtract(V0[3], V1[3], C6);
	V0[0] = VeVectorSwizzle<VE_SWIZZLE_W, VE_SWIZZLE_X, VE_SWIZZLE_W, VE_SWIZZLE_X>(MT.r[1]);
	V1[0] = VeVectorPermute<VE_PERMUTE_0Z, VE_PERMUTE_1Y, VE_PERMUTE_1X, VE_PERMUTE_0Z>(D0, D2);
	V0[1] = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_W, VE_SWIZZLE_X, VE_SWIZZLE_Z>(MT.r[0]);
	V1[1] = VeVectorPermute<VE_PERMUTE_1Y, VE_PERMUTE_0X, VE_PERMUTE_0W, VE_PERMUTE_1X>(D0, D2);
	V0[2] = VeVectorSwizzle<VE_SWIZZLE_W, VE_SWIZZLE_X, VE_SWIZZLE_W, VE_SWIZZLE_X>(MT.r[3]);
	V1[2] = VeVectorPermute<VE_PERMUTE_0Z, VE_PERMUTE_1W, VE_PERMUTE_1Z, VE_PERMUTE_0Z>(D1, D2);
	V0[3] = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_W, VE_SWIZZLE_X, VE_SWIZZLE_Z>(MT.r[2]);
	V1[3] = VeVectorPermute<VE_PERMUTE_1W, VE_PERMUTE_0X, VE_PERMUTE_0W, VE_PERMUTE_1Z>(D1, D2);
	VE_VECTOR C1 = VeVectorNegativeMultiplySubtract(V0[0], V1[0], C0);
	C0 = VeVectorMultiplyAdd(V0[0], V1[0], C0);
	VE_VECTOR C3 = VeVectorMultiplyAdd(V0[1], V1[1], C2);
	C2 = VeVectorNegativeMultiplySubtract(V0[1], V1[1], C2);
	VE_VECTOR C5 = VeVectorNegativeMultiplySubtract(V0[2], V1[2], C4);
	C4 = VeVectorMultiplyAdd(V0[2], V1[2], C4);
	VE_VECTOR C7 = VeVectorMultiplyAdd(V0[3], V1[3], C6);
	C6 = VeVectorNegativeMultiplySubtract(V0[3], V1[3], C6);
	VE_MATRIX R;
	R.r[0] = VeVectorSelect(C0, C1, g_MathSelect0101.v);
	R.r[1] = VeVectorSelect(C2, C3, g_MathSelect0101.v);
	R.r[2] = VeVectorSelect(C4, C5, g_MathSelect0101.v);
	R.r[3] = VeVectorSelect(C6, C7, g_MathSelect0101.v);
	VE_VECTOR Determinant = VeVector4Dot(R.r[0], MT.r[0]);
	if (pDeterminant != nullptr)
		*pDeterminant = Determinant;
	VE_VECTOR Reciprocal = VeVectorReciprocal(Determinant);
	VE_MATRIX Result;
	Result.r[0] = VeVectorMultiply(R.r[0], Reciprocal);
	Result.r[1] = VeVectorMultiply(R.r[1], Reciprocal);
	Result.r[2] = VeVectorMultiply(R.r[2], Reciprocal);
	Result.r[3] = VeVectorMultiply(R.r[3], Reciprocal);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX MT = VeMatrixTranspose(M);
	VE_VECTOR V00 = VE_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 1, 0, 0));
	VE_VECTOR V10 = VE_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(3, 2, 3, 2));
	VE_VECTOR V01 = VE_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 1, 0, 0));
	VE_VECTOR V11 = VE_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(3, 2, 3, 2));
	VE_VECTOR V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(2, 0, 2, 0));
	VE_VECTOR V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(3, 1, 3, 1));
	VE_VECTOR D0 = _mm_mul_ps(V00, V10);
	VE_VECTOR D1 = _mm_mul_ps(V01, V11);
	VE_VECTOR D2 = _mm_mul_ps(V02, V12);
	V00 = VE_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(3, 2, 3, 2));
	V10 = VE_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 1, 0, 0));
	V01 = VE_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(3, 2, 3, 2));
	V11 = VE_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 1, 0, 0));
	V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(3, 1, 3, 1));
	V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(2, 0, 2, 0));
	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	D0 = _mm_sub_ps(D0, V00);
	D1 = _mm_sub_ps(D1, V01);
	D2 = _mm_sub_ps(D2, V02);
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
	V00 = VE_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 0, 2, 1));
	V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
	V01 = VE_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(0, 1, 0, 2));
	V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
	VE_VECTOR V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
	V02 = VE_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 0, 2, 1));
	V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
	VE_VECTOR V03 = VE_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(0, 1, 0, 2));
	V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));
	VE_VECTOR C0 = _mm_mul_ps(V00, V10);
	VE_VECTOR C2 = _mm_mul_ps(V01, V11);
	VE_VECTOR C4 = _mm_mul_ps(V02, V12);
	VE_VECTOR C6 = _mm_mul_ps(V03, V13);
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
	V00 = VE_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(2, 1, 3, 2));
	V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
	V01 = VE_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 3, 2, 3));
	V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
	V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
	V02 = VE_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(2, 1, 3, 2));
	V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
	V03 = VE_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 3, 2, 3));
	V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));
	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	V03 = _mm_mul_ps(V03, V13);
	C0 = _mm_sub_ps(C0, V00);
	C2 = _mm_sub_ps(C2, V01);
	C4 = _mm_sub_ps(C4, V02);
	C6 = _mm_sub_ps(C6, V03);
	V00 = VE_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(0, 3, 0, 3));
	V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
	V10 = VE_PERMUTE_PS(V10, _MM_SHUFFLE(0, 2, 3, 0));
	V01 = VE_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(2, 0, 3, 1));
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
	V11 = VE_PERMUTE_PS(V11, _MM_SHUFFLE(2, 1, 0, 3));
	V02 = VE_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(0, 3, 0, 3));
	V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
	V12 = VE_PERMUTE_PS(V12, _MM_SHUFFLE(0, 2, 3, 0));
	V03 = VE_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(2, 0, 3, 1));
	V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
	V13 = VE_PERMUTE_PS(V13, _MM_SHUFFLE(2, 1, 0, 3));
	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	V03 = _mm_mul_ps(V03, V13);
	VE_VECTOR C1 = _mm_sub_ps(C0, V00);
	C0 = _mm_add_ps(C0, V00);
	VE_VECTOR C3 = _mm_add_ps(C2, V01);
	C2 = _mm_sub_ps(C2, V01);
	VE_VECTOR C5 = _mm_sub_ps(C4, V02);
	C4 = _mm_add_ps(C4, V02);
	VE_VECTOR C7 = _mm_add_ps(C6, V03);
	C6 = _mm_sub_ps(C6, V03);
	C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
	C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
	C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
	C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
	C0 = VE_PERMUTE_PS(C0, _MM_SHUFFLE(3, 1, 2, 0));
	C2 = VE_PERMUTE_PS(C2, _MM_SHUFFLE(3, 1, 2, 0));
	C4 = VE_PERMUTE_PS(C4, _MM_SHUFFLE(3, 1, 2, 0));
	C6 = VE_PERMUTE_PS(C6, _MM_SHUFFLE(3, 1, 2, 0));
	VE_VECTOR vTemp = VeVector4Dot(C0, MT.r[0]);
	if (pDeterminant != nullptr)
		*pDeterminant = vTemp;
	vTemp = _mm_div_ps(g_MathOne, vTemp);
	VE_MATRIX mResult;
	mResult.r[0] = _mm_mul_ps(C0, vTemp);
	mResult.r[1] = _mm_mul_ps(C2, vTemp);
	mResult.r[2] = _mm_mul_ps(C4, vTemp);
	mResult.r[3] = _mm_mul_ps(C6, vTemp);
	return mResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeMatrixDeterminant(
	VE_FMATRIX M) noexcept
{
	static const VE_VECTORF32 Sign = { 1.0f, -1.0f, 1.0f, -1.0f };
	VE_VECTOR V0 = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_X, VE_SWIZZLE_X, VE_SWIZZLE_X>(M.r[2]);
	VE_VECTOR V1 = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_Z, VE_SWIZZLE_Y, VE_SWIZZLE_Y>(M.r[3]);
	VE_VECTOR V2 = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_X, VE_SWIZZLE_X, VE_SWIZZLE_X>(M.r[2]);
	VE_VECTOR V3 = VeVectorSwizzle<VE_SWIZZLE_W, VE_SWIZZLE_W, VE_SWIZZLE_W, VE_SWIZZLE_Z>(M.r[3]);
	VE_VECTOR V4 = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_Z, VE_SWIZZLE_Y, VE_SWIZZLE_Y>(M.r[2]);
	VE_VECTOR V5 = VeVectorSwizzle<VE_SWIZZLE_W, VE_SWIZZLE_W, VE_SWIZZLE_W, VE_SWIZZLE_Z>(M.r[3]);
	VE_VECTOR P0 = VeVectorMultiply(V0, V1);
	VE_VECTOR P1 = VeVectorMultiply(V2, V3);
	VE_VECTOR P2 = VeVectorMultiply(V4, V5);
	V0 = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_Z, VE_SWIZZLE_Y, VE_SWIZZLE_Y>(M.r[2]);
	V1 = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_X, VE_SWIZZLE_X, VE_SWIZZLE_X>(M.r[3]);
	V2 = VeVectorSwizzle<VE_SWIZZLE_W, VE_SWIZZLE_W, VE_SWIZZLE_W, VE_SWIZZLE_Z>(M.r[2]);
	V3 = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_X, VE_SWIZZLE_X, VE_SWIZZLE_X>(M.r[3]);
	V4 = VeVectorSwizzle<VE_SWIZZLE_W, VE_SWIZZLE_W, VE_SWIZZLE_W, VE_SWIZZLE_Z>(M.r[2]);
	V5 = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_Z, VE_SWIZZLE_Y, VE_SWIZZLE_Y>(M.r[3]);
	P0 = VeVectorNegativeMultiplySubtract(V0, V1, P0);
	P1 = VeVectorNegativeMultiplySubtract(V2, V3, P1);
	P2 = VeVectorNegativeMultiplySubtract(V4, V5, P2);
	V0 = VeVectorSwizzle<VE_SWIZZLE_W, VE_SWIZZLE_W, VE_SWIZZLE_W, VE_SWIZZLE_Z>(M.r[1]);
	V1 = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_Z, VE_SWIZZLE_Y, VE_SWIZZLE_Y>(M.r[1]);
	V2 = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_X, VE_SWIZZLE_X, VE_SWIZZLE_X>(M.r[1]);
	VE_VECTOR S = VeVectorMultiply(M.r[0], Sign.v);
	VE_VECTOR R = VeVectorMultiply(V0, P0);
	R = VeVectorNegativeMultiplySubtract(V1, P1, R);
	R = VeVectorMultiplyAdd(V2, P2, R);
	return VeVector4Dot(S, R);
}
//--------------------------------------------------------------------------
#define VE3RANKDECOMPOSE(a, b, c, x, y, z)      \
    if((x) < (y))                   \
    {                               \
        if((y) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 1;                \
            (c) = 0;                \
        }                           \
        else                        \
        {                           \
            (a) = 1;                \
                                    \
            if((x) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 0;            \
            }                       \
            else                    \
            {                       \
                (b) = 0;            \
                (c) = 2;            \
            }                       \
        }                           \
    }                               \
    else                            \
    {                               \
        if((x) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 0;                \
            (c) = 1;                \
        }                           \
        else                        \
        {                           \
            (a) = 0;                \
                                    \
            if((y) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 1;            \
            }                       \
            else                    \
            {                       \
                (b) = 1;            \
                (c) = 2;            \
            }                       \
        }                           \
    }
//--------------------------------------------------------------------------
#define VE3_DECOMP_EPSILON 0.0001f
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionRotationMatrix(
	VE_FMATRIX M) noexcept;
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeMatrixDecompose(VE_VECTOR* outScale,
	VE_VECTOR* outRotQuat, VE_VECTOR* outTrans, VE_FMATRIX M) noexcept
{
	static const VE_VECTOR* pvCanonicalBasis[3] =
	{
		&g_MathIdentityR0.v,
		&g_MathIdentityR1.v,
		&g_MathIdentityR2.v
	};
	VE_ASSERT(outScale != nullptr);
	VE_ASSERT(outRotQuat != nullptr);
	VE_ASSERT(outTrans != nullptr);
	outTrans[0] = M.r[3];
	VE_VECTOR *ppvBasis[3];
	VE_MATRIX matTemp;
	ppvBasis[0] = &matTemp.r[0];
	ppvBasis[1] = &matTemp.r[1];
	ppvBasis[2] = &matTemp.r[2];
	matTemp.r[0] = M.r[0];
	matTemp.r[1] = M.r[1];
	matTemp.r[2] = M.r[2];
	matTemp.r[3] = g_MathIdentityR3.v;
	VeFloat32* pfScales = (VeFloat32*)outScale;
	VeSizeT a, b, c;
	VeVectorGetXPtr(&pfScales[0], VeVector3Length(ppvBasis[0][0]));
	VeVectorGetXPtr(&pfScales[1], VeVector3Length(ppvBasis[1][0]));
	VeVectorGetXPtr(&pfScales[2], VeVector3Length(ppvBasis[2][0]));
	pfScales[3] = 0.f;
	VE3RANKDECOMPOSE(a, b, c, pfScales[0], pfScales[1], pfScales[2]);
	if (pfScales[a] < VE3_DECOMP_EPSILON)
	{
		ppvBasis[a][0] = pvCanonicalBasis[a][0];
	}
	ppvBasis[a][0] = VeVector3Normalize(ppvBasis[a][0]);
	if (pfScales[b] < VE3_DECOMP_EPSILON)
	{
		VeSizeT aa, bb, cc;
		VeFloat32 fAbsX, fAbsY, fAbsZ;
		fAbsX = VeFabsf(VeVectorGetX(ppvBasis[a][0]));
		fAbsY = VeFabsf(VeVectorGetY(ppvBasis[a][0]));
		fAbsZ = VeFabsf(VeVectorGetZ(ppvBasis[a][0]));
		VE3RANKDECOMPOSE(aa, bb, cc, fAbsX, fAbsY, fAbsZ);
		ppvBasis[b][0] = VeVector3Cross(ppvBasis[a][0], pvCanonicalBasis[cc][0]);
	}
	ppvBasis[b][0] = VeVector3Normalize(ppvBasis[b][0]);
	if (pfScales[c] < VE3_DECOMP_EPSILON)
	{
		ppvBasis[c][0] = VeVector3Cross(ppvBasis[a][0], ppvBasis[b][0]);
	}
	ppvBasis[c][0] = VeVector3Normalize(ppvBasis[c][0]);
	VeFloat32 fDet = VeVectorGetX(VeMatrixDeterminant(matTemp));
	if (fDet < 0.0f)
	{
		pfScales[a] = -pfScales[a];
		ppvBasis[a][0] = VeVectorNegate(ppvBasis[a][0]);
		fDet = -fDet;
	}
	fDet -= 1.0f;
	fDet *= fDet;
	if (VE3_DECOMP_EPSILON < fDet)
	{
		return false;
	}
	outRotQuat[0] = VeQuaternionRotationMatrix(matTemp);
	return true;
}
//--------------------------------------------------------------------------
#undef VE3_DECOMP_EPSILON
#undef VE3RANKDECOMPOSE
//--------------------------------------------------------------------------
inline VE_MATRIX4X3 VE_MATH_CALLCONV VeMatrix4X3Identity() noexcept
{
	VE_MATRIX4X3 M;
	M.r[0] = g_MathIdentityR0.v;
	M.r[1] = g_MathIdentityR1.v;
	M.r[2] = g_MathIdentityR2.v;
	return M;
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixIdentity() noexcept
{
	VE_MATRIX M;
	M.r[0] = g_MathIdentityR0.v;
	M.r[1] = g_MathIdentityR1.v;
	M.r[2] = g_MathIdentityR2.v;
	M.r[3] = g_MathIdentityR3.v;
	return M;
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixSet(
	VeFloat32 m00, VeFloat32 m01, VeFloat32 m02, VeFloat32 m03,
	VeFloat32 m10, VeFloat32 m11, VeFloat32 m12, VeFloat32 m13,
	VeFloat32 m20, VeFloat32 m21, VeFloat32 m22, float m23,
	VeFloat32 m30, VeFloat32 m31, VeFloat32 m32, VeFloat32 m33
	) noexcept
{
	VE_MATRIX M;
#	if defined(VE_NO_INTRINSICS)
	M.m[0][0] = m00; M.m[0][1] = m01; M.m[0][2] = m02; M.m[0][3] = m03;
	M.m[1][0] = m10; M.m[1][1] = m11; M.m[1][2] = m12; M.m[1][3] = m13;
	M.m[2][0] = m20; M.m[2][1] = m21; M.m[2][2] = m22; M.m[2][3] = m23;
	M.m[3][0] = m30; M.m[3][1] = m31; M.m[3][2] = m32; M.m[3][3] = m33;
#	else
	M.r[0] = VeVectorSet(m00, m01, m02, m03);
	M.r[1] = VeVectorSet(m10, m11, m12, m13);
	M.r[2] = VeVectorSet(m20, m21, m22, m23);
	M.r[3] = VeVectorSet(m30, m31, m32, m33);
#	endif
	return M;
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixTranslation(
	VeFloat32 OffsetX, VeFloat32 OffsetY, VeFloat32 OffsetZ) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_MATRIX M;
	M.m[0][0] = 1.0f;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = 1.0f;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = 1.0f;
	M.m[2][3] = 0.0f;
	M.m[3][0] = OffsetX;
	M.m[3][1] = OffsetY;
	M.m[3][2] = OffsetZ;
	M.m[3][3] = 1.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	M.r[0] = g_MathIdentityR0.v;
	M.r[1] = g_MathIdentityR1.v;
	M.r[2] = g_MathIdentityR2.v;
	M.r[3] = VeVectorSet(OffsetX, OffsetY, OffsetZ, 1.f);
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixTranslationFromVector(
	VE_FVECTOR Offset) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_MATRIX M;
	M.m[0][0] = 1.0f;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = 1.0f;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = 1.0f;
	M.m[2][3] = 0.0f;
	M.m[3][0] = Offset.vector4_f32[0];
	M.m[3][1] = Offset.vector4_f32[1];
	M.m[3][2] = Offset.vector4_f32[2];
	M.m[3][3] = 1.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	M.r[0] = g_MathIdentityR0.v;
	M.r[1] = g_MathIdentityR1.v;
	M.r[2] = g_MathIdentityR2.v;
	M.r[3] = VeVectorSelect(g_MathIdentityR3.v, Offset, g_MathSelect1110.v);
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixScaling(
	VeFloat32 ScaleX, VeFloat32 ScaleY, VeFloat32 ScaleZ) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_MATRIX M;
	M.m[0][0] = ScaleX;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = ScaleY;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = ScaleZ;
	M.m[2][3] = 0.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = 0.0f;
	M.m[3][3] = 1.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	M.r[0] = _mm_set_ps(0, 0, 0, ScaleX);
	M.r[1] = _mm_set_ps(0, 0, ScaleY, 0);
	M.r[2] = _mm_set_ps(0, ScaleZ, 0, 0);
	M.r[3] = g_MathIdentityR3.v;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixScalingFromVector(
	VE_FVECTOR Scale) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_MATRIX M;
	M.m[0][0] = Scale.vector4_f32[0];
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = Scale.vector4_f32[1];
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = Scale.vector4_f32[2];
	M.m[2][3] = 0.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = 0.0f;
	M.m[3][3] = 1.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	M.r[0] = _mm_and_ps(Scale, g_MathMaskX);
	M.r[1] = _mm_and_ps(Scale, g_MathMaskY);
	M.r[2] = _mm_and_ps(Scale, g_MathMaskZ);
	M.r[3] = g_MathIdentityR3.v;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline void VeScalarSinCos(VeFloat32* pSin, VeFloat32* pCos,
	VeFloat32  Value) noexcept;
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixRotationX(
	VeFloat32 Angle) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fSinAngle;
	VeFloat32 fCosAngle;
	VeScalarSinCos(&fSinAngle, &fCosAngle, Angle);
	VE_MATRIX M;
	M.m[0][0] = 1.0f;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = fCosAngle;
	M.m[1][2] = fSinAngle;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = -fSinAngle;
	M.m[2][2] = fCosAngle;
	M.m[2][3] = 0.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = 0.0f;
	M.m[3][3] = 1.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VeFloat32 SinAngle;
	VeFloat32 CosAngle;
	VeScalarSinCos(&SinAngle, &CosAngle, Angle);
	VE_VECTOR vSin = _mm_set_ss(SinAngle);
	VE_VECTOR vCos = _mm_set_ss(CosAngle);
	vCos = _mm_shuffle_ps(vCos, vSin, _MM_SHUFFLE(3, 0, 0, 3));
	VE_MATRIX M;
	M.r[0] = g_MathIdentityR0;
	M.r[1] = vCos;
	vCos = VE_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 1, 2, 0));
	vCos = _mm_mul_ps(vCos, g_MathNegateY);
	M.r[2] = vCos;
	M.r[3] = g_MathIdentityR3;
	return M;
#	endif
}

//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixRotationY(
	VeFloat32 Angle) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fSinAngle;
	VeFloat32 fCosAngle;
	VeScalarSinCos(&fSinAngle, &fCosAngle, Angle);
	VE_MATRIX M;
	M.m[0][0] = fCosAngle;
	M.m[0][1] = 0.0f;
	M.m[0][2] = -fSinAngle;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = 1.0f;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = fSinAngle;
	M.m[2][1] = 0.0f;
	M.m[2][2] = fCosAngle;
	M.m[2][3] = 0.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = 0.0f;
	M.m[3][3] = 1.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VeFloat32 SinAngle;
	VeFloat32 CosAngle;
	VeScalarSinCos(&SinAngle, &CosAngle, Angle);
	VE_VECTOR vSin = _mm_set_ss(SinAngle);
	VE_VECTOR vCos = _mm_set_ss(CosAngle);
	vSin = _mm_shuffle_ps(vSin, vCos, _MM_SHUFFLE(3, 0, 3, 0));
	VE_MATRIX M;
	M.r[2] = vSin;
	M.r[1] = g_MathIdentityR1;
	vSin = VE_PERMUTE_PS(vSin, _MM_SHUFFLE(3, 0, 1, 2));
	vSin = _mm_mul_ps(vSin, g_MathNegateZ);
	M.r[0] = vSin;
	M.r[3] = g_MathIdentityR3;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixRotationZ(
	VeFloat32 Angle) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fSinAngle;
	VeFloat32 fCosAngle;
	VeScalarSinCos(&fSinAngle, &fCosAngle, Angle);
	VE_MATRIX M;
	M.m[0][0] = fCosAngle;
	M.m[0][1] = fSinAngle;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = -fSinAngle;
	M.m[1][1] = fCosAngle;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = 1.0f;
	M.m[2][3] = 0.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = 0.0f;
	M.m[3][3] = 1.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VeFloat32 SinAngle;
	VeFloat32 CosAngle;
	VeScalarSinCos(&SinAngle, &CosAngle, Angle);
	VE_VECTOR vSin = _mm_set_ss(SinAngle);
	VE_VECTOR vCos = _mm_set_ss(CosAngle);
	vCos = _mm_unpacklo_ps(vCos, vSin);
	VE_MATRIX M;
	M.r[0] = vCos;
	vCos = VE_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 2, 0, 1));
	vCos = _mm_mul_ps(vCos, g_MathNegateX);
	M.r[1] = vCos;
	M.r[2] = g_MathIdentityR2;
	M.r[3] = g_MathIdentityR3;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixRotationRollPitchYawFromVector(
	VE_FVECTOR Angles) noexcept;
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixRotationRollPitchYaw(
	VeFloat32 Pitch, VeFloat32 Yaw, VeFloat32 Roll) noexcept
{
	VE_VECTOR Angles = VeVectorSet(Pitch, Yaw, Roll, 0.0f);
	return VeMatrixRotationRollPitchYawFromVector(Angles);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionRotationRollPitchYawFromVector(
	VE_FVECTOR Angles) noexcept;
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixRotationQuaternion(
	VE_FVECTOR Quaternion) noexcept;
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixRotationRollPitchYawFromVector(
	VE_FVECTOR Angles) noexcept
{
	VE_VECTOR Q = VeQuaternionRotationRollPitchYawFromVector(Angles);
	return VeMatrixRotationQuaternion(Q);
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixRotationNormal(
	VE_FVECTOR NormalAxis, VeFloat32 Angle) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fSinAngle;
	VeFloat32 fCosAngle;
	VeScalarSinCos(&fSinAngle, &fCosAngle, Angle);
	VE_VECTOR A = VeVectorSet(fSinAngle, fCosAngle, 1.0f - fCosAngle, 0.0f);
	VE_VECTOR C2 = VeVectorSplatZ(A);
	VE_VECTOR C1 = VeVectorSplatY(A);
	VE_VECTOR C0 = VeVectorSplatX(A);
	VE_VECTOR N0 = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_Z, VE_SWIZZLE_X, VE_SWIZZLE_W>(NormalAxis);
	VE_VECTOR N1 = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_X, VE_SWIZZLE_Y, VE_SWIZZLE_W>(NormalAxis);
	VE_VECTOR V0 = VeVectorMultiply(C2, N0);
	V0 = VeVectorMultiply(V0, N1);
	VE_VECTOR R0 = VeVectorMultiply(C2, NormalAxis);
	R0 = VeVectorMultiplyAdd(R0, NormalAxis, C1);
	VE_VECTOR R1 = VeVectorMultiplyAdd(C0, NormalAxis, V0);
	VE_VECTOR R2 = VeVectorNegativeMultiplySubtract(C0, NormalAxis, V0);
	V0 = VeVectorSelect(A, R0, g_MathSelect1110.v);
	VE_VECTOR V1 = VeVectorPermute<VE_PERMUTE_0Z, VE_PERMUTE_1Y, VE_PERMUTE_1Z, VE_PERMUTE_0X>(R1, R2);
	VE_VECTOR V2 = VeVectorPermute<VE_PERMUTE_0Y, VE_PERMUTE_1X, VE_PERMUTE_0Y, VE_PERMUTE_1X>(R1, R2);
	VE_MATRIX M;
	M.r[0] = VeVectorPermute<VE_PERMUTE_0X, VE_PERMUTE_1X, VE_PERMUTE_1Y, VE_PERMUTE_0W>(V0, V1);
	M.r[1] = VeVectorPermute<VE_PERMUTE_1Z, VE_PERMUTE_0Y, VE_PERMUTE_1W, VE_PERMUTE_0W>(V0, V1);
	M.r[2] = VeVectorPermute<VE_PERMUTE_1X, VE_PERMUTE_1Y, VE_PERMUTE_0Z, VE_PERMUTE_0W>(V0, V2);
	M.r[3] = g_MathIdentityR3.v;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VeFloat32 fSinAngle;
	VeFloat32 fCosAngle;
	VeScalarSinCos(&fSinAngle, &fCosAngle, Angle);
	VE_VECTOR C2 = _mm_set_ps1(1.0f - fCosAngle);
	VE_VECTOR C1 = _mm_set_ps1(fCosAngle);
	VE_VECTOR C0 = _mm_set_ps1(fSinAngle);
	VE_VECTOR N0 = VE_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 0, 2, 1));
	VE_VECTOR N1 = VE_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 1, 0, 2));
	VE_VECTOR V0 = _mm_mul_ps(C2, N0);
	V0 = _mm_mul_ps(V0, N1);
	VE_VECTOR R0 = _mm_mul_ps(C2, NormalAxis);
	R0 = _mm_mul_ps(R0, NormalAxis);
	R0 = _mm_add_ps(R0, C1);
	VE_VECTOR R1 = _mm_mul_ps(C0, NormalAxis);
	R1 = _mm_add_ps(R1, V0);
	VE_VECTOR R2 = _mm_mul_ps(C0, NormalAxis);
	R2 = _mm_sub_ps(V0, R2);
	V0 = _mm_and_ps(R0, g_MathMask3);
	VE_VECTOR V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
	V1 = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 3, 2, 1));
	VE_VECTOR V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
	V2 = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 2, 0));
	R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
	R2 = VE_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 2, 0));
	VE_MATRIX M;
	M.r[0] = R2;
	R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
	R2 = VE_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 0, 2));
	M.r[1] = R2;
	V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
	M.r[2] = V2;
	M.r[3] = g_MathIdentityR3.v;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixRotationAxis(
	VE_FVECTOR Axis, VeFloat32 Angle) noexcept
{
	VE_ASSERT(!VeVector3Equal(Axis, VeVectorZero()));
	VE_ASSERT(!VeVector3IsInfinite(Axis));
	VE_VECTOR Normal = VeVector3Normalize(Axis);
	return VeMatrixRotationNormal(Normal, Angle);
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixRotationQuaternion(
	VE_FVECTOR Quaternion) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	static const VE_VECTORF32 Constant1110 = { 1.0f, 1.0f, 1.0f, 0.0f };
	VE_VECTOR Q0 = VeVectorAdd(Quaternion, Quaternion);
	VE_VECTOR Q1 = VeVectorMultiply(Quaternion, Q0);
	VE_VECTOR V0 = VeVectorPermute<VE_PERMUTE_0Y, VE_PERMUTE_0X, VE_PERMUTE_0X, VE_PERMUTE_1W>(Q1, Constant1110.v);
	VE_VECTOR V1 = VeVectorPermute<VE_PERMUTE_0Z, VE_PERMUTE_0Z, VE_PERMUTE_0Y, VE_PERMUTE_1W>(Q1, Constant1110.v);
	VE_VECTOR R0 = VeVectorSubtract(Constant1110, V0);
	R0 = VeVectorSubtract(R0, V1);
	V0 = VeVectorSwizzle<VE_SWIZZLE_X, VE_SWIZZLE_X, VE_SWIZZLE_Y, VE_SWIZZLE_W>(Quaternion);
	V1 = VeVectorSwizzle<VE_SWIZZLE_Z, VE_SWIZZLE_Y, VE_SWIZZLE_Z, VE_SWIZZLE_W>(Q0);
	V0 = VeVectorMultiply(V0, V1);
	V1 = VeVectorSplatW(Quaternion);
	VE_VECTOR V2 = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_Z, VE_SWIZZLE_X, VE_SWIZZLE_W>(Q0);
	V1 = VeVectorMultiply(V1, V2);
	VE_VECTOR R1 = VeVectorAdd(V0, V1);
	VE_VECTOR R2 = VeVectorSubtract(V0, V1);
	V0 = VeVectorPermute<VE_PERMUTE_0Y, VE_PERMUTE_1X, VE_PERMUTE_1Y, VE_PERMUTE_0Z>(R1, R2);
	V1 = VeVectorPermute<VE_PERMUTE_0X, VE_PERMUTE_1Z, VE_PERMUTE_0X, VE_PERMUTE_1Z>(R1, R2);
	VE_MATRIX M;
	M.r[0] = VeVectorPermute<VE_PERMUTE_0X, VE_PERMUTE_1X, VE_PERMUTE_1Y, VE_PERMUTE_0W>(R0, V0);
	M.r[1] = VeVectorPermute<VE_PERMUTE_1Z, VE_PERMUTE_0Y, VE_PERMUTE_1W, VE_PERMUTE_0W>(R0, V0);
	M.r[2] = VeVectorPermute<VE_PERMUTE_1X, VE_PERMUTE_1Y, VE_PERMUTE_0Z, VE_PERMUTE_0W>(R0, V1);
	M.r[3] = g_MathIdentityR3.v;
	return M;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 Constant1110 = { 1.0f, 1.0f, 1.0f, 0.0f };
	VE_VECTOR Q0 = _mm_add_ps(Quaternion, Quaternion);
	VE_VECTOR Q1 = _mm_mul_ps(Quaternion, Q0);
	VE_VECTOR V0 = VE_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 0, 0, 1));
	V0 = _mm_and_ps(V0, g_MathMask3);
	VE_VECTOR V1 = VE_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 1, 2, 2));
	V1 = _mm_and_ps(V1, g_MathMask3);
	VE_VECTOR R0 = _mm_sub_ps(Constant1110, V0);
	R0 = _mm_sub_ps(R0, V1);
	V0 = VE_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 1, 0, 0));
	V1 = VE_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 2, 1, 2));
	V0 = _mm_mul_ps(V0, V1);
	V1 = VE_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 3, 3, 3));
	VE_VECTOR V2 = VE_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 0, 2, 1));
	V1 = _mm_mul_ps(V1, V2);
	VE_VECTOR R1 = _mm_add_ps(V0, V1);
	VE_VECTOR R2 = _mm_sub_ps(V0, V1);
	V0 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(1, 0, 2, 1));
	V0 = VE_PERMUTE_PS(V0, _MM_SHUFFLE(1, 3, 2, 0));
	V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 2, 0, 0));
	V1 = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 0, 2, 0));
	Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(1, 0, 3, 0));
	Q1 = VE_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 2, 0));
	VE_MATRIX M;
	M.r[0] = Q1;
	Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(3, 2, 3, 1));
	Q1 = VE_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 0, 2));
	M.r[1] = Q1;
	Q1 = _mm_shuffle_ps(V1, R0, _MM_SHUFFLE(3, 2, 1, 0));
	M.r[2] = Q1;
	M.r[3] = g_MathIdentityR3;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixTransformation2D(
	VE_FVECTOR ScalingOrigin, VeFloat32 ScalingOrientation,
	VE_FVECTOR Scaling, VE_FVECTOR RotationOrigin, VeFloat32 Rotation,
	VE_GVECTOR Translation) noexcept
{
	VE_VECTOR VScalingOrigin = VeVectorSelect(g_MathSelect1100.v, ScalingOrigin, g_MathSelect1100.v);
	VE_VECTOR NegScalingOrigin = VeVectorNegate(VScalingOrigin);
	VE_MATRIX MScalingOriginI = VeMatrixTranslationFromVector(NegScalingOrigin);
	VE_MATRIX MScalingOrientation = VeMatrixRotationZ(ScalingOrientation);
	VE_MATRIX MScalingOrientationT = VeMatrixTranspose(MScalingOrientation);
	VE_VECTOR VScaling = VeVectorSelect(g_MathOne.v, Scaling, g_MathSelect1100.v);
	VE_MATRIX MScaling = VeMatrixScalingFromVector(VScaling);
	VE_VECTOR VRotationOrigin = VeVectorSelect(g_MathSelect1100.v, RotationOrigin, g_MathSelect1100.v);
	VE_MATRIX MRotation = VeMatrixRotationZ(Rotation);
	VE_VECTOR VTranslation = VeVectorSelect(g_MathSelect1100.v, Translation, g_MathSelect1100.v);
	VE_MATRIX M = VeMatrixMultiply(MScalingOriginI, MScalingOrientationT);
	M = VeMatrixMultiply(M, MScaling);
	M = VeMatrixMultiply(M, MScalingOrientation);
	M.r[3] = VeVectorAdd(M.r[3], VScalingOrigin);
	M.r[3] = VeVectorSubtract(M.r[3], VRotationOrigin);
	M = VeMatrixMultiply(M, MRotation);
	M.r[3] = VeVectorAdd(M.r[3], VRotationOrigin);
	M.r[3] = VeVectorAdd(M.r[3], VTranslation);
	return M;
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixTransformation(
	VE_FVECTOR ScalingOrigin, VE_FVECTOR ScalingOrientationQuaternion,
	VE_FVECTOR Scaling, VE_GVECTOR RotationOrigin,
	VE_HVECTOR RotationQuaternion, VE_HVECTOR Translation) noexcept
{
	VE_VECTOR VScalingOrigin = VeVectorSelect(g_MathSelect1110.v, ScalingOrigin, g_MathSelect1110.v);
	VE_VECTOR NegScalingOrigin = VeVectorNegate(ScalingOrigin);
	VE_MATRIX MScalingOriginI = VeMatrixTranslationFromVector(NegScalingOrigin);
	VE_MATRIX MScalingOrientation = VeMatrixRotationQuaternion(ScalingOrientationQuaternion);
	VE_MATRIX MScalingOrientationT = VeMatrixTranspose(MScalingOrientation);
	VE_MATRIX MScaling = VeMatrixScalingFromVector(Scaling);
	VE_VECTOR VRotationOrigin = VeVectorSelect(g_MathSelect1110.v, RotationOrigin, g_MathSelect1110.v);
	VE_MATRIX MRotation = VeMatrixRotationQuaternion(RotationQuaternion);
	VE_VECTOR VTranslation = VeVectorSelect(g_MathSelect1110.v, Translation, g_MathSelect1110.v);
	VE_MATRIX M;
	M = VeMatrixMultiply(MScalingOriginI, MScalingOrientationT);
	M = VeMatrixMultiply(M, MScaling);
	M = VeMatrixMultiply(M, MScalingOrientation);
	M.r[3] = VeVectorAdd(M.r[3], VScalingOrigin);
	M.r[3] = VeVectorSubtract(M.r[3], VRotationOrigin);
	M = VeMatrixMultiply(M, MRotation);
	M.r[3] = VeVectorAdd(M.r[3], VRotationOrigin);
	M.r[3] = VeVectorAdd(M.r[3], VTranslation);
	return M;
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixAffineTransformation2D(
	VE_FVECTOR Scaling, VE_FVECTOR RotationOrigin, VeFloat32 Rotation,
	VE_FVECTOR Translation) noexcept
{
	VE_VECTOR VScaling = VeVectorSelect(g_MathOne.v, Scaling, g_MathSelect1100.v);
	VE_MATRIX MScaling = VeMatrixScalingFromVector(VScaling);
	VE_VECTOR VRotationOrigin = VeVectorSelect(g_MathSelect1100.v, RotationOrigin, g_MathSelect1100.v);
	VE_MATRIX MRotation = VeMatrixRotationZ(Rotation);
	VE_VECTOR VTranslation = VeVectorSelect(g_MathSelect1100.v, Translation, g_MathSelect1100.v);
	VE_MATRIX M;
	M = MScaling;
	M.r[3] = VeVectorSubtract(M.r[3], VRotationOrigin);
	M = VeMatrixMultiply(M, MRotation);
	M.r[3] = VeVectorAdd(M.r[3], VRotationOrigin);
	M.r[3] = VeVectorAdd(M.r[3], VTranslation);
	return M;
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixAffineTransformation(
	VE_FVECTOR Scaling, VE_FVECTOR RotationOrigin,
	VE_FVECTOR RotationQuaternion, VE_GVECTOR Translation) noexcept
{
	VE_MATRIX MScaling = VeMatrixScalingFromVector(Scaling);
	VE_VECTOR VRotationOrigin = VeVectorSelect(g_MathSelect1110.v, RotationOrigin, g_MathSelect1110.v);
	VE_MATRIX MRotation = VeMatrixRotationQuaternion(RotationQuaternion);
	VE_VECTOR VTranslation = VeVectorSelect(g_MathSelect1110.v, Translation, g_MathSelect1110.v);
	VE_MATRIX M;
	M = MScaling;
	M.r[3] = VeVectorSubtract(M.r[3], VRotationOrigin);
	M = VeMatrixMultiply(M, MRotation);
	M.r[3] = VeVectorAdd(M.r[3], VRotationOrigin);
	M.r[3] = VeVectorAdd(M.r[3], VTranslation);
	return M;
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VePlaneIsInfinite(VE_FVECTOR P) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VePlaneNormalize(VE_FVECTOR P) noexcept;
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixReflect(
	VE_FVECTOR ReflectionPlane) noexcept
{
	VE_ASSERT(!VeVector3Equal(ReflectionPlane, VeVectorZero()));
	VE_ASSERT(!VePlaneIsInfinite(ReflectionPlane));
	static const VE_VECTORF32 NegativeTwo = { -2.0f, -2.0f, -2.0f, 0.0f };
	VE_VECTOR P = VePlaneNormalize(ReflectionPlane);
	VE_VECTOR S = VeVectorMultiply(P, NegativeTwo);
	VE_VECTOR A = VeVectorSplatX(P);
	VE_VECTOR B = VeVectorSplatY(P);
	VE_VECTOR C = VeVectorSplatZ(P);
	VE_VECTOR D = VeVectorSplatW(P);
	VE_MATRIX M;
	M.r[0] = VeVectorMultiplyAdd(A, S, g_MathIdentityR0.v);
	M.r[1] = VeVectorMultiplyAdd(B, S, g_MathIdentityR1.v);
	M.r[2] = VeVectorMultiplyAdd(C, S, g_MathIdentityR2.v);
	M.r[3] = VeVectorMultiplyAdd(D, S, g_MathIdentityR3.v);
	return M;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VePlaneDot(
	VE_FVECTOR P, VE_FVECTOR V) noexcept;
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixShadow(
	VE_FVECTOR ShadowPlane, VE_FVECTOR LightPosition) noexcept
{
	static const VE_VECTORU32 Select0001 = { VE_SELECT_0, VE_SELECT_0, VE_SELECT_0, VE_SELECT_1 };
	VE_ASSERT(!VeVector3Equal(ShadowPlane, VeVectorZero()));
	VE_ASSERT(!VePlaneIsInfinite(ShadowPlane));
	VE_VECTOR P = VePlaneNormalize(ShadowPlane);
	VE_VECTOR Dot = VePlaneDot(P, LightPosition);
	P = VeVectorNegate(P);
	VE_VECTOR D = VeVectorSplatW(P);
	VE_VECTOR C = VeVectorSplatZ(P);
	VE_VECTOR B = VeVectorSplatY(P);
	VE_VECTOR A = VeVectorSplatX(P);
	Dot = VeVectorSelect(Select0001.v, Dot, Select0001.v);
	VE_MATRIX M;
	M.r[3] = VeVectorMultiplyAdd(D, LightPosition, Dot);
	Dot = VeVectorRotateLeft(Dot, 1);
	M.r[2] = VeVectorMultiplyAdd(C, LightPosition, Dot);
	Dot = VeVectorRotateLeft(Dot, 1);
	M.r[1] = VeVectorMultiplyAdd(B, LightPosition, Dot);
	Dot = VeVectorRotateLeft(Dot, 1);
	M.r[0] = VeVectorMultiplyAdd(A, LightPosition, Dot);
	return M;
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixLookToLH(VE_FVECTOR EyePosition,
	VE_FVECTOR EyeDirection, VE_FVECTOR UpDirection) noexcept;
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixLookAtLH(VE_FVECTOR EyePosition,
	VE_FVECTOR FocusPosition, VE_FVECTOR UpDirection) noexcept
{
	VE_VECTOR EyeDirection = VeVectorSubtract(FocusPosition, EyePosition);
	return VeMatrixLookToLH(EyePosition, EyeDirection, UpDirection);
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixLookAtRH(VE_FVECTOR EyePosition,
	VE_FVECTOR FocusPosition, VE_FVECTOR UpDirection) noexcept
{
	VE_VECTOR NegEyeDirection = VeVectorSubtract(EyePosition, FocusPosition);
	return VeMatrixLookToLH(EyePosition, NegEyeDirection, UpDirection);
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixLookToLH(VE_FVECTOR EyePosition,
	VE_FVECTOR EyeDirection, VE_FVECTOR UpDirection) noexcept
{
	VE_ASSERT(!VeVector3Equal(EyeDirection, VeVectorZero()));
	VE_ASSERT(!VeVector3IsInfinite(EyeDirection));
	VE_ASSERT(!VeVector3Equal(UpDirection, VeVectorZero()));
	VE_ASSERT(!VeVector3IsInfinite(UpDirection));
	VE_VECTOR R2 = VeVector3Normalize(EyeDirection);
	VE_VECTOR R0 = VeVector3Cross(UpDirection, R2);
	R0 = VeVector3Normalize(R0);
	VE_VECTOR R1 = VeVector3Cross(R2, R0);
	VE_VECTOR NegEyePosition = VeVectorNegate(EyePosition);
	VE_VECTOR D0 = VeVector3Dot(R0, NegEyePosition);
	VE_VECTOR D1 = VeVector3Dot(R1, NegEyePosition);
	VE_VECTOR D2 = VeVector3Dot(R2, NegEyePosition);
	VE_MATRIX M;
	M.r[0] = VeVectorSelect(D0, R0, g_MathSelect1110.v);
	M.r[1] = VeVectorSelect(D1, R1, g_MathSelect1110.v);
	M.r[2] = VeVectorSelect(D2, R2, g_MathSelect1110.v);
	M.r[3] = g_MathIdentityR3.v;
	M = VeMatrixTranspose(M);
	return M;
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixLookToRH(VE_FVECTOR EyePosition,
	VE_FVECTOR EyeDirection, VE_FVECTOR UpDirection) noexcept
{
	VE_VECTOR NegEyeDirection = VeVectorNegate(EyeDirection);
	return VeMatrixLookToLH(EyePosition, NegEyeDirection, UpDirection);
}
//--------------------------------------------------------------------------
inline bool VeScalarNearEqual(
	VeFloat32 S1, VeFloat32 S2, VeFloat32 Epsilon) noexcept;
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixPerspectiveLH(VeFloat32 ViewWidth,
	VeFloat32 ViewHeight, VeFloat32 NearZ, VeFloat32 FarZ) noexcept
{
	VE_ASSERT(!VeScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(FarZ, NearZ, 0.00001f));
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 TwoNearZ = NearZ + NearZ;
	VeFloat32 fRange = FarZ / (FarZ - NearZ);
	VE_MATRIX M;
	M.m[0][0] = TwoNearZ / ViewWidth;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = TwoNearZ / ViewHeight;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = fRange;
	M.m[2][3] = 1.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = -fRange * NearZ;
	M.m[3][3] = 0.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	VeFloat32 TwoNearZ = NearZ + NearZ;
	VeFloat32 fRange = FarZ / (FarZ - NearZ);
	VE_VECTOR rMem =
	{
		TwoNearZ / ViewWidth,
		TwoNearZ / ViewHeight,
		fRange,
		-fRange * NearZ
	};
	VE_VECTOR vValues = rMem;
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_move_ss(vTemp, vValues);
	M.r[0] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskY);
	M.r[1] = vTemp;
	vValues = _mm_shuffle_ps(vValues, g_MathIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
	vTemp = _mm_setzero_ps();
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
	M.r[2] = vTemp;
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
	M.r[3] = vTemp;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixPerspectiveRH(VeFloat32 ViewWidth,
	VeFloat32 ViewHeight, VeFloat32 NearZ, VeFloat32 FarZ) noexcept
{
	VE_ASSERT(!VeScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(FarZ, NearZ, 0.00001f));
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 TwoNearZ = NearZ + NearZ;
	VeFloat32 fRange = FarZ / (NearZ - FarZ);
	VE_MATRIX M;
	M.m[0][0] = TwoNearZ / ViewWidth;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = TwoNearZ / ViewHeight;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = fRange;
	M.m[2][3] = -1.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = fRange * NearZ;
	M.m[3][3] = 0.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	VeFloat32 TwoNearZ = NearZ + NearZ;
	VeFloat32 fRange = FarZ / (NearZ - FarZ);
	VE_VECTOR rMem = {
		TwoNearZ / ViewWidth,
		TwoNearZ / ViewHeight,
		fRange,
		fRange * NearZ
	};
	VE_VECTOR vValues = rMem;
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_move_ss(vTemp, vValues);
	M.r[0] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskY);
	M.r[1] = vTemp;
	vValues = _mm_shuffle_ps(vValues, g_MathNegIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
	vTemp = _mm_setzero_ps();
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
	M.r[2] = vTemp;
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
	M.r[3] = vTemp;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixPerspectiveFovLH(
	VeFloat32 FovAngleY, VeFloat32 AspectHByW, VeFloat32 NearZ,
	VeFloat32 FarZ) noexcept
{
	VE_ASSERT(!VeScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
	VE_ASSERT(!VeScalarNearEqual(AspectHByW, 0.0f, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(FarZ, NearZ, 0.00001f));
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 SinFov;
	VeFloat32 CosFov;
	VeScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);
	VeFloat32 Height = CosFov / SinFov;
	VeFloat32 Width = Height / AspectHByW;
	VeFloat32 fRange = FarZ / (FarZ - NearZ);
	VE_MATRIX M;
	M.m[0][0] = Width;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = Height;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = fRange;
	M.m[2][3] = 1.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = -fRange * NearZ;
	M.m[3][3] = 0.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VeFloat32 SinFov;
	VeFloat32 CosFov;
	VeScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);
	VeFloat32 fRange = FarZ / (FarZ - NearZ);
	VeFloat32 Height = CosFov / SinFov;
	VE_VECTOR rMem =
	{
		Height / AspectHByW,
		Height,
		fRange,
		-fRange * NearZ
	};
	VE_VECTOR vValues = rMem;
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_move_ss(vTemp, vValues);
	VE_MATRIX M;
	M.r[0] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskY);
	M.r[1] = vTemp;
	vTemp = _mm_setzero_ps();
	vValues = _mm_shuffle_ps(vValues, g_MathIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
	M.r[2] = vTemp;
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
	M.r[3] = vTemp;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixPerspectiveFovRH(
	VeFloat32 FovAngleY, VeFloat32 AspectHByW,
	VeFloat32 NearZ, VeFloat32 FarZ) noexcept
{
	VE_ASSERT(!VeScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
	VE_ASSERT(!VeScalarNearEqual(AspectHByW, 0.0f, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(FarZ, NearZ, 0.00001f));
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 SinFov;
	VeFloat32 CosFov;
	VeScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);
	VeFloat32 Height = CosFov / SinFov;
	VeFloat32 Width = Height / AspectHByW;
	VeFloat32 fRange = FarZ / (NearZ - FarZ);
	VE_MATRIX M;
	M.m[0][0] = Width;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = Height;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = fRange;
	M.m[2][3] = -1.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = fRange * NearZ;
	M.m[3][3] = 0.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VeFloat32 SinFov;
	VeFloat32 CosFov;
	VeScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);
	VeFloat32 fRange = FarZ / (NearZ - FarZ);
	VeFloat32 Height = CosFov / SinFov;
	VE_VECTOR rMem =
	{
		Height / AspectHByW,
		Height,
		fRange,
		fRange * NearZ
	};
	VE_VECTOR vValues = rMem;
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_move_ss(vTemp, vValues);
	VE_MATRIX M;
	M.r[0] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskY);
	M.r[1] = vTemp;
	vTemp = _mm_setzero_ps();
	vValues = _mm_shuffle_ps(vValues, g_MathNegIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
	M.r[2] = vTemp;
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
	M.r[3] = vTemp;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixPerspectiveOffCenterLH(
	VeFloat32 ViewLeft, VeFloat32 ViewRight, VeFloat32 ViewBottom,
	VeFloat32 ViewTop, VeFloat32 NearZ, VeFloat32 FarZ) noexcept
{
	VE_ASSERT(!VeScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(FarZ, NearZ, 0.00001f));
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 TwoNearZ = NearZ + NearZ;
	VeFloat32 ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
	VeFloat32 ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
	VeFloat32 fRange = FarZ / (FarZ - NearZ);
	VE_MATRIX M;
	M.m[0][0] = TwoNearZ * ReciprocalWidth;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = TwoNearZ * ReciprocalHeight;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
	M.m[2][1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
	M.m[2][2] = fRange;
	M.m[2][3] = 1.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = -fRange * NearZ;
	M.m[3][3] = 0.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	VeFloat32 TwoNearZ = NearZ + NearZ;
	VeFloat32 ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
	VeFloat32 ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
	VeFloat32 fRange = FarZ / (FarZ - NearZ);
	VE_VECTOR rMem =
	{
		TwoNearZ*ReciprocalWidth,
		TwoNearZ*ReciprocalHeight,
		-fRange * NearZ,
		0
	};
	VE_VECTOR vValues = rMem;
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_move_ss(vTemp, vValues);
	M.r[0] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskY);
	M.r[1] = vTemp;
	M.r[2] = VeVectorSet(-(ViewLeft + ViewRight) * ReciprocalWidth,
		-(ViewTop + ViewBottom) * ReciprocalHeight, fRange, 1.0f);
	vValues = _mm_and_ps(vValues, g_MathMaskZ);
	M.r[3] = vValues;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixPerspectiveOffCenterRH(
	VeFloat32 ViewLeft, VeFloat32 ViewRight, VeFloat32 ViewBottom,
	VeFloat32 ViewTop, VeFloat32 NearZ, VeFloat32 FarZ) noexcept
{
	VE_ASSERT(!VeScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(FarZ, NearZ, 0.00001f));
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 TwoNearZ = NearZ + NearZ;
	VeFloat32 ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
	VeFloat32 ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
	VeFloat32 fRange = FarZ / (NearZ - FarZ);
	VE_MATRIX M;
	M.m[0][0] = TwoNearZ * ReciprocalWidth;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = TwoNearZ * ReciprocalHeight;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = (ViewLeft + ViewRight) * ReciprocalWidth;
	M.m[2][1] = (ViewTop + ViewBottom) * ReciprocalHeight;
	M.m[2][2] = fRange;
	M.m[2][3] = -1.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = fRange * NearZ;
	M.m[3][3] = 0.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	VeFloat32 TwoNearZ = NearZ + NearZ;
	VeFloat32 ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
	VeFloat32 ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
	VeFloat32 fRange = FarZ / (NearZ - FarZ);
	VE_VECTOR rMem =
	{
		TwoNearZ*ReciprocalWidth,
		TwoNearZ*ReciprocalHeight,
		fRange * NearZ,
		0
	};
	VE_VECTOR vValues = rMem;
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_move_ss(vTemp, vValues);
	M.r[0] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskY);
	M.r[1] = vTemp;
	M.r[2] = VeVectorSet((ViewLeft + ViewRight) * ReciprocalWidth,
		(ViewTop + ViewBottom) * ReciprocalHeight, fRange, -1.0f);
	vValues = _mm_and_ps(vValues, g_MathMaskZ);
	M.r[3] = vValues;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixOrthographicLH(
	VeFloat32 ViewWidth, VeFloat32 ViewHeight, VeFloat32 NearZ,
	VeFloat32 FarZ) noexcept
{
	VE_ASSERT(!VeScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(FarZ, NearZ, 0.00001f));
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fRange = 1.0f / (FarZ - NearZ);
	VE_MATRIX M;
	M.m[0][0] = 2.0f / ViewWidth;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = 2.0f / ViewHeight;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = fRange;
	M.m[2][3] = 0.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = -fRange * NearZ;
	M.m[3][3] = 1.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	VeFloat32 fRange = 1.0f / (FarZ - NearZ);
	VE_VECTOR rMem =
	{
		2.0f / ViewWidth,
		2.0f / ViewHeight,
		fRange,
		-fRange * NearZ
	};
	VE_VECTOR vValues = rMem;
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_move_ss(vTemp, vValues);
	M.r[0] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskY);
	M.r[1] = vTemp;
	vTemp = _mm_setzero_ps();
	vValues = _mm_shuffle_ps(vValues, g_MathIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
	M.r[2] = vTemp;
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
	M.r[3] = vTemp;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixOrthographicRH(
	VeFloat32 ViewWidth, VeFloat32 ViewHeight,
	VeFloat32 NearZ, VeFloat32 FarZ) noexcept
{
	VE_ASSERT(!VeScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(FarZ, NearZ, 0.00001f));
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fRange = 1.0f / (NearZ - FarZ);
	VE_MATRIX M;
	M.m[0][0] = 2.0f / ViewWidth;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = 2.0f / ViewHeight;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = fRange;
	M.m[2][3] = 0.0f;
	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = fRange * NearZ;
	M.m[3][3] = 1.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	VeFloat32 fRange = 1.0f / (NearZ - FarZ);
	VE_VECTOR rMem =
	{
		2.0f / ViewWidth,
		2.0f / ViewHeight,
		fRange,
		fRange * NearZ
	};
	VE_VECTOR vValues = rMem;
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_move_ss(vTemp, vValues);
	M.r[0] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskY);
	M.r[1] = vTemp;
	vTemp = _mm_setzero_ps();
	vValues = _mm_shuffle_ps(vValues, g_MathIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
	M.r[2] = vTemp;
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
	M.r[3] = vTemp;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixOrthographicOffCenterLH(
	VeFloat32 ViewLeft, VeFloat32 ViewRight, VeFloat32 ViewBottom,
	VeFloat32 ViewTop, VeFloat32 NearZ, VeFloat32 FarZ) noexcept
{
	VE_ASSERT(!VeScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(FarZ, NearZ, 0.00001f));
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
	VeFloat32 ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
	VeFloat32 fRange = 1.0f / (FarZ - NearZ);
	VE_MATRIX M;
	M.m[0][0] = ReciprocalWidth + ReciprocalWidth;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = ReciprocalHeight + ReciprocalHeight;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = fRange;
	M.m[2][3] = 0.0f;
	M.m[3][0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
	M.m[3][1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
	M.m[3][2] = -fRange * NearZ;
	M.m[3][3] = 1.0f;
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	VeFloat32 fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
	VeFloat32 fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
	VeFloat32 fRange = 1.0f / (FarZ - NearZ);
	VE_VECTOR rMem =
	{
		fReciprocalWidth,
		fReciprocalHeight,
		fRange,
		1.0f
	};
	VE_VECTOR rMem2 =
	{
		-(ViewLeft + ViewRight),
		-(ViewTop + ViewBottom),
		-NearZ,
		1.0f
	};
	VE_VECTOR vValues = rMem;
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_move_ss(vTemp, vValues);
	vTemp = _mm_add_ss(vTemp, vTemp);
	M.r[0] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskY);
	vTemp = _mm_add_ps(vTemp, vTemp);
	M.r[1] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskZ);
	M.r[2] = vTemp;
	vValues = _mm_mul_ps(vValues, rMem2);
	M.r[3] = vValues;
	return M;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixOrthographicOffCenterRH(
	VeFloat32 ViewLeft, VeFloat32 ViewRight, VeFloat32 ViewBottom,
	VeFloat32 ViewTop, VeFloat32 NearZ, VeFloat32 FarZ) noexcept
{
	VE_ASSERT(!VeScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
	VE_ASSERT(!VeScalarNearEqual(FarZ, NearZ, 0.00001f));
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
	VeFloat32 ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
	VeFloat32 fRange = 1.0f / (NearZ - FarZ);
	VE_MATRIX M;
	M.m[0][0] = ReciprocalWidth + ReciprocalWidth;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;
	M.m[1][0] = 0.0f;
	M.m[1][1] = ReciprocalHeight + ReciprocalHeight;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;
	M.m[2][0] = 0.0f;
	M.m[2][1] = 0.0f;
	M.m[2][2] = fRange;
	M.m[2][3] = 0.0f;
	M.r[3] = VeVectorSet(-(ViewLeft + ViewRight) * ReciprocalWidth,
		-(ViewTop + ViewBottom) * ReciprocalHeight, fRange * NearZ, 1.0f);
	return M;
#	elif defined(VE_ENABLE_SSE)
	VE_MATRIX M;
	VeFloat32 fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
	VeFloat32 fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
	VeFloat32 fRange = 1.0f / (NearZ - FarZ);
	VE_VECTOR rMem =
	{
		fReciprocalWidth,
		fReciprocalHeight,
		fRange,
		1.0f
	};
	VE_VECTOR rMem2 =
	{
		-(ViewLeft + ViewRight),
		-(ViewTop + ViewBottom),
		NearZ,
		1.0f
	};
	VE_VECTOR vValues = rMem;
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_move_ss(vTemp, vValues);
	vTemp = _mm_add_ss(vTemp, vTemp);
	M.r[0] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskY);
	vTemp = _mm_add_ps(vTemp, vTemp);
	M.r[1] = vTemp;
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_MathMaskZ);
	M.r[2] = vTemp;
	vValues = _mm_mul_ps(vValues, rMem2);
	M.r[3] = vValues;
	return M;
#	endif
}
//--------------------------------------------------------------------------
