////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMathVector.inl
//  Created:     2015/08/21 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
#if defined(VE_NO_INTRINSICS)
//--------------------------------------------------------------------------
#define VEISNAN(x)  ((*(VeUInt32*)&(x) & 0x7F800000) == 0x7F800000 && (*(VeUInt32*)&(x) & 0x7FFFFF) != 0)
#define VEISINF(x)  ((*(VeUInt32*)&(x) & 0x7FFFFFFF) == 0x7F800000)
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
#if defined(VE_ENABLE_SSE)
//--------------------------------------------------------------------------
#define VE3UNPACK3INTO4(l1,l2,l3) \
    VE_VECTOR V3 = _mm_shuffle_ps(l2,l3,_MM_SHUFFLE(0,0,3,2));\
    VE_VECTOR V2 = _mm_shuffle_ps(l2,l1,_MM_SHUFFLE(3,3,1,0));\
    V2 = VE_PERMUTE_PS(V2,_MM_SHUFFLE(1,1,0,2));\
    VE_VECTOR V4 = _mm_castsi128_ps( _mm_srli_si128(_mm_castps_si128(L3),32/8) );
//--------------------------------------------------------------------------
#define VE3PACK4INTO3(v2x) \
    v2x = _mm_shuffle_ps(V2,V3,_MM_SHUFFLE(1,0,2,1));\
    V2 = _mm_shuffle_ps(V2,V1,_MM_SHUFFLE(2,2,0,0));\
    V1 = _mm_shuffle_ps(V1,V2,_MM_SHUFFLE(0,2,1,0));\
    V3 = _mm_shuffle_ps(V3,V4,_MM_SHUFFLE(0,0,2,2));\
    V3 = _mm_shuffle_ps(V3,V4,_MM_SHUFFLE(2,1,2,0));\
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorZero() noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult = { 0.0f,0.0f,0.0f,0.0f };
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return _mm_setzero_ps();
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSet(
	VeFloat32 x, VeFloat32 y, VeFloat32 z, VeFloat32 w) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTORF32 vResult = { x,y,z,w };
	return vResult.v;
#	elif defined(VE_ENABLE_SSE)
	return _mm_set_ps(w, z, y, x);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetInt(
	VeUInt32 x, VeUInt32 y, VeUInt32 z, VeUInt32 w) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTORU32 vResult = { x,y,z,w };
	return vResult.v;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_set_epi32(w, z, y, x);
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorReplicate(
	VeFloat32 Value) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult;
	vResult.vector4_f32[0] = Value;
	vResult.vector4_f32[1] = Value;
	vResult.vector4_f32[2] = Value;
	vResult.vector4_f32[3] = Value;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return _mm_set_ps1(Value);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorReplicatePtr(
	const VeFloat32* pValue) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 Value = pValue[0];
	VE_VECTOR vResult;
	vResult.vector4_f32[0] = Value;
	vResult.vector4_f32[1] = Value;
	vResult.vector4_f32[2] = Value;
	vResult.vector4_f32[3] = Value;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return _mm_load_ps1(pValue);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorReplicateInt(
	VeUInt32 Value) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTORU32 vResult;
	vResult.u[0] = Value;
	vResult.u[1] = Value;
	vResult.u[2] = Value;
	vResult.u[3] = Value;
	return vResult.v;
#	elif defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_set1_epi32(Value);
	return _mm_castsi128_ps(vTemp);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorReplicateIntPtr(
	const VeUInt32* pValue) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 Value = pValue[0];
	VE_VECTORU32 vResult;
	vResult.u[0] = Value;
	vResult.u[1] = Value;
	vResult.u[2] = Value;
	vResult.u[3] = Value;
	return vResult.v;
#	elif defined(VE_ENABLE_SSE)
	return _mm_load_ps1(reinterpret_cast<const VeFloat32*>(pValue));
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorTrueInt() noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTORU32 vResult = { 0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU,0xFFFFFFFFU };
	return vResult.v;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_set1_epi32(-1);
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorFalseInt() noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult = { 0.0f,0.0f,0.0f,0.0f };
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return _mm_setzero_ps();
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSplatX(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult;
	vResult.vector4_f32[0] = V.vector4_f32[0];
	vResult.vector4_f32[1] = V.vector4_f32[0];
	vResult.vector4_f32[2] = V.vector4_f32[0];
	vResult.vector4_f32[3] = V.vector4_f32[0];
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSplatY(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult;
	vResult.vector4_f32[0] = V.vector4_f32[1];
	vResult.vector4_f32[1] = V.vector4_f32[1];
	vResult.vector4_f32[2] = V.vector4_f32[1];
	vResult.vector4_f32[3] = V.vector4_f32[1];
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSplatZ(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult;
	vResult.vector4_f32[0] = V.vector4_f32[2];
	vResult.vector4_f32[1] = V.vector4_f32[2];
	vResult.vector4_f32[2] = V.vector4_f32[2];
	vResult.vector4_f32[3] = V.vector4_f32[2];
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSplatW(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult;
	vResult.vector4_f32[0] = V.vector4_f32[3];
	vResult.vector4_f32[1] = V.vector4_f32[3];
	vResult.vector4_f32[2] = V.vector4_f32[3];
	vResult.vector4_f32[3] = V.vector4_f32[3];
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
#	endif
}

//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSplatOne() noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult;
	vResult.vector4_f32[0] = 1.0f;
	vResult.vector4_f32[1] = 1.0f;
	vResult.vector4_f32[2] = 1.0f;
	vResult.vector4_f32[3] = 1.0f;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return g_MathOne;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSplatInfinity() noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult;
	vResult.vector4_u32[0] = 0x7F800000;
	vResult.vector4_u32[1] = 0x7F800000;
	vResult.vector4_u32[2] = 0x7F800000;
	vResult.vector4_u32[3] = 0x7F800000;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return g_MathInfinity;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSplatQNaN() noexcept
{
#if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult;
	vResult.vector4_u32[0] = 0x7FC00000;
	vResult.vector4_u32[1] = 0x7FC00000;
	vResult.vector4_u32[2] = 0x7FC00000;
	vResult.vector4_u32[3] = 0x7FC00000;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return g_MathQNaN;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSplatEpsilon() noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult;
	vResult.vector4_u32[0] = 0x34000000;
	vResult.vector4_u32[1] = 0x34000000;
	vResult.vector4_u32[2] = 0x34000000;
	vResult.vector4_u32[3] = 0x34000000;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	return g_MathEpsilon;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSplatSignMask() noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult;
	vResult.vector4_u32[0] = 0x80000000U;
	vResult.vector4_u32[1] = 0x80000000U;
	vResult.vector4_u32[2] = 0x80000000U;
	vResult.vector4_u32[3] = 0x80000000U;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_set1_epi32(0x80000000);
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VeFloat32 VE_MATH_CALLCONV VeVectorGetByIndex(
	VE_FVECTOR V, VeSizeT i) noexcept
{
	VE_ASSERT(i < 4);
#	if defined(VE_NO_INTRINSICS)
	return V.vector4_f32[i];
#	elif defined(VE_ENABLE_SSE)
	return V.m128_f32[i];
#	endif
}
//--------------------------------------------------------------------------
inline VeFloat32 VE_MATH_CALLCONV VeVectorGetX(VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return V.vector4_f32[0];
#	elif defined(VE_ENABLE_SSE)
	return _mm_cvtss_f32(V);
#	endif
}
//--------------------------------------------------------------------------
inline VeFloat32 VE_MATH_CALLCONV VeVectorGetY(VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return V.vector4_f32[1];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	return _mm_cvtss_f32(vTemp);
#	endif
}
//--------------------------------------------------------------------------
inline VeFloat32 VE_MATH_CALLCONV VeVectorGetZ(VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return V.vector4_f32[2];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	return _mm_cvtss_f32(vTemp);
#	endif
}
//--------------------------------------------------------------------------
inline VeFloat32 VE_MATH_CALLCONV VeVectorGetW(VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return V.vector4_f32[3];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
	return _mm_cvtss_f32(vTemp);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorGetByIndexPtr(
	VeFloat32* f, VE_FVECTOR V, VeSizeT i) noexcept
{
	VE_ASSERT(f != nullptr);
	VE_ASSERT(i < 4);
#	if defined(VE_NO_INTRINSICS)
	*f = V.vector4_f32[i];
#	elif defined(VE_ENABLE_SSE)
	*f = V.m128_f32[i];
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorGetXPtr(
	VeFloat32* x, VE_FVECTOR V) noexcept
{
	VE_ASSERT(x != nullptr);
#	if defined(VE_NO_INTRINSICS)
	*x = V.vector4_f32[0];
#	elif defined(VE_ENABLE_SSE)
	_mm_store_ss(x, V);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorGetYPtr(
	VeFloat32* y, VE_FVECTOR V) noexcept
{
	VE_ASSERT(y != nullptr);
#	if defined(VE_NO_INTRINSICS)
	*y = V.vector4_f32[1];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	_mm_store_ss(y, vResult);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorGetZPtr(
	VeFloat32* z, VE_FVECTOR V) noexcept
{
	VE_ASSERT(z != nullptr);
#	if defined(VE_NO_INTRINSICS)
	*z = V.vector4_f32[2];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(z, vResult);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorGetWPtr(
	VeFloat32* w, VE_FVECTOR V) noexcept
{
	VE_ASSERT(w != nullptr);
#	if defined(VE_NO_INTRINSICS)
	*w = V.vector4_f32[3];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
	_mm_store_ss(w, vResult);
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVectorGetIntByIndex(
	VE_FVECTOR V, VeSizeT i) noexcept
{
	VE_ASSERT(i < 4);
#	if defined(VE_NO_INTRINSICS)
	return V.vector4_u32[i];
#	elif defined(VE_ENABLE_SSE)
	return V.m128_u32[i];
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVectorGetIntX(VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return V.vector4_u32[0];
#	elif defined(VE_ENABLE_SSE)
	return static_cast<VeUInt32>(_mm_cvtsi128_si32(_mm_castps_si128(V)));
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVectorGetIntY(VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return V.vector4_u32[1];
#	elif defined(VE_ENABLE_SSE)
	__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(1, 1, 1, 1));
	return static_cast<VeUInt32>(_mm_cvtsi128_si32(vResulti));
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVectorGetIntZ(VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return V.vector4_u32[2];
#	elif defined(VE_ENABLE_SSE)
	__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(2, 2, 2, 2));
	return static_cast<VeUInt32>(_mm_cvtsi128_si32(vResulti));
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVectorGetIntW(VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return V.vector4_u32[3];
#	elif defined(VE_ENABLE_SSE)
	__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(3, 3, 3, 3));
	return static_cast<VeUInt32>(_mm_cvtsi128_si32(vResulti));
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorGetIntByIndexPtr(
	VeUInt32 *x, VE_FVECTOR V, VeSizeT i) noexcept
{
	VE_ASSERT(x != nullptr);
	VE_ASSERT(i < 4);
#	if defined(VE_NO_INTRINSICS)
	*x = V.vector4_u32[i];
#	elif defined(VE_ENABLE_SSE)
	*x = V.m128_u32[i];
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorGetIntXPtr(
	VeUInt32* x, VE_FVECTOR V) noexcept
{
	VE_ASSERT(x != nullptr);
#	if defined(VE_NO_INTRINSICS)
	*x = V.vector4_u32[0];
#	elif defined(VE_ENABLE_SSE)
	_mm_store_ss(reinterpret_cast<VeFloat32*>(x), V);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorGetIntYPtr(
	VeUInt32 *y, VE_FVECTOR V) noexcept
{
	VE_ASSERT(y != nullptr);
#	if defined(VE_NO_INTRINSICS)
	*y = V.vector4_u32[1];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	_mm_store_ss(reinterpret_cast<VeFloat32*>(y), vResult);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorGetIntZPtr(
	VeUInt32 *z, VE_FVECTOR V) noexcept
{
	VE_ASSERT(z != nullptr);
#	if defined(VE_NO_INTRINSICS)
	*z = V.vector4_u32[2];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(reinterpret_cast<VeFloat32*>(z), vResult);
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorGetIntWPtr(
	VeUInt32 *w, VE_FVECTOR V) noexcept
{
	VE_ASSERT(w != nullptr);
#	if defined(VE_NO_INTRINSICS)
	*w = V.vector4_u32[3];
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
	_mm_store_ss(reinterpret_cast<VeFloat32*>(w), vResult);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetByIndex(
	VE_FVECTOR V, VeFloat32 f, VeSizeT i) noexcept
{
	VE_ASSERT(i < 4);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U = V;
	U.vector4_f32[i] = f;
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR U = V;
	U.m128_f32[i] = f;
	return U;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetX(
	VE_FVECTOR V, VeFloat32 x) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_f32[0] = x;
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_set_ss(x);
	vResult = _mm_move_ss(V, vResult);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetY(
	VE_FVECTOR V, VeFloat32 y) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = y;
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
	VE_VECTOR vTemp = _mm_set_ss(y);
	vResult = _mm_move_ss(vResult, vTemp);
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetZ(
	VE_FVECTOR V, VeFloat32 z) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = z;
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
	VE_VECTOR vTemp = _mm_set_ss(z);
	vResult = _mm_move_ss(vResult, vTemp);
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetW(
	VE_FVECTOR V, VeFloat32 w) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = w;
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
	VE_VECTOR vTemp = _mm_set_ss(w);
	vResult = _mm_move_ss(vResult, vTemp);
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetByIndexPtr(
	VE_FVECTOR V, const VeFloat32* f, VeSizeT i) noexcept
{
	VE_ASSERT(f != nullptr);
	VE_ASSERT(i < 4);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U = V;
	U.vector4_f32[i] = *f;
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR U = V;
	U.m128_f32[i] = *f;
	return U;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetXPtr(
	VE_FVECTOR V, const VeFloat32* x) noexcept
{
	VE_ASSERT(x != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_f32[0] = *x;
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_load_ss(x);
	vResult = _mm_move_ss(V, vResult);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetYPtr(
	VE_FVECTOR V, const VeFloat32* y) noexcept
{
	VE_ASSERT(y != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = *y;
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
	VE_VECTOR vTemp = _mm_load_ss(y);
	vResult = _mm_move_ss(vResult, vTemp);
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetZPtr(
	VE_FVECTOR V, const VeFloat32* z) noexcept
{
	VE_ASSERT(z != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = *z;
	U.vector4_f32[3] = V.vector4_f32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
	VE_VECTOR vTemp = _mm_load_ss(z);
	vResult = _mm_move_ss(vResult, vTemp);
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetWPtr(
	VE_FVECTOR V, const VeFloat32* w) noexcept
{
	VE_ASSERT(w != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_f32[0] = V.vector4_f32[0];
	U.vector4_f32[1] = V.vector4_f32[1];
	U.vector4_f32[2] = V.vector4_f32[2];
	U.vector4_f32[3] = *w;
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
	VE_VECTOR vTemp = _mm_load_ss(w);
	vResult = _mm_move_ss(vResult, vTemp);
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetIntByIndex(
	VE_FVECTOR V, VeUInt32 x, VeSizeT i) noexcept
{
	VE_ASSERT(i < 4);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U = V;
	U.vector4_u32[i] = x;
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTORU32 tmp;
	tmp.v = V;
	tmp.u[i] = x;
	return tmp;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetIntX(
	VE_FVECTOR V, VeUInt32 x) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_u32[0] = x;
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_cvtsi32_si128(x);
	VE_VECTOR vResult = _mm_move_ss(V, _mm_castsi128_ps(vTemp));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetIntY(
	VE_FVECTOR V, VeUInt32 y) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = y;
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
	__m128i vTemp = _mm_cvtsi32_si128(y);
	vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetIntZ(
	VE_FVECTOR V, VeUInt32 z) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = z;
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
	__m128i vTemp = _mm_cvtsi32_si128(z);
	vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetIntW(
	VE_FVECTOR V, VeUInt32 w) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = w;
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
	__m128i vTemp = _mm_cvtsi32_si128(w);
	vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetIntByIndexPtr(
	VE_FVECTOR V, const VeUInt32 *x, VeSizeT i) noexcept
{
	VE_ASSERT(x != nullptr);
	VE_ASSERT(i < 4);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U = V;
	U.vector4_u32[i] = *x;
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTORU32 tmp;
	tmp.v = V;
	tmp.u[i] = *x;
	return tmp;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetIntXPtr(
	VE_FVECTOR V, const VeUInt32 *x) noexcept
{
	VE_ASSERT(x != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_u32[0] = *x;
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_load_ss(reinterpret_cast<const VeFloat32 *>(x));
	VE_VECTOR vResult = _mm_move_ss(V, vTemp);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetIntYPtr(
	VE_FVECTOR V, const VeUInt32 *y) noexcept
{
	VE_ASSERT(y != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = *y;
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
	VE_VECTOR vTemp = _mm_load_ss(reinterpret_cast<const VeFloat32*>(y));
	vResult = _mm_move_ss(vResult, vTemp);
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetIntZPtr(
	VE_FVECTOR V, const VeUInt32 *z) noexcept
{
	VE_ASSERT(z != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = *z;
	U.vector4_u32[3] = V.vector4_u32[3];
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
	VE_VECTOR vTemp = _mm_load_ss(reinterpret_cast<const VeFloat32 *>(z));
	vResult = _mm_move_ss(vResult, vTemp);
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSetIntWPtr(
	VE_FVECTOR V, const VeUInt32 *w) noexcept
{
	VE_ASSERT(w != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR U;
	U.vector4_u32[0] = V.vector4_u32[0];
	U.vector4_u32[1] = V.vector4_u32[1];
	U.vector4_u32[2] = V.vector4_u32[2];
	U.vector4_u32[3] = *w;
	return U;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
	VE_VECTOR vTemp = _mm_load_ss(reinterpret_cast<const VeFloat32 *>(w));
	vResult = _mm_move_ss(vResult, vTemp);
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSwizzle(VE_FVECTOR V,
	VeUInt32 E0, VeUInt32 E1, VeUInt32 E2, VeUInt32 E3) noexcept
{
	VE_ASSERT((E0 < 4) && (E1 < 4) && (E2 < 4) && (E3 < 4));
#	if defined(VE_NO_INTRINSICS)

	VE_VECTOR Result = { V.vector4_f32[E0],
		V.vector4_f32[E1],
		V.vector4_f32[E2],
		V.vector4_f32[E3] };
	return Result;
#	else
	const VeUInt32 *aPtr = (const VeUInt32*)(&V);
	VE_VECTOR Result;
	VeUInt32 *pWork = (VeUInt32*)(&Result);
	pWork[0] = aPtr[E0];
	pWork[1] = aPtr[E1];
	pWork[2] = aPtr[E2];
	pWork[3] = aPtr[E3];
	return Result;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorPermute(VE_FVECTOR V1,
	VE_FVECTOR V2, VeUInt32 PermuteX, VeUInt32 PermuteY, VeUInt32 PermuteZ,
	VeUInt32 PermuteW) noexcept
{
	VE_ASSERT(PermuteX <= 7 && PermuteY <= 7 && PermuteZ <= 7 && PermuteW <= 7);
	const VeUInt32 *aPtr[2];
	aPtr[0] = (const VeUInt32*)(&V1);
	aPtr[1] = (const VeUInt32*)(&V2);

	VE_VECTOR Result;
	VeUInt32 *pWork = (VeUInt32*)(&Result);

	const VeUInt32 i0 = PermuteX & 3;
	const VeUInt32 vi0 = PermuteX >> 2;
	pWork[0] = aPtr[vi0][i0];

	const VeUInt32 i1 = PermuteY & 3;
	const VeUInt32 vi1 = PermuteY >> 2;
	pWork[1] = aPtr[vi1][i1];

	const VeUInt32 i2 = PermuteZ & 3;
	const VeUInt32 vi2 = PermuteZ >> 2;
	pWork[2] = aPtr[vi2][i2];

	const VeUInt32 i3 = PermuteW & 3;
	const VeUInt32 vi3 = PermuteW >> 2;
	pWork[3] = aPtr[vi3][i3];

	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSelectControl(
	VeUInt32 VectorIndex0, VeUInt32 VectorIndex1, VeUInt32 VectorIndex2,
	VeUInt32 VectorIndex3) noexcept
{
#	if defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_set_epi32(VectorIndex3, VectorIndex2, VectorIndex1, VectorIndex0);
	vTemp = _mm_cmpgt_epi32(vTemp, g_MathZero);
	return _mm_castsi128_ps(vTemp);
#	else
	VE_VECTOR    ControlVector;
	const VeUInt32  ControlElement[] =
	{
		VE_SELECT_0,
		VE_SELECT_1
	};
	VE_ASSERT(VectorIndex0 < 2);
	VE_ASSERT(VectorIndex1 < 2);
	VE_ASSERT(VectorIndex2 < 2);
	VE_ASSERT(VectorIndex3 < 2);
	ControlVector.vector4_u32[0] = ControlElement[VectorIndex0];
	ControlVector.vector4_u32[1] = ControlElement[VectorIndex1];
	ControlVector.vector4_u32[2] = ControlElement[VectorIndex2];
	ControlVector.vector4_u32[3] = ControlElement[VectorIndex3];
	return ControlVector;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSelect(
	VE_FVECTOR V1, VE_FVECTOR V2, VE_FVECTOR Control) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_u32[0] = (V1.vector4_u32[0] & ~Control.vector4_u32[0]) | (V2.vector4_u32[0] & Control.vector4_u32[0]);
	Result.vector4_u32[1] = (V1.vector4_u32[1] & ~Control.vector4_u32[1]) | (V2.vector4_u32[1] & Control.vector4_u32[1]);
	Result.vector4_u32[2] = (V1.vector4_u32[2] & ~Control.vector4_u32[2]) | (V2.vector4_u32[2] & Control.vector4_u32[2]);
	Result.vector4_u32[3] = (V1.vector4_u32[3] & ~Control.vector4_u32[3]) | (V2.vector4_u32[3] & Control.vector4_u32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp1 = _mm_andnot_ps(Control, V1);
	VE_VECTOR vTemp2 = _mm_and_ps(V2, Control);
	return _mm_or_ps(vTemp1, vTemp2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorMergeXY(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[0];
	Result.vector4_u32[1] = V2.vector4_u32[0];
	Result.vector4_u32[2] = V1.vector4_u32[1];
	Result.vector4_u32[3] = V2.vector4_u32[1];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_unpacklo_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorMergeZW(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[2];
	Result.vector4_u32[1] = V2.vector4_u32[2];
	Result.vector4_u32[2] = V1.vector4_u32[3];
	Result.vector4_u32[3] = V2.vector4_u32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_unpackhi_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorShiftLeft(
	VE_FVECTOR V1, VE_FVECTOR V2, VeUInt32 Elements) noexcept
{
	VE_ASSERT(Elements < 4);
	return VeVectorPermute(V1, V2, Elements, ((Elements)+1), ((Elements)+2), ((Elements)+3));
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorRotateLeft(
	VE_FVECTOR V, VeUInt32 Elements) noexcept
{
	VE_ASSERT(Elements < 4);
	return VeVectorSwizzle(V, Elements & 3, (Elements + 1) & 3, (Elements + 2) & 3, (Elements + 3) & 3);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorRotateRight(
	VE_FVECTOR V, VeUInt32 Elements) noexcept
{
	VE_ASSERT(Elements < 4);
	return VeVectorSwizzle(V, (4 - (Elements)) & 3, (5 - (Elements)) & 3, (6 - (Elements)) & 3, (7 - (Elements)) & 3);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorInsert(VE_FVECTOR VD,
	VE_FVECTOR VS, VeUInt32 VSLeftRotateElements, VeUInt32 Select0,
	VeUInt32 Select1, VeUInt32 Select2, VeUInt32 Select3) noexcept
{
	VE_VECTOR Control = VeVectorSelectControl(Select0 & 1, Select1 & 1, Select2 & 1, Select3 & 1);
	return VeVectorSelect(VD, VeVectorRotateLeft(VS, VSLeftRotateElements), Control);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] == V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] == V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] == V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] == V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	return _mm_cmpeq_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorEqualR(
	VeUInt32* pCR, VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	VE_ASSERT(pCR != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 ux = (V1.vector4_f32[0] == V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uy = (V1.vector4_f32[1] == V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uz = (V1.vector4_f32[2] == V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uw = (V1.vector4_f32[3] == V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	VeUInt32 CR = 0;
	if (ux&uy&uz&uw)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!(ux | uy | uz | uw))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	VE_VECTOR Control;
	Control.vector4_u32[0] = ux;
	Control.vector4_u32[1] = uy;
	Control.vector4_u32[2] = uz;
	Control.vector4_u32[3] = uw;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpeq_ps(V1, V2);
	VeUInt32 CR = 0;
	int iTest = _mm_movemask_ps(vTemp);
	if (iTest == 0xf)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return vTemp;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorEqualInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_u32[0] == V2.vector4_u32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_u32[1] == V2.vector4_u32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_u32[2] == V2.vector4_u32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_u32[3] == V2.vector4_u32[3]) ? 0xFFFFFFFF : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4EqualInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorEqualIntR(
	VeUInt32* pCR, VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	VE_ASSERT(pCR != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control = VeVectorEqualInt(V1, V2);
	*pCR = 0;
	if (VeVector4EqualInt(Control, VeVectorTrueInt()))
	{
		*pCR |= VE_CRMASK_CR6TRUE;
	}
	else if (VeVector4EqualInt(Control, VeVectorFalseInt()))
	{
		*pCR |= VE_CRMASK_CR6FALSE;
	}
	return Control;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	int iTemp = _mm_movemask_ps(_mm_castsi128_ps(V));
	VeUInt32 CR = 0;
	if (iTemp == 0x0F)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTemp)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorNearEqual(
	VE_FVECTOR V1, VE_FVECTOR V2, VE_FVECTOR Epsilon) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fDeltax = V1.vector4_f32[0] - V2.vector4_f32[0];
	VeFloat32 fDeltay = V1.vector4_f32[1] - V2.vector4_f32[1];
	VeFloat32 fDeltaz = V1.vector4_f32[2] - V2.vector4_f32[2];
	VeFloat32 fDeltaw = V1.vector4_f32[3] - V2.vector4_f32[3];
	fDeltax = VeFabsf(fDeltax);
	fDeltay = VeFabsf(fDeltay);
	fDeltaz = VeFabsf(fDeltaz);
	fDeltaw = VeFabsf(fDeltaw);
	VE_VECTOR Control;
	Control.vector4_u32[0] = (fDeltax <= Epsilon.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[1] = (fDeltay <= Epsilon.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[2] = (fDeltaz <= Epsilon.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[3] = (fDeltaw <= Epsilon.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vDelta = _mm_sub_ps(V1, V2);
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_sub_ps(vTemp, vDelta);
	vTemp = _mm_max_ps(vTemp, vDelta);
	vTemp = _mm_cmple_ps(vTemp, Epsilon);
	return vTemp;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorNotEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] != V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] != V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] != V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] != V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	return _mm_cmpneq_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorNotEqualInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_u32[0] != V2.vector4_u32[0]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[1] = (V1.vector4_u32[1] != V2.vector4_u32[1]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[2] = (V1.vector4_u32[2] != V2.vector4_u32[2]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[3] = (V1.vector4_u32[3] != V2.vector4_u32[3]) ? 0xFFFFFFFFU : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return _mm_xor_ps(_mm_castsi128_ps(V), g_MathNegOneMask);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorGreater(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] > V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] > V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] > V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] > V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	return _mm_cmpgt_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorGreaterR(
	VeUInt32* pCR, VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	VE_ASSERT(pCR != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 ux = (V1.vector4_f32[0] > V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uy = (V1.vector4_f32[1] > V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uz = (V1.vector4_f32[2] > V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uw = (V1.vector4_f32[3] > V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	VeUInt32 CR = 0;
	if (ux&uy&uz&uw)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!(ux | uy | uz | uw))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	VE_VECTOR Control;
	Control.vector4_u32[0] = ux;
	Control.vector4_u32[1] = uy;
	Control.vector4_u32[2] = uz;
	Control.vector4_u32[3] = uw;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpgt_ps(V1, V2);
	VeUInt32 CR = 0;
	VeInt32 iTest = _mm_movemask_ps(vTemp);
	if (iTest == 0xf)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return vTemp;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorGreaterOrEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] >= V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] >= V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] >= V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] >= V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	return _mm_cmpge_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorGreaterOrEqualR(
	VeUInt32* pCR, VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	VE_ASSERT(pCR != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 ux = (V1.vector4_f32[0] >= V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uy = (V1.vector4_f32[1] >= V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uz = (V1.vector4_f32[2] >= V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uw = (V1.vector4_f32[3] >= V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	VeUInt32 CR = 0;
	if (ux&uy&uz&uw)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!(ux | uy | uz | uw))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	VE_VECTOR Control;
	Control.vector4_u32[0] = ux;
	Control.vector4_u32[1] = uy;
	Control.vector4_u32[2] = uz;
	Control.vector4_u32[3] = uw;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpge_ps(V1, V2);
	VeUInt32 CR = 0;
	int iTest = _mm_movemask_ps(vTemp);
	if (iTest == 0xf)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	*pCR = CR;
	return vTemp;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorLess(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] < V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] < V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] < V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] < V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	return _mm_cmplt_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorLessOrEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control;
	Control.vector4_u32[0] = (V1.vector4_f32[0] <= V2.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V1.vector4_f32[1] <= V2.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V1.vector4_f32[2] <= V2.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V1.vector4_f32[3] <= V2.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	return _mm_cmple_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorInBounds(
	VE_FVECTOR V, VE_FVECTOR Bounds) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control;
	Control.vector4_u32[0] = (V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[1] = (V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[2] = (V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2]) ? 0xFFFFFFFF : 0;
	Control.vector4_u32[3] = (V.vector4_f32[3] <= Bounds.vector4_f32[3] && V.vector4_f32[3] >= -Bounds.vector4_f32[3]) ? 0xFFFFFFFF : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp1 = _mm_cmple_ps(V, Bounds);
	VE_VECTOR vTemp2 = _mm_mul_ps(Bounds, g_MathNegativeOne);
	vTemp2 = _mm_cmple_ps(vTemp2, V);
	vTemp1 = _mm_and_ps(vTemp1, vTemp2);
	return vTemp1;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorInBoundsR(
	VeUInt32* pCR, VE_FVECTOR V, VE_FVECTOR Bounds) noexcept
{
	VE_ASSERT(pCR != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 ux = (V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uy = (V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uz = (V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	VeUInt32 uw = (V.vector4_f32[3] <= Bounds.vector4_f32[3] && V.vector4_f32[3] >= -Bounds.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	VeUInt32 CR = 0;
	if (ux&uy&uz&uw)
	{
		CR = VE_CRMASK_CR6BOUNDS;
	}
	*pCR = CR;
	VE_VECTOR Control;
	Control.vector4_u32[0] = ux;
	Control.vector4_u32[1] = uy;
	Control.vector4_u32[2] = uz;
	Control.vector4_u32[3] = uw;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp1 = _mm_cmple_ps(V, Bounds);
	VE_VECTOR vTemp2 = _mm_mul_ps(Bounds, g_MathNegativeOne);
	vTemp2 = _mm_cmple_ps(vTemp2, V);
	vTemp1 = _mm_and_ps(vTemp1, vTemp2);
	VeUInt32 CR = 0;
	if (_mm_movemask_ps(vTemp1) == 0xf)
	{
		CR = VE_CRMASK_CR6BOUNDS;
	}
	*pCR = CR;
	return vTemp1;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorIsNaN(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control;
	Control.vector4_u32[0] = VEISNAN(V.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[1] = VEISNAN(V.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[2] = VEISNAN(V.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[3] = VEISNAN(V.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	return _mm_cmpneq_ps(V, V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorIsInfinite(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control;
	Control.vector4_u32[0] = VEISINF(V.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[1] = VEISINF(V.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[2] = VEISINF(V.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
	Control.vector4_u32[3] = VEISINF(V.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
	return Control;
#	elif defined(VE_ENABLE_SSE)
	__m128 vTemp = _mm_and_ps(V, g_MathAbsMask);
	vTemp = _mm_cmpeq_ps(vTemp, g_MathInfinity);
	return vTemp;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorMin(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = (V1.vector4_f32[0] < V2.vector4_f32[0]) ? V1.vector4_f32[0] : V2.vector4_f32[0];
	Result.vector4_f32[1] = (V1.vector4_f32[1] < V2.vector4_f32[1]) ? V1.vector4_f32[1] : V2.vector4_f32[1];
	Result.vector4_f32[2] = (V1.vector4_f32[2] < V2.vector4_f32[2]) ? V1.vector4_f32[2] : V2.vector4_f32[2];
	Result.vector4_f32[3] = (V1.vector4_f32[3] < V2.vector4_f32[3]) ? V1.vector4_f32[3] : V2.vector4_f32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_min_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorMax(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = (V1.vector4_f32[0] > V2.vector4_f32[0]) ? V1.vector4_f32[0] : V2.vector4_f32[0];
	Result.vector4_f32[1] = (V1.vector4_f32[1] > V2.vector4_f32[1]) ? V1.vector4_f32[1] : V2.vector4_f32[1];
	Result.vector4_f32[2] = (V1.vector4_f32[2] > V2.vector4_f32[2]) ? V1.vector4_f32[2] : V2.vector4_f32[2];
	Result.vector4_f32[3] = (V1.vector4_f32[3] > V2.vector4_f32[3]) ? V1.vector4_f32[3] : V2.vector4_f32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_max_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorRound(
	VE_FVECTOR V) noexcept
{
#if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeRoundf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeRoundf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeRoundf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeRoundf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128 sign = _mm_and_ps(V, g_MathNegativeZero);
	__m128 sMagic = _mm_or_ps(g_MathNoFraction, sign);
	__m128 R1 = _mm_add_ps(V, sMagic);
	R1 = _mm_sub_ps(R1, sMagic);
	__m128 R2 = _mm_and_ps(V, g_MathAbsMask);
	__m128 mask = _mm_cmple_ps(R2, g_MathNoFraction);
	R2 = _mm_andnot_ps(mask, V);
	R1 = _mm_and_ps(R1, mask);
	VE_VECTOR vResult = _mm_xor_ps(R1, R2);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorTruncate(VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	VeUInt32 i;
	Result.vector4_f32[0] = 0.0f;
	for (i = 0; i < 4; i++)
	{
		if (VEISNAN(V.vector4_f32[i]))
		{
			Result.vector4_u32[i] = 0x7FC00000;
		}
		else if (VeFabsf(V.vector4_f32[i]) < 8388608.0f)
		{
			Result.vector4_f32[i] = (VeFloat32)((VeInt32)V.vector4_f32[i]);
		}
		else
		{
			Result.vector4_f32[i] = V.vector4_f32[i];
		}
	}
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128i vTest = _mm_and_si128(_mm_castps_si128(V), g_MathAbsMask);
	vTest = _mm_cmplt_epi32(vTest, g_MathNoFraction);
	__m128i vInt = _mm_cvttps_epi32(V);
	VE_VECTOR vResult = _mm_cvtepi32_ps(vInt);
	vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
	vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
	vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorFloor(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeFloorf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeFloorf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeFloorf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeFloorf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128i vTest = _mm_and_si128(_mm_castps_si128(V), g_MathAbsMask);
	vTest = _mm_cmplt_epi32(vTest, g_MathNoFraction);
	__m128i vInt = _mm_cvttps_epi32(V);
	VE_VECTOR vResult = _mm_cvtepi32_ps(vInt);
	__m128 vLarger = _mm_cmpgt_ps(vResult, V);
	vLarger = _mm_cvtepi32_ps(_mm_castps_si128(vLarger));
	vResult = _mm_add_ps(vResult, vLarger);
	vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
	vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
	vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorCeiling(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = ceilf(V.vector4_f32[0]);
	Result.vector4_f32[1] = ceilf(V.vector4_f32[1]);
	Result.vector4_f32[2] = ceilf(V.vector4_f32[2]);
	Result.vector4_f32[3] = ceilf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128i vTest = _mm_and_si128(_mm_castps_si128(V), g_MathAbsMask);
	vTest = _mm_cmplt_epi32(vTest, g_MathNoFraction);
	__m128i vInt = _mm_cvttps_epi32(V);
	VE_VECTOR vResult = _mm_cvtepi32_ps(vInt);
	__m128 vSmaller = _mm_cmplt_ps(vResult, V);
	vSmaller = _mm_cvtepi32_ps(_mm_castps_si128(vSmaller));
	vResult = _mm_sub_ps(vResult, vSmaller);
	vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
	vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
	vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4LessOrEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorClamp(
	VE_FVECTOR V, VE_FVECTOR Min, VE_FVECTOR Max) noexcept
{
	VE_ASSERT(VeVector4LessOrEqual(Min, Max));
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVectorMax(Min, V);
	Result = VeVectorMin(Max, Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult;
	vResult = _mm_max_ps(Min, V);
	vResult = _mm_min_ps(vResult, Max);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSaturate(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	const VE_VECTOR Zero = VeVectorZero();
	return VeVectorClamp(V, Zero, g_MathOne.v);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_max_ps(V, g_MathZero);
	return _mm_min_ps(vResult, g_MathOne);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorAndInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[0] & V2.vector4_u32[0];
	Result.vector4_u32[1] = V1.vector4_u32[1] & V2.vector4_u32[1];
	Result.vector4_u32[2] = V1.vector4_u32[2] & V2.vector4_u32[2];
	Result.vector4_u32[3] = V1.vector4_u32[3] & V2.vector4_u32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_and_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorAndCInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[0] & ~V2.vector4_u32[0];
	Result.vector4_u32[1] = V1.vector4_u32[1] & ~V2.vector4_u32[1];
	Result.vector4_u32[2] = V1.vector4_u32[2] & ~V2.vector4_u32[2];
	Result.vector4_u32[3] = V1.vector4_u32[3] & ~V2.vector4_u32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_andnot_si128(_mm_castps_si128(V2), _mm_castps_si128(V1));
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorOrInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[0] | V2.vector4_u32[0];
	Result.vector4_u32[1] = V1.vector4_u32[1] | V2.vector4_u32[1];
	Result.vector4_u32[2] = V1.vector4_u32[2] | V2.vector4_u32[2];
	Result.vector4_u32[3] = V1.vector4_u32[3] | V2.vector4_u32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_or_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorNorInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_u32[0] = ~(V1.vector4_u32[0] | V2.vector4_u32[0]);
	Result.vector4_u32[1] = ~(V1.vector4_u32[1] | V2.vector4_u32[1]);
	Result.vector4_u32[2] = ~(V1.vector4_u32[2] | V2.vector4_u32[2]);
	Result.vector4_u32[3] = ~(V1.vector4_u32[3] | V2.vector4_u32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128i Result;
	Result = _mm_or_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
	Result = _mm_andnot_si128(Result, g_MathNegOneMask);
	return _mm_castsi128_ps(Result);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorXorInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_u32[0] = V1.vector4_u32[0] ^ V2.vector4_u32[0];
	Result.vector4_u32[1] = V1.vector4_u32[1] ^ V2.vector4_u32[1];
	Result.vector4_u32[2] = V1.vector4_u32[2] ^ V2.vector4_u32[2];
	Result.vector4_u32[3] = V1.vector4_u32[3] ^ V2.vector4_u32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128i V = _mm_xor_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return _mm_castsi128_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorNegate(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = -V.vector4_f32[0];
	Result.vector4_f32[1] = -V.vector4_f32[1];
	Result.vector4_f32[2] = -V.vector4_f32[2];
	Result.vector4_f32[3] = -V.vector4_f32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR Z;
	Z = _mm_setzero_ps();
	return _mm_sub_ps(Z, V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorAdd(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = V1.vector4_f32[0] + V2.vector4_f32[0];
	Result.vector4_f32[1] = V1.vector4_f32[1] + V2.vector4_f32[1];
	Result.vector4_f32[2] = V1.vector4_f32[2] + V2.vector4_f32[2];
	Result.vector4_f32[3] = V1.vector4_f32[3] + V2.vector4_f32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_add_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorAddAngles(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	const VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR Result = VeVectorAdd(V1, V2);
	VE_VECTOR Mask = VeVectorLess(Result, g_MathNegativePi.v);
	VE_VECTOR Offset = VeVectorSelect(Zero, g_MathTwoPi.v, Mask);
	Mask = VeVectorGreaterOrEqual(Result, g_MathPi.v);
	Offset = VeVectorSelect(Offset, g_MathNegativeTwoPi.v, Mask);
	Result = VeVectorAdd(Result, Offset);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_add_ps(V1, V2);
	VE_VECTOR vOffset = _mm_cmplt_ps(vResult, g_MathNegativePi);
	vOffset = _mm_and_ps(vOffset, g_MathTwoPi);
	vResult = _mm_add_ps(vResult, vOffset);
	vOffset = _mm_cmpge_ps(vResult, g_MathPi);
	vOffset = _mm_and_ps(vOffset, g_MathTwoPi);
	vResult = _mm_sub_ps(vResult, vOffset);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSubtract(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = V1.vector4_f32[0] - V2.vector4_f32[0];
	Result.vector4_f32[1] = V1.vector4_f32[1] - V2.vector4_f32[1];
	Result.vector4_f32[2] = V1.vector4_f32[2] - V2.vector4_f32[2];
	Result.vector4_f32[3] = V1.vector4_f32[3] - V2.vector4_f32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_sub_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSubtractAngles(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	const VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR Result = VeVectorSubtract(V1, V2);
	VE_VECTOR Mask = VeVectorLess(Result, g_MathNegativePi.v);
	VE_VECTOR Offset = VeVectorSelect(Zero, g_MathTwoPi.v, Mask);
	Mask = VeVectorGreaterOrEqual(Result, g_MathPi.v);
	Offset = VeVectorSelect(Offset, g_MathNegativeTwoPi.v, Mask);
	Result = VeVectorAdd(Result, Offset);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_sub_ps(V1, V2);
	VE_VECTOR vOffset = _mm_cmplt_ps(vResult, g_MathNegativePi);
	vOffset = _mm_and_ps(vOffset, g_MathTwoPi);
	vResult = _mm_add_ps(vResult, vOffset);
	vOffset = _mm_cmpge_ps(vResult, g_MathPi);
	vOffset = _mm_and_ps(vOffset, g_MathTwoPi);
	vResult = _mm_sub_ps(vResult, vOffset);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorMultiply(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = V1.vector4_f32[0] * V2.vector4_f32[0];
	Result.vector4_f32[1] = V1.vector4_f32[1] * V2.vector4_f32[1];
	Result.vector4_f32[2] = V1.vector4_f32[2] * V2.vector4_f32[2];
	Result.vector4_f32[3] = V1.vector4_f32[3] * V2.vector4_f32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_mul_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorMultiplyAdd(
	VE_FVECTOR V1, VE_FVECTOR V2, VE_FVECTOR V3) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = V1.vector4_f32[0] * V2.vector4_f32[0] + V3.vector4_f32[0];
	Result.vector4_f32[1] = V1.vector4_f32[1] * V2.vector4_f32[1] + V3.vector4_f32[1];
	Result.vector4_f32[2] = V1.vector4_f32[2] * V2.vector4_f32[2] + V3.vector4_f32[2];
	Result.vector4_f32[3] = V1.vector4_f32[3] * V2.vector4_f32[3] + V3.vector4_f32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_mul_ps(V1, V2);
	return _mm_add_ps(vResult, V3);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorDivide(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = V1.vector4_f32[0] / V2.vector4_f32[0];
	Result.vector4_f32[1] = V1.vector4_f32[1] / V2.vector4_f32[1];
	Result.vector4_f32[2] = V1.vector4_f32[2] / V2.vector4_f32[2];
	Result.vector4_f32[3] = V1.vector4_f32[3] / V2.vector4_f32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_div_ps(V1, V2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorNegativeMultiplySubtract(
	VE_FVECTOR V1, VE_FVECTOR V2, VE_FVECTOR V3) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = V3.vector4_f32[0] - (V1.vector4_f32[0] * V2.vector4_f32[0]);
	Result.vector4_f32[1] = V3.vector4_f32[1] - (V1.vector4_f32[1] * V2.vector4_f32[1]);
	Result.vector4_f32[2] = V3.vector4_f32[2] - (V1.vector4_f32[2] * V2.vector4_f32[2]);
	Result.vector4_f32[3] = V3.vector4_f32[3] - (V1.vector4_f32[3] * V2.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR R = _mm_mul_ps(V1, V2);
	return _mm_sub_ps(V3, R);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorScale(
	VE_FVECTOR V, VeFloat32 ScaleFactor) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = V.vector4_f32[0] * ScaleFactor;
	Result.vector4_f32[1] = V.vector4_f32[1] * ScaleFactor;
	Result.vector4_f32[2] = V.vector4_f32[2] * ScaleFactor;
	Result.vector4_f32[3] = V.vector4_f32[3] * ScaleFactor;
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_set_ps1(ScaleFactor);
	return _mm_mul_ps(vResult, V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorReciprocalEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = 1.f / V.vector4_f32[0];
	Result.vector4_f32[1] = 1.f / V.vector4_f32[1];
	Result.vector4_f32[2] = 1.f / V.vector4_f32[2];
	Result.vector4_f32[3] = 1.f / V.vector4_f32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_rcp_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorReciprocal(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = 1.f / V.vector4_f32[0];
	Result.vector4_f32[1] = 1.f / V.vector4_f32[1];
	Result.vector4_f32[2] = 1.f / V.vector4_f32[2];
	Result.vector4_f32[3] = 1.f / V.vector4_f32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_div_ps(g_MathOne, V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSqrtEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeSqrtf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeSqrtf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeSqrtf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeSqrtf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_sqrt_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSqrt(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeSqrtf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeSqrtf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeSqrtf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeSqrtf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_sqrt_ps(V);
#	endif
}

//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorReciprocalSqrtEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = 1.f / VeSqrtf(V.vector4_f32[0]);
	Result.vector4_f32[1] = 1.f / VeSqrtf(V.vector4_f32[1]);
	Result.vector4_f32[2] = 1.f / VeSqrtf(V.vector4_f32[2]);
	Result.vector4_f32[3] = 1.f / VeSqrtf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	return _mm_rsqrt_ps(V);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorReciprocalSqrt(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = 1.f / VeSqrtf(V.vector4_f32[0]);
	Result.vector4_f32[1] = 1.f / VeSqrtf(V.vector4_f32[1]);
	Result.vector4_f32[2] = 1.f / VeSqrtf(V.vector4_f32[2]);
	Result.vector4_f32[3] = 1.f / VeSqrtf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_sqrt_ps(V);
	vResult = _mm_div_ps(g_MathOne, vResult);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorExp2(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VePowf(2.0f, V.vector4_f32[0]);
	Result.vector4_f32[1] = VePowf(2.0f, V.vector4_f32[1]);
	Result.vector4_f32[2] = VePowf(2.0f, V.vector4_f32[2]);
	Result.vector4_f32[3] = VePowf(2.0f, V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128i itrunc = _mm_cvttps_epi32(V);
	__m128 ftrunc = _mm_cvtepi32_ps(itrunc);
	__m128 y = _mm_sub_ps(V, ftrunc);
	__m128 poly = _mm_mul_ps(g_MathExpEst7, y);
	poly = _mm_add_ps(g_MathExpEst6, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathExpEst5, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathExpEst4, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathExpEst3, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathExpEst2, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathExpEst1, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathOne, poly);
	__m128i biased = _mm_add_epi32(itrunc, g_MathExponentBias);
	biased = _mm_slli_epi32(biased, 23);
	__m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
	biased = _mm_add_epi32(itrunc, g_Math253);
	biased = _mm_slli_epi32(biased, 23);
	__m128 result1 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
	result1 = _mm_mul_ps(g_MathMinNormal.v, result1);
	__m128i comp = _mm_cmplt_epi32(_mm_castps_si128(V), g_MathBin128);
	__m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
	__m128i select1 = _mm_andnot_si128(comp, g_MathInfinity);
	__m128i result2 = _mm_or_si128(select0, select1);
	comp = _mm_cmplt_epi32(itrunc, g_MathSubnormalExponent);
	select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
	select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
	__m128i result3 = _mm_or_si128(select0, select1);
	comp = _mm_cmplt_epi32(_mm_castps_si128(V), g_MathBinNeg150);
	select0 = _mm_and_si128(comp, result3);
	select1 = _mm_andnot_si128(comp, g_MathZero);
	__m128i result4 = _mm_or_si128(select0, select1);
	__m128i sign = _mm_and_si128(_mm_castps_si128(V), g_MathNegativeZero);
	comp = _mm_cmpeq_epi32(sign, g_MathNegativeZero);
	select0 = _mm_and_si128(comp, result4);
	select1 = _mm_andnot_si128(comp, result2);
	__m128i result5 = _mm_or_si128(select0, select1);
	__m128i t0 = _mm_and_si128(_mm_castps_si128(V), g_MathQNaNTest);
	__m128i t1 = _mm_and_si128(_mm_castps_si128(V), g_MathInfinity);
	t0 = _mm_cmpeq_epi32(t0, g_MathZero);
	t1 = _mm_cmpeq_epi32(t1, g_MathInfinity);
	__m128i isNaN = _mm_andnot_si128(t0, t1);
	select0 = _mm_and_si128(isNaN, g_MathQNaN);
	select1 = _mm_andnot_si128(isNaN, result5);
	__m128i vResult = _mm_or_si128(select0, select1);
	return _mm_castsi128_ps(vResult);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorExpE(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeExpf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeExpf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeExpf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeExpf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128 Ve = _mm_mul_ps(g_MathLgE, V);
	__m128i itrunc = _mm_cvttps_epi32(Ve);
	__m128 ftrunc = _mm_cvtepi32_ps(itrunc);
	__m128 y = _mm_sub_ps(Ve, ftrunc);
	__m128 poly = _mm_mul_ps(g_MathExpEst7, y);
	poly = _mm_add_ps(g_MathExpEst6, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathExpEst5, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathExpEst4, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathExpEst3, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathExpEst2, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathExpEst1, poly);
	poly = _mm_mul_ps(poly, y);
	poly = _mm_add_ps(g_MathOne, poly);
	__m128i biased = _mm_add_epi32(itrunc, g_MathExponentBias);
	biased = _mm_slli_epi32(biased, 23);
	__m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
	biased = _mm_add_epi32(itrunc, g_Math253);
	biased = _mm_slli_epi32(biased, 23);
	__m128 result1 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
	result1 = _mm_mul_ps(g_MathMinNormal.v, result1);
	__m128i comp = _mm_cmplt_epi32(_mm_castps_si128(Ve), g_MathBin128);
	__m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
	__m128i select1 = _mm_andnot_si128(comp, g_MathInfinity);
	__m128i result2 = _mm_or_si128(select0, select1);
	comp = _mm_cmplt_epi32(itrunc, g_MathSubnormalExponent);
	select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
	select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
	__m128i result3 = _mm_or_si128(select0, select1);
	comp = _mm_cmplt_epi32(_mm_castps_si128(Ve), g_MathBinNeg150);
	select0 = _mm_and_si128(comp, result3);
	select1 = _mm_andnot_si128(comp, g_MathZero);
	__m128i result4 = _mm_or_si128(select0, select1);
	__m128i sign = _mm_and_si128(_mm_castps_si128(Ve), g_MathNegativeZero);
	comp = _mm_cmpeq_epi32(sign, g_MathNegativeZero);
	select0 = _mm_and_si128(comp, result4);
	select1 = _mm_andnot_si128(comp, result2);
	__m128i result5 = _mm_or_si128(select0, select1);
	__m128i t0 = _mm_and_si128(_mm_castps_si128(Ve), g_MathQNaNTest);
	__m128i t1 = _mm_and_si128(_mm_castps_si128(Ve), g_MathInfinity);
	t0 = _mm_cmpeq_epi32(t0, g_MathZero);
	t1 = _mm_cmpeq_epi32(t1, g_MathInfinity);
	__m128i isNaN = _mm_andnot_si128(t0, t1);
	select0 = _mm_and_si128(isNaN, g_MathQNaN);
	select1 = _mm_andnot_si128(isNaN, result5);
	__m128i vResult = _mm_or_si128(select0, select1);
	return _mm_castsi128_ps(vResult);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorExp(
	VE_FVECTOR V) noexcept
{
	return VeVectorExp2(V);
}
//--------------------------------------------------------------------------
#if defined(VE_ENABLE_SSE)
namespace venus
{
	inline __m128i multi_sll_epi32(__m128i value, __m128i count) noexcept
	{
		__m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
		__m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
		c = _mm_and_si128(c, g_MathMaskX);
		__m128i r0 = _mm_sll_epi32(v, c);
		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
		c = _mm_and_si128(c, g_MathMaskX);
		__m128i r1 = _mm_sll_epi32(v, c);
		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
		c = _mm_and_si128(c, g_MathMaskX);
		__m128i r2 = _mm_sll_epi32(v, c);
		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
		c = _mm_and_si128(c, g_MathMaskX);
		__m128i r3 = _mm_sll_epi32(v, c);
		__m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
		__m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
		__m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
		return _mm_castps_si128(result);
	}
	inline __m128i multi_srl_epi32(__m128i value, __m128i count) noexcept
	{
		__m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
		__m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
		c = _mm_and_si128(c, g_MathMaskX);
		__m128i r0 = _mm_srl_epi32(v, c);
		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
		c = _mm_and_si128(c, g_MathMaskX);
		__m128i r1 = _mm_srl_epi32(v, c);
		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
		c = _mm_and_si128(c, g_MathMaskX);
		__m128i r2 = _mm_srl_epi32(v, c);
		v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
		c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
		c = _mm_and_si128(c, g_MathMaskX);
		__m128i r3 = _mm_srl_epi32(v, c);
		__m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
		__m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
		__m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
		return _mm_castps_si128(result);
	}
	inline __m128i GetLeadingBit(const __m128i value) noexcept
	{
		static const VE_VECTORI32 g_Math0000FFFF = { 0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF };
		static const VE_VECTORI32 g_Math000000FF = { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF };
		static const VE_VECTORI32 g_Math0000000F = { 0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F };
		static const VE_VECTORI32 g_Math00000003 = { 0x00000003, 0x00000003, 0x00000003, 0x00000003 };
		__m128i v = value, r, c, b, s;
		c = _mm_cmpgt_epi32(v, g_Math0000FFFF);
		b = _mm_srli_epi32(c, 31);
		r = _mm_slli_epi32(b, 4);
		v = multi_srl_epi32(v, r);
		c = _mm_cmpgt_epi32(v, g_Math000000FF);
		b = _mm_srli_epi32(c, 31);
		s = _mm_slli_epi32(b, 3);
		v = multi_srl_epi32(v, s);
		r = _mm_or_si128(r, s);
		c = _mm_cmpgt_epi32(v, g_Math0000000F);
		b = _mm_srli_epi32(c, 31);
		s = _mm_slli_epi32(b, 2);
		v = multi_srl_epi32(v, s);
		r = _mm_or_si128(r, s);
		c = _mm_cmpgt_epi32(v, g_Math00000003);
		b = _mm_srli_epi32(c, 31);
		s = _mm_slli_epi32(b, 1);
		v = multi_srl_epi32(v, s);
		r = _mm_or_si128(r, s);
		s = _mm_srli_epi32(v, 1);
		r = _mm_or_si128(r, s);
		return r;
	}
}
#endif
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorLog2(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	const float fScale = 1.4426950f;
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeLogf(V.vector4_f32[0])*fScale;
	Result.vector4_f32[1] = VeLogf(V.vector4_f32[1])*fScale;
	Result.vector4_f32[2] = VeLogf(V.vector4_f32[2])*fScale;
	Result.vector4_f32[3] = VeLogf(V.vector4_f32[3])*fScale;
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128i rawBiased = _mm_and_si128(_mm_castps_si128(V), g_MathInfinity);
	__m128i trailing = _mm_and_si128(_mm_castps_si128(V), g_MathQNaNTest);
	__m128i isExponentZero = _mm_cmpeq_epi32(g_MathZero, rawBiased);
	__m128i biased = _mm_srli_epi32(rawBiased, 23);
	__m128i exponentNor = _mm_sub_epi32(biased, g_MathExponentBias);
	__m128i trailingNor = trailing;
	__m128i leading = venus::GetLeadingBit(trailing);
	__m128i shift = _mm_sub_epi32(g_MathNumTrailing, leading);
	__m128i exponentSub = _mm_sub_epi32(g_MathSubnormalExponent, shift);
	__m128i trailingSub = venus::multi_sll_epi32(trailing, shift);
	trailingSub = _mm_and_si128(trailingSub, g_MathQNaNTest);
	__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
	__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
	__m128i e = _mm_or_si128(select0, select1);
	select0 = _mm_and_si128(isExponentZero, trailingSub);
	select1 = _mm_andnot_si128(isExponentZero, trailingNor);
	__m128i t = _mm_or_si128(select0, select1);
	__m128i tmp = _mm_or_si128(g_MathOne, t);
	__m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), g_MathOne);
	__m128 log2 = _mm_mul_ps(g_MathLogEst7, y);
	log2 = _mm_add_ps(g_MathLogEst6, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst5, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst4, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst3, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst2, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst1, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst0, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(log2, _mm_cvtepi32_ps(e));
	__m128i isInfinite = _mm_and_si128(_mm_castps_si128(V), g_MathAbsMask);
	isInfinite = _mm_cmpeq_epi32(isInfinite, g_MathInfinity);
	__m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(V), g_MathZero);
	__m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(V), g_MathInfinity);
	__m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);
	__m128i isZero = _mm_and_si128(_mm_castps_si128(V), g_MathAbsMask);
	isZero = _mm_cmpeq_epi32(isZero, g_MathZero);
	__m128i t0 = _mm_and_si128(_mm_castps_si128(V), g_MathQNaNTest);
	__m128i t1 = _mm_and_si128(_mm_castps_si128(V), g_MathInfinity);
	t0 = _mm_cmpeq_epi32(t0, g_MathZero);
	t1 = _mm_cmpeq_epi32(t1, g_MathInfinity);
	__m128i isNaN = _mm_andnot_si128(t0, t1);
	select0 = _mm_and_si128(isInfinite, g_MathInfinity);
	select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
	__m128i result = _mm_or_si128(select0, select1);
	select0 = _mm_and_si128(isZero, g_MathNegInfinity);
	select1 = _mm_andnot_si128(isZero, g_MathNegQNaN);
	tmp = _mm_or_si128(select0, select1);
	select0 = _mm_and_si128(isPositive, result);
	select1 = _mm_andnot_si128(isPositive, tmp);
	result = _mm_or_si128(select0, select1);
	select0 = _mm_and_si128(isNaN, g_MathQNaN);
	select1 = _mm_andnot_si128(isNaN, result);
	result = _mm_or_si128(select0, select1);
	return _mm_castsi128_ps(result);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorLogE(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeLogf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeLogf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeLogf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeLogf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128i rawBiased = _mm_and_si128(_mm_castps_si128(V), g_MathInfinity);
	__m128i trailing = _mm_and_si128(_mm_castps_si128(V), g_MathQNaNTest);
	__m128i isExponentZero = _mm_cmpeq_epi32(g_MathZero, rawBiased);
	__m128i biased = _mm_srli_epi32(rawBiased, 23);
	__m128i exponentNor = _mm_sub_epi32(biased, g_MathExponentBias);
	__m128i trailingNor = trailing;
	__m128i leading = venus::GetLeadingBit(trailing);
	__m128i shift = _mm_sub_epi32(g_MathNumTrailing, leading);
	__m128i exponentSub = _mm_sub_epi32(g_MathSubnormalExponent, shift);
	__m128i trailingSub = venus::multi_sll_epi32(trailing, shift);
	trailingSub = _mm_and_si128(trailingSub, g_MathQNaNTest);
	__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
	__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
	__m128i e = _mm_or_si128(select0, select1);
	select0 = _mm_and_si128(isExponentZero, trailingSub);
	select1 = _mm_andnot_si128(isExponentZero, trailingNor);
	__m128i t = _mm_or_si128(select0, select1);
	__m128i tmp = _mm_or_si128(g_MathOne, t);
	__m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), g_MathOne);
	__m128 log2 = _mm_mul_ps(g_MathLogEst7, y);
	log2 = _mm_add_ps(g_MathLogEst6, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst5, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst4, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst3, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst2, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst1, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(g_MathLogEst0, log2);
	log2 = _mm_mul_ps(log2, y);
	log2 = _mm_add_ps(log2, _mm_cvtepi32_ps(e));
	log2 = _mm_mul_ps(g_MathInvLgE, log2);
	__m128i isInfinite = _mm_and_si128(_mm_castps_si128(V), g_MathAbsMask);
	isInfinite = _mm_cmpeq_epi32(isInfinite, g_MathInfinity);
	__m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(V), g_MathZero);
	__m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(V), g_MathInfinity);
	__m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);
	__m128i isZero = _mm_and_si128(_mm_castps_si128(V), g_MathAbsMask);
	isZero = _mm_cmpeq_epi32(isZero, g_MathZero);
	__m128i t0 = _mm_and_si128(_mm_castps_si128(V), g_MathQNaNTest);
	__m128i t1 = _mm_and_si128(_mm_castps_si128(V), g_MathInfinity);
	t0 = _mm_cmpeq_epi32(t0, g_MathZero);
	t1 = _mm_cmpeq_epi32(t1, g_MathInfinity);
	__m128i isNaN = _mm_andnot_si128(t0, t1);
	select0 = _mm_and_si128(isInfinite, g_MathInfinity);
	select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
	__m128i result = _mm_or_si128(select0, select1);
	select0 = _mm_and_si128(isZero, g_MathNegInfinity);
	select1 = _mm_andnot_si128(isZero, g_MathNegQNaN);
	tmp = _mm_or_si128(select0, select1);
	select0 = _mm_and_si128(isPositive, result);
	select1 = _mm_andnot_si128(isPositive, tmp);
	result = _mm_or_si128(select0, select1);
	select0 = _mm_and_si128(isNaN, g_MathQNaN);
	select1 = _mm_andnot_si128(isNaN, result);
	result = _mm_or_si128(select0, select1);
	return _mm_castsi128_ps(result);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorLog(
	VE_FVECTOR V) noexcept
{
	return VeVectorLog2(V);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorPow(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VePowf(V1.vector4_f32[0], V2.vector4_f32[0]);
	Result.vector4_f32[1] = VePowf(V1.vector4_f32[1], V2.vector4_f32[1]);
	Result.vector4_f32[2] = VePowf(V1.vector4_f32[2], V2.vector4_f32[2]);
	Result.vector4_f32[3] = VePowf(V1.vector4_f32[3], V2.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	alignas(16) float a[4];
	alignas(16) float b[4];
	_mm_store_ps(a, V1);
	_mm_store_ps(b, V2);
	VE_VECTOR vResult = _mm_setr_ps(
		VePowf(a[0], b[0]),
		VePowf(a[1], b[1]),
		VePowf(a[2], b[2]),
		VePowf(a[3], b[3]));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorAbs(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult;
	vResult.vector4_f32[0] = VeFabsf(V.vector4_f32[0]);
	vResult.vector4_f32[1] = VeFabsf(V.vector4_f32[1]);
	vResult.vector4_f32[2] = VeFabsf(V.vector4_f32[2]);
	vResult.vector4_f32[3] = VeFabsf(V.vector4_f32[3]);
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_setzero_ps();
	vResult = _mm_sub_ps(vResult, V);
	vResult = _mm_max_ps(vResult, V);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorMod(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Quotient = VeVectorDivide(V1, V2);
	Quotient = VeVectorTruncate(Quotient);
	VE_VECTOR Result = VeVectorNegativeMultiplySubtract(V2, Quotient, V1);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_div_ps(V1, V2);
	vResult = VeVectorTruncate(vResult);
	vResult = _mm_mul_ps(vResult, V2);
	vResult = _mm_sub_ps(V1, vResult);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorModAngles(
	VE_FVECTOR Angles) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V;
	VE_VECTOR Result;
	V = VeVectorMultiply(Angles, g_MathReciprocalTwoPi.v);
	V = VeVectorRound(V);
	Result = VeVectorNegativeMultiplySubtract(g_MathTwoPi.v, V, Angles);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = _mm_mul_ps(Angles, g_MathReciprocalTwoPi);
	vResult = VeVectorRound(vResult);
	vResult = _mm_mul_ps(vResult, g_MathTwoPi);
	vResult = _mm_sub_ps(Angles, vResult);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSin(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeSinf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeSinf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeSinf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeSinf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR x = VeVectorModAngles(V);
	__m128 sign = _mm_and_ps(x, g_MathNegativeZero);
	__m128 c = _mm_or_ps(g_MathPi, sign);
	__m128 absx = _mm_andnot_ps(sign, x);
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_MathHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);
	__m128 x2 = _mm_mul_ps(x, x);
	const VE_VECTOR SC1 = g_MathSinCoefficients1;
	VE_VECTOR vConstants = VE_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 Result = _mm_mul_ps(vConstants, x2);
	const VE_VECTOR SC0 = g_MathSinCoefficients0;
	vConstants = VE_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_MathOne);
	Result = _mm_mul_ps(Result, x);
	return Result;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorCos(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeCosf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeCosf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeCosf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeCosf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR x = VeVectorModAngles(V);
	VE_VECTOR sign = _mm_and_ps(x, g_MathNegativeZero);
	__m128 c = _mm_or_ps(g_MathPi, sign);
	__m128 absx = _mm_andnot_ps(sign, x);
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_MathHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, g_MathOne);
	select1 = _mm_andnot_ps(comp, g_MathNegativeOne);
	sign = _mm_or_ps(select0, select1);
	__m128 x2 = _mm_mul_ps(x, x);
	const VE_VECTOR CC1 = g_MathCosCoefficients1;
	VE_VECTOR vConstants = VE_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 Result = _mm_mul_ps(vConstants, x2);
	const VE_VECTOR CC0 = g_MathCosCoefficients0;
	vConstants = VE_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_MathOne);
	Result = _mm_mul_ps(Result, sign);
	return Result;
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorSinCos(
	VE_VECTOR* pSin, VE_VECTOR* pCos, VE_FVECTOR V) noexcept
{
	VE_ASSERT(pSin != nullptr);
	VE_ASSERT(pCos != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Sin;
	Sin.vector4_f32[0] = VeSinf(V.vector4_f32[0]);
	Sin.vector4_f32[1] = VeSinf(V.vector4_f32[1]);
	Sin.vector4_f32[2] = VeSinf(V.vector4_f32[2]);
	Sin.vector4_f32[3] = VeSinf(V.vector4_f32[3]);
	VE_VECTOR Cos;
	Cos.vector4_f32[0] = VeCosf(V.vector4_f32[0]);
	Cos.vector4_f32[1] = VeCosf(V.vector4_f32[1]);
	Cos.vector4_f32[2] = VeCosf(V.vector4_f32[2]);
	Cos.vector4_f32[3] = VeCosf(V.vector4_f32[3]);
	*pSin = Sin;
	*pCos = Cos;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR x = VeVectorModAngles(V);
	VE_VECTOR sign = _mm_and_ps(x, g_MathNegativeZero);
	__m128 c = _mm_or_ps(g_MathPi, sign);
	__m128 absx = _mm_andnot_ps(sign, x);
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_MathHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, g_MathOne);
	select1 = _mm_andnot_ps(comp, g_MathNegativeOne);
	sign = _mm_or_ps(select0, select1);
	__m128 x2 = _mm_mul_ps(x, x);
	const VE_VECTOR SC1 = g_MathSinCoefficients1;
	VE_VECTOR vConstants = VE_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 Result = _mm_mul_ps(vConstants, x2);
	const VE_VECTOR SC0 = g_MathSinCoefficients0;
	vConstants = VE_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_MathOne);
	Result = _mm_mul_ps(Result, x);
	*pSin = Result;
	const VE_VECTOR CC1 = g_MathCosCoefficients1;
	vConstants = VE_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_mul_ps(vConstants, x2);
	const VE_VECTOR CC0 = g_MathCosCoefficients0;
	vConstants = VE_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_MathOne);
	Result = _mm_mul_ps(Result, sign);
	*pCos = Result;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorTan(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeTanf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeTanf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeTanf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeTanf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 TanCoefficients0 = { 1.0f, -4.667168334e-1f, 2.566383229e-2f, -3.118153191e-4f };
	static const VE_VECTORF32 TanCoefficients1 = { 4.981943399e-7f, -1.333835001e-1f, 3.424887824e-3f, -1.786170734e-5f };
	static const VE_VECTORF32 TanConstants = { 1.570796371f, 6.077100628e-11f, 0.000244140625f, 0.63661977228f /*2 / Pi*/ };
	static const VE_VECTORU32 Mask = { 0x1, 0x1, 0x1, 0x1 };
	VE_VECTOR TwoDivPi = VeVectorSplatW(TanConstants.v);
	VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR C0 = VeVectorSplatX(TanConstants.v);
	VE_VECTOR C1 = VeVectorSplatY(TanConstants.v);
	VE_VECTOR Epsilon = VeVectorSplatZ(TanConstants.v);
	VE_VECTOR VA = VeVectorMultiply(V, TwoDivPi);
	VA = VeVectorRound(VA);
	VE_VECTOR VC = VeVectorNegativeMultiplySubtract(VA, C0, V);
	VE_VECTOR VB = VeVectorAbs(VA);
	VC = VeVectorNegativeMultiplySubtract(VA, C1, VC);
	reinterpret_cast<__m128i *>(&VB)[0] = _mm_cvttps_epi32(VB);
	VE_VECTOR VC2 = VeVectorMultiply(VC, VC);
	VE_VECTOR T7 = VeVectorSplatW(TanCoefficients1.v);
	VE_VECTOR T6 = VeVectorSplatZ(TanCoefficients1.v);
	VE_VECTOR T4 = VeVectorSplatX(TanCoefficients1.v);
	VE_VECTOR T3 = VeVectorSplatW(TanCoefficients0.v);
	VE_VECTOR T5 = VeVectorSplatY(TanCoefficients1.v);
	VE_VECTOR T2 = VeVectorSplatZ(TanCoefficients0.v);
	VE_VECTOR T1 = VeVectorSplatY(TanCoefficients0.v);
	VE_VECTOR T0 = VeVectorSplatX(TanCoefficients0.v);
	VE_VECTOR VBIsEven = VeVectorAndInt(VB, Mask.v);
	VBIsEven = VeVectorEqualInt(VBIsEven, Zero);
	VE_VECTOR N = VeVectorMultiplyAdd(VC2, T7, T6);
	VE_VECTOR D = VeVectorMultiplyAdd(VC2, T4, T3);
	N = VeVectorMultiplyAdd(VC2, N, T5);
	D = VeVectorMultiplyAdd(VC2, D, T2);
	N = VeVectorMultiply(VC2, N);
	D = VeVectorMultiplyAdd(VC2, D, T1);
	N = VeVectorMultiplyAdd(VC, N, VC);
	VE_VECTOR VCNearZero = VeVectorInBounds(VC, Epsilon);
	D = VeVectorMultiplyAdd(VC2, D, T0);
	N = VeVectorSelect(N, VC, VCNearZero);
	D = VeVectorSelect(D, g_MathOne.v, VCNearZero);
	VE_VECTOR R0 = VeVectorNegate(N);
	VE_VECTOR R1 = VeVectorDivide(N, D);
	R0 = VeVectorDivide(D, R0);
	VE_VECTOR VIsZero = VeVectorEqual(V, Zero);
	VE_VECTOR Result = VeVectorSelect(R0, R1, VBIsEven);
	Result = VeVectorSelect(Result, Zero, VIsZero);
	return Result;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSinH
(
	VE_FVECTOR V
	)
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeSinhf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeSinhf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeSinhf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeSinhf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 Scale = { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f };
	VE_VECTOR V1 = _mm_mul_ps(V, Scale);
	V1 = _mm_add_ps(V1, g_MathNegativeOne);
	VE_VECTOR V2 = _mm_mul_ps(V, Scale);
	V2 = _mm_sub_ps(g_MathNegativeOne, V2);
	VE_VECTOR E1 = VeVectorExp(V1);
	VE_VECTOR E2 = VeVectorExp(V2);
	return _mm_sub_ps(E1, E2);
#	endif
}

//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorCosH(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeCoshf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeCoshf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeCoshf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeCoshf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 Scale = { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f };
	VE_VECTOR V1 = _mm_mul_ps(V, Scale.v);
	V1 = _mm_add_ps(V1, g_MathNegativeOne.v);
	VE_VECTOR V2 = _mm_mul_ps(V, Scale.v);
	V2 = _mm_sub_ps(g_MathNegativeOne.v, V2);
	VE_VECTOR E1 = VeVectorExp(V1);
	VE_VECTOR E2 = VeVectorExp(V2);
	return _mm_add_ps(E1, E2);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorTanH(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeTanhf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeTanhf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeTanhf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeTanhf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 Scale = { 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f };
	VE_VECTOR E = _mm_mul_ps(V, Scale.v);
	E = VeVectorExp(E);
	E = _mm_mul_ps(E, g_MathOneHalf.v);
	E = _mm_add_ps(E, g_MathOneHalf.v);
	E = _mm_div_ps(g_MathOne.v, E);
	return _mm_sub_ps(g_MathOne.v, E);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorASin(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeAsinf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeAsinf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeAsinf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeAsinf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128 nonnegative = _mm_cmpge_ps(V, g_MathZero);
	__m128 mvalue = _mm_sub_ps(g_MathZero, V);
	__m128 x = _mm_max_ps(V, mvalue);
	__m128 oneMValue = _mm_sub_ps(g_MathOne, x);
	__m128 clampOneMValue = _mm_max_ps(g_MathZero, oneMValue);
	__m128 root = _mm_sqrt_ps(clampOneMValue);
	const VE_VECTOR AC1 = g_MathArcCoefficients1;
	VE_VECTOR vConstants = VE_PERMUTE_PS(AC1, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 t0 = _mm_mul_ps(vConstants, x);
	vConstants = VE_PERMUTE_PS(AC1, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AC1, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AC1, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	const VE_VECTOR AC0 = g_MathArcCoefficients0;
	vConstants = VE_PERMUTE_PS(AC0, _MM_SHUFFLE(3, 3, 3, 3));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AC0, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AC0, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AC0, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, root);
	__m128 t1 = _mm_sub_ps(g_MathPi, t0);
	t0 = _mm_and_ps(nonnegative, t0);
	t1 = _mm_andnot_ps(nonnegative, t1);
	t0 = _mm_or_ps(t0, t1);
	t0 = _mm_sub_ps(g_MathHalfPi, t0);
	return t0;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorACos(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeAcosf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeAcosf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeAcosf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeAcosf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128 nonnegative = _mm_cmpge_ps(V, g_MathZero);
	__m128 mvalue = _mm_sub_ps(g_MathZero, V);
	__m128 x = _mm_max_ps(V, mvalue);
	__m128 oneMValue = _mm_sub_ps(g_MathOne, x);
	__m128 clampOneMValue = _mm_max_ps(g_MathZero, oneMValue);
	__m128 root = _mm_sqrt_ps(clampOneMValue);
	const VE_VECTOR AC1 = g_MathArcCoefficients1;
	VE_VECTOR vConstants = VE_PERMUTE_PS(AC1, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 t0 = _mm_mul_ps(vConstants, x);
	vConstants = VE_PERMUTE_PS(AC1, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AC1, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AC1, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	const VE_VECTOR AC0 = g_MathArcCoefficients0;
	vConstants = VE_PERMUTE_PS(AC0, _MM_SHUFFLE(3, 3, 3, 3));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AC0, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AC0, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AC0, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, root);
	__m128 t1 = _mm_sub_ps(g_MathPi, t0);
	t0 = _mm_and_ps(nonnegative, t0);
	t1 = _mm_andnot_ps(nonnegative, t1);
	t0 = _mm_or_ps(t0, t1);
	return t0;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorATan(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeAtanf(V.vector4_f32[0]);
	Result.vector4_f32[1] = atanf(V.vector4_f32[1]);
	Result.vector4_f32[2] = atanf(V.vector4_f32[2]);
	Result.vector4_f32[3] = atanf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128 absV = VeVectorAbs(V);
	__m128 invV = _mm_div_ps(g_MathOne, V);
	__m128 comp = _mm_cmpgt_ps(V, g_MathOne);
	__m128 select0 = _mm_and_ps(comp, g_MathOne);
	__m128 select1 = _mm_andnot_ps(comp, g_MathNegativeOne);
	__m128 sign = _mm_or_ps(select0, select1);
	comp = _mm_cmple_ps(absV, g_MathOne);
	select0 = _mm_and_ps(comp, g_MathZero);
	select1 = _mm_andnot_ps(comp, sign);
	sign = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, V);
	select1 = _mm_andnot_ps(comp, invV);
	__m128 x = _mm_or_ps(select0, select1);
	__m128 x2 = _mm_mul_ps(x, x);
	const VE_VECTOR TC1 = g_MathATanCoefficients1;
	VE_VECTOR vConstants = VE_PERMUTE_PS(TC1, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 Result = _mm_mul_ps(vConstants, x2);
	vConstants = VE_PERMUTE_PS(TC1, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(TC1, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(TC1, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	const VE_VECTOR TC0 = g_MathATanCoefficients0;
	vConstants = VE_PERMUTE_PS(TC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(TC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(TC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(TC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_MathOne);
	Result = _mm_mul_ps(Result, x);
	__m128 result1 = _mm_mul_ps(sign, g_MathHalfPi);
	result1 = _mm_sub_ps(result1, Result);
	comp = _mm_cmpeq_ps(sign, g_MathZero);
	select0 = _mm_and_ps(comp, Result);
	select1 = _mm_andnot_ps(comp, result1);
	Result = _mm_or_ps(select0, select1);
	return Result;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorATan2(
	VE_FVECTOR Y, VE_FVECTOR X) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeAtan2f(Y.vector4_f32[0], X.vector4_f32[0]);
	Result.vector4_f32[1] = VeAtan2f(Y.vector4_f32[1], X.vector4_f32[1]);
	Result.vector4_f32[2] = VeAtan2f(Y.vector4_f32[2], X.vector4_f32[2]);
	Result.vector4_f32[3] = VeAtan2f(Y.vector4_f32[3], X.vector4_f32[3]);
	return Result;
#	else
	static const VE_VECTORF32 ATan2Constants = { VE_MATH_PI_F, VE_MATH_PI_2_F, VE_MATH_PI_4_F, VE_MATH_PI_F * 3.0f / 4.0f };
	VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR ATanResultValid = VeVectorTrueInt();
	VE_VECTOR Pi = VeVectorSplatX(ATan2Constants);
	VE_VECTOR PiOverTwo = VeVectorSplatY(ATan2Constants);
	VE_VECTOR PiOverFour = VeVectorSplatZ(ATan2Constants);
	VE_VECTOR ThreePiOverFour = VeVectorSplatW(ATan2Constants);
	VE_VECTOR YEqualsZero = VeVectorEqual(Y, Zero);
	VE_VECTOR XEqualsZero = VeVectorEqual(X, Zero);
	VE_VECTOR XIsPositive = VeVectorAndInt(X, g_MathNegativeZero.v);
	XIsPositive = VeVectorEqualInt(XIsPositive, Zero);
	VE_VECTOR YEqualsInfinity = VeVectorIsInfinite(Y);
	VE_VECTOR XEqualsInfinity = VeVectorIsInfinite(X);
	VE_VECTOR YSign = VeVectorAndInt(Y, g_MathNegativeZero.v);
	Pi = VeVectorOrInt(Pi, YSign);
	PiOverTwo = VeVectorOrInt(PiOverTwo, YSign);
	PiOverFour = VeVectorOrInt(PiOverFour, YSign);
	ThreePiOverFour = VeVectorOrInt(ThreePiOverFour, YSign);
	VE_VECTOR R1 = VeVectorSelect(Pi, YSign, XIsPositive);
	VE_VECTOR R2 = VeVectorSelect(ATanResultValid, PiOverTwo, XEqualsZero);
	VE_VECTOR R3 = VeVectorSelect(R2, R1, YEqualsZero);
	VE_VECTOR R4 = VeVectorSelect(ThreePiOverFour, PiOverFour, XIsPositive);
	VE_VECTOR R5 = VeVectorSelect(PiOverTwo, R4, XEqualsInfinity);
	VE_VECTOR Result = VeVectorSelect(R3, R5, YEqualsInfinity);
	ATanResultValid = VeVectorEqualInt(Result, ATanResultValid);
	VE_VECTOR V = VeVectorDivide(Y, X);
	VE_VECTOR R0 = VeVectorATan(V);
	R1 = VeVectorSelect(Pi, g_MathNegativeZero, XIsPositive);
	R2 = VeVectorAdd(R0, R1);
	return VeVectorSelect(Result, R2, ATanResultValid);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorSinEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeSinf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeSinf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeSinf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeSinf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR x = VeVectorModAngles(V);
	__m128 sign = _mm_and_ps(x, g_MathNegativeZero);
	__m128 c = _mm_or_ps(g_MathPi, sign);
	__m128 absx = _mm_andnot_ps(sign, x);
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_MathHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);
	__m128 x2 = _mm_mul_ps(x, x);
	const VE_VECTOR SEC = g_MathSinCoefficients1;
	VE_VECTOR vConstants = VE_PERMUTE_PS(SEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 Result = _mm_mul_ps(vConstants, x2);
	vConstants = VE_PERMUTE_PS(SEC, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(SEC, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_MathOne);
	Result = _mm_mul_ps(Result, x);
	return Result;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorCosEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeCosf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeCosf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeCosf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeCosf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR x = VeVectorModAngles(V);
	VE_VECTOR sign = _mm_and_ps(x, g_MathNegativeZero);
	__m128 c = _mm_or_ps(g_MathPi, sign);
	__m128 absx = _mm_andnot_ps(sign, x);
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_MathHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, g_MathOne);
	select1 = _mm_andnot_ps(comp, g_MathNegativeOne);
	sign = _mm_or_ps(select0, select1);
	__m128 x2 = _mm_mul_ps(x, x);
	const VE_VECTOR CEC = g_MathCosCoefficients1;
	VE_VECTOR vConstants = VE_PERMUTE_PS(CEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 Result = _mm_mul_ps(vConstants, x2);
	vConstants = VE_PERMUTE_PS(CEC, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(CEC, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_MathOne);
	Result = _mm_mul_ps(Result, sign);
	return Result;
#	endif
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVectorSinCosEst(
	VE_VECTOR* pSin, VE_VECTOR* pCos, VE_FVECTOR  V) noexcept
{
	VE_ASSERT(pSin != nullptr);
	VE_ASSERT(pCos != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Sin;
	Sin.vector4_f32[0] = VeSinf(V.vector4_f32[0]);
	Sin.vector4_f32[1] = VeSinf(V.vector4_f32[1]);
	Sin.vector4_f32[2] = VeSinf(V.vector4_f32[2]);
	Sin.vector4_f32[3] = VeSinf(V.vector4_f32[3]);
	VE_VECTOR Cos;
	Cos.vector4_f32[0] = VeCosf(V.vector4_f32[0]);
	Cos.vector4_f32[1] = VeCosf(V.vector4_f32[1]);
	Cos.vector4_f32[2] = VeCosf(V.vector4_f32[2]);
	Cos.vector4_f32[3] = VeCosf(V.vector4_f32[3]);
	*pSin = Sin;
	*pCos = Cos;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR x = VeVectorModAngles(V);
	VE_VECTOR sign = _mm_and_ps(x, g_MathNegativeZero);
	__m128 c = _mm_or_ps(g_MathPi, sign);
	__m128 absx = _mm_andnot_ps(sign, x);
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_MathHalfPi);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, g_MathOne);
	select1 = _mm_andnot_ps(comp, g_MathNegativeOne);
	sign = _mm_or_ps(select0, select1);
	__m128 x2 = _mm_mul_ps(x, x);
	const VE_VECTOR SEC = g_MathSinCoefficients1;
	VE_VECTOR vConstants = VE_PERMUTE_PS(SEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 Result = _mm_mul_ps(vConstants, x2);
	vConstants = VE_PERMUTE_PS(SEC, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(SEC, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_MathOne);
	Result = _mm_mul_ps(Result, x);
	*pSin = Result;
	const VE_VECTOR CEC = g_MathCosCoefficients1;
	vConstants = VE_PERMUTE_PS(CEC, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_mul_ps(vConstants, x2);
	vConstants = VE_PERMUTE_PS(CEC, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(CEC, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_MathOne);
	Result = _mm_mul_ps(Result, sign);
	*pCos = Result;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorTanEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeTanf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeTanf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeTanf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeTanf(V.vector4_f32[3]);
	return Result;
#	else
	VE_VECTOR OneOverPi = VeVectorSplatW(g_MathTanEstCoefficients.v);
	VE_VECTOR V1 = VeVectorMultiply(V, OneOverPi);
	V1 = VeVectorRound(V1);
	V1 = VeVectorNegativeMultiplySubtract(g_MathPi.v, V1, V);
	VE_VECTOR T0 = VeVectorSplatX(g_MathTanEstCoefficients.v);
	VE_VECTOR T1 = VeVectorSplatY(g_MathTanEstCoefficients.v);
	VE_VECTOR T2 = VeVectorSplatZ(g_MathTanEstCoefficients.v);
	VE_VECTOR V2T2 = VeVectorNegativeMultiplySubtract(V1, V1, T2);
	VE_VECTOR V2 = VeVectorMultiply(V1, V1);
	VE_VECTOR V1T0 = VeVectorMultiply(V1, T0);
	VE_VECTOR V1T1 = VeVectorMultiply(V1, T1);
	VE_VECTOR D = VeVectorReciprocalEst(V2T2);
	VE_VECTOR N = VeVectorMultiplyAdd(V2, V1T1, V1T0);
	return VeVectorMultiply(N, D);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorASinEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeAsinf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeAsinf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeAsinf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeAsinf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128 nonnegative = _mm_cmpge_ps(V, g_MathZero);
	__m128 mvalue = _mm_sub_ps(g_MathZero, V);
	__m128 x = _mm_max_ps(V, mvalue);
	__m128 oneMValue = _mm_sub_ps(g_MathOne, x);
	__m128 clampOneMValue = _mm_max_ps(g_MathZero, oneMValue);
	__m128 root = _mm_sqrt_ps(clampOneMValue);
	const VE_VECTOR AEC = g_MathArcEstCoefficients;
	VE_VECTOR vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 t0 = _mm_mul_ps(vConstants, x);
	vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, root);
	__m128 t1 = _mm_sub_ps(g_MathPi, t0);
	t0 = _mm_and_ps(nonnegative, t0);
	t1 = _mm_andnot_ps(nonnegative, t1);
	t0 = _mm_or_ps(t0, t1);
	t0 = _mm_sub_ps(g_MathHalfPi, t0);
	return t0;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorACosEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeAcosf(V.vector4_f32[0]);
	Result.vector4_f32[1] = VeAcosf(V.vector4_f32[1]);
	Result.vector4_f32[2] = VeAcosf(V.vector4_f32[2]);
	Result.vector4_f32[3] = VeAcosf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128 nonnegative = _mm_cmpge_ps(V, g_MathZero);
	__m128 mvalue = _mm_sub_ps(g_MathZero, V);
	__m128 x = _mm_max_ps(V, mvalue);
	__m128 oneMValue = _mm_sub_ps(g_MathOne, x);
	__m128 clampOneMValue = _mm_max_ps(g_MathZero, oneMValue);
	__m128 root = _mm_sqrt_ps(clampOneMValue);
	const VE_VECTOR AEC = g_MathArcEstCoefficients;
	VE_VECTOR vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 t0 = _mm_mul_ps(vConstants, x);
	vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, x);
	vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
	t0 = _mm_add_ps(t0, vConstants);
	t0 = _mm_mul_ps(t0, root);
	__m128 t1 = _mm_sub_ps(g_MathPi, t0);
	t0 = _mm_and_ps(nonnegative, t0);
	t1 = _mm_andnot_ps(nonnegative, t1);
	t0 = _mm_or_ps(t0, t1);
	return t0;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorATanEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeAtanf(V.vector4_f32[0]);
	Result.vector4_f32[1] = atanf(V.vector4_f32[1]);
	Result.vector4_f32[2] = atanf(V.vector4_f32[2]);
	Result.vector4_f32[3] = atanf(V.vector4_f32[3]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	__m128 absV = VeVectorAbs(V);
	__m128 invV = _mm_div_ps(g_MathOne, V);
	__m128 comp = _mm_cmpgt_ps(V, g_MathOne);
	__m128 select0 = _mm_and_ps(comp, g_MathOne);
	__m128 select1 = _mm_andnot_ps(comp, g_MathNegativeOne);
	__m128 sign = _mm_or_ps(select0, select1);
	comp = _mm_cmple_ps(absV, g_MathOne);
	select0 = _mm_and_ps(comp, g_MathZero);
	select1 = _mm_andnot_ps(comp, sign);
	sign = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, V);
	select1 = _mm_andnot_ps(comp, invV);
	__m128 x = _mm_or_ps(select0, select1);
	__m128 x2 = _mm_mul_ps(x, x);
	const VE_VECTOR AEC = g_MathATanEstCoefficients1;
	VE_VECTOR vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 Result = _mm_mul_ps(vConstants, x2);
	vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	vConstants = VE_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_MathATanEstCoefficients0);
	Result = _mm_mul_ps(Result, x);
	__m128 result1 = _mm_mul_ps(sign, g_MathHalfPi);
	result1 = _mm_sub_ps(result1, Result);
	comp = _mm_cmpeq_ps(sign, g_MathZero);
	select0 = _mm_and_ps(comp, Result);
	select1 = _mm_andnot_ps(comp, result1);
	Result = _mm_or_ps(select0, select1);
	return Result;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorATan2Est(
	VE_FVECTOR Y, VE_FVECTOR X) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = VeAtan2f(Y.vector4_f32[0], X.vector4_f32[0]);
	Result.vector4_f32[1] = VeAtan2f(Y.vector4_f32[1], X.vector4_f32[1]);
	Result.vector4_f32[2] = VeAtan2f(Y.vector4_f32[2], X.vector4_f32[2]);
	Result.vector4_f32[3] = VeAtan2f(Y.vector4_f32[3], X.vector4_f32[3]);
	return Result;
#	else
	static const VE_VECTORF32 ATan2Constants = { VE_MATH_PI_F, VE_MATH_PI_2_F, VE_MATH_PI_4_F, 2.3561944905f };
	const VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR ATanResultValid = VeVectorTrueInt();
	VE_VECTOR Pi = VeVectorSplatX(ATan2Constants);
	VE_VECTOR PiOverTwo = VeVectorSplatY(ATan2Constants);
	VE_VECTOR PiOverFour = VeVectorSplatZ(ATan2Constants);
	VE_VECTOR ThreePiOverFour = VeVectorSplatW(ATan2Constants);
	VE_VECTOR YEqualsZero = VeVectorEqual(Y, Zero);
	VE_VECTOR XEqualsZero = VeVectorEqual(X, Zero);
	VE_VECTOR XIsPositive = VeVectorAndInt(X, g_MathNegativeZero.v);
	XIsPositive = VeVectorEqualInt(XIsPositive, Zero);
	VE_VECTOR YEqualsInfinity = VeVectorIsInfinite(Y);
	VE_VECTOR XEqualsInfinity = VeVectorIsInfinite(X);
	VE_VECTOR YSign = VeVectorAndInt(Y, g_MathNegativeZero.v);
	Pi = VeVectorOrInt(Pi, YSign);
	PiOverTwo = VeVectorOrInt(PiOverTwo, YSign);
	PiOverFour = VeVectorOrInt(PiOverFour, YSign);
	ThreePiOverFour = VeVectorOrInt(ThreePiOverFour, YSign);
	VE_VECTOR R1 = VeVectorSelect(Pi, YSign, XIsPositive);
	VE_VECTOR R2 = VeVectorSelect(ATanResultValid, PiOverTwo, XEqualsZero);
	VE_VECTOR R3 = VeVectorSelect(R2, R1, YEqualsZero);
	VE_VECTOR R4 = VeVectorSelect(ThreePiOverFour, PiOverFour, XIsPositive);
	VE_VECTOR R5 = VeVectorSelect(PiOverTwo, R4, XEqualsInfinity);
	VE_VECTOR Result = VeVectorSelect(R3, R5, YEqualsInfinity);
	ATanResultValid = VeVectorEqualInt(Result, ATanResultValid);
	VE_VECTOR Reciprocal = VeVectorReciprocalEst(X);
	VE_VECTOR V = VeVectorMultiply(Y, Reciprocal);
	VE_VECTOR R0 = VeVectorATanEst(V);
	R1 = VeVectorSelect(Pi, g_MathNegativeZero, XIsPositive);
	R2 = VeVectorAdd(R0, R1);
	Result = VeVectorSelect(Result, R2, ATanResultValid);
	return Result;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorLerp(
	VE_FVECTOR V0, VE_FVECTOR V1, VeFloat32 t) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Scale = VeVectorReplicate(t);
	VE_VECTOR Length = VeVectorSubtract(V1, V0);
	return VeVectorMultiplyAdd(Length, Scale, V0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR L = _mm_sub_ps(V1, V0);
	VE_VECTOR S = _mm_set_ps1(t);
	VE_VECTOR Result = _mm_mul_ps(L, S);
	return _mm_add_ps(Result, V0);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorLerpV(
	VE_FVECTOR V0, VE_FVECTOR V1, VE_FVECTOR T) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Length = VeVectorSubtract(V1, V0);
	return VeVectorMultiplyAdd(Length, T, V0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR Length = _mm_sub_ps(V1, V0);
	VE_VECTOR Result = _mm_mul_ps(Length, T);
	return _mm_add_ps(Result, V0);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorHermite(
	VE_FVECTOR Position0, VE_FVECTOR Tangent0, VE_FVECTOR Position1,
	VE_GVECTOR Tangent1, VeFloat32 t) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 t2 = t * t;
	VeFloat32 t3 = t * t2;
	VE_VECTOR P0 = VeVectorReplicate(2.0f * t3 - 3.0f * t2 + 1.0f);
	VE_VECTOR T0 = VeVectorReplicate(t3 - 2.0f * t2 + t);
	VE_VECTOR P1 = VeVectorReplicate(-2.0f * t3 + 3.0f * t2);
	VE_VECTOR T1 = VeVectorReplicate(t3 - t2);
	VE_VECTOR Result = VeVectorMultiply(P0, Position0);
	Result = VeVectorMultiplyAdd(T0, Tangent0, Result);
	Result = VeVectorMultiplyAdd(P1, Position1, Result);
	Result = VeVectorMultiplyAdd(T1, Tangent1, Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VeFloat32 t2 = t * t;
	VeFloat32 t3 = t * t2;
	VE_VECTOR P0 = _mm_set_ps1(2.0f * t3 - 3.0f * t2 + 1.0f);
	VE_VECTOR T0 = _mm_set_ps1(t3 - 2.0f * t2 + t);
	VE_VECTOR P1 = _mm_set_ps1(-2.0f * t3 + 3.0f * t2);
	VE_VECTOR T1 = _mm_set_ps1(t3 - t2);
	VE_VECTOR vResult = _mm_mul_ps(P0, Position0);
	VE_VECTOR vTemp = _mm_mul_ps(T0, Tangent0);
	vResult = _mm_add_ps(vResult, vTemp);
	vTemp = _mm_mul_ps(P1, Position1);
	vResult = _mm_add_ps(vResult, vTemp);
	vTemp = _mm_mul_ps(T1, Tangent1);
	vResult = _mm_add_ps(vResult, vTemp);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorHermiteV(
	VE_FVECTOR Position0, VE_FVECTOR Tangent0, VE_FVECTOR Position1,
	VE_GVECTOR Tangent1, VE_HVECTOR T) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR T2 = VeVectorMultiply(T, T);
	VE_VECTOR T3 = VeVectorMultiply(T, T2);
	VE_VECTOR P0 = VeVectorReplicate(2.0f * T3.vector4_f32[0] - 3.0f * T2.vector4_f32[0] + 1.0f);
	VE_VECTOR T0 = VeVectorReplicate(T3.vector4_f32[1] - 2.0f * T2.vector4_f32[1] + T.vector4_f32[1]);
	VE_VECTOR P1 = VeVectorReplicate(-2.0f * T3.vector4_f32[2] + 3.0f * T2.vector4_f32[2]);
	VE_VECTOR T1 = VeVectorReplicate(T3.vector4_f32[3] - T2.vector4_f32[3]);
	VE_VECTOR Result = VeVectorMultiply(P0, Position0);
	Result = VeVectorMultiplyAdd(T0, Tangent0, Result);
	Result = VeVectorMultiplyAdd(P1, Position1, Result);
	Result = VeVectorMultiplyAdd(T1, Tangent1, Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 CatMulT2 = { -3.0f,-2.0f,3.0f,-1.0f };
	static const VE_VECTORF32 CatMulT3 = { 2.0f,1.0f,-2.0f,1.0f };
	VE_VECTOR T2 = _mm_mul_ps(T, T);
	VE_VECTOR T3 = _mm_mul_ps(T, T2);
	T2 = _mm_mul_ps(T2, CatMulT2);
	T3 = _mm_mul_ps(T3, CatMulT3);
	T3 = _mm_add_ps(T3, T2);
	T2 = _mm_and_ps(T, g_MathMaskY);
	T3 = _mm_add_ps(T3, T2);
	T3 = _mm_add_ps(T3, g_MathIdentityR0);
	VE_VECTOR vResult = VE_PERMUTE_PS(T3, _MM_SHUFFLE(0, 0, 0, 0));
	vResult = _mm_mul_ps(vResult, Position0);
	T2 = VE_PERMUTE_PS(T3, _MM_SHUFFLE(1, 1, 1, 1));
	T2 = _mm_mul_ps(T2, Tangent0);
	vResult = _mm_add_ps(vResult, T2);
	T2 = VE_PERMUTE_PS(T3, _MM_SHUFFLE(2, 2, 2, 2));
	T2 = _mm_mul_ps(T2, Position1);
	vResult = _mm_add_ps(vResult, T2);
	T3 = VE_PERMUTE_PS(T3, _MM_SHUFFLE(3, 3, 3, 3));
	T3 = _mm_mul_ps(T3, Tangent1);
	vResult = _mm_add_ps(vResult, T3);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorCatmullRom(
	VE_FVECTOR Position0, VE_FVECTOR Position1, VE_FVECTOR Position2,
	VE_GVECTOR Position3, VeFloat32 t) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 t2 = t * t;
	VeFloat32 t3 = t * t2;
	VE_VECTOR P0 = VeVectorReplicate((-t3 + 2.0f * t2 - t) * 0.5f);
	VE_VECTOR P1 = VeVectorReplicate((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
	VE_VECTOR P2 = VeVectorReplicate((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
	VE_VECTOR P3 = VeVectorReplicate((t3 - t2) * 0.5f);
	VE_VECTOR Result = VeVectorMultiply(P0, Position0);
	Result = VeVectorMultiplyAdd(P1, Position1, Result);
	Result = VeVectorMultiplyAdd(P2, Position2, Result);
	Result = VeVectorMultiplyAdd(P3, Position3, Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VeFloat32 t2 = t * t;
	VeFloat32 t3 = t * t2;
	VE_VECTOR P0 = _mm_set_ps1((-t3 + 2.0f * t2 - t) * 0.5f);
	VE_VECTOR P1 = _mm_set_ps1((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
	VE_VECTOR P2 = _mm_set_ps1((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
	VE_VECTOR P3 = _mm_set_ps1((t3 - t2) * 0.5f);
	P0 = _mm_mul_ps(P0, Position0);
	P1 = _mm_mul_ps(P1, Position1);
	P2 = _mm_mul_ps(P2, Position2);
	P3 = _mm_mul_ps(P3, Position3);
	P0 = _mm_add_ps(P0, P1);
	P2 = _mm_add_ps(P2, P3);
	P0 = _mm_add_ps(P0, P2);
	return P0;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorCatmullRomV(
	VE_FVECTOR Position0, VE_FVECTOR Position1, VE_FVECTOR Position2,
	VE_GVECTOR Position3, VE_HVECTOR T) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fx = T.vector4_f32[0];
	VeFloat32 fy = T.vector4_f32[1];
	VeFloat32 fz = T.vector4_f32[2];
	VeFloat32 fw = T.vector4_f32[3];
	VE_VECTOR vResult;
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
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 Catmul2 = { 2.0f,2.0f,2.0f,2.0f };
	static const VE_VECTORF32 Catmul3 = { 3.0f,3.0f,3.0f,3.0f };
	static const VE_VECTORF32 Catmul4 = { 4.0f,4.0f,4.0f,4.0f };
	static const VE_VECTORF32 Catmul5 = { 5.0f,5.0f,5.0f,5.0f };
	VE_VECTOR T2 = _mm_mul_ps(T, T);
	VE_VECTOR T3 = _mm_mul_ps(T, T2);
	VE_VECTOR vResult = _mm_add_ps(T2, T2);
	vResult = _mm_sub_ps(vResult, T);
	vResult = _mm_sub_ps(vResult, T3);
	vResult = _mm_mul_ps(vResult, Position0);
	VE_VECTOR vTemp = _mm_mul_ps(T3, Catmul3);
	VE_VECTOR vTemp2 = _mm_mul_ps(T2, Catmul5);
	vTemp = _mm_sub_ps(vTemp, vTemp2);
	vTemp = _mm_add_ps(vTemp, Catmul2);
	vTemp = _mm_mul_ps(vTemp, Position1);
	vResult = _mm_add_ps(vResult, vTemp);
	vTemp = _mm_mul_ps(T2, Catmul4);
	vTemp2 = _mm_mul_ps(T3, Catmul3);
	vTemp = _mm_sub_ps(vTemp, vTemp2);
	vTemp = _mm_add_ps(vTemp, T);
	vTemp = _mm_mul_ps(vTemp, Position2);
	vResult = _mm_add_ps(vResult, vTemp);
	T3 = _mm_sub_ps(T3, T2);
	T3 = _mm_mul_ps(T3, Position3);
	vResult = _mm_add_ps(vResult, T3);
	vResult = _mm_mul_ps(vResult, g_MathOneHalf);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorBaryCentric(
	VE_FVECTOR Position0, VE_FVECTOR Position1, VE_FVECTOR Position2,
	VeFloat32 f, VeFloat32 g) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR P10 = VeVectorSubtract(Position1, Position0);
	VE_VECTOR ScaleF = VeVectorReplicate(f);
	VE_VECTOR P20 = VeVectorSubtract(Position2, Position0);
	VE_VECTOR ScaleG = VeVectorReplicate(g);
	VE_VECTOR Result = VeVectorMultiplyAdd(P10, ScaleF, Position0);
	Result = VeVectorMultiplyAdd(P20, ScaleG, Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR R1 = _mm_sub_ps(Position1, Position0);
	VE_VECTOR SF = _mm_set_ps1(f);
	VE_VECTOR R2 = _mm_sub_ps(Position2, Position0);
	VE_VECTOR SG = _mm_set_ps1(g);
	R1 = _mm_mul_ps(R1, SF);
	R2 = _mm_mul_ps(R2, SG);
	R1 = _mm_add_ps(R1, Position0);
	R1 = _mm_add_ps(R1, R2);
	return R1;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVectorBaryCentricV(
	VE_FVECTOR Position0, VE_FVECTOR Position1, VE_FVECTOR Position2,
	VE_GVECTOR F, VE_HVECTOR G) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR P10 = VeVectorSubtract(Position1, Position0);
	VE_VECTOR P20 = VeVectorSubtract(Position2, Position0);
	VE_VECTOR Result = VeVectorMultiplyAdd(P10, F, Position0);
	Result = VeVectorMultiplyAdd(P20, G, Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR R1 = _mm_sub_ps(Position1, Position0);
	VE_VECTOR R2 = _mm_sub_ps(Position2, Position0);
	R1 = _mm_mul_ps(R1, F);
	R2 = _mm_mul_ps(R2, G);
	R1 = _mm_add_ps(R1, Position0);
	R1 = _mm_add_ps(R1, R2);
	return R1;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2Equal(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] == V2.vector4_f32[0]) && (V1.vector4_f32[1] == V2.vector4_f32[1])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpeq_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector2EqualR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if ((V1.vector4_f32[0] == V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] == V2.vector4_f32[1]))
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if ((V1.vector4_f32[0] != V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] != V2.vector4_f32[1]))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpeq_ps(V1, V2);
	VeInt32 iTest = _mm_movemask_ps(vTemp) & 3;
	VeUInt32 CR = 0;
	if (iTest == 3)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2EqualInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_u32[0] == V2.vector4_u32[0]) && (V1.vector4_u32[1] == V2.vector4_u32[1])) != 0);
#	elif defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 3) == 3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector2EqualIntR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if ((V1.vector4_u32[0] == V2.vector4_u32[0]) &&
		(V1.vector4_u32[1] == V2.vector4_u32[1]))
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if ((V1.vector4_u32[0] != V2.vector4_u32[0]) &&
		(V1.vector4_u32[1] != V2.vector4_u32[1]))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	int iTest = _mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 3;
	VeUInt32 CR = 0;
	if (iTest == 3)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2NearEqual(
	VE_FVECTOR V1, VE_FVECTOR V2, VE_FVECTOR Epsilon) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 dx = VeFabsf(V1.vector4_f32[0] - V2.vector4_f32[0]);
	VeFloat32 dy = VeFabsf(V1.vector4_f32[1] - V2.vector4_f32[1]);
	return ((dx <= Epsilon.vector4_f32[0]) &&
		(dy <= Epsilon.vector4_f32[1]));
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vDelta = _mm_sub_ps(V1, V2);
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_sub_ps(vTemp, vDelta);
	vTemp = _mm_max_ps(vTemp, vDelta);
	vTemp = _mm_cmple_ps(vTemp, Epsilon);
	return (((_mm_movemask_ps(vTemp) & 3) == 0x3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2NotEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] != V2.vector4_f32[0]) || (V1.vector4_f32[1] != V2.vector4_f32[1])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpeq_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 3) != 3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2NotEqualInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_u32[0] != V2.vector4_u32[0]) || (V1.vector4_u32[1] != V2.vector4_u32[1])) != 0);
#	elif defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 3) != 3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2Greater(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] > V2.vector4_f32[0]) && (V1.vector4_f32[1] > V2.vector4_f32[1])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpgt_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector2GreaterR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if ((V1.vector4_f32[0] > V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] > V2.vector4_f32[1]))
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if ((V1.vector4_f32[0] <= V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] <= V2.vector4_f32[1]))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpgt_ps(V1, V2);
	VeInt32 iTest = _mm_movemask_ps(vTemp) & 3;
	VeUInt32 CR = 0;
	if (iTest == 3)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#endif
}

//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2GreaterOrEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] >= V2.vector4_f32[0]) && (V1.vector4_f32[1] >= V2.vector4_f32[1])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpge_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector2GreaterOrEqualR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if ((V1.vector4_f32[0] >= V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] >= V2.vector4_f32[1]))
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if ((V1.vector4_f32[0] < V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] < V2.vector4_f32[1]))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpge_ps(V1, V2);
	VeInt32 iTest = _mm_movemask_ps(vTemp) & 3;
	VeUInt32 CR = 0;
	if (iTest == 3)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	endif
}

//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2Less(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] < V2.vector4_f32[0]) && (V1.vector4_f32[1] < V2.vector4_f32[1])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmplt_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2LessOrEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] <= V2.vector4_f32[0]) && (V1.vector4_f32[1] <= V2.vector4_f32[1])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmple_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2InBounds(
	VE_FVECTOR V, VE_FVECTOR Bounds) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) &&
		(V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp1 = _mm_cmple_ps(V, Bounds);
	VE_VECTOR vTemp2 = _mm_mul_ps(Bounds, g_MathNegativeOne);
	vTemp2 = _mm_cmple_ps(vTemp2, V);
	vTemp1 = _mm_and_ps(vTemp1, vTemp2);
	return (((_mm_movemask_ps(vTemp1) & 0x3) == 0x3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2IsNaN(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (VEISNAN(V.vector4_f32[0]) || VEISNAN(V.vector4_f32[1]));
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTempNan = _mm_cmpneq_ps(V, V);
	return ((_mm_movemask_ps(vTempNan) & 3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector2IsInfinite(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (VEISINF(V.vector4_f32[0]) || VEISINF(V.vector4_f32[1]));
#	elif defined(VE_ENABLE_SSE)
	__m128 vTemp = _mm_and_ps(V, g_MathAbsMask);
	vTemp = _mm_cmpeq_ps(vTemp, g_MathInfinity);
	return ((_mm_movemask_ps(vTemp) & 3) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2Dot(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#if defined(VE_NO_INTRINSICS)

	VE_VECTOR Result;
	Result.vector4_f32[0] =
		Result.vector4_f32[1] =
		Result.vector4_f32[2] =
		Result.vector4_f32[3] = V1.vector4_f32[0] * V2.vector4_f32[0] + V1.vector4_f32[1] * V2.vector4_f32[1];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V1, V2);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2Cross(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	float fCross = (V1.vector4_f32[0] * V2.vector4_f32[1]) - (V1.vector4_f32[1] * V2.vector4_f32[0]);
	VE_VECTOR vResult;
	vResult.vector4_f32[0] =
		vResult.vector4_f32[1] =
		vResult.vector4_f32[2] =
		vResult.vector4_f32[3] = fCross;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 1, 0, 1));
	vResult = _mm_mul_ps(vResult, V1);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(1, 1, 1, 1));
	vResult = _mm_sub_ss(vResult, vTemp);
	vResult = VE_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 0, 0, 0));
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2LengthSq(
	VE_FVECTOR V) noexcept
{
	return VeVector2Dot(V, V);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2ReciprocalLengthEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector2LengthSq(V);
	Result = VeVectorReciprocalSqrtEst(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = _mm_rsqrt_ss(vLengthSq);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2ReciprocalLength(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector2LengthSq(V);
	Result = VeVectorReciprocalSqrt(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = _mm_sqrt_ss(vLengthSq);
	vLengthSq = _mm_div_ss(g_MathOne, vLengthSq);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2LengthEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector2LengthSq(V);
	Result = VeVectorSqrtEst(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = _mm_sqrt_ss(vLengthSq);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2Length(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector2LengthSq(V);
	Result = VeVectorSqrt(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	vLengthSq = _mm_sqrt_ps(vLengthSq);
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2NormalizeEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector2ReciprocalLength(V);
	Result = VeVectorMultiply(V, Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = _mm_rsqrt_ss(vLengthSq);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	vLengthSq = _mm_mul_ps(vLengthSq, V);
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2Normalize(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult = VeVector2Length(V);
	VeFloat32 fLength = vResult.vector4_f32[0];
	if (fLength > 0)
	{
		fLength = 1.0f / fLength;
	}
	vResult.vector4_f32[0] = V.vector4_f32[0] * fLength;
	vResult.vector4_f32[1] = V.vector4_f32[1] * fLength;
	vResult.vector4_f32[2] = V.vector4_f32[2] * fLength;
	vResult.vector4_f32[3] = V.vector4_f32[3] * fLength;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	VE_VECTOR vResult = _mm_sqrt_ps(vLengthSq);
	VE_VECTOR vZeroMask = _mm_setzero_ps();
	vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
	vLengthSq = _mm_cmpneq_ps(vLengthSq, g_MathInfinity);
	vResult = _mm_div_ps(V, vResult);
	vResult = _mm_and_ps(vResult, vZeroMask);
	VE_VECTOR vTemp1 = _mm_andnot_ps(vLengthSq, g_MathQNaN);
	VE_VECTOR vTemp2 = _mm_and_ps(vResult, vLengthSq);
	vResult = _mm_or_ps(vTemp1, vTemp2);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2ClampLengthV(
	VE_FVECTOR V, VE_FVECTOR LengthMin, VE_FVECTOR LengthMax) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2ClampLength(
	VE_FVECTOR V, VeFloat32 LengthMin, VeFloat32 LengthMax) noexcept
{
	VE_VECTOR ClampMax = VeVectorReplicate(LengthMax);
	VE_VECTOR ClampMin = VeVectorReplicate(LengthMin);
	return VeVector2ClampLengthV(V, ClampMin, ClampMax);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2ClampLengthV(
	VE_FVECTOR V, VE_FVECTOR LengthMin, VE_FVECTOR LengthMax) noexcept
{
	VE_ASSERT((VeVectorGetY(LengthMin) == VeVectorGetX(LengthMin)));
	VE_ASSERT((VeVectorGetY(LengthMax) == VeVectorGetX(LengthMax)));
	VE_ASSERT(VeVector2GreaterOrEqual(LengthMin, g_MathZero));
	VE_ASSERT(VeVector2GreaterOrEqual(LengthMax, g_MathZero));
	VE_ASSERT(VeVector2GreaterOrEqual(LengthMax, LengthMin));
	VE_VECTOR LengthSq = VeVector2LengthSq(V);
	const VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR RcpLength = VeVectorReciprocalSqrt(LengthSq);
	VE_VECTOR InfiniteLength = VeVectorEqualInt(LengthSq, g_MathInfinity.v);
	VE_VECTOR ZeroLength = VeVectorEqual(LengthSq, Zero);
	VE_VECTOR Length = VeVectorMultiply(LengthSq, RcpLength);
	VE_VECTOR Normal = VeVectorMultiply(V, RcpLength);
	VE_VECTOR Select = VeVectorEqualInt(InfiniteLength, ZeroLength);
	Length = VeVectorSelect(LengthSq, Length, Select);
	Normal = VeVectorSelect(LengthSq, Normal, Select);
	VE_VECTOR ControlMax = VeVectorGreater(Length, LengthMax);
	VE_VECTOR ControlMin = VeVectorLess(Length, LengthMin);
	VE_VECTOR ClampLength = VeVectorSelect(Length, LengthMax, ControlMax);
	ClampLength = VeVectorSelect(ClampLength, LengthMin, ControlMin);
	VE_VECTOR Result = VeVectorMultiply(Normal, ClampLength);
	VE_VECTOR Control = VeVectorEqualInt(ControlMax, ControlMin);
	Result = VeVectorSelect(Result, V, Control);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2Reflect(
	VE_FVECTOR Incident, VE_FVECTOR Normal) noexcept
{
	VE_VECTOR Result;
	Result = VeVector2Dot(Incident, Normal);
	Result = VeVectorAdd(Result, Result);
	Result = VeVectorNegativeMultiplySubtract(Result, Normal, Incident);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2RefractV(VE_FVECTOR Incident,
	VE_FVECTOR Normal, VE_FVECTOR RefractionIndex) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2Refract(VE_FVECTOR Incident,
	VE_FVECTOR Normal, VeFloat32 RefractionIndex) noexcept
{
	VE_VECTOR Index = VeVectorReplicate(RefractionIndex);
	return VeVector2RefractV(Incident, Normal, Index);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2RefractV(VE_FVECTOR Incident,
	VE_FVECTOR Normal, VE_FVECTOR RefractionIndex) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 IDotN = (Incident.vector4_f32[0] * Normal.vector4_f32[0]) + (Incident.vector4_f32[1] * Normal.vector4_f32[1]);
	VeFloat32 RY = 1.0f - (IDotN*IDotN);
	VeFloat32 RX = 1.0f - (RY*RefractionIndex.vector4_f32[0] * RefractionIndex.vector4_f32[0]);
	RY = 1.0f - (RY*RefractionIndex.vector4_f32[1] * RefractionIndex.vector4_f32[1]);
	if (RX >= 0.0f)
	{
		RX = (RefractionIndex.vector4_f32[0] * Incident.vector4_f32[0]) - (Normal.vector4_f32[0] * ((RefractionIndex.vector4_f32[0] * IDotN) + VeSqrtf(RX)));
	}
	else
	{
		RX = 0.0f;
	}
	if (RY >= 0.0f)
	{
		RY = (RefractionIndex.vector4_f32[1] * Incident.vector4_f32[1]) - (Normal.vector4_f32[1] * ((RefractionIndex.vector4_f32[1] * IDotN) + VeSqrtf(RY)));
	}
	else
	{
		RY = 0.0f;
	}
	VE_VECTOR vResult;
	vResult.vector4_f32[0] = RX;
	vResult.vector4_f32[1] = RY;
	vResult.vector4_f32[2] = 0.0f;
	vResult.vector4_f32[3] = 0.0f;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR IDotN = VeVector2Dot(Incident, Normal);
	VE_VECTOR vTemp = _mm_mul_ps(IDotN, IDotN);
	vTemp = _mm_sub_ps(g_MathOne, vTemp);
	vTemp = _mm_mul_ps(vTemp, RefractionIndex);
	vTemp = _mm_mul_ps(vTemp, RefractionIndex);
	vTemp = _mm_sub_ps(g_MathOne, vTemp);
	VE_VECTOR vMask = _mm_cmpgt_ps(vTemp, g_MathZero);
	vTemp = _mm_sqrt_ps(vTemp);
	VE_VECTOR vResult = _mm_mul_ps(RefractionIndex, IDotN);
	vTemp = _mm_add_ps(vTemp, vResult);
	vResult = _mm_mul_ps(RefractionIndex, Incident);
	vTemp = _mm_mul_ps(vTemp, Normal);
	vResult = _mm_sub_ps(vResult, vTemp);
	vResult = _mm_and_ps(vResult, vMask);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2Orthogonal(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = -V.vector4_f32[1];
	Result.vector4_f32[1] = V.vector4_f32[0];
	Result.vector4_f32[2] = 0.f;
	Result.vector4_f32[3] = 0.f;
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
	vResult = _mm_mul_ps(vResult, g_MathNegateX);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2AngleBetweenNormalsEst(
	VE_FVECTOR N1, VE_FVECTOR N2) noexcept
{
	VE_VECTOR Result = VeVector2Dot(N1, N2);
	Result = VeVectorClamp(Result, g_MathNegativeOne.v, g_MathOne.v);
	Result = VeVectorACosEst(Result);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2AngleBetweenNormals(
	VE_FVECTOR N1, VE_FVECTOR N2) noexcept
{
	VE_VECTOR Result = VeVector2Dot(N1, N2);
	Result = VeVectorClamp(Result, g_MathNegativeOne, g_MathOne);
	Result = VeVectorACos(Result);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2AngleBetweenVectors(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	VE_VECTOR L1 = VeVector2ReciprocalLength(V1);
	VE_VECTOR L2 = VeVector2ReciprocalLength(V2);
	VE_VECTOR Dot = VeVector2Dot(V1, V2);
	L1 = VeVectorMultiply(L1, L2);
	VE_VECTOR CosAngle = VeVectorMultiply(Dot, L1);
	CosAngle = VeVectorClamp(CosAngle, g_MathNegativeOne.v, g_MathOne.v);
	return VeVectorACos(CosAngle);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2LinePointDistance(
	VE_FVECTOR LinePoint1, VE_FVECTOR LinePoint2, VE_FVECTOR Point) noexcept
{
	VE_VECTOR PointVector = VeVectorSubtract(Point, LinePoint1);
	VE_VECTOR LineVector = VeVectorSubtract(LinePoint2, LinePoint1);
	VE_VECTOR LengthSq = VeVector2LengthSq(LineVector);
	VE_VECTOR PointProjectionScale = VeVector2Dot(PointVector, LineVector);
	PointProjectionScale = VeVectorDivide(PointProjectionScale, LengthSq);
	VE_VECTOR DistanceVector = VeVectorMultiply(LineVector, PointProjectionScale);
	DistanceVector = VeVectorSubtract(PointVector, DistanceVector);
	return VeVector2Length(DistanceVector);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2IntersectLine(
	VE_FVECTOR Line1Point1, VE_FVECTOR Line1Point2, VE_FVECTOR Line2Point1,
	VE_GVECTOR Line2Point2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR V1 = VeVectorSubtract(Line1Point2, Line1Point1);
	VE_VECTOR V2 = VeVectorSubtract(Line2Point2, Line2Point1);
	VE_VECTOR V3 = VeVectorSubtract(Line1Point1, Line2Point1);
	VE_VECTOR C1 = VeVector2Cross(V1, V2);
	VE_VECTOR C2 = VeVector2Cross(V2, V3);
	VE_VECTOR Result;
	const VE_VECTOR Zero = VeVectorZero();
	if (VeVector2NearEqual(C1, Zero, g_MathEpsilon.v))
	{
		if (VeVector2NearEqual(C2, Zero, g_MathEpsilon.v))
		{
			Result = g_MathInfinity.v;
		}
		else
		{
			Result = g_MathQNaN.v;
		}
	}
	else
	{
		VE_VECTOR Scale = VeVectorReciprocal(C1);
		Scale = VeVectorMultiply(C2, Scale);
		Result = VeVectorMultiplyAdd(V1, Scale, Line1Point1);
	}
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR V1 = _mm_sub_ps(Line1Point2, Line1Point1);
	VE_VECTOR V2 = _mm_sub_ps(Line2Point2, Line2Point1);
	VE_VECTOR V3 = _mm_sub_ps(Line1Point1, Line2Point1);
	VE_VECTOR C1 = VeVector2Cross(V1, V2);
	VE_VECTOR C2 = VeVector2Cross(V2, V3);
	VE_VECTOR vResultMask = _mm_setzero_ps();
	vResultMask = _mm_sub_ps(vResultMask, C1);
	vResultMask = _mm_max_ps(vResultMask, C1);
	vResultMask = _mm_cmpgt_ps(vResultMask, g_MathEpsilon);
	VE_VECTOR vFailMask = _mm_setzero_ps();
	vFailMask = _mm_sub_ps(vFailMask, C2);
	vFailMask = _mm_max_ps(vFailMask, C2);
	vFailMask = _mm_cmple_ps(vFailMask, g_MathEpsilon);
	VE_VECTOR vFail = _mm_and_ps(vFailMask, g_MathInfinity);
	vFailMask = _mm_andnot_ps(vFailMask, g_MathQNaN);
	vFail = _mm_or_ps(vFail, vFailMask);
	VE_VECTOR vResult = _mm_div_ps(C2, C1);
	vResult = _mm_mul_ps(vResult, V1);
	vResult = _mm_add_ps(vResult, Line1Point1);
	vResult = _mm_and_ps(vResult, vResultMask);
	vResultMask = _mm_andnot_ps(vResultMask, vFail);
	vResult = _mm_or_ps(vResult, vResultMask);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2Transform(
	VE_FVECTOR V, VE_FMATRIX M) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Y = VeVectorSplatY(V);
	VE_VECTOR X = VeVectorSplatX(V);
	VE_VECTOR Result = VeVectorMultiplyAdd(Y, M.r[1], M.r[3]);
	Result = VeVectorMultiplyAdd(X, M.r[0], Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
	vResult = _mm_mul_ps(vResult, M.r[0]);
	VE_VECTOR vTemp = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	vTemp = _mm_mul_ps(vTemp, M.r[1]);
	vResult = _mm_add_ps(vResult, vTemp);
	vResult = _mm_add_ps(vResult, M.r[3]);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_FLOAT4* VE_MATH_CALLCONV VeVector2TransformStream(
	VE_FLOAT4* pOutputStream, VeSizeT OutputStride,
	const VE_FLOAT4* pInputStream, VeSizeT InputStride,
	VeSizeT VectorCount, VE_FMATRIX M) noexcept
{
	VE_ASSERT(pOutputStream != nullptr);
	VE_ASSERT(pInputStream != nullptr);
	VE_ASSERT(InputStride >= sizeof(VE_FLOAT2));
	VE_ASSERT(OutputStride >= sizeof(VE_FLOAT4));
#	if defined(VE_NO_INTRINSICS)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row3 = M.r[3];
	for (VeSizeT i = 0; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat2((const VE_FLOAT2*)pInputVector);
		VE_VECTOR Y = VeVectorSplatY(V);
		VE_VECTOR X = VeVectorSplatX(V);
		VE_VECTOR Result = VeVectorMultiplyAdd(Y, row1, row3);
		Result = VeVectorMultiplyAdd(X, row0, Result);
		VeStoreFloat4((VE_FLOAT4*)pOutputVector, Result);
		pInputVector += InputStride;
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	elif defined(VE_ENABLE_SSE)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row3 = M.r[3];
	VeSizeT i = 0;
	VeSizeT two = VectorCount >> 1;
	if (two > 0)
	{
		if (InputStride == sizeof(VE_FLOAT2))
		{
			if (!((VeSizeT)pOutputStream & 0xF) && !(OutputStride & 0xF))
			{
				for (VeSizeT j = 0; j < two; ++j)
				{
					VE_VECTOR V = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
					pInputVector += sizeof(VE_FLOAT2) * 2;
					VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
					VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
					VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
					X = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
					vTemp = _mm_mul_ps(Y, row1);
					vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					i += 2;
				}
			}
			else
			{
				for (VeSizeT j = 0; j < two; ++j)
				{
					VE_VECTOR V = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
					pInputVector += sizeof(VE_FLOAT2) * 2;
					VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
					VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
					VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
					X = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
					vTemp = _mm_mul_ps(Y, row1);
					vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					i += 2;
				}
			}
		}
	}
	if (!((VeSizeT)pInputVector & 0xF) && !(InputStride & 0xF))
	{
		if (!((VeSizeT)pOutputStream & 0xF) && !(OutputStride & 0xF))
		{
			for (; i < VectorCount; i++)
			{
				VE_VECTOR V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
				pInputVector += InputStride;
				VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
				VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
				VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
				vTemp = _mm_add_ps(vTemp, row3);
				vTemp = _mm_add_ps(vTemp, vTemp2);
				VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
				pOutputVector += OutputStride;
			}
		}
		else
		{
			for (; i < VectorCount; i++)
			{
				VE_VECTOR V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
				pInputVector += InputStride;
				VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
				VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
				VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
				vTemp = _mm_add_ps(vTemp, row3);
				vTemp = _mm_add_ps(vTemp, vTemp2);
				_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
				pOutputVector += OutputStride;
			}
		}
	}
	else
	{
		for (; i < VectorCount; i++)
		{
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pInputVector));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pInputVector + 4));
			pInputVector += InputStride;
			VE_VECTOR Y = VE_PERMUTE_PS(y, _MM_SHUFFLE(0, 0, 0, 0));
			VE_VECTOR X = VE_PERMUTE_PS(x, _MM_SHUFFLE(0, 0, 0, 0));
			VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
			VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
			vTemp = _mm_add_ps(vTemp, row3);
			vTemp = _mm_add_ps(vTemp, vTemp2);
			_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
			pOutputVector += OutputStride;
		}
	}
	return pOutputStream;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2TransformCoord(
	VE_FVECTOR V, VE_FMATRIX M) noexcept
{
	VE_VECTOR Y = VeVectorSplatY(V);
	VE_VECTOR X = VeVectorSplatX(V);
	VE_VECTOR Result = VeVectorMultiplyAdd(Y, M.r[1], M.r[3]);
	Result = VeVectorMultiplyAdd(X, M.r[0], Result);
	VE_VECTOR W = VeVectorSplatW(Result);
	return VeVectorDivide(Result, W);
}
//--------------------------------------------------------------------------
inline VE_FLOAT2* VE_MATH_CALLCONV VeVector2TransformCoordStream(
	VE_FLOAT2* pOutputStream, VeSizeT OutputStride,
	const VE_FLOAT2* pInputStream, VeSizeT InputStride,
	VeSizeT VectorCount, VE_FMATRIX M) noexcept
{
	VE_ASSERT(pOutputStream != nullptr);
	VE_ASSERT(pInputStream != nullptr);
	VE_ASSERT(InputStride >= sizeof(VE_FLOAT2));
	VE_ASSERT(OutputStride >= sizeof(VE_FLOAT2));
#	if defined(VE_NO_INTRINSICS)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row3 = M.r[3];
	for (VeSizeT i = 0; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat2((const VE_FLOAT2*)pInputVector);
		VE_VECTOR Y = VeVectorSplatY(V);
		VE_VECTOR X = VeVectorSplatX(V);
		VE_VECTOR Result = VeVectorMultiplyAdd(Y, row1, row3);
		Result = VeVectorMultiplyAdd(X, row0, Result);
		VE_VECTOR W = VeVectorSplatW(Result);
		Result = VeVectorDivide(Result, W);
		VeStoreFloat2((VE_FLOAT2*)pOutputVector, Result);
		pInputVector += InputStride;
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	elif defined(VE_ENABLE_SSE)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row3 = M.r[3];
	VeSizeT i = 0;
	VeSizeT two = VectorCount >> 1;
	if (two > 0)
	{
		if (InputStride == sizeof(VE_FLOAT2))
		{
			if (OutputStride == sizeof(VE_FLOAT2))
			{
				if (!((VeSizeT)pOutputStream & 0xF))
				{
					for (size_t j = 0; j < two; ++j)
					{
						VE_VECTOR V = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						pInputVector += sizeof(VE_FLOAT2) * 2;
						VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
						VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						VE_VECTOR V1 = _mm_div_ps(vTemp, W);
						Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
						X = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						vTemp = _mm_mul_ps(Y, row1);
						vTemp2 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						VE_VECTOR V2 = _mm_div_ps(vTemp, W);
						vTemp = _mm_movelh_ps(V1, V2);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
						pOutputVector += sizeof(VE_FLOAT2) * 2;
						i += 2;
					}
				}
				else
				{
					for (VeSizeT j = 0; j < two; ++j)
					{
						VE_VECTOR V = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						pInputVector += sizeof(VE_FLOAT2) * 2;
						VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
						VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						VE_VECTOR V1 = _mm_div_ps(vTemp, W);
						Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
						X = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						vTemp = _mm_mul_ps(Y, row1);
						vTemp2 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						VE_VECTOR V2 = _mm_div_ps(vTemp, W);
						vTemp = _mm_movelh_ps(V1, V2);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
						pOutputVector += sizeof(VE_FLOAT2) * 2;
						i += 2;
					}
				}
			}
			else
			{
				for (VeSizeT j = 0; j < two; ++j)
				{
					VE_VECTOR V = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
					pInputVector += sizeof(VE_FLOAT2) * 2;
					VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
					VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
					VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					vTemp2 = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
					_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
					Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
					X = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
					vTemp = _mm_mul_ps(Y, row1);
					vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					vTemp2 = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
					_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
					i += 2;
				}
			}
		}
	}
	if (!((VeSizeT)pInputVector & 0xF) && !(InputStride & 0xF))
	{
		for (; i < VectorCount; i++)
		{
			VE_VECTOR V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
			pInputVector += InputStride;
			VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
			VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
			vTemp = _mm_add_ps(vTemp, row3);
			vTemp = _mm_add_ps(vTemp, vTemp2);
			VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
			vTemp = _mm_div_ps(vTemp, W);
			vTemp2 = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
			_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
			pOutputVector += OutputStride;
		}
	}
	else
	{
		for (; i < VectorCount; i++)
		{
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pInputVector));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pInputVector + 4));
			pInputVector += InputStride;
			VE_VECTOR Y = VE_PERMUTE_PS(y, _MM_SHUFFLE(0, 0, 0, 0));
			VE_VECTOR X = VE_PERMUTE_PS(x, _MM_SHUFFLE(0, 0, 0, 0));
			VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
			VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
			vTemp = _mm_add_ps(vTemp, row3);
			vTemp = _mm_add_ps(vTemp, vTemp2);
			VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
			vTemp = _mm_div_ps(vTemp, W);
			vTemp2 = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
			_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector + 4), vTemp2);
			pOutputVector += OutputStride;
		}
	}
	return pOutputStream;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector2TransformNormal(
	VE_FVECTOR V, VE_FMATRIX M) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Y = VeVectorSplatY(V);
	VE_VECTOR X = VeVectorSplatX(V);
	VE_VECTOR Result = VeVectorMultiply(Y, M.r[1]);
	Result = VeVectorMultiplyAdd(X, M.r[0], Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
	vResult = _mm_mul_ps(vResult, M.r[0]);
	VE_VECTOR vTemp = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	vTemp = _mm_mul_ps(vTemp, M.r[1]);
	vResult = _mm_add_ps(vResult, vTemp);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_FLOAT2* VE_MATH_CALLCONV VeVector2TransformNormalStream(
	VE_FLOAT2* pOutputStream, VeSizeT OutputStride,
	const VE_FLOAT2* pInputStream, VeSizeT InputStride,
	VeSizeT VectorCount, VE_FMATRIX M) noexcept
{
	VE_ASSERT(pOutputStream != nullptr);
	VE_ASSERT(pInputStream != nullptr);
	VE_ASSERT(InputStride >= sizeof(VE_FLOAT2));
	VE_ASSERT(OutputStride >= sizeof(VE_FLOAT2));
#	if defined(VE_NO_INTRINSICS)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	for (VeSizeT i = 0; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat2((const VE_FLOAT2*)pInputVector);
		VE_VECTOR Y = VeVectorSplatY(V);
		VE_VECTOR X = VeVectorSplatX(V);
		VE_VECTOR Result = VeVectorMultiply(Y, row1);
		Result = VeVectorMultiplyAdd(X, row0, Result);
		VeStoreFloat2((VE_FLOAT2*)pOutputVector, Result);
		pInputVector += InputStride;
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	elif defined(VE_ENABLE_SSE)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	VeSizeT i = 0;
	VeSizeT two = VectorCount >> 1;
	if (two > 0)
	{
		if (InputStride == sizeof(VE_FLOAT2))
		{
			if (OutputStride == sizeof(VE_FLOAT2))
			{
				if (!((VeSizeT)pOutputStream & 0xF))
				{
					for (size_t j = 0; j < two; ++j)
					{
						VE_VECTOR V = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						pInputVector += sizeof(VE_FLOAT2) * 2;
						VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
						VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
						VE_VECTOR V1 = _mm_add_ps(vTemp, vTemp2);
						Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
						X = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						vTemp = _mm_mul_ps(Y, row1);
						vTemp2 = _mm_mul_ps(X, row0);
						VE_VECTOR V2 = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_movelh_ps(V1, V2);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
						pOutputVector += sizeof(VE_FLOAT2) * 2;
						i += 2;
					}
				}
				else
				{
					for (VeSizeT j = 0; j < two; ++j)
					{
						VE_VECTOR V = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						pInputVector += sizeof(VE_FLOAT2) * 2;
						VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
						VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
						VE_VECTOR V1 = _mm_add_ps(vTemp, vTemp2);
						Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
						X = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						vTemp = _mm_mul_ps(Y, row1);
						vTemp2 = _mm_mul_ps(X, row0);
						VE_VECTOR V2 = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_movelh_ps(V1, V2);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
						pOutputVector += sizeof(VE_FLOAT2) * 2;
						i += 2;
					}
				}
			}
			else
			{
				for (VeSizeT j = 0; j < two; ++j)
				{
					VE_VECTOR V = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
					pInputVector += sizeof(VE_FLOAT2) * 2;
					VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
					VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
					VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp2 = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
					_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
					Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
					X = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
					vTemp = _mm_mul_ps(Y, row1);
					vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp2 = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
					_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
					i += 2;
				}
			}
		}
	}
	if (!((VeSizeT)pInputVector & 0xF) && !(InputStride & 0xF))
	{
		for (; i < VectorCount; i++)
		{
			VE_VECTOR V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
			pInputVector += InputStride;
			VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
			VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
			vTemp = _mm_add_ps(vTemp, vTemp2);
			vTemp2 = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
			_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
			pOutputVector += OutputStride;
		}
	}
	else
	{
		for (; i < VectorCount; i++)
		{
			__m128 x = _mm_load_ss(reinterpret_cast<const VeFloat32*>(pInputVector));
			__m128 y = _mm_load_ss(reinterpret_cast<const VeFloat32*>(pInputVector + 4));
			pInputVector += InputStride;
			VE_VECTOR Y = VE_PERMUTE_PS(y, _MM_SHUFFLE(0, 0, 0, 0));
			VE_VECTOR X = VE_PERMUTE_PS(x, _MM_SHUFFLE(0, 0, 0, 0));
			VE_VECTOR vTemp = _mm_mul_ps(Y, row1);
			VE_VECTOR vTemp2 = _mm_mul_ps(X, row0);
			vTemp = _mm_add_ps(vTemp, vTemp2);
			vTemp2 = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
			_mm_store_ss(reinterpret_cast<VeFloat32*>(pOutputVector + 4), vTemp2);
			pOutputVector += OutputStride;
		}
	}

	return pOutputStream;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3Equal(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] == V2.vector4_f32[0]) && (V1.vector4_f32[1] == V2.vector4_f32[1]) && (V1.vector4_f32[2] == V2.vector4_f32[2])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpeq_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector3EqualR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if ((V1.vector4_f32[0] == V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] == V2.vector4_f32[1]) &&
		(V1.vector4_f32[2] == V2.vector4_f32[2]))
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if ((V1.vector4_f32[0] != V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] != V2.vector4_f32[1]) &&
		(V1.vector4_f32[2] != V2.vector4_f32[2]))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpeq_ps(V1, V2);
	int iTest = _mm_movemask_ps(vTemp) & 7;
	VeUInt32 CR = 0;
	if (iTest == 7)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3EqualInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_u32[0] == V2.vector4_u32[0]) && (V1.vector4_u32[1] == V2.vector4_u32[1]) && (V1.vector4_u32[2] == V2.vector4_u32[2])) != 0);
#elif defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 7) == 7) != 0);
#endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector3EqualIntR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if ((V1.vector4_u32[0] == V2.vector4_u32[0]) &&
		(V1.vector4_u32[1] == V2.vector4_u32[1]) &&
		(V1.vector4_u32[2] == V2.vector4_u32[2]))
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if ((V1.vector4_u32[0] != V2.vector4_u32[0]) &&
		(V1.vector4_u32[1] != V2.vector4_u32[1]) &&
		(V1.vector4_u32[2] != V2.vector4_u32[2]))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	VeInt32 iTemp = _mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 7;
	VeUInt32 CR = 0;
	if (iTemp == 7)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTemp)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3NearEqual(
	VE_FVECTOR V1, VE_FVECTOR V2, VE_FVECTOR Epsilon) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 dx, dy, dz;
	dx = VeFabsf(V1.vector4_f32[0] - V2.vector4_f32[0]);
	dy = VeFabsf(V1.vector4_f32[1] - V2.vector4_f32[1]);
	dz = VeFabsf(V1.vector4_f32[2] - V2.vector4_f32[2]);
	return (((dx <= Epsilon.vector4_f32[0]) &&
		(dy <= Epsilon.vector4_f32[1]) &&
		(dz <= Epsilon.vector4_f32[2])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vDelta = _mm_sub_ps(V1, V2);
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_sub_ps(vTemp, vDelta);
	vTemp = _mm_max_ps(vTemp, vDelta);
	vTemp = _mm_cmple_ps(vTemp, Epsilon);
	return (((_mm_movemask_ps(vTemp) & 7) == 0x7) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3NotEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] != V2.vector4_f32[0]) || (V1.vector4_f32[1] != V2.vector4_f32[1]) || (V1.vector4_f32[2] != V2.vector4_f32[2])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpeq_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 7) != 7) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3NotEqualInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_u32[0] != V2.vector4_u32[0]) || (V1.vector4_u32[1] != V2.vector4_u32[1]) || (V1.vector4_u32[2] != V2.vector4_u32[2])) != 0);
#	elif defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 7) != 7) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3Greater(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] > V2.vector4_f32[0]) && (V1.vector4_f32[1] > V2.vector4_f32[1]) && (V1.vector4_f32[2] > V2.vector4_f32[2])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpgt_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector3GreaterR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if ((V1.vector4_f32[0] > V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] > V2.vector4_f32[1]) &&
		(V1.vector4_f32[2] > V2.vector4_f32[2]))
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if ((V1.vector4_f32[0] <= V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] <= V2.vector4_f32[1]) &&
		(V1.vector4_f32[2] <= V2.vector4_f32[2]))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpgt_ps(V1, V2);
	VeUInt32 CR = 0;
	int iTest = _mm_movemask_ps(vTemp) & 7;
	if (iTest == 7)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3GreaterOrEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] >= V2.vector4_f32[0]) && (V1.vector4_f32[1] >= V2.vector4_f32[1]) && (V1.vector4_f32[2] >= V2.vector4_f32[2])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpge_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector3GreaterOrEqualR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if ((V1.vector4_f32[0] >= V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] >= V2.vector4_f32[1]) &&
		(V1.vector4_f32[2] >= V2.vector4_f32[2]))
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if ((V1.vector4_f32[0] < V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] < V2.vector4_f32[1]) &&
		(V1.vector4_f32[2] < V2.vector4_f32[2]))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpge_ps(V1, V2);
	VeUInt32 CR = 0;
	VeInt32 iTest = _mm_movemask_ps(vTemp) & 7;
	if (iTest == 7)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3Less(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] < V2.vector4_f32[0]) && (V1.vector4_f32[1] < V2.vector4_f32[1]) && (V1.vector4_f32[2] < V2.vector4_f32[2])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmplt_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3LessOrEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] <= V2.vector4_f32[0]) && (V1.vector4_f32[1] <= V2.vector4_f32[1]) && (V1.vector4_f32[2] <= V2.vector4_f32[2])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmple_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3InBounds(
	VE_FVECTOR V, VE_FVECTOR Bounds) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) &&
		(V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) &&
		(V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp1 = _mm_cmple_ps(V, Bounds);
	VE_VECTOR vTemp2 = _mm_mul_ps(Bounds, g_MathNegativeOne);
	vTemp2 = _mm_cmple_ps(vTemp2, V);
	vTemp1 = _mm_and_ps(vTemp1, vTemp2);
	return (((_mm_movemask_ps(vTemp1) & 0x7) == 0x7) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3IsNaN(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (VEISNAN(V.vector4_f32[0]) ||
		VEISNAN(V.vector4_f32[1]) ||
		VEISNAN(V.vector4_f32[2]));
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTempNan = _mm_cmpneq_ps(V, V);
	return ((_mm_movemask_ps(vTempNan) & 7) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector3IsInfinite(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (VEISINF(V.vector4_f32[0]) ||
		VEISINF(V.vector4_f32[1]) ||
		VEISINF(V.vector4_f32[2]));
#	elif defined(VE_ENABLE_SSE)
	__m128 vTemp = _mm_and_ps(V, g_MathAbsMask);
	vTemp = _mm_cmpeq_ps(vTemp, g_MathInfinity);
	return ((_mm_movemask_ps(vTemp) & 7) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3Dot(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	float fValue = V1.vector4_f32[0] * V2.vector4_f32[0] + V1.vector4_f32[1] * V2.vector4_f32[1] + V1.vector4_f32[2] * V2.vector4_f32[2];
	VE_VECTOR vResult;
	vResult.vector4_f32[0] =
		vResult.vector4_f32[1] =
		vResult.vector4_f32[2] =
		vResult.vector4_f32[3] = fValue;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vDot = _mm_mul_ps(V1, V2);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
	vDot = _mm_add_ss(vDot, vTemp);
	vTemp = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vDot = _mm_add_ss(vDot, vTemp);
	return VE_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3Cross(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR vResult = {
		(V1.vector4_f32[1] * V2.vector4_f32[2]) - (V1.vector4_f32[2] * V2.vector4_f32[1]),
		(V1.vector4_f32[2] * V2.vector4_f32[0]) - (V1.vector4_f32[0] * V2.vector4_f32[2]),
		(V1.vector4_f32[0] * V2.vector4_f32[1]) - (V1.vector4_f32[1] * V2.vector4_f32[0]),
		0.0f
	};
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp1 = VE_PERMUTE_PS(V1, _MM_SHUFFLE(3, 0, 2, 1));
	VE_VECTOR vTemp2 = VE_PERMUTE_PS(V2, _MM_SHUFFLE(3, 1, 0, 2));
	VE_VECTOR vResult = _mm_mul_ps(vTemp1, vTemp2);
	vTemp1 = VE_PERMUTE_PS(vTemp1, _MM_SHUFFLE(3, 0, 2, 1));
	vTemp2 = VE_PERMUTE_PS(vTemp2, _MM_SHUFFLE(3, 1, 0, 2));
	vTemp1 = _mm_mul_ps(vTemp1, vTemp2);
	vResult = _mm_sub_ps(vResult, vTemp1);
	return _mm_and_ps(vResult, g_MathMask3);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3LengthSq(
	VE_FVECTOR V) noexcept
{
	return VeVector3Dot(V, V);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3ReciprocalLengthEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector3LengthSq(V);
	Result = VeVectorReciprocalSqrtEst(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vTemp = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	vLengthSq = _mm_rsqrt_ps(vLengthSq);
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3ReciprocalLength(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector3LengthSq(V);
	Result = VeVectorReciprocalSqrt(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vDot = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
	vDot = _mm_add_ss(vDot, vTemp);
	vTemp = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vDot = _mm_add_ss(vDot, vTemp);
	vDot = VE_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
	vDot = _mm_sqrt_ps(vDot);
	vDot = _mm_div_ps(g_MathOne, vDot);
	return vDot;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3LengthEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector3LengthSq(V);
	Result = VeVectorSqrtEst(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vTemp = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	vLengthSq = _mm_sqrt_ps(vLengthSq);
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3Length(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector3LengthSq(V);
	Result = VeVectorSqrt(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vTemp = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	vLengthSq = _mm_sqrt_ps(vLengthSq);
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3NormalizeEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector3ReciprocalLength(V);
	Result = VeVectorMultiply(V, Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vDot = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
	vDot = _mm_add_ss(vDot, vTemp);
	vTemp = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vDot = _mm_add_ss(vDot, vTemp);
	vDot = VE_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
	vDot = _mm_rsqrt_ps(vDot);
	vDot = _mm_mul_ps(vDot, V);
	return vDot;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3Normalize(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fLength;
	VE_VECTOR vResult;
	vResult = VeVector3Length(V);
	fLength = vResult.vector4_f32[0];
	if (fLength > 0)
	{
		fLength = 1.0f / fLength;
	}
	vResult.vector4_f32[0] = V.vector4_f32[0] * fLength;
	vResult.vector4_f32[1] = V.vector4_f32[1] * fLength;
	vResult.vector4_f32[2] = V.vector4_f32[2] * fLength;
	vResult.vector4_f32[3] = V.vector4_f32[3] * fLength;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vTemp = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	VE_VECTOR vResult = _mm_sqrt_ps(vLengthSq);
	VE_VECTOR vZeroMask = _mm_setzero_ps();
	vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
	vLengthSq = _mm_cmpneq_ps(vLengthSq, g_MathInfinity);
	vResult = _mm_div_ps(V, vResult);
	vResult = _mm_and_ps(vResult, vZeroMask);
	VE_VECTOR vTemp1 = _mm_andnot_ps(vLengthSq, g_MathQNaN);
	VE_VECTOR vTemp2 = _mm_and_ps(vResult, vLengthSq);
	vResult = _mm_or_ps(vTemp1, vTemp2);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3ClampLengthV(
	VE_FVECTOR V, VE_FVECTOR LengthMin, VE_FVECTOR LengthMax) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3ClampLength(
	VE_FVECTOR V, VeFloat32 LengthMin, VeFloat32 LengthMax) noexcept
{
	VE_VECTOR ClampMax = VeVectorReplicate(LengthMax);
	VE_VECTOR ClampMin = VeVectorReplicate(LengthMin);
	return VeVector3ClampLengthV(V, ClampMin, ClampMax);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3ClampLengthV(
	VE_FVECTOR V, VE_FVECTOR LengthMin, VE_FVECTOR LengthMax) noexcept
{
	VE_ASSERT((VeVectorGetY(LengthMin) == VeVectorGetX(LengthMin)) && (VeVectorGetZ(LengthMin) == VeVectorGetX(LengthMin)));
	VE_ASSERT((VeVectorGetY(LengthMax) == VeVectorGetX(LengthMax)) && (VeVectorGetZ(LengthMax) == VeVectorGetX(LengthMax)));
	VE_ASSERT(VeVector3GreaterOrEqual(LengthMin, VeVectorZero()));
	VE_ASSERT(VeVector3GreaterOrEqual(LengthMax, VeVectorZero()));
	VE_ASSERT(VeVector3GreaterOrEqual(LengthMax, LengthMin));
	VE_VECTOR LengthSq = VeVector3LengthSq(V);
	const VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR RcpLength = VeVectorReciprocalSqrt(LengthSq);
	VE_VECTOR InfiniteLength = VeVectorEqualInt(LengthSq, g_MathInfinity.v);
	VE_VECTOR ZeroLength = VeVectorEqual(LengthSq, Zero);
	VE_VECTOR Normal = VeVectorMultiply(V, RcpLength);
	VE_VECTOR Length = VeVectorMultiply(LengthSq, RcpLength);
	VE_VECTOR Select = VeVectorEqualInt(InfiniteLength, ZeroLength);
	Length = VeVectorSelect(LengthSq, Length, Select);
	Normal = VeVectorSelect(LengthSq, Normal, Select);
	VE_VECTOR ControlMax = VeVectorGreater(Length, LengthMax);
	VE_VECTOR ControlMin = VeVectorLess(Length, LengthMin);
	VE_VECTOR ClampLength = VeVectorSelect(Length, LengthMax, ControlMax);
	ClampLength = VeVectorSelect(ClampLength, LengthMin, ControlMin);
	VE_VECTOR Result = VeVectorMultiply(Normal, ClampLength);
	VE_VECTOR Control = VeVectorEqualInt(ControlMax, ControlMin);
	Result = VeVectorSelect(Result, V, Control);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3Reflect(
	VE_FVECTOR Incident, VE_FVECTOR Normal) noexcept
{
	VE_VECTOR Result = VeVector3Dot(Incident, Normal);
	Result = VeVectorAdd(Result, Result);
	Result = VeVectorNegativeMultiplySubtract(Result, Normal, Incident);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3RefractV(VE_FVECTOR Incident,
	VE_FVECTOR Normal, VE_FVECTOR RefractionIndex) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3Refract(VE_FVECTOR Incident,
	VE_FVECTOR Normal, VeFloat32 RefractionIndex) noexcept
{
	VE_VECTOR Index = VeVectorReplicate(RefractionIndex);
	return VeVector3RefractV(Incident, Normal, Index);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3RefractV(VE_FVECTOR Incident,
	VE_FVECTOR Normal, VE_FVECTOR RefractionIndex) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	const VE_VECTOR  Zero = VeVectorZero();
	VE_VECTOR IDotN = VeVector3Dot(Incident, Normal);
	VE_VECTOR R = VeVectorNegativeMultiplySubtract(IDotN, IDotN, g_MathOne.v);
	R = VeVectorMultiply(R, RefractionIndex);
	R = VeVectorNegativeMultiplySubtract(R, RefractionIndex, g_MathOne.v);
	if (VeVector4LessOrEqual(R, Zero))
	{
		return Zero;
	}
	else
	{
		R = VeVectorSqrt(R);
		R = VeVectorMultiplyAdd(RefractionIndex, IDotN, R);
		VE_VECTOR Result = VeVectorMultiply(RefractionIndex, Incident);
		Result = VeVectorNegativeMultiplySubtract(Normal, R, Result);
		return Result;
	}
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR IDotN = VeVector3Dot(Incident, Normal);
	VE_VECTOR R = _mm_mul_ps(IDotN, IDotN);
	R = _mm_sub_ps(g_MathOne, R);
	R = _mm_mul_ps(R, RefractionIndex);
	R = _mm_mul_ps(R, RefractionIndex);
	R = _mm_sub_ps(g_MathOne, R);
	VE_VECTOR vResult = _mm_cmple_ps(R, g_MathZero);
	if (_mm_movemask_ps(vResult) == 0x0f)
	{
		vResult = g_MathZero;
	}
	else
	{
		R = _mm_sqrt_ps(R);
		vResult = _mm_mul_ps(RefractionIndex, IDotN);
		R = _mm_add_ps(R, vResult);
		vResult = _mm_mul_ps(RefractionIndex, Incident);
		R = _mm_mul_ps(R, Normal);
		vResult = _mm_sub_ps(vResult, R);
	}
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3Orthogonal(
	VE_FVECTOR V) noexcept
{
	VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR Z = VeVectorSplatZ(V);
	VE_VECTOR YZYY = VeVectorSwizzle<VE_SWIZZLE_Y, VE_SWIZZLE_Z, VE_SWIZZLE_Y, VE_SWIZZLE_Y>(V);
	VE_VECTOR NegativeV = VeVectorSubtract(Zero, V);
	VE_VECTOR ZIsNegative = VeVectorLess(Z, Zero);
	VE_VECTOR YZYYIsNegative = VeVectorLess(YZYY, Zero);
	VE_VECTOR S = VeVectorAdd(YZYY, Z);
	VE_VECTOR D = VeVectorSubtract(YZYY, Z);
	VE_VECTOR Select = VeVectorEqualInt(ZIsNegative, YZYYIsNegative);
	VE_VECTOR R0 = VeVectorPermute<VE_PERMUTE_1X, VE_PERMUTE_0X, VE_PERMUTE_0X, VE_PERMUTE_0X>(NegativeV, S);
	VE_VECTOR R1 = VeVectorPermute<VE_PERMUTE_1X, VE_PERMUTE_0X, VE_PERMUTE_0X, VE_PERMUTE_0X>(V, D);
	return VeVectorSelect(R1, R0, Select);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3AngleBetweenNormalsEst(
	VE_FVECTOR N1, VE_FVECTOR N2) noexcept
{
	VE_VECTOR Result = VeVector3Dot(N1, N2);
	Result = VeVectorClamp(Result, g_MathNegativeOne.v, g_MathOne.v);
	Result = VeVectorACosEst(Result);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3AngleBetweenNormals(
	VE_FVECTOR N1, VE_FVECTOR N2) noexcept
{
	VE_VECTOR Result = VeVector3Dot(N1, N2);
	Result = VeVectorClamp(Result, g_MathNegativeOne.v, g_MathOne.v);
	Result = VeVectorACos(Result);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3AngleBetweenVectors(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	VE_VECTOR L1 = VeVector3ReciprocalLength(V1);
	VE_VECTOR L2 = VeVector3ReciprocalLength(V2);
	VE_VECTOR Dot = VeVector3Dot(V1, V2);
	L1 = VeVectorMultiply(L1, L2);
	VE_VECTOR CosAngle = VeVectorMultiply(Dot, L1);
	CosAngle = VeVectorClamp(CosAngle, g_MathNegativeOne.v, g_MathOne.v);
	return VeVectorACos(CosAngle);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3LinePointDistance(
	VE_FVECTOR LinePoint1, VE_FVECTOR LinePoint2, VE_FVECTOR Point) noexcept
{
	VE_VECTOR PointVector = VeVectorSubtract(Point, LinePoint1);
	VE_VECTOR LineVector = VeVectorSubtract(LinePoint2, LinePoint1);
	VE_VECTOR LengthSq = VeVector3LengthSq(LineVector);
	VE_VECTOR PointProjectionScale = VeVector3Dot(PointVector, LineVector);
	PointProjectionScale = VeVectorDivide(PointProjectionScale, LengthSq);
	VE_VECTOR DistanceVector = VeVectorMultiply(LineVector, PointProjectionScale);
	DistanceVector = VeVectorSubtract(PointVector, DistanceVector);
	return VeVector3Length(DistanceVector);
}
//--------------------------------------------------------------------------
inline void VE_MATH_CALLCONV VeVector3ComponentsFromNormal(
	VE_VECTOR* pParallel, VE_VECTOR* pPerpendicular, VE_FVECTOR  V,
	VE_FVECTOR  Normal) noexcept
{
	VE_ASSERT(pParallel != nullptr);
	VE_ASSERT(pPerpendicular != nullptr);
	VE_VECTOR Scale = VeVector3Dot(V, Normal);
	VE_VECTOR Parallel = VeVectorMultiply(Normal, Scale);
	*pParallel = Parallel;
	*pPerpendicular = VeVectorSubtract(V, Parallel);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionConjugate(
	VE_FVECTOR Q) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeQuaternionMultiply(
	VE_FVECTOR Q1, VE_FVECTOR Q2) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3Rotate(
	VE_FVECTOR V, VE_FVECTOR RotationQuaternion)
{
	VE_VECTOR A = VeVectorSelect(g_MathSelect1110.v, V, g_MathSelect1110.v);
	VE_VECTOR Q = VeQuaternionConjugate(RotationQuaternion);
	VE_VECTOR Result = VeQuaternionMultiply(Q, A);
	return VeQuaternionMultiply(Result, RotationQuaternion);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3InverseRotate(
	VE_FVECTOR V, VE_FVECTOR RotationQuaternion) noexcept
{
	VE_VECTOR A = VeVectorSelect(g_MathSelect1110.v, V, g_MathSelect1110.v);
	VE_VECTOR Result = VeQuaternionMultiply(RotationQuaternion, A);
	VE_VECTOR Q = VeQuaternionConjugate(RotationQuaternion);
	return VeQuaternionMultiply(Result, Q);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3Transform(
	VE_FVECTOR V, VE_FMATRIX M) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Z = VeVectorSplatZ(V);
	VE_VECTOR Y = VeVectorSplatY(V);
	VE_VECTOR X = VeVectorSplatX(V);
	VE_VECTOR Result = VeVectorMultiplyAdd(Z, M.r[2], M.r[3]);
	Result = VeVectorMultiplyAdd(Y, M.r[1], Result);
	Result = VeVectorMultiplyAdd(X, M.r[0], Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
	vResult = _mm_mul_ps(vResult, M.r[0]);
	VE_VECTOR vTemp = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	vTemp = _mm_mul_ps(vTemp, M.r[1]);
	vResult = _mm_add_ps(vResult, vTemp);
	vTemp = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	vTemp = _mm_mul_ps(vTemp, M.r[2]);
	vResult = _mm_add_ps(vResult, vTemp);
	vResult = _mm_add_ps(vResult, M.r[3]);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_FLOAT4* VE_MATH_CALLCONV VeVector3TransformStream(
	VE_FLOAT4* pOutputStream, VeSizeT OutputStride,
	const VE_FLOAT3* pInputStream, VeSizeT InputStride,
	VeSizeT VectorCount, VE_FMATRIX M) noexcept
{
	VE_ASSERT(pOutputStream != nullptr);
	VE_ASSERT(pInputStream != nullptr);
	VE_ASSERT(InputStride >= sizeof(VE_FLOAT3));
	VE_ASSERT(OutputStride >= sizeof(VE_FLOAT4));
#	if defined(VE_NO_INTRINSICS)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (uint8_t*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row2 = M.r[2];
	const VE_VECTOR row3 = M.r[3];
	for (VeSizeT i = 0; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat3((const VE_FLOAT3*)pInputVector);
		VE_VECTOR Z = VeVectorSplatZ(V);
		VE_VECTOR Y = VeVectorSplatY(V);
		VE_VECTOR X = VeVectorSplatX(V);
		VE_VECTOR Result = VeVectorMultiplyAdd(Z, row2, row3);
		Result = VeVectorMultiplyAdd(Y, row1, Result);
		Result = VeVectorMultiplyAdd(X, row0, Result);
		VeStoreFloat4((VE_FLOAT4*)pOutputVector, Result);
		pInputVector += InputStride;
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	elif defined(VE_ENABLE_SSE)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row2 = M.r[2];
	const VE_VECTOR row3 = M.r[3];
	VeSizeT i = 0;
	VeSizeT four = VectorCount >> 2;
	if (four > 0)
	{
		if (InputStride == sizeof(VE_FLOAT3))
		{
			if (!((VeSizeT)pOutputStream & 0xF) && !(OutputStride & 0xF))
			{
				for (VeSizeT j = 0; j < four; ++j)
				{
					__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
					__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
					__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
					pInputVector += sizeof(VE_FLOAT3) * 4;
					VE3UNPACK3INTO4(V1, L2, L3);
					VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
					VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
					VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
					VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
					VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
					VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					VE_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					i += 4;
				}
			}
			else
			{
				for (VeSizeT j = 0; j < four; ++j)
				{
					__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
					__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
					__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
					pInputVector += sizeof(VE_FLOAT3) * 4;
					VE3UNPACK3INTO4(V1, L2, L3);
					VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
					VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
					VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
					VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
					VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
					VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					i += 4;
				}
			}
		}
	}
	if (!((VeSizeT)pOutputStream & 0xF) && !(OutputStride & 0xF))
	{
		for (; i < VectorCount; ++i)
		{
			VE_VECTOR V = VeLoadFloat3(reinterpret_cast<const VE_FLOAT3*>(pInputVector));
			pInputVector += InputStride;
			VE_VECTOR Z = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
			VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
			VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
			vTemp = _mm_add_ps(vTemp, row3);
			vTemp = _mm_add_ps(vTemp, vTemp2);
			vTemp = _mm_add_ps(vTemp, vTemp3);
			VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
			pOutputVector += OutputStride;
		}
	}
	else
	{
		for (; i < VectorCount; ++i)
		{
			VE_VECTOR V = VeLoadFloat3(reinterpret_cast<const VE_FLOAT3*>(pInputVector));
			pInputVector += InputStride;
			VE_VECTOR Z = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
			VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
			VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
			vTemp = _mm_add_ps(vTemp, row3);
			vTemp = _mm_add_ps(vTemp, vTemp2);
			vTemp = _mm_add_ps(vTemp, vTemp3);
			_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTemp);
			pOutputVector += OutputStride;
		}
	}
	return pOutputStream;
#	endif
}

//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3TransformCoord(
	VE_FVECTOR V, VE_FMATRIX M) noexcept
{
	VE_VECTOR Z = VeVectorSplatZ(V);
	VE_VECTOR Y = VeVectorSplatY(V);
	VE_VECTOR X = VeVectorSplatX(V);
	VE_VECTOR Result = VeVectorMultiplyAdd(Z, M.r[2], M.r[3]);
	Result = VeVectorMultiplyAdd(Y, M.r[1], Result);
	Result = VeVectorMultiplyAdd(X, M.r[0], Result);
	VE_VECTOR W = VeVectorSplatW(Result);
	return VeVectorDivide(Result, W);
}
//--------------------------------------------------------------------------
inline VE_FLOAT3* VE_MATH_CALLCONV VeVector3TransformCoordStream(
	VE_FLOAT3* pOutputStream, VeSizeT OutputStride,
	const VE_FLOAT3* pInputStream, VeSizeT InputStride,
	VeSizeT VectorCount, VE_FMATRIX M) noexcept
{
	VE_ASSERT(pOutputStream != nullptr);
	VE_ASSERT(pInputStream != nullptr);
	VE_ASSERT(InputStride >= sizeof(VE_FLOAT3));
	VE_ASSERT(OutputStride >= sizeof(VE_FLOAT3));
#	if defined(VE_NO_INTRINSICS)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row2 = M.r[2];
	const VE_VECTOR row3 = M.r[3];
	for (VeSizeT i = 0; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat3((const VE_FLOAT3*)pInputVector);
		VE_VECTOR Z = VeVectorSplatZ(V);
		VE_VECTOR Y = VeVectorSplatY(V);
		VE_VECTOR X = VeVectorSplatX(V);
		VE_VECTOR Result = VeVectorMultiplyAdd(Z, row2, row3);
		Result = VeVectorMultiplyAdd(Y, row1, Result);
		Result = VeVectorMultiplyAdd(X, row0, Result);
		VE_VECTOR W = VeVectorSplatW(Result);
		Result = VeVectorDivide(Result, W);
		VeStoreFloat3((VE_FLOAT3*)pOutputVector, Result);
		pInputVector += InputStride;
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	elif defined(VE_ENABLE_SSE)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row2 = M.r[2];
	const VE_VECTOR row3 = M.r[3];
	VeSizeT i = 0;
	VeSizeT four = VectorCount >> 2;
	if (four > 0)
	{
		if (InputStride == sizeof(VE_FLOAT3))
		{
			if (OutputStride == sizeof(VE_FLOAT3))
			{
				if (!((VeSizeT)pOutputStream & 0xF))
				{
					for (VeSizeT j = 0; j < four; ++j)
					{
						__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
						__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
						pInputVector += sizeof(VE_FLOAT3) << 2;
						VE3UNPACK3INTO4(V1, L2, L3);
						VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
						VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
						VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
						VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V1 = _mm_div_ps(vTemp, W);
						Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V2 = _mm_div_ps(vTemp, W);
						Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V3 = _mm_div_ps(vTemp, W);
						Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V4 = _mm_div_ps(vTemp, W);
						VE3PACK4INTO3(vTemp);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), V1);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector + 16), vTemp);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector + 32), V3);
						pOutputVector += sizeof(VE_FLOAT3) << 2;
						i += 4;
					}
				}
				else
				{
					for (VeSizeT j = 0; j < four; ++j)
					{
						__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
						__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
						pInputVector += sizeof(VE_FLOAT3) << 2;
						VE3UNPACK3INTO4(V1, L2, L3);
						VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
						VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
						VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
						VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V1 = _mm_div_ps(vTemp, W);
						Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V2 = _mm_div_ps(vTemp, W);
						Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V3 = _mm_div_ps(vTemp, W);
						Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V4 = _mm_div_ps(vTemp, W);
						VE3PACK4INTO3(vTemp);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), V1);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector + 16), vTemp);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector + 32), V3);
						pOutputVector += sizeof(VE_FLOAT3) * 4;
						i += 4;
					}
				}
			}
			else
			{
				for (VeSizeT j = 0; j < four; ++j)
				{
					__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
					__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
					__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
					pInputVector += sizeof(VE_FLOAT3) * 4;
					VE3UNPACK3INTO4(V1, L2, L3);
					VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
					VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
					VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
					VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
					VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
					VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					i += 4;
				}
			}
		}
	}
	for (; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat3(reinterpret_cast<const VE_FLOAT3*>(pInputVector));
		pInputVector += InputStride;
		VE_VECTOR Z = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
		VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
		VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
		VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
		VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
		VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
		vTemp = _mm_add_ps(vTemp, row3);
		vTemp = _mm_add_ps(vTemp, vTemp2);
		vTemp = _mm_add_ps(vTemp, vTemp3);
		VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
		vTemp = _mm_div_ps(vTemp, W);
		VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3TransformNormal(
	VE_FVECTOR V, VE_FMATRIX M) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Z = VeVectorSplatZ(V);
	VE_VECTOR Y = VeVectorSplatY(V);
	VE_VECTOR X = VeVectorSplatX(V);
	VE_VECTOR Result = VeVectorMultiply(Z, M.r[2]);
	Result = VeVectorMultiplyAdd(Y, M.r[1], Result);
	Result = VeVectorMultiplyAdd(X, M.r[0], Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
	vResult = _mm_mul_ps(vResult, M.r[0]);
	VE_VECTOR vTemp = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	vTemp = _mm_mul_ps(vTemp, M.r[1]);
	vResult = _mm_add_ps(vResult, vTemp);
	vTemp = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	vTemp = _mm_mul_ps(vTemp, M.r[2]);
	vResult = _mm_add_ps(vResult, vTemp);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_FLOAT3* VE_MATH_CALLCONV VeVector3TransformNormalStream(
	VE_FLOAT3* pOutputStream, VeSizeT OutputStride,
	const VE_FLOAT3* pInputStream, VeSizeT InputStride,
	VeSizeT VectorCount, VE_FMATRIX M) noexcept
{
	VE_ASSERT(pOutputStream != nullptr);
	VE_ASSERT(pInputStream != nullptr);
	VE_ASSERT(InputStride >= sizeof(VE_FLOAT3));
	VE_ASSERT(OutputStride >= sizeof(VE_FLOAT3));
#	if defined(VE_NO_INTRINSICS)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row2 = M.r[2];
	for (VeSizeT i = 0; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat3((const VE_FLOAT3*)pInputVector);
		VE_VECTOR Z = VeVectorSplatZ(V);
		VE_VECTOR Y = VeVectorSplatY(V);
		VE_VECTOR X = VeVectorSplatX(V);
		VE_VECTOR Result = VeVectorMultiply(Z, row2);
		Result = VeVectorMultiplyAdd(Y, row1, Result);
		Result = VeVectorMultiplyAdd(X, row0, Result);
		VeStoreFloat3((VE_FLOAT3*)pOutputVector, Result);
		pInputVector += InputStride;
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	elif defined(VE_ENABLE_SSE)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row2 = M.r[2];
	VeSizeT i = 0;
	VeSizeT four = VectorCount >> 2;
	if (four > 0)
	{
		if (InputStride == sizeof(VE_FLOAT3))
		{
			if (OutputStride == sizeof(VE_FLOAT3))
			{
				if (!((VeSizeT)pOutputStream & 0xF))
				{
					for (VeSizeT j = 0; j < four; ++j)
					{
						__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
						__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
						pInputVector += sizeof(VE_FLOAT3) * 4;
						VE3UNPACK3INTO4(V1, L2, L3);
						VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
						VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
						VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
						VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V1 = _mm_add_ps(vTemp, vTemp3);
						Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V2 = _mm_add_ps(vTemp, vTemp3);
						Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V3 = _mm_add_ps(vTemp, vTemp3);
						Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V4 = _mm_add_ps(vTemp, vTemp3);
						VE3PACK4INTO3(vTemp);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), V1);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector + 16), vTemp);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector + 32), V3);
						pOutputVector += sizeof(VE_FLOAT3) * 4;
						i += 4;
					}
				}
				else
				{
					for (VeSizeT j = 0; j < four; ++j)
					{
						__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
						__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
						pInputVector += sizeof(VE_FLOAT3) * 4;
						VE3UNPACK3INTO4(V1, L2, L3);
						VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
						VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
						VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
						VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V1 = _mm_add_ps(vTemp, vTemp3);
						Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V2 = _mm_add_ps(vTemp, vTemp3);
						Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V3 = _mm_add_ps(vTemp, vTemp3);
						Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V4 = _mm_add_ps(vTemp, vTemp3);
						VE3PACK4INTO3(vTemp);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), V1);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector + 16), vTemp);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector + 32), V3);
						pOutputVector += sizeof(VE_FLOAT3) * 4;
						i += 4;
					}
				}
			}
			else
			{
				for (VeSizeT j = 0; j < four; ++j)
				{
					__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
					__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
					__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
					pInputVector += sizeof(VE_FLOAT3) * 4;
					VE3UNPACK3INTO4(V1, L2, L3);
					VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
					VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
					VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
					VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
					VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
					VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					i += 4;
				}
			}
		}
	}
	for (; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat3(reinterpret_cast<const VE_FLOAT3*>(pInputVector));
		pInputVector += InputStride;
		VE_VECTOR Z = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
		VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
		VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
		VE_VECTOR vTemp = _mm_mul_ps(Z, row2);
		VE_VECTOR vTemp2 = _mm_mul_ps(Y, row1);
		VE_VECTOR vTemp3 = _mm_mul_ps(X, row0);
		vTemp = _mm_add_ps(vTemp, vTemp2);
		vTemp = _mm_add_ps(vTemp, vTemp3);
		VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixMultiply(
	VE_FMATRIX M1, VE_CMATRIX M2) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3Project(
	VE_FVECTOR V, VeFloat32 ViewportX, VeFloat32 ViewportY,
	VeFloat32 ViewportWidth, VeFloat32 ViewportHeight,
	VeFloat32 ViewportMinZ, VeFloat32 ViewportMaxZ, VE_FMATRIX Projection,
	VE_CMATRIX View, VE_CMATRIX World) noexcept
{
	const VeFloat32 HalfViewportWidth = ViewportWidth * 0.5f;
	const VeFloat32 HalfViewportHeight = ViewportHeight * 0.5f;
	VE_VECTOR Scale = VeVectorSet(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 0.0f);
	VE_VECTOR Offset = VeVectorSet(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);
	VE_MATRIX Transform = VeMatrixMultiply(World, View);
	Transform = VeMatrixMultiply(Transform, Projection);
	VE_VECTOR Result = VeVector3TransformCoord(V, Transform);
	Result = VeVectorMultiplyAdd(Result, Scale, Offset);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_FLOAT3* VE_MATH_CALLCONV VeVector3ProjectStream(
	VE_FLOAT3* pOutputStream, VeSizeT OutputStride,
	const VE_FLOAT3* pInputStream, VeSizeT InputStride,
	VeSizeT VectorCount, VeFloat32 ViewportX, VeFloat32 ViewportY,
	VeFloat32 ViewportWidth, VeFloat32 ViewportHeight,
	VeFloat32 ViewportMinZ, VeFloat32 ViewportMaxZ,
	VE_FMATRIX Projection, VE_CMATRIX View, VE_CMATRIX World) noexcept
{
	VE_ASSERT(pOutputStream != nullptr);
	VE_ASSERT(pInputStream != nullptr);
	VE_ASSERT(InputStride >= sizeof(VE_FLOAT3));
	VE_ASSERT(OutputStride >= sizeof(VE_FLOAT3));
#	if defined(VE_NO_INTRINSICS)
	const VeFloat32 HalfViewportWidth = ViewportWidth * 0.5f;
	const VeFloat32 HalfViewportHeight = ViewportHeight * 0.5f;
	VE_VECTOR Scale = VeVectorSet(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 1.0f);
	VE_VECTOR Offset = VeVectorSet(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);
	VE_MATRIX Transform = VeMatrixMultiply(World, View);
	Transform = VeMatrixMultiply(Transform, Projection);
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	for (VeSizeT i = 0; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat3((const VE_FLOAT3*)pInputVector);
		VE_VECTOR Result = VeVector3TransformCoord(V, Transform);
		Result = VeVectorMultiplyAdd(Result, Scale, Offset);
		VeStoreFloat3((VE_FLOAT3*)pOutputVector, Result);
		pInputVector += InputStride;
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	elif defined(VE_ENABLE_SSE)
	const VeFloat32 HalfViewportWidth = ViewportWidth * 0.5f;
	const VeFloat32 HalfViewportHeight = ViewportHeight * 0.5f;
	VE_VECTOR Scale = VeVectorSet(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 1.0f);
	VE_VECTOR Offset = VeVectorSet(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);
	VE_MATRIX Transform = VeMatrixMultiply(World, View);
	Transform = VeMatrixMultiply(Transform, Projection);
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	VeSizeT i = 0;
	VeSizeT four = VectorCount >> 2;
	if (four > 0)
	{
		if (InputStride == sizeof(VE_FLOAT3))
		{
			if (OutputStride == sizeof(VE_FLOAT3))
			{
				if (!((VeSizeT)pOutputStream & 0xF))
				{
					for (VeSizeT j = 0; j < four; ++j)
					{
						__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
						__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
						pInputVector += sizeof(VE_FLOAT3) * 4;
						VE3UNPACK3INTO4(V1, L2, L3);
						VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
						VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Z, Transform.r[2]);
						VE_VECTOR vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						VE_VECTOR vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						vTemp = _mm_div_ps(vTemp, W);
						vTemp = _mm_mul_ps(vTemp, Scale);
						V1 = _mm_add_ps(vTemp, Offset);
						Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						vTemp = _mm_div_ps(vTemp, W);
						vTemp = _mm_mul_ps(vTemp, Scale);
						V2 = _mm_add_ps(vTemp, Offset);
						Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						vTemp = _mm_div_ps(vTemp, W);
						vTemp = _mm_mul_ps(vTemp, Scale);
						V3 = _mm_add_ps(vTemp, Offset);
						Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						vTemp = _mm_div_ps(vTemp, W);
						vTemp = _mm_mul_ps(vTemp, Scale);
						V4 = _mm_add_ps(vTemp, Offset);
						VE3PACK4INTO3(vTemp);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), V1);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector + 16), vTemp);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector + 32), V3);
						pOutputVector += sizeof(VE_FLOAT3) * 4;
						i += 4;
					}
				}
				else
				{
					for (VeSizeT j = 0; j < four; ++j)
					{
						__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
						__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
						pInputVector += sizeof(VE_FLOAT3) * 4;
						VE3UNPACK3INTO4(V1, L2, L3);
						VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
						VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Z, Transform.r[2]);
						VE_VECTOR vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						VE_VECTOR vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						vTemp = _mm_div_ps(vTemp, W);
						vTemp = _mm_mul_ps(vTemp, Scale);
						V1 = _mm_add_ps(vTemp, Offset);
						Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						vTemp = _mm_div_ps(vTemp, W);
						vTemp = _mm_mul_ps(vTemp, Scale);
						V2 = _mm_add_ps(vTemp, Offset);
						Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						vTemp = _mm_div_ps(vTemp, W);
						vTemp = _mm_mul_ps(vTemp, Scale);
						V3 = _mm_add_ps(vTemp, Offset);
						Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						vTemp = _mm_div_ps(vTemp, W);
						vTemp = _mm_mul_ps(vTemp, Scale);
						V4 = _mm_add_ps(vTemp, Offset);
						VE3PACK4INTO3(vTemp);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), V1);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector + 16), vTemp);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector + 32), V3);
						pOutputVector += sizeof(VE_FLOAT3) * 4;
						i += 4;
					}
				}
			}
			else
			{
				for (VeSizeT j = 0; j < four; ++j)
				{
					__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
					__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
					__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
					pInputVector += sizeof(VE_FLOAT3) * 4;
					VE3UNPACK3INTO4(V1, L2, L3);
					VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
					VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
					VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
					VE_VECTOR vTemp = _mm_mul_ps(Z, Transform.r[2]);
					VE_VECTOR vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
					VE_VECTOR vTemp3 = _mm_mul_ps(X, Transform.r[0]);
					vTemp = _mm_add_ps(vTemp, Transform.r[3]);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					vTemp = _mm_mul_ps(vTemp, Scale);
					vTemp = _mm_add_ps(vTemp, Offset);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, Transform.r[2]);
					vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
					vTemp3 = _mm_mul_ps(X, Transform.r[0]);
					vTemp = _mm_add_ps(vTemp, Transform.r[3]);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					vTemp = _mm_mul_ps(vTemp, Scale);
					vTemp = _mm_add_ps(vTemp, Offset);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, Transform.r[2]);
					vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
					vTemp3 = _mm_mul_ps(X, Transform.r[0]);
					vTemp = _mm_add_ps(vTemp, Transform.r[3]);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					vTemp = _mm_mul_ps(vTemp, Scale);
					vTemp = _mm_add_ps(vTemp, Offset);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, Transform.r[2]);
					vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
					vTemp3 = _mm_mul_ps(X, Transform.r[0]);
					vTemp = _mm_add_ps(vTemp, Transform.r[3]);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					vTemp = _mm_mul_ps(vTemp, Scale);
					vTemp = _mm_add_ps(vTemp, Offset);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					i += 4;
				}
			}
		}
	}
	for (; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat3(reinterpret_cast<const VE_FLOAT3*>(pInputVector));
		pInputVector += InputStride;
		VE_VECTOR Z = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
		VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
		VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
		VE_VECTOR vTemp = _mm_mul_ps(Z, Transform.r[2]);
		VE_VECTOR vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
		VE_VECTOR vTemp3 = _mm_mul_ps(X, Transform.r[0]);
		vTemp = _mm_add_ps(vTemp, Transform.r[3]);
		vTemp = _mm_add_ps(vTemp, vTemp2);
		vTemp = _mm_add_ps(vTemp, vTemp3);
		VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
		vTemp = _mm_div_ps(vTemp, W);
		vTemp = _mm_mul_ps(vTemp, Scale);
		vTemp = _mm_add_ps(vTemp, Offset);
		VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	endif
}
//--------------------------------------------------------------------------
inline VE_MATRIX VE_MATH_CALLCONV VeMatrixInverse(
	VE_VECTOR* pDeterminant, VE_FMATRIX M) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector3Unproject(
	VE_FVECTOR V, VeFloat32 ViewportX, VeFloat32 ViewportY,
	VeFloat32 ViewportWidth, VeFloat32 ViewportHeight,
	VeFloat32 ViewportMinZ, VeFloat32 ViewportMaxZ, VE_FMATRIX Projection,
	VE_CMATRIX View, VE_CMATRIX World) noexcept
{
	static const VE_VECTORF32 D = { -1.0f, 1.0f, 0.0f, 0.0f };
	VE_VECTOR Scale = VeVectorSet(ViewportWidth * 0.5f, -ViewportHeight * 0.5f, ViewportMaxZ - ViewportMinZ, 1.0f);
	Scale = VeVectorReciprocal(Scale);
	VE_VECTOR Offset = VeVectorSet(-ViewportX, -ViewportY, -ViewportMinZ, 0.0f);
	Offset = VeVectorMultiplyAdd(Scale, Offset, D.v);
	VE_MATRIX Transform = VeMatrixMultiply(World, View);
	Transform = VeMatrixMultiply(Transform, Projection);
	Transform = VeMatrixInverse(nullptr, Transform);
	VE_VECTOR Result = VeVectorMultiplyAdd(V, Scale, Offset);
	return VeVector3TransformCoord(Result, Transform);
}
//--------------------------------------------------------------------------
inline VE_FLOAT3* VE_MATH_CALLCONV VeVector3UnprojectStream(
	VE_FLOAT3* pOutputStream, VeSizeT OutputStride,
	const VE_FLOAT3* pInputStream, VeSizeT InputStride,
	VeSizeT VectorCount, VeFloat32 ViewportX, VeFloat32 ViewportY,
	VeFloat32 ViewportWidth, VeFloat32 ViewportHeight,
	VeFloat32 ViewportMinZ, VeFloat32 ViewportMaxZ, VE_FMATRIX Projection,
	VE_CMATRIX View, VE_CMATRIX World) noexcept
{
	VE_ASSERT(pOutputStream != nullptr);
	VE_ASSERT(pInputStream != nullptr);
	VE_ASSERT(InputStride >= sizeof(VE_FLOAT3));
	VE_ASSERT(OutputStride >= sizeof(VE_FLOAT3));
#	if defined(VE_NO_INTRINSICS)
	static const VE_VECTORF32 D = { -1.0f, 1.0f, 0.0f, 0.0f };
	VE_VECTOR Scale = VeVectorSet(ViewportWidth * 0.5f, -ViewportHeight * 0.5f, ViewportMaxZ - ViewportMinZ, 1.0f);
	Scale = VeVectorReciprocal(Scale);
	VE_VECTOR Offset = VeVectorSet(-ViewportX, -ViewportY, -ViewportMinZ, 0.0f);
	Offset = VeVectorMultiplyAdd(Scale, Offset, D.v);
	VE_MATRIX Transform = VeMatrixMultiply(World, View);
	Transform = VeMatrixMultiply(Transform, Projection);
	Transform = VeMatrixInverse(nullptr, Transform);
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	for (VeSizeT i = 0; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat3((const VE_FLOAT3*)pInputVector);
		VE_VECTOR Result = VeVectorMultiplyAdd(V, Scale, Offset);
		Result = VeVector3TransformCoord(Result, Transform);
		VeStoreFloat3((VE_FLOAT3*)pOutputVector, Result);
		pInputVector += InputStride;
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 D = { -1.0f, 1.0f, 0.0f, 0.0f };
	VE_VECTOR Scale = VeVectorSet(ViewportWidth * 0.5f, -ViewportHeight * 0.5f, ViewportMaxZ - ViewportMinZ, 1.0f);
	Scale = VeVectorReciprocal(Scale);
	VE_VECTOR Offset = VeVectorSet(-ViewportX, -ViewportY, -ViewportMinZ, 0.0f);
	Offset = _mm_mul_ps(Scale, Offset);
	Offset = _mm_add_ps(Offset, D);
	VE_MATRIX Transform = VeMatrixMultiply(World, View);
	Transform = VeMatrixMultiply(Transform, Projection);
	Transform = VeMatrixInverse(nullptr, Transform);
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	VeSizeT i = 0;
	VeSizeT four = VectorCount >> 2;
	if (four > 0)
	{
		if (InputStride == sizeof(VE_FLOAT3))
		{
			if (OutputStride == sizeof(VE_FLOAT3))
			{
				if (!((VeSizeT)pOutputStream & 0xF))
				{
					for (VeSizeT j = 0; j < four; ++j)
					{
						__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
						__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
						pInputVector += sizeof(VE_FLOAT3) * 4;
						VE3UNPACK3INTO4(V1, L2, L3);
						V1 = _mm_mul_ps(V1, Scale);
						V1 = _mm_add_ps(V1, Offset);
						VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
						VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Z, Transform.r[2]);
						VE_VECTOR vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						VE_VECTOR vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V1 = _mm_div_ps(vTemp, W);
						V2 = _mm_mul_ps(V2, Scale);
						V2 = _mm_add_ps(V2, Offset);
						Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V2 = _mm_div_ps(vTemp, W);
						V3 = _mm_mul_ps(V3, Scale);
						V3 = _mm_add_ps(V3, Offset);
						Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V3 = _mm_div_ps(vTemp, W);
						V4 = _mm_mul_ps(V4, Scale);
						V4 = _mm_add_ps(V4, Offset);
						Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V4 = _mm_div_ps(vTemp, W);
						VE3PACK4INTO3(vTemp);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), V1);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector + 16), vTemp);
						VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector + 32), V3);
						pOutputVector += sizeof(VE_FLOAT3) * 4;
						i += 4;
					}
				}
				else
				{
					for (VeSizeT j = 0; j < four; ++j)
					{
						__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
						__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
						__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
						pInputVector += sizeof(VE_FLOAT3) * 4;
						VE3UNPACK3INTO4(V1, L2, L3);
						V1 = _mm_mul_ps(V1, Scale);
						V1 = _mm_add_ps(V1, Offset);
						VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
						VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
						VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
						VE_VECTOR vTemp = _mm_mul_ps(Z, Transform.r[2]);
						VE_VECTOR vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						VE_VECTOR vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V1 = _mm_div_ps(vTemp, W);
						V2 = _mm_mul_ps(V2, Scale);
						V2 = _mm_add_ps(V2, Offset);
						Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V2 = _mm_div_ps(vTemp, W);
						V3 = _mm_mul_ps(V3, Scale);
						V3 = _mm_add_ps(V3, Offset);
						Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V3 = _mm_div_ps(vTemp, W);
						V4 = _mm_mul_ps(V4, Scale);
						V4 = _mm_add_ps(V4, Offset);
						Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
						Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
						X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
						vTemp = _mm_mul_ps(Z, Transform.r[2]);
						vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
						vTemp3 = _mm_mul_ps(X, Transform.r[0]);
						vTemp = _mm_add_ps(vTemp, Transform.r[3]);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						vTemp = _mm_add_ps(vTemp, vTemp3);
						W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
						V4 = _mm_div_ps(vTemp, W);
						VE3PACK4INTO3(vTemp);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), V1);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector + 16), vTemp);
						_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector + 32), V3);
						pOutputVector += sizeof(VE_FLOAT3) * 4;
						i += 4;
					}
				}
			}
			else
			{
				for (VeSizeT j = 0; j < four; ++j)
				{
					__m128 V1 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
					__m128 L2 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 16));
					__m128 L3 = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector + 32));
					pInputVector += sizeof(VE_FLOAT3) * 4;
					VE3UNPACK3INTO4(V1, L2, L3);
					V1 = _mm_mul_ps(V1, Scale);
					V1 = _mm_add_ps(V1, Offset);
					VE_VECTOR Z = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
					VE_VECTOR Y = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
					VE_VECTOR X = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));
					VE_VECTOR vTemp = _mm_mul_ps(Z, Transform.r[2]);
					VE_VECTOR vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
					VE_VECTOR vTemp3 = _mm_mul_ps(X, Transform.r[0]);
					vTemp = _mm_add_ps(vTemp, Transform.r[3]);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					V2 = _mm_mul_ps(V2, Scale);
					V2 = _mm_add_ps(V2, Offset);
					Z = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, Transform.r[2]);
					vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
					vTemp3 = _mm_mul_ps(X, Transform.r[0]);
					vTemp = _mm_add_ps(vTemp, Transform.r[3]);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					V3 = _mm_mul_ps(V3, Scale);
					V3 = _mm_add_ps(V3, Offset);
					Z = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, Transform.r[2]);
					vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
					vTemp3 = _mm_mul_ps(X, Transform.r[0]);
					vTemp = _mm_add_ps(vTemp, Transform.r[3]);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					V4 = _mm_mul_ps(V4, Scale);
					V4 = _mm_add_ps(V4, Offset);
					Z = VE_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
					Y = VE_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
					X = VE_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));
					vTemp = _mm_mul_ps(Z, Transform.r[2]);
					vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
					vTemp3 = _mm_mul_ps(X, Transform.r[0]);
					vTemp = _mm_add_ps(vTemp, Transform.r[3]);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
					vTemp = _mm_div_ps(vTemp, W);
					VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
					i += 4;
				}
			}
		}
	}
	for (; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat3(reinterpret_cast<const VE_FLOAT3*>(pInputVector));
		pInputVector += InputStride;
		V = _mm_mul_ps(V, Scale);
		V = _mm_add_ps(V, Offset);
		VE_VECTOR Z = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
		VE_VECTOR Y = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
		VE_VECTOR X = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
		VE_VECTOR vTemp = _mm_mul_ps(Z, Transform.r[2]);
		VE_VECTOR vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
		VE_VECTOR vTemp3 = _mm_mul_ps(X, Transform.r[0]);
		vTemp = _mm_add_ps(vTemp, Transform.r[3]);
		vTemp = _mm_add_ps(vTemp, vTemp2);
		vTemp = _mm_add_ps(vTemp, vTemp3);
		VE_VECTOR W = VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
		vTemp = _mm_div_ps(vTemp, W);
		VeStoreFloat3(reinterpret_cast<VE_FLOAT3*>(pOutputVector), vTemp);
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4Equal(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] == V2.vector4_f32[0]) && (V1.vector4_f32[1] == V2.vector4_f32[1]) && (V1.vector4_f32[2] == V2.vector4_f32[2]) && (V1.vector4_f32[3] == V2.vector4_f32[3])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpeq_ps(V1, V2);
	return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#	else
	return VeComparisonAllTrue(VeVector4EqualR(V1, V2));
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector4EqualR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if ((V1.vector4_f32[0] == V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] == V2.vector4_f32[1]) &&
		(V1.vector4_f32[2] == V2.vector4_f32[2]) &&
		(V1.vector4_f32[3] == V2.vector4_f32[3]))
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if ((V1.vector4_f32[0] != V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] != V2.vector4_f32[1]) &&
		(V1.vector4_f32[2] != V2.vector4_f32[2]) &&
		(V1.vector4_f32[3] != V2.vector4_f32[3]))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpeq_ps(V1, V2);
	VeInt32 iTest = _mm_movemask_ps(vTemp);
	VeUInt32 CR = 0;
	if (iTest == 0xf)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (iTest == 0)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4EqualInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_u32[0] == V2.vector4_u32[0]) && (V1.vector4_u32[1] == V2.vector4_u32[1]) && (V1.vector4_u32[2] == V2.vector4_u32[2]) && (V1.vector4_u32[3] == V2.vector4_u32[3])) != 0);
#	elif defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) == 0xf) != 0);
#	else
	return VeComparisonAllTrue(VeVector4EqualIntR(V1, V2));
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector4EqualIntR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if (V1.vector4_u32[0] == V2.vector4_u32[0] &&
		V1.vector4_u32[1] == V2.vector4_u32[1] &&
		V1.vector4_u32[2] == V2.vector4_u32[2] &&
		V1.vector4_u32[3] == V2.vector4_u32[3])
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (V1.vector4_u32[0] != V2.vector4_u32[0] &&
		V1.vector4_u32[1] != V2.vector4_u32[1] &&
		V1.vector4_u32[2] != V2.vector4_u32[2] &&
		V1.vector4_u32[3] != V2.vector4_u32[3])
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	VeInt32 iTest = _mm_movemask_ps(_mm_castsi128_ps(vTemp));
	VeUInt32 CR = 0;
	if (iTest == 0xf)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (iTest == 0)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4NearEqual(
	VE_FVECTOR V1, VE_FVECTOR V2, VE_FVECTOR Epsilon) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 dx, dy, dz, dw;
	dx = VeFabsf(V1.vector4_f32[0] - V2.vector4_f32[0]);
	dy = VeFabsf(V1.vector4_f32[1] - V2.vector4_f32[1]);
	dz = VeFabsf(V1.vector4_f32[2] - V2.vector4_f32[2]);
	dw = VeFabsf(V1.vector4_f32[3] - V2.vector4_f32[3]);
	return (((dx <= Epsilon.vector4_f32[0]) &&
		(dy <= Epsilon.vector4_f32[1]) &&
		(dz <= Epsilon.vector4_f32[2]) &&
		(dw <= Epsilon.vector4_f32[3])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vDelta = _mm_sub_ps(V1, V2);
	VE_VECTOR vTemp = _mm_setzero_ps();
	vTemp = _mm_sub_ps(vTemp, vDelta);
	vTemp = _mm_max_ps(vTemp, vDelta);
	vTemp = _mm_cmple_ps(vTemp, Epsilon);
	return ((_mm_movemask_ps(vTemp) == 0xf) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4NotEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] != V2.vector4_f32[0]) || (V1.vector4_f32[1] != V2.vector4_f32[1]) || (V1.vector4_f32[2] != V2.vector4_f32[2]) || (V1.vector4_f32[3] != V2.vector4_f32[3])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpneq_ps(V1, V2);
	return ((_mm_movemask_ps(vTemp)) != 0);
#	else
	return VeComparisonAnyFalse(VeVector4EqualR(V1, V2));
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4NotEqualInt(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_u32[0] != V2.vector4_u32[0]) || (V1.vector4_u32[1] != V2.vector4_u32[1]) || (V1.vector4_u32[2] != V2.vector4_u32[2]) || (V1.vector4_u32[3] != V2.vector4_u32[3])) != 0);
#	elif defined(VE_ENABLE_SSE)
	__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
	return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) != 0xF) != 0);
#	else
	return VeComparisonAnyFalse(VeVector4EqualIntR(V1, V2));
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4Greater(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] > V2.vector4_f32[0]) && (V1.vector4_f32[1] > V2.vector4_f32[1]) && (V1.vector4_f32[2] > V2.vector4_f32[2]) && (V1.vector4_f32[3] > V2.vector4_f32[3])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpgt_ps(V1, V2);
	return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#	else
	return VeComparisonAllTrue(VeVector4GreaterR(V1, V2));
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector4GreaterR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if (V1.vector4_f32[0] > V2.vector4_f32[0] &&
		V1.vector4_f32[1] > V2.vector4_f32[1] &&
		V1.vector4_f32[2] > V2.vector4_f32[2] &&
		V1.vector4_f32[3] > V2.vector4_f32[3])
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (V1.vector4_f32[0] <= V2.vector4_f32[0] &&
		V1.vector4_f32[1] <= V2.vector4_f32[1] &&
		V1.vector4_f32[2] <= V2.vector4_f32[2] &&
		V1.vector4_f32[3] <= V2.vector4_f32[3])
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	VeUInt32 CR = 0;
	VE_VECTOR vTemp = _mm_cmpgt_ps(V1, V2);
	VeInt32 iTest = _mm_movemask_ps(vTemp);
	if (iTest == 0xf) {
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4GreaterOrEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] >= V2.vector4_f32[0]) && (V1.vector4_f32[1] >= V2.vector4_f32[1]) && (V1.vector4_f32[2] >= V2.vector4_f32[2]) && (V1.vector4_f32[3] >= V2.vector4_f32[3])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmpge_ps(V1, V2);
	return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#	else
	return VeComparisonAllTrue(VeVector4GreaterOrEqualR(V1, V2));
#	endif
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_MATH_CALLCONV VeVector4GreaterOrEqualR(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeUInt32 CR = 0;
	if ((V1.vector4_f32[0] >= V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] >= V2.vector4_f32[1]) &&
		(V1.vector4_f32[2] >= V2.vector4_f32[2]) &&
		(V1.vector4_f32[3] >= V2.vector4_f32[3]))
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if ((V1.vector4_f32[0] < V2.vector4_f32[0]) &&
		(V1.vector4_f32[1] < V2.vector4_f32[1]) &&
		(V1.vector4_f32[2] < V2.vector4_f32[2]) &&
		(V1.vector4_f32[3] < V2.vector4_f32[3]))
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	elif defined(VE_ENABLE_SSE)
	VeUInt32 CR = 0;
	VE_VECTOR vTemp = _mm_cmpge_ps(V1, V2);
	VeInt32 iTest = _mm_movemask_ps(vTemp);
	if (iTest == 0x0f)
	{
		CR = VE_CRMASK_CR6TRUE;
	}
	else if (!iTest)
	{
		CR = VE_CRMASK_CR6FALSE;
	}
	return CR;
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4Less(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] < V2.vector4_f32[0]) && (V1.vector4_f32[1] < V2.vector4_f32[1]) && (V1.vector4_f32[2] < V2.vector4_f32[2]) && (V1.vector4_f32[3] < V2.vector4_f32[3])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmplt_ps(V1, V2);
	return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#	else
	return VeComparisonAllTrue(VeVector4GreaterR(V2, V1));
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4LessOrEqual(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V1.vector4_f32[0] <= V2.vector4_f32[0]) && (V1.vector4_f32[1] <= V2.vector4_f32[1]) && (V1.vector4_f32[2] <= V2.vector4_f32[2]) && (V1.vector4_f32[3] <= V2.vector4_f32[3])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_cmple_ps(V1, V2);
	return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#	else
	return VeComparisonAllTrue(VeVector4GreaterOrEqualR(V2, V1));
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4InBounds(
	VE_FVECTOR V, VE_FVECTOR Bounds) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (((V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) &&
		(V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) &&
		(V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2]) &&
		(V.vector4_f32[3] <= Bounds.vector4_f32[3] && V.vector4_f32[3] >= -Bounds.vector4_f32[3])) != 0);
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp1 = _mm_cmple_ps(V, Bounds);
	VE_VECTOR vTemp2 = _mm_mul_ps(Bounds, g_MathNegativeOne);
	vTemp2 = _mm_cmple_ps(vTemp2, V);
	vTemp1 = _mm_and_ps(vTemp1, vTemp2);
	return ((_mm_movemask_ps(vTemp1) == 0x0f) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4IsNaN(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (VEISNAN(V.vector4_f32[0]) ||
		VEISNAN(V.vector4_f32[1]) ||
		VEISNAN(V.vector4_f32[2]) ||
		VEISNAN(V.vector4_f32[3]));
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTempNan = _mm_cmpneq_ps(V, V);
	return (_mm_movemask_ps(vTempNan) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline bool VE_MATH_CALLCONV VeVector4IsInfinite(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	return (VEISINF(V.vector4_f32[0]) ||
		VEISINF(V.vector4_f32[1]) ||
		VEISINF(V.vector4_f32[2]) ||
		VEISINF(V.vector4_f32[3]));
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp = _mm_and_ps(V, g_MathAbsMask);
	vTemp = _mm_cmpeq_ps(vTemp, g_MathInfinity);
	return (_mm_movemask_ps(vTemp) != 0);
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4Dot(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] =
		Result.vector4_f32[1] =
		Result.vector4_f32[2] =
		Result.vector4_f32[3] = V1.vector4_f32[0] * V2.vector4_f32[0] + V1.vector4_f32[1] * V2.vector4_f32[1] + V1.vector4_f32[2] * V2.vector4_f32[2] + V1.vector4_f32[3] * V2.vector4_f32[3];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTemp2 = V2;
	VE_VECTOR vTemp = _mm_mul_ps(V1, vTemp2);
	vTemp2 = _mm_shuffle_ps(vTemp2, vTemp, _MM_SHUFFLE(1, 0, 0, 0));
	vTemp2 = _mm_add_ps(vTemp2, vTemp);
	vTemp = _mm_shuffle_ps(vTemp, vTemp2, _MM_SHUFFLE(0, 3, 0, 0));
	vTemp = _mm_add_ps(vTemp, vTemp2);
	return VE_PERMUTE_PS(vTemp, _MM_SHUFFLE(2, 2, 2, 2));
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4Cross(
	VE_FVECTOR V1, VE_FVECTOR V2, VE_FVECTOR V3) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = (((V2.vector4_f32[2] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[2]))*V1.vector4_f32[1]) - (((V2.vector4_f32[1] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[1]))*V1.vector4_f32[2]) + (((V2.vector4_f32[1] * V3.vector4_f32[2]) - (V2.vector4_f32[2] * V3.vector4_f32[1]))*V1.vector4_f32[3]);
	Result.vector4_f32[1] = (((V2.vector4_f32[3] * V3.vector4_f32[2]) - (V2.vector4_f32[2] * V3.vector4_f32[3]))*V1.vector4_f32[0]) - (((V2.vector4_f32[3] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[3]))*V1.vector4_f32[2]) + (((V2.vector4_f32[2] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[2]))*V1.vector4_f32[3]);
	Result.vector4_f32[2] = (((V2.vector4_f32[1] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[1]))*V1.vector4_f32[0]) - (((V2.vector4_f32[0] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[0]))*V1.vector4_f32[1]) + (((V2.vector4_f32[0] * V3.vector4_f32[1]) - (V2.vector4_f32[1] * V3.vector4_f32[0]))*V1.vector4_f32[3]);
	Result.vector4_f32[3] = (((V2.vector4_f32[2] * V3.vector4_f32[1]) - (V2.vector4_f32[1] * V3.vector4_f32[2]))*V1.vector4_f32[0]) - (((V2.vector4_f32[2] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[2]))*V1.vector4_f32[1]) + (((V2.vector4_f32[1] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[1]))*V1.vector4_f32[2]);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vResult = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 1, 3, 2));
	VE_VECTOR vTemp3 = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 3, 2, 3));
	vResult = _mm_mul_ps(vResult, vTemp3);
	VE_VECTOR vTemp2 = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 3, 2, 3));
	vTemp3 = VE_PERMUTE_PS(vTemp3, _MM_SHUFFLE(1, 3, 0, 1));
	vTemp2 = _mm_mul_ps(vTemp2, vTemp3);
	vResult = _mm_sub_ps(vResult, vTemp2);
	VE_VECTOR vTemp1 = VE_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 1));
	vResult = _mm_mul_ps(vResult, vTemp1);
	vTemp2 = VE_PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 3, 1));
	vTemp3 = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 3, 0, 3));
	vTemp3 = _mm_mul_ps(vTemp3, vTemp2);
	vTemp2 = VE_PERMUTE_PS(vTemp2, _MM_SHUFFLE(2, 1, 2, 1));
	vTemp1 = VE_PERMUTE_PS(V3, _MM_SHUFFLE(2, 0, 3, 1));
	vTemp2 = _mm_mul_ps(vTemp2, vTemp1);
	vTemp3 = _mm_sub_ps(vTemp3, vTemp2);
	vTemp1 = VE_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 2, 2));
	vTemp1 = _mm_mul_ps(vTemp1, vTemp3);
	vResult = _mm_sub_ps(vResult, vTemp1);
	vTemp2 = VE_PERMUTE_PS(V2, _MM_SHUFFLE(1, 0, 2, 1));
	vTemp3 = VE_PERMUTE_PS(V3, _MM_SHUFFLE(0, 1, 0, 2));
	vTemp3 = _mm_mul_ps(vTemp3, vTemp2);
	vTemp2 = VE_PERMUTE_PS(vTemp2, _MM_SHUFFLE(2, 0, 2, 1));
	vTemp1 = VE_PERMUTE_PS(V3, _MM_SHUFFLE(1, 0, 2, 1));
	vTemp1 = _mm_mul_ps(vTemp1, vTemp2);
	vTemp3 = _mm_sub_ps(vTemp3, vTemp1);
	vTemp1 = VE_PERMUTE_PS(V1, _MM_SHUFFLE(2, 3, 3, 3));
	vTemp3 = _mm_mul_ps(vTemp3, vTemp1);
	vResult = _mm_add_ps(vResult, vTemp3);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4LengthSq(
	VE_FVECTOR V) noexcept
{
	return VeVector4Dot(V, V);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4ReciprocalLengthEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector4LengthSq(V);
	Result = VeVectorReciprocalSqrtEst(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
	vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
	vLengthSq = _mm_rsqrt_ps(vLengthSq);
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4ReciprocalLength(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector4LengthSq(V);
	Result = VeVectorReciprocalSqrt(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
	vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
	vLengthSq = _mm_sqrt_ps(vLengthSq);
	vLengthSq = _mm_div_ps(g_MathOne, vLengthSq);
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4LengthEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector4LengthSq(V);
	Result = VeVectorSqrtEst(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
	vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
	vLengthSq = _mm_sqrt_ps(vLengthSq);
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4Length(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS) 
	VE_VECTOR Result;
	Result = VeVector4LengthSq(V);
	Result = VeVectorSqrt(Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
	vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
	vLengthSq = _mm_sqrt_ps(vLengthSq);
	return vLengthSq;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4NormalizeEst(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result = VeVector4ReciprocalLength(V);
	Result = VeVectorMultiply(V, Result);
	return Result;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
	vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
	VE_VECTOR vResult = _mm_rsqrt_ps(vLengthSq);
	vResult = _mm_mul_ps(vResult, V);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4Normalize(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fLength;
	VE_VECTOR vResult;
	vResult = VeVector4Length(V);
	fLength = vResult.vector4_f32[0];
	if (fLength > 0)
	{
		fLength = 1.0f / fLength;
	}
	vResult.vector4_f32[0] = V.vector4_f32[0] * fLength;
	vResult.vector4_f32[1] = V.vector4_f32[1] * fLength;
	vResult.vector4_f32[2] = V.vector4_f32[2] * fLength;
	vResult.vector4_f32[3] = V.vector4_f32[3] * fLength;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vLengthSq = _mm_mul_ps(V, V);
	VE_VECTOR vTemp = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
	vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
	vLengthSq = _mm_add_ps(vLengthSq, vTemp);
	vLengthSq = VE_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
	VE_VECTOR vResult = _mm_sqrt_ps(vLengthSq);
	VE_VECTOR vZeroMask = _mm_setzero_ps();
	vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
	vLengthSq = _mm_cmpneq_ps(vLengthSq, g_MathInfinity);
	vResult = _mm_div_ps(V, vResult);
	vResult = _mm_and_ps(vResult, vZeroMask);
	VE_VECTOR vTemp1 = _mm_andnot_ps(vLengthSq, g_MathQNaN);
	VE_VECTOR vTemp2 = _mm_and_ps(vResult, vLengthSq);
	vResult = _mm_or_ps(vTemp1, vTemp2);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4ClampLengthV(
	VE_FVECTOR V, VE_FVECTOR LengthMin, VE_FVECTOR LengthMax) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4ClampLength(
	VE_FVECTOR V, VeFloat32 LengthMin, VeFloat32 LengthMax) noexcept
{
	VE_VECTOR ClampMax = VeVectorReplicate(LengthMax);
	VE_VECTOR ClampMin = VeVectorReplicate(LengthMin);
	return VeVector4ClampLengthV(V, ClampMin, ClampMax);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4ClampLengthV(
	VE_FVECTOR V, VE_FVECTOR LengthMin, VE_FVECTOR LengthMax) noexcept
{
	VE_ASSERT((VeVectorGetY(LengthMin) == VeVectorGetX(LengthMin)) && (VeVectorGetZ(LengthMin) == VeVectorGetX(LengthMin)) && (VeVectorGetW(LengthMin) == VeVectorGetX(LengthMin)));
	VE_ASSERT((VeVectorGetY(LengthMax) == VeVectorGetX(LengthMax)) && (VeVectorGetZ(LengthMax) == VeVectorGetX(LengthMax)) && (VeVectorGetW(LengthMax) == VeVectorGetX(LengthMax)));
	VE_ASSERT(VeVector4GreaterOrEqual(LengthMin, VeVectorZero()));
	VE_ASSERT(VeVector4GreaterOrEqual(LengthMax, VeVectorZero()));
	VE_ASSERT(VeVector4GreaterOrEqual(LengthMax, LengthMin));
	VE_VECTOR LengthSq = VeVector4LengthSq(V);
	const VE_VECTOR Zero = VeVectorZero();
	VE_VECTOR RcpLength = VeVectorReciprocalSqrt(LengthSq);
	VE_VECTOR InfiniteLength = VeVectorEqualInt(LengthSq, g_MathInfinity.v);
	VE_VECTOR ZeroLength = VeVectorEqual(LengthSq, Zero);
	VE_VECTOR Normal = VeVectorMultiply(V, RcpLength);
	VE_VECTOR Length = VeVectorMultiply(LengthSq, RcpLength);
	VE_VECTOR Select = VeVectorEqualInt(InfiniteLength, ZeroLength);
	Length = VeVectorSelect(LengthSq, Length, Select);
	Normal = VeVectorSelect(LengthSq, Normal, Select);
	VE_VECTOR ControlMax = VeVectorGreater(Length, LengthMax);
	VE_VECTOR ControlMin = VeVectorLess(Length, LengthMin);
	VE_VECTOR ClampLength = VeVectorSelect(Length, LengthMax, ControlMax);
	ClampLength = VeVectorSelect(ClampLength, LengthMin, ControlMin);
	VE_VECTOR Result = VeVectorMultiply(Normal, ClampLength);
	VE_VECTOR Control = VeVectorEqualInt(ControlMax, ControlMin);
	Result = VeVectorSelect(Result, V, Control);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4Reflect(
	VE_FVECTOR Incident, VE_FVECTOR Normal) noexcept
{
	VE_VECTOR Result = VeVector4Dot(Incident, Normal);
	Result = VeVectorAdd(Result, Result);
	Result = VeVectorNegativeMultiplySubtract(Result, Normal, Incident);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4RefractV(VE_FVECTOR Incident,
	VE_FVECTOR Normal, VE_FVECTOR RefractionIndex) noexcept;
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4Refract(VE_FVECTOR Incident,
	VE_FVECTOR Normal, VeFloat32 RefractionIndex) noexcept
{
	VE_VECTOR Index = VeVectorReplicate(RefractionIndex);
	return VeVector4RefractV(Incident, Normal, Index);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4RefractV(VE_FVECTOR Incident,
	VE_FVECTOR Normal, VE_FVECTOR RefractionIndex) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR        IDotN;
	VE_VECTOR        R;
	const VE_VECTOR  Zero = VeVectorZero();
	IDotN = VeVector4Dot(Incident, Normal);
	R = VeVectorNegativeMultiplySubtract(IDotN, IDotN, g_MathOne.v);
	R = VeVectorMultiply(R, RefractionIndex);
	R = VeVectorNegativeMultiplySubtract(R, RefractionIndex, g_MathOne.v);
	if (VeVector4LessOrEqual(R, Zero))
	{
		return Zero;
	}
	else
	{
		VE_VECTOR Result;
		R = VeVectorSqrt(R);
		R = VeVectorMultiplyAdd(RefractionIndex, IDotN, R);
		Result = VeVectorMultiply(RefractionIndex, Incident);
		Result = VeVectorNegativeMultiplySubtract(Normal, R, Result);
		return Result;
	}
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR IDotN = VeVector4Dot(Incident, Normal);
	VE_VECTOR R = _mm_mul_ps(IDotN, IDotN);
	R = _mm_sub_ps(g_MathOne, R);
	R = _mm_mul_ps(R, RefractionIndex);
	R = _mm_mul_ps(R, RefractionIndex);
	R = _mm_sub_ps(g_MathOne, R);
	VE_VECTOR vResult = _mm_cmple_ps(R, g_MathZero);
	if (_mm_movemask_ps(vResult) == 0x0f)
	{
		vResult = g_MathZero;
	}
	else
	{
		R = _mm_sqrt_ps(R);
		vResult = _mm_mul_ps(RefractionIndex, IDotN);
		R = _mm_add_ps(R, vResult);
		vResult = _mm_mul_ps(RefractionIndex, Incident);
		R = _mm_mul_ps(R, Normal);
		vResult = _mm_sub_ps(vResult, R);
	}
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4Orthogonal(
	VE_FVECTOR V) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Result;
	Result.vector4_f32[0] = V.vector4_f32[2];
	Result.vector4_f32[1] = V.vector4_f32[3];
	Result.vector4_f32[2] = -V.vector4_f32[0];
	Result.vector4_f32[3] = -V.vector4_f32[1];
	return Result;
#	elif defined(VE_ENABLE_SSE)
	static const VE_VECTORF32 FlipZW = { 1.0f,1.0f,-1.0f,-1.0f };
	VE_VECTOR vResult = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 0, 3, 2));
	vResult = _mm_mul_ps(vResult, FlipZW);
	return vResult;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4AngleBetweenNormalsEst(
	VE_FVECTOR N1, VE_FVECTOR N2) noexcept
{
	VE_VECTOR Result = VeVector4Dot(N1, N2);
	Result = VeVectorClamp(Result, g_MathNegativeOne.v, g_MathOne.v);
	Result = VeVectorACosEst(Result);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4AngleBetweenNormals(
	VE_FVECTOR N1, VE_FVECTOR N2) noexcept
{
	VE_VECTOR Result = VeVector4Dot(N1, N2);
	Result = VeVectorClamp(Result, g_MathNegativeOne.v, g_MathOne.v);
	Result = VeVectorACos(Result);
	return Result;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4AngleBetweenVectors(
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	VE_VECTOR L1 = VeVector4ReciprocalLength(V1);
	VE_VECTOR L2 = VeVector4ReciprocalLength(V2);
	VE_VECTOR Dot = VeVector4Dot(V1, V2);
	L1 = VeVectorMultiply(L1, L2);
	VE_VECTOR CosAngle = VeVectorMultiply(Dot, L1);
	CosAngle = VeVectorClamp(CosAngle, g_MathNegativeOne.v, g_MathOne.v);
	return VeVectorACos(CosAngle);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV VeVector4Transform(
	VE_FVECTOR V, VE_FMATRIX M) noexcept
{
#	if defined(VE_NO_INTRINSICS)
	VeFloat32 fX = (M.m[0][0] * V.vector4_f32[0]) + (M.m[1][0] * V.vector4_f32[1]) + (M.m[2][0] * V.vector4_f32[2]) + (M.m[3][0] * V.vector4_f32[3]);
	VeFloat32 fY = (M.m[0][1] * V.vector4_f32[0]) + (M.m[1][1] * V.vector4_f32[1]) + (M.m[2][1] * V.vector4_f32[2]) + (M.m[3][1] * V.vector4_f32[3]);
	VeFloat32 fZ = (M.m[0][2] * V.vector4_f32[0]) + (M.m[1][2] * V.vector4_f32[1]) + (M.m[2][2] * V.vector4_f32[2]) + (M.m[3][2] * V.vector4_f32[3]);
	VeFloat32 fW = (M.m[0][3] * V.vector4_f32[0]) + (M.m[1][3] * V.vector4_f32[1]) + (M.m[2][3] * V.vector4_f32[2]) + (M.m[3][3] * V.vector4_f32[3]);
	VE_VECTOR vResult;
	vResult.vector4_f32[0] = fX;
	vResult.vector4_f32[1] = fY;
	vResult.vector4_f32[2] = fZ;
	vResult.vector4_f32[3] = fW;
	return vResult;
#	elif defined(VE_ENABLE_SSE)
	VE_VECTOR vTempX = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
	VE_VECTOR vTempY = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	VE_VECTOR vTempZ = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	VE_VECTOR vTempW = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
	vTempX = _mm_mul_ps(vTempX, M.r[0]);
	vTempY = _mm_mul_ps(vTempY, M.r[1]);
	vTempZ = _mm_mul_ps(vTempZ, M.r[2]);
	vTempW = _mm_mul_ps(vTempW, M.r[3]);
	vTempX = _mm_add_ps(vTempX, vTempY);
	vTempZ = _mm_add_ps(vTempZ, vTempW);
	vTempX = _mm_add_ps(vTempX, vTempZ);
	return vTempX;
#	endif
}
//--------------------------------------------------------------------------
inline VE_FLOAT4* VE_MATH_CALLCONV VeVector4TransformStream(
	VE_FLOAT4* pOutputStream, VeSizeT OutputStride,
	const VE_FLOAT4* pInputStream, VeSizeT InputStride,
	VeSizeT VectorCount, VE_FMATRIX M) noexcept
{
	VE_ASSERT(pOutputStream != nullptr);
	VE_ASSERT(pInputStream != nullptr);
	VE_ASSERT(InputStride >= sizeof(VE_FLOAT4));
	VE_ASSERT(OutputStride >= sizeof(VE_FLOAT4));
#	if defined(VE_NO_INTRINSICS)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row2 = M.r[2];
	const VE_VECTOR row3 = M.r[3];
	for (VeSizeT i = 0; i < VectorCount; i++)
	{
		VE_VECTOR V = VeLoadFloat4((const VE_FLOAT4*)pInputVector);
		VE_VECTOR W = VeVectorSplatW(V);
		VE_VECTOR Z = VeVectorSplatZ(V);
		VE_VECTOR Y = VeVectorSplatY(V);
		VE_VECTOR X = VeVectorSplatX(V);
		VE_VECTOR Result = VeVectorMultiply(W, row3);
		Result = VeVectorMultiplyAdd(Z, row2, Result);
		Result = VeVectorMultiplyAdd(Y, row1, Result);
		Result = VeVectorMultiplyAdd(X, row0, Result);
		VeStoreFloat4((VE_FLOAT4*)pOutputVector, Result);
		pInputVector += InputStride;
		pOutputVector += OutputStride;
	}
	return pOutputStream;
#	elif defined(VE_ENABLE_SSE)
	const VeUInt8* pInputVector = (const VeUInt8*)pInputStream;
	VeUInt8* pOutputVector = (VeUInt8*)pOutputStream;
	const VE_VECTOR row0 = M.r[0];
	const VE_VECTOR row1 = M.r[1];
	const VE_VECTOR row2 = M.r[2];
	const VE_VECTOR row3 = M.r[3];
	if (!((VeSizeT)pOutputStream & 0xF) && !(OutputStride & 0xF))
	{
		if (!((VeSizeT)pInputStream & 0xF) && !(InputStride & 0xF))
		{
			for (VeSizeT i = 0; i < VectorCount; i++)
			{
				__m128 V = _mm_load_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
				pInputVector += InputStride;
				VE_VECTOR vTempX = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
				VE_VECTOR vTempY = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				VE_VECTOR vTempZ = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				VE_VECTOR vTempW = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
				vTempX = _mm_mul_ps(vTempX, row0);
				vTempY = _mm_mul_ps(vTempY, row1);
				vTempZ = _mm_mul_ps(vTempZ, row2);
				vTempW = _mm_mul_ps(vTempW, row3);
				vTempX = _mm_add_ps(vTempX, vTempY);
				vTempZ = _mm_add_ps(vTempZ, vTempW);
				vTempX = _mm_add_ps(vTempX, vTempZ);
				VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), vTempX);
				pOutputVector += OutputStride;
			}
		}
		else
		{
			for (VeSizeT i = 0; i < VectorCount; i++)
			{
				__m128 V = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
				pInputVector += InputStride;
				VE_VECTOR vTempX = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
				VE_VECTOR vTempY = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				VE_VECTOR vTempZ = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				VE_VECTOR vTempW = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
				vTempX = _mm_mul_ps(vTempX, row0);
				vTempY = _mm_mul_ps(vTempY, row1);
				vTempZ = _mm_mul_ps(vTempZ, row2);
				vTempW = _mm_mul_ps(vTempW, row3);
				vTempX = _mm_add_ps(vTempX, vTempY);
				vTempZ = _mm_add_ps(vTempZ, vTempW);
				vTempX = _mm_add_ps(vTempX, vTempZ);
				VE_STREAM_PS(reinterpret_cast<VeFloat32*>(pOutputVector), vTempX);
				pOutputVector += OutputStride;
			}
		}
	}
	else
	{
		if (!((VeSizeT)pInputStream & 0xF) && !(InputStride & 0xF))
		{
			for (VeSizeT i = 0; i < VectorCount; i++)
			{
				__m128 V = _mm_load_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
				pInputVector += InputStride;
				VE_VECTOR vTempX = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
				VE_VECTOR vTempY = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				VE_VECTOR vTempZ = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				VE_VECTOR vTempW = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
				vTempX = _mm_mul_ps(vTempX, row0);
				vTempY = _mm_mul_ps(vTempY, row1);
				vTempZ = _mm_mul_ps(vTempZ, row2);
				vTempW = _mm_mul_ps(vTempW, row3);
				vTempX = _mm_add_ps(vTempX, vTempY);
				vTempZ = _mm_add_ps(vTempZ, vTempW);
				vTempX = _mm_add_ps(vTempX, vTempZ);
				_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTempX);
				pOutputVector += OutputStride;
			}
		}
		else
		{
			for (VeSizeT i = 0; i < VectorCount; i++)
			{
				__m128 V = _mm_loadu_ps(reinterpret_cast<const VeFloat32*>(pInputVector));
				pInputVector += InputStride;
				VE_VECTOR vTempX = VE_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
				VE_VECTOR vTempY = VE_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				VE_VECTOR vTempZ = VE_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				VE_VECTOR vTempW = VE_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
				vTempX = _mm_mul_ps(vTempX, row0);
				vTempY = _mm_mul_ps(vTempY, row1);
				vTempZ = _mm_mul_ps(vTempZ, row2);
				vTempW = _mm_mul_ps(vTempW, row3);
				vTempX = _mm_add_ps(vTempX, vTempY);
				vTempZ = _mm_add_ps(vTempZ, vTempW);
				vTempX = _mm_add_ps(vTempX, vTempZ);
				_mm_storeu_ps(reinterpret_cast<VeFloat32*>(pOutputVector), vTempX);
				pOutputVector += OutputStride;
			}
		}
	}
	return pOutputStream;
#	endif
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV operator+ (VE_FVECTOR V) noexcept
{
	return V;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV operator- (VE_FVECTOR V) noexcept
{
	return VeVectorNegate(V);
}
//--------------------------------------------------------------------------
inline VE_VECTOR& VE_MATH_CALLCONV operator+= (
	VE_VECTOR& V1, VE_FVECTOR V2) noexcept
{
	V1 = VeVectorAdd(V1, V2);
	return V1;
}
//--------------------------------------------------------------------------
inline VE_VECTOR& VE_MATH_CALLCONV operator-= (
	VE_VECTOR& V1, VE_FVECTOR V2) noexcept
{
	V1 = VeVectorSubtract(V1, V2);
	return V1;
}
//--------------------------------------------------------------------------
inline VE_VECTOR& VE_MATH_CALLCONV operator*= (
	VE_VECTOR& V1, VE_FVECTOR V2) noexcept
{
	V1 = VeVectorMultiply(V1, V2);
	return V1;
}
//--------------------------------------------------------------------------
inline VE_VECTOR& VE_MATH_CALLCONV operator/= (
	VE_VECTOR& V1, VE_FVECTOR V2) noexcept
{
	V1 = VeVectorDivide(V1, V2);
	return V1;
}

//--------------------------------------------------------------------------
inline VE_VECTOR& operator*= (VE_VECTOR&   V, const VeFloat32 S) noexcept
{
	V = VeVectorScale(V, S);
	return V;
}
//--------------------------------------------------------------------------
inline VE_VECTOR& operator/= (VE_VECTOR& V, const VeFloat32 S) noexcept
{
	VE_VECTOR vS = VeVectorReplicate(S);
	V = VeVectorDivide(V, vS);
	return V;
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV operator+ (
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	return VeVectorAdd(V1, V2);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV operator- (
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	return VeVectorSubtract(V1, V2);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV operator* (
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	return VeVectorMultiply(V1, V2);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV operator/ (
	VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	return VeVectorDivide(V1, V2);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV operator* (
	VE_FVECTOR V, const VeFloat32 S) noexcept
{
	return VeVectorScale(V, S);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV operator/ (
	VE_FVECTOR V, const VeFloat32 S) noexcept
{
	VE_VECTOR vS = VeVectorReplicate(S);
	return VeVectorDivide(V, vS);
}
//--------------------------------------------------------------------------
inline VE_VECTOR VE_MATH_CALLCONV operator* (
	VeFloat32 S, VE_FVECTOR V) noexcept
{
	return VeVectorScale(V, S);
}
//--------------------------------------------------------------------------
#if defined(VE_NO_INTRINSICS)
#	undef VEISNAN
#	undef VEISINF
#endif
//--------------------------------------------------------------------------
#if defined(VE_ENABLE_SSE)
#	undef VE3UNPACK3INTO4
#	undef VE3PACK4INTO3
#endif
//--------------------------------------------------------------------------
