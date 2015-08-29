////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRendererWindowD3D12.h
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

struct CD3DX12_RESOURCE_BARRIER : public D3D12_RESOURCE_BARRIER
{
	CD3DX12_RESOURCE_BARRIER()
	{}
	explicit CD3DX12_RESOURCE_BARRIER(const D3D12_RESOURCE_BARRIER &o) :
		D3D12_RESOURCE_BARRIER(o)
	{}
	static inline CD3DX12_RESOURCE_BARRIER Transition(
		_In_ ID3D12Resource* pResource,
		D3D12_RESOURCE_STATES stateBefore,
		D3D12_RESOURCE_STATES stateAfter,
		UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
		D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE)
	{
		CD3DX12_RESOURCE_BARRIER result;
		ZeroMemory(&result, sizeof(result));
		D3D12_RESOURCE_BARRIER &barrier = result;
		result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		result.Flags = flags;
		barrier.Transition.pResource = pResource;
		barrier.Transition.StateBefore = stateBefore;
		barrier.Transition.StateAfter = stateAfter;
		barrier.Transition.Subresource = subresource;
		return result;
	}
	static inline CD3DX12_RESOURCE_BARRIER Aliasing(
		_In_ ID3D12Resource* pResourceBefore,
		_In_ ID3D12Resource* pResourceAfter)
	{
		CD3DX12_RESOURCE_BARRIER result;
		ZeroMemory(&result, sizeof(result));
		D3D12_RESOURCE_BARRIER &barrier = result;
		result.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
		barrier.Aliasing.pResourceBefore = pResourceBefore;
		barrier.Aliasing.pResourceAfter = pResourceAfter;
		return result;
	}
	static inline CD3DX12_RESOURCE_BARRIER UAV(
		_In_ ID3D12Resource* pResource)
	{
		CD3DX12_RESOURCE_BARRIER result;
		ZeroMemory(&result, sizeof(result));
		D3D12_RESOURCE_BARRIER &barrier = result;
		result.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		barrier.UAV.pResource = pResource;
		return result;
	}
	operator const D3D12_RESOURCE_BARRIER&() const { return *this; }
};

class VeRenderWindowD3D12 : public VeRenderWindow
{
	VeNoCopy(VeRenderWindowD3D12);
	VeRTTIDecl(VeRenderWindowD3D12, VeRenderWindow);
public:
	VeRenderWindowD3D12(const VeWindowPtr& spWindow) noexcept;

	virtual ~VeRenderWindowD3D12() noexcept;

	void Init(VeRendererD3D12& kRenderer) noexcept;

	void Term() noexcept;

	virtual bool IsValid() noexcept override;

	virtual void Update() noexcept override
	{
		VE_ASSERT(m_u32FrameIndex < VeRendererD3D12::FRAME_COUNT);
		FrameCache& kFrame = m_akFrameCache[m_u32FrameIndex];
		if (kFrame.m_u64FenceValue > m_pkFence->GetCompletedValue())
		{
			VE_ASSERT_EQ(m_pkFence->SetEventOnCompletion(
				kFrame.m_u64FenceValue, m_kFenceEvent), S_OK);
			WaitForSingleObject(m_kFenceEvent, INFINITE);
		}

		VE_ASSERT_EQ(kFrame.m_pkDirectAllocator->Reset(), S_OK);
		VE_ASSERT_EQ(kFrame.m_pkDirectList->Reset(
			kFrame.m_pkDirectAllocator, nullptr), S_OK);

		D3D12_RESOURCE_BARRIER kBarrier;
		kBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		kBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		kBarrier.Transition.pResource = kFrame.m_pkBufferResource;
		kBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		kBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		kBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		kFrame.m_pkDirectList->ResourceBarrier(1, &kBarrier);

		const VeFloat32 clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		kFrame.m_pkDirectList->ClearRenderTargetView(kFrame.m_hHandle,
			clearColor, 0, nullptr);

		kBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		kBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		kFrame.m_pkDirectList->ResourceBarrier(1, &kBarrier);

		VE_ASSERT_EQ(kFrame.m_pkDirectList->Close(), S_OK);

		ID3D12CommandList* ppCommandLists[] = { kFrame.m_pkDirectList };
		m_pkCommandQueue->ExecuteCommandLists(1, ppCommandLists);		

		VE_ASSERT_EQ(m_pkSwapChain->Present(0, 0), S_OK);

		m_u32FrameIndex = m_pkSwapChain->GetCurrentBackBufferIndex();

		kFrame.m_u64FenceValue = m_u64FenceValue;
		VE_ASSERT_EQ(m_pkCommandQueue->Signal(m_pkFence, m_u64FenceValue++), S_OK);
	}

protected:
	VeRefNode<VeRenderWindowD3D12*> m_kNode;
	ID3D12CommandQueue* m_pkCommandQueue = nullptr;
	IDXGISwapChain3* m_pkSwapChain = nullptr;
	struct FrameCache
	{
		ID3D12Resource* m_pkBufferResource = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE m_hHandle = {};
		ID3D12CommandAllocator* m_pkDirectAllocator = nullptr;
		ID3D12GraphicsCommandList* m_pkDirectList = nullptr;
		VeUInt64 m_u64FenceValue = 0;
	} m_akFrameCache[VeRendererD3D12::FRAME_COUNT];

	//ID3D12PipelineState* m_pkState = nullptr;

	ID3D12Fence* m_pkFence = nullptr;
	HANDLE m_kFenceEvent = nullptr;
	VeUInt64 m_u64FenceValue = 0;


	//ID3D12Resource* m_apkBackBufferResource[VeRendererD3D12::FRAME_COUNT];
	//D3D12_CPU_DESCRIPTOR_HANDLE m_ahRenderTargetView[VeRendererD3D12::FRAME_COUNT];




	VeUInt32 m_u32FrameIndex = 0;

};
