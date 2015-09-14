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
#include "VeRenderBufferD3D12.h"

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
	InitEnums();
}
//--------------------------------------------------------------------------
VeRendererD3D12::~VeRendererD3D12() noexcept
{
	VE_ASSERT((!m_pkDXGIFactory) && (!m_pkDefaultAdapter) && (!m_pkDevice));
}
//--------------------------------------------------------------------------
bool VeRendererD3D12::Init() noexcept
{	
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
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pkDebugController))))
		{
			pkDebugController->EnableDebugLayer();
		}
		VE_SAFE_RELEASE(pkDebugController);
	}
#	endif

	if (VE_FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&m_pkDXGIFactory))))
		return false;
	
	if(m_pkDXGIFactory->EnumAdapters1(0, &m_pkDefaultAdapter) == DXGI_ERROR_NOT_FOUND)
		return false;	

	if (VE_FAILED(D3D12CreateDevice(m_pkDefaultAdapter,
		D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pkDevice))))
		return false;

	m_kRTVHeap.Init(m_pkDevice);
	m_kDSVHeap.Init(m_pkDevice);
	m_kSRVHeap.Init(m_pkDevice);
	InitCopyQueue();

	ve_sys.CORE.I.LogFormat("VeRendererD3D12 renderer is created.");
	return true;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::Term() noexcept
{
	for (auto obj : m_kRenderWindowList)
	{
		obj->Term();
	}
	VE_ASSERT(m_kRenderWindowList.empty());
	for (auto obj : m_kRenderBufferList)
	{
		obj->Term();
	}
	VE_ASSERT(m_kRenderBufferList.empty());
	for (auto obj : m_kRootSignatureList)
	{
		VE_SAFE_RELEASE(obj->m_pkRootSignature);
	}
	m_kRootSignatureList.clear();
	for (auto obj : m_kPipelineStateList)
	{
		VE_SAFE_RELEASE(obj->m_pkPipelineState);
	}
	m_kPipelineStateList.clear();
	TermCopyQueue();
	m_kSRVHeap.Term();
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
}
//--------------------------------------------------------------------------
void VeRendererD3D12::BeginSyncCopy() noexcept
{
	const VeUInt64 u64LastCompleted = m_pkCopyFence->GetCompletedValue();
	if (u64LastCompleted < m_u64CopyFenceValue)
	{
		VE_ASSERT_GE(m_pkCopyFence->SetEventOnCompletion(
			m_u64CopyFenceValue, m_kCopyFenceEvent), S_OK);
		WaitForSingleObject(m_kCopyFenceEvent, INFINITE);
	}
	if (m_kCopyResList.size())
	{
		for (auto res : m_kCopyResList)
		{
			res->Release();
		}
		m_kCopyResList.clear();
		VE_ASSERT_GE(m_pkCopyAllocator->Reset(), S_OK);
		VE_ASSERT_GE(m_pkCopyCommandList->Reset(m_pkCopyAllocator, nullptr), S_OK);
	}
}
//--------------------------------------------------------------------------
void VeRendererD3D12::EndSyncCopy() noexcept
{
	if (m_kCopyResList.size())
	{
		VE_ASSERT_GE(m_pkCopyCommandList->Close(), S_OK);
		m_pkCopyQueue->ExecuteCommandLists(1, (ID3D12CommandList*const*)&m_pkCopyCommandList);
		VE_ASSERT_GE(m_pkCopyQueue->Signal(m_pkCopyFence, ++m_u64CopyFenceValue), S_OK);
	}
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
VeVector<D3D_SHADER_MACRO> GetMacro(VeJSONValue& val) noexcept
{
	VeVector<D3D_SHADER_MACRO> kRes;
	auto it = val.FindMember("define");
	if (it == val.MemberEnd()) return kRes;
	VeJSONValue& def = it->value;
	if (def.IsObject())
	{
		for (it = def.MemberBegin(); it != def.MemberEnd(); ++it)
		{
			if (it->value.IsString())
			{
				kRes.push_back({ it->name.GetString(), it->value.GetString() });
			}
		}
	}
	if (kRes.size())
	{
		kRes.push_back({ nullptr,nullptr });
	}
	return kRes;
}
//--------------------------------------------------------------------------
class ShaderInclude : public ID3DInclude
{
public:
	ShaderInclude(const VeStringMap<VeUInt32>& kShaderNameMap,
		const VeVector<VeBlobPtr>& kShaderList) noexcept
		: m_kShaderNameMap(kShaderNameMap), m_kShaderList(kShaderList) {}

	HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName,
		LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override
	{
		auto it = m_kShaderNameMap.find(pFileName);
		if (it != m_kShaderNameMap.end())
		{
			const VeBlob& kBlob = *m_kShaderList[it->second];
			*ppData = kBlob;
			*pBytes = VeUInt32(kBlob.GetSize());
			return S_OK;
		}
		return E_FAIL;
	}

	HRESULT Close(LPCVOID pData) override
	{
		return S_OK;
	}

protected:
	const VeStringMap<VeUInt32>& m_kShaderNameMap;
	const VeVector<VeBlobPtr>& m_kShaderList;

};
//--------------------------------------------------------------------------
std::pair<VeBlobPtr, VeRenderer::ShaderType>
VeRendererD3D12::CompileShader(const VeChar8* pcFile,
	const VeChar8* pcTarget, const VeChar8* pcPath, VeJSONValue& kConfig,
	const VeStringMap<VeUInt32>& kShaderNameMap,
	const VeVector<VeBlobPtr>& kShaderList) noexcept
{
	auto it = kShaderNameMap.find(pcFile);
	if (it == kShaderNameMap.end())
	{
		ve_sys.CORE.E.LogFormat("Shader[\"%s\"] not found.", pcFile);
		return std::make_pair(nullptr, SHADER_TYPE_NUM);
	}
	const VeBlob& kHLSL = *kShaderList[it->second];
	const VeChar8* pcEntry = kConfig("entry", "");	
	const D3D_SHADER_MACRO* pkMacro(nullptr);
	auto kDefines = GetMacro(kConfig);
	if (kDefines.size()) pkMacro = &kDefines.front();
	ShaderInclude kInclude(kShaderNameMap, kShaderList);
#	ifdef VE_DEBUG
	VeUInt32 u32CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#	else
	VeUInt32 u32CompileFlags = 0;
#	endif
	ID3DBlob* pkCode = nullptr;
	ID3DBlob* pkError = nullptr;
	if (SUCCEEDED(D3DCompile(kHLSL, kHLSL.GetSize(), pcPath,
		pkMacro, &kInclude, pcEntry, pcTarget, u32CompileFlags, 0, &pkCode, &pkError)))
	{
		VeBlobPtr spBlob = VE_NEW VeBlob(pkCode->GetBufferSize());
		VeMemoryCopy(spBlob->GetBuffer(), pkCode->GetBufferPointer(), pkCode->GetBufferSize());
		VE_SAFE_RELEASE(pkCode);
		VE_SAFE_RELEASE(pkError);
		return std::make_pair(spBlob, GetTargetType(pcTarget));
	}
	if (pkError)
	{
		ve_sys.CORE.E.LogFormat((const VeChar8*)pkError->GetBufferPointer());
	}
	VE_SAFE_RELEASE(pkCode);
	VE_SAFE_RELEASE(pkError);
	return std::make_pair(nullptr, TYPE_CS);
}
//--------------------------------------------------------------------------
VeBlobPtr VeRendererD3D12::SerializeRootSignature(
	VeJSONValue& kConfig) noexcept
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
			kParameter.ParameterType = kValue("type", D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE);
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

						kRange.RangeType = kValueRange("type", D3D12_DESCRIPTOR_RANGE_TYPE_SRV);
						kRange.NumDescriptors = kValueRange("num", 0u);
						kRange.BaseShaderRegister = kValueRange("register", 0u);
						kRange.RegisterSpace = kValueRange("space", 0u);
						kRange.OffsetInDescriptorsFromTableStart = kValueRange("offset", 0u);
					}
				}
				kParameter.ShaderVisibility = kValue("visibility", D3D12_SHADER_VISIBILITY_ALL);
			}
			break;
			case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
			{
				kParameter.Constants.Num32BitValues = kValue("num", 0u);
				kParameter.Constants.ShaderRegister = kValue("register", 0u);
				kParameter.Constants.RegisterSpace = kValue("space", 0u);
			}
			break;
			case D3D12_ROOT_PARAMETER_TYPE_CBV:
			case D3D12_ROOT_PARAMETER_TYPE_SRV:
			case D3D12_ROOT_PARAMETER_TYPE_UAV:
			{
				kParameter.Descriptor.ShaderRegister = kValue("register", 0u);
				kParameter.Descriptor.RegisterSpace = kValue("space", 0u);
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
			kSampler.Filter = kValue("filter", D3D12_FILTER_MIN_MAG_MIP_LINEAR);
			kSampler.AddressU = kValue("addU", D3D12_TEXTURE_ADDRESS_MODE_WRAP);
			kSampler.AddressV = kValue("addV", D3D12_TEXTURE_ADDRESS_MODE_WRAP);
			kSampler.AddressW = kValue("addW", D3D12_TEXTURE_ADDRESS_MODE_WRAP);
			kSampler.MipLODBias = kValue("lod_bias", 0.0f);
			kSampler.MaxAnisotropy = kValue("max_ani", 0u);
			kSampler.ComparisonFunc = kValue("comp_func", D3D12_COMPARISON_FUNC_ALWAYS);
			kSampler.BorderColor = kValue("border_color", D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK);
			kSampler.MinLOD = kValue("min_lod", 0.0f);
			kSampler.MaxLOD = kValue("max_lod", D3D12_FLOAT32_MAX);
			kSampler.ShaderRegister = kValue("register", 0u);
			kSampler.RegisterSpace = kValue("space", 0u);
			kSampler.ShaderVisibility = kValue("visibility", D3D12_SHADER_VISIBILITY_ALL);
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

	kDesc.Flags |= ve_parser.ParseFlags(itFlags->value.GetString(),
		D3D12_ROOT_SIGNATURE_FLAG_NONE);

	VeBlobPtr spBlob;
	ID3DBlob* pkCode = nullptr;
	ID3DBlob* pkError = nullptr;
	if (SUCCEEDED(D3D12SerializeRootSignature(&kDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pkCode, &pkError)))
	{
		spBlob = VE_NEW VeBlob(pkCode->GetBufferSize());
		VeMemoryCopy(spBlob->GetBuffer(), pkCode->GetBufferPointer(), pkCode->GetBufferSize());
	}
	else
	{
		if (pkError)
		{
			ve_sys.CORE.E.LogFormat((const VeChar8*)pkError->GetBufferPointer());
		}
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
		spBlob->GetSize(), IID_PPV_ARGS(&pkRootSig))))
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
	VeJSONValue& kConfig, VeBlobPtr& spOut) noexcept
{
	VeMemoryOStream kStream;
	spOut = kStream.GetBlob();
	bool bGraphics = kConfig("graphics", true);
	return bGraphics ? CreateGraphicsPipelineState(kConfig, kStream)
		: CreateComputePipelineState(kConfig, kStream);
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
void FillStencilOp(VeJSONValue& kVal,
	D3D12_DEPTH_STENCILOP_DESC& kDesc) noexcept
{
	kDesc.StencilFailOp = kVal("fail", D3D12_STENCIL_OP_KEEP);
	kDesc.StencilDepthFailOp = kVal("depth_fail", D3D12_STENCIL_OP_KEEP);
	kDesc.StencilPassOp = kVal("pass", D3D12_STENCIL_OP_KEEP);
	kDesc.StencilFunc = kVal("func", D3D12_COMPARISON_FUNC_NEVER);
}
//--------------------------------------------------------------------------
VeRenderer::PipelineStatePtr VeRendererD3D12::CreateGraphicsPipelineState(
	VeJSONValue& kConfig, VeMemoryOStream& kOut) noexcept
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC kDesc = {};
	{
		const VeChar8* pcName = kConfig("root_signature", "");
		auto it = m_kRootSignatureMap.find(pcName);
		if (it == m_kRootSignatureMap.end()) return nullptr;
		RootSignatureD3D12* pkRootSig = VeDynamicCast(RootSignatureD3D12, it->second.p());
		if ((!pkRootSig) || (!pkRootSig->m_pkRootSignature)) return nullptr;
		kDesc.pRootSignature = pkRootSig->m_pkRootSignature;
	}
	{
		const VeChar8* pcName = kConfig(ve_parser.EnumToStr(TYPE_VS), "");
		auto it = m_akShaderMap[TYPE_VS].find(pcName);
		if (it != m_akShaderMap[TYPE_VS].end())
		{
			kDesc.VS.pShaderBytecode = it->second->GetBuffer();
			kDesc.VS.BytecodeLength = it->second->GetSize();
		}
	}
	{
		const VeChar8* pcName = kConfig(ve_parser.EnumToStr(TYPE_PS), "");
		auto it = m_akShaderMap[TYPE_PS].find(pcName);
		if (it != m_akShaderMap[TYPE_PS].end())
		{
			kDesc.PS.pShaderBytecode = it->second->GetBuffer();
			kDesc.PS.BytecodeLength = it->second->GetSize();
		}
	}
	{
		const VeChar8* pcName = kConfig(ve_parser.EnumToStr(TYPE_GS), "");
		auto it = m_akShaderMap[TYPE_GS].find(pcName);
		if (it != m_akShaderMap[TYPE_GS].end())
		{
			kDesc.GS.pShaderBytecode = it->second->GetBuffer();
			kDesc.GS.BytecodeLength = it->second->GetSize();
		}
	}
	{
		const VeChar8* pcName = kConfig(ve_parser.EnumToStr(TYPE_HS), "");
		auto it = m_akShaderMap[TYPE_HS].find(pcName);
		if (it != m_akShaderMap[TYPE_HS].end())
		{
			kDesc.HS.pShaderBytecode = it->second->GetBuffer();
			kDesc.HS.BytecodeLength = it->second->GetSize();
		}
	}
	{
		const VeChar8* pcName = kConfig(ve_parser.EnumToStr(TYPE_DS), "");
		auto it = m_akShaderMap[TYPE_DS].find(pcName);
		if (it != m_akShaderMap[TYPE_DS].end())
		{
			kDesc.DS.pShaderBytecode = it->second->GetBuffer();
			kDesc.DS.BytecodeLength = it->second->GetSize();
		}
	}
	{
		bool bNeed = true;
		auto itBlend = kConfig.FindMember("blend_state");
		if (itBlend != kConfig.MemberEnd())
		{
			if (itBlend->value.IsString())
			{
				BlendType eType = VeTo(itBlend->value, VE_TMAX(BlendType));
				if (eType != VE_TMAX(BlendType))
				{
					FillBlendState(kDesc.BlendState, eType);
					bNeed = false;
				}
			}
			else if (itBlend->value.IsObject())
			{
				bNeed = false;
				VeJSONValue& kBlend = itBlend->value;
				kDesc.BlendState.AlphaToCoverageEnable = kBlend("alpha_to_coverage", false);
				kDesc.BlendState.IndependentBlendEnable = kBlend("independent", false);
				auto itRTBlend = kBlend.FindMember("blend");
				if (itRTBlend != kBlend.MemberEnd())
				{
					VeUInt32 u32Count(0);
					if (itRTBlend->value.IsObject())
					{
						kDesc.BlendState.RenderTarget[u32Count].BlendEnable = itRTBlend->value("blend", false);
						kDesc.BlendState.RenderTarget[u32Count].LogicOpEnable = itRTBlend->value("logic_op", false);
						kDesc.BlendState.RenderTarget[u32Count].SrcBlend = itRTBlend->value("src", D3D12_BLEND_ZERO);
						kDesc.BlendState.RenderTarget[u32Count].DestBlend = itRTBlend->value("dst", D3D12_BLEND_ZERO);
						kDesc.BlendState.RenderTarget[u32Count].BlendOp = itRTBlend->value("op", D3D12_BLEND_OP_ADD);
						kDesc.BlendState.RenderTarget[u32Count].SrcBlendAlpha = itRTBlend->value("src_alpha", D3D12_BLEND_ZERO);
						kDesc.BlendState.RenderTarget[u32Count].DestBlendAlpha = itRTBlend->value("dst_alpha", D3D12_BLEND_ZERO);
						kDesc.BlendState.RenderTarget[u32Count].BlendOpAlpha = itRTBlend->value("op_alpha", D3D12_BLEND_OP_ADD);
						kDesc.BlendState.RenderTarget[u32Count].LogicOp = itRTBlend->value("logic_op", D3D12_LOGIC_OP_CLEAR);
						auto itMask = itRTBlend->value.FindMember("mask");
						if (itMask != itRTBlend->value.MemberEnd()
							&& itMask->value.IsString())
						{
							kDesc.BlendState.RenderTarget[u32Count].RenderTargetWriteMask |= ve_parser.ParseFlags(itMask->value.GetString(), VE_TMIN(D3D12_COLOR_WRITE_ENABLE));
						}
						else
						{
							kDesc.BlendState.RenderTarget[u32Count].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
						}
						++u32Count;
					}
					else if (itRTBlend->value.IsArray())
					{
						for (auto itRT = itRTBlend->value.Begin(); itRT != itRTBlend->value.End(); ++itRT)
						{
							if (itRT->IsObject())
							{

								kDesc.BlendState.RenderTarget[u32Count].BlendEnable = itRTBlend->value("blend", false);
								kDesc.BlendState.RenderTarget[u32Count].LogicOpEnable = itRTBlend->value("logic_op", false);
								kDesc.BlendState.RenderTarget[u32Count].SrcBlend = itRTBlend->value("src", D3D12_BLEND_ZERO);
								kDesc.BlendState.RenderTarget[u32Count].DestBlend = itRTBlend->value("dst", D3D12_BLEND_ZERO);
								kDesc.BlendState.RenderTarget[u32Count].BlendOp = itRTBlend->value("op", D3D12_BLEND_OP_ADD);
								kDesc.BlendState.RenderTarget[u32Count].SrcBlendAlpha = itRTBlend->value("src_alpha", D3D12_BLEND_ZERO);
								kDesc.BlendState.RenderTarget[u32Count].DestBlendAlpha = itRTBlend->value("dst_alpha", D3D12_BLEND_ZERO);
								kDesc.BlendState.RenderTarget[u32Count].BlendOpAlpha = itRTBlend->value("op_alpha", D3D12_BLEND_OP_ADD);
								kDesc.BlendState.RenderTarget[u32Count].LogicOp = itRTBlend->value("logic_op", D3D12_LOGIC_OP_CLEAR);
								auto itMask = itRTBlend->value.FindMember("mask");
								if (itMask != itRTBlend->value.MemberEnd()
									&& itMask->value.IsString())
								{
									kDesc.BlendState.RenderTarget[u32Count].RenderTargetWriteMask |= ve_parser.ParseFlags(itMask->value.GetString(), VE_TMIN(D3D12_COLOR_WRITE_ENABLE));
								}
								else
								{
									kDesc.BlendState.RenderTarget[u32Count].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
								}
								if ((++u32Count) >= 8) break;
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
		kDesc.SampleMask = kConfig("sample_mask", VE_UINT32_MAX);
	}
	{
		bool bNeed = true;
		auto itRaster = kConfig.FindMember("rasterizer_state");
		if (itRaster != kConfig.MemberEnd())
		{
			if (itRaster->value.IsString())
			{
				RasterType eType = VeTo(itRaster->value, VE_TMAX(RasterType));
				if (eType != VE_TMAX(RasterType))
				{
					FillRasterizerState(kDesc.RasterizerState, eType);
					bNeed = false;
				}
			}
			else if (itRaster->value.IsObject())
			{
				bNeed = false;
				kDesc.RasterizerState.FillMode = itRaster->value("fill_mode", D3D12_FILL_MODE_SOLID);
				kDesc.RasterizerState.CullMode = itRaster->value("cull_mode", D3D12_CULL_MODE_BACK);
				kDesc.RasterizerState.FrontCounterClockwise = itRaster->value("front_ccw", false);
				kDesc.RasterizerState.DepthBias = itRaster->value("depth_bias", 0);
				kDesc.RasterizerState.DepthBiasClamp = itRaster->value("depth_bias_clamp", 0.0f);
				kDesc.RasterizerState.SlopeScaledDepthBias = itRaster->value("slope_scaled_depth_bias", 0.0f);
				kDesc.RasterizerState.DepthClipEnable = itRaster->value("depth_clip", true);
				kDesc.RasterizerState.MultisampleEnable = itRaster->value("msaa", false);
				kDesc.RasterizerState.AntialiasedLineEnable = itRaster->value("aa_line", false);
				kDesc.RasterizerState.ForcedSampleCount = itRaster->value("sample_count", 0u);
				kDesc.RasterizerState.ConservativeRaster = itRaster->value("conservative_raster", false) ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
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
				DepthStencilType eType = VeTo(itDepth->value, VE_TMAX(DepthStencilType));
				if (eType != VE_TMAX(DepthStencilType))
				{
					FillDepthStencilState(kDesc.DepthStencilState, eType);
					bNeed = false;
				}
			}
			else if (itDepth->value.IsObject())
			{
				bNeed = false;
				kDesc.DepthStencilState.DepthEnable = itDepth->value("depth", true);
				kDesc.DepthStencilState.DepthWriteMask = itDepth->value("depth_write", true) ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
				kDesc.DepthStencilState.DepthFunc = itDepth->value("depth_func", D3D12_COMPARISON_FUNC_LESS_EQUAL);
				kDesc.DepthStencilState.StencilEnable = itDepth->value("stencil", false);
				kDesc.DepthStencilState.StencilReadMask = (VeUInt8)itDepth->value("stencil_read", 0xffu);
				kDesc.DepthStencilState.StencilWriteMask = (VeUInt8)itDepth->value("stencil_write", 0xffu);
				auto itStencilOp = itDepth->value.FindMember("stencil_op");
				if (itStencilOp != itDepth->value.MemberEnd())
				{
					if (itStencilOp->value.IsObject())
					{
						FillStencilOp(itStencilOp->value, kDesc.DepthStencilState.FrontFace);
						kDesc.DepthStencilState.BackFace = kDesc.DepthStencilState.FrontFace;
					}
					else if (itStencilOp->value.IsArray())
					{
						if (itStencilOp->value.Size() == 1)
						{
							FillStencilOp(itStencilOp->value[0], kDesc.DepthStencilState.FrontFace);
							kDesc.DepthStencilState.BackFace = kDesc.DepthStencilState.FrontFace;
						}
						else if (itStencilOp->value.Size() > 1)
						{
							FillStencilOp(itStencilOp->value[0], kDesc.DepthStencilState.FrontFace);
							FillStencilOp(itStencilOp->value[1], kDesc.DepthStencilState.FrontFace);
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
			kElement.SemanticName = kInputValue("sem_name", "");
			kElement.SemanticIndex = kInputValue("sem_index", 0u);
			kElement.Format = kInputValue("format", DXGI_FORMAT_UNKNOWN);
			kElement.InputSlot = kInputValue("slot", 0u);
			kElement.AlignedByteOffset = kInputValue("offset", 0u);
			kElement.InputSlotClass = kInputValue("class", D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA);
			kElement.InstanceDataStepRate = kInputValue("step_rate", 0u);
		}
	}
	kDesc.IBStripCutValue = kConfig("ib_cut", D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED);
	kDesc.PrimitiveTopologyType = kConfig("prim_topo", D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	{
		auto itRTVFormat = kConfig.FindMember("rtv_format");
		if (itRTVFormat != kConfig.MemberEnd())
		{
			if (itRTVFormat->value.IsString())
			{
				kDesc.NumRenderTargets = 1;
				kDesc.RTVFormats[0] = VeTo(itRTVFormat->value, DXGI_FORMAT_UNKNOWN);
			}
			else if (itRTVFormat->value.IsArray())
			{
				kDesc.NumRenderTargets = VE_MIN(itRTVFormat->value.Size(), 8);
				for (VeUInt32 i(0); i < kDesc.NumRenderTargets; ++i)
				{
					kDesc.RTVFormats[i] = VeTo(itRTVFormat->value[i], DXGI_FORMAT_UNKNOWN);
				}
			}
		}
		auto itDSVFormat = kConfig.FindMember("dsv_format");
		if (itDSVFormat != kConfig.MemberEnd())
		{
			if (itDSVFormat->value.IsString())
			{
				kDesc.DSVFormat = VeTo(itDSVFormat->value, DXGI_FORMAT_UNKNOWN);
			}
		}
	}

	kDesc.SampleDesc.Count = 1;

	ID3D12PipelineState* pkPipelineState(nullptr);
	if (SUCCEEDED(m_pkDevice->CreateGraphicsPipelineState(&kDesc,
		IID_PPV_ARGS(&pkPipelineState))))
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
	VeJSONValue& kConfig, VeMemoryOStream& kOut) noexcept
{
	return nullptr;
}
//--------------------------------------------------------------------------
VeRenderBufferPtr VeRendererD3D12::CreateBuffer(VeRenderBuffer::Type eType,
	VeRenderBuffer::Useage eUse, VeUInt32 u32Size) noexcept
{
	if (eType == VeRenderBuffer::TYPE_STATIC)
	{
		VeStaticBufferD3D12* pkBuffer = VE_NEW VeStaticBufferD3D12(eUse, u32Size);
		pkBuffer->Init(*this);
		return pkBuffer;
	}
	else if (eType == VeRenderBuffer::TYPE_DYNAMIC)
	{
		VeDynamicBufferD3D12* pkBuffer = VE_NEW VeDynamicBufferD3D12(eUse, u32Size);
		pkBuffer->Init(*this);
		return pkBuffer;
	}
	return nullptr;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::InitEnums() noexcept
{
	VE_ENUM(D3D12_ROOT_PARAMETER_TYPE,
	{
		{ D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, "table" },
		{ D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS, "32bit" },
		{ D3D12_ROOT_PARAMETER_TYPE_CBV, "cbv" },
		{ D3D12_ROOT_PARAMETER_TYPE_SRV, "srv" },
		{ D3D12_ROOT_PARAMETER_TYPE_UAV, "uav" }
	});

	VE_ENUM(D3D12_DESCRIPTOR_RANGE_TYPE,
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, "srv" },
		{ D3D12_DESCRIPTOR_RANGE_TYPE_UAV, "uav" },
		{ D3D12_DESCRIPTOR_RANGE_TYPE_CBV, "cbv" },
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, "sampler" }
	});

	VE_ENUM(D3D12_SHADER_VISIBILITY,
	{
		{ D3D12_SHADER_VISIBILITY_ALL, "all" },
		{ D3D12_SHADER_VISIBILITY_VERTEX, "vs" },
		{ D3D12_SHADER_VISIBILITY_HULL, "hs" },
		{ D3D12_SHADER_VISIBILITY_DOMAIN, "ds" },
		{ D3D12_SHADER_VISIBILITY_GEOMETRY, "gs" },
		{ D3D12_SHADER_VISIBILITY_PIXEL, "ps" }
	});

	VE_ENUM(D3D12_ROOT_SIGNATURE_FLAGS,
	{
		{ D3D12_ROOT_SIGNATURE_FLAG_NONE, "none" },
		{ D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, "ia" },
		{ D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS, "nvs" },
		{ D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS, "nhs" },
		{ D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS, "nds" },
		{ D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS, "ngs" },
		{ D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS, "nps" },
		{ D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT, "so" }
	});

	VE_ENUM(D3D12_FILTER,
	{
		{ D3D12_FILTER_MIN_MAG_MIP_POINT, "min_mag_mip_point" },
		{ D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR, "min_mag_point_mip_linear" },
		{ D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT, "min_point_mag_linear_mip_point" },
		{ D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR, "min_point_mag_mip_linear" },
		{ D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT, "min_linear_mag_mip_point" },
		{ D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, "min_linear_mag_point_mip_linear" },
		{ D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT, "min_mag_linear_mip_point" },
		{ D3D12_FILTER_MIN_MAG_MIP_LINEAR, "min_mag_mip_linear" },
		{ D3D12_FILTER_ANISOTROPIC, "anisotropic" },
		{ D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT, "comparison_min_mag_mip_point" },
		{ D3D12_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR, "comparison_min_mag_point_mip_linear" },
		{ D3D12_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT, "comparison_min_point_mag_linear_mip_point" },
		{ D3D12_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR, "comparison_min_point_mag_mip_linear" },
		{ D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT, "comparison_min_linear_mag_mip_point" },
		{ D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR, "comparison_min_linear_mag_point_mip_linear" },
		{ D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, "comparison_min_mag_linear_mip_point" },
		{ D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, "comparison_min_mag_mip_linear" },
		{ D3D12_FILTER_COMPARISON_ANISOTROPIC, "comparison_anisotropic" },
		{ D3D12_FILTER_MINIMUM_MIN_MAG_MIP_POINT, "minimum_min_mag_mip_point" },
		{ D3D12_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR, "minimum_min_mag_point_mip_linear" },
		{ D3D12_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT, "minimum_min_point_mag_linear_mip_point" },
		{ D3D12_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR, "minimum_min_point_mag_mip_linear" },
		{ D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT, "minimum_min_linear_mag_mip_point" },
		{ D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR, "minimum_min_linear_mag_point_mip_linear" },
		{ D3D12_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT, "minimum_min_mag_linear_mip_point" },
		{ D3D12_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR, "minimum_min_mag_mip_linear" },
		{ D3D12_FILTER_MINIMUM_ANISOTROPIC, "minimum_anisotropic" },
		{ D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_POINT, "maximum_min_mag_mip_point" },
		{ D3D12_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR, "maximum_min_mag_point_mip_linear" },
		{ D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT, "maximum_min_point_mag_linear_mip_point" },
		{ D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR, "maximum_min_point_mag_mip_linear" },
		{ D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT, "maximum_min_linear_mag_mip_point" },
		{ D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR, "maximum_min_linear_mag_point_mip_linear" },
		{ D3D12_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT, "maximum_min_mag_linear_mip_point" },
		{ D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR, "maximum_min_mag_mip_linear" },
		{ D3D12_FILTER_MAXIMUM_ANISOTROPIC, "maximum_anisotropic" }
	});

	VE_ENUM(D3D12_TEXTURE_ADDRESS_MODE,
	{
		{ D3D12_TEXTURE_ADDRESS_MODE_WRAP, "wrap" },
		{ D3D12_TEXTURE_ADDRESS_MODE_MIRROR, "mirror" },
		{ D3D12_TEXTURE_ADDRESS_MODE_CLAMP, "clamp" },
		{ D3D12_TEXTURE_ADDRESS_MODE_BORDER, "border" },
		{ D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE, "mirror_once" }
	});

	VE_ENUM(D3D12_COMPARISON_FUNC,
	{
		{ D3D12_COMPARISON_FUNC_NEVER, "never" },
		{ D3D12_COMPARISON_FUNC_LESS, "less" },
		{ D3D12_COMPARISON_FUNC_EQUAL, "equal" },
		{ D3D12_COMPARISON_FUNC_LESS_EQUAL, "less_equal" },
		{ D3D12_COMPARISON_FUNC_GREATER, "greater" },
		{ D3D12_COMPARISON_FUNC_NOT_EQUAL, "not_equal" },
		{ D3D12_COMPARISON_FUNC_GREATER_EQUAL, "greater_equal" },
		{ D3D12_COMPARISON_FUNC_ALWAYS, "always" }
	});

	VE_ENUM(D3D12_STATIC_BORDER_COLOR,
	{
		{ D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, "trans_black" },
		{ D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK, "opaque_black" },
		{ D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE, "opaque_white" }
	});

	VE_ENUM(DXGI_FORMAT,
	{
		{ DXGI_FORMAT_R32G32B32A32_TYPELESS, "r32g32b32a32_typeless" },
		{ DXGI_FORMAT_R32G32B32A32_FLOAT, "r32g32b32a32_float" },
		{ DXGI_FORMAT_R32G32B32A32_UINT, "r32g32b32a32_uint" },
		{ DXGI_FORMAT_R32G32B32A32_SINT, "r32g32b32a32_sint" },
		{ DXGI_FORMAT_R32G32B32_TYPELESS, "r32g32b32_typeless" },
		{ DXGI_FORMAT_R32G32B32_FLOAT, "r32g32b32_float" },
		{ DXGI_FORMAT_R32G32B32_UINT, "r32g32b32_uint" },
		{ DXGI_FORMAT_R32G32B32_SINT, "r32g32b32_sint" },
		{ DXGI_FORMAT_R16G16B16A16_TYPELESS, "r16g16b16a16_typeless" },
		{ DXGI_FORMAT_R16G16B16A16_FLOAT, "r16g16b16a16_float" },
		{ DXGI_FORMAT_R16G16B16A16_UNORM, "r16g16b16a16_unorm" },
		{ DXGI_FORMAT_R16G16B16A16_UINT, "r16g16b16a16_uint" },
		{ DXGI_FORMAT_R16G16B16A16_SNORM, "r16g16b16a16_snorm" },
		{ DXGI_FORMAT_R16G16B16A16_SINT, "r16g16b16a16_sint" },
		{ DXGI_FORMAT_R32G32_TYPELESS, "r32g32_typeless" },
		{ DXGI_FORMAT_R32G32_FLOAT, "r32g32_float" },
		{ DXGI_FORMAT_R32G32_UINT, "r32g32_uint" },
		{ DXGI_FORMAT_R32G32_SINT, "r32g32_sint" },
		{ DXGI_FORMAT_R32G8X24_TYPELESS, "r32g8x24_typeless" },
		{ DXGI_FORMAT_D32_FLOAT_S8X24_UINT, "d32_float_s8x24_uint" },
		{ DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS, "r32_float_x8x24_typeless" },
		{ DXGI_FORMAT_X32_TYPELESS_G8X24_UINT, "x32_typeless_g8x24_uint" },
		{ DXGI_FORMAT_R10G10B10A2_TYPELESS, "r10g10b10a2_typeless" },
		{ DXGI_FORMAT_R10G10B10A2_UNORM, "r10g10b10a2_unorm" },
		{ DXGI_FORMAT_R10G10B10A2_UINT, "r10g10b10a2_uint" },
		{ DXGI_FORMAT_R11G11B10_FLOAT, "r11g11b10_float" },
		{ DXGI_FORMAT_R8G8B8A8_TYPELESS, "r8g8b8a8_typeless" },
		{ DXGI_FORMAT_R8G8B8A8_UNORM, "r8g8b8a8_unorm" },
		{ DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, "r8g8b8a8_unorm_srgb" },
		{ DXGI_FORMAT_R8G8B8A8_UINT, "r8g8b8a8_uint" },
		{ DXGI_FORMAT_R8G8B8A8_SNORM, "r8g8b8a8_snorm" },
		{ DXGI_FORMAT_R8G8B8A8_SINT, "r8g8b8a8_sint" },
		{ DXGI_FORMAT_R16G16_TYPELESS, "r16g16_typeless" },
		{ DXGI_FORMAT_R16G16_FLOAT, "r16g16_float" },
		{ DXGI_FORMAT_R16G16_UNORM, "r16g16_unorm" },
		{ DXGI_FORMAT_R16G16_UINT, "r16g16_uint" },
		{ DXGI_FORMAT_R16G16_SNORM, "r16g16_snorm" },
		{ DXGI_FORMAT_R16G16_SINT, "r16g16_sint" },
		{ DXGI_FORMAT_R32_TYPELESS, "r32_typeless" },
		{ DXGI_FORMAT_D32_FLOAT, "d32_float" },
		{ DXGI_FORMAT_R32_FLOAT, "r32_float" },
		{ DXGI_FORMAT_R32_UINT, "r32_uint" },
		{ DXGI_FORMAT_R32_SINT, "r32_sint" },
		{ DXGI_FORMAT_R24G8_TYPELESS, "r24g8_typeless" },
		{ DXGI_FORMAT_D24_UNORM_S8_UINT, "d24_unorm_s8_uint" },
		{ DXGI_FORMAT_R24_UNORM_X8_TYPELESS, "r24_unorm_x8_typeless" },
		{ DXGI_FORMAT_X24_TYPELESS_G8_UINT, "x24_typeless_g8_uint" },
		{ DXGI_FORMAT_R8G8_TYPELESS, "r8g8_typeless" },
		{ DXGI_FORMAT_R8G8_UNORM, "r8g8_unorm" },
		{ DXGI_FORMAT_R8G8_UINT, "r8g8_uint" },
		{ DXGI_FORMAT_R8G8_SNORM, "r8g8_snorm" },
		{ DXGI_FORMAT_R8G8_SINT, "r8g8_sint" },
		{ DXGI_FORMAT_R16_TYPELESS, "r16_typeless" },
		{ DXGI_FORMAT_R16_FLOAT, "r16_float" },
		{ DXGI_FORMAT_D16_UNORM, "d16_unorm" },
		{ DXGI_FORMAT_R16_UNORM, "r16_unorm" },
		{ DXGI_FORMAT_R16_UINT, "r16_uint" },
		{ DXGI_FORMAT_R16_SNORM, "r16_snorm" },
		{ DXGI_FORMAT_R16_SINT, "r16_sint" },
		{ DXGI_FORMAT_R8_TYPELESS, "r8_typeless" },
		{ DXGI_FORMAT_R8_UNORM, "r8_unorm" },
		{ DXGI_FORMAT_R8_UINT, "r8_uint" },
		{ DXGI_FORMAT_R8_SNORM, "r8_snorm" },
		{ DXGI_FORMAT_R8_SINT, "r8_sint" },
		{ DXGI_FORMAT_A8_UNORM, "a8_unorm" },
		{ DXGI_FORMAT_R1_UNORM, "r1_unorm" },
		{ DXGI_FORMAT_R9G9B9E5_SHAREDEXP, "r9g9b9e5_sharedexp" },
		{ DXGI_FORMAT_R8G8_B8G8_UNORM, "r8g8_b8g8_unorm" },
		{ DXGI_FORMAT_G8R8_G8B8_UNORM, "g8r8_g8b8_unorm" },
		{ DXGI_FORMAT_BC1_TYPELESS, "bc1_typeless" },
		{ DXGI_FORMAT_BC1_UNORM, "bc1_unorm" },
		{ DXGI_FORMAT_BC1_UNORM_SRGB, "bc1_unorm_srgb" },
		{ DXGI_FORMAT_BC2_TYPELESS, "bc2_typeless" },
		{ DXGI_FORMAT_BC2_UNORM, "bc2_unorm" },
		{ DXGI_FORMAT_BC2_UNORM_SRGB, "bc2_unorm_srgb" },
		{ DXGI_FORMAT_BC3_TYPELESS, "bc3_typeless" },
		{ DXGI_FORMAT_BC3_UNORM, "bc3_unorm" },
		{ DXGI_FORMAT_BC3_UNORM_SRGB, "bc3_unorm_srgb" },
		{ DXGI_FORMAT_BC4_TYPELESS, "bc4_typeless" },
		{ DXGI_FORMAT_BC4_UNORM, "bc4_unorm" },
		{ DXGI_FORMAT_BC4_SNORM, "bc4_snorm" },
		{ DXGI_FORMAT_BC5_TYPELESS, "bc5_typeless" },
		{ DXGI_FORMAT_BC5_UNORM, "bc5_unorm" },
		{ DXGI_FORMAT_BC5_SNORM, "bc5_snorm" },
		{ DXGI_FORMAT_B5G6R5_UNORM, "b5g6r5_unorm" },
		{ DXGI_FORMAT_B5G5R5A1_UNORM, "b5g5r5a1_unorm" },
		{ DXGI_FORMAT_B8G8R8A8_UNORM, "b8g8r8a8_unorm" },
		{ DXGI_FORMAT_B8G8R8X8_UNORM, "b8g8r8x8_unorm" },
		{ DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM, "r10g10b10_xr_bias_a2_unorm" },
		{ DXGI_FORMAT_B8G8R8A8_TYPELESS, "b8g8r8a8_typeless" },
		{ DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, "b8g8r8a8_unorm_srgb" },
		{ DXGI_FORMAT_B8G8R8X8_TYPELESS, "b8g8r8x8_typeless" },
		{ DXGI_FORMAT_B8G8R8X8_UNORM_SRGB, "b8g8r8x8_unorm_srgb" },
		{ DXGI_FORMAT_BC6H_TYPELESS, "bc6h_typeless" },
		{ DXGI_FORMAT_BC6H_UF16, "bc6h_uf16" },
		{ DXGI_FORMAT_BC6H_SF16, "bc6h_sf16" },
		{ DXGI_FORMAT_BC7_TYPELESS, "bc7_typeless" },
		{ DXGI_FORMAT_BC7_UNORM, "bc7_unorm" },
		{ DXGI_FORMAT_BC7_UNORM_SRGB, "bc7_unorm_srgb" },
		{ DXGI_FORMAT_AYUV, "ayuv" },
		{ DXGI_FORMAT_Y410, "y410" },
		{ DXGI_FORMAT_Y416, "y416" },
		{ DXGI_FORMAT_NV12, "nv12" },
		{ DXGI_FORMAT_P010, "p010" },
		{ DXGI_FORMAT_P016, "p016" },
		{ DXGI_FORMAT_420_OPAQUE, "420_opaque" },
		{ DXGI_FORMAT_YUY2, "yuy2" },
		{ DXGI_FORMAT_Y210, "y210" },
		{ DXGI_FORMAT_Y216, "y216" },
		{ DXGI_FORMAT_NV11, "nv11" },
		{ DXGI_FORMAT_AI44, "ai44" },
		{ DXGI_FORMAT_IA44, "ia44" },
		{ DXGI_FORMAT_P8, "p8" },
		{ DXGI_FORMAT_A8P8, "a8p8" },
		{ DXGI_FORMAT_B4G4R4A4_UNORM, "b4g4r4a4_unorm" },
		{ DXGI_FORMAT_P208, "p208" },
		{ DXGI_FORMAT_V208, "v208" },
		{ DXGI_FORMAT_V408, "v408" }
	});

	VE_ENUM(D3D12_INPUT_CLASSIFICATION,
	{
		{ D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, "vertex" },
		{ D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, "instance" }
	});

	VE_ENUM(D3D12_BLEND,
	{
		{ D3D12_BLEND_ZERO, "zero" },
		{ D3D12_BLEND_ONE, "one" },
		{ D3D12_BLEND_SRC_COLOR, "src_color" },
		{ D3D12_BLEND_INV_SRC_COLOR, "inv_src_color" },
		{ D3D12_BLEND_SRC_ALPHA, "src_alpha" },
		{ D3D12_BLEND_INV_SRC_ALPHA, "inv_src_alpha" },
		{ D3D12_BLEND_DEST_ALPHA, "dest_alpha" },
		{ D3D12_BLEND_INV_DEST_ALPHA, "inv_dest_alpha" },
		{ D3D12_BLEND_DEST_COLOR, "dest_color" },
		{ D3D12_BLEND_INV_DEST_COLOR, "inv_dest_color" },
		{ D3D12_BLEND_SRC_ALPHA_SAT, "src_alpha_sat" },
		{ D3D12_BLEND_BLEND_FACTOR, "blend_factor" },
		{ D3D12_BLEND_INV_BLEND_FACTOR, "inv_blend_factor" },
		{ D3D12_BLEND_SRC1_COLOR, "src1_color" },
		{ D3D12_BLEND_INV_SRC1_COLOR, "inv_src1_color" },
		{ D3D12_BLEND_SRC1_ALPHA, "src1_alpha" },
		{ D3D12_BLEND_INV_SRC1_ALPHA, "inv_src1_alpha" }		
	});

	VE_ENUM(D3D12_BLEND_OP,
	{
		{ D3D12_BLEND_OP_ADD, "add" },
		{ D3D12_BLEND_OP_SUBTRACT, "subtract" },
		{ D3D12_BLEND_OP_REV_SUBTRACT, "rev_subtract" },
		{ D3D12_BLEND_OP_MIN, "min" },
		{ D3D12_BLEND_OP_MAX, "max" }
	});

	VE_ENUM(D3D12_LOGIC_OP,
	{
		{ D3D12_LOGIC_OP_CLEAR, "clear" },
		{ D3D12_LOGIC_OP_SET, "set" },
		{ D3D12_LOGIC_OP_COPY, "copy" },
		{ D3D12_LOGIC_OP_COPY_INVERTED, "copy_inverted" },
		{ D3D12_LOGIC_OP_NOOP, "noop" },
		{ D3D12_LOGIC_OP_INVERT, "invert" },
		{ D3D12_LOGIC_OP_AND, "and" },
		{ D3D12_LOGIC_OP_NAND, "nand" },
		{ D3D12_LOGIC_OP_OR, "or" },
		{ D3D12_LOGIC_OP_NOR, "nor" },
		{ D3D12_LOGIC_OP_XOR, "xor" },
		{ D3D12_LOGIC_OP_EQUIV, "equiv" },
		{ D3D12_LOGIC_OP_AND_REVERSE, "and_reverse" },
		{ D3D12_LOGIC_OP_AND_INVERTED, "and_inverted" },
		{ D3D12_LOGIC_OP_OR_REVERSE, "or_reverse" },
		{ D3D12_LOGIC_OP_OR_INVERTED, "or_inverted" }
	});

	VE_ENUM(D3D12_COLOR_WRITE_ENABLE,
	{
		{ D3D12_COLOR_WRITE_ENABLE_RED, "r" },
		{ D3D12_COLOR_WRITE_ENABLE_GREEN, "g" },
		{ D3D12_COLOR_WRITE_ENABLE_BLUE, "b" },
		{ D3D12_COLOR_WRITE_ENABLE_ALPHA, "all" }
	});

	VE_ENUM(D3D12_FILL_MODE,
	{
		{ D3D12_FILL_MODE_WIREFRAME, "wireframe" },
		{ D3D12_FILL_MODE_SOLID, "solid" }
	});

	VE_ENUM(D3D12_CULL_MODE,
	{
		{ D3D12_CULL_MODE_NONE, "none" },
		{ D3D12_CULL_MODE_FRONT, "front" },
		{ D3D12_CULL_MODE_BACK, "back" }
	});

	VE_ENUM(D3D12_STENCIL_OP,
	{
		{ D3D12_STENCIL_OP_KEEP, "keep" },
		{ D3D12_STENCIL_OP_ZERO, "zero" },
		{ D3D12_STENCIL_OP_REPLACE, "replace" },
		{ D3D12_STENCIL_OP_INCR_SAT, "inc_sat" },
		{ D3D12_STENCIL_OP_DECR_SAT, "dec_sat" },
		{ D3D12_STENCIL_OP_INVERT, "invert" },
		{ D3D12_STENCIL_OP_INCR, "inc" },
		{ D3D12_STENCIL_OP_DECR, "dec" }
	});

	VE_ENUM(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE,
	{
		{ D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED, "disable" },
		{ D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF, "16bit" },
		{ D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF, "32bit" }
	});

	VE_ENUM(D3D12_PRIMITIVE_TOPOLOGY_TYPE,
	{
		{ D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED, "undef" },
		{ D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, "point" },
		{ D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, "line" },
		{ D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, "triangle" },
		{ D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH, "patch" }
	});
}
//--------------------------------------------------------------------------
void VeRendererD3D12::InitCopyQueue() noexcept
{
	D3D12_COMMAND_QUEUE_DESC kDesc =
	{
		D3D12_COMMAND_LIST_TYPE_COPY,
		D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
		D3D12_COMMAND_QUEUE_FLAG_NONE
	};
	VE_ASSERT_GE(m_pkDevice->CreateCommandQueue(&kDesc,
		IID_PPV_ARGS(&m_pkCopyQueue)), S_OK);
	VE_ASSERT_GE(m_pkDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS(&m_pkCopyAllocator)), S_OK);
	VE_ASSERT_GE(m_pkDevice->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_COPY, m_pkCopyAllocator, nullptr,
		IID_PPV_ARGS(&m_pkCopyCommandList)), S_OK);
	VE_ASSERT_GE(m_pkDevice->CreateFence(m_u64CopyFenceValue++,
		D3D12_FENCE_FLAG_SHARED, IID_PPV_ARGS(&m_pkCopyFence)), S_OK);
	m_kCopyFenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	VE_ASSERT(m_kCopyFenceEvent);
	VE_ASSERT_GE(m_pkCopyQueue->Signal(m_pkCopyFence, m_u64CopyFenceValue), S_OK);
	VE_ASSERT_GE(m_pkCopyFence->SetEventOnCompletion(m_u64CopyFenceValue, m_kCopyFenceEvent), S_OK);
	WaitForSingleObject(m_kCopyFenceEvent, INFINITE);
}
//--------------------------------------------------------------------------
void VeRendererD3D12::TermCopyQueue() noexcept
{
	const VeUInt64 u64LastCompleted = m_pkCopyFence->GetCompletedValue();
	VE_ASSERT_GE(m_pkCopyQueue->Signal(m_pkCopyFence, ++m_u64CopyFenceValue), S_OK);
	if (u64LastCompleted < m_u64CopyFenceValue)
	{
		VE_ASSERT_GE(m_pkCopyFence->SetEventOnCompletion(
			m_u64CopyFenceValue, m_kCopyFenceEvent), S_OK);
		WaitForSingleObject(m_kCopyFenceEvent, INFINITE);
		for (auto res : m_kCopyResList)
		{
			res->Release();
		}
		m_kCopyResList.clear();
	}
	CloseHandle(m_kCopyFenceEvent);
	VE_SAFE_RELEASE(m_pkCopyFence);
	VE_SAFE_RELEASE(m_pkCopyCommandList);
	VE_SAFE_RELEASE(m_pkCopyAllocator);
	VE_SAFE_RELEASE(m_pkCopyQueue);
}
//--------------------------------------------------------------------------
VeRenderer::ShaderType VeRendererD3D12::GetTargetType(
	const VeChar8* pcTarget) noexcept
{
	switch (VeToLower(*pcTarget))
	{
	case 'v':
		return TYPE_VS;
	case 'p':
		return TYPE_PS;
	case 'g':
		return TYPE_GS;
	case 'h':
		return TYPE_HS;
	case 'd':
		return TYPE_DS;
	case 'c':
		return TYPE_CS;
	default:
		return SHADER_TYPE_NUM;
	}
}
//--------------------------------------------------------------------------
VeRendererPtr CreateRendererD3D12() noexcept
{
	return VE_NEW VeRendererD3D12();
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
