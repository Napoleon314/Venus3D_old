////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      VeRenderer
//  File name:   VeRenderState.h
//  Created:     2016/07/29 by Albert
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

enum VePixelFormat
{
	VE_PF_UNKNOWN = 0,
	VE_PF_R32G32B32A32_TYPELESS = 1,
	VE_PF_R32G32B32A32_FLOAT = 2,
	VE_PF_R32G32B32A32_UINT = 3,
	VE_PF_R32G32B32A32_SINT = 4,
	VE_PF_R32G32B32_TYPELESS = 5,
	VE_PF_R32G32B32_FLOAT = 6,
	VE_PF_R32G32B32_UINT = 7,
	VE_PF_R32G32B32_SINT = 8,
	VE_PF_R16G16B16A16_TYPELESS = 9,
	VE_PF_R16G16B16A16_FLOAT = 10,
	VE_PF_R16G16B16A16_UNORM = 11,
	VE_PF_R16G16B16A16_UINT = 12,
	VE_PF_R16G16B16A16_SNORM = 13,
	VE_PF_R16G16B16A16_SINT = 14,
	VE_PF_R32G32_TYPELESS = 15,
	VE_PF_R32G32_FLOAT = 16,
	VE_PF_R32G32_UINT = 17,
	VE_PF_R32G32_SINT = 18,
	VE_PF_R32G8X24_TYPELESS = 19,
	VE_PF_D32_FLOAT_S8X24_UINT = 20,
	VE_PF_R32_FLOAT_X8X24_TYPELESS = 21,
	VE_PF_X32_TYPELESS_G8X24_UINT = 22,
	VE_PF_R10G10B10A2_TYPELESS = 23,
	VE_PF_R10G10B10A2_UNORM = 24,
	VE_PF_R10G10B10A2_UINT = 25,
	VE_PF_R11G11B10_FLOAT = 26,
	VE_PF_R8G8B8A8_TYPELESS = 27,
	VE_PF_R8G8B8A8_UNORM = 28,
	VE_PF_R8G8B8A8_UNORM_SRGB = 29,
	VE_PF_R8G8B8A8_UINT = 30,
	VE_PF_R8G8B8A8_SNORM = 31,
	VE_PF_R8G8B8A8_SINT = 32,
	VE_PF_R16G16_TYPELESS = 33,
	VE_PF_R16G16_FLOAT = 34,
	VE_PF_R16G16_UNORM = 35,
	VE_PF_R16G16_UINT = 36,
	VE_PF_R16G16_SNORM = 37,
	VE_PF_R16G16_SINT = 38,
	VE_PF_R32_TYPELESS = 39,
	VE_PF_D32_FLOAT = 40,
	VE_PF_R32_FLOAT = 41,
	VE_PF_R32_UINT = 42,
	VE_PF_R32_SINT = 43,
	VE_PF_R24G8_TYPELESS = 44,
	VE_PF_D24_UNORM_S8_UINT = 45,
	VE_PF_R24_UNORM_X8_TYPELESS = 46,
	VE_PF_X24_TYPELESS_G8_UINT = 47,
	VE_PF_R8G8_TYPELESS = 48,
	VE_PF_R8G8_UNORM = 49,
	VE_PF_R8G8_UINT = 50,
	VE_PF_R8G8_SNORM = 51,
	VE_PF_R8G8_SINT = 52,
	VE_PF_R16_TYPELESS = 53,
	VE_PF_R16_FLOAT = 54,
	VE_PF_D16_UNORM = 55,
	VE_PF_R16_UNORM = 56,
	VE_PF_R16_UINT = 57,
	VE_PF_R16_SNORM = 58,
	VE_PF_R16_SINT = 59,
	VE_PF_R8_TYPELESS = 60,
	VE_PF_R8_UNORM = 61,
	VE_PF_R8_UINT = 62,
	VE_PF_R8_SNORM = 63,
	VE_PF_R8_SINT = 64,
	VE_PF_A8_UNORM = 65,
	VE_PF_R1_UNORM = 66,
	VE_PF_R9G9B9E5_SHAREDEXP = 67,
	VE_PF_R8G8_B8G8_UNORM = 68,
	VE_PF_G8R8_G8B8_UNORM = 69,
	VE_PF_BC1_TYPELESS = 70,
	VE_PF_BC1_UNORM = 71,
	VE_PF_BC1_UNORM_SRGB = 72,
	VE_PF_BC2_TYPELESS = 73,
	VE_PF_BC2_UNORM = 74,
	VE_PF_BC2_UNORM_SRGB = 75,
	VE_PF_BC3_TYPELESS = 76,
	VE_PF_BC3_UNORM = 77,
	VE_PF_BC3_UNORM_SRGB = 78,
	VE_PF_BC4_TYPELESS = 79,
	VE_PF_BC4_UNORM = 80,
	VE_PF_BC4_SNORM = 81,
	VE_PF_BC5_TYPELESS = 82,
	VE_PF_BC5_UNORM = 83,
	VE_PF_BC5_SNORM = 84,
	VE_PF_B5G6R5_UNORM = 85,
	VE_PF_B5G5R5A1_UNORM = 86,
	VE_PF_B8G8R8A8_UNORM = 87,
	VE_PF_B8G8R8X8_UNORM = 88,
	VE_PF_R10G10B10_XR_BIAS_A2_UNORM = 89,
	VE_PF_B8G8R8A8_TYPELESS = 90,
	VE_PF_B8G8R8A8_UNORM_SRGB = 91,
	VE_PF_B8G8R8X8_TYPELESS = 92,
	VE_PF_B8G8R8X8_UNORM_SRGB = 93,
	VE_PF_BC6H_TYPELESS = 94,
	VE_PF_BC6H_UF16 = 95,
	VE_PF_BC6H_SF16 = 96,
	VE_PF_BC7_TYPELESS = 97,
	VE_PF_BC7_UNORM = 98,
	VE_PF_BC7_UNORM_SRGB = 99,
	VE_PF_AYUV = 100,
	VE_PF_Y410 = 101,
	VE_PF_Y416 = 102,
	VE_PF_NV12 = 103,
	VE_PF_P010 = 104,
	VE_PF_P016 = 105,
	VE_PF_420_OPAQUE = 106,
	VE_PF_YUY2 = 107,
	VE_PF_Y210 = 108,
	VE_PF_Y216 = 109,
	VE_PF_NV11 = 110,
	VE_PF_AI44 = 111,
	VE_PF_IA44 = 112,
	VE_PF_P8 = 113,
	VE_PF_A8P8 = 114,
	VE_PF_B4G4R4A4_UNORM = 115,

	VE_PF_P208 = 130,
	VE_PF_V208 = 131,
	VE_PF_V408 = 132,

	VE_PF_FORCE_UINT = 0xffffffff
};

enum VeSematicName
{
	VE_SN_BINORMAL,
	VE_SN_BLENDINDICES,
	VE_SN_BLENDWEIGHT,
	VE_SN_COLOR,
	VE_SN_NORMAL,
	VE_SN_POSITION,
	VE_SN_POSITIONT,
	VE_SN_PSIZE,
	VE_SN_TANGENT,
	VE_SN_TEXCOORD,
	VE_SN_NUM
};

enum VeInputClassification
{
	VE_IC_PER_VERTEX_DATA = 0,
	VE_IC_PER_INSTANCE_DATA = 1
};

enum VePrimitiveTopology
{
	VE_PT_UNDEFINED = 0,
	VE_PT_POINTLIST = 1,
	VE_PT_LINELIST = 2,
	VE_PT_LINESTRIP = 3,
	VE_PT_TRIANGLELIST = 4,
	VE_PT_TRIANGLESTRIP = 5,
	VE_PT_LINELIST_ADJ = 10,
	VE_PT_LINESTRIP_ADJ = 11,
	VE_PT_TRIANGLELIST_ADJ = 12,
	VE_PT_TRIANGLESTRIP_ADJ = 13,
	VE_PT_1_CONTROL_POINT_PATCHLIST = 33,
	VE_PT_2_CONTROL_POINT_PATCHLIST = 34,
	VE_PT_3_CONTROL_POINT_PATCHLIST = 35,
	VE_PT_4_CONTROL_POINT_PATCHLIST = 36,
	VE_PT_5_CONTROL_POINT_PATCHLIST = 37,
	VE_PT_6_CONTROL_POINT_PATCHLIST = 38,
	VE_PT_7_CONTROL_POINT_PATCHLIST = 39,
	VE_PT_8_CONTROL_POINT_PATCHLIST = 40,
	VE_PT_9_CONTROL_POINT_PATCHLIST = 41,
	VE_PT_10_CONTROL_POINT_PATCHLIST = 42,
	VE_PT_11_CONTROL_POINT_PATCHLIST = 43,
	VE_PT_12_CONTROL_POINT_PATCHLIST = 44,
	VE_PT_13_CONTROL_POINT_PATCHLIST = 45,
	VE_PT_14_CONTROL_POINT_PATCHLIST = 46,
	VE_PT_15_CONTROL_POINT_PATCHLIST = 47,
	VE_PT_16_CONTROL_POINT_PATCHLIST = 48,
	VE_PT_17_CONTROL_POINT_PATCHLIST = 49,
	VE_PT_18_CONTROL_POINT_PATCHLIST = 50,
	VE_PT_19_CONTROL_POINT_PATCHLIST = 51,
	VE_PT_20_CONTROL_POINT_PATCHLIST = 52,
	VE_PT_21_CONTROL_POINT_PATCHLIST = 53,
	VE_PT_22_CONTROL_POINT_PATCHLIST = 54,
	VE_PT_23_CONTROL_POINT_PATCHLIST = 55,
	VE_PT_24_CONTROL_POINT_PATCHLIST = 56,
	VE_PT_25_CONTROL_POINT_PATCHLIST = 57,
	VE_PT_26_CONTROL_POINT_PATCHLIST = 58,
	VE_PT_27_CONTROL_POINT_PATCHLIST = 59,
	VE_PT_28_CONTROL_POINT_PATCHLIST = 60,
	VE_PT_29_CONTROL_POINT_PATCHLIST = 61,
	VE_PT_30_CONTROL_POINT_PATCHLIST = 62,
	VE_PT_31_CONTROL_POINT_PATCHLIST = 63,
	VE_PT_32_CONTROL_POINT_PATCHLIST = 64,
	VE_PT_MAX
};

class VENUS_API VeRenderState : public VeRefObject
{
	VeNoCopy(VeRenderState);
	VeRTTIDecl(VeRenderState);
public:
	VeRenderState() noexcept = default;

	virtual ~VeRenderState() noexcept = default;

};

VeSmartPointer(VeRenderState);

class VENUS_API VeVertexShader : public VeRenderState
{
	VeNoCopy(VeVertexShader);
	VeRTTIDecl(VeVertexShader, VeRenderState);
public:
	VeVertexShader() noexcept = default;

	virtual ~VeVertexShader() noexcept = default;
};

VeSmartPointer(VeVertexShader);

class VENUS_API VePixelShader : public VeRenderState
{
	VeNoCopy(VePixelShader);
	VeRTTIDecl(VePixelShader, VeRenderState);
public:
	VePixelShader() noexcept = default;

	virtual ~VePixelShader() noexcept = default;
};

VeSmartPointer(VePixelShader);

class VENUS_API VeGeometryShader : public VeRenderState
{
	VeNoCopy(VeGeometryShader);
	VeRTTIDecl(VeGeometryShader, VeRenderState);
public:
	VeGeometryShader() noexcept = default;

	virtual ~VeGeometryShader() noexcept = default;
};

VeSmartPointer(VeGeometryShader);

class VENUS_API VeHullShader : public VeRenderState
{
	VeNoCopy(VeHullShader);
	VeRTTIDecl(VeHullShader, VeRenderState);
public:
	VeHullShader() noexcept = default;

	virtual ~VeHullShader() noexcept = default;
};

VeSmartPointer(VeHullShader);

class VENUS_API VeDomainShader : public VeRenderState
{
	VeNoCopy(VeDomainShader);
	VeRTTIDecl(VeDomainShader, VeRenderState);
public:
	VeDomainShader() noexcept = default;

	virtual ~VeDomainShader() noexcept = default;
};

VeSmartPointer(VeDomainShader);

class VENUS_API VeComputeShader : public VeRenderState
{
	VeNoCopy(VeComputeShader);
	VeRTTIDecl(VeComputeShader, VeRenderState);
public:
	VeComputeShader() noexcept = default;

	virtual ~VeComputeShader() noexcept = default;
};

VeSmartPointer(VeComputeShader);

class VENUS_API VeInputLayout : public VeRenderState
{
	VeNoCopy(VeInputLayout);
	VeRTTIDecl(VeInputLayout, VeRenderState);
public:
	struct ElementDesc
	{
		VeSematicName m_eName;
		uint32_t m_u32Index;
		VePixelFormat m_eFormat;
		uint32_t m_u32Slot;
		uint32_t m_u32Offset;
		VeInputClassification m_eClass;
		uint32_t m_u32Rate;

		ElementDesc(VeSematicName eName, uint32_t u32Index,
			VePixelFormat eFormat, uint32_t u32Slot, uint32_t u32Offset,
			VeInputClassification eClass, uint32_t u32Rate) noexcept
			: m_eName(eName), m_u32Index(u32Index), m_eFormat(eFormat)
			, m_u32Slot(u32Slot), m_u32Offset(u32Offset), m_eClass(eClass)
			, m_u32Rate(u32Rate) {}

		ElementDesc(std::initializer_list<std::tuple<VeSematicName, uint32_t, VePixelFormat,
			uint32_t, uint32_t, VeInputClassification, uint32_t>> l) noexcept
		{
			m_eName = std::get<0>(*l.begin());
			m_u32Index = std::get<1>(*l.begin());
			m_eFormat = std::get<2>(*l.begin());
			m_u32Slot = std::get<3>(*l.begin());
			m_u32Offset = std::get<4>(*l.begin());
			m_eClass = std::get<5>(*l.begin());
			m_u32Rate = std::get<6>(*l.begin());
		}

	};

	VeInputLayout() noexcept = default;

	virtual ~VeInputLayout() noexcept = default;


};

VeSmartPointer(VeInputLayout);
