////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Venus3D
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   VeSIMDMath.h
//  Created:     2016/07/02 by Albert
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

#pragma once

constexpr float VE_PI = 3.141592654f;
constexpr float VE_2PI = 6.283185307f;
constexpr float VE_1DIVPI = 0.318309886f;
constexpr float VE_1DIV2PI = 0.159154943f;
constexpr float VE_PIDIV2 = 1.570796327f;
constexpr float VE_PIDIV4 = 0.785398163f;

constexpr uint32_t VE_SELECT_0 = 0x00000000;
constexpr uint32_t VE_SELECT_1 = 0xFFFFFFFF;

constexpr uint32_t VE_PERMUTE_0X = 0;
constexpr uint32_t VE_PERMUTE_0Y = 1;
constexpr uint32_t VE_PERMUTE_0Z = 2;
constexpr uint32_t VE_PERMUTE_0W = 3;
constexpr uint32_t VE_PERMUTE_1X = 4;
constexpr uint32_t VE_PERMUTE_1Y = 5;
constexpr uint32_t VE_PERMUTE_1Z = 6;
constexpr uint32_t VE_PERMUTE_1W = 7;

constexpr uint32_t VE_SWIZZLE_X = 0;
constexpr uint32_t VE_SWIZZLE_Y = 1;
constexpr uint32_t VE_SWIZZLE_Z = 2;
constexpr uint32_t VE_SWIZZLE_W = 3;

constexpr uint32_t VE_CRMASK_CR6 = 0x000000F0;
constexpr uint32_t VE_CRMASK_CR6TRUE = 0x00000080;
constexpr uint32_t VE_CRMASK_CR6FALSE = 0x00000020;
constexpr uint32_t VE_CRMASK_CR6BOUNDS = VE_CRMASK_CR6FALSE;

inline constexpr float VeConvertToRadians(float fDegrees) noexcept { return fDegrees * (VE_PI / 180.0f); }
inline constexpr float VeConvertToDegrees(float fRadians) noexcept { return fRadians * (180.0f / VE_PI); }

inline bool VeComparisonAllTrue(uint32_t CR) noexcept { return (((CR)& VE_CRMASK_CR6TRUE) == VE_CRMASK_CR6TRUE); }
inline bool VeComparisonAnyTrue(uint32_t CR) noexcept { return (((CR)& VE_CRMASK_CR6FALSE) != VE_CRMASK_CR6FALSE); }
inline bool VeComparisonAllFalse(uint32_t CR) noexcept { return (((CR)& VE_CRMASK_CR6FALSE) == VE_CRMASK_CR6FALSE); }
inline bool VeComparisonAnyFalse(uint32_t CR) noexcept { return (((CR)& VE_CRMASK_CR6TRUE) != VE_CRMASK_CR6TRUE); }
inline bool VeComparisonMixed(uint32_t CR) noexcept { return (((CR)& VE_CRMASK_CR6) == 0); }
inline bool VeComparisonAllInBounds(uint32_t CR) noexcept { return (((CR)& VE_CRMASK_CR6BOUNDS) == VE_CRMASK_CR6BOUNDS); }
inline bool VeComparisonAnyOutOfBounds(uint32_t CR) noexcept { return (((CR)& VE_CRMASK_CR6BOUNDS) != VE_CRMASK_CR6BOUNDS); }

//--------------------------------------------------------------------------
// Vector intrinsic: Four 32 bit floating point components aligned on a 16
// byte boundary and mapped to hardware vector registers
#if defined(VE_SSE_INTRINSICS)
typedef __m128 VMVECTOR;
#elif defined(VE_NEON_INTRINSICS)
typedef float32x4_t VMVECTOR;
#else
struct VMVECTOR
{
	union
	{
		float       vector4_f32[4];
		uint32_t    vector4_u32[4];
	};
};
#endif

#ifdef VE_NO_INTRINSICS
typedef const VMVECTOR& FVMVECTOR;
#else
typedef const VMVECTOR FVMVECTOR;
#endif

#ifdef VE_NO_INTRINSICS
typedef const VMVECTOR& GVMVECTOR;
#else
typedef const VMVECTOR GVMVECTOR;
#endif

#ifdef VE_NO_INTRINSICS
typedef const VMVECTOR& HVMVECTOR;
#else
typedef const VMVECTOR HVMVECTOR;
#endif

typedef const VMVECTOR& CVMVECTOR;

//--------------------------------------------------------------------------
// Conversion types for constants
struct alignas(16) VMVECTORF32
{
	union
	{
		float f[4];
		VMVECTOR v;
	};

	inline operator VMVECTOR() const noexcept { return v; }
	inline operator const float*() const noexcept { return f; }
#	ifdef VE_SSE_INTRINSICS
	inline operator __m128i() const noexcept { return _mm_castps_si128(v); }
	inline operator __m128d() const noexcept { return _mm_castps_pd(v); }
#	endif
};

struct alignas(16) VMVECTORI32
{
	union
	{
		int32_t i[4];
		VMVECTOR v;
	};

	inline operator VMVECTOR() const noexcept { return v; }
#	ifdef VE_SSE_INTRINSICS
	inline operator __m128i() const noexcept { return _mm_castps_si128(v); }
	inline operator __m128d() const noexcept { return _mm_castps_pd(v); }
#	endif
};

struct alignas(16) VMVECTORU8
{
	union
	{
		uint8_t u[16];
		VMVECTOR v;
	};

	inline operator VMVECTOR() const noexcept { return v; }
#	ifdef VE_SSE_INTRINSICS
	inline operator __m128i() const noexcept { return _mm_castps_si128(v); }
	inline operator __m128d() const noexcept { return _mm_castps_pd(v); }
#	endif
};

struct alignas(16) VMVECTORU32
{
	union
	{
		uint32_t u[4];
		VMVECTOR v;
	};

	inline operator VMVECTOR() const noexcept { return v; }
#	ifdef VE_SSE_INTRINSICS
	inline operator __m128i() const noexcept { return _mm_castps_si128(v); }
	inline operator __m128d() const noexcept { return _mm_castps_pd(v); }
#	endif
};

//--------------------------------------------------------------------------
// Vector operators
VMVECTOR    VE_VECTORCALL     operator+ (FVMVECTOR V) noexcept;
VMVECTOR    VE_VECTORCALL     operator- (FVMVECTOR V) noexcept;

VMVECTOR&   VE_VECTORCALL     operator+= (VMVECTOR& V1, FVMVECTOR V2) noexcept;
VMVECTOR&   VE_VECTORCALL     operator-= (VMVECTOR& V1, FVMVECTOR V2) noexcept;
VMVECTOR&   VE_VECTORCALL     operator*= (VMVECTOR& V1, FVMVECTOR V2) noexcept;
VMVECTOR&   VE_VECTORCALL     operator/= (VMVECTOR& V1, FVMVECTOR V2) noexcept;

VMVECTOR&   operator*= (VMVECTOR& V, float S) noexcept;
VMVECTOR&   operator/= (VMVECTOR& V, float S) noexcept;

VMVECTOR    VE_VECTORCALL     operator+ (FVMVECTOR V1, FVMVECTOR V2) noexcept;
VMVECTOR    VE_VECTORCALL     operator- (FVMVECTOR V1, FVMVECTOR V2) noexcept;
VMVECTOR    VE_VECTORCALL     operator* (FVMVECTOR V1, FVMVECTOR V2) noexcept;
VMVECTOR    VE_VECTORCALL     operator/ (FVMVECTOR V1, FVMVECTOR V2) noexcept;
VMVECTOR    VE_VECTORCALL     operator* (FVMVECTOR V, float S) noexcept;
VMVECTOR    VE_VECTORCALL     operator* (float S, FVMVECTOR V) noexcept;
VMVECTOR    VE_VECTORCALL     operator/ (FVMVECTOR V, float S) noexcept;

//--------------------------------------------------------------------------
// Matrix type: Sixteen 32 bit floating point components aligned on a
// 16 byte boundary and mapped to four hardware vector registers
struct VMMATRIX;

#ifdef VE_NO_INTRINSICS
typedef const VMMATRIX& FVMMATRIX;
#else
typedef const VMMATRIX FVMMATRIX;
#endif

typedef const VMMATRIX& CVMMATRIX;

#ifdef VE_NO_INTRINSICS
struct VMMATRIX
#else
struct alignas(16) VMMATRIX
#endif
{
#	ifdef VE_NO_INTRINSICS
	union
	{
		VMVECTOR r[4];
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
#	else
	VMVECTOR r[4];
#	endif

	VMMATRIX() = default;

#	if defined(_MSC_VER) && _MSC_VER >= 1900
	constexpr VMMATRIX(FVMVECTOR R0, FVMVECTOR R1, FVMVECTOR R2, CVMVECTOR R3) noexcept : r{ R0,R1,R2,R3 } {}
#	else
	VMMATRIX(FVMVECTOR R0, FVMVECTOR R1, FVMVECTOR R2, CVMVECTOR R3) noexcept { r[0] = R0; r[1] = R1; r[2] = R2; r[3] = R3; }
#	endif

	VMMATRIX(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) noexcept;

	explicit VMMATRIX(const float * pArray) noexcept;

#	ifdef VE_NO_INTRINSICS
	float       operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
	float&      operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
#	endif

	VMMATRIX&   operator= (const VMMATRIX& M) noexcept { r[0] = M.r[0]; r[1] = M.r[1]; r[2] = M.r[2]; r[3] = M.r[3]; return *this; }

	VMMATRIX    operator+ () const noexcept { return *this; }
	VMMATRIX    operator- () const noexcept;

	VMMATRIX&   VE_VECTORCALL     operator+= (FVMMATRIX M) noexcept;
	VMMATRIX&   VE_VECTORCALL     operator-= (FVMMATRIX M) noexcept;
	VMMATRIX&   VE_VECTORCALL     operator*= (FVMMATRIX M) noexcept;
	VMMATRIX&   operator*= (float S) noexcept;
	VMMATRIX&   operator/= (float S) noexcept;

	VMMATRIX    VE_VECTORCALL     operator+ (FVMMATRIX M) const noexcept;
	VMMATRIX    VE_VECTORCALL     operator- (FVMMATRIX M) const noexcept;
	VMMATRIX    VE_VECTORCALL     operator* (FVMMATRIX M) const noexcept;
	VMMATRIX    operator* (float S) const noexcept;
	VMMATRIX    operator/ (float S) const noexcept;

	friend VMMATRIX     VE_VECTORCALL     operator* (float S, FVMMATRIX M) noexcept;

};

//--------------------------------------------------------------------------
// 2D Vector; 32 bit floating point components
struct VMFLOAT2
{
	float x;
	float y;

	VMFLOAT2() noexcept = default;
	constexpr VMFLOAT2(float _x, float _y) noexcept : x(_x), y(_y) {}
	explicit VMFLOAT2(const float *pArray) noexcept : x(pArray[0]), y(pArray[1]) {}

	VMFLOAT2& operator= (const VMFLOAT2& Float2) noexcept { x = Float2.x; y = Float2.y; return *this; }
};

// 2D Vector; 32 bit floating point components aligned on a 16 byte boundary
struct alignas(16) VMFLOAT2A : public VMFLOAT2
{
	VMFLOAT2A() noexcept = default;
	constexpr VMFLOAT2A(float _x, float _y) noexcept : VMFLOAT2(_x, _y) {}
	explicit VMFLOAT2A(const float *pArray) noexcept : VMFLOAT2(pArray) {}

	VMFLOAT2A& operator= (const VMFLOAT2A& Float2) noexcept { x = Float2.x; y = Float2.y; return *this; }
};

//--------------------------------------------------------------------------
// 2D Vector; 32 bit signed integer components
struct VMINT2
{
	int32_t x;
	int32_t y;

	VMINT2() noexcept = default;
	constexpr VMINT2(int32_t _x, int32_t _y) noexcept : x(_x), y(_y) {}
	explicit VMINT2(const int32_t *pArray) noexcept : x(pArray[0]), y(pArray[1]) {}

	VMINT2& operator= (const VMINT2& Int2) noexcept { x = Int2.x; y = Int2.y; return *this; }
};

// 2D Vector; 32 bit unsigned integer components
struct VMUINT2
{
	uint32_t x;
	uint32_t y;

	VMUINT2() noexcept = default;
	constexpr VMUINT2(uint32_t _x, uint32_t _y) noexcept : x(_x), y(_y) {}
	explicit VMUINT2(const uint32_t *pArray) noexcept : x(pArray[0]), y(pArray[1]) {}

	VMUINT2& operator= (const VMUINT2& UInt2) noexcept { x = UInt2.x; y = UInt2.y; return *this; }
};

//--------------------------------------------------------------------------
// 3D Vector; 32 bit floating point components
struct VMFLOAT3
{
	float x;
	float y;
	float z;

	VMFLOAT3() noexcept = default;
	constexpr VMFLOAT3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}
	explicit VMFLOAT3(const float *pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

	VMFLOAT3& operator= (const VMFLOAT3& Float3) noexcept { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
};

// 3D Vector; 32 bit floating point components aligned on a 16 byte boundary
struct alignas(16) VMFLOAT3A : public VMFLOAT3
{
	VMFLOAT3A() noexcept = default;
	constexpr VMFLOAT3A(float _x, float _y, float _z) noexcept : VMFLOAT3(_x, _y, _z) {}
	explicit VMFLOAT3A(const float *pArray) noexcept : VMFLOAT3(pArray) {}

	VMFLOAT3A& operator= (const VMFLOAT3A& Float3) noexcept { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
};

//--------------------------------------------------------------------------
// 3D Vector; 32 bit signed integer components
struct VMINT3
{
	int32_t x;
	int32_t y;
	int32_t z;

	VMINT3() noexcept = default;
	constexpr VMINT3(int32_t _x, int32_t _y, int32_t _z) noexcept : x(_x), y(_y), z(_z) {}
	explicit VMINT3(const int32_t *pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

	VMINT3& operator= (const VMINT3& i3) noexcept { x = i3.x; y = i3.y; z = i3.z; return *this; }
};

// 3D Vector; 32 bit unsigned integer components
struct VMUINT3
{
	uint32_t x;
	uint32_t y;
	uint32_t z;

	VMUINT3() noexcept = default;
	constexpr VMUINT3(uint32_t _x, uint32_t _y, uint32_t _z) noexcept : x(_x), y(_y), z(_z) {}
	explicit VMUINT3(const uint32_t *pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

	VMUINT3& operator= (const VMUINT3& u3) noexcept { x = u3.x; y = u3.y; z = u3.z; return *this; }
};

//--------------------------------------------------------------------------
// 4D Vector; 32 bit floating point components
struct VMFLOAT4
{
	float x;
	float y;
	float z;
	float w;

	VMFLOAT4() noexcept = default;
	constexpr VMFLOAT4(float _x, float _y, float _z, float _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
	explicit VMFLOAT4(const float *pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

	VMFLOAT4& operator= (const VMFLOAT4& Float4) noexcept { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
};

// 4D Vector; 32 bit floating point components aligned on a 16 byte boundary
struct alignas(16) VMFLOAT4A : public VMFLOAT4
{
	VMFLOAT4A() noexcept = default;
	constexpr VMFLOAT4A(float _x, float _y, float _z, float _w) noexcept : VMFLOAT4(_x, _y, _z, _w) {}
	explicit VMFLOAT4A(const float *pArray) noexcept : VMFLOAT4(pArray) {}

	VMFLOAT4A& operator= (const VMFLOAT4A& Float4) noexcept { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
};

//--------------------------------------------------------------------------
// 4D Vector; 32 bit signed integer components
struct VMINT4
{
	int32_t x;
	int32_t y;
	int32_t z;
	int32_t w;

	VMINT4() noexcept = default;
	constexpr VMINT4(int32_t _x, int32_t _y, int32_t _z, int32_t _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
	explicit VMINT4(const int32_t *pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

	VMINT4& operator= (const VMINT4& Int4) noexcept { x = Int4.x; y = Int4.y; z = Int4.z; w = Int4.w; return *this; }
};

// 4D Vector; 32 bit unsigned integer components
struct VMUINT4
{
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint32_t w;

	VMUINT4() noexcept = default;
	constexpr VMUINT4(uint32_t _x, uint32_t _y, uint32_t _z, uint32_t _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
	explicit VMUINT4(const uint32_t *pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

	VMUINT4& operator= (const VMUINT4& UInt4) noexcept { x = UInt4.x; y = UInt4.y; z = UInt4.z; w = UInt4.w; return *this; }
};

//--------------------------------------------------------------------------
// 3x3 Matrix: 32 bit floating point components
struct VMFLOAT3X3
{
	union
	{
		struct
		{
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};
		float m[3][3];
	};

	VMFLOAT3X3() noexcept = default;
	constexpr VMFLOAT3X3(float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22) noexcept
		: _11(m00), _12(m01), _13(m02),
		_21(m10), _22(m11), _23(m12),
		_31(m20), _32(m21), _33(m22) {}
	explicit VMFLOAT3X3(const float *pArray) noexcept;

	float       operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
	float&      operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }

	VMFLOAT3X3& operator= (const VMFLOAT3X3& Float3x3) noexcept;
};

//--------------------------------------------------------------------------
// 4x3 Matrix: 32 bit floating point components
struct VMFLOAT4X3
{
	union
	{
		struct
		{
			float _11, _21, _31, _41;
			float _12, _22, _32, _42;
			float _13, _23, _33, _43;
		};
		float m[3][4];
	};

	VMFLOAT4X3() noexcept = default;
	constexpr VMFLOAT4X3(float m00, float m10, float m20, float m30,
			float m01, float m11, float m21, float m31,
			float m02, float m12, float m22, float m32) noexcept
		: _11(m00), _21(m10), _31(m20), _41(m30),
		_12(m01), _22(m11), _32(m21), _42(m31),
		_13(m02), _23(m12), _33(m22), _43(m32) {}
	explicit VMFLOAT4X3(const float *pArray) noexcept;

	float       operator() (size_t Row, size_t Column) const noexcept { return m[Column][Row]; }
	float&      operator() (size_t Row, size_t Column) noexcept { return m[Column][Row]; }

	VMFLOAT4X3& operator= (const VMFLOAT4X3& Float4x3) noexcept;

};

// 4x3 Matrix: 32 bit floating point components aligned on a 16 byte boundary
struct alignas(16) VMFLOAT4X3A : public VMFLOAT4X3
{
	VMFLOAT4X3A() noexcept = default;
	constexpr VMFLOAT4X3A(float m00, float m10, float m20, float m30,
		float m01, float m11, float m21, float m31,
		float m02, float m12, float m22, float m32) noexcept
		: VMFLOAT4X3(m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32) {}
	explicit VMFLOAT4X3A(const float *pArray) noexcept : VMFLOAT4X3(pArray) {}

	float       operator() (size_t Row, size_t Column) const noexcept { return m[Column][Row]; }
	float&      operator() (size_t Row, size_t Column) noexcept { return m[Column][Row]; }

	VMFLOAT4X3A& operator= (const VMFLOAT4X3A& Float4x3) noexcept;
};

//--------------------------------------------------------------------------
// 4x4 Matrix: 32 bit floating point components
struct VMFLOAT4X4
{
	union
	{
		struct
		{
			float _11, _21, _31, _41;
			float _12, _22, _32, _42;
			float _13, _23, _33, _43;
			float _14, _24, _34, _44;
		};
		float m[4][4];
	};

	VMFLOAT4X4() noexcept = default;
	constexpr VMFLOAT4X4(float m00, float m10, float m20, float m30,
			float m01, float m11, float m21, float m31,
			float m02, float m12, float m22, float m32,
			float m03, float m13, float m23, float m33) noexcept
		: _11(m00), _21(m10), _31(m20), _41(m30),
		_12(m01), _22(m11), _32(m21), _42(m31),
		_13(m02), _23(m12), _33(m22), _43(m32),
		_14(m03), _24(m13), _34(m23), _44(m33) {}
	explicit VMFLOAT4X4(const float *pArray) noexcept;

	float       operator() (size_t Row, size_t Column) const noexcept { return m[Column][Row]; }
	float&      operator() (size_t Row, size_t Column) noexcept { return m[Column][Row]; }

	VMFLOAT4X4& operator= (const VMFLOAT4X4& Float4x4) noexcept;
};

// 4x4 Matrix: 32 bit floating point components aligned on a 16 byte boundary
struct alignas(16) VMFLOAT4X4A : public VMFLOAT4X4
{
	VMFLOAT4X4A() noexcept = default;
	constexpr VMFLOAT4X4A(float m00, float m10, float m20, float m30,
		float m01, float m11, float m21, float m31,
		float m02, float m12, float m22, float m32,
		float m03, float m13, float m23, float m33) noexcept
		: VMFLOAT4X4(m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33) {}
	explicit VMFLOAT4X4A(const float *pArray) noexcept : VMFLOAT4X4(pArray) {}

	float       operator() (size_t Row, size_t Column) const noexcept { return m[Column][Row]; }
	float&      operator() (size_t Row, size_t Column) noexcept { return m[Column][Row]; }

	VMFLOAT4X4A& operator= (const VMFLOAT4X4A& Float4x4) noexcept;
};

//--------------------------------------------------------------------------
//Data conversion operations
VMVECTOR    VE_VECTORCALL     VeConvertVectorIntToFloat(FVMVECTOR VInt, uint32_t DivExponent) noexcept;
VMVECTOR    VE_VECTORCALL     VeConvertVectorFloatToInt(FVMVECTOR VFloat, uint32_t MulExponent) noexcept;
VMVECTOR    VE_VECTORCALL     VeConvertVectorUIntToFloat(FVMVECTOR VUInt, uint32_t DivExponent) noexcept;
VMVECTOR    VE_VECTORCALL     VeConvertVectorFloatToUInt(FVMVECTOR VFloat, uint32_t MulExponent) noexcept;

VMVECTOR    VE_VECTORCALL     VeVectorSetBinaryConstant(uint32_t C0, uint32_t C1, uint32_t C2, uint32_t C3) noexcept;
VMVECTOR    VE_VECTORCALL     VeVectorSplatConstant(int32_t IntConstant, uint32_t DivExponent) noexcept;
VMVECTOR    VE_VECTORCALL     VeVectorSplatConstantInt(int32_t IntConstant) noexcept;

//--------------------------------------------------------------------------
//Load operations
VMVECTOR    VE_VECTORCALL     VeLoadInt(const uint32_t* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadFloat(const float* pSource) noexcept;

VMVECTOR    VE_VECTORCALL     VeLoadInt2(const uint32_t* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadInt2A(const uint32_t* PSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadFloat2(const VMFLOAT2* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadFloat2A(const VMFLOAT2A* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadSInt2(const VMINT2* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadUInt2(const VMUINT2* pSource) noexcept;

VMVECTOR    VE_VECTORCALL     VeLoadInt3(const uint32_t* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadInt3A(const uint32_t* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadFloat3(const VMFLOAT3* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadFloat3A(const VMFLOAT3A* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadSInt3(const VMINT3* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadUInt3(const VMUINT3* pSource) noexcept;

VMVECTOR    VE_VECTORCALL     VeLoadInt4(const uint32_t* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadInt4A(const uint32_t* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadFloat4(const VMFLOAT4* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadFloat4A(const VMFLOAT4A* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadSInt4(const VMINT4* pSource) noexcept;
VMVECTOR    VE_VECTORCALL     VeLoadUInt4(const VMUINT4* pSource) noexcept;

VMMATRIX    VE_VECTORCALL     VeLoadFloat3x3(const VMFLOAT3X3* pSource) noexcept;
VMMATRIX    VE_VECTORCALL     VeLoadFloat4x3(const VMFLOAT4X3* pSource) noexcept;
VMMATRIX    VE_VECTORCALL     VeLoadFloat4x3A(const VMFLOAT4X3A* pSource) noexcept;
VMMATRIX    VE_VECTORCALL     VeLoadFloat4x4(const VMFLOAT4X4* pSource) noexcept;
VMMATRIX    VE_VECTORCALL     VeLoadFloat4x4A(const VMFLOAT4X4A* pSource) noexcept;

//--------------------------------------------------------------------------
//Store operations
void        VE_VECTORCALL     VeStoreInt(uint32_t* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreFloat(float* pDestination, FVMVECTOR V) noexcept;

void        VE_VECTORCALL     VeStoreInt2(uint32_t* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreInt2A(uint32_t* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreFloat2(VMFLOAT2* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreFloat2A(VMFLOAT2A* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreSInt2(VMINT2* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreUInt2(VMUINT2* pDestination, FVMVECTOR V) noexcept;

void        VE_VECTORCALL     VeStoreInt3(uint32_t* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreInt3A(uint32_t* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreFloat3(VMFLOAT3* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreFloat3A(VMFLOAT3A* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreSInt3(VMINT3* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreUInt3(VMUINT3* pDestination, FVMVECTOR V) noexcept;

void        VE_VECTORCALL     VeStoreInt4(uint32_t* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreInt4A(uint32_t* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreFloat4(VMFLOAT4* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreFloat4A(VMFLOAT4A* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreSInt4(VMINT4* pDestination, FVMVECTOR V) noexcept;
void        VE_VECTORCALL     VeStoreUInt4(VMUINT4* pDestination, FVMVECTOR V) noexcept;

void        VE_VECTORCALL     VeStoreFloat3x3(VMFLOAT3X3* pDestination, FVMMATRIX M) noexcept;
void        VE_VECTORCALL     VeStoreFloat4x3(VMFLOAT4X3* pDestination, FVMMATRIX M) noexcept;
void        VE_VECTORCALL     VeStoreFloat4x3A(VMFLOAT4X3A* pDestination, FVMMATRIX M) noexcept;
void        VE_VECTORCALL     VeStoreFloat4x4(VMFLOAT4X4* pDestination, FVMMATRIX M) noexcept;
void        VE_VECTORCALL     VeStoreFloat4x4A(VMFLOAT4X4A* pDestination, FVMMATRIX M) noexcept;

//--------------------------------------------------------------------------
//General vector operations



#include "VeSIMDMathConvert.inl"
