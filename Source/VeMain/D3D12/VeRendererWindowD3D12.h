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

#include "d3dx12.h"

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
		VE_ASSERT(m_kNode.is_attach());
		VeRendererD3D12& kRenderer = *VeMemberCast(
			&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());

		VE_ASSERT(m_u32FrameIndex < VeRendererD3D12::FRAME_COUNT);
		FrameCache& kFrame = m_akFrameCache[m_u32FrameIndex];
		if (kFrame.m_u64FenceValue > m_pkFence->GetCompletedValue())
		{
			VE_ASSERT_GE(m_pkFence->SetEventOnCompletion(
				kFrame.m_u64FenceValue, m_kFenceEvent), S_OK);
			WaitForSingleObject(m_kFenceEvent, INFINITE);
		}

		VE_ASSERT_GE(kFrame.m_pkDirectAllocator->Reset(), S_OK);
		VE_ASSERT_GE(kFrame.m_pkDirectList->Reset(
			kFrame.m_pkDirectAllocator,
			kRenderer.m_kPipelineStateList.get_head_node()->m_Content->m_pkPipelineState), S_OK);

		kFrame.m_pkDirectList->SetGraphicsRootSignature(
			kRenderer.m_kRootSignatureList.get_head_node()->m_Content->m_pkRootSignature);
		kFrame.m_pkDirectList->RSSetViewports(1, &m_kViewport);
		kFrame.m_pkDirectList->RSSetScissorRects(1, &m_kScissorRect);

		D3D12_RESOURCE_BARRIER kBarrier;
		kBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		kBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		kBarrier.Transition.pResource = kFrame.m_pkBufferResource;
		kBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		kBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		kBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		kFrame.m_pkDirectList->ResourceBarrier(1, &kBarrier);


		kFrame.m_pkDirectList->OMSetRenderTargets(1, &kFrame.m_hHandle, FALSE, nullptr);

		const VeFloat32 clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		kFrame.m_pkDirectList->ClearRenderTargetView(kFrame.m_hHandle,
			clearColor, 0, nullptr);
		
		kFrame.m_pkDirectList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		kFrame.m_pkDirectList->IASetVertexBuffers(0, 1, &m_pkVertexBufferView);
		kFrame.m_pkDirectList->DrawInstanced(3, 1, 0, 0);


		kBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		kBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		kFrame.m_pkDirectList->ResourceBarrier(1, &kBarrier);

		VE_ASSERT_GE(kFrame.m_pkDirectList->Close(), S_OK);

		ID3D12CommandList* ppCommandLists[] = { kFrame.m_pkDirectList };
		m_pkCommandQueue->ExecuteCommandLists(1, ppCommandLists);		

		VE_ASSERT_GE(m_pkSwapChain->Present(1, 0), S_OK);

		m_u32FrameIndex = m_pkSwapChain->GetCurrentBackBufferIndex();

		kFrame.m_u64FenceValue = m_u64FenceValue;
		VE_ASSERT_GE(m_pkCommandQueue->Signal(m_pkFence, m_u64FenceValue++), S_OK);
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

	D3D12_VIEWPORT m_kViewport;
	D3D12_RECT m_kScissorRect;
	ID3D12Resource* m_pkVertexBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_pkVertexBufferView;

	ID3D12Fence* m_pkFence = nullptr;
	HANDLE m_kFenceEvent = nullptr;
	VeUInt64 m_u64FenceValue = 0;
	

	//ID3D12Resource* m_apkBackBufferResource[VeRendererD3D12::FRAME_COUNT];
	//D3D12_CPU_DESCRIPTOR_HANDLE m_ahRenderTargetView[VeRendererD3D12::FRAME_COUNT];




	VeUInt32 m_u32FrameIndex = 0;

};
