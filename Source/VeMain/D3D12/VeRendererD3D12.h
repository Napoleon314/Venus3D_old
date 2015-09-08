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
	static constexpr VeUInt32 SRV_COUNT = 4096;

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

	class DynamicCBufferD3D12 : public DynamicCBuffer
	{
		VeNoCopy(DynamicCBufferD3D12);
		VeRTTIDecl(DynamicCBufferD3D12, DynamicCBuffer);
	public:
		DynamicCBufferD3D12(VeSizeT stSize) noexcept
		{
			m_kNode.m_Content = this;
			m_stSize = (stSize + 255) & (~255);
		}

		virtual ~DynamicCBufferD3D12() noexcept
		{
			Term();
		}

		void Init(VeRendererD3D12& kRenderer) noexcept
		{
			D3D12_HEAP_PROPERTIES kHeapProp =
			{
				D3D12_HEAP_TYPE_UPLOAD,
				D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
				D3D12_MEMORY_POOL_UNKNOWN,
				0,
				0
			};
			D3D12_RESOURCE_DESC kResDesc =
			{
				D3D12_RESOURCE_DIMENSION_BUFFER,
				0,
				m_stSize * FRAME_COUNT,
				1,
				1,
				1,
				DXGI_FORMAT_UNKNOWN,
				{ 1, 0 },
				D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
				D3D12_RESOURCE_FLAG_NONE
			};
			VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommittedResource(
				&kHeapProp, D3D12_HEAP_FLAG_NONE, &kResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
				__uuidof(ID3D12Resource), (void**)&m_pkResource), S_OK);
			VE_ASSERT_GE(m_pkResource->Map(0, nullptr, &m_pvMappedBuffer), S_OK);
			VE_ASSERT(!(((VeSizeT)m_pvMappedBuffer) & 0xf));
			for (VeUInt32 i(0); i < FRAME_COUNT; ++i)
			{
				m_ahCBV[i].m_u32Offset = kRenderer.m_kSRVHeap.Alloc();
				m_ahCBV[i].m_kCPUHandle.ptr = kRenderer.m_kSRVHeap.GetCPUStart().ptr + m_ahCBV[i].m_u32Offset;
				m_ahCBV[i].m_kGPUHandle.ptr = kRenderer.m_kSRVHeap.GetGPUStart().ptr + m_ahCBV[i].m_u32Offset;
				D3D12_CONSTANT_BUFFER_VIEW_DESC kCBVDesc = {
					m_pkResource->GetGPUVirtualAddress() + i * m_stSize,
					(UINT)m_stSize
				};
				kRenderer.m_pkDevice->CreateConstantBufferView(&kCBVDesc, m_ahCBV[i].m_kCPUHandle);
			}
			kRenderer.m_kDyanmicCBufferList.attach_back(m_kNode);
		}

		void Term() noexcept
		{
			if (m_kNode.is_attach())
			{
				VeRendererD3D12& kRenderer = *VeMemberCast(
					&VeRendererD3D12::m_kDyanmicCBufferList, m_kNode.get_list());

				for (VeUInt32 i(FRAME_COUNT-1); i < FRAME_COUNT; --i)
				{
					kRenderer.m_kSRVHeap.Free(m_ahCBV[i].m_u32Offset);
				}
				m_pvMappedBuffer = nullptr;
				m_pkResource->Unmap(0, nullptr);
				VE_SAFE_RELEASE(m_pkResource);
			}
		}

		void* GetActiveBuffer() noexcept
		{
			return (VeByte*)m_pvMappedBuffer + m_stSize * m_u32Active;
		}

		D3D12_GPU_DESCRIPTOR_HANDLE GetActive() noexcept
		{
			return m_ahCBV[m_u32Active].m_kGPUHandle;
		}

		virtual void* Map() noexcept override
		{
			if ((++m_u32Active) >= FRAME_COUNT)
				m_u32Active -= FRAME_COUNT;
			return GetActiveBuffer();
		}

		virtual void Unmap() noexcept override
		{

		}

		virtual void Update(void* pvData) noexcept override
		{
			if ((++m_u32Active) >= FRAME_COUNT)
				m_u32Active -= FRAME_COUNT;
			VeCrazyCopy(GetActiveBuffer(), pvData, m_stSize);
		}

		virtual VeSizeT GetSize() noexcept override
		{
			return m_stSize;
		}
		
		VeRefNode<DynamicCBufferD3D12*> m_kNode;
		VeSizeT m_stSize = 0;
		ID3D12Resource* m_pkResource = nullptr;
		void* m_pvMappedBuffer = nullptr;
		struct  
		{
			VeUInt32 m_u32Offset;
			D3D12_CPU_DESCRIPTOR_HANDLE m_kCPUHandle;
			D3D12_GPU_DESCRIPTOR_HANDLE m_kGPUHandle;
		} m_ahCBV[FRAME_COUNT];
		VeUInt32 m_u32Active = FRAME_COUNT - 1;

	};

	class StaticVBufferD3D12 : public StaticVBuffer
	{
		VeNoCopy(StaticVBufferD3D12);
		VeRTTIDecl(StaticVBufferD3D12, StaticVBuffer);
	public:
		StaticVBufferD3D12() noexcept
		{
			m_kNode.m_Content = this;
		}

		virtual ~StaticVBufferD3D12() noexcept
		{
			
		}

		void Init(VeRendererD3D12& kRenderer) noexcept
		{
			
		}

		void Term() noexcept
		{

		}

		VeRefNode<StaticVBufferD3D12*> m_kNode;
		ID3D12Resource* m_pkBuffer = nullptr;
		VeVector<D3D12_VERTEX_BUFFER_VIEW> m_kViewList;

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
			VE_ASSERT_GE(pkDevice->CreateDescriptorHeap(
				&kHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pkHeap), S_OK);
			VE_ASSERT(m_pkHeap);
			m_u32DescIncSize = pkDevice->GetDescriptorHandleIncrementSize(TYPE);
		}

		void Term() noexcept
		{
			VE_SAFE_RELEASE(m_pkHeap);
			m_kFreeIndexList.clear();
		}

		VeUInt32 Alloc() noexcept
		{	
			if(m_kFreeIndexList.size())
			{
				VeUInt32 u32Pointer = m_kFreeIndexList.back();
				m_kFreeIndexList.pop_back();
				return u32Pointer;
			}
			else if (m_u32FreeStart < NUM)
			{
				return (m_u32FreeStart++) * m_u32DescIncSize;
			}
			else
			{
				VE_ASSERT(!"Can not alloc");
				return VE_UINT32_MAX;
			}
		}

		void Free(VeUInt32 u32Pointer) noexcept
		{
			VE_ASSERT(u32Pointer % m_u32DescIncSize == 0
				&& (u32Pointer / m_u32DescIncSize) < NUM);
			m_kFreeIndexList.push_back(u32Pointer);
		}

		ID3D12DescriptorHeap* Get() noexcept
		{
			return m_pkHeap;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUStart() noexcept
		{
			return m_pkHeap->GetCPUDescriptorHandleForHeapStart();
		}

		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUStart() noexcept
		{
			return m_pkHeap->GetGPUDescriptorHandleForHeapStart();
		}

	private:
		ID3D12DescriptorHeap* m_pkHeap = nullptr;
		VeUInt32 m_u32DescIncSize = 0;
		VeUInt32 m_u32FreeStart = 0;
		VeVector<VeUInt32> m_kFreeIndexList;
	};

	typedef DescriptorHeapShell<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_COUNT, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> RTVHeap;
	typedef DescriptorHeapShell<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, DSV_COUNT, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> DSVHeap;
	typedef DescriptorHeapShell<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, SRV_COUNT, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> SRVHeap;

	VeRendererD3D12() noexcept;

	virtual ~VeRendererD3D12() noexcept;

	virtual bool Init() noexcept override;

	virtual void Term() noexcept override;

	virtual void BeginSyncCopy() noexcept override;

	virtual void EndSyncCopy() noexcept override;

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

	virtual DynamicCBufferPtr CreateDynamicCBuffer(VeSizeT stSize) noexcept override;

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
	bool m_bHasCopyTask = false;

	RTVHeap m_kRTVHeap;
	DSVHeap m_kDSVHeap;
	SRVHeap m_kSRVHeap;

	VeRefList<VeRenderWindowD3D12*> m_kRenderWindowList;
	VeRefList<RootSignatureD3D12*> m_kRootSignatureList;
	VeRefList<PipelineStateD3D12*> m_kPipelineStateList;
	VeRefList<DynamicCBufferD3D12*> m_kDyanmicCBufferList;

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
