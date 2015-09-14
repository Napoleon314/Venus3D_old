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
		{ DS_NONE, "cull_back" },
		{ DS_STANDARD, "cull_none" }
	});
}
//--------------------------------------------------------------------------
VeRenderer::~VeRenderer() noexcept
{

}
//--------------------------------------------------------------------------
void VeRenderer::RegistResTypes() noexcept
{
	
}
//--------------------------------------------------------------------------
void VeRenderer::UnregistResTypes() noexcept
{
	
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
	u32Flag.store(0, std::memory_order_release);
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
		}
	}
	struct ShaderCode
	{
		VeBlobPtr m_spCode;
		ShaderType m_eType;
		const VeChar8* m_pcName;
	};
	VeVector<ShaderCode> kShaderCodeList;
	kShaderCodeList.resize(kShaderScript.size());
	u32Flag.store(0, std::memory_order_release);
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
				kShaderCodeList[u32Index] = { kRes.first, kRes.second, pcName };
			}
		}
	});
	for (auto& code : kShaderCodeList)
	{
		if (code.m_spCode && code.m_eType < SHADER_TYPE_NUM && code.m_pcName)
		{
			m_akShaderMap[code.m_eType][code.m_pcName] = code.m_spCode;
		}
	}
	struct RootSignatureCode
	{
		VeBlobPtr m_spCode;
		RootSignaturePtr m_spObj;
		const VeChar8* m_pcName;
	};
	VeVector<RootSignatureCode> kRootSignatureList;
	kRootSignatureList.resize(kRootSignatureScript.size());
	u32Flag.store(0, std::memory_order_release);
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
					kRootSignatureList[u32Index] = { spBlob ,CreateRootSignature(spBlob), pcName };
				}
			}
		}
	});
	for (auto& root : kRootSignatureList)
	{
		m_kRootSignatureMap[root.m_pcName] = root.m_spObj;
	}
	struct PipelineStateCode
	{
		VeBlobPtr m_spCode;
		PipelineStatePtr m_spObj;
		const VeChar8* m_pcName;
	};
	VeVector<PipelineStateCode> kPipelineStateList;
	kPipelineStateList.resize(kPipelineStateScript.size());
	u32Flag.store(0, std::memory_order_release);
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
				kPipelineStateList[u32Index].m_spObj = CreatePipelineState(
					*pkValue, kPipelineStateList[u32Index].m_spCode);
				kPipelineStateList[u32Index].m_pcName = pcName;
			}
		}
	});
	for (auto& state : kPipelineStateList)
	{
		m_kPipelineStateMap[state.m_pcName] = state.m_spObj;
	}
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
