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
			IID_PPV_ARGS(&m_pkCommandQueue))))
		{
			VE_SAFE_RELEASE(m_pkCommandQueue);
			VE_SAFE_RELEASE(m_pkSwapChain);
			return;
		}
		DXGI_SWAP_CHAIN_DESC kSwapChainDesc = {};
		kSwapChainDesc.BufferCount = VeRendererD3D12::FRAME_COUNT;
		kSwapChainDesc.BufferDesc.Width = m_spTargetWindow->GetWidth();
		kSwapChainDesc.BufferDesc.Height = m_spTargetWindow->GetHeight();
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
		if (FAILED(pkSwapChain->QueryInterface(IID_PPV_ARGS(&m_pkSwapChain))))
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
		for (VeInt32 i(0); i < VeRendererD3D12::FRAME_COUNT; ++i)
		{
			FrameCache& kFrame = m_akFrameCache[i];
			VE_ASSERT_GE(m_pkSwapChain->GetBuffer(i, IID_PPV_ARGS(&kFrame.m_pkBufferResource)), S_OK);
			kFrame.m_hHandle.ptr = kRenderer.m_kRTVHeap.GetCPUStart().ptr + kRenderer.m_kRTVHeap.Alloc();
			kRenderer.m_pkDevice->CreateRenderTargetView(
				kFrame.m_pkBufferResource, nullptr, kFrame.m_hHandle);
			VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(&kFrame.m_pkDirectAllocator)), S_OK);
			VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandList(0,
				D3D12_COMMAND_LIST_TYPE_DIRECT, kFrame.m_pkDirectAllocator,
				nullptr, IID_PPV_ARGS(&kFrame.m_pkDirectList)), S_OK);
			VE_ASSERT_GE(kFrame.m_pkDirectList->Close(), S_OK);
			kFrame.m_u64FenceValue = 0;
		}

		VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&m_pkBundleAllocator)), S_OK);

		m_u64FenceValue = 0;
		VE_ASSERT_GE(kRenderer.m_pkDevice->CreateFence(m_u64FenceValue++,
			D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pkFence)), S_OK);
		m_kFenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
		VE_ASSERT(m_kFenceEvent);
		const VeUInt64 u64FenceToWaitFor = m_u64FenceValue++;
		VE_ASSERT_GE(m_pkCommandQueue->Signal(m_pkFence, u64FenceToWaitFor), S_OK);
		VE_ASSERT_GE(m_pkFence->SetEventOnCompletion(u64FenceToWaitFor, m_kFenceEvent), S_OK);
		WaitForSingleObject(m_kFenceEvent, INFINITE);
		m_u32FrameIndex = m_pkSwapChain->GetCurrentBackBufferIndex();
		kRenderer.m_kRenderWindowList.attach_back(m_kNode);
	}
	

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
		m_vertexBufferView.BufferLocation = kRenderer.m_kRenderBufferList.get_tail_node()->m_Content->m_pkResource->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(VE_FLOAT3);
		m_vertexBufferView.SizeInBytes = kRenderer.m_kRenderBufferList.get_head_node()->m_Content->GetSize();
	}
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::Term() noexcept
{
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

		for (auto& list : m_kBundleCommandList)
		{
			VE_SAFE_RELEASE(list);
		}
		VE_SAFE_RELEASE(m_pkBundleAllocator);
		m_kBundleCommandList.clear();
		m_kBundleList.clear();

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
void VeRenderWindowD3D12::SetupCompositorList(const VeChar8* pcHint,
	const VeChar8** ppcList, VeSizeT stNum) noexcept
{




}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::ResetFrameForm() noexcept
{
	VE_ASSERT_GE(m_pkBundleAllocator->Reset(), S_OK);
	for (auto& list : m_kBundleCommandList)
	{
		VE_ASSERT_GE(list->Reset(m_pkBundleAllocator, nullptr), S_OK);
	}
	m_stUsedBundleNum = 0;
	m_kBundleList.clear();
	m_kProcessList.clear();
}
//--------------------------------------------------------------------------
ID3D12GraphicsCommandList* VeRenderWindowD3D12::AddBundle() noexcept
{
	VeRendererD3D12& kRenderer = *VeMemberCast(
		&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());
	if (m_stUsedBundleNum >= m_kBundleCommandList.size())
	{
		ID3D12GraphicsCommandList* pkBundle = nullptr;
		VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandList(0,
			D3D12_COMMAND_LIST_TYPE_BUNDLE, m_pkBundleAllocator,
			nullptr, IID_PPV_ARGS(&pkBundle)), S_OK);
		m_kBundleCommandList.push_back(pkBundle);
	}
	++m_stUsedBundleNum;
	return m_kBundleCommandList.back();
}
//--------------------------------------------------------------------------
