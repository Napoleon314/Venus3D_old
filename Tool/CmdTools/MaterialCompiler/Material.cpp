////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      MaterialCompiler
//  File name:   Material.cpp
//  Created:     2016/08/11 by Albert
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

#include "Material.h"

//--------------------------------------------------------------------------
Material::Material(const char* pcName) noexcept
	: m_kName(pcName)
{

}
//--------------------------------------------------------------------------
Material::~Material() noexcept
{

}
//--------------------------------------------------------------------------
bool Material::Init(const char* pcPath) noexcept
{
	VeDirectoryPtr spPath = ve_res_mgr.OpenDirectory(pcPath);
	if (!spPath)
	{
		VeUserLogE("Input directory does not exist.");
		return false;
	}
	m_kPath = pcPath;
	m_kSourceArray.clear();
	auto it = spPath->FindFirst("*.xml");
	if (it)
	{
		do
		{
			AddConfig(spPath, it->data());
		} while (it->next());
	}
	return true;
}
//--------------------------------------------------------------------------
void Material::AddConfig(const VeDirectoryPtr& spDir,
	VeDirectory::ChildData& data) noexcept
{
	auto arch = spDir->OpenArchive(data.m_pcName);
	if (arch)
	{
		m_kSourceArray.resize(m_kSourceArray.size() + 1);
		auto& src = m_kSourceArray.back();
		src.first = VE_NEW VeBlob(data.m_stSize);
		VE_ASSERT_ALWAYS(arch->Read(src.first->data(), data.m_stSize) == data.m_stSize);
		src.second.parse((char*)src.first->data());
	}
}
//--------------------------------------------------------------------------
bool Material::Compile() noexcept
{
	if (!CompileShaders()) return false;
	if (!CompileRootSignatures()) return false;
	return true;
}
//--------------------------------------------------------------------------
bool Material::CompileShaders() noexcept
{
	for (auto& src : m_kSourceArray)
	{
		{
			auto node = src.second.first_node()->first_node("vertex_shaders");
			if (node)
			{
				auto it = node->first_node();
				while (it)
				{
					if (!CompileShader(VERTEX_SHADER, it)) return false;
					it = it->next_sibling();
				}
			}
		}
		{
			auto node = src.second.first_node()->first_node("pixel_shaders");
			if (node)
			{
				auto it = node->first_node();
				while (it)
				{
					if (!CompileShader(PIXEL_SHADER, it)) return false;
					it = it->next_sibling();
				}
			}
		}
		{
			auto node = src.second.first_node()->first_node("geometry_shaders");
			if (node)
			{
				auto it = node->first_node();
				while (it)
				{
					if (!CompileShader(GEOMETRY_SHADER, it)) return false;
					it = it->next_sibling();
				}
			}
		}
		{
			auto node = src.second.first_node()->first_node("hull_shaders");
			if (node)
			{
				auto it = node->first_node();
				while (it)
				{
					if (!CompileShader(HULL_SHADER, it)) return false;
					it = it->next_sibling();
				}
			}
		}
		{
			auto node = src.second.first_node()->first_node("domain_shaders");
			if (node)
			{
				auto it = node->first_node();
				while (it)
				{
					if (!CompileShader(DOMAIN_SHADER, it)) return false;
					it = it->next_sibling();
				}
			}
		}
		{
			auto node = src.second.first_node()->first_node("compute_shaders");
			if (node)
			{
				auto it = node->first_node();
				while (it)
				{
					if (!CompileShader(COMPUTE_SHADER, it)) return false;
					it = it->next_sibling();
				}
			}
		}
	}
	return true;
}
//--------------------------------------------------------------------------
bool Material::CompileShader(ShaderType eType,
	venus::xml_node* pkNode) noexcept
{
	const char* pcName = pkNode->name();
	const char* pcTarget = nullptr;
	switch (eType)
	{
	case Material::VERTEX_SHADER:
		pcTarget = "vs_5_0";
		break;
	case Material::PIXEL_SHADER:
		pcTarget = "ps_5_0";
		break;
	case Material::GEOMETRY_SHADER:
		pcTarget = "gs_5_0";
		break;
	case Material::HULL_SHADER:
		pcTarget = "hs_5_0";
		break;
	case Material::DOMAIN_SHADER:
		pcTarget = "ds_5_0";
		break;
	case Material::COMPUTE_SHADER:
		pcTarget = "cs_5_0";
		break;
	default:
		break;
	}
	if (!pcTarget)
	{
		VeUserLogE("Unknown target when compiling shader: ", pcName);
	}

	const char* pcFile = nullptr;
	const char* pcEntry = nullptr;
	vtd::vector<D3D_SHADER_MACRO> kMacros;

	auto it = pkNode->first_node();
	if (it && (!vtd::strcmp(it->name(), "file")))
	{
		pcFile = it->value();
		it = it->next_sibling();
		if (it && (!vtd::strcmp(it->name(), "entry")))
		{
			pcEntry = it->value();
			it = it->next_sibling();
			if (it && (!vtd::strcmp(it->name(), "macros")))
			{
				auto macro = it->first_node();
				while (macro)
				{
					kMacros.push_back({ macro->name(), macro->value() });
					macro = macro->next_sibling();
				}
			}
		}
	}
	
	if (!pcFile)
	{
		VeUserLogE("File missing when compiling shader: ", pcName);
	}
	if (!pcEntry)
	{
		VeUserLogE("Entry missing when compiling shader: ", pcName);
	}

	char acBuffer[VE_MAX_PATH_LEN];
	VeSprintf(acBuffer, "%s\\%s", m_kPath.c_str(), pcFile);

	int32_t i32Num = MultiByteToWideChar(CP_UTF8, 0, acBuffer, -1, nullptr, 0);
	VE_ASSERT(i32Num >= 0);
	VeDyanmicStack<WCHAR> kFile(i32Num + 1);
	kFile[i32Num] = 0;
	MultiByteToWideChar(CP_UTF8, 0, acBuffer, -1, kFile.data(), i32Num);

	if (kMacros.size())
	{
		kMacros.push_back({ nullptr, nullptr });
	}
	
	ID3DBlob* pkBlob = nullptr;
	if (FAILED(D3DCompileFromFile(kFile.data(), kMacros.size() ? kMacros.data() : nullptr,
		nullptr, pcEntry, pcTarget,
		0, 0,
		&pkBlob, nullptr)))
	{
		VeUserLogE("Compiling shader: ", pcName, " failed.");
		return false;
	}
	auto& blob = m_akShaderCodes[eType][pcName];
	blob = VE_NEW VeBlob(pkBlob->GetBufferSize());
	memcpy(blob->data(), pkBlob->GetBufferPointer(), pkBlob->GetBufferSize());
	VE_SAFE_RELEASE(pkBlob);
	return true;
}
//--------------------------------------------------------------------------
bool Material::CompileRootSignatures() noexcept
{
	for (auto& src : m_kSourceArray)
	{
		auto node = src.second.first_node()->first_node("root_signatures");
		if (node)
		{
			auto it = node->first_node();
			while (it)
			{
				if (!CompileRootSignature(it)) return false;
				it = it->next_sibling();
			}
		}
	}
	return true;
}
//--------------------------------------------------------------------------
bool Material::CompileRootSignature(venus::xml_node* pkNode) noexcept
{
	const char* pcName = pkNode->name();
	VeVector<CD3D12_ROOT_PARAMETER> kParameters;
	{
		auto para = pkNode->first_node("parameters");
		if (para)
		{
			auto it = para->first_node();
			while (it)
			{
				kParameters.resize(kParameters.size() + 1);
				CD3D12_ROOT_PARAMETER& obj = kParameters.back();
				obj.ParameterType = venus::convert_str(it->name(),
					D3D12_ROOT_PARAMETER_TYPE(UINT32_MAX));
				obj.ShaderVisibility = ((venus::xml_node*)it)->convert_attribute(
					"shader_visibility", D3D12_SHADER_VISIBILITY_ALL);
				switch (obj.ParameterType)
				{
				case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
				{
					obj.DescriptorTable = { 0, nullptr };
					auto itRange = it->first_node("range");
					while (itRange)
					{
						D3D12_DESCRIPTOR_RANGE kRange;
						kRange.RangeType = ((venus::xml_node*)itRange)->convert_attribute(
							"type", D3D12_DESCRIPTOR_RANGE_TYPE_SRV);
						kRange.NumDescriptors = ((venus::xml_node*)itRange)->convert_attribute(
							"num", 1);
						kRange.BaseShaderRegister = ((venus::xml_node*)itRange)->convert_attribute(
							"base", 0);
						kRange.RegisterSpace = ((venus::xml_node*)itRange)->convert_attribute(
							"space", 0);
						kRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
						obj.AddDescriptorRange(kRange);
						itRange = itRange->next_sibling("range");
					}
					break;
				}
				case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
					obj.Constants.ShaderRegister = ((venus::xml_node*)it)->convert_attribute(
						"base", 0);
					obj.Constants.RegisterSpace = ((venus::xml_node*)it)->convert_attribute(
						"space", 0);
					obj.Constants.Num32BitValues = ((venus::xml_node*)it)->convert_attribute(
						"num", 1);
					break;
				case D3D12_ROOT_PARAMETER_TYPE_CBV:
					obj.Descriptor.ShaderRegister = ((venus::xml_node*)it)->convert_attribute(
						"base", 0);
					obj.Descriptor.RegisterSpace = ((venus::xml_node*)it)->convert_attribute(
						"space", 0);
					break;
				case D3D12_ROOT_PARAMETER_TYPE_SRV:
					obj.Descriptor.ShaderRegister = ((venus::xml_node*)it)->convert_attribute(
						"base", 0);
					obj.Descriptor.RegisterSpace = ((venus::xml_node*)it)->convert_attribute(
						"space", 0);
					break;
				case D3D12_ROOT_PARAMETER_TYPE_UAV:
					obj.Descriptor.ShaderRegister = ((venus::xml_node*)it)->convert_attribute(
						"base", 0);
					obj.Descriptor.RegisterSpace = ((venus::xml_node*)it)->convert_attribute(
						"space", 0);
					break;
				default:
					break;
				}


				it = it->next_sibling();
			}
		}
	}
	VeVector<D3D12_STATIC_SAMPLER_DESC> kSamplers;
	{
		auto para = pkNode->first_node("samplers");
		if (para)
		{
			auto it = para->first_node();
			while (it)
			{
				kSamplers.resize(kSamplers.size() + 1);
				D3D12_STATIC_SAMPLER_DESC& obj = kSamplers.back();
				obj.Filter = ((venus::xml_node*)it)->convert_attribute(
					"filter", D3D12_FILTER_MIN_MAG_MIP_POINT);
				obj.AddressU = ((venus::xml_node*)it)->convert_attribute(
					"add_u", D3D12_TEXTURE_ADDRESS_MODE_WRAP);
				obj.AddressV = ((venus::xml_node*)it)->convert_attribute(
					"add_v", D3D12_TEXTURE_ADDRESS_MODE_WRAP);
				obj.AddressW = ((venus::xml_node*)it)->convert_attribute(
					"add_w", D3D12_TEXTURE_ADDRESS_MODE_WRAP);
				obj.MipLODBias = ((venus::xml_node*)it)->convert_attribute(
					"lod_bias", 0.0f);
				obj.MaxAnisotropy = ((venus::xml_node*)it)->convert_attribute(
					"max_anis", 1);
				obj.ComparisonFunc = ((venus::xml_node*)it)->convert_attribute(
					"compare", D3D12_COMPARISON_FUNC_ALWAYS);
				obj.BorderColor = ((venus::xml_node*)it)->convert_attribute(
					"border", D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK);
				obj.MinLOD = ((venus::xml_node*)it)->convert_attribute(
					"min_lod", 0.0f);
				obj.MaxLOD = ((venus::xml_node*)it)->convert_attribute(
					"max_lod", D3D12_FLOAT32_MAX);
				obj.ShaderRegister = ((venus::xml_node*)it)->convert_attribute(
					"base", 0);
				obj.RegisterSpace = ((venus::xml_node*)it)->convert_attribute(
					"space", 0);
				obj.ShaderVisibility = ((venus::xml_node*)it)->convert_attribute(
					"shader_visibility", D3D12_SHADER_VISIBILITY_PIXEL);

				it = it->next_sibling();
			}
		}
	}
	D3D12_ROOT_SIGNATURE_DESC kDesc;

	kDesc.NumParameters = (UINT)kParameters.size();
	kDesc.pParameters = kParameters.size() ? kParameters.data() : nullptr;
	kDesc.NumStaticSamplers = (UINT)kSamplers.size();
	kDesc.pStaticSamplers = kSamplers.size() ? kSamplers.data() : nullptr;
	kDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
	if (pkNode->convert_attribute("input_layout", false))
	{
		kDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}
	if (pkNode->convert_attribute("stream_out", false))
	{
		kDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;
	}
	if (pkNode->convert_attribute("deny_vs", false))
	{
		kDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
	}
	if (pkNode->convert_attribute("deny_ps", false))
	{
		kDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
	}
	if (pkNode->convert_attribute("deny_gs", false))
	{
		kDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	}
	if (pkNode->convert_attribute("deny_hs", false))
	{
		kDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
	}
	if (pkNode->convert_attribute("deny_ds", false))
	{
		kDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
	}
	ID3DBlob* pkBlob = nullptr;
	ID3DBlob* pkErr = nullptr;
	if (SUCCEEDED(D3D12SerializeRootSignature(&kDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&pkBlob, &pkErr)))
	{
		auto& blob = m_kRootSignature[pcName];
		blob = VE_NEW VeBlob(pkBlob->GetBufferSize());
		memcpy(blob->data(), pkBlob->GetBufferPointer(), pkBlob->GetBufferSize());
	}
	VE_SAFE_RELEASE(pkBlob);
	VE_SAFE_RELEASE(pkErr);

	return true;
}
//--------------------------------------------------------------------------
