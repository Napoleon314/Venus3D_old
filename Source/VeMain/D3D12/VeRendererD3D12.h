////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRendererD3D12.h
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef VE_ENABLE_D3D12

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

#define D3D_COMPLIER "d3dcompiler_47"

class VeRendererD3D12 : public VeRenderer
{
	VeNoCopy(VeRendererD3D12);
	VeRTTIDecl(VeRendererD3D12, VeRenderer);
public:
	static constexpr VeUInt32 FRAME_COUNT = 3;
	static constexpr VeUInt32 RTV_COUNT = 32;
	static constexpr VeUInt32 DSV_COUNT = 1;

	enum BlendType
	{
		REPLACE,
		ADD,
		BLEND
	};

	enum RasterType
	{
		CULL_BACK,
		CULL_FRONT,
		CULL_NONE
	};

	enum DepthStencilType
	{
		DS_NONE,
		DS_STANDARD
	};

	class RootSignatureD3D12 : public RootSignature
	{
		VeNoCopy(RootSignatureD3D12);
		VeRTTIDecl(RootSignatureD3D12, RootSignature);
	public:
		RootSignatureD3D12(ID3D12RootSignature* pkRootSignature) noexcept
			: m_pkRootSignature(pkRootSignature)
		{
			m_kNode.m_Content = this;
		}

		virtual ~RootSignatureD3D12() noexcept
		{
			VE_SAFE_RELEASE(m_pkRootSignature);
		}

		VeRefNode<RootSignatureD3D12*> m_kNode;
		ID3D12RootSignature* m_pkRootSignature = nullptr;

	};

	class PipelineStateD3D12 : public PipelineState
	{
		VeNoCopy(PipelineStateD3D12);
		VeRTTIDecl(PipelineStateD3D12, PipelineState);
	public:
		PipelineStateD3D12(ID3D12PipelineState* pkState) noexcept
			: m_pkPipelineState(pkState)
		{
			m_kNode.m_Content = this;
		}

		virtual ~PipelineStateD3D12() noexcept
		{
			VE_SAFE_RELEASE(m_pkPipelineState);
		}

		VeRefNode<PipelineStateD3D12*> m_kNode;
		ID3D12PipelineState* m_pkPipelineState = nullptr;

	};

	template <D3D12_DESCRIPTOR_HEAP_TYPE TYPE, VeUInt32 NUM, D3D12_DESCRIPTOR_HEAP_FLAGS FLAGS>
	class DescriptorHeapShell
	{
	public:
		DescriptorHeapShell() noexcept = default;

		void Init(ID3D12Device* pkDevice) noexcept
		{
			D3D12_DESCRIPTOR_HEAP_DESC kHeapDesc =
			{
				TYPE, NUM, FLAGS, 0
			};
			VE_ASSERT_EQ(pkDevice->CreateDescriptorHeap(
				&kHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pkHeap), S_OK);
			VE_ASSERT(m_pkHeap);
			m_kRoot = m_pkHeap->GetCPUDescriptorHandleForHeapStart();
			m_u32DescIncSize = pkDevice->GetDescriptorHandleIncrementSize(TYPE);
		}

		void Term() noexcept
		{
			VE_SAFE_RELEASE(m_pkHeap);
			m_kFreeIndexList.clear();
		}

		D3D12_CPU_DESCRIPTOR_HANDLE Alloc() noexcept
		{
			D3D12_CPU_DESCRIPTOR_HANDLE kRes;			
			if(m_kFreeIndexList.size())
			{
				kRes.ptr = m_kRoot.ptr + m_kFreeIndexList.back();
				m_kFreeIndexList.pop_back();
			}
			else if (m_u32FreeStart < NUM)
			{
				kRes.ptr = m_kRoot.ptr + m_u32FreeStart * m_u32DescIncSize;
				++m_u32FreeStart;
			}
			else
			{
				kRes.ptr = 0;
			}
			return kRes;
		}

		void Free(D3D12_CPU_DESCRIPTOR_HANDLE hHandle) noexcept
		{
			if (hHandle.ptr >= m_kRoot.ptr)
			{
				VeUInt32 u32Delta = (VeUInt32)(hHandle.ptr - m_kRoot.ptr);
				if ((u32Delta % m_u32DescIncSize == 0)
					&& (u32Delta / m_u32DescIncSize < NUM))
				{
					m_kFreeIndexList.push_back(u32Delta);
				}
			}
	
		}
	private:
		ID3D12DescriptorHeap* m_pkHeap = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE m_kRoot;
		VeUInt32 m_u32DescIncSize = 0;
		VeUInt32 m_u32FreeStart = 0;
		VeVector<VeUInt32> m_kFreeIndexList;
	};

	typedef DescriptorHeapShell<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_COUNT, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> RTVHeap;
	typedef DescriptorHeapShell<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, DSV_COUNT, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> DSVHeap;

	VeRendererD3D12() noexcept;

	virtual ~VeRendererD3D12() noexcept;

	virtual bool Init() noexcept override;

	virtual void Term() noexcept override;

	virtual VeRenderWindowPtr CreateRenderWindow(const VeWindowPtr& spWindow) noexcept override;

	virtual bool IsSupported(const VeChar8* pcPlatform) noexcept override;

	virtual VeShader::Type GetShaderType(const VeChar8* pcTarget) noexcept override;

	virtual VeBlobPtr CompileShader(const VeChar8* pcName, const VeChar8* pcTarget,
		const VeChar8* pcConfigPath, VeJSONValue& kConfig,
		const VeResourceManager::FileCachePtr& spCache) noexcept override;

	virtual VeBlobPtr SerializeRootSignature(const VeChar8* pcName, VeJSONValue& kConfig,
		const VeResourceGroupPtr& spGroup) noexcept override;

	virtual RootSignaturePtr CreateRootSignature(const VeBlobPtr& spBlob) noexcept override;

	virtual PipelineStatePtr CreatePipelineState(VeJSONValue& kConfig) noexcept override;

	PipelineStatePtr CreateGraphicsPipelineState(VeJSONValue& kConfig) noexcept;

	PipelineStatePtr CreateComputePipelineState(VeJSONValue& kConfig) noexcept;

protected:
	friend class VeRenderWindowD3D12;

	void InitParsers() noexcept;

	void TermParsers() noexcept;

	void InitCopyQueue() noexcept;

	void TermCopyQueue() noexcept;

	VeSharedLibPtr m_spD3D12;
	VeSharedLibPtr m_spDXGI;
	VeSharedLibPtr m_spD3DCompiler;

	IDXGIFactory1* m_pkDXGIFactory = nullptr;
	IDXGIAdapter1* m_pkDefaultAdapter = nullptr;
	ID3D12Device* m_pkDevice = nullptr;

	ID3D12CommandQueue* m_pkCopyQueue = nullptr;
	ID3D12CommandAllocator* m_pkCopyAllocator = nullptr;
	ID3D12GraphicsCommandList* m_pkCopyCommandList = nullptr;
	ID3D12Fence* m_pkCopyFence = nullptr;
	HANDLE m_kCopyFenceEvent = nullptr;
	VeUInt64 m_u64CopyFenceValue = 0;
	VeTaskQueue m_kCopyQueue;

	RTVHeap m_kRTVHeap;
	DSVHeap m_kDSVHeap;

	VeRefList<VeRenderWindowD3D12*> m_kRenderWindowList;
	VeRefList<RootSignatureD3D12*> m_kRootSignatureList;
	VeRefList<PipelineStateD3D12*> m_kPipelineStateList;

	HRESULT (WINAPI* D3D12GetDebugInterface)(
		_In_ REFIID riid, _COM_Outptr_opt_ void** ppvDebug) = nullptr;

	HRESULT (WINAPI* D3D12CreateDevice)(
		_In_opt_ IUnknown* pAdapter,
		D3D_FEATURE_LEVEL MinimumFeatureLevel,
		_In_ REFIID riid,
		_COM_Outptr_opt_ void** ppDevice) = nullptr;

	HRESULT(WINAPI* D3D12SerializeRootSignature)(
		_In_ const D3D12_ROOT_SIGNATURE_DESC* pRootSignature,
		_In_ D3D_ROOT_SIGNATURE_VERSION Version,
		_Out_ ID3DBlob** ppBlob,
		_Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob) = nullptr;

	HRESULT(WINAPI* CreateDXGIFactory1)(
		REFIID riid, _COM_Outptr_ void **ppFactory) = nullptr;

	HRESULT(WINAPI* D3DCompile)(
		LPCVOID pSrcData, SIZE_T SrcDataSize, LPCSTR pFileName,
		CONST D3D_SHADER_MACRO* pDefines, ID3DInclude* pInclude,
		LPCSTR pEntrypoint, LPCSTR pTarget, UINT Flags1, UINT Flags2,
		ID3DBlob** ppCode, ID3DBlob** ppErrorMsgs) = nullptr;

	VeStringMap<BlendType> m_kBlendTypeParser;
	VeStringMap<RasterType> m_kRasterTypeParser;
	VeStringMap<DepthStencilType> m_kDSTypeParser;

	VeStringMap<D3D12_ROOT_PARAMETER_TYPE> m_kRootParameterTypeParser;
	VeStringMap<D3D12_DESCRIPTOR_RANGE_TYPE> m_kDescriptorRangeTypeParser;
	VeStringMap<D3D12_SHADER_VISIBILITY> m_kShaderVisibilityParser;
	VeStringMap<D3D12_ROOT_SIGNATURE_FLAGS> m_kRootSignatureFlagsParser;
	VeStringMap<D3D12_FILTER> m_kFilterParser;
	VeStringMap<D3D12_TEXTURE_ADDRESS_MODE> m_kTexAddressModeParser;
	VeStringMap<D3D12_COMPARISON_FUNC> m_kComparisonFuncParser;
	VeStringMap<D3D12_STATIC_BORDER_COLOR> m_kStaticBorderColorParser;
	VeStringMap<DXGI_FORMAT> m_kFormatParser;
	VeStringMap<D3D12_INPUT_CLASSIFICATION> m_kInputClassParser;
	VeStringMap<D3D12_BLEND> m_kBlendParser;
	VeStringMap<D3D12_BLEND_OP> m_kBlendOpParser;
	VeStringMap<D3D12_LOGIC_OP> m_kLogicOpParser;
	VeStringMap<D3D12_COLOR_WRITE_ENABLE> m_kColorWriteParser;
	VeStringMap<D3D12_FILL_MODE> m_kFillModeParser;
	VeStringMap<D3D12_CULL_MODE> m_kCullModeParser;
	VeStringMap<D3D12_STENCIL_OP> m_kStencilOpParser;
	VeStringMap<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE> m_kIBCutParser;
	VeStringMap<D3D12_PRIMITIVE_TOPOLOGY_TYPE> m_kPrimTopoTypeParser;

};

VeRendererPtr CreateRendererD3D12() noexcept;

#endif
