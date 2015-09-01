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

	}
	kStackBuffer;
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
			auto itType = kValue.FindMember("type");
			if (itType == kValue.MemberEnd()) return nullptr;
			if (!itType->value.IsString()) return nullptr;
			auto itTypeEnum = m_kRootParameterTypeParser.find(itType->value.GetString());
			if (itTypeEnum == m_kRootParameterTypeParser.end()) return nullptr;
			kParameter.ParameterType = itTypeEnum->second;
			switch (kParameter.ParameterType)
			{
			case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
			{
				auto itRange = kValue.FindMember("ranges");
				if (itRange == kValue.MemberEnd()) return nullptr;
				if (!itRange->value.IsArray()) return nullptr;
				if (itRange->value.Empty()) return nullptr;
				D3D12_DESCRIPTOR_RANGE* pvRange = kStackBuffer.SetRangeNum(i, itRange->value.Size());
				for (VeUInt32 i(0); i < itRange->value.Size(); ++i)
				{
					D3D12_DESCRIPTOR_RANGE& kRange = pvRange[i];
					VeJSONValue& kValueRange = itRange->value[i];
					if (!kValueRange.IsObject()) return nullptr;
					auto itRangeType = kValueRange.FindMember("type");
					if (itRangeType == kValueRange.MemberEnd()) return nullptr;
					if (!itRangeType->value.IsString()) return nullptr;
					auto itRangeTypeEnum = m_kDescriptorRangeTypeParser.find(itRangeType->value.GetString());
					if (itRangeTypeEnum == m_kDescriptorRangeTypeParser.end()) return nullptr;
					kRange.RangeType = itRangeTypeEnum->second;

					auto itNum = kValueRange.FindMember("num");
					if (itNum == kValueRange.MemberEnd()) return nullptr;
					if (!itNum->value.IsNumber()) return nullptr;
					kRange.NumDescriptors = itNum->value.GetUint();

					auto itReg = kValueRange.FindMember("register");
					if (itReg == kValueRange.MemberEnd()) return nullptr;
					if (!itReg->value.IsNumber()) return nullptr;
					kRange.BaseShaderRegister = itReg->value.GetUint();

					auto itSpace = kValueRange.FindMember("space");
					if (itSpace == kValueRange.MemberEnd()) return nullptr;
					if (!itSpace->value.IsNumber()) return nullptr;
					kRange.RegisterSpace = itSpace->value.GetUint();

					auto itOffset = kValueRange.FindMember("offset");
					if (itOffset == kValueRange.MemberEnd()) return nullptr;
					if (!itOffset->value.IsNumber()) return nullptr;
					kRange.OffsetInDescriptorsFromTableStart = itOffset->value.GetUint();
				}
				auto itVis = kValue.FindMember("visibility");
				if (itVis == kValue.MemberEnd()) return nullptr;
				if (!itVis->value.IsString()) return nullptr;
				auto itVisEnum = m_kShaderVisibilityParser.find(itVis->value.GetString());
				if (itVisEnum == m_kShaderVisibilityParser.end()) return nullptr;
				kParameter.ShaderVisibility = itVisEnum->second;
			}
			break;
			case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
			{
				auto itNum = kValue.FindMember("num");
				if (itNum == kValue.MemberEnd()) return nullptr;
				if (!itNum->value.IsNumber()) return nullptr;
				kParameter.Constants.Num32BitValues = itNum->value.GetUint();

				auto itReg = kValue.FindMember("register");
				if (itReg == kValue.MemberEnd()) return nullptr;
				if (!itReg->value.IsNumber()) return nullptr;
				kParameter.Constants.ShaderRegister = itReg->value.GetUint();

				auto itSpace = kValue.FindMember("space");
				if (itSpace == kValue.MemberEnd()) return nullptr;
				if (!itSpace->value.IsNumber()) return nullptr;
				kParameter.Constants.RegisterSpace = itSpace->value.GetUint();
			}
			break;
			case D3D12_ROOT_PARAMETER_TYPE_CBV:
			case D3D12_ROOT_PARAMETER_TYPE_SRV:
			case D3D12_ROOT_PARAMETER_TYPE_UAV:
			{
				auto itReg = kValue.FindMember("register");
				if (itReg == kValue.MemberEnd()) return nullptr;
				if (!itReg->value.IsNumber()) return nullptr;
				kParameter.Descriptor.ShaderRegister = itReg->value.GetUint();

				auto itSpace = kValue.FindMember("space");
				if (itSpace == kValue.MemberEnd()) return nullptr;
				if (!itSpace->value.IsNumber()) return nullptr;
				kParameter.Descriptor.RegisterSpace = itSpace->value.GetUint();
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

			auto itFilter = kValue.FindMember("filter");
			if (itFilter != kValue.MemberEnd())
			{
				if (!itFilter->value.IsString()) return nullptr;
				auto itFilterEnum = m_kFilterParser.find(itFilter->value.GetString());
				if (itFilterEnum == m_kFilterParser.end()) return nullptr;
				kSampler.Filter = itFilterEnum->second;
			}
			else
			{
				kSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			}

			auto itAddU = kValue.FindMember("addU");
			if (itAddU != kValue.MemberEnd())
			{
				if (!itAddU->value.IsString()) return nullptr;
				auto itAddUEnum = m_kTexAddressModeParser.find(itAddU->value.GetString());
				if (itAddUEnum == m_kTexAddressModeParser.end()) return nullptr;
				kSampler.AddressU = itAddUEnum->second;
			}
			else
			{
				kSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			}
			
			auto itAddV = kValue.FindMember("addV");
			if (itAddV != kValue.MemberEnd())
			{
				if (!itAddV->value.IsString()) return nullptr;
				auto itAddVEnum = m_kTexAddressModeParser.find(itAddV->value.GetString());
				if (itAddVEnum == m_kTexAddressModeParser.end()) return nullptr;
				kSampler.AddressV = itAddVEnum->second;
			}
			else
			{
				kSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			}
			

			auto itAddW = kValue.FindMember("addW");
			if (itAddW != kValue.MemberEnd())
			{
				if (!itAddW->value.IsString()) return nullptr;
				auto itAddWEnum = m_kTexAddressModeParser.find(itAddW->value.GetString());
				if (itAddWEnum == m_kTexAddressModeParser.end()) return nullptr;
				kSampler.AddressW = itAddWEnum->second;
			}
			else
			{
				kSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			}
			

			auto itLodBias = kValue.FindMember("lod_bias");
			if (itLodBias != kValue.MemberEnd())
			{
				if (!itLodBias->value.IsNumber()) return nullptr;
				kSampler.MipLODBias = itLodBias->value.GetDouble();
			}
			else
			{
				kSampler.MipLODBias = 0;
			}
			
			
			auto itMaxAni = kValue.FindMember("max_ani");
			if (itMaxAni != kValue.MemberEnd())
			{
				if (!itMaxAni->value.IsNumber()) return nullptr;
				kSampler.MaxAnisotropy = itMaxAni->value.GetUint();
			}
			else
			{
				kSampler.MaxAnisotropy = 0;
			}
			
			auto itCompFunc = kValue.FindMember("comp_func");
			if (itCompFunc != kValue.MemberEnd())
			{
				if (!itCompFunc->value.IsString()) return nullptr;
				auto itCompFuncEnum = m_kComparisonFuncParser.find(itCompFunc->value.GetString());
				if (itCompFuncEnum == m_kComparisonFuncParser.end()) return nullptr;
				kSampler.ComparisonFunc = itCompFuncEnum->second;
			}
			else
			{
				kSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			}
			

			auto itBorderColor = kValue.FindMember("border_color");
			if (itBorderColor != kValue.MemberEnd())
			{
				if (!itBorderColor->value.IsString()) return nullptr;
				auto itBorderColorEnum = m_kStaticBorderColorParser.find(itBorderColor->value.GetString());
				if (itBorderColorEnum == m_kStaticBorderColorParser.end()) return nullptr;
				kSampler.BorderColor = itBorderColorEnum->second;
			}
			else
			{
				kSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			}
			

			auto itMinLod = kValue.FindMember("min_lod");
			if (itMinLod != kValue.MemberEnd())
			{
				if (!itMinLod->value.IsNumber()) return nullptr;
				kSampler.MinLOD = itMinLod->value.GetDouble();
			}
			else
			{
				kSampler.MinLOD = 0;
			}

			auto itMaxLod = kValue.FindMember("max_lod");
			if (itMaxLod != kValue.MemberEnd())
			{
				if (!itMaxLod->value.IsNumber()) return nullptr;
				kSampler.MaxLOD = itMaxLod->value.GetDouble();
			}
			else
			{
				kSampler.MaxLOD = D3D12_FLOAT32_MAX;
			}			

			auto itRegister = kValue.FindMember("register");
			if (itRegister == kValue.MemberEnd()) return nullptr;
			if (!itRegister->value.IsNumber()) return nullptr;
			kSampler.ShaderRegister = itRegister->value.GetUint();

			auto itSpace = kValue.FindMember("space");
			if (itSpace == kValue.MemberEnd()) return nullptr;
			if (!itSpace->value.IsNumber()) return nullptr;
			kSampler.RegisterSpace = itSpace->value.GetUint();

			auto itVis = kValue.FindMember("visibility");
			if (itVis == kValue.MemberEnd()) return nullptr;
			if (!itVis->value.IsString()) return nullptr;
			auto itVisEnum = m_kShaderVisibilityParser.find(itVis->value.GetString());
			if (itVisEnum == m_kShaderVisibilityParser.end()) return nullptr;
			kSampler.ShaderVisibility = itVisEnum->second;
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
void VeRendererD3D12::InitParsers() noexcept
{
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
}
//--------------------------------------------------------------------------
void VeRendererD3D12::TermParsers() noexcept
{
	m_kRootParameterTypeParser.clear();
	m_kDescriptorRangeTypeParser.clear();
	m_kShaderVisibilityParser.clear();
	m_kRootSignatureFlagsParser.clear();
	m_kFilterParser.clear();
	m_kTexAddressModeParser.clear();
	m_kComparisonFuncParser.clear();
	m_kStaticBorderColorParser.clear();
}
//--------------------------------------------------------------------------
VeRendererPtr CreateRendererD3D12() noexcept
{
	return VE_NEW VeRendererD3D12();
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
