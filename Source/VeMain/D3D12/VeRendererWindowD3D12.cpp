////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRendererWindowD3D12.cpp
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
VeRTTIImpl(VeRenderWindowD3D12, VeRenderWindow);
//--------------------------------------------------------------------------
VeRenderWindowD3D12::VeRenderWindowD3D12(
	const VeWindowPtr& spWindow) noexcept
	: VeRenderWindow(spWindow)
{
	m_kNode.m_Content = this;
}
//--------------------------------------------------------------------------
VeRenderWindowD3D12::~VeRenderWindowD3D12() noexcept
{
	Term();
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::Init(VeRendererD3D12& kRenderer) noexcept
{
	if ((!m_kNode.is_attach()) && m_spTargetWindow)
	{
		D3D12_COMMAND_QUEUE_DESC kQueueDesc = {};
		kQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		kQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		if (FAILED(kRenderer.m_pkDevice->CreateCommandQueue(&kQueueDesc,
			__uuidof(ID3D12CommandQueue), (void**)&m_pkCommandQueue)))
		{
			VE_SAFE_RELEASE(m_pkCommandQueue);
			VE_SAFE_RELEASE(m_pkSwapChain);
			return;
		}
		DXGI_SWAP_CHAIN_DESC kSwapChainDesc = {};
		kSwapChainDesc.BufferCount = VeRendererD3D12::FRAME_COUNT;
		kSwapChainDesc.BufferDesc.Width = m_spTargetWindow->GetWidth();
		kSwapChainDesc.BufferDesc.Height = m_spTargetWindow->GetHeight();
		kSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		if(VE_MASK_HAS_ALL(m_spTargetWindow->GetFlags(), VE_WINDOW_ALLOW_HIGHDPI))
			kSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;			
		kSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		kSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		kSwapChainDesc.OutputWindow = m_spTargetWindow->GetWMInfo().win.window;
		kSwapChainDesc.SampleDesc.Count = 1;
		kSwapChainDesc.Windowed = TRUE;
		IDXGISwapChain* pkSwapChain;
		if (FAILED(kRenderer.m_pkDXGIFactory->CreateSwapChain(m_pkCommandQueue,
			&kSwapChainDesc, &pkSwapChain)))
		{
			VE_SAFE_RELEASE(m_pkCommandQueue);
			VE_SAFE_RELEASE(pkSwapChain);
			return;
		}
		if (FAILED(pkSwapChain->QueryInterface(
			__uuidof(IDXGISwapChain3), (void**)&m_pkSwapChain)))
		{
			VE_SAFE_RELEASE(m_pkCommandQueue);
			VE_SAFE_RELEASE(pkSwapChain);
			VE_SAFE_RELEASE(m_pkSwapChain);
			return;
		}
		else
		{
			VE_SAFE_RELEASE(pkSwapChain);
		}
		VE_ASSERT(m_pkCommandQueue && m_pkSwapChain);
		m_u32FrameIndex = m_pkSwapChain->GetCurrentBackBufferIndex();
		for (VeInt32 i(0); i < VeRendererD3D12::FRAME_COUNT; ++i)
		{
			FrameCache& kFrame = m_akFrameCache[i];
			VE_ASSERT_GE(m_pkSwapChain->GetBuffer(i, __uuidof(ID3D12Resource),
				(void**)&kFrame.m_pkBufferResource), S_OK);
			kFrame.m_hHandle.ptr = kRenderer.m_kRTVHeap.GetCPUStart().ptr + kRenderer.m_kRTVHeap.Alloc();
			kRenderer.m_pkDevice->CreateRenderTargetView(
				kFrame.m_pkBufferResource, nullptr, kFrame.m_hHandle);
			VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator),
				(void**)&kFrame.m_pkDirectAllocator), S_OK);
			VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandList(0,
				D3D12_COMMAND_LIST_TYPE_DIRECT, kFrame.m_pkDirectAllocator,
				nullptr, __uuidof(ID3D12GraphicsCommandList),
				(void**)&kFrame.m_pkDirectList), S_OK);
			VE_ASSERT_GE(kFrame.m_pkDirectList->Close(), S_OK);
			kFrame.m_u64FenceValue = 0;
		}

		m_u64FenceValue = 0;
		VE_ASSERT_GE(kRenderer.m_pkDevice->CreateFence(m_u64FenceValue++,
			D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&m_pkFence), S_OK);
		m_kFenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
		VE_ASSERT(m_kFenceEvent);
		const VeUInt64 u64FenceToWaitFor = m_u64FenceValue++;
		VE_ASSERT_GE(m_pkCommandQueue->Signal(m_pkFence, u64FenceToWaitFor), S_OK);
		VE_ASSERT_GE(m_pkFence->SetEventOnCompletion(u64FenceToWaitFor, m_kFenceEvent), S_OK);
		WaitForSingleObject(m_kFenceEvent, INFINITE);

		kRenderer.m_kRenderWindowList.attach_back(m_kNode);
	}
	m_u32FrameIndex = m_pkSwapChain->GetCurrentBackBufferIndex();

	m_kViewport.TopLeftX = 0;
	m_kViewport.TopLeftY = 0;
	m_kViewport.Width = m_spTargetWindow->GetWidth();
	m_kViewport.Height = m_spTargetWindow->GetHeight();
	m_kViewport.MinDepth = 0.0f;
	m_kViewport.MaxDepth = 1.0f;

	m_kScissorRect.left = 0;
	m_kScissorRect.right = m_spTargetWindow->GetWidth();
	m_kScissorRect.top = 0;
	m_kScissorRect.bottom = m_spTargetWindow->GetHeight();

	{
		VE_FLOAT3 triangleVertices[] =
		{
			{ -100.0f, 100.0f, 0.0f },
			{ 100.0f, -100.0f, 0.0f },
			{ 100.0f, 100.0f, 0.0f }
		};

		const UINT vertexBufferSize = sizeof(triangleVertices);

		D3D12_HEAP_PROPERTIES kHeapProp = {};
		kHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		kHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		kHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		kHeapProp.CreationNodeMask = 0;
		kHeapProp.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC kResDesc = {};
		kResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		kResDesc.Alignment = 0;
		kResDesc.Width = vertexBufferSize;
		kResDesc.Height = 1;
		kResDesc.DepthOrArraySize = 1;
		kResDesc.MipLevels = 1;
		kResDesc.Format = DXGI_FORMAT_UNKNOWN;
		kResDesc.SampleDesc.Count = 1;
		kResDesc.SampleDesc.Quality = 0;
		kResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		kResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		
		VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommittedResource(
			&kHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&kResDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_vertexBuffer)), S_OK);

		UINT8* pVertexDataBegin;
		VE_ASSERT_GE(m_vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pVertexDataBegin)), S_OK);
		memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		m_vertexBuffer->Unmap(0, nullptr);

		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(VE_FLOAT3);
		m_vertexBufferView.SizeInBytes = vertexBufferSize;
	}
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::Term() noexcept
{
	VE_SAFE_RELEASE(m_vertexBuffer);
	if (m_kNode.is_attach())
	{
		VeRendererD3D12& kRenderer = *VeMemberCast(
			&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());

		const VeUInt64 u64LastCompleted = m_pkFence->GetCompletedValue();
		VE_ASSERT_GE(m_pkCommandQueue->Signal(m_pkFence, m_u64FenceValue), S_OK);
		if (u64LastCompleted < m_u64FenceValue)
		{
			VE_ASSERT_GE(m_pkFence->SetEventOnCompletion(m_u64FenceValue, m_kFenceEvent), S_OK);
			WaitForSingleObject(m_kFenceEvent, INFINITE);
		}
		CloseHandle(m_kFenceEvent);
		VE_SAFE_RELEASE(m_pkFence);

		for (VeInt32 i(VeRendererD3D12::FRAME_COUNT - 1); i >= 0; --i)
		{
			FrameCache& kFrame = m_akFrameCache[i];
			VE_SAFE_RELEASE(kFrame.m_pkDirectList);
			VE_SAFE_RELEASE(kFrame.m_pkDirectAllocator);
			kRenderer.m_kRTVHeap.Free(kFrame.m_hHandle.ptr - kRenderer.m_kRTVHeap.GetCPUStart().ptr);
			VE_SAFE_RELEASE(kFrame.m_pkBufferResource);
		}
		VE_SAFE_RELEASE(m_pkCommandQueue);
		VE_SAFE_RELEASE(m_pkSwapChain);
		m_kNode.detach();
	}	
}
//--------------------------------------------------------------------------
bool VeRenderWindowD3D12::IsValid() noexcept
{
	return VeRenderWindow::IsValid() ? m_kNode.is_attach() : false;
}
//--------------------------------------------------------------------------
