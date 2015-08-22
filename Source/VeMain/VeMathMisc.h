////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMathMisc.h
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeQuaternionEqual(
	VE_FVECTOR Q1, VE_FVECTOR Q2) noexcept
{
	return VeVector4Equal(Q1, Q2);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeQuaternionNotEqual(
	VE_FVECTOR Q1, VE_FVECTOR Q2) noexcept
{
	return VeVector4NotEqual(Q1, Q2);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeQuaternionIsNaN(
	VE_FVECTOR Q) noexcept
{
	return VeVector4IsNaN(Q);
}

//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeQuaternionIsInfinite(
	VE_FVECTOR Q) noexcept
{
	return VeVector4IsInfinite(Q);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeQuaternionIsIdentity(
	VE_FVECTOR Q) noexcept
{
	return VeVector4Equal(Q, g_MathIdentityR3.v);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionDot(
	VE_FVECTOR Q1, VE_FVECTOR Q2) noexcept
{
	return VeVector4Dot(Q1, Q2);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionMultiply(
	VE_FVECTOR Q1, VE_FVECTOR Q2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result =
	{
		(Q2.vector4_f32[3] * Q1.vector4_f32[0]) + (Q2.vector4_f32[0] * Q1.vector4_f32[3]) + (Q2.vector4_f32[1] * Q1.vector4_f32[2]) - (Q2.vector4_f32[2] * Q1.vector4_f32[1]),
		(Q2.vector4_f32[3] * Q1.vector4_f32[1]) - (Q2.vector4_f32[0] * Q1.vector4_f32[2]) + (Q2.vector4_f32[1] * Q1.vector4_f32[3]) + (Q2.vector4_f32[2] * Q1.vector4_f32[0]),
		(Q2.vector4_f32[3] * Q1.vector4_f32[2]) + (Q2.vector4_f32[0] * Q1.vector4_f32[1]) - (Q2.vector4_f32[1] * Q1.vector4_f32[0]) + (Q2.vector4_f32[2] * Q1.vector4_f32[3]),
		(Q2.vector4_f32[3] * Q1.vector4_f32[3]) - (Q2.vector4_f32[0] * Q1.vector4_f32[0]) - (Q2.vector4_f32[1] * Q1.vector4_f32[1]) - (Q2.vector4_f32[2] * Q1.vector4_f32[2])
	};
	return Result;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 ControlWZYX = { 1.0f,-1.0f, 1.0f,-1.0f };
	static const VE_VECTORF32 ControlZWXY = { 1.0f, 1.0f,-1.0f,-1.0f };
	static const VE_VECTORF32 ControlYXWZ = { -1.0f, 1.0f, 1.0f,-1.0f };
	VE_VECTOR Q2X = Q2;
	VE_VECTOR Q2Y = Q2;
	VE_VECTOR Q2Z = Q2;
	VE_VECTOR vResult = Q2;
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 3, 3, 3));
	Q2X = VE_PERMUTE_PS(Q2X, _MM_SHUFFLE(0, 0, 0, 0));
	Q2Y = VE_PERMUTE_PS(Q2Y, _MM_SHUFFLE(1, 1, 1, 1));
	Q2Z = VE_PERMUTE_PS(Q2Z, _MM_SHUFFLE(2, 2, 2, 2));
	vResult = _mm_mul_ps(vResult, Q1);
	VE_VECTOR Q1Shuffle = Q1;
	Q1Shuffle = VE_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
	Q2X = _mm_mul_ps(Q2X, Q1Shuffle);
	Q1Shuffle = VE_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(2, 3, 0, 1));
	Q2X = _mm_mul_ps(Q2X, ControlWZYX);
	Q2Y = _mm_mul_ps(Q2Y, Q1Shuffle);
	Q1Shuffle = VE_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
	Q2Y = _mm_mul_ps(Q2Y, ControlZWXY);
	Q2Z = _mm_mul_ps(Q2Z, Q1Shuffle);
	vResult = _mm_add_ps(vResult, Q2X);
	Q2Z = _mm_mul_ps(Q2Z, ControlYXWZ);
	Q2Y = _mm_add_ps(Q2Y, Q2Z);
	vResult = _mm_add_ps(vResult, Q2Y);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionLengthSq(
	VE_FVECTOR Q) noexcept
{
	return VeVector4LengthSq(Q);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionReciprocalLength(
	VE_FVECTOR Q) noexcept
{
	return VeVector4ReciprocalLength(Q);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionLength(
	VE_FVECTOR Q) noexcept
{
	return VeVector4Length(Q);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionNormalizeEst(
	VE_FVECTOR Q) noexcept
{
	return VeVector4NormalizeEst(Q);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionNormalize(
	VE_FVECTOR Q) noexcept
{
	return VeVector4Normalize(Q);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionConjugate(
	VE_FVECTOR Q) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result = {
		-Q.vector4_f32[0],
		-Q.vector4_f32[1],
		-Q.vector4_f32[2],
		Q.vector4_f32[3]
	};
	return Result;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 NegativeOne3 = { -1.0f,-1.0f,-1.0f,1.0f };
	return _mm_mul_ps(Q, NegativeOne3);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionInverse(
	VE_FVECTOR Q) noexcept
{
	const VE_VECTOR  Zero = VeVectorZero();
	VE_VECTOR L = VeVector4LengthSq(Q);
	VE_VECTOR Conjugate = VeQuaternionConjugate(Q);
	VE_VECTOR Control = VeVectorLessOrEqual(L, g_MathEpsilon.v);
	VE_VECTOR Result = VeVectorDivide(Conjugate, L);
	Result = VeVectorSelect(Result, Zero, Control);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionLn(
	VE_FVECTOR Q) noexcept
{
	static const VE_VECTORF32 OneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };
	VE_VECTOR QW = VeVectorSplatW(Q);
	VE_VECTOR Q0 = VeVectorSelect(g_MathSelect1110.v, Q, g_MathSelect1110.v);
	VE_VECTOR ControlW = VeVectorInBounds(QW, OneMinusEpsilon.v);
	VE_VECTOR Theta = VeVectorACos(QW);
	VE_VECTOR SinTheta = VeVectorSin(Theta);
	VE_VECTOR S = VeVectorDivide(Theta, SinTheta);
	VE_VECTOR Result = VeVectorMultiply(Q0, S);
	Result = VeVectorSelect(Q0, Result, ControlW);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionExp(
	VE_FVECTOR Q) noexcept
{
	VE_VECTOR Theta = VeVector3Length(Q);
	VE_VECTOR SinTheta, CosTheta;
	VeVectorSinCos(&SinTheta, &CosTheta, Theta);
	VE_VECTOR S = VeVectorDivide(SinTheta, Theta);
	VE_VECTOR Result = VeVectorMultiply(Q, S);
	const VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR Control = VeVectorNearEqual(Theta, Zero, g_MathEpsilon.v);
	Result = VeVectorSelect(Result, Q, Control);
	Result = VeVectorSelect(CosTheta, Result, g_MathSelect1110.v);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionSlerpV(
	VE_FVECTOR Q0, VE_FVECTOR Q1, VE_FVECTOR T) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionSlerp(
	VE_FVECTOR Q0, VE_FVECTOR Q1, VeFloat32 t) noexcept
{
	VE_VECTOR T = VeVectorReplicate(t);
	return VeQuaternionSlerpV(Q0, Q1, T);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionSlerpV(
	VE_FVECTOR Q0, VE_FVECTOR Q1, VE_FVECTOR T) noexcept
{
	VE_ASSERT((VeVectorGetY(T) == VeVectorGetX(T))
		&& (VeVectorGetZ(T) == VeVectorGetX(T))
		&& (VeVectorGetW(T) == VeVectorGetX(T)));
#	if defined(VE_NO_INTRINSICS)
	const VE_VECTORF32 OneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };
	VE_VECTOR CosOmega = VeQuaternionDot(Q0, Q1);
	const VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR Control = VeVectorLess(CosOmega, Zero);
	VE_VECTOR Sign = VeVectorSelect(g_MathOne.v, g_MathNegativeOne.v, Control);
	CosOmega = VeVectorMultiply(CosOmega, Sign);
	Control = VeVectorLess(CosOmega, OneMinusEpsilon);
	VE_VECTOR SinOmega = VeVectorNegativeMultiplySubtract(CosOmega, CosOmega, g_MathOne.v);
	SinOmega = VeVectorSqrt(SinOmega);
	VE_VECTOR Omega = VeVectorATan2(SinOmega, CosOmega);
	VE_VECTOR SignMask = VeVectorSplatSignMask();
	VE_VECTOR V01 = VeVectorShiftLeft(T, Zero, 2);
	SignMask = VeVectorShiftLeft(SignMask, Zero, 3);
	V01 = VeVectorXorInt(V01, SignMask);
	V01 = VeVectorAdd(g_MathIdentityR0.v, V01);
	VE_VECTOR InvSinOmega = VeVectorReciprocal(SinOmega);
	VE_VECTOR S0 = VeVectorMultiply(V01, Omega);
	S0 = VeVectorSin(S0);
	S0 = VeVectorMultiply(S0, InvSinOmega);
	S0 = VeVectorSelect(V01, S0, Control);
	VE_VECTOR S1 = VeVectorSplatY(S0);
	S0 = VeVectorSplatX(S0);
	S1 = VeVectorMultiply(S1, Sign);
	VE_VECTOR Result = VeVectorMultiply(Q0, S0);
	Result = VeVectorMultiplyAdd(Q1, S1, Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 OneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };
	static const VE_VECTORU32 SignMask2 = { 0x80000000,0x00000000,0x00000000,0x00000000 };
	static const VE_VECTORU32 MaskXY = { 0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000 };
	VE_VECTOR CosOmega = VeQuaternionDot(Q0, Q1);
	const VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR Control = VeVectorLess(CosOmega, Zero);
	VE_VECTOR Sign = VeVectorSelect(g_MathOne, g_MathNegativeOne, Control);
	CosOmega = _mm_mul_ps(CosOmega, Sign);
	Control = VeVectorLess(CosOmega, OneMinusEpsilon);
	VE_VECTOR SinOmega = _mm_mul_ps(CosOmega, CosOmega);
	SinOmega = _mm_sub_ps(g_MathOne, SinOmega);
	SinOmega = _mm_sqrt_ps(SinOmega);
	VE_VECTOR Omega = VeVectorATan2(SinOmega, CosOmega);
	VE_VECTOR V01 = VE_PERMUTE_PS(T, _MM_SHUFFLE(2, 3, 0, 1));
	V01 = _mm_and_ps(V01, MaskXY);
	V01 = _mm_xor_ps(V01, SignMask2);
	V01 = _mm_add_ps(g_MathIdentityR0, V01);
	VE_VECTOR S0 = _mm_mul_ps(V01, Omega);
	S0 = VeVectorSin(S0);
	S0 = _mm_div_ps(S0, SinOmega);
	S0 = VeVectorSelect(V01, S0, Control);
	VE_VECTOR S1 = VeVectorSplatY(S0);
	S0 = VeVectorSplatX(S0);
	S1 = _mm_mul_ps(S1, Sign);
	VE_VECTOR Result = _mm_mul_ps(Q0, S0);
	S1 = _mm_mul_ps(S1, Q1);
	Result = _mm_add_ps(Result, S1);
	return Result;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionSquadV(VE_FVECTOR Q0,
	VE_FVECTOR Q1, VE_FVECTOR Q2, VE_GVECTOR Q3, VE_HVECTOR T) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionSquad(VE_FVECTOR Q0,
	VE_FVECTOR Q1, VE_FVECTOR Q2, VE_GVECTOR Q3, VeFloat32 t) noexcept
{
	VE_VECTOR T = VeVectorReplicate(t);
	return VeQuaternionSquadV(Q0, Q1, Q2, Q3, T);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionSquadV(VE_FVECTOR Q0,
	VE_FVECTOR Q1, VE_FVECTOR Q2, VE_GVECTOR Q3, VE_HVECTOR T) noexcept
{
	VE_ASSERT((VeVectorGetY(T) == VeVectorGetX(T))
		&& (VeVectorGetZ(T) == VeVectorGetX(T))
		&& (VeVectorGetW(T) == VeVectorGetX(T)));
	VE_VECTOR TP = T;
	const VE_VECTOR Two = VeVectorSplatConstant(2, 0);
	VE_VECTOR Q03 = VeQuaternionSlerpV(Q0, Q3, T);
	VE_VECTOR Q12 = VeQuaternionSlerpV(Q1, Q2, T);
	TP = VeVectorNegativeMultiplySubtract(TP, TP, TP);
	TP = VeVectorMultiply(TP, Two);
	VE_VECTOR Result = VeQuaternionSlerpV(Q03, Q12, TP);
	return Result;
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeQuaternionSquadSetup(
	VE_VECTOR* pA, VE_VECTOR* pB, VE_VECTOR* pC, VE_FVECTOR  Q0,
	VE_FVECTOR Q1, VE_FVECTOR Q2, VE_GVECTOR  Q3) noexcept
{
	VE_ASSERT(pA);
	VE_ASSERT(pB);
	VE_ASSERT(pC);
	VE_VECTOR LS12 = VeQuaternionLengthSq(VeVectorAdd(Q1, Q2));
	VE_VECTOR LD12 = VeQuaternionLengthSq(VeVectorSubtract(Q1, Q2));
	VE_VECTOR SQ2 = VeVectorNegate(Q2);
	VE_VECTOR Control1 = VeVectorLess(LS12, LD12);
	SQ2 = VeVectorSelect(Q2, SQ2, Control1);
	VE_VECTOR LS01 = VeQuaternionLengthSq(VeVectorAdd(Q0, Q1));
	VE_VECTOR LD01 = VeQuaternionLengthSq(VeVectorSubtract(Q0, Q1));
	VE_VECTOR SQ0 = VeVectorNegate(Q0);
	VE_VECTOR LS23 = VeQuaternionLengthSq(VeVectorAdd(SQ2, Q3));
	VE_VECTOR LD23 = VeQuaternionLengthSq(VeVectorSubtract(SQ2, Q3));
	VE_VECTOR SQ3 = VeVectorNegate(Q3);
	VE_VECTOR Control0 = VeVectorLess(LS01, LD01);
	VE_VECTOR Control2 = VeVectorLess(LS23, LD23);
	SQ0 = VeVectorSelect(Q0, SQ0, Control0);
	SQ3 = VeVectorSelect(Q3, SQ3, Control2);
	VE_VECTOR InvQ1 = VeQuaternionInverse(Q1);
	VE_VECTOR InvQ2 = VeQuaternionInverse(SQ2);
	VE_VECTOR LnQ0 = VeQuaternionLn(VeQuaternionMultiply(InvQ1, SQ0));
	VE_VECTOR LnQ2 = VeQuaternionLn(VeQuaternionMultiply(InvQ1, SQ2));
	VE_VECTOR LnQ1 = VeQuaternionLn(VeQuaternionMultiply(InvQ2, Q1));
	VE_VECTOR LnQ3 = VeQuaternionLn(VeQuaternionMultiply(InvQ2, SQ3));
	const VE_VECTOR NegativeOneQuarter = VeVectorSplatConstant(-1, 2);
	VE_VECTOR ExpQ02 = VeVectorMultiply(VeVectorAdd(LnQ0, LnQ2), NegativeOneQuarter);
	VE_VECTOR ExpQ13 = VeVectorMultiply(VeVectorAdd(LnQ1, LnQ3), NegativeOneQuarter);
	ExpQ02 = VeQuaternionExp(ExpQ02);
	ExpQ13 = VeQuaternionExp(ExpQ13);
	*pA = VeQuaternionMultiply(Q1, ExpQ02);
	*pB = VeQuaternionMultiply(SQ2, ExpQ13);
	*pC = SQ2;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionBaryCentric(
	VE_FVECTOR Q0, VE_FVECTOR Q1, VE_FVECTOR Q2, VeFloat32 f,
	VeFloat32 g) noexcept
{
	VeFloat32 s = f + g;
	VE_VECTOR Result;
	if ((s < 0.00001f) && (s > -0.00001f))
	{
		Result = Q0;
	}
	else
	{
		VE_VECTOR Q01 = VeQuaternionSlerp(Q0, Q1, s);
		VE_VECTOR Q02 = VeQuaternionSlerp(Q0, Q2, s);
		Result = VeQuaternionSlerp(Q01, Q02, g / s);
	}
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionBaryCentricV(
	VE_FVECTOR Q0, VE_FVECTOR Q1, VE_FVECTOR Q2, VE_GVECTOR F,
	VE_HVECTOR G) noexcept
{
	VE_ASSERT((VeVectorGetY(F) == VeVectorGetX(F))
		&& (VeVectorGetZ(F) == VeVectorGetX(F))
		&& (VeVectorGetW(F) == VeVectorGetX(F)));
	VE_ASSERT((VeVectorGetY(G) == VeVectorGetX(G))
		&& (VeVectorGetZ(G) == VeVectorGetX(G))
		&& (VeVectorGetW(G) == VeVectorGetX(G)));
	const VE_VECTOR Epsilon = VeVectorSplatConstant(1, 16);
	VE_VECTOR S = VeVectorAdd(F, G);
	VE_VECTOR Result;
	if (VeVector4InBounds(S, Epsilon))
	{
		Result = Q0;
	}
	else
	{
		VE_VECTOR Q01 = VeQuaternionSlerpV(Q0, Q1, S);
		VE_VECTOR Q02 = VeQuaternionSlerpV(Q0, Q2, S);
		VE_VECTOR GS = VeVectorReciprocal(S);
		GS = VeVectorMultiply(G, GS);
		Result = VeQuaternionSlerpV(Q01, Q02, GS);
	}
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionIdentity() noexcept
{
	return g_MathIdentityR3.v;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionRotationRollPitchYaw(
	VeFloat32 Pitch, VeFloat32 Yaw, VeFloat32 Roll) noexcept
{
	VE_VECTOR Angles = VeVectorSet(Pitch, Yaw, Roll, 0.0f);
	VE_VECTOR Q = VeQuaternionRotationRollPitchYawFromVector(Angles);
	return Q;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionRotationRollPitchYawFromVector(
	VE_FVECTOR Angles) noexcept
{
	static const VE_VECTORF32 Sign = { 1.0f, -1.0f, -1.0f, 1.0f };
	VE_VECTOR HalfAngles = VeVectorMultiply(Angles, g_MathOneHalf.v);
	VE_VECTOR SinAngles, CosAngles;
	VeVectorSinCos(&SinAngles, &CosAngles, HalfAngles);
	VE_VECTOR P0 = VeVectorPermute<VE_PERMUTE_0X, VE_PERMUTE_1X, VE_PERMUTE_1X, VE_PERMUTE_1X>(SinAngles, CosAngles);
	VE_VECTOR Y0 = VeVectorPermute<VE_PERMUTE_1Y, VE_PERMUTE_0Y, VE_PERMUTE_1Y, VE_PERMUTE_1Y>(SinAngles, CosAngles);
	VE_VECTOR R0 = VeVectorPermute<VE_PERMUTE_1Z, VE_PERMUTE_1Z, VE_PERMUTE_0Z, VE_PERMUTE_1Z>(SinAngles, CosAngles);
	VE_VECTOR P1 = VeVectorPermute<VE_PERMUTE_0X, VE_PERMUTE_1X, VE_PERMUTE_1X, VE_PERMUTE_1X>(CosAngles, SinAngles);
	VE_VECTOR Y1 = VeVectorPermute<VE_PERMUTE_1Y, VE_PERMUTE_0Y, VE_PERMUTE_1Y, VE_PERMUTE_1Y>(CosAngles, SinAngles);
	VE_VECTOR R1 = VeVectorPermute<VE_PERMUTE_1Z, VE_PERMUTE_1Z, VE_PERMUTE_0Z, VE_PERMUTE_1Z>(CosAngles, SinAngles);
	VE_VECTOR Q1 = VeVectorMultiply(P1, Sign.v);
	VE_VECTOR Q0 = VeVectorMultiply(P0, Y0);
	Q1 = VeVectorMultiply(Q1, Y1);
	Q0 = VeVectorMultiply(Q0, R0);
	VE_VECTOR Q = VeVectorMultiplyAdd(Q1, R1, Q0);
	return Q;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionRotationNormal(
	VE_FVECTOR NormalAxis, VeFloat32 Angle) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR N = VeVectorSelect(g_MathOne.v, NormalAxis, g_MathSelect1110.v);
	VeFloat32 SinV, CosV;
	VeScalarSinCos(&SinV, &CosV, 0.5f * Angle);
	VE_VECTOR Scale = VeVectorSet(SinV, SinV, SinV, CosV);
	return VeVectorMultiply(N, Scale);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR N = _mm_and_ps(NormalAxis, g_MathMask3);
	N = _mm_or_ps(N, g_MathIdentityR3);
	VE_VECTOR Scale = _mm_set_ps1(0.5f * Angle);
	VE_VECTOR vSine;
	VE_VECTOR vCosine;
	VeVectorSinCos(&vSine, &vCosine, Scale);
	Scale = _mm_and_ps(vSine, g_MathMask3);
	vCosine = _mm_and_ps(vCosine, g_MathMaskW);
	Scale = _mm_or_ps(Scale, vCosine);
	N = _mm_mul_ps(N, Scale);
	return N;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionRotationAxis(
	VE_FVECTOR Axis, VeFloat32 Angle) noexcept
{
	VE_ASSERT(!VeVector3Equal(Axis, VeVectorZero()));
	VE_ASSERT(!VeVector3IsInfinite(Axis));
	VE_VECTOR Normal = VeVector3Normalize(Axis);
	VE_VECTOR Q = VeQuaternionRotationNormal(Normal, Angle);
	return Q;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionRotationMatrix(
	VE_FMATRIX M) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTORF32 q;
	VeFloat32 r22 = M.m[2][2];
	if (r22 <= 0.f)
	{
		VeFloat32 dif10 = M.m[1][1] - M.m[0][0];
		VeFloat32 omr22 = 1.f - r22;
		if (dif10 <= 0.f)
		{
			VeFloat32 fourXSqr = omr22 - dif10;
			VeFloat32 inv4x = 0.5f / VeSqrtf(fourXSqr);
			q.f[0] = fourXSqr*inv4x;
			q.f[1] = (M.m[0][1] + M.m[1][0])*inv4x;
			q.f[2] = (M.m[0][2] + M.m[2][0])*inv4x;
			q.f[3] = (M.m[1][2] - M.m[2][1])*inv4x;
		}
		else
		{
			VeFloat32 fourYSqr = omr22 + dif10;
			VeFloat32 inv4y = 0.5f / VeSqrtf(fourYSqr);
			q.f[0] = (M.m[0][1] + M.m[1][0])*inv4y;
			q.f[1] = fourYSqr*inv4y;
			q.f[2] = (M.m[1][2] + M.m[2][1])*inv4y;
			q.f[3] = (M.m[2][0] - M.m[0][2])*inv4y;
		}
	}
	else
	{
		VeFloat32 sum10 = M.m[1][1] + M.m[0][0];
		VeFloat32 opr22 = 1.f + r22;
		if (sum10 <= 0.f)
		{
			VeFloat32 fourZSqr = opr22 - sum10;
			VeFloat32 inv4z = 0.5f / VeSqrtf(fourZSqr);
			q.f[0] = (M.m[0][2] + M.m[2][0])*inv4z;
			q.f[1] = (M.m[1][2] + M.m[2][1])*inv4z;
			q.f[2] = fourZSqr*inv4z;
			q.f[3] = (M.m[0][1] - M.m[1][0])*inv4z;
		}
		else
		{
			VeFloat32 fourWSqr = opr22 + sum10;
			VeFloat32 inv4w = 0.5f / VeSqrtf(fourWSqr);
			q.f[0] = (M.m[1][2] - M.m[2][1])*inv4w;
			q.f[1] = (M.m[2][0] - M.m[0][2])*inv4w;
			q.f[2] = (M.m[0][1] - M.m[1][0])*inv4w;
			q.f[3] = fourWSqr*inv4w;
		}
	}
	return q.v;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 VEPMMP = { +1.0f, -1.0f, -1.0f, +1.0f };
	static const VE_VECTORF32 VEMPMP = { -1.0f, +1.0f, -1.0f, +1.0f };
	static const VE_VECTORF32 VEMMPP = { -1.0f, -1.0f, +1.0f, +1.0f };
	VE_VECTOR r0 = M.r[0];
	VE_VECTOR r1 = M.r[1];
	VE_VECTOR r2 = M.r[2];
	VE_VECTOR r00 = VE_PERMUTE_PS(r0, _MM_SHUFFLE(0, 0, 0, 0));
	VE_VECTOR r11 = VE_PERMUTE_PS(r1, _MM_SHUFFLE(1, 1, 1, 1));
	VE_VECTOR r22 = VE_PERMUTE_PS(r2, _MM_SHUFFLE(2, 2, 2, 2));
	VE_VECTOR r11mr00 = _mm_sub_ps(r11, r00);
	VE_VECTOR x2gey2 = _mm_cmple_ps(r11mr00, g_MathZero);
	VE_VECTOR r11pr00 = _mm_add_ps(r11, r00);
	VE_VECTOR z2gew2 = _mm_cmple_ps(r11pr00, g_MathZero);
	VE_VECTOR x2py2gez2pw2 = _mm_cmple_ps(r22, g_MathZero);
	VE_VECTOR t0 = _mm_mul_ps(VEPMMP, r00);
	VE_VECTOR t1 = _mm_mul_ps(VEMPMP, r11);
	VE_VECTOR t2 = _mm_mul_ps(VEMMPP, r22);
	VE_VECTOR x2y2z2w2 = _mm_add_ps(t0, t1);
	x2y2z2w2 = _mm_add_ps(t2, x2y2z2w2);
	x2y2z2w2 = _mm_add_ps(x2y2z2w2, g_MathOne);
	t0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 2, 2, 1));
	t1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 0, 0));
	t1 = VE_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
	VE_VECTOR xyxzyz = _mm_add_ps(t0, t1);
	t0 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(0, 0, 0, 1));
	t1 = _mm_shuffle_ps(r1, r0, _MM_SHUFFLE(1, 2, 2, 2));
	t1 = VE_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
	VE_VECTOR xwywzw = _mm_sub_ps(t0, t1);
	xwywzw = _mm_mul_ps(VEMPMP, xwywzw);
	t0 = _mm_shuffle_ps(x2y2z2w2, xyxzyz, _MM_SHUFFLE(0, 0, 1, 0));
	t1 = _mm_shuffle_ps(x2y2z2w2, xwywzw, _MM_SHUFFLE(0, 2, 3, 2));
	t2 = _mm_shuffle_ps(xyxzyz, xwywzw, _MM_SHUFFLE(1, 0, 2, 1));
	VE_VECTOR tensor0 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(2, 0, 2, 0));
	VE_VECTOR tensor1 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(3, 1, 1, 2));
	VE_VECTOR tensor2 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(2, 0, 1, 0));
	VE_VECTOR tensor3 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(1, 2, 3, 2));
	t0 = _mm_and_ps(x2gey2, tensor0);
	t1 = _mm_andnot_ps(x2gey2, tensor1);
	t0 = _mm_or_ps(t0, t1);
	t1 = _mm_and_ps(z2gew2, tensor2);
	t2 = _mm_andnot_ps(z2gew2, tensor3);
	t1 = _mm_or_ps(t1, t2);
	t0 = _mm_and_ps(x2py2gez2pw2, t0);
	t1 = _mm_andnot_ps(x2py2gez2pw2, t1);
	t2 = _mm_or_ps(t0, t1);
	t0 = VeVector4Length(t2);
	return _mm_div_ps(t2, t0);
#	endif
}
//--------------------------------------------------------------------------
inline VeFloat32 VeScalarACos(VeFloat32 Value) noexcept;
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeQuaternionToAxisAngle(
	VE_VECTOR* pAxis, VeFloat32* pAngle, VE_FVECTOR Q) noexcept
{
	VE_ASSERT(pAxis);
	VE_ASSERT(pAngle);
	*pAxis = Q;
	*pAngle = 2.0f * VeScalarACos(VeVectorGetW(Q));
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VePlaneEqual(
	VE_FVECTOR P1, VE_FVECTOR P2) noexcept
{
	return VeVector4Equal(P1, P2);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VePlaneNearEqual(
	VE_FVECTOR P1, VE_FVECTOR P2, VE_FVECTOR Epsilon) noexcept
{
	VE_VECTOR NP1 = VePlaneNormalize(P1);
	VE_VECTOR NP2 = VePlaneNormalize(P2);
	return VeVector4NearEqual(NP1, NP2, Epsilon);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VePlaneNotEqual(
	VE_FVECTOR P1, VE_FVECTOR P2) noexcept
{
	return VeVector4NotEqual(P1, P2);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VePlaneIsNaN(
	VE_FVECTOR P) noexcept
{
	return VeVector4IsNaN(P);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VePlaneIsInfinite(
	VE_FVECTOR P) noexcept
{
	return VeVector4IsInfinite(P);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VePlaneDot(
	VE_FVECTOR P, VE_FVECTOR V) noexcept
{
	return VeVector4Dot(P, V);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VePlaneDotCoord(
	VE_FVECTOR P, VE_FVECTOR V) noexcept
{
	VE_VECTOR V3 = VeVectorSelect(g_MathOne.v, V, g_MathSelect1110.v);
	VE_VECTOR Result = VeVector4Dot(P, V3);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VePlaneDotNormal(
	VE_FVECTOR P, VE_FVECTOR V) noexcept
{
	return VeVector3Dot(P, V);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VePlaneNormalizeEst(
	VE_FVECTOR P) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result = VeVector3ReciprocalLengthEst(P);
	return VeVectorMultiply(P, Result);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vDot = _mm_mul_ps(P, P);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
	vDot = _mm_add_ss(vDot, vTemp);
	vTemp = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vDot = _mm_add_ss(vDot, vTemp);
	vDot = VE_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
	vDot = _mm_rsqrt_ps(vDot);
	vDot = _mm_mul_ps(vDot, P);
	return vDot;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VePlaneNormalize(
	VE_FVECTOR P) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fLengthSq = VeSqrtf((P.vector4_f32[0] * P.vector4_f32[0]) + (P.vector4_f32[1] * P.vector4_f32[1]) + (P.vector4_f32[2] * P.vector4_f32[2]));
	if (fLengthSq)
	{
		fLengthSq = 1.0f / fLengthSq;
	}
	{
		VE_VECTOR vResult =
		{
			P.vector4_f32[0] * fLengthSq,
			P.vector4_f32[1] * fLengthSq,
			P.vector4_f32[2] * fLengthSq,
			P.vector4_f32[3] * fLengthSq
		};
		return vResult;
	}
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(P, P);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vTemp = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	VE_VECTOR vResult = _mm_sqrt_ps(vLengthSq);
	vLengthSq = _mm_cmpneq_ps(vLengthSq, g_MathInfinity);
	vResult = _mm_div_ps(P, vResult);
	vResult = _mm_and_ps(vResult, vLengthSq);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VePlaneIntersectLine(
	VE_FVECTOR P, VE_FVECTOR LinePoint1, VE_FVECTOR LinePoint2) noexcept
{
	VE_VECTOR V1 = VeVector3Dot(P, LinePoint1);
	VE_VECTOR V2 = VeVector3Dot(P, LinePoint2);
	VE_VECTOR D = VeVectorSubtract(V1, V2);
	VE_VECTOR VT = VePlaneDotCoord(P, LinePoint1);
	VT = VeVectorDivide(VT, D);
	VE_VECTOR Point = VeVectorSubtract(LinePoint2, LinePoint1);
	Point = VeVectorMultiplyAdd(Point, VT, LinePoint1);
	const VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR Control = VeVectorNearEqual(D, Zero, g_MathEpsilon.v);
	return VeVectorSelect(Point, g_MathQNaN.v, Control);
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VePlaneIntersectPlane(
	VE_VECTOR* pLinePoint1, VE_VECTOR* pLinePoint2, VE_FVECTOR  P1,
	VE_FVECTOR  P2) noexcept
{
	VE_ASSERT(pLinePoint1);
	VE_ASSERT(pLinePoint2);
	VE_VECTOR V1 = VeVector3Cross(P2, P1);
	VE_VECTOR LengthSq = VeVector3LengthSq(V1);
	VE_VECTOR V2 = VeVector3Cross(P2, V1);
	VE_VECTOR P1W = VeVectorSplatW(P1);
	VE_VECTOR Point = VeVectorMultiply(V2, P1W);
	VE_VECTOR V3 = VeVector3Cross(V1, P1);
	VE_VECTOR P2W = VeVectorSplatW(P2);
	Point = VeVectorMultiplyAdd(V3, P2W, Point);
	VE_VECTOR LinePoint1 = VeVectorDivide(Point, LengthSq);
	VE_VECTOR LinePoint2 = VeVectorAdd(LinePoint1, V1);
	VE_VECTOR Control = VeVectorLessOrEqual(LengthSq, g_MathEpsilon.v);
	*pLinePoint1 = VeVectorSelect(LinePoint1, g_MathQNaN.v, Control);
	*pLinePoint2 = VeVectorSelect(LinePoint2, g_MathQNaN.v, Control);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VePlaneTransform(
	VE_FVECTOR P, VE_FMATRIX M) noexcept
{
	VE_VECTOR W = VeVectorSplatW(P);
	VE_VECTOR Z = VeVectorSplatZ(P);
	VE_VECTOR Y = VeVectorSplatY(P);
	VE_VECTOR X = VeVectorSplatX(P);
	VE_VECTOR Result = VeVectorMultiply(W, M.r[3]);
	Result = VeVectorMultiplyAdd(Z, M.r[2], Result);
	Result = VeVectorMultiplyAdd(Y, M.r[1], Result);
	Result = VeVectorMultiplyAdd(X, M.r[0], Result);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_FLOAT4* VE_MATH_CALLCONV VePlaneTransformStream(
	VE_FLOAT4* pOutputStream, VeSizeT OutputStride,
	const VE_FLOAT4* pInputStream, VeSizeT InputStride,
	VeSizeT PlaneCount, VE_FMATRIX M) noexcept
{
	return VeVector4TransformStream(pOutputStream, OutputStride,
		pInputStream, InputStride, PlaneCount, M);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VePlaneFromPointNormal(
	VE_FVECTOR Point, VE_FVECTOR Normal) noexcept
{
	VE_VECTOR W = VeVector3Dot(Point, Normal);
	W = VeVectorNegate(W);
	return VeVectorSelect(W, Normal, g_MathSelect1110.v);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VePlaneFromPoints(
	VE_FVECTOR Point1, VE_FVECTOR Point2, VE_FVECTOR Point3) noexcept
{
	VE_VECTOR V21 = VeVectorSubtract(Point1, Point2);
	VE_VECTOR V31 = VeVectorSubtract(Point1, Point3);
	VE_VECTOR N = VeVector3Cross(V21, V31);
	N = VeVector3Normalize(N);
	VE_VECTOR D = VePlaneDotNormal(N, Point1);
	D = VeVectorNegate(D);
	VE_VECTOR Result = VeVectorSelect(D, N, g_MathSelect1110.v);
	return Result;
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeColorEqual(
	VE_FVECTOR C1, VE_FVECTOR C2) noexcept
{
	return VeVector4Equal(C1, C2);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeColorNotEqual(
	VE_FVECTOR C1, VE_FVECTOR C2) noexcept
{
	return VeVector4NotEqual(C1, C2);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeColorGreater(
	VE_FVECTOR C1, VE_FVECTOR C2) noexcept
{
	return VeVector4Greater(C1, C2);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeColorGreaterOrEqual(
	VE_FVECTOR C1, VE_FVECTOR C2) noexcept
{
	return VeVector4GreaterOrEqual(C1, C2);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeColorLess(
	VE_FVECTOR C1, VE_FVECTOR C2) noexcept
{
	return VeVector4Less(C1, C2);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeColorLessOrEqual(
	VE_FVECTOR C1, VE_FVECTOR C2) noexcept
{
	return VeVector4LessOrEqual(C1, C2);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeColorIsNaN(
	VE_FVECTOR C) noexcept
{
	return VeVector4IsNaN(C);
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeColorIsInfinite(
	VE_FVECTOR C) noexcept
{
	return VeVector4IsInfinite(C);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorNegative(
	VE_FVECTOR vColor) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTORF32 vResult =
	{
		1.0f - vColor.vector4_f32[0],
		1.0f - vColor.vector4_f32[1],
		1.0f - vColor.vector4_f32[2],
		vColor.vector4_f32[3]
	};
	return vResult.v;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_xor_ps(vColor, g_MathNegate3);
	return _mm_add_ps(vTemp, g_MathOne3);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorModulate(
	VE_FVECTOR C1, VE_FVECTOR C2) noexcept
{
	return VeVectorMultiply(C1, C2);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorAdjustSaturation(
	VE_FVECTOR vColor, VeFloat32 fSaturation) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	const VE_VECTORF32 gvLuminance = { 0.2125f, 0.7154f, 0.0721f, 0.0f };
	VeFloat32 fLuminance = (vColor.vector4_f32[0] * gvLuminance.f[0]) + (vColor.vector4_f32[1] * gvLuminance.f[1]) + (vColor.vector4_f32[2] * gvLuminance.f[2]);
	VE_VECTOR vResult;
	vResult.vector4_f32[0] = ((vColor.vector4_f32[0] - fLuminance)*fSaturation) + fLuminance;
	vResult.vector4_f32[1] = ((vColor.vector4_f32[1] - fLuminance)*fSaturation) + fLuminance;
	vResult.vector4_f32[2] = ((vColor.vector4_f32[2] - fLuminance)*fSaturation) + fLuminance;
	vResult.vector4_f32[3] = vColor.vector4_f32[3];
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 gvLuminance = { 0.2125f, 0.7154f, 0.0721f, 0.0f };
	VE_VECTOR vLuminance = VeVector3Dot(vColor, gvLuminance);
	VE_VECTOR vSaturation = _mm_set_ps1(fSaturation);
	VE_VECTOR vResult = _mm_sub_ps(vColor, vLuminance);
	vResult = _mm_mul_ps(vResult, vSaturation);
	vResult = _mm_add_ps(vResult, vLuminance);
	vLuminance = _mm_shuffle_ps(vResult, vColor, _MM_SHUFFLE(3, 2, 2, 2));
	vResult = _mm_shuffle_ps(vResult, vLuminance, _MM_SHUFFLE(3, 0, 1, 0));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorAdjustContrast(
	VE_FVECTOR vColor, VeFloat32 fContrast) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTORF32 vResult =
	{
		((vColor.vector4_f32[0] - 0.5f) * fContrast) + 0.5f,
		((vColor.vector4_f32[1] - 0.5f) * fContrast) + 0.5f,
		((vColor.vector4_f32[2] - 0.5f) * fContrast) + 0.5f,
		vColor.vector4_f32[3]
	};
	return vResult.v;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vScale = _mm_set_ps1(fContrast);
	VE_VECTOR vResult = _mm_sub_ps(vColor, g_MathOneHalf);
	vResult = _mm_mul_ps(vResult, vScale);
	vResult = _mm_add_ps(vResult, g_MathOneHalf);
	vScale = _mm_shuffle_ps(vResult, vColor, _MM_SHUFFLE(3, 2, 2, 2));
	vResult = _mm_shuffle_ps(vResult, vScale, _MM_SHUFFLE(3, 0, 1, 0));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorRGBToHSL(VE_FVECTOR rgb) noexcept
{
	VE_VECTOR r = VeVectorSplatX(rgb);
	VE_VECTOR g = VeVectorSplatY(rgb);
	VE_VECTOR b = VeVectorSplatZ(rgb);
	VE_VECTOR min = VeVectorMin(r, VeVectorMin(g, b));
	VE_VECTOR max = VeVectorMax(r, VeVectorMax(g, b));
	VE_VECTOR l = VeVectorMultiply(VeVectorAdd(min, max), g_MathOneHalf);
	VE_VECTOR d = VeVectorSubtract(max, min);
	VE_VECTOR la = VeVectorSelect(rgb, l, g_MathSelect1110);
	if (VeVector3Less(d, g_MathEpsilon))
	{
		return VeVectorSelect(la, g_MathZero, g_MathSelect1100);
	}
	else
	{
		VE_VECTOR s, h;
		VE_VECTOR d2 = VeVectorAdd(min, max);
		if (VeVector3Greater(l, g_MathOneHalf))
		{
			s = VeVectorDivide(d, VeVectorSubtract(g_MathTwo, d2));
		}
		else
		{
			s = VeVectorDivide(d, d2);
		}

		if (VeVector3Equal(r, max))
		{
			h = VeVectorDivide(VeVectorSubtract(g, b), d);
		}
		else if (VeVector3Equal(g, max))
		{
			h = VeVectorDivide(VeVectorSubtract(b, r), d);
			h = VeVectorAdd(h, g_MathTwo);
		}
		else
		{
			h = VeVectorDivide(VeVectorSubtract(r, g), d);
			h = VeVectorAdd(h, g_MathFour);
		}
		h = VeVectorDivide(h, g_MathSix);
		if (VeVector3Less(h, g_MathZero))
			h = VeVectorAdd(h, g_MathOne);
		VE_VECTOR lha = VeVectorSelect(la, h, g_MathSelect1100);
		return VeVectorSelect(s, lha, g_MathSelect1011);
	}
}
//--------------------------------------------------------------------------
namespace venus
{
	inline VE_VECTOR VE_MATH_CALLCONV VeColorHue2Clr(
		VE_FVECTOR p, VE_FVECTOR q, VE_FVECTOR h) noexcept
	{
		static const VE_VECTORF32 oneSixth = { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f };
		static const VE_VECTORF32 twoThirds = { 2.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f };
		VE_VECTOR t = h;
		if (VeVector3Less(t, g_MathZero))
			t = VeVectorAdd(t, g_MathOne);
		if (VeVector3Greater(t, g_MathOne))
			t = VeVectorSubtract(t, g_MathOne);
		if (VeVector3Less(t, oneSixth))
		{
			VE_VECTOR t1 = VeVectorSubtract(q, p);
			VE_VECTOR t2 = VeVectorMultiply(g_MathSix, t);
			return VeVectorMultiplyAdd(t1, t2, p);
		}
		if (VeVector3Less(t, g_MathOneHalf))
			return q;
		if (VeVector3Less(t, twoThirds))
		{
			VE_VECTOR t1 = VeVectorSubtract(q, p);
			VE_VECTOR t2 = VeVectorMultiply(g_MathSix, VeVectorSubtract(twoThirds, t));
			return VeVectorMultiplyAdd(t1, t2, p);
		}
		return p;
	}
};
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorHSLToRGB(VE_FVECTOR hsl) noexcept
{
	static const VE_VECTORF32 oneThird = { 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f };
	VE_VECTOR s = VeVectorSplatY(hsl);
	VE_VECTOR l = VeVectorSplatZ(hsl);
	if (VeVector3NearEqual(s, g_MathZero, g_MathEpsilon))
	{
		return VeVectorSelect(hsl, l, g_MathSelect1110);
	}
	else
	{
		VE_VECTOR h = VeVectorSplatX(hsl);
		VE_VECTOR q;
		if (VeVector3Less(l, g_MathOneHalf))
		{
			q = VeVectorMultiply(l, VeVectorAdd(g_MathOne, s));
		}
		else
		{
			q = VeVectorSubtract(VeVectorAdd(l, s), VeVectorMultiply(l, s));
		}
		VE_VECTOR p = VeVectorSubtract(VeVectorMultiply(g_MathTwo, l), q);
		VE_VECTOR r = venus::VeColorHue2Clr(p, q, VeVectorAdd(h, oneThird));
		VE_VECTOR g = venus::VeColorHue2Clr(p, q, h);
		VE_VECTOR b = venus::VeColorHue2Clr(p, q, VeVectorSubtract(h, oneThird));
		VE_VECTOR rg = VeVectorSelect(g, r, g_MathSelect1000);
		VE_VECTOR ba = VeVectorSelect(hsl, b, g_MathSelect1110);
		return VeVectorSelect(ba, rg, g_MathSelect1100);
	}
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorRGBToHSV(VE_FVECTOR rgb) noexcept
{
	VE_VECTOR r = VeVectorSplatX(rgb);
	VE_VECTOR g = VeVectorSplatY(rgb);
	VE_VECTOR b = VeVectorSplatZ(rgb);
	VE_VECTOR min = VeVectorMin(r, VeVectorMin(g, b));
	VE_VECTOR v = VeVectorMax(r, VeVectorMax(g, b));
	VE_VECTOR d = VeVectorSubtract(v, min);
	VE_VECTOR s = (VeVector3NearEqual(v, g_MathZero, g_MathEpsilon)) ? g_MathZero : VeVectorDivide(d, v);
	if (VeVector3Less(d, g_MathEpsilon))
	{
		VE_VECTOR hv = VeVectorSelect(v, g_MathZero, g_MathSelect1000);
		VE_VECTOR hva = VeVectorSelect(rgb, hv, g_MathSelect1110);
		return VeVectorSelect(s, hva, g_MathSelect1011);
	}
	else
	{
		VE_VECTOR h;
		if (VeVector3Equal(r, v))
		{
			h = VeVectorDivide(VeVectorSubtract(g, b), d);
			if (VeVector3Less(g, b))
				h = VeVectorAdd(h, g_MathSix);
		}
		else if (VeVector3Equal(g, v))
		{
			h = VeVectorDivide(VeVectorSubtract(b, r), d);
			h = VeVectorAdd(h, g_MathTwo);
		}
		else
		{
			h = VeVectorDivide(VeVectorSubtract(r, g), d);
			h = VeVectorAdd(h, g_MathFour);
		}
		h = VeVectorDivide(h, g_MathSix);
		VE_VECTOR hv = VeVectorSelect(v, h, g_MathSelect1000);
		VE_VECTOR hva = VeVectorSelect(rgb, hv, g_MathSelect1110);
		return VeVectorSelect(s, hva, g_MathSelect1011);
	}
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorHSVToRGB(VE_FVECTOR hsv)
{
	VE_VECTOR h = VeVectorSplatX(hsv);
	VE_VECTOR s = VeVectorSplatY(hsv);
	VE_VECTOR v = VeVectorSplatZ(hsv);
	VE_VECTOR h6 = VeVectorMultiply(h, g_MathSix);
	VE_VECTOR i = VeVectorFloor(h6);
	VE_VECTOR f = VeVectorSubtract(h6, i);
	VE_VECTOR p = VeVectorMultiply(v, VeVectorSubtract(g_MathOne, s));
	VE_VECTOR q = VeVectorMultiply(v, VeVectorSubtract(g_MathOne, VeVectorMultiply(f, s)));
	VE_VECTOR t = VeVectorMultiply(v, VeVectorSubtract(g_MathOne, VeVectorMultiply(VeVectorSubtract(g_MathOne, f), s)));
	VeInt32 ii = static_cast<VeInt32>(VeVectorGetX(VeVectorMod(i, g_MathSix)));
	VE_VECTOR _rgb;
	switch (ii)
	{
	case 0:
	{
		VE_VECTOR vt = VeVectorSelect(t, v, g_MathSelect1000);
		_rgb = VeVectorSelect(p, vt, g_MathSelect1100);
	}
	break;
	case 1:
	{
		VE_VECTOR qv = VeVectorSelect(v, q, g_MathSelect1000);
		_rgb = VeVectorSelect(p, qv, g_MathSelect1100);
	}
	break;
	case 2:
	{
		VE_VECTOR pv = VeVectorSelect(v, p, g_MathSelect1000);
		_rgb = VeVectorSelect(t, pv, g_MathSelect1100);
	}
	break;
	case 3:
	{
		VE_VECTOR pq = VeVectorSelect(q, p, g_MathSelect1000);
		_rgb = VeVectorSelect(v, pq, g_MathSelect1100);
	}
	break;
	case 4:
	{
		VE_VECTOR tp = VeVectorSelect(p, t, g_MathSelect1000);
		_rgb = VeVectorSelect(v, tp, g_MathSelect1100);
	}
	break;
	default:
	{
		VE_VECTOR vp = VeVectorSelect(p, v, g_MathSelect1000);
		_rgb = VeVectorSelect(q, vp, g_MathSelect1100);
	}
	break;
	}

	return VeVectorSelect(hsv, _rgb, g_MathSelect1110);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorRGBToYUV(VE_FVECTOR rgb) noexcept
{
	static const VE_VECTORF32 Scale0 = { 0.299f, -0.147f,  0.615f, 0.0f };
	static const VE_VECTORF32 Scale1 = { 0.587f, -0.289f, -0.515f, 0.0f };
	static const VE_VECTORF32 Scale2 = { 0.114f,  0.436f, -0.100f, 0.0f };
	VE_MATRIX M(Scale0, Scale1, Scale2, g_MathZero);
	VE_VECTOR clr = VeVector3Transform(rgb, M);
	return VeVectorSelect(rgb, clr, g_MathSelect1110);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorYUVToRGB(VE_FVECTOR yuv) noexcept
{
	static const VE_VECTORF32 Scale1 = { 0.0f, -0.395f, 2.032f, 0.0f };
	static const VE_VECTORF32 Scale2 = { 1.140f, -0.581f,   0.0f, 0.0f };
	VE_MATRIX M(g_MathOne, Scale1, Scale2, g_MathZero);
	VE_VECTOR clr = VeVector3Transform(yuv, M);
	return VeVectorSelect(yuv, clr, g_MathSelect1110);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorRGBToYUV_HD(
	VE_FVECTOR rgb) noexcept
{
	static const VE_VECTORF32 Scale0 = { 0.2126f, -0.0997f,  0.6150f, 0.0f };
	static const VE_VECTORF32 Scale1 = { 0.7152f, -0.3354f, -0.5586f, 0.0f };
	static const VE_VECTORF32 Scale2 = { 0.0722f,  0.4351f, -0.0564f, 0.0f };
	VE_MATRIX M(Scale0, Scale1, Scale2, g_MathZero);
	VE_VECTOR clr = VeVector3Transform(rgb, M);
	return VeVectorSelect(rgb, clr, g_MathSelect1110);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorYUVToRGB_HD(
	VE_FVECTOR yuv) noexcept
{
	static const VE_VECTORF32 Scale1 = { 0.0f, -0.2153f, 2.1324f, 0.0f };
	static const VE_VECTORF32 Scale2 = { 1.2803f, -0.3806f, 0.0f, 0.0f };
	VE_MATRIX M(g_MathOne, Scale1, Scale2, g_MathZero);
	VE_VECTOR clr = VeVector3Transform(yuv, M);
	return VeVectorSelect(yuv, clr, g_MathSelect1110);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorRGBToXYZ(VE_FVECTOR rgb) noexcept
{
	static const VE_VECTORF32 Scale0 = { 0.4887180f, 0.1762044f, 0.0000000f, 0.0f };
	static const VE_VECTORF32 Scale1 = { 0.3106803f, 0.8129847f, 0.0102048f, 0.0f };
	static const VE_VECTORF32 Scale2 = { 0.2006017f, 0.0108109f, 0.9897952f, 0.0f };
	static const VE_VECTORF32 Scale = { 1.f / 0.17697f, 1.f / 0.17697f, 1.f / 0.17697f, 0.0f };
	VE_MATRIX M(Scale0, Scale1, Scale2, g_MathZero);
	VE_VECTOR clr = VeVectorMultiply(VeVector3Transform(rgb, M), Scale);
	return VeVectorSelect(rgb, clr, g_MathSelect1110);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorXYZToRGB(VE_FVECTOR xyz) noexcept
{
	static const VE_VECTORF32 Scale0 = { 2.3706743f, -0.5138850f,  0.0052982f, 0.0f };
	static const VE_VECTORF32 Scale1 = { -0.9000405f,  1.4253036f, -0.0146949f, 0.0f };
	static const VE_VECTORF32 Scale2 = { -0.4706338f,  0.0885814f,  1.0093968f, 0.0f };
	static const VE_VECTORF32 Scale = { 0.17697f, 0.17697f, 0.17697f, 0.0f };
	VE_MATRIX M(Scale0, Scale1, Scale2, g_MathZero);
	VE_VECTOR clr = VeVector3Transform(VeVectorMultiply(xyz, Scale), M);
	return VeVectorSelect(xyz, clr, g_MathSelect1110);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorXYZToSRGB(VE_FVECTOR xyz) noexcept
{
	static const VE_VECTORF32 Scale0 = { 3.2406f, -0.9689f,  0.0557f, 0.0f };
	static const VE_VECTORF32 Scale1 = { -1.5372f,  1.8758f, -0.2040f, 0.0f };
	static const VE_VECTORF32 Scale2 = { -0.4986f,  0.0415f,  1.0570f, 0.0f };
	static const VE_VECTORF32 Cutoff = { 0.0031308f, 0.0031308f, 0.0031308f, 0.0f };
	static const VE_VECTORF32 Exp = { 1.0f / 2.4f, 1.0f / 2.4f, 1.0f / 2.4f, 1.0f };
	VE_MATRIX M(Scale0, Scale1, Scale2, g_MathZero);
	VE_VECTOR lclr = VeVector3Transform(xyz, M);
	VE_VECTOR sel = VeVectorGreater(lclr, Cutoff);
	VE_VECTOR smallC = VeVectorMultiply(lclr, g_MathsrgbScale);
	VE_VECTOR largeC = VeVectorSubtract(VeVectorMultiply(g_MathsrgbA1, VeVectorPow(lclr, Exp)), g_MathsrgbA);
	VE_VECTOR clr = VeVectorSelect(smallC, largeC, sel);
	return VeVectorSelect(xyz, clr, g_MathSelect1110);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorSRGBToXYZ(VE_FVECTOR srgb) noexcept
{
	static const VE_VECTORF32 Scale0 = { 0.4124f, 0.2126f, 0.0193f, 0.0f };
	static const VE_VECTORF32 Scale1 = { 0.3576f, 0.7152f, 0.1192f, 0.0f };
	static const VE_VECTORF32 Scale2 = { 0.1805f, 0.0722f, 0.9505f, 0.0f };
	static const VE_VECTORF32 Cutoff = { 0.04045f, 0.04045f, 0.04045f, 0.0f };
	static const VE_VECTORF32 Exp = { 2.4f, 2.4f, 2.4f, 1.0f };
	VE_VECTOR sel = VeVectorGreater(srgb, Cutoff);
	VE_VECTOR smallC = VeVectorDivide(srgb, g_MathsrgbScale);
	VE_VECTOR largeC = VeVectorPow(VeVectorDivide(VeVectorAdd(srgb, g_MathsrgbA), g_MathsrgbA1), Exp);
	VE_VECTOR lclr = VeVectorSelect(smallC, largeC, sel);
	VE_MATRIX M(Scale0, Scale1, Scale2, g_MathZero);
	VE_VECTOR clr = VeVector3Transform(lclr, M);
	return VeVectorSelect(srgb, clr, g_MathSelect1110);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorRGBToSRGB(VE_FVECTOR rgb) noexcept
{
	static const VE_VECTORF32 Cutoff = { 0.0031308f, 0.0031308f, 0.0031308f, 1.f };
	static const VE_VECTORF32 Linear = { 12.92f, 12.92f, 12.92f, 1.f };
	static const VE_VECTORF32 Scale = { 1.055f, 1.055f, 1.055f, 1.f };
	static const VE_VECTORF32 Bias = { 0.055f, 0.055f, 0.055f, 0.f };
	static const VE_VECTORF32 InvGamma = { 1.0f / 2.4f, 1.0f / 2.4f, 1.0f / 2.4f, 1.f };
	VE_VECTOR V = VeVectorSaturate(rgb);
	VE_VECTOR V0 = VeVectorMultiply(V, Linear);
	VE_VECTOR V1 = Scale * VeVectorPow(V, InvGamma) - Bias;
	VE_VECTOR select = VeVectorLess(V, Cutoff);
	V = VeVectorSelect(V1, V0, select);
	return VeVectorSelect(rgb, V, g_MathSelect1110);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeColorSRGBToRGB(VE_FVECTOR srgb) noexcept
{
	static const VE_VECTORF32 Cutoff = { 0.04045f, 0.04045f, 0.04045f, 1.f };
	static const VE_VECTORF32 ILinear = { 1.f / 12.92f, 1.f / 12.92f, 1.f / 12.92f, 1.f };
	static const VE_VECTORF32 Scale = { 1.f / 1.055f, 1.f / 1.055f, 1.f / 1.055f, 1.f };
	static const VE_VECTORF32 Bias = { 0.055f, 0.055f, 0.055f, 0.f };
	static const VE_VECTORF32 Gamma = { 2.4f, 2.4f, 2.4f, 1.f };
	VE_VECTOR V = VeVectorSaturate(srgb);
	VE_VECTOR V0 = VeVectorMultiply(V, ILinear);
	VE_VECTOR V1 = VeVectorPow((V + Bias) * Scale, Gamma);
	VE_VECTOR select = VeVectorGreater(V, Cutoff);
	V = VeVectorSelect(V0, V1, select);
	return VeVectorSelect(srgb, V, g_MathSelect1110);
}
//--------------------------------------------------------------------------
inline bool VeVerifyCPUSupport()
{
#	if defined(VE_ENABLE_SSE)
	return VeCPUInfo::HasSSE2();
#	else
	return true;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeFresnelTerm(
	VE_FVECTOR CosIncidentAngle, VE_FVECTOR RefractionIndex) noexcept
{
	VE_ASSERT(!VeVector4IsInfinite(CosIncidentAngle));
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR G = VeVectorMultiplyAdd(RefractionIndex, RefractionIndex, g_MathNegativeOne.v);
	G = VeVectorMultiplyAdd(CosIncidentAngle, CosIncidentAngle, G);
	G = VeVectorAbs(G);
	G = VeVectorSqrt(G);
	VE_VECTOR S = VeVectorAdd(G, CosIncidentAngle);
	VE_VECTOR D = VeVectorSubtract(G, CosIncidentAngle);
	VE_VECTOR V0 = VeVectorMultiply(D, D);
	VE_VECTOR V1 = VeVectorMultiply(S, S);
	V1 = VeVectorReciprocal(V1);
	V0 = VeVectorMultiply(g_MathOneHalf.v, V0);
	V0 = VeVectorMultiply(V0, V1);
	VE_VECTOR V2 = VeVectorMultiplyAdd(CosIncidentAngle, S, g_MathNegativeOne.v);
	VE_VECTOR V3 = VeVectorMultiplyAdd(CosIncidentAngle, D, g_MathOne.v);
	V2 = VeVectorMultiply(V2, V2);
	V3 = VeVectorMultiply(V3, V3);
	V3 = VeVectorReciprocal(V3);
	V2 = VeVectorMultiplyAdd(V2, V3, g_MathOne.v);
	VE_VECTOR Result = VeVectorMultiply(V0, V2);
	Result = VeVectorSaturate(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR G = _mm_mul_ps(RefractionIndex, RefractionIndex);
	VE_VECTOR vTemp = _mm_mul_ps(CosIncidentAngle, CosIncidentAngle);
	G = _mm_sub_ps(G, g_MathOne);
	vTemp = _mm_add_ps(vTemp, G);
	G = _mm_setzero_ps();
	G = _mm_sub_ps(G, vTemp);
	G = _mm_max_ps(G, vTemp);
	G = _mm_sqrt_ps(G);
	VE_VECTOR GAddC = _mm_add_ps(G, CosIncidentAngle);
	VE_VECTOR GSubC = _mm_sub_ps(G, CosIncidentAngle);
	VE_VECTOR vResult = _mm_mul_ps(GSubC, GSubC);
	vTemp = _mm_mul_ps(GAddC, GAddC);
	vResult = _mm_mul_ps(vResult, g_MathOneHalf);
	vResult = _mm_div_ps(vResult, vTemp);
	GAddC = _mm_mul_ps(GAddC, CosIncidentAngle);
	GSubC = _mm_mul_ps(GSubC, CosIncidentAngle);
	GAddC = _mm_sub_ps(GAddC, g_MathOne);
	GSubC = _mm_add_ps(GSubC, g_MathOne);
	GAddC = _mm_mul_ps(GAddC, GAddC);
	GSubC = _mm_mul_ps(GSubC, GSubC);
	GAddC = _mm_div_ps(GAddC, GSubC);
	GAddC = _mm_add_ps(GAddC, g_MathOne);
	vResult = _mm_mul_ps(vResult, GAddC);
	vResult = _mm_max_ps(vResult, g_MathZero);
	vResult = _mm_min_ps(vResult, g_MathOne);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline bool VeScalarNearEqual(VeFloat32 S1, VeFloat32 S2,
	VeFloat32 Epsilon) noexcept
{
	VeFloat32 Delta = S1 - S2;
	return (VeFabsf(Delta) <= Epsilon);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeScalarModAngle(VeFloat32 Angle) noexcept
{
	Angle = Angle + VE_MATH_PI_F;
	VeFloat32 fTemp = VeFabsf(Angle);
	fTemp = fTemp - (VE_MATH_2PI_F * (VeFloat32)((VeInt32)(fTemp / VE_MATH_2PI_F)));
	fTemp = fTemp - VE_MATH_PI_F;
	if (Angle<0.0f)
	{
		fTemp = -fTemp;
	}
	return fTemp;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeScalarSin(VeFloat32 Value) noexcept
{
	VeFloat32 quotient = VE_MATH_1_2PI_F * Value;
	if (Value >= 0.0f)
	{
		quotient = (VeFloat32)((VeInt32)(quotient + 0.5f));
	}
	else
	{
		quotient = (VeFloat32)((VeInt32)(quotient - 0.5f));
	}
	VeFloat32 y = Value - VE_MATH_2PI_F * quotient;
	if (y > VE_MATH_PI_2_F)
	{
		y = VE_MATH_PI_F - y;
	}
	else if (y < -VE_MATH_PI_2_F)
	{
		y = -VE_MATH_PI_F - y;
	}
	VeFloat32 y2 = y * y;
	return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeScalarSinEst(VeFloat32 Value) noexcept
{
	VeFloat32 quotient = VE_MATH_1_2PI_F * Value;
	if (Value >= 0.0f)
	{
		quotient = (VeFloat32)((VeInt32)(quotient + 0.5f));
	}
	else
	{
		quotient = (VeFloat32)((VeInt32)(quotient - 0.5f));
	}
	VeFloat32 y = Value - VE_MATH_2PI_F * quotient;
	if (y > VE_MATH_PI_2_F)
	{
		y = VE_MATH_PI_F - y;
	}
	else if (y < -VE_MATH_PI_2_F)
	{
		y = -VE_MATH_PI_F - y;
	}
	VeFloat32 y2 = y * y;
	return (((-0.00018524670f * y2 + 0.0083139502f) * y2 - 0.16665852f) * y2 + 1.0f) * y;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeScalarCos(VeFloat32 Value) noexcept
{
	VeFloat32 quotient = VE_MATH_1_2PI_F * Value;
	if (Value >= 0.0f)
	{
		quotient = (VeFloat32)((VeInt32)(quotient + 0.5f));
	}
	else
	{
		quotient = (VeFloat32)((VeInt32)(quotient - 0.5f));
	}
	VeFloat32 y = Value - VE_MATH_2PI_F * quotient;
	VeFloat32 sign;
	if (y > VE_MATH_PI_2_F)
	{
		y = VE_MATH_PI_F - y;
		sign = -1.0f;
	}
	else if (y < -VE_MATH_PI_2_F)
	{
		y = -VE_MATH_PI_F - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}
	VeFloat32 y2 = y*y;
	VeFloat32 p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	return sign*p;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeScalarCosEst(VeFloat32 Value) noexcept
{
	VeFloat32 quotient = VE_MATH_1_2PI_F * Value;
	if (Value >= 0.0f)
	{
		quotient = (VeFloat32)((VeInt32)(quotient + 0.5f));
	}
	else
	{
		quotient = (VeFloat32)((VeInt32)(quotient - 0.5f));
	}
	VeFloat32 y = Value - VE_MATH_2PI_F * quotient;
	VeFloat32 sign;
	if (y > VE_MATH_PI_2_F)
	{
		y = VE_MATH_PI_F - y;
		sign = -1.0f;
	}
	else if (y < -VE_MATH_PI_2_F)
	{
		y = -VE_MATH_PI_F - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}
	VeFloat32 y2 = y * y;
	VeFloat32 p = ((-0.0012712436f * y2 + 0.041493919f) * y2 - 0.49992746f) * y2 + 1.0f;
	return sign*p;
}
//--------------------------------------------------------------------------
inline void VeScalarSinCos(
	VeFloat32* pSin, VeFloat32* pCos, VeFloat32  Value) noexcept
{
	VE_ASSERT(pSin);
	VE_ASSERT(pCos);
	VeFloat32 quotient = VE_MATH_1_2PI_F*Value;
	if (Value >= 0.0f)
	{
		quotient = (VeFloat32)((VeInt32)(quotient + 0.5f));
	}
	else
	{
		quotient = (VeFloat32)((VeInt32)(quotient - 0.5f));
	}
	VeFloat32 y = Value - VE_MATH_2PI_F*quotient;
	VeFloat32 sign;
	if (y > VE_MATH_PI_2_F)
	{
		y = VE_MATH_PI_F - y;
		sign = -1.0f;
	}
	else if (y < -VE_MATH_PI_2_F)
	{
		y = -VE_MATH_PI_F - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}
	VeFloat32 y2 = y * y;
	*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
	VeFloat32 p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*pCos = sign*p;
}
//--------------------------------------------------------------------------
inline void VeScalarSinCosEst(
	VeFloat32* pSin, VeFloat32* pCos, VeFloat32  Value) noexcept
{
	VE_ASSERT(pSin);
	VE_ASSERT(pCos);
	VeFloat32 quotient = VE_MATH_1_2PI_F*Value;
	if (Value >= 0.0f)
	{
		quotient = (VeFloat32)((VeInt32)(quotient + 0.5f));
	}
	else
	{
		quotient = (VeFloat32)((VeInt32)(quotient - 0.5f));
	}
	VeFloat32 y = Value - VE_MATH_2PI_F*quotient;
	VeFloat32 sign;
	if (y > VE_MATH_PI_2_F)
	{
		y = VE_MATH_PI_F - y;
		sign = -1.0f;
	}
	else if (y < -VE_MATH_PI_2_F)
	{
		y = -VE_MATH_PI_F - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}
	VeFloat32 y2 = y * y;
	*pSin = (((-0.00018524670f * y2 + 0.0083139502f) * y2 - 0.16665852f) * y2 + 1.0f) * y;
	VeFloat32 p = ((-0.0012712436f * y2 + 0.041493919f) * y2 - 0.49992746f) * y2 + 1.0f;
	*pCos = sign*p;
}
//--------------------------------------------------------------------------
inline VeFloat32 VeScalarASin(VeFloat32 Value) noexcept
{
	bool nonnegative = (Value >= 0.0f);
	VeFloat32 x = VeFabsf(Value);
	VeFloat32 omx = 1.0f - x;
	if (omx < 0.0f)
	{
		omx = 0.0f;
	}
	VeFloat32 root = VeSqrtf(omx);
	VeFloat32 result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
	result *= root;
	return (nonnegative ? VE_MATH_PI_2_F - result : result - VE_MATH_PI_2_F);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeScalarASinEst(VeFloat32 Value) noexcept
{
	bool nonnegative = (Value >= 0.0f);
	VeFloat32 x = VeFabsf(Value);
	VeFloat32 omx = 1.0f - x;
	if (omx < 0.0f)
	{
		omx = 0.0f;
	}
	VeFloat32 root = VeSqrtf(omx);
	VeFloat32 result = ((-0.0187293f*x + 0.0742610f)*x - 0.2121144f)*x + 1.5707288f;
	result *= root;
	return (nonnegative ? VE_MATH_PI_2_F - result : result - VE_MATH_PI_2_F);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeScalarACos(VeFloat32 Value) noexcept
{
	bool nonnegative = (Value >= 0.0f);
	VeFloat32 x = VeFabsf(Value);
	VeFloat32 omx = 1.0f - x;
	if (omx < 0.0f)
	{
		omx = 0.0f;
	}
	VeFloat32 root = VeSqrtf(omx);
	VeFloat32 result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
	result *= root;
	return (nonnegative ? result : VE_MATH_PI_F - result);
}
//--------------------------------------------------------------------------
inline VeFloat32 VeScalarACosEst(VeFloat32 Value) noexcept
{
	bool nonnegative = (Value >= 0.0f);
	VeFloat32 x = VeFabsf(Value);
	VeFloat32 omx = 1.0f - x;
	if (omx < 0.0f)
	{
		omx = 0.0f;
	}
	VeFloat32 root = VeSqrtf(omx);
	VeFloat32 result = ((-0.0187293f * x + 0.0742610f) * x - 0.2121144f) * x + 1.5707288f;
	result *= root;
	return (nonnegative ? result : VE_MATH_PI_F - result);
}
//--------------------------------------------------------------------------
