////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMathVector.inl
//  Created:     2015/08/19 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#if defined(VE_NO_INTRINSICS)
#define VEISNAN(x)  ((*(VeUInt32*)&(x) & 0x7F800000) == 0x7F800000 && (*(VeUInt32*)&(x) & 0x7FFFFF) != 0)
#define VEISINF(x)  ((*(VeUInt32*)&(x) & 0x7FFFFFFF) == 0x7F800000)
#endif

#if defined(VE_ENABLE_SSE)

#define VE3UNPACK3INTO4(l1,l2,l3) \
    VE_VECTOR V3 = _mm_shuffle_ps(l2,l3,_MM_SHUFFLE(0,0,3,2));\
    VE_VECTOR V2 = _mm_shuffle_ps(l2,l1,_MM_SHUFFLE(3,3,1,0));\
    V2 = XM_PERMUTE_PS(V2,_MM_SHUFFLE(1,1,0,2));\
    VE_VECTOR V4 = _mm_castsi128_ps( _mm_srli_si128(_mm_castps_si128(L3),32/8) );

#define VE3PACK4INTO3(v2x) \
    v2x = _mm_shuffle_ps(V2,V3,_MM_SHUFFLE(1,0,2,1));\
    V2 = _mm_shuffle_ps(V2,V1,_MM_SHUFFLE(2,2,0,0));\
    V1 = _mm_shuffle_ps(V1,V2,_MM_SHUFFLE(0,2,1,0));\
    V3 = _mm_shuffle_ps(V3,V4,_MM_SHUFFLE(0,0,2,2));\
    V3 = _mm_shuffle_ps(V3,V4,_MM_SHUFFLE(2,1,2,0));\

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
		XM_SELECT_0,
		XM_SELECT_1
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
inline VE_VECTOR VE_MATH_CALLCONV VeVectorEqualIntR(
	VeUInt32* pCR, VE_FVECTOR V1, VE_FVECTOR V2) noexcept
{
	VE_ASSERT(pCR != nullptr);
#	if defined(VE_NO_INTRINSICS)
	VE_VECTOR Control = VeVectorEqualInt(V1, V2);
	*pCR = 0;
	if (XMVector4EqualInt(Control, VeVectorTrueInt()))
	{
		*pCR |= VE_CRMASK_CR6TRUE;
	}
	else if (XMVector4EqualInt(Control, VeVectorFalseInt()))
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
