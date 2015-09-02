////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRendererD3D12.cpp
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "../VeMainPch.h"
#include "VeRendererD3D12.h"
#include "VeRendererWindowD3D12.h"

//--------------------------------------------------------------------------
#ifdef VE_ENABLE_D3D12
//--------------------------------------------------------------------------
VeRTTIImpl(VeRendererD3D12, VeRenderer);
//--------------------------------------------------------------------------
VeRTTIImpl(VeRendererD3D12::RootSignatureD3D12, VeRenderer::RootSignature);
//--------------------------------------------------------------------------
VeRTTIImpl(VeRendererD3D12::PipelineStateD3D12, VeRenderer::PipelineState);
//--------------------------------------------------------------------------
VeRendererD3D12::VeRendererD3D12() noexcept
	: VeRenderer(API_D3D12)
{
	
}
//--------------------------------------------------------------------------
VeRendererD3D12::~VeRendererD3D12() noexcept
{
	VE_ASSERT((!m_pkDXGIFactory) && (!m_pkDefaultAdapter) && (!m_pkDevice));
}
//--------------------------------------------------------------------------
bool VeRendererD3D12::Init() noexcept
{
	InitParsers();
	m_spD3D12 = VE_NEW VeSharedLib(VE_SHARED_LIB_NAME("d3d12"));
	if (!m_spD3D12->Load()) return false;
	m_spDXGI = VE_NEW VeSharedLib(VE_SHARED_LIB_NAME("dxgi"));
	if (!m_spDXGI->Load()) return false;
	m_spD3DCompiler = VE_NEW VeSharedLib(VE_SHARED_LIB_NAME(D3D_COMPLIER));
	if (!m_spD3DCompiler->Load()) return false;

	D3D12GetDebugInterface = (decltype(D3D12GetDebugInterface))
		m_spD3D12->GetProc("D3D12GetDebugInterface");
	if (!D3D12GetDebugInterface) return false;

	D3D12CreateDevice = (decltype(D3D12CreateDevice))
		m_spD3D12->GetProc("D3D12CreateDevice");
	if (!D3D12CreateDevice) return false;

	D3D12SerializeRootSignature = (decltype(D3D12SerializeRootSignature))
		m_spD3D12->GetProc("D3D12SerializeRootSignature");
	if (!D3D12SerializeRootSignature) return false;

	CreateDXGIFactory1 = (decltype(CreateDXGIFactory1))
		m_spDXGI->GetProc("CreateDXGIFactory1");
	if (!CreateDXGIFactory1) return false;

	D3DCompile = (decltype(D3DCompile))
		m_spD3DCompiler->GetProc("D3DCompile");
	if (!D3DCompile) return false;

#	ifdef VE_DEBUG
	{
		ID3D12Debug* pkDebugController(nullptr);
		if (SUCCEEDED(D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&pkDebugController)))
		{
			pkDebugController->EnableDebugLayer();
		}
		VE_SAFE_RELEASE(pkDebugController);
	}
#	endif

	if (VE_FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&m_pkDXGIFactory)))
		return false;
	
	if(m_pkDXGIFactory->EnumAdapters1(0, &m_pkDefaultAdapter) == DXGI_ERROR_NOT_FOUND)
		return false;	

	if (VE_FAILED(D3D12CreateDevice(m_pkDefaultAdapter,
		D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&m_pkDevice)))
		return false;

	m_kRTVHeap.Init(m_pkDevice);
	m_kDSVHeap.Init(m_pkDevice);	

	ve_sys.CORE.I.LogFormat("VeRendererD3D12 renderer is created.");
	return true;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::Term() noexcept
{
	for (auto win : m_kRenderWindowList)
	{
		win->Term();
	}
	for (auto obj : m_kRootSignatureList)
	{
		VE_SAFE_RELEASE(obj->m_pkRootSignature);
	}
	for (auto obj : m_kPipelineStateList)
	{
		VE_SAFE_RELEASE(obj->m_pkPipelineState);
	}
	VE_ASSERT(m_kRenderWindowList.empty());
	m_kDSVHeap.Term();
	m_kRTVHeap.Term();
	VE_SAFE_RELEASE(m_pkDevice);
	VE_SAFE_RELEASE(m_pkDefaultAdapter);
	VE_SAFE_RELEASE(m_pkDXGIFactory);
	D3D12GetDebugInterface = nullptr;
	D3D12CreateDevice = nullptr;
	CreateDXGIFactory1 = nullptr;
	m_spD3D12 = nullptr;
	m_spDXGI = nullptr;
	m_spD3DCompiler = nullptr;
	TermParsers();
}
//--------------------------------------------------------------------------
VeRenderWindowPtr VeRendererD3D12::CreateRenderWindow(
	const VeWindowPtr& spWindow) noexcept
{
	VeRenderWindowD3D12* pkRenderWindow = VE_NEW VeRenderWindowD3D12(spWindow);
	VE_ASSERT(pkRenderWindow);
	pkRenderWindow->Init(*this);
	return pkRenderWindow;
}
//--------------------------------------------------------------------------
bool VeRendererD3D12::IsSupported(const VeChar8* pcPlatform) noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	VeStrcpy(acBuffer, pcPlatform);
	VeChar8* pcContext;
	VeChar8* pcTemp = VeStrtok(acBuffer, "|", &pcContext);
	while (pcTemp)
	{
		if (VeStricmp(pcTemp, "d12") == 0)
		{
			return true;
		}
		pcTemp = VeStrtok<VeChar8>(nullptr, "|", &pcContext);
	}
	return false;
}
//--------------------------------------------------------------------------
VeShader::Type VeRendererD3D12::GetShaderType(
	const VeChar8* pcTarget) noexcept
{
	switch (VeToLower(*pcTarget))
	{
	case 'v':
		return VeShader::TYPE_VS;
	case 'p':
		return VeShader::TYPE_PS;
	case 'g':
		return VeShader::TYPE_GS;
	case 'h':
		return VeShader::TYPE_HS;
	case 'd':
		return VeShader::TYPE_DS;
	case 'c':
		return VeShader::TYPE_CS;
	default:
		return VeShader::TYPE_NUM;
	}
}
//--------------------------------------------------------------------------
VeBlobPtr VeRendererD3D12::CompileShader(
	const VeChar8* pcName, const VeChar8* pcTarget,
	const VeChar8* pcConfigPath, VeJSONValue& kConfig,
	const VeResourceManager::FileCachePtr& spCache) noexcept
{
	auto e = kConfig.FindMember("entry");
	if (e == kConfig.MemberEnd() || (!e->value.IsString()))
	{
		ve_sys.CORE.E.LogFormat("Shader script error in [\"%s\"].", pcConfigPath);
		return nullptr;
	}
	const VeChar8* pcEntry = e->value.GetString();
	const VeBlobPtr& spHLSL = spCache->GetData()->GetBlob();
#	ifdef VE_DEBUG
	VeUInt32 u32CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#	else
	VeUInt32 u32CompileFlags = 0;
#	endif
	VeBlobPtr spBlob;
	ID3DBlob* pkCode = nullptr;
	ID3DBlob* pkError = nullptr;
	if (SUCCEEDED(D3DCompile(spHLSL->GetBuffer(), spHLSL->GetSize(), spCache->GetFullPath(),
		nullptr, nullptr, pcEntry, pcTarget, u32CompileFlags, 0, &pkCode, &pkError)))
	{
		spBlob = VE_NEW VeBlob(pkCode->GetBufferSize());
		VeMemoryCopy(spBlob->GetBuffer(), pkCode->GetBufferPointer(), pkCode->GetBufferSize());
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeSprintf(acBuffer, "%s.%so", pcName, VeShader::GetTypeName(GetShaderType(pcTarget)));
		spCache->GetGroup()->GetWriteDir()->WriteAsync(acBuffer, VE_NEW VeMemoryOStream(spBlob));
		VeCoreDebugOutput("[\"%s\"===>\"%s\"] is successful.", spCache->GetFileName(), acBuffer);
	}
	else
	{
		if (pkError)
		{
			ve_sys.CORE.E.LogFormat((const VeChar8*)pkError->GetBufferPointer());
		}
		spBlob = nullptr;
	}
	VE_SAFE_RELEASE(pkCode);
	VE_SAFE_RELEASE(pkError);
	return spBlob;
}
//--------------------------------------------------------------------------
VeBlobPtr VeRendererD3D12::SerializeRootSignature(const VeChar8* pcName,
	VeJSONValue& kConfig, const VeResourceGroupPtr& spGroup) noexcept
{
	struct Temp
	{
		~Temp() noexcept
		{
			if (m_pkParameter && m_u32ParametersNum)
			{
				for (VeUInt32 i(m_u32ParametersNum - 1); i < m_u32ParametersNum; --i)
				{
					D3D12_ROOT_PARAMETER& kPara = m_pkParameter[i];
					if (kPara.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
					{
						VE_ASSERT(kPara.DescriptorTable.pDescriptorRanges);
						VeStackFree(kPara.DescriptorTable.pDescriptorRanges);
					}
				}
				VeStackFree(m_pkParameter);
				m_pkParameter = nullptr;
			}

			if (m_pkSampler && m_u32SamplersNum)
			{
				VeStackFree(m_pkSampler);
				m_pkSampler = nullptr;
			}
		}

		void SetParameterNum(VeUInt32 u32Num) noexcept
		{
			VE_ASSERT(m_u32ParametersNum == 0 && m_pkParameter == nullptr);
			m_u32ParametersNum = u32Num;
			m_pkParameter = VeStackAlloc(D3D12_ROOT_PARAMETER, u32Num);
			VeZeroMemory(m_pkParameter, sizeof(D3D12_ROOT_PARAMETER) * u32Num);
		}

		D3D12_DESCRIPTOR_RANGE* SetRangeNum(VeUInt32 u32Index, VeUInt32 u32Num) noexcept
		{
			VE_ASSERT(u32Index < m_u32ParametersNum && (m_pkParameter[u32Index].ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE));
			m_pkParameter[u32Index].DescriptorTable.NumDescriptorRanges = u32Num;
			D3D12_DESCRIPTOR_RANGE* pvRange = VeStackAlloc(D3D12_DESCRIPTOR_RANGE, u32Num);
			VeZeroMemory(pvRange, sizeof(D3D12_DESCRIPTOR_RANGE) * u32Num);
			m_pkParameter[u32Index].DescriptorTable.pDescriptorRanges = pvRange;
			return pvRange;
		}

		void SetSamplerNum(VeUInt32 u32Num) noexcept
		{
			VE_ASSERT(m_u32SamplersNum == 0 && m_pkSampler == nullptr);
			m_u32SamplersNum = u32Num;
			m_pkSampler = VeStackAlloc(D3D12_STATIC_SAMPLER_DESC, u32Num);
			VeZeroMemory(m_pkSampler, sizeof(D3D12_STATIC_SAMPLER_DESC) * u32Num);
		}
		
		VeUInt32 m_u32ParametersNum = 0;
		D3D12_ROOT_PARAMETER* m_pkParameter = nullptr;
		VeUInt32 m_u32SamplersNum = 0;
		D3D12_STATIC_SAMPLER_DESC* m_pkSampler = nullptr;

	} kStackBuffer;
	auto iParameters = kConfig.FindMember("parameters");
	if (iParameters != kConfig.MemberEnd()
		&& iParameters->value.IsArray()
		&& iParameters->value.Size())
	{
		kStackBuffer.SetParameterNum(iParameters->value.Size());
		for (VeUInt32 i(0); i < kStackBuffer.m_u32ParametersNum; ++i)
		{
			VeJSONValue& kValue = iParameters->value[i];
			if (!kValue.IsObject()) return nullptr;
			D3D12_ROOT_PARAMETER& kParameter = kStackBuffer.m_pkParameter[i];
			kParameter.ParameterType = VeToEnum(kValue, "type", m_kRootParameterTypeParser, D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE);
			switch (kParameter.ParameterType)
			{
			case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
			{
				auto itRange = kValue.FindMember("ranges");
				if (itRange != kValue.MemberEnd()
					&& itRange->value.IsArray()
					&& itRange->value.Size())
				{
					D3D12_DESCRIPTOR_RANGE* pvRange = kStackBuffer.SetRangeNum(i, itRange->value.Size());
					for (VeUInt32 i(0); i < itRange->value.Size(); ++i)
					{
						D3D12_DESCRIPTOR_RANGE& kRange = pvRange[i];
						VeJSONValue& kValueRange = itRange->value[i];
						if (!kValueRange.IsObject()) return nullptr;

						kRange.RangeType = VeToEnum(kValueRange, "type", m_kDescriptorRangeTypeParser, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);
						kRange.NumDescriptors = VeToNumber(kValueRange, "num", 0u);
						kRange.BaseShaderRegister = VeToNumber(kValueRange, "register", 0u);
						kRange.RegisterSpace = VeToNumber(kValueRange, "space", 0u);
						kRange.OffsetInDescriptorsFromTableStart = VeToNumber(kValueRange, "offset", 0u);
					}
				}
				kParameter.ShaderVisibility = VeToEnum(kValue, "visibility", m_kShaderVisibilityParser, D3D12_SHADER_VISIBILITY_ALL);
			}
			break;
			case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
			{
				kParameter.Constants.Num32BitValues = VeToNumber(kValue, "num", 0u);
				kParameter.Constants.ShaderRegister = VeToNumber(kValue, "register", 0u);
				kParameter.Constants.RegisterSpace = VeToNumber(kValue, "space", 0u);
			}
			break;
			case D3D12_ROOT_PARAMETER_TYPE_CBV:
			case D3D12_ROOT_PARAMETER_TYPE_SRV:
			case D3D12_ROOT_PARAMETER_TYPE_UAV:
			{
				kParameter.Descriptor.ShaderRegister = VeToNumber(kValue, "register", 0u);
				kParameter.Descriptor.RegisterSpace = VeToNumber(kValue, "space", 0u);
			}
			break;
			default:
				return nullptr;
			}
		}
	}	
	auto iSamplers = kConfig.FindMember("samplers");
	if (iSamplers != kConfig.MemberEnd()
		&& iSamplers->value.IsArray()
		&& iSamplers->value.Size())
	{
		kStackBuffer.SetSamplerNum(iSamplers->value.Size());
		for (VeUInt32 i(0); i < kStackBuffer.m_u32SamplersNum; ++i)
		{
			VeJSONValue& kValue = iSamplers->value[i];
			if (!kValue.IsObject()) return nullptr;
			D3D12_STATIC_SAMPLER_DESC& kSampler = kStackBuffer.m_pkSampler[i];
			kSampler.Filter = VeToEnum(kValue, "filter", m_kFilterParser, D3D12_FILTER_MIN_MAG_MIP_LINEAR);
			kSampler.AddressU = VeToEnum(kValue, "addU", m_kTexAddressModeParser, D3D12_TEXTURE_ADDRESS_MODE_WRAP);
			kSampler.AddressV = VeToEnum(kValue, "addV", m_kTexAddressModeParser, D3D12_TEXTURE_ADDRESS_MODE_WRAP);
			kSampler.AddressW = VeToEnum(kValue, "addW", m_kTexAddressModeParser, D3D12_TEXTURE_ADDRESS_MODE_WRAP);
			kSampler.MipLODBias = VeToNumber(kValue, "lod_bias", 0.0f);
			kSampler.MaxAnisotropy = VeToNumber(kValue, "max_ani", 0u);
			kSampler.ComparisonFunc = VeToEnum(kValue, "comp_func", m_kComparisonFuncParser, D3D12_COMPARISON_FUNC_ALWAYS);
			kSampler.BorderColor = VeToEnum(kValue, "border_color", m_kStaticBorderColorParser, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK);
			kSampler.MinLOD = VeToNumber(kValue, "min_lod", 0.0f);
			kSampler.MaxLOD = VeToNumber(kValue, "max_lod", D3D12_FLOAT32_MAX);
			kSampler.ShaderRegister = VeToNumber(kValue, "register", 0u);
			kSampler.RegisterSpace = VeToNumber(kValue, "space", 0u);
			kSampler.ShaderVisibility = VeToEnum(kValue, "visibility", m_kShaderVisibilityParser, D3D12_SHADER_VISIBILITY_ALL);
		}
	}	
	
	D3D12_ROOT_SIGNATURE_DESC kDesc;
	kDesc.NumParameters = kStackBuffer.m_u32ParametersNum;
	kDesc.pParameters = kStackBuffer.m_pkParameter;
	kDesc.NumStaticSamplers = kStackBuffer.m_u32SamplersNum;
	kDesc.pStaticSamplers = kStackBuffer.m_pkSampler;
	kDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	auto itFlags = kConfig.FindMember("flags");
	if (itFlags == kConfig.MemberEnd()) return nullptr;
	if (!itFlags->value.IsString()) return nullptr;
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	VeStrcpy(acBuffer, itFlags->value.GetString());
	VeChar8* pcContext;
	VeChar8* pcTemp = VeStrtok(acBuffer, "|", &pcContext);
	while (pcTemp)
	{
		auto itFlag = m_kRootSignatureFlagsParser.find(pcTemp);
		if (itFlag != m_kRootSignatureFlagsParser.end())
		{
			kDesc.Flags |= itFlag->second;
		}
		pcTemp = VeStrtok<VeChar8>(nullptr, "|", &pcContext);
	}
	
	VeBlobPtr spBlob;
	ID3DBlob* pkCode = nullptr;
	ID3DBlob* pkError = nullptr;
	if (SUCCEEDED(D3D12SerializeRootSignature(&kDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pkCode, &pkError)))
	{
		spBlob = VE_NEW VeBlob(pkCode->GetBufferSize());
		VeMemoryCopy(spBlob->GetBuffer(), pkCode->GetBufferPointer(), pkCode->GetBufferSize());
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeSprintf(acBuffer, "%s.rso", pcName);
		spGroup->GetWriteDir()->WriteAsync(acBuffer, VE_NEW VeMemoryOStream(spBlob));
	}
	else
	{
		if (pkError)
		{
			ve_sys.CORE.E.LogFormat((const VeChar8*)pkError->GetBufferPointer());
		}
		spBlob = nullptr;
	}
	VE_SAFE_RELEASE(pkCode);
	VE_SAFE_RELEASE(pkError);
	return spBlob;
}
//--------------------------------------------------------------------------
VeRendererD3D12::RootSignaturePtr VeRendererD3D12::CreateRootSignature(
	const VeBlobPtr& spBlob) noexcept
{
	ID3D12RootSignature* pkRootSig(nullptr);
	if (SUCCEEDED(m_pkDevice->CreateRootSignature(0, spBlob->GetBuffer(),
		spBlob->GetSize(), __uuidof(ID3D12RootSignature), (void**)&pkRootSig)))
	{
		RootSignatureD3D12* pkObj = VE_NEW RootSignatureD3D12(pkRootSig);
		m_kRootSignatureList.attach_back(pkObj->m_kNode);
		return pkObj;
	}
	else
	{
		VE_SAFE_RELEASE(pkRootSig);
		return nullptr;
	}
}
//--------------------------------------------------------------------------
VeRenderer::PipelineStatePtr VeRendererD3D12::CreatePipelineState(
	VeJSONValue& kConfig) noexcept
{
	bool bGraphics = VeToBoolean(kConfig, "graphics", true);
	return bGraphics ? CreateGraphicsPipelineState(kConfig)
		: CreateComputePipelineState(kConfig);
}
//--------------------------------------------------------------------------
void FillBlendState(D3D12_BLEND_DESC& kDesc,
	VeRendererD3D12::BlendType eType) noexcept
{
	kDesc.AlphaToCoverageEnable = FALSE;
	kDesc.IndependentBlendEnable = FALSE;
	switch (eType)
	{
	case VeRendererD3D12::ADD:
		kDesc.RenderTarget[0] =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		break;
	case VeRendererD3D12::BLEND:
		kDesc.RenderTarget[0] =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		break;
	default:
		kDesc.RenderTarget[0] =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		break;
	}
}
//--------------------------------------------------------------------------
void FillRasterizerState(D3D12_RASTERIZER_DESC& kDesc,
	VeRendererD3D12::RasterType eType) noexcept
{
	switch (eType)
	{
	case VeRendererD3D12::CULL_FRONT:
		kDesc.FillMode = D3D12_FILL_MODE_SOLID;
		kDesc.CullMode = D3D12_CULL_MODE_FRONT;
		kDesc.FrontCounterClockwise = FALSE;
		kDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		kDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		kDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		kDesc.DepthClipEnable = TRUE;
		kDesc.MultisampleEnable = FALSE;
		kDesc.AntialiasedLineEnable = FALSE;
		kDesc.ForcedSampleCount = 0;
		kDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		break;
	case VeRendererD3D12::CULL_NONE:
		kDesc.FillMode = D3D12_FILL_MODE_SOLID;
		kDesc.CullMode = D3D12_CULL_MODE_NONE;
		kDesc.FrontCounterClockwise = FALSE;
		kDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		kDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		kDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		kDesc.DepthClipEnable = TRUE;
		kDesc.MultisampleEnable = FALSE;
		kDesc.AntialiasedLineEnable = FALSE;
		kDesc.ForcedSampleCount = 0;
		kDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		break;
	default:
		kDesc.FillMode = D3D12_FILL_MODE_SOLID;
		kDesc.CullMode = D3D12_CULL_MODE_BACK;
		kDesc.FrontCounterClockwise = FALSE;
		kDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		kDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		kDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		kDesc.DepthClipEnable = TRUE;
		kDesc.MultisampleEnable = FALSE;
		kDesc.AntialiasedLineEnable = FALSE;
		kDesc.ForcedSampleCount = 0;
		kDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		break;
	}
}
//--------------------------------------------------------------------------
void FillDepthStencilState(D3D12_DEPTH_STENCIL_DESC& kDesc,
	VeRendererD3D12::DepthStencilType eType) noexcept
{
	switch (eType)
	{
	case VeRendererD3D12::DS_STANDARD:
		kDesc.DepthEnable = TRUE;
		kDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		kDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		kDesc.StencilEnable = FALSE;
		break;
	default:
		kDesc.DepthEnable = FALSE;
		kDesc.StencilEnable = FALSE;
		break;
	}
}
//--------------------------------------------------------------------------
VeRenderer::PipelineStatePtr VeRendererD3D12::CreateGraphicsPipelineState(
	VeJSONValue& kConfig) noexcept
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC kDesc = {};
	{
		auto itName = kConfig.FindMember(VeRootSignature::GetName());
		if (itName != kConfig.MemberEnd() && itName->value.IsString())
		{
			VeRootSignaturePtr spRes = ve_res_mgr.Get<VeRootSignature>(VeRootSignature::GetName(), itName->value.GetString(), false);
			if ((!spRes) || (!spRes->GetObject())) return nullptr;
			RootSignatureD3D12* pkRootSig = VeDynamicCast(RootSignatureD3D12, &*spRes->GetObject());
			if ((!pkRootSig) || (!pkRootSig->m_pkRootSignature)) return nullptr;
			kDesc.pRootSignature = pkRootSig->m_pkRootSignature;
		}
	}
	{
		auto itName = kConfig.FindMember(VeShader::GetTypeName(VeShader::TYPE_VS));
		if (itName != kConfig.MemberEnd() && itName->value.IsString())
		{
			VeShaderPtr spRes = ve_res_mgr.Get<VeShader>(VeShader::GetTypeName(VeShader::TYPE_VS), itName->value.GetString(), false);
			if ((!spRes) || (!spRes->GetObject())) return nullptr;
			VeBlobPtr spBlob = spRes->GetObject();
			kDesc.VS.pShaderBytecode = spBlob->GetBuffer();
			kDesc.VS.BytecodeLength = spBlob->GetSize();
		}
	}
	{
		auto itName = kConfig.FindMember(VeShader::GetTypeName(VeShader::TYPE_PS));
		if (itName != kConfig.MemberEnd() && itName->value.IsString())
		{
			VeShaderPtr spRes = ve_res_mgr.Get<VeShader>(VeShader::GetTypeName(VeShader::TYPE_PS), itName->value.GetString(), false);
			if ((!spRes) || (!spRes->GetObject())) return nullptr;
			VeBlobPtr spBlob = spRes->GetObject();
			kDesc.PS.pShaderBytecode = spBlob->GetBuffer();
			kDesc.PS.BytecodeLength = spBlob->GetSize();
		}
	}
	{
		auto itName = kConfig.FindMember(VeShader::GetTypeName(VeShader::TYPE_DS));
		if (itName != kConfig.MemberEnd() && itName->value.IsString())
		{
			VeShaderPtr spRes = ve_res_mgr.Get<VeShader>(VeShader::GetTypeName(VeShader::TYPE_DS), itName->value.GetString(), false);
			if ((!spRes) || (!spRes->GetObject())) return nullptr;
			VeBlobPtr spBlob = spRes->GetObject();
			kDesc.DS.pShaderBytecode = spBlob->GetBuffer();
			kDesc.DS.BytecodeLength = spBlob->GetSize();
		}
	}
	{
		auto itName = kConfig.FindMember(VeShader::GetTypeName(VeShader::TYPE_HS));
		if (itName != kConfig.MemberEnd() && itName->value.IsString())
		{
			VeShaderPtr spRes = ve_res_mgr.Get<VeShader>(VeShader::GetTypeName(VeShader::TYPE_HS), itName->value.GetString(), false);
			if ((!spRes) || (!spRes->GetObject())) return nullptr;
			VeBlobPtr spBlob = spRes->GetObject();
			kDesc.HS.pShaderBytecode = spBlob->GetBuffer();
			kDesc.HS.BytecodeLength = spBlob->GetSize();
		}
	}
	{
		auto itName = kConfig.FindMember(VeShader::GetTypeName(VeShader::TYPE_GS));
		if (itName != kConfig.MemberEnd() && itName->value.IsString())
		{
			VeShaderPtr spRes = ve_res_mgr.Get<VeShader>(VeShader::GetTypeName(VeShader::TYPE_GS), itName->value.GetString(), false);
			if ((!spRes) || (!spRes->GetObject())) return nullptr;
			VeBlobPtr spBlob = spRes->GetObject();
			kDesc.GS.pShaderBytecode = spBlob->GetBuffer();
			kDesc.GS.BytecodeLength = spBlob->GetSize();
		}
	}
	{
		bool bNeed = true;
		auto itBlend = kConfig.FindMember("blend_state");
		if (itBlend != kConfig.MemberEnd())
		{
			if (itBlend->value.IsString())
			{
				auto itBlendEnum = m_kBlendTypeParser.find(itBlend->value.GetString());
				if (itBlendEnum != m_kBlendTypeParser.end())
				{
					FillBlendState(kDesc.BlendState, itBlendEnum->second);
					bNeed = false;
				}				
			}
			else if (itBlend->value.IsObject())
			{
				bNeed = false;
				VeJSONValue& kBlend = itBlend->value;
				kDesc.BlendState.AlphaToCoverageEnable = VeToBool(kBlend, "alpha_to_coverage");
				kDesc.BlendState.IndependentBlendEnable = VeToBool(kBlend, "independent");				
				auto itRTBlend = kBlend.FindMember("blend");
				if (itRTBlend != kBlend.MemberEnd())
				{
					VeUInt32 u32Count(0);
					if (itRTBlend->value.IsObject())
					{						
						kDesc.BlendState.RenderTarget[u32Count].BlendEnable = VeToBool(itRTBlend->value, "blend");
						kDesc.BlendState.RenderTarget[u32Count].LogicOpEnable = VeToBool(itRTBlend->value, "logic_op");
						kDesc.BlendState.RenderTarget[u32Count].SrcBlend = VeToEnum(itRTBlend->value, "src", m_kBlendParser, D3D12_BLEND_ZERO);
						kDesc.BlendState.RenderTarget[u32Count].DestBlend = VeToEnum(itRTBlend->value, "dst", m_kBlendParser, D3D12_BLEND_ZERO);
						kDesc.BlendState.RenderTarget[u32Count].BlendOp = VeToEnum(itRTBlend->value, "op", m_kBlendOpParser, D3D12_BLEND_OP_ADD);
						kDesc.BlendState.RenderTarget[u32Count].SrcBlendAlpha = VeToEnum(itRTBlend->value, "src_alpha", m_kBlendParser, D3D12_BLEND_ZERO);
						kDesc.BlendState.RenderTarget[u32Count].DestBlendAlpha = VeToEnum(itRTBlend->value, "dst_alpha", m_kBlendParser, D3D12_BLEND_ZERO);
						kDesc.BlendState.RenderTarget[u32Count].BlendOpAlpha = VeToEnum(itRTBlend->value, "op_alpha", m_kBlendOpParser, D3D12_BLEND_OP_ADD);
						kDesc.BlendState.RenderTarget[u32Count].LogicOp = VeToEnum(itRTBlend->value, "logic_op", m_kLogicOpParser, D3D12_LOGIC_OP_CLEAR);
						auto itMask = itRTBlend->value.FindMember("mask");
						if (itMask != itRTBlend->value.MemberEnd()
							&& itMask->value.IsString())
						{
							VeChar8 acBuffer[VE_MAX_PATH_LEN];
							VeStrcpy(acBuffer, itMask->value.GetString());
							VeChar8* pcContext;
							VeChar8* pcTemp = VeStrtok(acBuffer, "|", &pcContext);
							while (pcTemp)
							{
								auto itFlag = m_kColorWriteParser.find(pcTemp);
								if (itFlag != m_kColorWriteParser.end())
								{
									kDesc.BlendState.RenderTarget[u32Count].RenderTargetWriteMask |= itFlag->second;
								}
								pcTemp = VeStrtok<VeChar8>(nullptr, "|", &pcContext);
							}
						}
						else
						{
							kDesc.BlendState.RenderTarget[u32Count].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
						}
						++u32Count;
					}
					else if(itRTBlend->value.IsArray())
					{
						for (auto itRT = itRTBlend->value.Begin(); itRT != itRTBlend->value.End(); ++itRT)
						{
							if (itRT->IsObject())
							{
								
								kDesc.BlendState.RenderTarget[u32Count].BlendEnable = VeToBool(itRTBlend->value, "blend");
								kDesc.BlendState.RenderTarget[u32Count].LogicOpEnable = VeToBool(itRTBlend->value, "logic_op");
								kDesc.BlendState.RenderTarget[u32Count].SrcBlend = VeToEnum(itRTBlend->value, "src", m_kBlendParser, D3D12_BLEND_ZERO);
								kDesc.BlendState.RenderTarget[u32Count].DestBlend = VeToEnum(itRTBlend->value, "dst", m_kBlendParser, D3D12_BLEND_ZERO);
								kDesc.BlendState.RenderTarget[u32Count].BlendOp = VeToEnum(itRTBlend->value, "op", m_kBlendOpParser, D3D12_BLEND_OP_ADD);
								kDesc.BlendState.RenderTarget[u32Count].SrcBlendAlpha = VeToEnum(itRTBlend->value, "src_alpha", m_kBlendParser, D3D12_BLEND_ZERO);
								kDesc.BlendState.RenderTarget[u32Count].DestBlendAlpha = VeToEnum(itRTBlend->value, "dst_alpha", m_kBlendParser, D3D12_BLEND_ZERO);
								kDesc.BlendState.RenderTarget[u32Count].BlendOpAlpha = VeToEnum(itRTBlend->value, "op_alpha", m_kBlendOpParser, D3D12_BLEND_OP_ADD);
								kDesc.BlendState.RenderTarget[u32Count].LogicOp = VeToEnum(itRTBlend->value, "logic_op", m_kLogicOpParser, D3D12_LOGIC_OP_CLEAR);
								auto itMask = itRTBlend->value.FindMember("mask");
								if (itMask != itRTBlend->value.MemberEnd()
									&& itMask->value.IsString())
								{
									VeChar8 acBuffer[VE_MAX_PATH_LEN];
									VeStrcpy(acBuffer, itMask->value.GetString());
									VeChar8* pcContext;
									VeChar8* pcTemp = VeStrtok(acBuffer, "|", &pcContext);
									while (pcTemp)
									{
										auto itFlag = m_kColorWriteParser.find(pcTemp);
										if (itFlag != m_kColorWriteParser.end())
										{
											kDesc.BlendState.RenderTarget[u32Count].RenderTargetWriteMask |= itFlag->second;
										}
										pcTemp = VeStrtok<VeChar8>(nullptr, "|", &pcContext);
									}
								}
								else
								{
									kDesc.BlendState.RenderTarget[u32Count].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
								}
								if((++u32Count) >= 8) break;
							}								
						}
					}
					if (u32Count)
					{
						for (VeUInt32 i(u32Count); i < 8; ++i)
						{
							kDesc.BlendState.RenderTarget[i] = kDesc.BlendState.RenderTarget[i - 1];
						}						
					}
				}
			}
		}
		if (bNeed)
		{
			FillBlendState(kDesc.BlendState, REPLACE);
		}
		kDesc.SampleMask = VeToNumber(kConfig, "sample_mask", 0u);
	}
	{
		bool bNeed = true;
		auto itRaster = kConfig.FindMember("rasterizer_state");
		if (itRaster != kConfig.MemberEnd())
		{
			if (itRaster->value.IsString())
			{
				auto itRasterEnum = m_kRasterTypeParser.find(itRaster->value.GetString());
				if (itRasterEnum != m_kRasterTypeParser.end())
				{
					FillRasterizerState(kDesc.RasterizerState, itRasterEnum->second);
					bNeed = false;
				}
			}
			else if (itRaster->value.IsObject())
			{
				bNeed = false;
				kDesc.RasterizerState.FillMode = VeToEnum(itRaster->value, "fill_mode", m_kFillModeParser, D3D12_FILL_MODE_SOLID);
				kDesc.RasterizerState.CullMode = VeToEnum(itRaster->value, "cull_mode", m_kCullModeParser, D3D12_CULL_MODE_BACK);
				kDesc.RasterizerState.FrontCounterClockwise = VeToBool(itRaster->value, "front_ccw");
				kDesc.RasterizerState.DepthBias = VeToNumber(itRaster->value, "depth_bias", 0);
				kDesc.RasterizerState.DepthBiasClamp = VeToNumber(itRaster->value, "depth_bias_clamp", 0.0f);
				kDesc.RasterizerState.SlopeScaledDepthBias = VeToNumber(itRaster->value, "slope_scaled_depth_bias", 0.0f);
				kDesc.RasterizerState.DepthClipEnable = VeToBool(itRaster->value, "depth_clip", VE_TRUE);
				kDesc.RasterizerState.MultisampleEnable = VeToBool(itRaster->value, "msaa");
				kDesc.RasterizerState.AntialiasedLineEnable = VeToBool(itRaster->value, "aa_line");
				kDesc.RasterizerState.ForcedSampleCount = VeToNumber(itRaster->value, "sample_count", 0u);
				kDesc.RasterizerState.ConservativeRaster = VeToBool(itRaster->value, "conservative_raster") ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			}
		}
		if (bNeed)
		{
			FillRasterizerState(kDesc.RasterizerState, CULL_BACK);
		}
	}
	{
		bool bNeed = true;
		auto itDepth = kConfig.FindMember("depth_stencil_state");
		if (itDepth != kConfig.MemberEnd())
		{
			if (itDepth->value.IsString())
			{
				auto itDepthEnum = m_kDSTypeParser.find(itDepth->value.GetString());
				if (itDepthEnum != m_kDSTypeParser.end())
				{
					FillDepthStencilState(kDesc.DepthStencilState, itDepthEnum->second);
					bNeed = false;
				}
			}
			else if (itDepth->value.IsObject())
			{
				bNeed = false;
				kDesc.DepthStencilState.DepthEnable = VeToBool(itDepth->value, "depth", VE_TRUE);
				kDesc.DepthStencilState.DepthWriteMask = VeToBool(itDepth->value, "depth_write", VE_TRUE) ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
				kDesc.DepthStencilState.DepthFunc = VeToEnum(itDepth->value, "depth_func", m_kComparisonFuncParser, D3D12_COMPARISON_FUNC_LESS_EQUAL);
				kDesc.DepthStencilState.StencilEnable = VeToBool(itDepth->value, "stencil");
				kDesc.DepthStencilState.StencilReadMask = (VeUInt8)VeToNumber(itDepth->value, "stencil_read", 0xffu);
				kDesc.DepthStencilState.StencilWriteMask = (VeUInt8)VeToNumber(itDepth->value, "stencil_write", 0xffu);
				auto itStencilOp = itDepth->value.FindMember("stencil_op");
				if (itStencilOp != itDepth->value.MemberEnd())
				{
					std::function<void(VeJSONValue&,D3D12_DEPTH_STENCILOP_DESC&)> kFillStencilOp
						= [this](VeJSONValue& kVal, D3D12_DEPTH_STENCILOP_DESC& kDesc) noexcept
					{
						kDesc.StencilFailOp = VeToEnum(kVal, "fail", m_kStencilOpParser, D3D12_STENCIL_OP_KEEP);
						kDesc.StencilDepthFailOp = VeToEnum(kVal, "depth_fail", m_kStencilOpParser, D3D12_STENCIL_OP_KEEP);
						kDesc.StencilPassOp = VeToEnum(kVal, "pass", m_kStencilOpParser, D3D12_STENCIL_OP_KEEP);
						kDesc.StencilFunc = VeToEnum(kVal, "func", m_kComparisonFuncParser, D3D12_COMPARISON_FUNC_NEVER);
					};

					if (itStencilOp->value.IsObject())
					{
						kFillStencilOp(itStencilOp->value, kDesc.DepthStencilState.FrontFace);
						kDesc.DepthStencilState.BackFace = kDesc.DepthStencilState.FrontFace;
					}
					else if (itStencilOp->value.IsArray())
					{
						if (itStencilOp->value.Size() == 1)
						{
							kFillStencilOp(itStencilOp->value[0], kDesc.DepthStencilState.FrontFace);
							kDesc.DepthStencilState.BackFace = kDesc.DepthStencilState.FrontFace;
						}
						else if (itStencilOp->value.Size() > 1)
						{
							kFillStencilOp(itStencilOp->value[0], kDesc.DepthStencilState.FrontFace);
							kFillStencilOp(itStencilOp->value[1], kDesc.DepthStencilState.FrontFace);
						}
					}
				}
			}
		}
		if (bNeed)
		{
			FillDepthStencilState(kDesc.DepthStencilState, DS_NONE);
		}
	}
	D3D12_INPUT_ELEMENT_DESC akElements[32];
	{
		auto itInput = kConfig.FindMember("input_layout");
		if (itInput == kConfig.MemberEnd()
			|| (!itInput->value.IsArray())
			|| itInput->value.Empty()) return nullptr;
		kDesc.InputLayout.pInputElementDescs = akElements;
		kDesc.InputLayout.NumElements = itInput->value.Size();
		VE_ASSERT(kDesc.InputLayout.NumElements <= 32);
		for (VeUInt32 i(0); i < kDesc.InputLayout.NumElements; ++i)
		{
			D3D12_INPUT_ELEMENT_DESC& kElement = akElements[i];
			VeJSONValue& kInputValue = itInput->value[i];
			kElement.SemanticName = VeToString(kInputValue, "sem_name");
			kElement.SemanticIndex = VeToNumber(kInputValue, "sem_index", 0u);
			kElement.Format = VeToEnum(kInputValue, "format", m_kFormatParser, DXGI_FORMAT_UNKNOWN);
			kElement.InputSlot = VeToNumber(kInputValue, "slot", 0u);
			kElement.AlignedByteOffset = VeToNumber(kInputValue, "offset", 0u);
			kElement.InputSlotClass = VeToEnum(kInputValue, "class", m_kInputClassParser, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA);
			kElement.InstanceDataStepRate = VeToNumber(kInputValue, "step_rate", 0u);
		}
	}
	kDesc.IBStripCutValue = VeToEnum(kConfig, "ib_cut", m_kIBCutParser, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED);
	kDesc.PrimitiveTopologyType = VeToEnum(kConfig, "prim_topo", m_kPrimTopoTypeParser, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	{
		auto itRTVFormat = kConfig.FindMember("rtv_format");
		if (itRTVFormat != kConfig.MemberEnd())
		{
			if (itRTVFormat->value.IsString())
			{
				kDesc.NumRenderTargets = 1;
				kDesc.RTVFormats[0] = VeToEnum(itRTVFormat->value, m_kFormatParser, DXGI_FORMAT_UNKNOWN);
			}
			else if (itRTVFormat->value.IsArray())
			{
				kDesc.NumRenderTargets = VE_MIN(itRTVFormat->value.Size(), 8);
				for (VeUInt32 i(0); i < kDesc.NumRenderTargets; ++i)
				{
					kDesc.RTVFormats[i] = VeToEnum(itRTVFormat->value[i], m_kFormatParser, DXGI_FORMAT_UNKNOWN);
				}
			}
		}
		auto itDSVFormat = kConfig.FindMember("dsv_format");
		if (itDSVFormat != kConfig.MemberEnd())
		{
			if (itDSVFormat->value.IsString())
			{
				kDesc.DSVFormat = VeToEnum(itDSVFormat->value, m_kFormatParser, DXGI_FORMAT_UNKNOWN);
			}
		}
	}		
	
	kDesc.SampleDesc.Count = 1;

	ID3D12PipelineState* pkPipelineState(nullptr);
	if (SUCCEEDED(m_pkDevice->CreateGraphicsPipelineState(&kDesc,
		__uuidof(ID3D12PipelineState), (void**)&pkPipelineState)))
	{
		PipelineStateD3D12* pkObj = VE_NEW PipelineStateD3D12(pkPipelineState);
		m_kPipelineStateList.attach_back(pkObj->m_kNode);
		return pkObj;
	}
	else
	{
		VE_SAFE_RELEASE(pkPipelineState);
		return nullptr;
	}
}
//--------------------------------------------------------------------------
VeRenderer::PipelineStatePtr VeRendererD3D12::CreateComputePipelineState(
	VeJSONValue& kConfig) noexcept
{
	return nullptr;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::InitParsers() noexcept
{
	m_kBlendTypeParser["replace"] = REPLACE;
	m_kBlendTypeParser["add"] = ADD;
	m_kBlendTypeParser["blend"] = BLEND;

	m_kRasterTypeParser["cull_back"] = CULL_BACK;
	m_kRasterTypeParser["cull_front"] = CULL_FRONT;
	m_kRasterTypeParser["cull_none"] = CULL_NONE;

	m_kDSTypeParser["none"] = DS_NONE;
	m_kDSTypeParser["standard"] = DS_STANDARD;

	m_kRootParameterTypeParser["table"] = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	m_kRootParameterTypeParser["32bit"] = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	m_kRootParameterTypeParser["cbv"] = D3D12_ROOT_PARAMETER_TYPE_CBV;
	m_kRootParameterTypeParser["srv"] = D3D12_ROOT_PARAMETER_TYPE_SRV;
	m_kRootParameterTypeParser["uav"] = D3D12_ROOT_PARAMETER_TYPE_UAV;

	m_kDescriptorRangeTypeParser["srv"] = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	m_kDescriptorRangeTypeParser["uav"] = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	m_kDescriptorRangeTypeParser["cbv"] = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	m_kDescriptorRangeTypeParser["sampler"] = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;

	m_kShaderVisibilityParser["all"] = D3D12_SHADER_VISIBILITY_ALL;
	m_kShaderVisibilityParser["vs"] = D3D12_SHADER_VISIBILITY_VERTEX;
	m_kShaderVisibilityParser["hs"] = D3D12_SHADER_VISIBILITY_HULL;
	m_kShaderVisibilityParser["ds"] = D3D12_SHADER_VISIBILITY_DOMAIN;
	m_kShaderVisibilityParser["gs"] = D3D12_SHADER_VISIBILITY_GEOMETRY;
	m_kShaderVisibilityParser["ps"] = D3D12_SHADER_VISIBILITY_PIXEL;

	m_kRootSignatureFlagsParser["none"] = D3D12_ROOT_SIGNATURE_FLAG_NONE;
	m_kRootSignatureFlagsParser["ia"] = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	m_kRootSignatureFlagsParser["nvs"] = D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
	m_kRootSignatureFlagsParser["nhs"] = D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
	m_kRootSignatureFlagsParser["nds"] = D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
	m_kRootSignatureFlagsParser["ngs"] = D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	m_kRootSignatureFlagsParser["nps"] = D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
	m_kRootSignatureFlagsParser["so"] = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;

	m_kFilterParser["min_mag_mip_point"] = D3D12_FILTER_MIN_MAG_MIP_POINT;
	m_kFilterParser["min_mag_point_mip_linear"] = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	m_kFilterParser["min_point_mag_linear_mip_point"] = D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	m_kFilterParser["min_point_mag_mip_linear"] = D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	m_kFilterParser["min_linear_mag_mip_point"] = D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	m_kFilterParser["min_linear_mag_point_mip_linear"] = D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	m_kFilterParser["min_mag_linear_mip_point"] = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	m_kFilterParser["min_mag_mip_linear"] = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	m_kFilterParser["anisotropic"] = D3D12_FILTER_ANISOTROPIC;
	m_kFilterParser["comparison_min_mag_mip_point"] = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	m_kFilterParser["comparison_min_mag_point_mip_linear"] = D3D12_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
	m_kFilterParser["comparison_min_point_mag_linear_mip_point"] = D3D12_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
	m_kFilterParser["comparison_min_point_mag_mip_linear"] = D3D12_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
	m_kFilterParser["comparison_min_linear_mag_mip_point"] = D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
	m_kFilterParser["comparison_min_linear_mag_point_mip_linear"] = D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	m_kFilterParser["comparison_min_mag_linear_mip_point"] = D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	m_kFilterParser["comparison_min_mag_mip_linear"] = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	m_kFilterParser["comparison_anisotropic"] = D3D12_FILTER_COMPARISON_ANISOTROPIC;
	m_kFilterParser["minimum_min_mag_mip_point"] = D3D12_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
	m_kFilterParser["minimum_min_mag_point_mip_linear"] = D3D12_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR;
	m_kFilterParser["minimum_min_point_mag_linear_mip_point"] = D3D12_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
	m_kFilterParser["minimum_min_point_mag_mip_linear"] = D3D12_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR;
	m_kFilterParser["minimum_min_linear_mag_mip_point"] = D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT;
	m_kFilterParser["minimum_min_linear_mag_point_mip_linear"] = D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	m_kFilterParser["minimum_min_mag_linear_mip_point"] = D3D12_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT;
	m_kFilterParser["minimum_min_mag_mip_linear"] = D3D12_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR;
	m_kFilterParser["minimum_anisotropic"] = D3D12_FILTER_MINIMUM_ANISOTROPIC;
	m_kFilterParser["maximum_min_mag_mip_point"] = D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
	m_kFilterParser["maximum_min_mag_point_mip_linear"] = D3D12_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR;
	m_kFilterParser["maximum_min_point_mag_linear_mip_point"] = D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
	m_kFilterParser["maximum_min_point_mag_mip_linear"] = D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR;
	m_kFilterParser["maximum_min_linear_mag_mip_point"] = D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT;
	m_kFilterParser["maximum_min_linear_mag_point_mip_linear"] = D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	m_kFilterParser["maximum_min_mag_linear_mip_point"] = D3D12_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;
	m_kFilterParser["maximum_min_mag_mip_linear"] = D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
	m_kFilterParser["maximum_anisotropic"] = D3D12_FILTER_MAXIMUM_ANISOTROPIC;

	m_kTexAddressModeParser["wrap"] = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	m_kTexAddressModeParser["mirror"] = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
	m_kTexAddressModeParser["clamp"] = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	m_kTexAddressModeParser["border"] = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	m_kTexAddressModeParser["mirror_once"] = D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;

	m_kComparisonFuncParser["never"] = D3D12_COMPARISON_FUNC_NEVER;
	m_kComparisonFuncParser["less"] = D3D12_COMPARISON_FUNC_LESS;
	m_kComparisonFuncParser["equal"] = D3D12_COMPARISON_FUNC_EQUAL;
	m_kComparisonFuncParser["less_equal"] = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	m_kComparisonFuncParser["greater"] = D3D12_COMPARISON_FUNC_GREATER;
	m_kComparisonFuncParser["not_equal"] = D3D12_COMPARISON_FUNC_NOT_EQUAL;
	m_kComparisonFuncParser["greater_equal"] = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
	m_kComparisonFuncParser["always"] = D3D12_COMPARISON_FUNC_ALWAYS;

	m_kStaticBorderColorParser["trans_black"] = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	m_kStaticBorderColorParser["opaque_black"] = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	m_kStaticBorderColorParser["opaque_white"] = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;

	m_kFormatParser["r32g32b32a32_typeless"] = DXGI_FORMAT_R32G32B32A32_TYPELESS;
	m_kFormatParser["r32g32b32a32_float"] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_kFormatParser["r32g32b32a32_uint"] = DXGI_FORMAT_R32G32B32A32_UINT;
	m_kFormatParser["r32g32b32a32_sint"] = DXGI_FORMAT_R32G32B32A32_SINT;
	m_kFormatParser["r32g32b32_typeless"] = DXGI_FORMAT_R32G32B32_TYPELESS;
	m_kFormatParser["r32g32b32_float"] = DXGI_FORMAT_R32G32B32_FLOAT;
	m_kFormatParser["r32g32b32_uint"] = DXGI_FORMAT_R32G32B32_UINT;
	m_kFormatParser["r32g32b32_sint"] = DXGI_FORMAT_R32G32B32_SINT;
	m_kFormatParser["r16g16b16a16_typeless"] = DXGI_FORMAT_R16G16B16A16_TYPELESS;
	m_kFormatParser["r16g16b16a16_float"] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	m_kFormatParser["r16g16b16a16_unorm"] = DXGI_FORMAT_R16G16B16A16_UNORM;
	m_kFormatParser["r16g16b16a16_uint"] = DXGI_FORMAT_R16G16B16A16_UINT;
	m_kFormatParser["r16g16b16a16_snorm"] = DXGI_FORMAT_R16G16B16A16_SNORM;
	m_kFormatParser["r16g16b16a16_sint"] = DXGI_FORMAT_R16G16B16A16_SINT;
	m_kFormatParser["r32g32_typeless"] = DXGI_FORMAT_R32G32_TYPELESS;
	m_kFormatParser["r32g32_float"] = DXGI_FORMAT_R32G32_FLOAT;
	m_kFormatParser["r32g32_uint"] = DXGI_FORMAT_R32G32_UINT;
	m_kFormatParser["r32g32_sint"] = DXGI_FORMAT_R32G32_SINT;
	m_kFormatParser["r32g8x24_typeless"] = DXGI_FORMAT_R32G8X24_TYPELESS;
	m_kFormatParser["d32_float_s8x24_uint"] = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	m_kFormatParser["r32_float_x8x24_typeless"] = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
	m_kFormatParser["x32_typeless_g8x24_uint"] = DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
	m_kFormatParser["r10g10b10a2_typeless"] = DXGI_FORMAT_R10G10B10A2_TYPELESS;
	m_kFormatParser["r10g10b10a2_unorm"] = DXGI_FORMAT_R10G10B10A2_UNORM;
	m_kFormatParser["r10g10b10a2_uint"] = DXGI_FORMAT_R10G10B10A2_UINT;
	m_kFormatParser["r11g11b10_float"] = DXGI_FORMAT_R11G11B10_FLOAT;
	m_kFormatParser["r8g8b8a8_typeless"] = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	m_kFormatParser["r8g8b8a8_unorm"] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_kFormatParser["r8g8b8a8_unorm_srgb"] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	m_kFormatParser["r8g8b8a8_uint"] = DXGI_FORMAT_R8G8B8A8_UINT;
	m_kFormatParser["r8g8b8a8_snorm"] = DXGI_FORMAT_R8G8B8A8_SNORM;
	m_kFormatParser["r8g8b8a8_sint"] = DXGI_FORMAT_R8G8B8A8_SINT;
	m_kFormatParser["r16g16_typeless"] = DXGI_FORMAT_R16G16_TYPELESS;
	m_kFormatParser["r16g16_float"] = DXGI_FORMAT_R16G16_FLOAT;
	m_kFormatParser["r16g16_unorm"] = DXGI_FORMAT_R16G16_UNORM;
	m_kFormatParser["r16g16_uint"] = DXGI_FORMAT_R16G16_UINT;
	m_kFormatParser["r16g16_snorm"] = DXGI_FORMAT_R16G16_SNORM;
	m_kFormatParser["r16g16_sint"] = DXGI_FORMAT_R16G16_SINT;
	m_kFormatParser["r32_typeless"] = DXGI_FORMAT_R32_TYPELESS;
	m_kFormatParser["d32_float"] = DXGI_FORMAT_D32_FLOAT;
	m_kFormatParser["r32_float"] = DXGI_FORMAT_R32_FLOAT;
	m_kFormatParser["r32_uint"] = DXGI_FORMAT_R32_UINT;
	m_kFormatParser["r32_sint"] = DXGI_FORMAT_R32_SINT;
	m_kFormatParser["r24g8_typeless"] = DXGI_FORMAT_R24G8_TYPELESS;
	m_kFormatParser["d24_unorm_s8_uint"] = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_kFormatParser["r24_unorm_x8_typeless"] = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	m_kFormatParser["x24_typeless_g8_uint"] = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	m_kFormatParser["r8g8_typeless"] = DXGI_FORMAT_R8G8_TYPELESS;
	m_kFormatParser["r8g8_unorm"] = DXGI_FORMAT_R8G8_UNORM;
	m_kFormatParser["r8g8_uint"] = DXGI_FORMAT_R8G8_UINT;
	m_kFormatParser["r8g8_snorm"] = DXGI_FORMAT_R8G8_SNORM;
	m_kFormatParser["r8g8_sint"] = DXGI_FORMAT_R8G8_SINT;
	m_kFormatParser["r16_typeless"] = DXGI_FORMAT_R16_TYPELESS;
	m_kFormatParser["r16_float"] = DXGI_FORMAT_R16_FLOAT;
	m_kFormatParser["d16_unorm"] = DXGI_FORMAT_D16_UNORM;
	m_kFormatParser["r16_unorm"] = DXGI_FORMAT_R16_UNORM;
	m_kFormatParser["r16_uint"] = DXGI_FORMAT_R16_UINT;
	m_kFormatParser["r16_snorm"] = DXGI_FORMAT_R16_SNORM;
	m_kFormatParser["r16_sint"] = DXGI_FORMAT_R16_SINT;
	m_kFormatParser["r8_typeless"] = DXGI_FORMAT_R8_TYPELESS;
	m_kFormatParser["r8_unorm"] = DXGI_FORMAT_R8_UNORM;
	m_kFormatParser["r8_uint"] = DXGI_FORMAT_R8_UINT;
	m_kFormatParser["r8_snorm"] = DXGI_FORMAT_R8_SNORM;
	m_kFormatParser["r8_sint"] = DXGI_FORMAT_R8_SINT;
	m_kFormatParser["a8_unorm"] = DXGI_FORMAT_A8_UNORM;
	m_kFormatParser["r1_unorm"] = DXGI_FORMAT_R1_UNORM;
	m_kFormatParser["r9g9b9e5_sharedexp"] = DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
	m_kFormatParser["r8g8_b8g8_unorm"] = DXGI_FORMAT_R8G8_B8G8_UNORM;
	m_kFormatParser["g8r8_g8b8_unorm"] = DXGI_FORMAT_G8R8_G8B8_UNORM;
	m_kFormatParser["bc1_typeless"] = DXGI_FORMAT_BC1_TYPELESS;
	m_kFormatParser["bc1_unorm"] = DXGI_FORMAT_BC1_UNORM;
	m_kFormatParser["bc1_unorm_srgb"] = DXGI_FORMAT_BC1_UNORM_SRGB;
	m_kFormatParser["bc2_typeless"] = DXGI_FORMAT_BC2_TYPELESS;
	m_kFormatParser["bc2_unorm"] = DXGI_FORMAT_BC2_UNORM;
	m_kFormatParser["bc2_unorm_srgb"] = DXGI_FORMAT_BC2_UNORM_SRGB;
	m_kFormatParser["bc3_typeless"] = DXGI_FORMAT_BC3_TYPELESS;
	m_kFormatParser["bc3_unorm"] = DXGI_FORMAT_BC3_UNORM;
	m_kFormatParser["bc3_unorm_srgb"] = DXGI_FORMAT_BC3_UNORM_SRGB;
	m_kFormatParser["bc4_typeless"] = DXGI_FORMAT_BC4_TYPELESS;
	m_kFormatParser["bc4_unorm"] = DXGI_FORMAT_BC4_UNORM;
	m_kFormatParser["bc4_snorm"] = DXGI_FORMAT_BC4_SNORM;
	m_kFormatParser["bc5_typeless"] = DXGI_FORMAT_BC5_TYPELESS;
	m_kFormatParser["bc5_unorm"] = DXGI_FORMAT_BC5_UNORM;
	m_kFormatParser["bc5_snorm"] = DXGI_FORMAT_BC5_SNORM;
	m_kFormatParser["b5g6r5_unorm"] = DXGI_FORMAT_B5G6R5_UNORM;
	m_kFormatParser["b5g5r5a1_unorm"] = DXGI_FORMAT_B5G5R5A1_UNORM;
	m_kFormatParser["b8g8r8a8_unorm"] = DXGI_FORMAT_B8G8R8A8_UNORM;
	m_kFormatParser["b8g8r8x8_unorm"] = DXGI_FORMAT_B8G8R8X8_UNORM;
	m_kFormatParser["r10g10b10_xr_bias_a2_unorm"] = DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
	m_kFormatParser["b8g8r8a8_typeless"] = DXGI_FORMAT_B8G8R8A8_TYPELESS;
	m_kFormatParser["b8g8r8a8_unorm_srgb"] = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	m_kFormatParser["b8g8r8x8_typeless"] = DXGI_FORMAT_B8G8R8X8_TYPELESS;
	m_kFormatParser["b8g8r8x8_unorm_srgb"] = DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
	m_kFormatParser["bc6h_typeless"] = DXGI_FORMAT_BC6H_TYPELESS;
	m_kFormatParser["bc6h_uf16"] = DXGI_FORMAT_BC6H_UF16;
	m_kFormatParser["bc6h_sf16"] = DXGI_FORMAT_BC6H_SF16;
	m_kFormatParser["bc7_typeless"] = DXGI_FORMAT_BC7_TYPELESS;
	m_kFormatParser["bc7_unorm"] = DXGI_FORMAT_BC7_UNORM;
	m_kFormatParser["bc7_unorm_srgb"] = DXGI_FORMAT_BC7_UNORM_SRGB;
	m_kFormatParser["ayuv"] = DXGI_FORMAT_AYUV;
	m_kFormatParser["y410"] = DXGI_FORMAT_Y410;
	m_kFormatParser["y416"] = DXGI_FORMAT_Y416;
	m_kFormatParser["nv12"] = DXGI_FORMAT_NV12;
	m_kFormatParser["p010"] = DXGI_FORMAT_P010;
	m_kFormatParser["p016"] = DXGI_FORMAT_P016;
	m_kFormatParser["420_opaque"] = DXGI_FORMAT_420_OPAQUE;
	m_kFormatParser["yuy2"] = DXGI_FORMAT_YUY2;
	m_kFormatParser["y210"] = DXGI_FORMAT_Y210;
	m_kFormatParser["y216"] = DXGI_FORMAT_Y216;
	m_kFormatParser["nv11"] = DXGI_FORMAT_NV11;
	m_kFormatParser["ai44"] = DXGI_FORMAT_AI44;
	m_kFormatParser["ia44"] = DXGI_FORMAT_IA44;
	m_kFormatParser["p8"] = DXGI_FORMAT_P8;
	m_kFormatParser["a8p8"] = DXGI_FORMAT_A8P8;
	m_kFormatParser["b4g4r4a4_unorm"] = DXGI_FORMAT_B4G4R4A4_UNORM;
	m_kFormatParser["p208"] = DXGI_FORMAT_P208;
	m_kFormatParser["v208"] = DXGI_FORMAT_V208;
	m_kFormatParser["v408"] = DXGI_FORMAT_V408;

	m_kInputClassParser["vertex"] = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	m_kInputClassParser["instance"] = D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;

	m_kBlendParser["zero"] = D3D12_BLEND_ZERO;
	m_kBlendParser["one"] = D3D12_BLEND_ONE;
	m_kBlendParser["src_color"] = D3D12_BLEND_SRC_COLOR;
	m_kBlendParser["inv_src_color"] = D3D12_BLEND_INV_SRC_COLOR;
	m_kBlendParser["src_alpha"] = D3D12_BLEND_SRC_ALPHA;
	m_kBlendParser["inv_src_alpha"] = D3D12_BLEND_INV_SRC_ALPHA;
	m_kBlendParser["dest_alpha"] = D3D12_BLEND_DEST_ALPHA;
	m_kBlendParser["inv_dest_alpha"] = D3D12_BLEND_INV_DEST_ALPHA;
	m_kBlendParser["dest_color"] = D3D12_BLEND_DEST_COLOR;
	m_kBlendParser["inv_dest_color"] = D3D12_BLEND_INV_DEST_COLOR;
	m_kBlendParser["src_alpha_sat"] = D3D12_BLEND_SRC_ALPHA_SAT;
	m_kBlendParser["blend_factor"] = D3D12_BLEND_BLEND_FACTOR;
	m_kBlendParser["inv_blend_factor"] = D3D12_BLEND_INV_BLEND_FACTOR;
	m_kBlendParser["src1_color"] = D3D12_BLEND_SRC1_COLOR;
	m_kBlendParser["inv_src1_color"] = D3D12_BLEND_INV_SRC1_COLOR;
	m_kBlendParser["src1_alpha"] = D3D12_BLEND_SRC1_ALPHA;
	m_kBlendParser["inv_src1_alpha"] = D3D12_BLEND_INV_SRC1_ALPHA;

	m_kBlendOpParser["add"] = D3D12_BLEND_OP_ADD;
	m_kBlendOpParser["subtract"] = D3D12_BLEND_OP_SUBTRACT;
	m_kBlendOpParser["rev_subtract"] = D3D12_BLEND_OP_REV_SUBTRACT;
	m_kBlendOpParser["min"] = D3D12_BLEND_OP_MIN;
	m_kBlendOpParser["max"] = D3D12_BLEND_OP_MAX;

	m_kLogicOpParser["clear"] = D3D12_LOGIC_OP_CLEAR;
	m_kLogicOpParser["set"] = D3D12_LOGIC_OP_SET;
	m_kLogicOpParser["copy"] = D3D12_LOGIC_OP_COPY;
	m_kLogicOpParser["copy_inverted"] = D3D12_LOGIC_OP_COPY_INVERTED;
	m_kLogicOpParser["noop"] = D3D12_LOGIC_OP_NOOP;
	m_kLogicOpParser["invert"] = D3D12_LOGIC_OP_INVERT;
	m_kLogicOpParser["and"] = D3D12_LOGIC_OP_AND;
	m_kLogicOpParser["nand"] = D3D12_LOGIC_OP_NAND;
	m_kLogicOpParser["or"] = D3D12_LOGIC_OP_OR;
	m_kLogicOpParser["nor"] = D3D12_LOGIC_OP_NOR;
	m_kLogicOpParser["xor"] = D3D12_LOGIC_OP_XOR;
	m_kLogicOpParser["equiv"] = D3D12_LOGIC_OP_EQUIV;
	m_kLogicOpParser["and_reverse"] = D3D12_LOGIC_OP_AND_REVERSE;
	m_kLogicOpParser["and_inverted"] = D3D12_LOGIC_OP_AND_INVERTED;
	m_kLogicOpParser["or_reverse"] = D3D12_LOGIC_OP_OR_REVERSE;
	m_kLogicOpParser["or_inverted"] = D3D12_LOGIC_OP_OR_INVERTED;

	m_kColorWriteParser["r"] = D3D12_COLOR_WRITE_ENABLE_RED;
	m_kColorWriteParser["g"] = D3D12_COLOR_WRITE_ENABLE_GREEN;
	m_kColorWriteParser["b"] = D3D12_COLOR_WRITE_ENABLE_BLUE;
	m_kColorWriteParser["a"] = D3D12_COLOR_WRITE_ENABLE_ALPHA;
	m_kColorWriteParser["all"] = D3D12_COLOR_WRITE_ENABLE_ALL;

	m_kFillModeParser["wireframe"] = D3D12_FILL_MODE_WIREFRAME;
	m_kFillModeParser["solid"] = D3D12_FILL_MODE_SOLID;

	m_kCullModeParser["none"] = D3D12_CULL_MODE_NONE;
	m_kCullModeParser["front"] = D3D12_CULL_MODE_FRONT;
	m_kCullModeParser["back"] = D3D12_CULL_MODE_BACK;

	m_kStencilOpParser["keep"] = D3D12_STENCIL_OP_KEEP;
	m_kStencilOpParser["zero"] = D3D12_STENCIL_OP_ZERO;
	m_kStencilOpParser["replace"] = D3D12_STENCIL_OP_REPLACE;
	m_kStencilOpParser["inc_sat"] = D3D12_STENCIL_OP_INCR_SAT;
	m_kStencilOpParser["dec_sat"] = D3D12_STENCIL_OP_DECR_SAT;
	m_kStencilOpParser["invert"] = D3D12_STENCIL_OP_INVERT;
	m_kStencilOpParser["inc"] = D3D12_STENCIL_OP_INCR;
	m_kStencilOpParser["dec"] = D3D12_STENCIL_OP_DECR;

	m_kIBCutParser["disable"] = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	m_kIBCutParser["16bit"] = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF;
	m_kIBCutParser["32bit"] = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF;

	m_kPrimTopoTypeParser["undef"] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
	m_kPrimTopoTypeParser["point"] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	m_kPrimTopoTypeParser["line"] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	m_kPrimTopoTypeParser["triangle"] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_kPrimTopoTypeParser["patch"] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::TermParsers() noexcept
{
	m_kBlendTypeParser.clear();
	m_kRasterTypeParser.clear();
	m_kDSTypeParser.clear();
	m_kRootParameterTypeParser.clear();
	m_kDescriptorRangeTypeParser.clear();
	m_kShaderVisibilityParser.clear();
	m_kRootSignatureFlagsParser.clear();
	m_kFilterParser.clear();
	m_kTexAddressModeParser.clear();
	m_kComparisonFuncParser.clear();
	m_kStaticBorderColorParser.clear();
	m_kFormatParser.clear();
	m_kInputClassParser.clear();
	m_kBlendParser.clear();
	m_kBlendOpParser.clear();
	m_kLogicOpParser.clear();
	m_kColorWriteParser.clear();
	m_kFillModeParser.clear();
	m_kCullModeParser.clear();
	m_kStencilOpParser.clear();
	m_kIBCutParser.clear();
	m_kPrimTopoTypeParser.clear();
}
//--------------------------------------------------------------------------
VeRendererPtr CreateRendererD3D12() noexcept
{
	return VE_NEW VeRendererD3D12();
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
