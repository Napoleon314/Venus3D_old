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

#include "VeRenderBufferD3D12.h"

class VeRenderWindowD3D12 : public VeRenderWindow
{
	VeNoCopy(VeRenderWindowD3D12);
	VeRTTIDecl(VeRenderWindowD3D12, VeRenderWindow);
public:
	enum QueueProcessType
	{
		TYPE_EXCUTE,
		TYPE_SIGNAL,
		TYPE_WAIT
	};

	enum RecordType
	{
		REC_BARRIER,
		REC_CLEAR_RTV,
		REC_CLEAR_DSV,
		REC_CLEAR_UAV,
		REC_RENDER_TARGET,
		REC_RENDER_QUAD,
		REC_QUAD,
	};

	struct RecordTask : public VeRefObject
	{
		RecordTask(RecordType eType) noexcept
			: m_eType(eType) {}

		const RecordType m_eType;
	};

	typedef VePointer<RecordTask> RecordTaskPtr;

	struct RecordBarrier : public RecordTask
	{
		RecordBarrier() noexcept : RecordTask(REC_BARRIER) {}

		VeVector<D3D12_RESOURCE_BARRIER> m_kBarrierList[VeRendererD3D12::FRAME_COUNT];
	};

	typedef VePointer<RecordBarrier> RecordBarrierPtr;

	struct RecordClearRTV : public RecordTask
	{
		RecordClearRTV() noexcept : RecordTask(REC_CLEAR_RTV) {}

		D3D12_CPU_DESCRIPTOR_HANDLE m_hHandle[VeRendererD3D12::FRAME_COUNT];
		VeColor m_kColor;
	};

	struct RecordClearDSV : public RecordTask
	{
		RecordClearDSV() noexcept : RecordTask(REC_CLEAR_DSV) {}

		D3D12_CPU_DESCRIPTOR_HANDLE m_hHandle[VeRendererD3D12::FRAME_COUNT];
		VeFloat32 m_f32Depth = 0;
		VeUInt8 m_u8Stencil = 0;
		bool m_bClearDepth = false;
		bool m_bClearStencil = false;
	};

	struct RecordRenderTarget : public RecordTask
	{
		RecordRenderTarget() noexcept : RecordTask(REC_RENDER_TARGET) {}

		struct
		{
			VeVector<D3D12_CPU_DESCRIPTOR_HANDLE> m_kRTVList;
			D3D12_CPU_DESCRIPTOR_HANDLE m_hDSV = {};
		} m_kRenderTarget[VeRendererD3D12::FRAME_COUNT];
		D3D12_VIEWPORT m_kViewport;
		D3D12_RECT m_kScissorRect;
	};

	typedef VePointer<RecordRenderTarget> RecordRenderTargetPtr;

	struct RecordRenderQuad : public RecordTask
	{
		RecordRenderQuad() noexcept : RecordTask(REC_RENDER_QUAD) {}

		ID3D12RootSignature* m_pkRootSignature = nullptr;
		ID3D12PipelineState* m_pkPipelineState = nullptr;		
	};

	struct Recorder
	{
		VeUInt32 m_u32CommandIndex;
		VeVector<RecordTaskPtr> m_kTaskList;
	};

	struct Camera
	{
		struct Stage
		{
			VeUInt32 m_u32GCLStart;
			VeUInt32 m_u32Mask;
			VeVector<D3D12_GPU_DESCRIPTOR_HANDLE> m_kContext;
		};
		VeVector<Stage> m_kStageList;
	};

	struct Bundle
	{
		VeUInt16 m_u16BundleIndex;
		VeUInt16 m_u16CommandListIndex;
	};

	struct QueueProcess
	{
		QueueProcessType m_eType;
		union
		{
			VeUInt32 m_u32Value;
			struct 
			{
				VeUInt16 m_u16Start;
				VeUInt16 m_u16Num;
			};
		};		
	};	

	VeRenderWindowD3D12(const VeWindowPtr& spWindow) noexcept;

	virtual ~VeRenderWindowD3D12() noexcept;

	void Init(VeRendererD3D12& kRenderer) noexcept;

	void Term() noexcept;

	virtual bool IsValid() noexcept override;

	virtual void SetupCompositorList(const VeChar8** ppcList,
		VeSizeT stNum, const VeChar8* pcHint) noexcept override;

	virtual VeUInt32 GetRecorderNum() noexcept override;

	virtual void Record(VeUInt32 u32Index) noexcept override;

	virtual void BeginCommandLists(VeUInt32 u32Thread) noexcept;

	virtual void SendDrawCallList(VeUInt32 u32Thread, VeUInt32 u32Camera,
		VeRenderDrawCall* pkDrawCallList, VeSizeT stNum) noexcept override;

	virtual void Begin() noexcept override;

	virtual void End() noexcept override;

	/*virtual void Update(void* pvCBuffer, void* pvVBuffer) noexcept override
	{
		VE_ASSERT(m_kNode.is_attach());
		VeRendererD3D12& kRenderer = *VeMemberCast(
			&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());

		VE_ASSERT(m_u32FrameIndex < VeRendererD3D12::FRAME_COUNT);
		FrameCache& kFrame = m_akFrameCache[m_u32FrameIndex];

		VE_ASSERT_GE(kFrame.m_pkDirectAllocator->Reset(), S_OK);
		VE_ASSERT_GE(kFrame.m_pkDirectList->Reset(kFrame.m_pkDirectAllocator,
			kRenderer.m_kPipelineStateList.get_head_node()->m_Content->m_pkPipelineState), S_OK);

		kFrame.m_pkDirectList->SetGraphicsRootSignature(
			kRenderer.m_kRootSignatureList.get_head_node()->m_Content->m_pkRootSignature);

		kFrame.m_pkDirectList->RSSetViewports(1, &m_kViewport);
		kFrame.m_pkDirectList->RSSetScissorRects(1, &m_kScissorRect);

		ID3D12DescriptorHeap* ppHeaps[] = { kRenderer.m_kSRVHeap.Get() };
		kFrame.m_pkDirectList->SetDescriptorHeaps(1, ppHeaps);

		VeDynamicBufferD3D12* pkCB = (VeDynamicBufferD3D12*)pvCBuffer;

		kFrame.m_pkDirectList->SetGraphicsRootDescriptorTable(
			0, pkCB->m_pkCBView[pkCB->m_u32Active].m_kGPUHandle);		

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

		kFrame.m_pkDirectList->OMSetRenderTargets(1, &kFrame.m_hHandle, FALSE, nullptr);		
		
		kFrame.m_pkDirectList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		kFrame.m_pkDirectList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
		kFrame.m_pkDirectList->DrawInstanced(3, 1, 0, 0);


		kBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		kBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		kFrame.m_pkDirectList->ResourceBarrier(1, &kBarrier);

		VE_ASSERT_GE(kFrame.m_pkDirectList->Close(), S_OK);

		ID3D12CommandList* ppCommandLists[] = { kFrame.m_pkDirectList };
		m_pkCommandQueue->ExecuteCommandLists(1, ppCommandLists);
	}*/

protected:
	void ResizeDirectList(VeSizeT stNum) noexcept;

	void ResizeBundleList(VeSizeT stNum) noexcept;

	VeRefNode<VeRenderWindowD3D12*> m_kNode;
	ID3D12CommandQueue* m_pkCommandQueue = nullptr;
	IDXGISwapChain3* m_pkSwapChain = nullptr;

	struct FrameCache
	{
		ID3D12Resource* m_pkBufferResource = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE m_hHandle = {};
		ID3D12CommandAllocator* m_pkDirectAllocator = nullptr;
		VeVector<ID3D12GraphicsCommandList*> m_kDirectCommandList;
		ID3D12CommandAllocator* m_pkBundleAllocator = nullptr;
		VeVector<ID3D12GraphicsCommandList*> m_kBundleCommandList;
		VeUInt64 m_u64FenceValue = 0;
	} m_akFrameCache[VeRendererD3D12::FRAME_COUNT];

	struct
	{		
		VeVector<Recorder> m_kRecorderList;
		VeVector<Camera> m_kCameraList;
		VeVector<QueueProcess> m_kProcessList;
	};

	D3D12_VIEWPORT m_kViewport;
	D3D12_RECT m_kScissorRect;

	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	ID3D12Fence* m_pkFence = nullptr;
	HANDLE m_kFenceEvent = nullptr;
	VeUInt64 m_u64FenceValue = 0;

	VeUInt32 m_u32FrameIndex = 0;

};
