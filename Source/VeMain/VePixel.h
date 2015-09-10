////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VePixel.h
//  Created:     2015/08/07 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#define VE_ALPHA_OPAQUE 255
#define VE_ALPHA_TRANSPARENT 0

enum
{
	VE_PIXELTYPE_UNKNOWN,
	VE_PIXELTYPE_INDEX1,
	VE_PIXELTYPE_INDEX4,
	VE_PIXELTYPE_INDEX8,
	VE_PIXELTYPE_PACKED8,
	VE_PIXELTYPE_PACKED16,
	VE_PIXELTYPE_PACKED32,
	VE_PIXELTYPE_ARRAYU8,
	VE_PIXELTYPE_ARRAYU16,
	VE_PIXELTYPE_ARRAYU32,
	VE_PIXELTYPE_ARRAYF16,
	VE_PIXELTYPE_ARRAYF32
};

enum
{
	VE_BITMAPORDER_NONE,
	VE_BITMAPORDER_4321,
	VE_BITMAPORDER_1234
};

enum
{
	VE_PACKEDORDER_NONE,
	VE_PACKEDORDER_XRGB,
	VE_PACKEDORDER_RGBX,
	VE_PACKEDORDER_ARGB,
	VE_PACKEDORDER_RGBA,
	VE_PACKEDORDER_XBGR,
	VE_PACKEDORDER_BGRX,
	VE_PACKEDORDER_ABGR,
	VE_PACKEDORDER_BGRA
};

enum
{
	VE_ARRAYORDER_NONE,
	VE_ARRAYORDER_RGB,
	VE_ARRAYORDER_RGBA,
	VE_ARRAYORDER_ARGB,
	VE_ARRAYORDER_BGR,
	VE_ARRAYORDER_BGRA,
	VE_ARRAYORDER_ABGR
};

enum
{
	VE_PACKEDLAYOUT_NONE,
	VE_PACKEDLAYOUT_332,
	VE_PACKEDLAYOUT_4444,
	VE_PACKEDLAYOUT_1555,
	VE_PACKEDLAYOUT_5551,
	VE_PACKEDLAYOUT_565,
	VE_PACKEDLAYOUT_8888,
	VE_PACKEDLAYOUT_2101010,
	VE_PACKEDLAYOUT_1010102
};

#define VE_DEFINE_PIXELFOURCC(A, B, C, D) VE_FOURCC(A, B, C, D)

#define VE_DEFINE_PIXELFORMAT(type, order, layout, bits, bytes) \
    ((1 << 28) | ((type) << 24) | ((order) << 20) | ((layout) << 16) | \
     ((bits) << 8) | ((bytes) << 0))

#define VE_PIXELFLAG(X)    (((X) >> 28) & 0x0F)
#define VE_PIXELTYPE(X)    (((X) >> 24) & 0x0F)
#define VE_PIXELORDER(X)   (((X) >> 20) & 0x0F)
#define VE_PIXELLAYOUT(X)  (((X) >> 16) & 0x0F)
#define VE_BITSPERPIXEL(X) (((X) >> 8) & 0xFF)
#define VE_BYTESPERPIXEL(X) \
    (VE_ISPIXELFORMAT_FOURCC(X) ? \
        ((((X) == VE_PIXELFORMAT_YUY2) || \
          ((X) == VE_PIXELFORMAT_UYVY) || \
          ((X) == VE_PIXELFORMAT_YVYU)) ? 2 : 1) : (((X) >> 0) & 0xFF))

#define VE_ISPIXELFORMAT_INDEXED(format)   \
    (!VE_ISPIXELFORMAT_FOURCC(format) && \
     ((VE_PIXELTYPE(format) == VE_PIXELTYPE_INDEX1) || \
      (VE_PIXELTYPE(format) == VE_PIXELTYPE_INDEX4) || \
      (VE_PIXELTYPE(format) == VE_PIXELTYPE_INDEX8)))

#define VE_ISPIXELFORMAT_ALPHA(format)   \
    (!VE_ISPIXELFORMAT_FOURCC(format) && \
     ((VE_PIXELORDER(format) == VE_PACKEDORDER_ARGB) || \
      (VE_PIXELORDER(format) == VE_PACKEDORDER_RGBA) || \
      (VE_PIXELORDER(format) == VE_PACKEDORDER_ABGR) || \
      (VE_PIXELORDER(format) == VE_PACKEDORDER_BGRA)))

#define VE_ISPIXELFORMAT_FOURCC(format)    \
    ((format) && (VE_PIXELFLAG(format) != 1))

#define VE_FOURCC VE_MAKE_ID

enum
{
	VE_PIXELFORMAT_UNKNOWN,
	VE_PIXELFORMAT_INDEX1LSB =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_INDEX1, VE_BITMAPORDER_4321, 0,
		1, 0),
	VE_PIXELFORMAT_INDEX1MSB =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_INDEX1, VE_BITMAPORDER_1234, 0,
		1, 0),
	VE_PIXELFORMAT_INDEX4LSB =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_INDEX4, VE_BITMAPORDER_4321, 0,
		4, 0),
	VE_PIXELFORMAT_INDEX4MSB =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_INDEX4, VE_BITMAPORDER_1234, 0,
		4, 0),
	VE_PIXELFORMAT_INDEX8 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_INDEX8, 0, 0, 8, 1),
	VE_PIXELFORMAT_RGB332 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED8, VE_PACKEDORDER_XRGB,
		VE_PACKEDLAYOUT_332, 8, 1),
	VE_PIXELFORMAT_RGB444 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_XRGB,
		VE_PACKEDLAYOUT_4444, 12, 2),
	VE_PIXELFORMAT_RGB555 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_XRGB,
		VE_PACKEDLAYOUT_1555, 15, 2),
	VE_PIXELFORMAT_BGR555 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_XBGR,
		VE_PACKEDLAYOUT_1555, 15, 2),
	VE_PIXELFORMAT_ARGB4444 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_ARGB,
		VE_PACKEDLAYOUT_4444, 16, 2),
	VE_PIXELFORMAT_RGBA4444 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_RGBA,
		VE_PACKEDLAYOUT_4444, 16, 2),
	VE_PIXELFORMAT_ABGR4444 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_ABGR,
		VE_PACKEDLAYOUT_4444, 16, 2),
	VE_PIXELFORMAT_BGRA4444 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_BGRA,
		VE_PACKEDLAYOUT_4444, 16, 2),
	VE_PIXELFORMAT_ARGB1555 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_ARGB,
		VE_PACKEDLAYOUT_1555, 16, 2),
	VE_PIXELFORMAT_RGBA5551 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_RGBA,
		VE_PACKEDLAYOUT_5551, 16, 2),
	VE_PIXELFORMAT_ABGR1555 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_ABGR,
		VE_PACKEDLAYOUT_1555, 16, 2),
	VE_PIXELFORMAT_BGRA5551 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_BGRA,
		VE_PACKEDLAYOUT_5551, 16, 2),
	VE_PIXELFORMAT_RGB565 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_XRGB,
		VE_PACKEDLAYOUT_565, 16, 2),
	VE_PIXELFORMAT_BGR565 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED16, VE_PACKEDORDER_XBGR,
		VE_PACKEDLAYOUT_565, 16, 2),
	VE_PIXELFORMAT_RGB24 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_ARRAYU8, VE_ARRAYORDER_RGB, 0,
		24, 3),
	VE_PIXELFORMAT_BGR24 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_ARRAYU8, VE_ARRAYORDER_BGR, 0,
		24, 3),
	VE_PIXELFORMAT_RGB888 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED32, VE_PACKEDORDER_XRGB,
		VE_PACKEDLAYOUT_8888, 24, 4),
	VE_PIXELFORMAT_RGBX8888 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED32, VE_PACKEDORDER_RGBX,
		VE_PACKEDLAYOUT_8888, 24, 4),
	VE_PIXELFORMAT_BGR888 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED32, VE_PACKEDORDER_XBGR,
		VE_PACKEDLAYOUT_8888, 24, 4),
	VE_PIXELFORMAT_BGRX8888 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED32, VE_PACKEDORDER_BGRX,
		VE_PACKEDLAYOUT_8888, 24, 4),
	VE_PIXELFORMAT_ARGB8888 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED32, VE_PACKEDORDER_ARGB,
		VE_PACKEDLAYOUT_8888, 32, 4),
	VE_PIXELFORMAT_RGBA8888 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED32, VE_PACKEDORDER_RGBA,
		VE_PACKEDLAYOUT_8888, 32, 4),
	VE_PIXELFORMAT_ABGR8888 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED32, VE_PACKEDORDER_ABGR,
		VE_PACKEDLAYOUT_8888, 32, 4),
	VE_PIXELFORMAT_BGRA8888 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED32, VE_PACKEDORDER_BGRA,
		VE_PACKEDLAYOUT_8888, 32, 4),
	VE_PIXELFORMAT_ARGB2101010 =
	VE_DEFINE_PIXELFORMAT(VE_PIXELTYPE_PACKED32, VE_PACKEDORDER_ARGB,
		VE_PACKEDLAYOUT_2101010, 32, 4),

	VE_PIXELFORMAT_YV12 =
	VE_FOURCC('Y', 'V', '1', '2'),
	VE_PIXELFORMAT_IYUV =
	VE_FOURCC('I', 'Y', 'U', 'V'),
	VE_PIXELFORMAT_YUY2 =
	VE_FOURCC('Y', 'U', 'Y', '2'),
	VE_PIXELFORMAT_UYVY =
	VE_FOURCC('U', 'Y', 'V', 'Y'),
	VE_PIXELFORMAT_YVYU =
	VE_FOURCC('Y', 'V', 'Y', 'U')
};

class VeRGBA;

class VE_MAIN_API alignas(16) VeColor : public VeMemObject
{
public:
	union
	{
		struct
		{
			VeFloat32 r, g, b, a;
		};
		VeFloat32 f[4];
		VE_FLOAT4A v;
	};

	VeColor(VeFloat32 fR = 0.0f, VeFloat32 fG = 0.0f, VeFloat32 fB = 0.0f, VeFloat32 fA = 0.0f) noexcept
	{
		VE_VECTOR vec = VeVectorSet(fR, fG, fB, fA);
		VeStoreFloat4A(&v, vec);
	}

	VeColor(std::initializer_list<VeFloat32> kInitList) noexcept
	{
		VE_VECTOR vec = VeLoadFloat4((const VE_FLOAT4*)kInitList.begin());
		VeStoreFloat4A(&v, vec);
	}

	inline operator VeRGBA () const noexcept;

	inline VeColor& operator = (VeFloat32 f32Scalar) noexcept;

	inline bool operator == (const VeColor& c) const noexcept;

	inline bool operator != (const VeColor& c) const noexcept;

	inline VeColor operator + (const VeColor& c) const noexcept;

	inline VeColor operator - (const VeColor& c) const noexcept;

	inline VeColor operator * (VeFloat32 f32Scalar) const noexcept;

	inline VeColor operator * (const VeColor& c) const noexcept;

	inline VeColor operator / (VeFloat32 f32Scalar) const noexcept;

	inline VeColor operator / (const VeColor& c) const noexcept;

	inline VeColor operator - () const noexcept;

	friend inline VeColor operator * (VeFloat32 f32Scalar, const VeColor& c) noexcept;

	inline VeColor& operator += (const VeColor& c) noexcept;

	inline VeColor& operator -= (const VeColor& c) noexcept;

	inline VeColor& operator *= (VeFloat32 f32Scalar) noexcept;

	inline VeColor& operator *= (const VeColor& c) noexcept;

	inline VeColor& operator /= (VeFloat32 f32Scalar) noexcept;

	inline VeColor& operator /= (const VeColor& c) noexcept;

	inline void Clamp() noexcept;

	inline void SetZero() noexcept;

	inline void Set(VeFloat32 f32R, VeFloat32 f32G, VeFloat32 f32B, VeFloat32 f32A = 1.0f) noexcept;

	inline void Scale() noexcept;

	inline void GetAs(VeRGBA& kColor) const noexcept;

	static const VeColor ZERO;
	static const VeColor WHITE;
	static const VeColor BLACK;

};

class VE_MAIN_API VeRGBA : public VeMemObject
{
public:
	union
	{
		struct
		{
			VeUInt8 r, g, b, a;
		};
		VeUInt8 m_au8Channel[4];
		VeUInt32 m_u32Color;
	};

	enum
	{
		R, G, B, A
	};	

	VeRGBA(VeUInt32 u32Color) noexcept
		: m_u32Color(u32Color) {}

	VeRGBA(VeUInt8 u8Red = 0, VeUInt8 u8Green = 0, VeUInt8 u8Blue = 0, VeUInt8 u8Alpha = 0) noexcept
	{
		r = u8Red;
		g = u8Green;
		b = u8Blue;
		a = u8Alpha;
	}

	inline operator VeColor () noexcept;

	inline VeRGBA& operator = (VeUInt8 uiValue) noexcept;

	inline bool operator == (const VeRGBA& c) const noexcept;

	inline bool operator!= (const VeRGBA& c) const noexcept;

	inline VeRGBA operator + (const VeRGBA& c) const noexcept;

	inline VeRGBA operator - (const VeRGBA& c) const noexcept;

	inline VeRGBA operator * (VeFloat32 f32Scalar) const noexcept;

	inline VeRGBA operator * (const VeRGBA& c) const noexcept;

	inline VeRGBA operator / (VeFloat32 f32Scalar) const noexcept;

	inline VeRGBA operator / (const VeRGBA& c) const noexcept;

	inline VeRGBA operator - () const noexcept;

	friend inline VeRGBA operator * (VeFloat32 f32Scalar, const VeRGBA& c) noexcept;

	inline VeRGBA& operator += (const VeRGBA& c) noexcept;

	inline VeRGBA& operator -= (const VeRGBA& c) noexcept;

	inline VeRGBA& operator *= (VeFloat32 f32Scalar) noexcept;

	inline VeRGBA& operator *= (const VeRGBA& c) noexcept;

	inline VeRGBA& operator /= (VeFloat32 f32Scalar) noexcept;

	inline VeRGBA& operator /= (const VeRGBA& c) noexcept;

	inline void GetAs(VeUInt32& u32Color) const noexcept;

	inline void GetAsBGRA(VeUInt32& u32Color) const noexcept;

	inline void GetAs(VeColor& kColor) const noexcept;

	static const VeRGBA ZERO;
	static const VeRGBA WHITE;
	static const VeRGBA BLACK;

};

struct VePalette : public VeRefObject
{
	VePalette(VeInt32 i32NumColors) noexcept;

	VeVector<VeRGBA> m_kColors;
	VeUInt32 m_u32Version;
};

typedef VePointer<VePalette> VePalettePtr;
struct VePixelFormat;
typedef VePointer<VePixelFormat> VePixelFormatPtr;

struct VePixelFormat : public VeRefObject
{
	VeUInt32 m_u32Format;
	VePalettePtr m_spPalette;
	VeUInt8 m_u8BitsPerPixel;
	VeUInt8 m_u8BytesPerPixel;
	VeUInt8 m_u8Padding1 = 0;
	VeUInt8 m_u8Padding2 = 0;
	VeUInt32 m_u32Rmask;
	VeUInt32 m_u32Gmask;
	VeUInt32 m_u32Bmask;
	VeUInt32 m_u32Amask;
	VeUInt8 m_u8Rloss;
	VeUInt8 m_u8Gloss;
	VeUInt8 m_u8Bloss;
	VeUInt8 m_u8Aloss;
	VeUInt8 m_u8Rshift;
	VeUInt8 m_u8Gshift;
	VeUInt8 m_u8Bshift;
	VeUInt8 m_u8Ashift;

	inline bool SetPalette(const VePalettePtr& spPalette) noexcept;
	
	static VePixelFormatPtr Create(VeUInt32 u32PixelFormat) noexcept;

protected:
	VePixelFormat();

	bool Init(VeUInt32 u32PixelFormat) noexcept;

	VeRefNode<VePixelFormat*> m_kNode;

	static VeRefList<VePixelFormat*> ms_kList;

};

const VeChar8* VeGetPixelFormatName(VeUInt32 u32Format) noexcept;

bool VePixelFormatEnumToMasks(VeUInt32 u32Format, VeInt32& i32Bpp, VeUInt32& u32Rmask,
	VeUInt32& u32Gmask, VeUInt32& u32Bmask, VeUInt32& u32Amask) noexcept;

VeUInt32 VeMasksToPixelFormatEnum(VeInt32 i32Bpp, VeUInt32 u32Rmask,
	VeUInt32 u32Gmask, VeUInt32 u32Bmask, VeUInt32 u32Amask) noexcept;

#include "VePixel.inl"
