////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderWindowD3D12.h
//  Created:     2015/09/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

VeSmartPointer(VeRenderTextureD3D12);

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
		REC_VIEWPORT,
		REC_SCISSOR_RECT,
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

		VeVector<D3D12_RESOURCE_BARRIER> m_akBarrierList[VeRendererD3D12::FRAME_COUNT];
	};

	typedef VePointer<RecordBarrier> RecordBarrierPtr;

	struct RecordClearRTV : public RecordTask
	{
		RecordClearRTV() noexcept : RecordTask(REC_CLEAR_RTV) {}

		D3D12_CPU_DESCRIPTOR_HANDLE m_ahHandle[VeRendererD3D12::FRAME_COUNT];
		VeColor m_kColor;
	};

	struct RecordClearDSV : public RecordTask
	{
		RecordClearDSV() noexcept : RecordTask(REC_CLEAR_DSV) {}

		D3D12_CPU_DESCRIPTOR_HANDLE m_ahHandle[VeRendererD3D12::FRAME_COUNT];
		D3D12_CLEAR_FLAGS m_eFlags = VE_TMIN(D3D12_CLEAR_FLAGS);
		VeFloat32 m_f32Depth = 0;
		VeUInt8 m_u8Stencil = 0;
	};

	struct RecordRenderTarget : public RecordTask
	{
		RecordRenderTarget() noexcept : RecordTask(REC_RENDER_TARGET) {}

		VeVector<D3D12_CPU_DESCRIPTOR_HANDLE> m_akRTVList[VeRendererD3D12::FRAME_COUNT];
		D3D12_CPU_DESCRIPTOR_HANDLE m_ahDSV[VeRendererD3D12::FRAME_COUNT];
	};

	typedef VePointer<RecordRenderTarget> RecordRenderTargetPtr;

	struct RecordViewport : public RecordTask
	{
		RecordViewport() noexcept : RecordTask(REC_VIEWPORT) {}

		VeVector<D3D12_VIEWPORT> m_kViewportList;
	};

	typedef VePointer<RecordViewport> RecordViewportPtr;

	struct RecordScissorRect : public RecordTask
	{
		RecordScissorRect() noexcept : RecordTask(REC_SCISSOR_RECT) {}

		VeVector<D3D12_RECT> m_kScissorRectList;
	};

	typedef VePointer<RecordScissorRect> RecordScissorRectPtr;

	struct RecordRenderQuad : public RecordTask
	{
		RecordRenderQuad() noexcept : RecordTask(REC_RENDER_QUAD) {}

		ID3D12RootSignature* m_pkRootSignature = nullptr;
		ID3D12PipelineState* m_pkPipelineState = nullptr;
		VeVector<std::pair<VeUInt32, D3D12_GPU_DESCRIPTOR_HANDLE>> m_kTable;
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

protected:
	void ResizeDirectList(VeSizeT stNum) noexcept;

	void ResizeBundleList(VeSizeT stNum) noexcept;

	struct ViewData
	{
		ID3D12Resource* m_pkResource = nullptr;
		VeUInt32 m_u32Width = 0;
		VeUInt32 m_u32Height = 0;
		VeUInt32 m_u32Depth = 0;
		VeUInt32 m_u32SubResource = 0;
	};

	struct TargetCache
	{
		VeRenderer::FrameTarget* m_pkConfig = nullptr;
		RecordRenderTargetPtr m_spRecorder;
		VeVector<ViewData> m_kRTVList;
		ViewData m_kDSV;
	};
	
	typedef VeVector<VeVector<std::pair<RecordBarrierPtr, RecordBarrierPtr>>> RecordBarrierMap;

	struct BarrierPath
	{
		VeSizeT i, j;
		VeVector<D3D12_RESOURCE_STATES> m_kStates;
		D3D12_RESOURCE_STATES m_eAllState = D3D12_RESOURCE_STATE_COMMON;
	};

	typedef VeMap<ID3D12Resource*, VeVector<BarrierPath>> BarrierPathCache;

	typedef VeMap<std::pair<VeUInt32, VeUInt32>, std::pair<RecordViewportPtr, RecordScissorRectPtr>> RectMap;

	static bool HasChange(BarrierPath& kPath, VeUInt32 u32SubResource,
		D3D12_RESOURCE_STATES eState) noexcept;

	static bool IsNeighbor(RecordBarrierMap& kBarriers, BarrierPath& kPath,
		VeSizeT i, VeSizeT j) noexcept;

	static D3D12_RESOURCE_STATES GetState(BarrierPath& kPath, VeUInt32 u32SubResource) noexcept;

	static std::pair<RecordViewportPtr, RecordScissorRectPtr>& Get(
		RectMap& kMap, std::pair<VeUInt32, VeUInt32>& kResolution) noexcept;

	static std::pair<VeUInt32, VeUInt32> GetResolution(TargetCache& tar) noexcept;

	void BarrierEvent(RecordBarrierMap& kBarriers, BarrierPathCache& kResPathCache,
		VeSizeT i, VeSizeT j, ID3D12Resource* pkResource,
		VeUInt32 u32SubResource, D3D12_RESOURCE_STATES eState) noexcept;

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
		VeStringMap<VeRenderTextureD3D12Ptr> m_kResourceMap;
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
