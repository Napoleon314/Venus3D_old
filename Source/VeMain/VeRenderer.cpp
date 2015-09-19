////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderer.cpp
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderer);
//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderer::RootSignature);
//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderer::PipelineState);
//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderer::Binding);
//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderer::Geometry);
//--------------------------------------------------------------------------
VeRenderer::VeRenderer(API eType) noexcept
	: m_eType(eType)
{
	VE_ENUM(API,
	{
		{ API_NONE, "none" },
		{ API_D3D11, "d11" },
		{ API_D3D12, "d12" },
		{ API_OGL, "gl" },
		{ API_OGLES2, "gles2" },
		{ API_OGLES3, "gles3" },
		{ API_MASK, "all" }
	});

	VE_ENUM(ShaderType,
	{
		{ TYPE_VS, "vs" },
		{ TYPE_PS, "ps" },
		{ TYPE_GS, "gs" },
		{ TYPE_HS, "hs" },
		{ TYPE_DS, "ds" },
		{ TYPE_CS, "cs" }
	});

	VE_ENUM(BlendType,
	{
		{ REPLACE, "replace" },
		{ ADD, "add" },
		{ BLEND, "blend" }
	});

	VE_ENUM(RasterType,
	{
		{ CULL_BACK, "cull_back" },
		{ CULL_FRONT, "cull_front" },
		{ CULL_NONE, "cull_none" }
	});

	VE_ENUM(DepthStencilType,
	{
		{ DS_NONE, "none" },
		{ DS_STANDARD, "standard" }
	});

	VE_ENUM(PrimitiveTopologyType,
	{
		{ PRIMITIVE_TOPOLOGY_TYPE_POINT, "point" },
		{ PRIMITIVE_TOPOLOGY_TYPE_LINE, "line" },
		{ PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, "triangle" },
		{ PRIMITIVE_TOPOLOGY_TYPE_PATCH, "patch" }
	});

	VE_ENUM(FramePassType,
	{
		{ PASS_CLEAR, "clear" },
		{ PASS_QUAD, "quad" }
	});	

	VE_ENUM(FrameTargetType,
	{
		{ TARGET_NONE, "none" },
		{ TARGET_INPUT, "input" },
		{ TARGET_OUTPUT, "output" }
	});	

	VE_ENUM(ClearFlag,
	{
		{ CLEAR_COLOR, "c" },
		{ CLEAR_DEPTH, "d" },
		{ CLEAR_STENCIL, "s" },
		{ CLEAR_ALL, "all" }
	});

	VE_ENUM(VeRenderResource::Dimension,
	{
		{ VeRenderResource::DIMENSION_BUFFER, "buffer" },
		{ VeRenderResource::DIMENSION_TEXTURE1D, "tex1" },
		{ VeRenderResource::DIMENSION_TEXTURE2D, "tex2" },
		{ VeRenderResource::DIMENSION_TEXTURE3D, "tex3" }
	});

	VE_ENUM(VeRenderResource::Format,
	{
		{ VeRenderResource::FORMAT_R32G32B32A32_TYPELESS, "r32g32b32a32_typeless" },
		{ VeRenderResource::FORMAT_R32G32B32A32_FLOAT, "r32g32b32a32_float" },
		{ VeRenderResource::FORMAT_R32G32B32A32_UINT, "r32g32b32a32_uint" },
		{ VeRenderResource::FORMAT_R32G32B32A32_SINT, "r32g32b32a32_sint" },
		{ VeRenderResource::FORMAT_R32G32B32_TYPELESS, "r32g32b32_typeless" },
		{ VeRenderResource::FORMAT_R32G32B32_FLOAT, "r32g32b32_float" },
		{ VeRenderResource::FORMAT_R32G32B32_UINT, "r32g32b32_uint" },
		{ VeRenderResource::FORMAT_R32G32B32_SINT, "r32g32b32_sint" },
		{ VeRenderResource::FORMAT_R16G16B16A16_TYPELESS, "r16g16b16a16_typeless" },
		{ VeRenderResource::FORMAT_R16G16B16A16_FLOAT, "r16g16b16a16_float" },
		{ VeRenderResource::FORMAT_R16G16B16A16_UNORM, "r16g16b16a16_unorm" },
		{ VeRenderResource::FORMAT_R16G16B16A16_UINT, "r16g16b16a16_uint" },
		{ VeRenderResource::FORMAT_R16G16B16A16_SNORM, "r16g16b16a16_snorm" },
		{ VeRenderResource::FORMAT_R16G16B16A16_SINT, "r16g16b16a16_sint" },
		{ VeRenderResource::FORMAT_R32G32_TYPELESS, "r32g32_typeless" },
		{ VeRenderResource::FORMAT_R32G32_FLOAT, "r32g32_float" },
		{ VeRenderResource::FORMAT_R32G32_UINT, "r32g32_uint" },
		{ VeRenderResource::FORMAT_R32G32_SINT, "r32g32_sint" },
		{ VeRenderResource::FORMAT_R32G8X24_TYPELESS, "r32g8x24_typeless" },
		{ VeRenderResource::FORMAT_D32_FLOAT_S8X24_UINT, "d32_float_s8x24_uint" },
		{ VeRenderResource::FORMAT_R32_FLOAT_X8X24_TYPELESS, "r32_float_x8x24_typeless" },
		{ VeRenderResource::FORMAT_X32_TYPELESS_G8X24_UINT, "x32_typeless_g8x24_uint" },
		{ VeRenderResource::FORMAT_R10G10B10A2_TYPELESS, "r10g10b10a2_typeless" },
		{ VeRenderResource::FORMAT_R10G10B10A2_UNORM, "r10g10b10a2_unorm" },
		{ VeRenderResource::FORMAT_R10G10B10A2_UINT, "r10g10b10a2_uint" },
		{ VeRenderResource::FORMAT_R11G11B10_FLOAT, "r11g11b10_float" },
		{ VeRenderResource::FORMAT_R8G8B8A8_TYPELESS, "r8g8b8a8_typeless" },
		{ VeRenderResource::FORMAT_R8G8B8A8_UNORM, "r8g8b8a8_unorm" },
		{ VeRenderResource::FORMAT_R8G8B8A8_UNORM_SRGB, "r8g8b8a8_unorm_srgb" },
		{ VeRenderResource::FORMAT_R8G8B8A8_UINT, "r8g8b8a8_uint" },
		{ VeRenderResource::FORMAT_R8G8B8A8_SNORM, "r8g8b8a8_snorm" },
		{ VeRenderResource::FORMAT_R8G8B8A8_SINT, "r8g8b8a8_sint" },
		{ VeRenderResource::FORMAT_R16G16_TYPELESS, "r16g16_typeless" },
		{ VeRenderResource::FORMAT_R16G16_FLOAT, "r16g16_float" },
		{ VeRenderResource::FORMAT_R16G16_UNORM, "r16g16_unorm" },
		{ VeRenderResource::FORMAT_R16G16_UINT, "r16g16_uint" },
		{ VeRenderResource::FORMAT_R16G16_SNORM, "r16g16_snorm" },
		{ VeRenderResource::FORMAT_R16G16_SINT, "r16g16_sint" },
		{ VeRenderResource::FORMAT_R32_TYPELESS, "r32_typeless" },
		{ VeRenderResource::FORMAT_D32_FLOAT, "d32_float" },
		{ VeRenderResource::FORMAT_R32_FLOAT, "r32_float" },
		{ VeRenderResource::FORMAT_R32_UINT, "r32_uint" },
		{ VeRenderResource::FORMAT_R32_SINT, "r32_sint" },
		{ VeRenderResource::FORMAT_R24G8_TYPELESS, "r24g8_typeless" },
		{ VeRenderResource::FORMAT_D24_UNORM_S8_UINT, "d24_unorm_s8_uint" },
		{ VeRenderResource::FORMAT_R24_UNORM_X8_TYPELESS, "r24_unorm_x8_typeless" },
		{ VeRenderResource::FORMAT_X24_TYPELESS_G8_UINT, "x24_typeless_g8_uint" },
		{ VeRenderResource::FORMAT_R8G8_TYPELESS, "r8g8_typeless" },
		{ VeRenderResource::FORMAT_R8G8_UNORM, "r8g8_unorm" },
		{ VeRenderResource::FORMAT_R8G8_UINT, "r8g8_uint" },
		{ VeRenderResource::FORMAT_R8G8_SNORM, "r8g8_snorm" },
		{ VeRenderResource::FORMAT_R8G8_SINT, "r8g8_sint" },
		{ VeRenderResource::FORMAT_R16_TYPELESS, "r16_typeless" },
		{ VeRenderResource::FORMAT_R16_FLOAT, "r16_float" },
		{ VeRenderResource::FORMAT_D16_UNORM, "d16_unorm" },
		{ VeRenderResource::FORMAT_R16_UNORM, "r16_unorm" },
		{ VeRenderResource::FORMAT_R16_UINT, "r16_uint" },
		{ VeRenderResource::FORMAT_R16_SNORM, "r16_snorm" },
		{ VeRenderResource::FORMAT_R16_SINT, "r16_sint" },
		{ VeRenderResource::FORMAT_R8_TYPELESS, "r8_typeless" },
		{ VeRenderResource::FORMAT_R8_UNORM, "r8_unorm" },
		{ VeRenderResource::FORMAT_R8_UINT, "r8_uint" },
		{ VeRenderResource::FORMAT_R8_SNORM, "r8_snorm" },
		{ VeRenderResource::FORMAT_R8_SINT, "r8_sint" },
		{ VeRenderResource::FORMAT_A8_UNORM, "a8_unorm" },
		{ VeRenderResource::FORMAT_R1_UNORM, "r1_unorm" },
		{ VeRenderResource::FORMAT_R9G9B9E5_SHAREDEXP, "r9g9b9e5_sharedexp" },
		{ VeRenderResource::FORMAT_R8G8_B8G8_UNORM, "r8g8_b8g8_unorm" },
		{ VeRenderResource::FORMAT_G8R8_G8B8_UNORM, "g8r8_g8b8_unorm" },
		{ VeRenderResource::FORMAT_BC1_TYPELESS, "bc1_typeless" },
		{ VeRenderResource::FORMAT_BC1_UNORM, "bc1_unorm" },
		{ VeRenderResource::FORMAT_BC1_UNORM_SRGB, "bc1_unorm_srgb" },
		{ VeRenderResource::FORMAT_BC2_TYPELESS, "bc2_typeless" },
		{ VeRenderResource::FORMAT_BC2_UNORM, "bc2_unorm" },
		{ VeRenderResource::FORMAT_BC2_UNORM_SRGB, "bc2_unorm_srgb" },
		{ VeRenderResource::FORMAT_BC3_TYPELESS, "bc3_typeless" },
		{ VeRenderResource::FORMAT_BC3_UNORM, "bc3_unorm" },
		{ VeRenderResource::FORMAT_BC3_UNORM_SRGB, "bc3_unorm_srgb" },
		{ VeRenderResource::FORMAT_BC4_TYPELESS, "bc4_typeless" },
		{ VeRenderResource::FORMAT_BC4_UNORM, "bc4_unorm" },
		{ VeRenderResource::FORMAT_BC4_SNORM, "bc4_snorm" },
		{ VeRenderResource::FORMAT_BC5_TYPELESS, "bc5_typeless" },
		{ VeRenderResource::FORMAT_BC5_UNORM, "bc5_unorm" },
		{ VeRenderResource::FORMAT_BC5_SNORM, "bc5_snorm" },
		{ VeRenderResource::FORMAT_B5G6R5_UNORM, "b5g6r5_unorm" },
		{ VeRenderResource::FORMAT_B5G5R5A1_UNORM, "b5g5r5a1_unorm" },
		{ VeRenderResource::FORMAT_B8G8R8A8_UNORM, "b8g8r8a8_unorm" },
		{ VeRenderResource::FORMAT_B8G8R8X8_UNORM, "b8g8r8x8_unorm" },
		{ VeRenderResource::FORMAT_R10G10B10_XR_BIAS_A2_UNORM, "r10g10b10_xr_bias_a2_unorm" },
		{ VeRenderResource::FORMAT_B8G8R8A8_TYPELESS, "b8g8r8a8_typeless" },
		{ VeRenderResource::FORMAT_B8G8R8A8_UNORM_SRGB, "b8g8r8a8_unorm_srgb" },
		{ VeRenderResource::FORMAT_B8G8R8X8_TYPELESS, "b8g8r8x8_typeless" },
		{ VeRenderResource::FORMAT_B8G8R8X8_UNORM_SRGB, "b8g8r8x8_unorm_srgb" },
		{ VeRenderResource::FORMAT_BC6H_TYPELESS, "bc6h_typeless" },
		{ VeRenderResource::FORMAT_BC6H_UF16, "bc6h_uf16" },
		{ VeRenderResource::FORMAT_BC6H_SF16, "bc6h_sf16" },
		{ VeRenderResource::FORMAT_BC7_TYPELESS, "bc7_typeless" },
		{ VeRenderResource::FORMAT_BC7_UNORM, "bc7_unorm" },
		{ VeRenderResource::FORMAT_BC7_UNORM_SRGB, "bc7_unorm_srgb" },
		{ VeRenderResource::FORMAT_AYUV, "ayuv" },
		{ VeRenderResource::FORMAT_Y410, "y410" },
		{ VeRenderResource::FORMAT_Y416, "y416" },
		{ VeRenderResource::FORMAT_NV12, "nv12" },
		{ VeRenderResource::FORMAT_P010, "p010" },
		{ VeRenderResource::FORMAT_P016, "p016" },
		{ VeRenderResource::FORMAT_420_OPAQUE, "420_opaque" },
		{ VeRenderResource::FORMAT_YUY2, "yuy2" },
		{ VeRenderResource::FORMAT_Y210, "y210" },
		{ VeRenderResource::FORMAT_Y216, "y216" },
		{ VeRenderResource::FORMAT_NV11, "nv11" },
		{ VeRenderResource::FORMAT_AI44, "ai44" },
		{ VeRenderResource::FORMAT_IA44, "ia44" },
		{ VeRenderResource::FORMAT_P8, "p8" },
		{ VeRenderResource::FORMAT_A8P8, "a8p8" },
		{ VeRenderResource::FORMAT_B4G4R4A4_UNORM, "b4g4r4a4_unorm" },
		{ VeRenderResource::FORMAT_P208, "p208" },
		{ VeRenderResource::FORMAT_V208, "v208" },
		{ VeRenderResource::FORMAT_V408, "v408" }
	});
}
//--------------------------------------------------------------------------
VeRenderer::~VeRenderer() noexcept
{

}
//--------------------------------------------------------------------------
void VeRenderer::RegistResTypes() noexcept
{
	VeTexture::Regist();
}
//--------------------------------------------------------------------------
void VeRenderer::UnregistResTypes() noexcept
{
	VeTexture::Unregist();
}
//--------------------------------------------------------------------------
static void AddScriptTo(VeVector<VeJSONDoc>& kList,
	VeJSONValue& kVal) noexcept
{
	if (kVal.IsArray())
	{
		for (auto it = kVal.Begin(); it != kVal.End(); ++it)
		{
			if (it->IsArray())
			{
				kList.resize(kList.size() + 1);
				kList.back().CopyFrom(*it, kList.back().GetAllocator());
			}
		}
	}
}
//--------------------------------------------------------------------------
static void AddScriptTo(VeVector<VeJSONValue*>& kList,
	VeJSONValue& kVal) noexcept
{
	if (kVal.IsArray())
	{
		for (auto it = kVal.Begin(); it != kVal.End(); ++it)
		{
			if (it->IsArray())
			{
				kList.push_back(it);
			}
		}
	}
}
//--------------------------------------------------------------------------
void VeRenderer::PreCache(const VeDirectoryPtr& spCache,
	const VeDirectoryPtr& spShader, const VeDirectoryPtr& spScript) noexcept
{
	VeVector<VeFixedString> kShaderNameList;
	spShader->FindFileList("*", kShaderNameList);
	VeStringMap<VeUInt32> kShaderNameMap;
	for (VeUInt32 i(0); i < kShaderNameList.size(); ++i)
	{
		kShaderNameMap[kShaderNameList[i]] = i;
	}
	VeVector<VeBlobPtr> kShaderList;
	kShaderList.resize(kShaderNameList.size());
	std::atomic_uint u32Flag;
	u32Flag.store(0, std::memory_order_relaxed);
	ve_paral.Do([&](VeUInt32 u32Thread) noexcept
	{
		while (true)
		{
			VeUInt32 u32Index = u32Flag.fetch_add(1, std::memory_order_relaxed);
			if (u32Index >= kShaderNameList.size()) break;
			VeBinaryIStreamPtr spStream = spShader->OpenIStream(kShaderNameList[u32Index]);
			VE_ASSERT(spStream);
			VeSizeT stLen = spStream->RemainingLength();
			kShaderList[u32Index] = VE_NEW VeBlob(stLen);
			spStream->Read(kShaderList[u32Index]->GetBuffer(), stLen);
		}
	});
	VeVector<VeFixedString> kScriptNameList;
	spScript->FindFileList("*.json", kScriptNameList);
	VeVector<VeJSONDoc> kScriptList;
	kScriptList.resize(kScriptNameList.size());
	u32Flag.store(0, std::memory_order_release);
	ve_paral.Do([&](VeUInt32 u32Thread) noexcept
	{
		while (true)
		{
			VeUInt32 u32Index = u32Flag.fetch_add(1, std::memory_order_relaxed);
			if (u32Index >= kScriptNameList.size()) break;
			VeBinaryIStreamPtr spStream = spScript->OpenIStream(kScriptNameList[u32Index]);
			VE_ASSERT(spStream);
			VeSizeT stLen = spStream->RemainingLength();
			VeBlobPtr spBuffer = VE_NEW VeBlob(stLen);
			spStream->Read(spBuffer->GetBuffer(), stLen);
			kScriptList[u32Index].Parse<0>((const VeChar8*)spBuffer->GetBuffer());
		}
	});
	VeVector<VeJSONDoc> kShaderScript;
	VeVector<VeJSONDoc> kRootSignatureScript;
	VeVector<VeJSONDoc> kPipelineStateScript;
	VeVector<VeJSONDoc> kCompositorScript;
	for (auto& doc : kScriptList)
	{
		for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
		{
			if (!VeStrcmp(it->name.GetString(), "shader"))
			{
				AddScriptTo(kShaderScript, it->value);
			}
			else if (!VeStrcmp(it->name.GetString(), "root_signature"))
			{
				AddScriptTo(kRootSignatureScript, it->value);
			}
			else if (!VeStrcmp(it->name.GetString(), "pipeline_state"))
			{
				AddScriptTo(kPipelineStateScript, it->value);
			}
			else if (!VeStrcmp(it->name.GetString(), "compositor"))
			{
				AddScriptTo(kCompositorScript, it->value);
			}
		}
	}
	VeSpinLock kLock;
	u32Flag.store(0, std::memory_order_relaxed);
	ve_paral.Do([&](VeUInt32 u32Thread) noexcept
	{
		while (true)
		{
			VeUInt32 u32Index = u32Flag.fetch_add(1, std::memory_order_relaxed);
			if (u32Index >= kShaderScript.size()) break;
			VeJSONValue& lkValue = kShaderScript[u32Index];
			VeJSONValue* pkValue = nullptr;
			const VeChar8* pcName = nullptr;
			const VeChar8* pcFile = nullptr;
			const VeChar8* pcTarget = nullptr;
			for (auto it = lkValue.Begin(); it != lkValue.End(); ++it)
			{
				if (it->IsObject())
				{					
					auto p = it->FindMember("platform");
					if (p != it->MemberEnd())
					{
						if (p->value.IsString())
						{
							API eAPI = ve_parser.ParseFlags(p->value.GetString(), API_NONE);
							if (!VE_MASK_HAS_ALL(eAPI, GetAPI()))
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
					pcName = (*it)("name", "");
					pcFile = (*it)("file", "");
					pcTarget = (*it)("target", "");					
					if (*pcName && *pcFile && *pcTarget) pkValue = &(*it);
					break;
				}
			}
			if (pkValue)
			{
				auto kRes = CompileShader(pcFile, pcTarget, spShader->GetName(),
					*pkValue, kShaderNameMap, kShaderList);
				{
					if (kRes.first)
					{
						VE_LOCK_MUTEX(kLock);
						m_akShaderMap[kRes.second][pcName] = kRes.first;
					}					
				}				
			}
		}
	});
	u32Flag.store(0, std::memory_order_relaxed);
	ve_paral.Do([&](VeUInt32 u32Thread) noexcept
	{
		while (true)
		{
			VeUInt32 u32Index = u32Flag.fetch_add(1, std::memory_order_relaxed);
			if (u32Index >= kRootSignatureScript.size()) break;
			VeJSONValue& lkValue = kRootSignatureScript[u32Index];
			VeJSONValue* pkValue = nullptr;
			const VeChar8* pcName = nullptr;
			for (auto it = lkValue.Begin(); it != lkValue.End(); ++it)
			{
				if (it->IsObject())
				{
					auto p = it->FindMember("platform");
					if (p != it->MemberEnd())
					{
						if (p->value.IsString())
						{
							VeRenderer::API eAPI = ve_parser.ParseFlags(
								p->value.GetString(), VeRenderer::API_NONE);
							if (!VE_MASK_HAS_ALL(eAPI, ve_renderer_ptr->GetAPI()))
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
					pcName = (*it)("name", "");
					if (*pcName) pkValue = &(*it);
					break;
				}
			}
			if (pkValue)
			{
				VeBlobPtr spBlob = SerializeRootSignature(*pkValue);
				if (spBlob)
				{
					RootSignaturePtr spRoot = CreateRootSignature(spBlob);
					{
						VE_LOCK_MUTEX(kLock);
						m_kRootSignatureMap[pcName] = spRoot;
					}					
				}
			}
		}
	});	
	u32Flag.store(0, std::memory_order_relaxed);
	ve_paral.Do([&](VeUInt32 u32Thread) noexcept
	{
		while (true)
		{
			VeUInt32 u32Index = u32Flag.fetch_add(1, std::memory_order_relaxed);
			if (u32Index >= kPipelineStateScript.size()) break;
			VeJSONValue& lkValue = kPipelineStateScript[u32Index];
			VeJSONValue* pkValue = nullptr;
			const VeChar8* pcName = nullptr;
			for (auto it = lkValue.Begin(); it != lkValue.End(); ++it)
			{
				if (it->IsObject())
				{
					auto p = it->FindMember("platform");
					if (p != it->MemberEnd())
					{
						if (p->value.IsString())
						{
							VeRenderer::API eAPI = ve_parser.ParseFlags(
								p->value.GetString(), VeRenderer::API_NONE);
							if (!VE_MASK_HAS_ALL(eAPI, ve_renderer_ptr->GetAPI()))
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
					pcName = (*it)("name", "");
					if (*pcName) pkValue = &(*it);
					break;
				}
			}
			if (pkValue)
			{
				VeBlobPtr spOut;
				PipelineStatePtr spState = CreatePipelineState(*pkValue, spOut);
				{
					VE_LOCK_MUTEX(kLock);
					m_kPipelineStateMap[pcName] = spState;
				}
			}
		}
	});
	u32Flag.store(0, std::memory_order_relaxed);
	ve_paral.Do([&](VeUInt32 u32Thread) noexcept
	{
		while (true)
		{
			VeUInt32 u32Index = u32Flag.fetch_add(1, std::memory_order_relaxed);
			if (u32Index >= kCompositorScript.size()) break;
			VeJSONValue& lkValue = kCompositorScript[u32Index];
			VeJSONValue* pkValue = nullptr;
			const VeChar8* pcName = nullptr;
			for (auto it = lkValue.Begin(); it != lkValue.End(); ++it)
			{
				if (it->IsObject())
				{
					auto p = it->FindMember("platform");
					if (p != it->MemberEnd())
					{
						if (p->value.IsString())
						{
							VeRenderer::API eAPI = ve_parser.ParseFlags(
								p->value.GetString(), VeRenderer::API_NONE);
							if (!VE_MASK_HAS_ALL(eAPI, ve_renderer_ptr->GetAPI()))
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
					pcName = (*it)("name", "");
					if (*pcName) pkValue = &(*it);
					break;
				}
			}
			if (pkValue)
			{
				FrameCompositorPtr spCompositor = CreateCompositor(*pkValue);
				if (spCompositor)
				{
					VE_LOCK_MUTEX(kLock);
					m_kCompositorMap[pcName] = spCompositor;
				}
			}
		}
	});
}
//--------------------------------------------------------------------------
VeRenderer::FrameCompositorPtr VeRenderer::CreateCompositor(
	VeJSONValue& kValue) noexcept
{
	auto tech = kValue.FindMember("technique");
	if (tech != kValue.MemberEnd())
	{
		FrameCompositorPtr spCompositor = VE_NEW FrameCompositor();
		for (auto it = tech->value.Begin(); it != tech->value.End(); ++it)
		{
			spCompositor->m_kTechniqueList.resize(
				spCompositor->m_kTechniqueList.size() + 1);
			SetTechnique(spCompositor->m_kTechniqueList.back(), *it);
		}
		return spCompositor;
	}
	return nullptr;
}
//--------------------------------------------------------------------------
void VeRenderer::SetTechnique(FrameTechnique& kTechnique,
	VeJSONValue& kValue) noexcept
{
	kTechnique.m_kName = kValue("name", "");
	auto res = kValue.FindMember("resource");
	if (res != kValue.MemberEnd())
	{
		if (res->value.IsArray())
		{
			for (auto it = res->value.Begin(); it != res->value.End(); ++it)
			{
				kTechnique.m_kResourceList.resize(
					kTechnique.m_kResourceList.size() + 1);
				SetResource(kTechnique.m_kResourceList.back(), *it);
			}
		}
		else if(res->value.IsObject())
		{
			kTechnique.m_kResourceList.resize(
				kTechnique.m_kResourceList.size() + 1);
			SetResource(kTechnique.m_kResourceList.back(), res->value);
		}		
	}
	auto tar = kValue.FindMember("target");
	if (tar != kValue.MemberEnd())
	{
		for (auto it = tar->value.Begin(); it != tar->value.End(); ++it)
		{
			kTechnique.m_kTargetList.resize(
				kTechnique.m_kTargetList.size() + 1);
			SetTarget(kTechnique.m_kTargetList.back(), *it);
		}
	}
	auto cli = kValue.FindMember("click");
	if (cli != kValue.MemberEnd())
	{
		for (auto it = cli->value.Begin(); it != cli->value.End(); ++it)
		{
			kTechnique.m_kClickList.resize(
				kTechnique.m_kClickList.size() + 1);
			SetClick(kTechnique.m_kClickList.back(), *it);
		}
	}
}
//--------------------------------------------------------------------------
void VeRenderer::SetResource(FrameResource& kRes,
	VeJSONValue& kValue) noexcept
{
	if (kValue.IsObject())
	{
		kRes.m_kName = kValue("name", "");
		kRes.m_eDimension = kValue("dimension", VeRenderResource::DIMENSION_TEXTURE2D);
		kRes.m_eFormat = kValue("format", VeRenderResource::FORMAT_UNKNOWN);
		kRes.m_u32Width = kValue("w", 0u);
		kRes.m_u32Height = kValue("h", 0u);
		kRes.m_u16Depth = kValue("d", 1u);
		kRes.m_u16MipLevels = kValue("m", 1u);
		kRes.m_u16Count = kValue("c", 1u);
		kRes.m_u16Quality = kValue("q", 0u);
	}
}
//--------------------------------------------------------------------------
void VeRenderer::SetTarget(FrameTarget& kTarget,
	VeJSONValue& kValue) noexcept
{
	kTarget.m_kName = kValue("name", "");
	kTarget.m_eType = kValue("type", TARGET_NONE);
	auto rtv = kValue.FindMember("rtv");
	if (rtv != kValue.MemberEnd())
	{
		if (rtv->value.IsArray())
		{
			for (auto it = rtv->value.Begin(); it != rtv->value.End(); ++it)
			{
				kTarget.m_kRTVList.resize(kTarget.m_kRTVList.size() + 1);
				SetRTV(kTarget.m_kRTVList.back(), kValue);
			}
		}
		else if(rtv->value.IsObject())
		{
			kTarget.m_kRTVList.resize(kTarget.m_kRTVList.size() + 1);
			SetRTV(kTarget.m_kRTVList.back(), kValue);
		}
	}
	auto dsv = kValue.FindMember("dsv");
	if (dsv != kValue.MemberEnd())
	{
		SetDSV(kTarget.m_kDSV, dsv->value);
	}
}
//--------------------------------------------------------------------------
void VeRenderer::SetRTV(FrameRTV& kRTV, VeJSONValue& kValue) noexcept
{
	kRTV.m_kResName = kValue("resource", "");
	kRTV.m_u32Param0 = kValue("p0", 0u);
	kRTV.m_u32Param1 = kValue("p1", 0u);
	kRTV.m_u32Param2 = kValue("p2", 0u);
	kRTV.m_u32Param3 = kValue("p3", 0u);
}
//--------------------------------------------------------------------------
void VeRenderer::SetDSV(FrameDSV& kDSV, VeJSONValue& kValue) noexcept
{
	kDSV.m_kResName = kValue("resource", "");
	kDSV.m_u32Param0 = kValue("p0", 0u);
	kDSV.m_u32Param1 = kValue("p1", 0u);
	kDSV.m_u32Param2 = kValue("p2", 0u);
	kDSV.m_u32Param3 = kValue("p3", 0u);
}
//--------------------------------------------------------------------------
void VeRenderer::SetClick(FrameClick& kClick,
	VeJSONValue& kValue) noexcept
{
	kClick.m_kTarget = kValue("target", "");
	auto pass = kValue.FindMember("pass");
	if (pass != kValue.MemberEnd())
	{
		for (auto it = pass->value.Begin(); it != pass->value.End(); ++it)
		{
			FramePassPtr spPass = CreatePass(*it);
			if (spPass)
			{
				kClick.m_kPassList.push_back(spPass);
			}			
		}
	}
}
//--------------------------------------------------------------------------
VeRenderer::FramePassPtr VeRenderer::CreatePass(
	VeJSONValue& kValue) noexcept
{
	FramePassType eType = kValue("type", PASS_MAX);
	switch (eType)
	{
	case VeRenderer::PASS_CLEAR:
		return CreateClear(kValue);
	case VeRenderer::PASS_QUAD:
		return CreateQuad(kValue);
	default:
		break;
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeRenderer::FramePassPtr VeRenderer::CreateClear(
	VeJSONValue& kValue) noexcept
{
	FrameClear* pkClear = VE_NEW FrameClear();
	auto flag = kValue.FindMember("flag");
	if (flag != kValue.MemberEnd() && flag->value.IsString())
	{
		pkClear->m_u32Flags |= ve_parser.ParseFlags(
			flag->value.GetString(), VE_TMIN(ClearFlag));
	}
	auto color = kValue.FindMember("color");
	if (color != kValue.MemberEnd())
	{
		if (color->value.IsArray())
		{
			for (auto it = color->value.Begin(); it != color->value.End(); ++it)
			{
				pkClear->m_kColorArray.push_back(VeTo(*it, VeColor::BLACK));
			}			
		}
		else if (color->value.IsString())
		{
			pkClear->m_kColorArray.push_back(VeTo(color->value, VeColor::BLACK));
		}
	}
	pkClear->m_f32Depth = kValue("depth", 0.0f);
	pkClear->m_u8Stencil = (VeUInt8)kValue("stencil", 0u);
	return pkClear;
}
//--------------------------------------------------------------------------
VeRenderer::FramePassPtr VeRenderer::CreateQuad(
	VeJSONValue& kValue) noexcept
{
	FrameQuad* pkQuad = VE_NEW FrameQuad();
	pkQuad->m_kRootSignature = kValue("root_signature", "");
	pkQuad->m_kPipelineState = kValue("pipeline_state", "");
	return pkQuad;
}
//--------------------------------------------------------------------------
VeRenderWindowPtr VeRenderer::CreateRenderWindow(const VeChar8* pcTitle,
	VeInt32 x, VeInt32 y, VeInt32 w, VeInt32 h, VeUInt32 u32Flags) noexcept
{
	VeWindowPtr spWindow = VE_NEW VeWindow();
	spWindow->Create(pcTitle, x, y, w, h, u32Flags);
	VeRenderWindowPtr spRes = CreateRenderWindow(spWindow);
	spWindow = nullptr;
	spRes->m_bNeedDestory = true;
	return spRes;
}
//--------------------------------------------------------------------------
