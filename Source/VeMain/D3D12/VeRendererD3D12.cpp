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
#include "VeRenderWindowD3D12.h"
#include "VeRenderBufferD3D12.h"
#include "VeRenderTextureD3D12.h"

//--------------------------------------------------------------------------
#ifdef VE_ENABLE_D3D12
//--------------------------------------------------------------------------
VeRTTIImpl(VeRendererD3D12, VeRenderer);
//--------------------------------------------------------------------------
VeRTTIImpl(VeRendererD3D12::RootSignatureD3D12, VeRenderer::RootSignature);
//--------------------------------------------------------------------------
VeRTTIImpl(VeRendererD3D12::PipelineStateD3D12, VeRenderer::PipelineState);
//--------------------------------------------------------------------------
VeRTTIImpl(VeRendererD3D12::GeometryD3D12, VeRenderer::Geometry);
//--------------------------------------------------------------------------
VeRTTIImpl(VeRendererD3D12::BindingD3D12, VeRenderer::Binding);
//--------------------------------------------------------------------------
VeRendererD3D12::VeRendererD3D12() noexcept
	: VeRenderer(API_D3D12)
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
	InitStartup();

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
	for (auto obj : m_kRenderTextureList)
	{
		obj->Term();
	}
	VE_ASSERT(m_kRenderTextureList.empty());
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
	TermStartup();
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

	STDMETHOD(Open)(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName,
		LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) override
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

	STDMETHOD(Close)(THIS_ LPCVOID pData) override
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
						VeFree((void*)kPara.DescriptorTable.pDescriptorRanges);
					}
				}
				VeFree(m_pkParameter);
				m_pkParameter = nullptr;
			}

			if (m_pkSampler && m_u32SamplersNum)
			{
				VeFree(m_pkSampler);
				m_pkSampler = nullptr;
			}
		}

		void SetParameterNum(VeUInt32 u32Num) noexcept
		{
			VE_ASSERT(m_u32ParametersNum == 0 && m_pkParameter == nullptr);
			m_u32ParametersNum = u32Num;
			m_pkParameter = VeAlloc(D3D12_ROOT_PARAMETER, u32Num);
			VeZeroMemory(m_pkParameter, sizeof(D3D12_ROOT_PARAMETER) * u32Num);
		}

		D3D12_DESCRIPTOR_RANGE* SetRangeNum(VeUInt32 u32Index, VeUInt32 u32Num) noexcept
		{
			VE_ASSERT(u32Index < m_u32ParametersNum && (m_pkParameter[u32Index].ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE));
			m_pkParameter[u32Index].DescriptorTable.NumDescriptorRanges = u32Num;
			D3D12_DESCRIPTOR_RANGE* pvRange = VeAlloc(D3D12_DESCRIPTOR_RANGE, u32Num);
			VeZeroMemory(pvRange, sizeof(D3D12_DESCRIPTOR_RANGE) * u32Num);
			m_pkParameter[u32Index].DescriptorTable.pDescriptorRanges = pvRange;
			return pvRange;
		}

		void SetSamplerNum(VeUInt32 u32Num) noexcept
		{
			VE_ASSERT(m_u32SamplersNum == 0 && m_pkSampler == nullptr);
			m_u32SamplersNum = u32Num;
			m_pkSampler = VeAlloc(D3D12_STATIC_SAMPLER_DESC, u32Num);
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
						kRange.OffsetInDescriptorsFromTableStart = kValueRange("offset", D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);
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
	VeRenderer::BlendType eType) noexcept
{
	kDesc.AlphaToCoverageEnable = FALSE;
	kDesc.IndependentBlendEnable = FALSE;
	switch (eType)
	{
	case VeRenderer::ADD:
		kDesc.RenderTarget[0] =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		break;
	case VeRenderer::BLEND:
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
	VeRenderer::RasterType eType) noexcept
{
	switch (eType)
	{
	case VeRenderer::CULL_FRONT:
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
	case VeRenderer::CULL_NONE:
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
	VeRenderer::DepthStencilType eType) noexcept
{
	switch (eType)
	{
	case VeRenderer::DS_STANDARD:
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
		RootSignatureD3D12* pkRootSig = VeSmartPointerCast(RootSignatureD3D12, it->second);
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
			kElement.Format = (DXGI_FORMAT)kInputValue("format", VeRenderResource::FORMAT_UNKNOWN);
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
				kDesc.RTVFormats[0] = (DXGI_FORMAT)VeTo(itRTVFormat->value, VeRenderResource::FORMAT_UNKNOWN);
			}
			else if (itRTVFormat->value.IsArray())
			{
				kDesc.NumRenderTargets = VE_MIN(itRTVFormat->value.Size(), 8);
				for (VeUInt32 i(0); i < kDesc.NumRenderTargets; ++i)
				{
					kDesc.RTVFormats[i] = (DXGI_FORMAT)VeTo(itRTVFormat->value[i], VeRenderResource::FORMAT_UNKNOWN);
				}
			}
		}
		auto itDSVFormat = kConfig.FindMember("dsv_format");
		if (itDSVFormat != kConfig.MemberEnd())
		{
			if (itDSVFormat->value.IsString())
			{
				kDesc.DSVFormat = (DXGI_FORMAT)VeTo(itDSVFormat->value, VeRenderResource::FORMAT_UNKNOWN);
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
VeRendererD3D12::GeometryPtr VeRendererD3D12::CreateGeometry() noexcept
{
	return VE_NEW GeometryD3D12();
}
//--------------------------------------------------------------------------
VeRendererD3D12::BindingPtr VeRendererD3D12::CreateBinding() noexcept
{
	return VE_NEW BindingD3D12();
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
VeRenderTexturePtr VeRendererD3D12::CreateTexture(
	VeRenderResource::Dimension eDim, VeRenderTexture::Useage eUse,
	VeRenderResource::Format eFormat, VeUInt32 u32Width, VeUInt32 u32Height,
	VeUInt16 u16Depth, VeUInt16 u16MipLevels,
	VeUInt16 u16Count, VeUInt16 u16Quality) noexcept
{
	VeRenderTextureD3D12* pkTexture = VE_NEW VeRenderTextureD3D12(eDim,
		eUse, eFormat, u32Width, u32Height, u16Depth, u16MipLevels,
		u16Count, u16Quality);
	pkTexture->Init(*this);
	return pkTexture;
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
void VeRendererD3D12::InitStartup() noexcept
{
	VeUInt16 av3QuadData[] =
	{
		0, 0,
		VE_UINT16_MAX, 0,
		0, VE_UINT16_MAX,
		VE_UINT16_MAX, VE_UINT16_MAX
	};

	m_spQuadBuffer = VE_NEW VeStaticBufferD3D12(
		VeRenderBuffer::USEAGE_VB, sizeof(av3QuadData));
	m_spQuadBuffer->Init(*this);

	BeginSyncCopy();
	m_spQuadBuffer->UpdateSync(av3QuadData, sizeof(av3QuadData));
	EndSyncCopy();

	m_kQuadVBV.BufferLocation = m_spQuadBuffer->m_pkResource->GetGPUVirtualAddress();
	m_kQuadVBV.SizeInBytes = m_spQuadBuffer->GetSize();
	m_kQuadVBV.StrideInBytes = sizeof(VeUInt16) * 2;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::TermStartup() noexcept
{
	m_spQuadBuffer = nullptr;
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
void VeRendererD3D12::BindingD3D12::ClearTableList() noexcept
{
	m_kTableList.clear();
}
//--------------------------------------------------------------------------
void VeRendererD3D12::BindingD3D12::AddContextIndex(
	VeUInt32 u32Index, VeSizeT stContextIndex) noexcept
{
	m_kTableList.resize(m_kTableList.size() + 1);
	m_kTableList.back().m_eType = TYPE_CONTEXT_INDEX;
	m_kTableList.back().m_u32Index = u32Index;
	m_kTableList.back().m_stContextIndex = stContextIndex;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::BindingD3D12::AddCustomer(
	VeUInt32 u32Index, VeSizeT stCustomerIndex) noexcept
{
	m_kTableList.resize(m_kTableList.size() + 1);
	m_kTableList.back().m_eType = TYPE_CUSTOMER_INDEX;
	m_kTableList.back().m_u32Index = u32Index;
	m_kTableList.back().m_stCustomerIndex = stCustomerIndex;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::BindingD3D12::AddResource(VeUInt32 u32Index,
	const VeRenderResourcePtr& spResource) noexcept
{
	VE_ASSERT(spResource);
	{
		VeStaticBufferD3D12* pkObj = VeDynamicCast(VeStaticBufferD3D12, spResource.p());
		if (pkObj && pkObj->CanUse(VeRenderBuffer::USEAGE_CB))
		{
			m_kTableList.resize(m_kTableList.size() + 1);
			m_kTableList.back().m_eType = TYPE_STATIC_HANDLE;
			m_kTableList.back().m_u32Index = u32Index;
			m_kTableList.back().m_hHandle = pkObj->m_pkCBView->m_kGPUHandle;
			return;
		}
	}
	{
		VeDynamicBufferD3D12* pkObj = VeDynamicCast(VeDynamicBufferD3D12, spResource.p());
		if (pkObj && pkObj->CanUse(VeRenderBuffer::USEAGE_CB))
		{
			m_kTableList.resize(m_kTableList.size() + 1);
			m_kTableList.back().m_eType = TYPE_DYANMIC_BUFFER;
			m_kTableList.back().m_u32Index = u32Index;
			m_kTableList.back().m_pvDynamicObject = pkObj;
			return;
		}
	}
}
//--------------------------------------------------------------------------
bool VeRendererD3D12::GeometryD3D12::IsValid() noexcept
{
	return m_kVBVList.size()
		&& m_eTopology > D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED
		&& m_eTopology <= D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::GeometryD3D12::SetPrimitiveTopologyType(
	PrimitiveTopologyType eType) noexcept
{
	m_eTopology = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)eType;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::GeometryD3D12::SetVertexBufferNum(
	VeUInt32 u32Num) noexcept
{
	m_kVBVList.resize(u32Num);
}
//--------------------------------------------------------------------------
void VeRendererD3D12::GeometryD3D12::SetVertexBuffer(VeUInt32 u32Index,
	const VeRenderBufferPtr& spBuffer, VeUInt32 u32Stride) noexcept
{
	VE_ASSERT(u32Index < m_kVBVList.size() && spBuffer
		&& spBuffer->CanUse(VeRenderBuffer::USEAGE_VB));
	D3D12_VERTEX_BUFFER_VIEW& kView = m_kVBVList[u32Index];
	VeRenderBufferD3D12& kBuffer = (VeRenderBufferD3D12&)*spBuffer;
	kView.BufferLocation = kBuffer.m_pkResource->GetGPUVirtualAddress();
	kView.SizeInBytes = kBuffer.GetSize();
	kView.StrideInBytes = u32Stride;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::GeometryD3D12::SetVertexBuffer(VeUInt32 u32Index,
	const VeRenderBufferPtr& spBuffer, VeUInt32 u32Offset,
	VeUInt32 u32Size, VeUInt32 u32Stride) noexcept
{
	VE_ASSERT(u32Index < m_kVBVList.size() && spBuffer
		&& spBuffer->CanUse(VeRenderBuffer::USEAGE_VB));
	D3D12_VERTEX_BUFFER_VIEW& kView = m_kVBVList[u32Index];
	VeRenderBufferD3D12& kBuffer = (VeRenderBufferD3D12&)*spBuffer;
	kView.BufferLocation = kBuffer.m_pkResource->GetGPUVirtualAddress() + u32Offset;
	kView.SizeInBytes = u32Size;
	kView.StrideInBytes = u32Stride;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::GeometryD3D12::ClearIndexBuffer() noexcept
{
	m_kIBV = {};
}
//--------------------------------------------------------------------------
void VeRendererD3D12::GeometryD3D12::SetIndexBuffer(
	const VeRenderBufferPtr& spBuffer, bool bUse32Bit) noexcept
{
	VE_ASSERT(spBuffer && spBuffer->CanUse(VeRenderBuffer::USEAGE_IB));
	VeRenderBufferD3D12& kBuffer = (VeRenderBufferD3D12&)*spBuffer;
	m_kIBV.BufferLocation = kBuffer.m_pkResource->GetGPUVirtualAddress();
	m_kIBV.SizeInBytes = spBuffer->GetSize();
	m_kIBV.Format = bUse32Bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::GeometryD3D12::SetIndexBuffer(
	const VeRenderBufferPtr& spBuffer, VeUInt32 u32Offset,
	VeUInt32 u32Size, bool bUse32Bit) noexcept
{
	VE_ASSERT(spBuffer && spBuffer->CanUse(VeRenderBuffer::USEAGE_IB));
	VeRenderBufferD3D12& kBuffer = (VeRenderBufferD3D12&)*spBuffer;
	m_kIBV.BufferLocation = kBuffer.m_pkResource->GetGPUVirtualAddress() + u32Offset;
	m_kIBV.SizeInBytes = u32Size;
	m_kIBV.Format = bUse32Bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
}
//--------------------------------------------------------------------------
VeRendererPtr CreateRendererD3D12() noexcept
{
	return VE_NEW VeRendererD3D12();
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
