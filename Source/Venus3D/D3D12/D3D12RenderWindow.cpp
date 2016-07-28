////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      D3D12
//  File name:   VeRenderWindowD3D12.cpp
//  Created:     2016/07/23 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D3D12Renderer.h"
#include "D3D12RenderWindow.h"

//--------------------------------------------------------------------------
#ifdef VE_ENABLE_D3D12
//--------------------------------------------------------------------------
VeRTTIImpl(D3D12RenderWindow, VeRenderWindow);
//--------------------------------------------------------------------------
D3D12RenderWindow::D3D12RenderWindow(const VeWindowPtr& spWindow) noexcept
	: VeRenderWindow(spWindow)
{
	m_kNode._Content = this;
}
//--------------------------------------------------------------------------
D3D12RenderWindow::~D3D12RenderWindow() noexcept
{
	Term();
}
//--------------------------------------------------------------------------
void D3D12RenderWindow::Init(D3D12Renderer& kRenderer) noexcept
{
	if ((!m_kNode.is_attach()) && m_spTargetWindow)
	{
		D3D12_COMMAND_QUEUE_DESC kQueueDesc = {};
		kQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		kQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandQueue(&kQueueDesc, IID_PPV_ARGS(&m_pkCommandQueue)), S_OK);
		DXGI_SWAP_CHAIN_DESC kSwapChainDesc = {};
		kSwapChainDesc.BufferCount = D3D12Renderer::FRAME_COUNT;
		kSwapChainDesc.BufferDesc.Width = m_spTargetWindow->GetWidth();
		kSwapChainDesc.BufferDesc.Height = m_spTargetWindow->GetHeight();
		kSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
		kSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		kSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		kSwapChainDesc.OutputWindow = (HWND)(m_spTargetWindow->GetNativeHandle());
		kSwapChainDesc.SampleDesc.Count = 1;
		kSwapChainDesc.Windowed = TRUE;
		IDXGISwapChain* pkSwapChain;
		VE_ASSERT_GE(kRenderer.m_pkDXGIFactory->CreateSwapChain(m_pkCommandQueue, &kSwapChainDesc, &pkSwapChain), S_OK);
		VE_ASSERT_GE(pkSwapChain->QueryInterface(IID_PPV_ARGS(&m_pkSwapChain)), S_OK);
		VE_SAFE_RELEASE(pkSwapChain);
		VE_ASSERT(m_pkCommandQueue && m_pkSwapChain);
		for (uint32_t i(0); i < D3D12Renderer::FRAME_COUNT; ++i)
		{
			FrameCache& kFrame = m_akFrameCache[i];
			VE_ASSERT_GE(m_pkSwapChain->GetBuffer(i, IID_PPV_ARGS(&kFrame.m_pkBufferResource)), S_OK);
			kFrame.m_hHandle.ptr = kRenderer.m_kRTVHeap.GetCPUStart().ptr + kRenderer.m_kRTVHeap.Alloc();
			kRenderer.m_pkDevice->CreateRenderTargetView(
				kFrame.m_pkBufferResource, nullptr, kFrame.m_hHandle);
			VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(&kFrame.m_pkDirectAllocator)), S_OK);
			VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_BUNDLE,
				IID_PPV_ARGS(&kFrame.m_pkBundleAllocator)), S_OK);
			kFrame.m_u64FenceValue = 0;

			VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
				kFrame.m_pkDirectAllocator, nullptr, IID_PPV_ARGS(&kFrame.m_pkTestList)), S_OK);
			VE_ASSERT_GE(kFrame.m_pkTestList->Close(), S_OK);
		}

		m_u64FenceValue = 0;
		VE_ASSERT_GE(kRenderer.m_pkDevice->CreateFence(m_u64FenceValue++,
			D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pkFence)), S_OK);
		m_kFenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
		VE_ASSERT(m_kFenceEvent);
		const uint64_t u64FenceToWaitFor = m_u64FenceValue++;
		VE_ASSERT_GE(m_pkCommandQueue->Signal(m_pkFence, u64FenceToWaitFor), S_OK);
		VE_ASSERT_GE(m_pkFence->SetEventOnCompletion(u64FenceToWaitFor, m_kFenceEvent), S_OK);
		WaitForSingleObject(m_kFenceEvent, INFINITE);
		m_u32FramePtr = m_pkSwapChain->GetCurrentBackBufferIndex();
		m_u64FrameIndex = 0;

		m_spTargetWindow->Show();

		kRenderer.m_kRenderWindowList.attach_back(m_kNode);
	}
}
//--------------------------------------------------------------------------
void D3D12RenderWindow::Term() noexcept
{
	if (m_kNode.is_attach())
	{
		D3D12Renderer& kRenderer = *vtd::member_cast(
			&D3D12Renderer::m_kRenderWindowList, m_kNode.get_list());

		const uint64_t u64LastCompleted = m_pkFence->GetCompletedValue();
		VE_ASSERT_GE(m_pkCommandQueue->Signal(m_pkFence, m_u64FenceValue), S_OK);
		if (u64LastCompleted < m_u64FenceValue)
		{
			VE_ASSERT_GE(m_pkFence->SetEventOnCompletion(m_u64FenceValue, m_kFenceEvent), S_OK);
			WaitForSingleObject(m_kFenceEvent, INFINITE);
		}
		CloseHandle(m_kFenceEvent);
		VE_SAFE_RELEASE(m_pkFence);

		for (auto& kFrame : m_akFrameCache)
		{
			VE_SAFE_RELEASE(kFrame.m_pkTestList);

			for (auto& list : kFrame.m_kDirectCommandList)
			{
				VE_SAFE_RELEASE(list);
			}
			VE_SAFE_RELEASE(kFrame.m_pkDirectAllocator);
			kFrame.m_kDirectCommandList.clear();
			for (auto& list : kFrame.m_kBundleCommandList)
			{
				VE_SAFE_RELEASE(list);
			}
			VE_SAFE_RELEASE(kFrame.m_pkBundleAllocator);
			kFrame.m_kBundleCommandList.clear();
			kRenderer.m_kRTVHeap.Free((uint32_t)(kFrame.m_hHandle.ptr - kRenderer.m_kRTVHeap.GetCPUStart().ptr));
			VE_SAFE_RELEASE(kFrame.m_pkBufferResource);
		}
		VE_SAFE_RELEASE(m_pkCommandQueue);
		VE_SAFE_RELEASE(m_pkSwapChain);
		m_kNode.detach();
	}
}
//--------------------------------------------------------------------------
bool D3D12RenderWindow::IsValid() noexcept
{
	return VeRenderWindow::IsValid() ? m_kNode.is_attach() : false;
}
//--------------------------------------------------------------------------
void D3D12RenderWindow::Begin() noexcept
{
	VE_ASSERT(IsValid() && m_u32FramePtr < D3D12Renderer::FRAME_COUNT);
	FrameCache& kFrame = m_akFrameCache[m_u32FramePtr];
	if (kFrame.m_u64FenceValue > m_pkFence->GetCompletedValue())
	{
		VE_ASSERT_GE(m_pkFence->SetEventOnCompletion(
			kFrame.m_u64FenceValue, m_kFenceEvent), S_OK);
		WaitForSingleObject(m_kFenceEvent, INFINITE);
	}
	/*VeRendererD3D12& kRenderer = *vtd::member_cast(
		&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());
	if (kRenderer.m_kCopyResList.size())
	{
		m_pkCommandQueue->Wait(kRenderer.m_pkCopyFence, kRenderer.m_u64CopyFenceValue);
	}*/
	VE_ASSERT_GE(kFrame.m_pkDirectAllocator->Reset(), S_OK);
}
//--------------------------------------------------------------------------
void D3D12RenderWindow::End() noexcept
{
	VE_ASSERT(IsValid() && m_u32FramePtr < D3D12Renderer::FRAME_COUNT);
	FrameCache& kFrame = m_akFrameCache[m_u32FramePtr];

	VE_ASSERT_GE(kFrame.m_pkTestList->Reset(kFrame.m_pkDirectAllocator, nullptr), S_OK);

	{
		auto kTrans = BarrierTransition(kFrame.m_pkBufferResource, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		kFrame.m_pkTestList->ResourceBarrier(1, &kTrans);
	}

	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	kFrame.m_pkTestList->ClearRenderTargetView(kFrame.m_hHandle, clearColor, 0, nullptr);

	{
		auto kTrans = BarrierTransition(kFrame.m_pkBufferResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		kFrame.m_pkTestList->ResourceBarrier(1, &kTrans);
	}

	VE_ASSERT_GE(kFrame.m_pkTestList->Close(), S_OK);

	m_pkCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList*const*)&kFrame.m_pkTestList);

	VE_ASSERT_GE(m_pkSwapChain->Present(m_bSync ? 1 : 0, 0), S_OK);
	m_u32FramePtr = m_pkSwapChain->GetCurrentBackBufferIndex();
	kFrame.m_u64FenceValue = m_u64FenceValue;
	VE_ASSERT_GE(m_pkCommandQueue->Signal(m_pkFence, m_u64FenceValue++), S_OK);
	++m_u64FrameIndex;
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
