////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderWindowD3D12.cpp
//  Created:     2015/09/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "../VeMainPch.h"
#include "VeRendererD3D12.h"
#include "VeRenderWindowD3D12.h"
#include "VeRenderBufferD3D12.h"
#include "VeRenderTextureD3D12.h"

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
			VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_BUNDLE,
				IID_PPV_ARGS(&kFrame.m_pkBundleAllocator)), S_OK);
			kFrame.m_u64FenceValue = 0;
		}		

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

		m_kRecorderList.clear();
		m_kCameraList.clear();
		m_kProcessList.clear();

		for (VeInt32 i(VeRendererD3D12::FRAME_COUNT - 1); i >= 0; --i)
		{
			FrameCache& kFrame = m_akFrameCache[i];
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
void VeRenderWindowD3D12::SetupCompositorList(const VeChar8** ppcList,
	VeSizeT stNum, const VeChar8* pcHint) noexcept
{
	VeRendererD3D12& kRenderer = *VeMemberCast(
		&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());

	VeVector<VeRenderer::FrameTechnique*> kTechList;
	{		
		for (VeSizeT i(0); i < stNum; ++i)
		{
			auto it = kRenderer.m_kCompositorMap.find(ppcList[i]);
			if (it != kRenderer.m_kCompositorMap.end())
			{
				VeRenderer::FrameTechnique* pkTech = nullptr;
				if (pcHint)
				{
					VeFixedString kHint = pcHint;
					for (auto& tech : it->second->m_kTechniqueList)
					{
						if (tech.m_kName == kHint)
						{
							pkTech = &tech;
							break;
						}
					}
				}
				if (pkTech)
				{
					kTechList.push_back(pkTech);
				}
				else if(it->second->m_kTechniqueList.size())
				{
					kTechList.push_back(&(it->second->m_kTechniqueList.front()));
				}
			}
		}
	}

	struct ResourceCache
	{
		D3D12_RESOURCE_STATES m_eState;
		VeUInt32 m_u32SubRes;
		ID3D12Resource* m_apkResource[VeRendererD3D12::FRAME_COUNT];
	};
	VeVector<ResourceCache> kResourceCache;
	kResourceCache.resize(1);

	{
		ResourceCache& kCache = kResourceCache.back();
		kCache.m_eState = D3D12_RESOURCE_STATE_PRESENT;
		kCache.m_u32SubRes = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		for (VeSizeT i(0); i < VeRendererD3D12::FRAME_COUNT; ++i)
		{
			kCache.m_apkResource[i] = m_akFrameCache[i].m_pkBufferResource;
		}
	}	

	struct TargetCache
	{
		VeRenderer::FrameTarget* m_pkConfig;		
		RecordRenderTargetPtr m_spRecorder;
		VeVector<VeUInt32> m_kRTV;
		VeUInt32 m_u32DSV;
	};	
	VeVector<VeStringMap<TargetCache>> kTargets;

	kTargets.resize(kTechList.size());
	for (VeSizeT i(kTechList.size() - 1); i < kTechList.size(); --i)
	{
		VeRenderer::FrameTechnique& kTech = *kTechList[i];
		for (VeSizeT j(0); j < kTech.m_kTargetList.size(); ++j)
		{
			VeRenderer::FrameTarget& kTarget = kTech.m_kTargetList[j];
			RecordRenderTargetPtr spTarget;
			VeVector<VeUInt32> kRTV;
			VeUInt32 u32DSV;
			if (kTarget.m_eType == VeRenderer::TARGET_OUTPUT)
			{
				if (i == (kTechList.size() - 1))
				{
					spTarget = VE_NEW RecordRenderTarget();
					for (VeSizeT k(0); k < VeRendererD3D12::FRAME_COUNT; ++k)
					{
						spTarget->m_kRenderTarget[k].m_kRTVList.push_back(m_akFrameCache[k].m_hHandle);
					}
					spTarget->m_kViewport = { 0, 0, (VeFloat32)GetWidth(), (VeFloat32)GetHeight(), 0, 1 };
					spTarget->m_kScissorRect = { 0, 0, GetWidth(), GetHeight() };
					kRTV.push_back(0);
					u32DSV = VE_UINT32_MAX;
				}
				else
				{
					auto it = kTargets[i + 1].find(kTarget.m_kName);
					if (it != kTargets[i + 1].end() && it->second.m_pkConfig->m_eType == VeRenderer::TARGET_INPUT)
					{
						spTarget = it->second.m_spRecorder;
					}
				}
			}
			if (spTarget)
			{
				auto& tar = kTargets[i][kTarget.m_kName];
				tar.m_pkConfig = &kTarget;
				tar.m_spRecorder = spTarget;
				tar.m_kRTV = std::move(kRTV);
				tar.m_u32DSV = u32DSV;
			}
		}
	}

	VeUInt32 u32GCLIndex(0);
	m_kRecorderList.resize(1);
	m_kRecorderList.back().m_u32CommandIndex = 0;
	m_kRecorderList.back().m_kTaskList.clear();	

	RecordRenderTargetPtr spCurrent;
	
	for (VeSizeT i(0); i < kTechList.size(); ++i)
	{
		VeRenderer::FrameTechnique& kTech = *kTechList[i];
		auto& kMap = kTargets[i];
		for (VeSizeT j(0); j < kTech.m_kClickList.size(); ++j)
		{
			VeRenderer::FrameClick& kClick = kTech.m_kClickList[j];
			auto it = kMap.find(kClick.m_kTarget);
			if(it == kMap.end()) continue;
			TargetCache& kCache = it->second;
			{
				RecordBarrierPtr spBarrier = VE_NEW RecordBarrier();
				for (VeSizeT k(0); k < kCache.m_kRTV.size(); ++k)
				{
					ResourceCache& kRes = kResourceCache[kCache.m_kRTV[k]];
					if (!VE_MASK_HAS_ALL(kRes.m_eState, D3D12_RESOURCE_STATE_RENDER_TARGET))
					{
						for (VeSizeT l(0); l < VeRendererD3D12::FRAME_COUNT; ++l)
						{						
							spBarrier->m_kBarrierList[l].resize(spBarrier->m_kBarrierList[l].size() + 1);
							spBarrier->m_kBarrierList[l].back().Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
							spBarrier->m_kBarrierList[l].back().Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
							spBarrier->m_kBarrierList[l].back().Transition.pResource = kRes.m_apkResource[l];
							spBarrier->m_kBarrierList[l].back().Transition.Subresource = kRes.m_u32SubRes;
							spBarrier->m_kBarrierList[l].back().Transition.StateBefore = kRes.m_eState;
							spBarrier->m_kBarrierList[l].back().Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
						}
						kRes.m_eState = D3D12_RESOURCE_STATE_RENDER_TARGET;
					}					
				}
				if (kCache.m_u32DSV < kResourceCache.size())
				{
					ResourceCache& kRes = kResourceCache[kCache.m_u32DSV];
					if (!VE_MASK_HAS_ALL(kRes.m_eState, D3D12_RESOURCE_STATE_DEPTH_WRITE))
					{
						for (VeSizeT k(0); k < VeRendererD3D12::FRAME_COUNT; ++k)
						{
							spBarrier->m_kBarrierList[k].resize(spBarrier->m_kBarrierList[k].size() + 1);
							spBarrier->m_kBarrierList[k].back().Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
							spBarrier->m_kBarrierList[k].back().Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
							spBarrier->m_kBarrierList[k].back().Transition.pResource = kRes.m_apkResource[k];
							spBarrier->m_kBarrierList[k].back().Transition.Subresource = kRes.m_u32SubRes;
							spBarrier->m_kBarrierList[k].back().Transition.StateBefore = kRes.m_eState;
							spBarrier->m_kBarrierList[k].back().Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
						}
						kRes.m_eState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
					}
				}
				if (spBarrier->m_kBarrierList->size())
				{
					m_kRecorderList.back().m_kTaskList.push_back(spBarrier);
				}
			}			
			for (VeSizeT k(0); k < kClick.m_kPassList.size(); ++k)
			{
				switch (kClick.m_kPassList[k]->m_eType)
				{
				case VeRenderer::PASS_CLEAR:
				{
					VeRenderer::FrameClear& kClear = (VeRenderer::FrameClear&)*kClick.m_kPassList[k];
					if (VE_MASK_HAS_ANY(kClear.m_u32Flags, VeRenderer::CLEAR_COLOR)
						&& kClear.m_kColorArray.size())
					{
						VeSizeT stColorNum = kCache.m_spRecorder->m_kRenderTarget->m_kRTVList.size();
						for (VeSizeT l(0); l < stColorNum; ++l)
						{
							RecordClearRTV* pkTask = VE_NEW RecordClearRTV();
							for (VeSizeT m(0); m < VeRendererD3D12::FRAME_COUNT; ++m)
							{
								pkTask->m_hHandle[m] = kCache.m_spRecorder->m_kRenderTarget[m].m_kRTVList[l];
							}
							if (l < kClear.m_kColorArray.size())
							{
								pkTask->m_kColor = kClear.m_kColorArray[l];
							}
							else
							{
								pkTask->m_kColor = kClear.m_kColorArray.back();
							}
							m_kRecorderList.back().m_kTaskList.push_back(pkTask);
						}
					}
				}
				break;
				case VeRenderer::PASS_QUAD:
				{
					if (spCurrent != kCache.m_spRecorder)
					{
						m_kRecorderList.back().m_kTaskList.push_back(kCache.m_spRecorder);
						spCurrent = kCache.m_spRecorder;
					}
					VeRenderer::FrameQuad& kQuad = (VeRenderer::FrameQuad&)*kClick.m_kPassList[k];					
					ID3D12RootSignature* pkRootSignature = nullptr;
					ID3D12PipelineState* pkPipelineState = nullptr;
					auto itRoot = kRenderer.m_kRootSignatureMap.find(kQuad.m_kRootSignature);
					if(itRoot != kRenderer.m_kRootSignatureMap.end())
					{
						pkRootSignature = VeSmartPointerCast(VeRendererD3D12::RootSignatureD3D12,
							itRoot->second)->m_pkRootSignature;
					}
					auto itPSO = kRenderer.m_kPipelineStateMap.find(kQuad.m_kPipelineState);
					if (itPSO != kRenderer.m_kPipelineStateMap.end())
					{
						pkPipelineState = VeSmartPointerCast(VeRendererD3D12::PipelineStateD3D12,
							itPSO->second)->m_pkPipelineState;
					}
					if (pkRootSignature && pkPipelineState)
					{
						RecordRenderQuad* pkQuad = VE_NEW RecordRenderQuad();
						pkQuad->m_pkRootSignature = pkRootSignature;
						pkQuad->m_pkPipelineState = pkPipelineState;
						for (auto& itTab : kQuad.m_kTable)
						{
							auto itRes = kRenderer.m_kResourceMap.find(itTab.second);
							if (itRes != kRenderer.m_kResourceMap.end())
							{
								switch (itRes->second->GetDimension())
								{
								case VeRenderResource::DIMENSION_TEXTURE1D:
								case VeRenderResource::DIMENSION_TEXTURE2D:
								case VeRenderResource::DIMENSION_TEXTURE3D:								
								{
									VeRenderTextureD3D12* pkTex = VeDynamicCast(VeRenderTextureD3D12, itRes->second.p());
									if (pkTex && pkTex->m_kSRVList.size())
									{
										pkQuad->m_kTable.push_back(std::make_pair(itTab.first, pkTex->m_kSRVList.front().m_kGPUHandle));
									}
								}
								break;
								default:
									break;
								}							

							}
						}
						m_kRecorderList.back().m_kTaskList.push_back(pkQuad);
					}
				}
				break;
				default:
					break;
				}
			}
		}
	}

	if (kResourceCache[0].m_eState != D3D12_RESOURCE_STATE_PRESENT)
	{
		RecordBarrier* pkBarrier = VE_NEW RecordBarrier();
		for (VeSizeT i(0); i < VeRendererD3D12::FRAME_COUNT; ++i)
		{
			pkBarrier->m_kBarrierList[i].resize(pkBarrier->m_kBarrierList[i].size() + 1);
			pkBarrier->m_kBarrierList[i].back().Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			pkBarrier->m_kBarrierList[i].back().Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			pkBarrier->m_kBarrierList[i].back().Transition.pResource = kResourceCache[0].m_apkResource[i];
			pkBarrier->m_kBarrierList[i].back().Transition.Subresource = kResourceCache[0].m_u32SubRes;
			pkBarrier->m_kBarrierList[i].back().Transition.StateBefore = kResourceCache[0].m_eState;
			pkBarrier->m_kBarrierList[i].back().Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		}
		kResourceCache[0].m_eState = D3D12_RESOURCE_STATE_PRESENT;
		m_kRecorderList.back().m_kTaskList.push_back(pkBarrier);
	}

	m_kProcessList.resize(1);
	m_kProcessList.back().m_eType = TYPE_EXCUTE;
	m_kProcessList.back().m_u16Start = 0;
	m_kProcessList.back().m_u16Num = 1;
	++u32GCLIndex;

	ResizeDirectList(u32GCLIndex);
}
//--------------------------------------------------------------------------
VeUInt32 VeRenderWindowD3D12::GetRecorderNum() noexcept
{
	return (VeUInt32)m_kRecorderList.size();
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::Record(VeUInt32 u32Index) noexcept
{
	VeRendererD3D12& kRenderer = *VeMemberCast(
		&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());

	VE_ASSERT(u32Index < m_kRecorderList.size());
	Recorder& kRecorder = m_kRecorderList[u32Index];
	FrameCache& kFrame = m_akFrameCache[m_u32FrameIndex];
	ID3D12GraphicsCommandList* pkGCL = kFrame.m_kDirectCommandList[kRecorder.m_u32CommandIndex];
	VE_ASSERT_GE(pkGCL->Reset(kFrame.m_pkDirectAllocator, nullptr), S_OK);
	ID3D12DescriptorHeap* ppHeaps[] = { kRenderer.m_kSRVHeap.Get() };
	pkGCL->SetDescriptorHeaps(1, ppHeaps);
	for (auto& task : kRecorder.m_kTaskList)
	{
		switch (task->m_eType)
		{
		case REC_BARRIER:
		{
			auto& list = ((RecordBarrier*)task)->m_kBarrierList[m_u32FrameIndex];	
			pkGCL->ResourceBarrier((VeUInt32)list.size(), &list.front());
		}
		break;
		case REC_CLEAR_RTV:
		{
			RecordClearRTV& rec = *((RecordClearRTV*)task);
			pkGCL->ClearRenderTargetView(rec.m_hHandle[m_u32FrameIndex],
				(const FLOAT*)&(rec.m_kColor), 0, nullptr);
		}
		break;
		case REC_RENDER_TARGET:
		{
			RecordRenderTarget& rec = *((RecordRenderTarget*)task);
			auto& tar = rec.m_kRenderTarget[m_u32FrameIndex];
			pkGCL->OMSetRenderTargets((VeUInt32)tar.m_kRTVList.size(),
				&tar.m_kRTVList.front(), FALSE,
				tar.m_hDSV.ptr ? &tar.m_hDSV : nullptr);
			pkGCL->RSSetViewports(1, &rec.m_kViewport);
			pkGCL->RSSetScissorRects(1, &rec.m_kScissorRect);
		}
		break;
		case REC_RENDER_QUAD:
		{
			RecordRenderQuad& rec = *((RecordRenderQuad*)task);
			pkGCL->SetPipelineState(rec.m_pkPipelineState);
			pkGCL->SetGraphicsRootSignature(rec.m_pkRootSignature);
			for (auto& itTab : rec.m_kTable)
			{
				pkGCL->SetGraphicsRootDescriptorTable(itTab.first, itTab.second);
			}
			pkGCL->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			pkGCL->IASetVertexBuffers(0, 1, &kRenderer.m_kQuadVBV);
			pkGCL->DrawInstanced(4, 1, 0, 0);
		}
		break;
		default:
			break;
		}

	}
	VE_ASSERT_GE(pkGCL->Close(), S_OK);
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::BeginCommandLists(VeUInt32 u32Thread) noexcept
{

}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::SendDrawCallList(VeUInt32 u32Thread,
	VeUInt32 u32Camera, VeRenderDrawCall* pkDrawCallList,
	VeSizeT stNum) noexcept
{

}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::ResizeDirectList(VeSizeT stNum) noexcept
{
	for (VeInt32 i(0); i < VeRendererD3D12::FRAME_COUNT; ++i)
	{
		FrameCache& kFrame = m_akFrameCache[i];
		VeSizeT stCurrentNum = kFrame.m_kDirectCommandList.size();
		if (stNum > stCurrentNum)
		{
			kFrame.m_kDirectCommandList.resize(stNum);
			VeRendererD3D12& kRenderer = *VeMemberCast(
				&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());
			for (VeSizeT i(stCurrentNum); i < stNum; ++i)
			{
				VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandList(0,
					D3D12_COMMAND_LIST_TYPE_DIRECT, kFrame.m_pkDirectAllocator,
					nullptr, IID_PPV_ARGS(&kFrame.m_kDirectCommandList[i])), S_OK);
				VE_ASSERT_GE(kFrame.m_kDirectCommandList[i]->Close(), S_OK);
			}
		}
		else if (stNum < stCurrentNum)
		{
			for (VeSizeT i(stNum); i < stCurrentNum; ++i)
			{
				VE_SAFE_RELEASE(kFrame.m_kDirectCommandList[i]);
			}
			kFrame.m_kDirectCommandList.resize(stNum);
		}
	}
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::ResizeBundleList(VeSizeT stNum) noexcept
{
	for (VeInt32 i(0); i < VeRendererD3D12::FRAME_COUNT; ++i)
	{
		FrameCache& kFrame = m_akFrameCache[i];
		VE_ASSERT_GE(kFrame.m_pkBundleAllocator->Reset(), S_OK);
		VeSizeT stCurrentNum = kFrame.m_kBundleCommandList.size();
		if (stNum >= stCurrentNum)
		{
			for (auto& list : kFrame.m_kBundleCommandList)
			{
				VE_ASSERT_GE(list->Reset(kFrame.m_pkBundleAllocator, nullptr), S_OK);
			}
			kFrame.m_kBundleCommandList.resize(stNum);
			VeRendererD3D12& kRenderer = *VeMemberCast(
				&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());
			for (VeSizeT i(stCurrentNum); i < stNum; ++i)
			{
				VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommandList(0,
					D3D12_COMMAND_LIST_TYPE_BUNDLE, kFrame.m_pkBundleAllocator,
					nullptr, IID_PPV_ARGS(&kFrame.m_kBundleCommandList[i])), S_OK);
			}
		}
		else if (stNum < stCurrentNum)
		{
			for (VeSizeT i(stNum); i < stCurrentNum; ++i)
			{
				VE_ASSERT_GE(kFrame.m_kBundleCommandList[i]->Close(), S_OK);
				VE_SAFE_RELEASE(kFrame.m_kBundleCommandList[i]);
			}
			kFrame.m_kBundleCommandList.resize(stNum);
		}
	}	
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::Begin() noexcept
{
	VE_ASSERT(m_kNode.is_attach()
		&& m_u32FrameIndex < VeRendererD3D12::FRAME_COUNT);
	FrameCache& kFrame = m_akFrameCache[m_u32FrameIndex];
	if (kFrame.m_u64FenceValue > m_pkFence->GetCompletedValue())
	{
		VE_ASSERT_GE(m_pkFence->SetEventOnCompletion(
			kFrame.m_u64FenceValue, m_kFenceEvent), S_OK);
		WaitForSingleObject(m_kFenceEvent, INFINITE);
	}
	VeRendererD3D12& kRenderer = *VeMemberCast(
		&VeRendererD3D12::m_kRenderWindowList, m_kNode.get_list());
	if (kRenderer.m_kCopyResList.size())
	{
		m_pkCommandQueue->Wait(kRenderer.m_pkCopyFence, kRenderer.m_u64CopyFenceValue);
	}
	VE_ASSERT_GE(kFrame.m_pkDirectAllocator->Reset(), S_OK);
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::End() noexcept
{	
	FrameCache& kFrame = m_akFrameCache[m_u32FrameIndex];
	ID3D12CommandList*const* pkGCL = (ID3D12CommandList*const*)&kFrame.m_kDirectCommandList.front();

	for (auto& process : m_kProcessList)
	{
		switch (process.m_eType)
		{
		case TYPE_EXCUTE:
			m_pkCommandQueue->ExecuteCommandLists(process.m_u16Num,
				pkGCL + process.m_u16Start);
			break;
		default:
			break;
		}
	}

	VE_ASSERT_GE(m_pkSwapChain->Present(m_bSync? 1 : 0, 0), S_OK);
	m_u32FrameIndex = m_pkSwapChain->GetCurrentBackBufferIndex();
	kFrame.m_u64FenceValue = m_u64FenceValue;
	VE_ASSERT_GE(m_pkCommandQueue->Signal(m_pkFence, m_u64FenceValue++), S_OK);
}
//--------------------------------------------------------------------------
