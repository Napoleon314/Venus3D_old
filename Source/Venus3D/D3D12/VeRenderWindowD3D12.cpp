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
#include "VeRenderWindowD3D12.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderWindowD3D12, VeRenderWindow);
//--------------------------------------------------------------------------
VeRenderWindowD3D12::VeRenderWindowD3D12(
	const VeWindowPtr& spWindow) noexcept : VeRenderWindow(spWindow)
{
	m_kNode._Content = this;
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
		assert_eq(VE_SUCCEEDED(kRenderer.m_pkDevice->CreateCommandQueue(
			&kQueueDesc, IID_PPV_ARGS(&m_pkCommandQueue))), true);
		DXGI_SWAP_CHAIN_DESC kSwapChainDesc = {};
		kSwapChainDesc.BufferCount = VeRendererD3D12::FRAME_COUNT;
		kSwapChainDesc.BufferDesc.Width = m_spTargetWindow->GetWidth();
		kSwapChainDesc.BufferDesc.Height = m_spTargetWindow->GetHeight();
		kSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
		kSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		kSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		kSwapChainDesc.OutputWindow = (HWND)(m_spTargetWindow->GetNativeHandle());
		kSwapChainDesc.SampleDesc.Count = 1;
		kSwapChainDesc.Windowed = TRUE;
		IDXGISwapChain* pkSwapChain;
		assert_eq(VE_SUCCEEDED(kRenderer.m_pkDXGIFactory->CreateSwapChain(
			m_pkCommandQueue, &kSwapChainDesc, &pkSwapChain)), true);
		assert_eq(VE_SUCCEEDED(pkSwapChain->QueryInterface(
			IID_PPV_ARGS(&m_pkSwapChain))), true);
		VE_SAFE_RELEASE(pkSwapChain);
		assert(m_pkCommandQueue && m_pkSwapChain);
		for (uint32_t i(0); i < VeRendererD3D12::FRAME_COUNT; ++i)
		{
			FrameCache& kFrame = m_akFrameCache[i];
			assert_ge(m_pkSwapChain->GetBuffer(i, IID_PPV_ARGS(&kFrame.m_pkBufferResource)), S_OK);
			kFrame.m_hHandle.ptr = kRenderer.m_kRTVHeap.GetCPUStart().ptr + kRenderer.m_kRTVHeap.Alloc();
			kRenderer.m_pkDevice->CreateRenderTargetView(
				kFrame.m_pkBufferResource, nullptr, kFrame.m_hHandle);
			assert_ge(kRenderer.m_pkDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(&kFrame.m_pkDirectAllocator)), S_OK);
			assert_ge(kRenderer.m_pkDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_BUNDLE,
				IID_PPV_ARGS(&kFrame.m_pkBundleAllocator)), S_OK);
			kFrame.m_u64FenceValue = 0;

			assert_ge(kRenderer.m_pkDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
				kFrame.m_pkDirectAllocator, nullptr, IID_PPV_ARGS(&kFrame.m_pkTestList)), S_OK);
			assert_ge(kFrame.m_pkTestList->Close(), S_OK);
		}

		m_u64FenceValue = 0;
		assert_ge(kRenderer.m_pkDevice->CreateFence(m_u64FenceValue++,
			D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pkFence)), S_OK);
		m_kFenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
		assert(m_kFenceEvent);
		const uint64_t u64FenceToWaitFor = m_u64FenceValue++;
		assert_ge(m_pkCommandQueue->Signal(m_pkFence, u64FenceToWaitFor), S_OK);
		assert_ge(m_pkFence->SetEventOnCompletion(u64FenceToWaitFor, m_kFenceEvent), S_OK);
		WaitForSingleObject(m_kFenceEvent, INFINITE);
		m_u32FramePtr = m_pkSwapChain->GetCurrentBackBufferIndex();
		m_u64FrameIndex = 0;

		m_spTargetWindow->Show();

		kRenderer.m_kRenderWindowList.attach_back(m_kNode);
	}
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::Term() noexcept
{
	if (m_kNode.is_attach())
	{
		VeRendererD3D12& kRenderer = *vtd::member_cast(
			&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());

		const uint64_t u64LastCompleted = m_pkFence->GetCompletedValue();
		assert_ge(m_pkCommandQueue->Signal(m_pkFence, m_u64FenceValue), S_OK);
		if (u64LastCompleted < m_u64FenceValue)
		{
			assert_ge(m_pkFence->SetEventOnCompletion(m_u64FenceValue, m_kFenceEvent), S_OK);
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
bool VeRenderWindowD3D12::IsValid() noexcept
{
	return VeRenderWindow::IsValid() ? m_kNode.is_attach() : false;
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::Begin() noexcept
{
	assert(IsValid() && m_u32FramePtr < VeRendererD3D12::FRAME_COUNT);
	FrameCache& kFrame = m_akFrameCache[m_u32FramePtr];
	if (kFrame.m_u64FenceValue > m_pkFence->GetCompletedValue())
	{
		assert_ge(m_pkFence->SetEventOnCompletion(
			kFrame.m_u64FenceValue, m_kFenceEvent), S_OK);
		WaitForSingleObject(m_kFenceEvent, INFINITE);
	}
	/*VeRendererD3D12& kRenderer = *vtd::member_cast(
		&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());
	if (kRenderer.m_kCopyResList.size())
	{
		m_pkCommandQueue->Wait(kRenderer.m_pkCopyFence, kRenderer.m_u64CopyFenceValue);
	}*/
	assert_ge(kFrame.m_pkDirectAllocator->Reset(), S_OK);
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::End() noexcept
{
	assert(IsValid() && m_u32FramePtr < VeRendererD3D12::FRAME_COUNT);
	FrameCache& kFrame = m_akFrameCache[m_u32FramePtr];

	assert_ge(kFrame.m_pkTestList->Reset(kFrame.m_pkDirectAllocator, nullptr), S_OK);

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

	assert_ge(kFrame.m_pkTestList->Close(), S_OK);

	m_pkCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList*const*)&kFrame.m_pkTestList);

	assert_ge(m_pkSwapChain->Present(m_bSync ? 1 : 0, 0), S_OK);
	m_u32FramePtr = m_pkSwapChain->GetCurrentBackBufferIndex();
	kFrame.m_u64FenceValue = m_u64FenceValue;
	assert_ge(m_pkCommandQueue->Signal(m_pkFence, m_u64FenceValue++), S_OK);
	++m_u64FrameIndex;
}
//--------------------------------------------------------------------------
