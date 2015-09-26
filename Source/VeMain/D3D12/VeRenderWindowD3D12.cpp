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

		m_kResourceMap.clear();
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
bool VeRenderWindowD3D12::HasChange(BarrierPath& kPath,
	VeUInt32 u32SubResource, D3D12_RESOURCE_STATES eState) noexcept
{
	if (u32SubResource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES)
	{
		if (kPath.m_eAllState != eState) return true;
		for (auto state : kPath.m_kStates)
		{
			if (state != eState) return true;
		}
		return false;
	}
	else
	{
		if (kPath.m_kStates.size() > u32SubResource)
		{
			return kPath.m_kStates[u32SubResource] != eState;
		}
		else
		{
			return kPath.m_eAllState != eState;
		}
	}
}
//--------------------------------------------------------------------------
bool VeRenderWindowD3D12::IsNeighbor(RecordBarrierMap& kBarriers,
	BarrierPath& kPath, VeSizeT i, VeSizeT j) noexcept
{
	VE_ASSERT(kPath.i != i || kPath.j != j);
	if (kPath.i == i && kPath.j + 1 == j) return true;
	if (j == 0 && kPath.i + 1 == i
		&& (kBarriers[kPath.i].size() - 1 == kPath.j))
		return true;
	return false;
}
//--------------------------------------------------------------------------
D3D12_RESOURCE_STATES VeRenderWindowD3D12::GetState(BarrierPath& kPath,
	VeUInt32 u32SubResource) noexcept
{
	if (kPath.m_kStates.empty())
	{
		return kPath.m_eAllState;
	}
	else if (u32SubResource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES)
	{
		for (auto state : kPath.m_kStates)
		{
			if (state != kPath.m_eAllState) return D3D12_RESOURCE_STATE_COMMON;
		}
		return kPath.m_eAllState;
	}
	else if (u32SubResource < kPath.m_kStates.size())
	{
		return kPath.m_kStates[u32SubResource];
	}
	else
	{
		return kPath.m_eAllState;
	}
}
//--------------------------------------------------------------------------
std::pair<VeRenderWindowD3D12::RecordViewportPtr,
	VeRenderWindowD3D12::RecordScissorRectPtr>&
VeRenderWindowD3D12::Get(RectMap& kMap,
	std::pair<VeUInt32, VeUInt32>& kResolution) noexcept
{
	auto& data = kMap[kResolution];
	if (!data.first)
	{
		data.first = VE_NEW RecordViewport();
		data.first->m_kViewportList.resize(1);
		data.first->m_kViewportList[0].TopLeftX = 0;
		data.first->m_kViewportList[0].TopLeftY = 0;
		data.first->m_kViewportList[0].Width = kResolution.first;
		data.first->m_kViewportList[0].Height = kResolution.second;
		data.first->m_kViewportList[0].MinDepth = 0;
		data.first->m_kViewportList[0].MaxDepth = 1;
	}
	if (!data.second)
	{
		data.second = VE_NEW RecordScissorRect();
		data.second->m_kScissorRectList.resize(1);
		data.second->m_kScissorRectList[0].left = 0;
		data.second->m_kScissorRectList[0].top = 0;
		data.second->m_kScissorRectList[0].right = kResolution.first;
		data.second->m_kScissorRectList[0].bottom = kResolution.second;
	}
	return data;
}
//--------------------------------------------------------------------------
std::pair<VeUInt32, VeUInt32> VeRenderWindowD3D12::GetResolution(
	TargetCache& tar) noexcept
{
	if(tar.m_kRTVList.size())
	{
		return std::make_pair(tar.m_kRTVList.front().m_u32Width,
			tar.m_kRTVList.front().m_u32Height);
	}
	VE_ASSERT(tar.m_kDSV.m_pkResource);
	return std::make_pair(tar.m_kDSV.m_u32Width, tar.m_kDSV.m_u32Height);
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::BarrierEvent(RecordBarrierMap& kBarriers,
	BarrierPathCache& kResPathCache, VeSizeT i, VeSizeT j,
	ID3D12Resource* pkResource, VeUInt32 u32SubResource,
	D3D12_RESOURCE_STATES eState) noexcept
{
	auto& path = kResPathCache[pkResource];
	BarrierPath kCurrent;
	kCurrent.i = i;
	kCurrent.j = j;
	if (u32SubResource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES
		|| path.empty() || path.back().m_kStates.empty())
	{
		kCurrent.m_eAllState = eState;
	}
	else
	{
		kCurrent.m_eAllState = path.back().m_eAllState;
		kCurrent.m_kStates = path.back().m_kStates;
		if (kCurrent.m_kStates.size() <= u32SubResource)
		{
			VeSizeT stStart = kCurrent.m_kStates.size();
			kCurrent.m_kStates.resize(u32SubResource + 1);
			for (VeSizeT k(stStart); k < kCurrent.m_kStates.size(); ++k)
			{
				kCurrent.m_kStates[k] = kCurrent.m_eAllState;
			}
		}
		kCurrent.m_kStates[u32SubResource] = eState;
	}
	if (path.empty())
	{
		RecordBarrierPtr& spBarrier = kBarriers[i][j].first;
		if (!spBarrier) spBarrier = VE_NEW RecordBarrier();
		for (VeSizeT k(0); k < VeRendererD3D12::FRAME_COUNT; ++k)
		{
			spBarrier->m_akBarrierList[k].resize(spBarrier->m_akBarrierList[k].size() + 1);
			D3D12_RESOURCE_BARRIER& kBar = spBarrier->m_akBarrierList[k].back();
			kBar.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			kBar.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			if (pkResource)
			{
				kBar.Transition.pResource = pkResource;
			}
			else
			{
				kBar.Transition.pResource = m_akFrameCache[k].m_pkBufferResource;
			}
			kBar.Transition.Subresource = u32SubResource;
			kBar.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
			kBar.Transition.StateAfter = eState;
		}
	}
	else
	{
		for (auto it = path.rbegin(); it != path.rend(); ++it)
		{
			if (HasChange(*it, u32SubResource, eState))
			{
				if (IsNeighbor(kBarriers, *it, i, j))
				{
					RecordBarrierPtr& spBarrier = kBarriers[i][j].first;
					if (!spBarrier) spBarrier = VE_NEW RecordBarrier();
					for (VeSizeT k(0); k < VeRendererD3D12::FRAME_COUNT; ++k)
					{
						spBarrier->m_akBarrierList[k].resize(spBarrier->m_akBarrierList[k].size() + 1);
						D3D12_RESOURCE_BARRIER& kBar = spBarrier->m_akBarrierList[k].back();
						kBar.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
						kBar.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
						if (pkResource)
						{
							kBar.Transition.pResource = pkResource;
						}
						else
						{
							kBar.Transition.pResource = m_akFrameCache[k].m_pkBufferResource;
						}
						if (kCurrent.m_kStates.empty())
						{
							kBar.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
							
						}
						else
						{
							kBar.Transition.Subresource = u32SubResource;
						}
						kBar.Transition.StateBefore = GetState(*it, u32SubResource);						
						kBar.Transition.StateAfter = eState;
					}
				}
				else
				{
					RecordBarrierPtr& spStart = kBarriers[it->i][it->j].second;
					if (!spStart) spStart = VE_NEW RecordBarrier();
					RecordBarrierPtr& spEnd = kBarriers[i][j].first;
					if (!spEnd) spEnd = VE_NEW RecordBarrier();

					for (VeSizeT k(0); k < VeRendererD3D12::FRAME_COUNT; ++k)
					{
						spStart->m_akBarrierList[k].resize(spStart->m_akBarrierList[k].size() + 1);
						spEnd->m_akBarrierList[k].resize(spEnd->m_akBarrierList[k].size() + 1);
						D3D12_RESOURCE_BARRIER& kStart = spStart->m_akBarrierList[k].back();
						D3D12_RESOURCE_BARRIER& kEnd = spEnd->m_akBarrierList[k].back();
						kStart.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
						kEnd.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
						kStart.Flags = D3D12_RESOURCE_BARRIER_FLAG_BEGIN_ONLY;
						kEnd.Flags = D3D12_RESOURCE_BARRIER_FLAG_END_ONLY;
						if (pkResource)
						{
							kStart.Transition.pResource = pkResource;
							kEnd.Transition.pResource = pkResource;
						}
						else
						{
							kStart.Transition.pResource = m_akFrameCache[k].m_pkBufferResource;
							kEnd.Transition.pResource = m_akFrameCache[k].m_pkBufferResource;
						}
						if (kCurrent.m_kStates.empty())
						{
							kStart.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
							kEnd.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
						}
						else
						{
							kStart.Transition.Subresource = u32SubResource;
							kEnd.Transition.Subresource = u32SubResource;
						}
						kStart.Transition.StateBefore = GetState(*it, u32SubResource);
						kEnd.Transition.StateBefore = GetState(*it, u32SubResource);
						kStart.Transition.StateAfter = eState;
						kEnd.Transition.StateAfter = eState;
					}

				}
				break;
			}
		}
	}
	path.push_back(kCurrent);
}
//--------------------------------------------------------------------------
void VeRenderWindowD3D12::SetupCompositorList(const VeChar8** ppcList,
	VeSizeT stNum, VeUInt32 u32ThreadNum, const VeChar8* pcHint) noexcept
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

	{
		m_kResourceMap.clear();
		VeStringMap<VeFloat64> kValueMap;
		kValueMap["screen_w"] = (VeFloat64)GetWidth();
		kValueMap["screen_h"] = (VeFloat64)GetHeight();

		for (auto pTech : kTechList)
		{
			for (auto& res : pTech->m_kResourceList)
			{
				auto it = m_kResourceMap.find(res.m_kName);
				if (it == m_kResourceMap.end())
				{
					VeUInt32 w = (VeUInt32)ve_parser.CalculateExpression(
						kValueMap, res.m_kWidth, 0);
					VeUInt32 h = (VeUInt32)ve_parser.CalculateExpression(
						kValueMap, res.m_kHeight, 0);
					VeUInt16 d = (VeUInt16)ve_parser.CalculateExpression(
						kValueMap, res.m_kDepth, 0);
					VeUInt16 m = (VeUInt16)ve_parser.CalculateExpression(
						kValueMap, res.m_kMipLevels, 0);
					VeUInt16 c = (VeUInt16)ve_parser.CalculateExpression(
						kValueMap, res.m_kCount, 0);
					VeUInt16 q = (VeUInt16)ve_parser.CalculateExpression(
						kValueMap, res.m_kQuality, 0);
					VeUInt32 u32Use(0);
					if (res.m_kSRVList.size()) u32Use |= VeRenderTexture::USEAGE_SRV;
					if (res.m_kRTVList.size()) u32Use |= VeRenderTexture::USEAGE_RTV;
					if (res.m_kDSVList.size()) u32Use |= VeRenderTexture::USEAGE_DSV;

					VeRenderTextureD3D12* pkTexture = VE_NEW VeRenderTextureD3D12(
						res.m_eDimension, (VeRenderTexture::Useage)u32Use,
						res.m_eFormat, w, h, d, m, c, q);
					pkTexture->Init(kRenderer);
					if (res.m_kSRVList.size())
					{
						pkTexture->SetSRVNum((VeUInt32)res.m_kSRVList.size());
						for (VeUInt32 i(0); i < res.m_kSRVList.size(); ++i)
						{
							auto& srv = res.m_kSRVList[i];
							pkTexture->SetSRV(i, srv.m_eType, srv.m_eFormat,
								srv.m_u32Param0, srv.m_u32Param1);
						}
					}
					if (res.m_kRTVList.size())
					{
						pkTexture->SetRTVNum((VeUInt32)res.m_kRTVList.size());
						for (VeUInt32 i(0); i < res.m_kRTVList.size(); ++i)
						{
							auto& rtv = res.m_kRTVList[i];
							pkTexture->SetRTV(i, rtv.m_eType, rtv.m_eFormat,
								rtv.m_u32Param0, rtv.m_u32Param1);
						}
					}
					if (res.m_kDSVList.size())
					{
						pkTexture->SetDSVNum((VeUInt32)res.m_kDSVList.size());
						for (VeUInt32 i(0); i < res.m_kDSVList.size(); ++i)
						{
							auto& dsv = res.m_kDSVList[i];
							pkTexture->SetDSV(i, dsv.m_eType, dsv.m_eFormat,
								dsv.m_u32Param0, dsv.m_u32Param1);
						}
					}
					m_kResourceMap[res.m_kName] = pkTexture;
				}
			}
		}
	}
	
	VeVector<VeStringMap<TargetCache>> kTargets;

	kTargets.resize(kTechList.size());
	for (VeSizeT i(kTechList.size() - 1); i < kTechList.size(); --i)
	{
		VeRenderer::FrameTechnique& kTech = *kTechList[i];
		for (VeSizeT j(0); j < kTech.m_kTargetList.size(); ++j)
		{
			VeRenderer::FrameTarget& kTarget = kTech.m_kTargetList[j];
			RecordRenderTargetPtr spTarget;
			VeVector<ViewData> kRTVList;
			ViewData kDSV;
			if (kTarget.m_eType == VeRenderer::TARGET_OUTPUT)
			{
				if (i == (kTechList.size() - 1))
				{
					spTarget = VE_NEW RecordRenderTarget();
					for (VeSizeT k(0); k < VeRendererD3D12::FRAME_COUNT; ++k)
					{
						spTarget->m_akRTVList[k].push_back(m_akFrameCache[k].m_hHandle);
						spTarget->m_ahDSV[k].ptr = 0;
					}
					kRTVList.resize(kRTVList.size() + 1);
					kRTVList.back().m_u32Width = GetWidth();
					kRTVList.back().m_u32Height = GetHeight();
					kRTVList.back().m_u32Depth = 1;
					kRTVList.back().m_u32SubResource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
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
			else
			{
				spTarget = VE_NEW RecordRenderTarget();
				for (auto& rtv : kTarget.m_kRTVList)
				{
					auto it = m_kResourceMap.find(rtv.m_kResName);
					if (it != m_kResourceMap.end())
					{
						if (rtv.m_u32Index < it->second->m_kRTVList.size())
						{
							auto& kView = it->second->m_kRTVList[rtv.m_u32Index];
							for (VeSizeT k(0); k < VeRendererD3D12::FRAME_COUNT; ++k)
							{
								spTarget->m_akRTVList[k].push_back(kView.m_hCPUHandle);
							}
							kRTVList.resize(kRTVList.size() + 1);
							kRTVList.back().m_pkResource = it->second->m_pkResource;
							kRTVList.back().m_u32Width = kView.m_u32Width;
							kRTVList.back().m_u32Height = kView.m_u32Height;
							kRTVList.back().m_u32Depth = kView.m_u32Depth;
							kRTVList.back().m_u32SubResource = kView.m_u32SubResource;
						}
					}
				}
				auto it = m_kResourceMap.find(kTarget.m_kDSV.m_kResName);
				if (it != m_kResourceMap.end())
				{
					if (kTarget.m_kDSV.m_u32Index < it->second->m_kDSVList.size())
					{
						auto& kView = it->second->m_kDSVList[kTarget.m_kDSV.m_u32Index];
						for (VeSizeT k(0); k < VeRendererD3D12::FRAME_COUNT; ++k)
						{
							spTarget->m_ahDSV[k] = kView.m_hCPUHandle;
						}						
						kDSV.m_pkResource = it->second->m_pkResource;
						kDSV.m_u32Width = kView.m_u32Width;
						kDSV.m_u32Height = kView.m_u32Height;
						kDSV.m_u32Depth = kView.m_u32Depth;
						kDSV.m_u32SubResource = kView.m_u32SubResource;
					}
				}
			}

			if (spTarget)
			{
				auto& tar = kTargets[i][kTarget.m_kName];
				VE_ASSERT(!tar.m_pkConfig);
				tar.m_pkConfig = &kTarget;
				tar.m_spRecorder = spTarget;
				tar.m_kRTVList = std::move(kRTVList);
				tar.m_kDSV = kDSV;
			}
		}
	}

	RecordBarrierMap kBarriers;
	kBarriers.resize(kTechList.size());
	
	BarrierPathCache kResPathCache;
	VeMap<ID3D12Resource*, VeMap<VeUInt32, D3D12_RESOURCE_STATES>> kClickCache;

	for (VeSizeT i(0); i < kTechList.size(); ++i)
	{
		auto& kMap = kTargets[i];
		VeRenderer::FrameTechnique& kTech = *kTechList[i];
		kBarriers[i].resize(kTech.m_kClickList.size());
		for (VeSizeT j(0); j < kTech.m_kClickList.size(); ++j)
		{			
			VeRenderer::FrameClick& kClick = kTech.m_kClickList[j];
			auto it = kMap.find(kClick.m_kTarget);
			if (it == kMap.end()) continue;
			auto& tar = it->second;
			kClickCache.clear();
			for (auto rtv : tar.m_kRTVList)
			{
				kClickCache[rtv.m_pkResource][rtv.m_u32SubResource] = D3D12_RESOURCE_STATE_RENDER_TARGET;
			}
			if(tar.m_kDSV.m_pkResource)
			{
				kClickCache[tar.m_kDSV.m_pkResource][tar.m_kDSV.m_u32SubResource] = D3D12_RESOURCE_STATE_DEPTH_WRITE;
			}
			for (auto& con : kClick.m_kContextList)
			{
				auto itRes = m_kResourceMap.find(con);
				if (itRes != m_kResourceMap.end())
				{					
					auto& cache = kClickCache[itRes->second->m_pkResource];
					cache.clear();
					cache[D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES] = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				}
			}
			for (auto& cache : kClickCache)
			{
				for (auto& state : cache.second)
				{
					BarrierEvent(kBarriers, kResPathCache, i, j,
						cache.first, state.first, state.second);
				}
			}
		}
	}

	VeUInt32 u32GCLIndex(0);
	bool bScene = false;

	m_kRecorderList.resize(1);
	m_kRecorderList.back().m_u32CommandIndex = 0;
	m_kRecorderList.back().m_kTaskList.clear();

	m_kProcessList.resize(1);
	m_kProcessList.back().m_eType = TYPE_EXCUTE;
	m_kProcessList.back().m_u16Start = 0;
	m_kProcessList.back().m_u16Num = 1;

	m_kCameraList.clear();

	RectMap kRectMap;

	RecordRenderTargetPtr spCurrent;
	RecordViewportPtr spViewport;
	RecordScissorRectPtr spScissorRect;
	
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

			auto& bar = kBarriers[i][j];
			if(bar.first)
			{
				if (bScene)
				{
					if (m_kProcessList.back().m_eType == TYPE_EXCUTE)
					{
						m_kProcessList.back().m_u16Num += 1;
					}
					else
					{
						m_kProcessList.resize(m_kProcessList.size() + 1);
						m_kProcessList.back().m_eType = TYPE_EXCUTE;
						m_kProcessList.back().m_u16Start = u32GCLIndex + 1;
						m_kProcessList.back().m_u16Num = 1;
					}
					++u32GCLIndex;
					m_kRecorderList.resize(m_kRecorderList.size() + 1);
					m_kRecorderList.back().m_u32CommandIndex = u32GCLIndex;
					m_kRecorderList.back().m_kTaskList.clear();
					bScene = false;
				}
				m_kRecorderList.back().m_kTaskList.push_back(bar.first);
			}			
			for (VeSizeT k(0); k < kClick.m_kPassList.size(); ++k)
			{
				switch (kClick.m_kPassList[k]->m_eType)
				{
				case VeRenderer::PASS_CLEAR:
				{
					if (bScene)
					{
						if (m_kProcessList.back().m_eType == TYPE_EXCUTE)
						{
							m_kProcessList.back().m_u16Num += 1;
						}
						else
						{
							m_kProcessList.resize(m_kProcessList.size() + 1);
							m_kProcessList.back().m_eType = TYPE_EXCUTE;
							m_kProcessList.back().m_u16Start = u32GCLIndex + 1;
							m_kProcessList.back().m_u16Num = 1;
						}
						++u32GCLIndex;
						m_kRecorderList.resize(m_kRecorderList.size() + 1);
						m_kRecorderList.back().m_u32CommandIndex = u32GCLIndex;
						m_kRecorderList.back().m_kTaskList.clear();
						bScene = false;
					}
					VeRenderer::FrameClear& kClear = (VeRenderer::FrameClear&)*kClick.m_kPassList[k];
					if (VE_MASK_HAS_ANY(kClear.m_u32Flags, VeRenderer::CLEAR_COLOR)
						&& kClear.m_kColorArray.size())
					{
						VeSizeT stColorNum = kCache.m_spRecorder->m_akRTVList->size();
						for (VeSizeT l(0); l < stColorNum; ++l)
						{
							RecordClearRTV* pkTask = VE_NEW RecordClearRTV();
							for (VeSizeT m(0); m < VeRendererD3D12::FRAME_COUNT; ++m)
							{
								pkTask->m_ahHandle[m] = kCache.m_spRecorder->m_akRTVList[m][l];
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
					if (VE_MASK_HAS_ANY(kClear.m_u32Flags, VeRenderer::CLEAR_DEPTH)
						|| VE_MASK_HAS_ANY(kClear.m_u32Flags, VeRenderer::CLEAR_STENCIL))
					{
						if (kCache.m_spRecorder->m_ahDSV->ptr)
						{
							RecordClearDSV* pkTask = VE_NEW RecordClearDSV();
							pkTask->m_eFlags = VE_TMIN(D3D12_CLEAR_FLAGS);
							if (VE_MASK_HAS_ANY(kClear.m_u32Flags, VeRenderer::CLEAR_DEPTH))
							{
								pkTask->m_eFlags |= D3D12_CLEAR_FLAG_DEPTH;
							}
							if (VE_MASK_HAS_ANY(kClear.m_u32Flags, VeRenderer::CLEAR_STENCIL))
							{
								pkTask->m_eFlags |= D3D12_CLEAR_FLAG_STENCIL;
							}
							pkTask->m_f32Depth = kClear.m_f32Depth;
							pkTask->m_u8Stencil = kClear.m_u8Stencil;
							for (VeSizeT m(0); m < VeRendererD3D12::FRAME_COUNT; ++m)
							{
								pkTask->m_ahHandle[m] = kCache.m_spRecorder->m_ahDSV[m];
							}							
							m_kRecorderList.back().m_kTaskList.push_back(pkTask);
						}
					}
				}
				break;
				case VeRenderer::PASS_QUAD:
				{
					if (bScene)
					{
						if (m_kProcessList.back().m_eType == TYPE_EXCUTE)
						{
							m_kProcessList.back().m_u16Num += 1;
						}
						else
						{
							m_kProcessList.resize(m_kProcessList.size() + 1);
							m_kProcessList.back().m_eType = TYPE_EXCUTE;
							m_kProcessList.back().m_u16Start = u32GCLIndex + 1;
							m_kProcessList.back().m_u16Num = 1;
						}
						++u32GCLIndex;
						m_kRecorderList.resize(m_kRecorderList.size() + 1);
						m_kRecorderList.back().m_u32CommandIndex = u32GCLIndex;
						m_kRecorderList.back().m_kTaskList.clear();
						bScene = false;
					}
					if (spCurrent != kCache.m_spRecorder)
					{
						m_kRecorderList.back().m_kTaskList.push_back(kCache.m_spRecorder);
						spCurrent = kCache.m_spRecorder;
					}
					auto& rect = Get(kRectMap, GetResolution(kCache));
					if (spViewport != rect.first)
					{
						m_kRecorderList.back().m_kTaskList.push_back(rect.first);
						spViewport = rect.first;
					}
					if (spScissorRect != rect.second)
					{
						m_kRecorderList.back().m_kTaskList.push_back(rect.second);
						spScissorRect = rect.second;
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
							bool bFinished = false;
							for (auto context : kClick.m_kContextList)
							{
								if (context == itTab.second)
								{
									auto itRTRes = m_kResourceMap.find(itTab.second);
									if (itRTRes != m_kResourceMap.end())
									{
										VeRenderTextureD3D12* pkTex = itRTRes->second;
										if (pkTex->m_kSRVList.size())
										{
											pkQuad->m_kTable.push_back(std::make_pair(itTab.first, pkTex->m_kSRVList.front().m_hGPUHandle));
											bFinished = true;
										}										
										break;
									}
								}
							}
							if(!bFinished)
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
											pkQuad->m_kTable.push_back(std::make_pair(itTab.first, pkTex->m_kSRVList.front().m_hGPUHandle));
										}
									}
									break;
									default:
										break;
									}
								}
							}							
						}
						m_kRecorderList.back().m_kTaskList.push_back(pkQuad);
					}
				}
				break;
				case VeRenderer::PASS_SCENE:
				{
					VeRenderer::FrameScene& kScene = (VeRenderer::FrameScene&)*kClick.m_kPassList[k];
					if (!bScene)
					{
						spCurrent = nullptr;
						spViewport = nullptr;
						spScissorRect = nullptr;
						bScene = true;
					}
					m_kCameraList.resize(m_kCameraList.size() + 1);
					m_kCameraList.back().m_u32CameraMask = VE_MASK(kScene.m_u32Camera);
					m_kCameraList.back().m_kStageList.resize(kScene.m_u32Stage);
					for (auto& stage : m_kCameraList.back().m_kStageList)
					{
						stage = u32GCLIndex + 1;
						if (m_kProcessList.back().m_eType == TYPE_EXCUTE)
						{
							m_kProcessList.back().m_u16Num += u32ThreadNum;
						}
						else
						{
							m_kProcessList.resize(m_kProcessList.size() + 1);
							m_kProcessList.back().m_eType = TYPE_EXCUTE;
							m_kProcessList.back().m_u16Start = stage;
							m_kProcessList.back().m_u16Num = u32ThreadNum;
						}
						u32GCLIndex += u32ThreadNum;
					}		
				}
				break;
				default:
					break;
				}
			}		
			if (bar.second)
			{
				if (bScene)
				{
					if (m_kProcessList.back().m_eType == TYPE_EXCUTE)
					{
						m_kProcessList.back().m_u16Num += 1;
					}
					else
					{
						m_kProcessList.resize(m_kProcessList.size() + 1);
						m_kProcessList.back().m_eType = TYPE_EXCUTE;
						m_kProcessList.back().m_u16Start = u32GCLIndex + 1;
						m_kProcessList.back().m_u16Num = 1;
					}
					++u32GCLIndex;
					m_kRecorderList.resize(m_kRecorderList.size() + 1);
					m_kRecorderList.back().m_u32CommandIndex = u32GCLIndex;
					m_kRecorderList.back().m_kTaskList.clear();
					bScene = false;
				}
				m_kRecorderList.back().m_kTaskList.push_back(bar.second);
			}
		}
	}

	auto it = kResPathCache.find(nullptr);
	if (it != kResPathCache.end())
	{
		if (it->second.size())
		{
			D3D12_RESOURCE_STATES eResPresent = GetState(it->second.back(), D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
			RecordBarrier* pkBarrier = VE_NEW RecordBarrier();
			for (VeSizeT i(0); i < VeRendererD3D12::FRAME_COUNT; ++i)
			{
				pkBarrier->m_akBarrierList[i].resize(pkBarrier->m_akBarrierList[i].size() + 1);
				pkBarrier->m_akBarrierList[i].back().Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				pkBarrier->m_akBarrierList[i].back().Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				pkBarrier->m_akBarrierList[i].back().Transition.pResource = m_akFrameCache[i].m_pkBufferResource;
				pkBarrier->m_akBarrierList[i].back().Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				pkBarrier->m_akBarrierList[i].back().Transition.StateBefore = eResPresent;
				pkBarrier->m_akBarrierList[i].back().Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			}
			if (bScene)
			{
				if (m_kProcessList.back().m_eType == TYPE_EXCUTE)
				{
					m_kProcessList.back().m_u16Num += 1;
				}
				else
				{
					m_kProcessList.resize(m_kProcessList.size() + 1);
					m_kProcessList.back().m_eType = TYPE_EXCUTE;
					m_kProcessList.back().m_u16Start = u32GCLIndex + 1;
					m_kProcessList.back().m_u16Num = 1;
				}
				++u32GCLIndex;
				m_kRecorderList.resize(m_kRecorderList.size() + 1);
				m_kRecorderList.back().m_u32CommandIndex = u32GCLIndex;
				m_kRecorderList.back().m_kTaskList.clear();
				bScene = false;
			}
			m_kRecorderList.back().m_kTaskList.push_back(pkBarrier);
		}
	}

	m_u32ThreadNum = u32ThreadNum;
	ResizeDirectList(u32GCLIndex + 1);
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
			auto& list = ((RecordBarrier*)task)->m_akBarrierList[m_u32FrameIndex];
			pkGCL->ResourceBarrier((VeUInt32)list.size(), &list.front());
		}
		break;
		case REC_CLEAR_RTV:
		{
			RecordClearRTV& rec = *((RecordClearRTV*)task);
			pkGCL->ClearRenderTargetView(rec.m_ahHandle[m_u32FrameIndex],
				(const FLOAT*)&(rec.m_kColor), 0, nullptr);
		}
		break;
		case REC_CLEAR_DSV:
		{
			RecordClearDSV& rec = *((RecordClearDSV*)task);
			pkGCL->ClearDepthStencilView(rec.m_ahHandle[m_u32FrameIndex],
				rec.m_eFlags, rec.m_f32Depth, rec.m_u8Stencil, 0, nullptr);
		}
		break;
		case REC_RENDER_TARGET:
		{
			RecordRenderTarget& rec = *((RecordRenderTarget*)task);
			pkGCL->OMSetRenderTargets((VeUInt32)rec.m_akRTVList[m_u32FrameIndex].size(),
				&rec.m_akRTVList[m_u32FrameIndex].front(), FALSE,
				rec.m_ahDSV[m_u32FrameIndex].ptr ? &rec.m_ahDSV[m_u32FrameIndex] : nullptr);
		}
		break;
		case REC_VIEWPORT:
		{
			RecordViewport& rec = *((RecordViewport*)task);
			pkGCL->RSSetViewports((VeUInt32)rec.m_kViewportList.size(), &rec.m_kViewportList.front());
		}
		break;
		case REC_SCISSOR_RECT:
		{
			RecordScissorRect& rec = *((RecordScissorRect*)task);
			pkGCL->RSSetScissorRects((VeUInt32)rec.m_kScissorRectList.size(), &rec.m_kScissorRectList.front());
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
void VeRenderWindowD3D12::RecordScene(VeUInt32 u32Thread) noexcept
{
	for (auto& cam : m_kCameraList)
	{
		for (auto idx : cam.m_kStageList)
		{
			FrameCache& kFrame = m_akFrameCache[m_u32FrameIndex];
			ID3D12GraphicsCommandList* pkGCL = kFrame.m_kDirectCommandList[idx + u32Thread];
			VE_ASSERT_GE(pkGCL->Reset(kFrame.m_pkDirectAllocator, nullptr), S_OK);

			VE_ASSERT_GE(pkGCL->Close(), S_OK);
		}
	}
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
