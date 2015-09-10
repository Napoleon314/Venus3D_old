////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderBufferD3D12.cpp
//  Created:     2015/09/11 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "../VeMainPch.h"
#include "VeRendererD3D12.h"
#include "VeRenderBufferD3D12.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderBufferD3D12, VeRenderBuffer);
//--------------------------------------------------------------------------
VeRenderBufferD3D12::VeRenderBufferD3D12(Type eType,
	Useage eUse, VeUInt32 u32Size) noexcept
	: VeRenderBuffer(eType, eUse, u32Size)
{
	m_kNode.m_Content = this;
}
//--------------------------------------------------------------------------
VeRenderBufferD3D12::~VeRenderBufferD3D12() noexcept
{
	VE_ASSERT(!m_pkResource);
	VE_ASSERT(!m_kNode.is_attach());
}
//--------------------------------------------------------------------------
VeRendererD3D12& VeRenderBufferD3D12::GetRenderer() noexcept
{
	VE_ASSERT(m_kNode.is_attach());
	return *VeMemberCast(&VeRendererD3D12::m_kRenderBufferList,
		m_kNode.get_list());
}
//--------------------------------------------------------------------------
VeRTTIImpl(VeStaticBufferD3D12, VeRenderBufferD3D12);
//--------------------------------------------------------------------------
VeStaticBufferD3D12::VeStaticBufferD3D12(
	Useage eUse, VeUInt32 u32Size) noexcept
	: VeRenderBufferD3D12(TYPE_STATIC, eUse, u32Size)
{
	if (CanUse(VeRenderBuffer::USEAGE_CB))
	{
		m_u32Size = (m_u32Size + 255) & (~255);
	}
}
//--------------------------------------------------------------------------
VeStaticBufferD3D12::~VeStaticBufferD3D12() noexcept
{
	Term();
}
//--------------------------------------------------------------------------
void VeStaticBufferD3D12::Init(VeRendererD3D12& kRenderer) noexcept
{
	D3D12_RESOURCE_STATES eState = D3D12_RESOURCE_STATE_COMMON;
	if (VE_MASK_HAS_ANY(m_u16Useage, USEAGE_VB | USEAGE_CB))
		eState |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	if (VE_MASK_HAS_ANY(m_u16Useage, USEAGE_IB))
		eState |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
	VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommittedResource(
		&HeapProp(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
		&BufferDesc(m_u32Size), eState, nullptr,
		IID_PPV_ARGS(&m_pkResource)), S_OK);
	if (VE_MASK_HAS_ANY(m_u16Useage, USEAGE_CB))
	{
		m_pkCBView = VeAlloc(CBufferView, 1);
		m_pkCBView->m_u32Offset = kRenderer.m_kSRVHeap.Alloc();
		m_pkCBView->m_kCPUHandle.ptr = kRenderer.m_kSRVHeap.GetCPUStart().ptr + m_pkCBView->m_u32Offset;
		m_pkCBView->m_kGPUHandle.ptr = kRenderer.m_kSRVHeap.GetGPUStart().ptr + m_pkCBView->m_u32Offset;
		D3D12_CONSTANT_BUFFER_VIEW_DESC kCBVDesc = { m_pkResource->GetGPUVirtualAddress(), m_u32Size };
		kRenderer.m_pkDevice->CreateConstantBufferView(&kCBVDesc, m_pkCBView->m_kCPUHandle);
	}
	kRenderer.m_kRenderBufferList.attach_back(m_kNode);
}
//--------------------------------------------------------------------------
void VeStaticBufferD3D12::Term() noexcept
{
	if (m_kNode.is_attach())
	{
		VeRendererD3D12& kRenderer = GetRenderer();
		if (m_pkCBView)
		{
			kRenderer.m_kSRVHeap.Free(m_pkCBView->m_u32Offset);
			VeFree(m_pkCBView);
			m_pkCBView = nullptr;
		}
		VE_SAFE_RELEASE(m_pkResource);
		m_kNode.detach();
	}
}
//--------------------------------------------------------------------------
void VeStaticBufferD3D12::UpdateSync(void* pvData) noexcept
{
	if (m_kNode.is_attach())
	{
		VeRendererD3D12& kRenderer = GetRenderer();
		ID3D12Resource* pkIntermediate = nullptr;
		VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommittedResource(
			&HeapProp(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
			&BufferDesc(m_u32Size), D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&pkIntermediate)), S_OK);
		void* pvDst;
		VE_ASSERT_GE(pkIntermediate->Map(0, nullptr, &pvDst), S_OK);
		VeCrazyCopy(pvDst, pvData, m_u32Size);
		pkIntermediate->Unmap(0, nullptr);
		kRenderer.m_pkCopyCommandList->ResourceBarrier(1, &BarrierTransition(
			m_pkResource, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		kRenderer.m_pkCopyCommandList->CopyBufferRegion(m_pkResource, 0, pkIntermediate, 0, m_u32Size);
		kRenderer.m_pkCopyCommandList->ResourceBarrier(1, &BarrierTransition(
			m_pkResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON));
		kRenderer.m_kCopyResList.push_back(pkIntermediate);
		m_pkResource->AddRef();
		kRenderer.m_kCopyResList.push_back(m_pkResource);
	}
}
//--------------------------------------------------------------------------
VeRTTIImpl(VeDynamicBufferD3D12, VeRenderBufferD3D12);
//--------------------------------------------------------------------------
VeDynamicBufferD3D12::VeDynamicBufferD3D12(
	Useage eUse, VeUInt32 u32Size) noexcept
	: VeRenderBufferD3D12(TYPE_DYNAMIC, eUse, u32Size)
{
	if (CanUse(VeRenderBuffer::USEAGE_CB))
	{
		m_u32Size = (m_u32Size + 255) & (~255);
	}
}
//--------------------------------------------------------------------------
VeDynamicBufferD3D12::~VeDynamicBufferD3D12() noexcept
{
	Term();
}
//--------------------------------------------------------------------------
void VeDynamicBufferD3D12::Init(VeRendererD3D12& kRenderer) noexcept
{
	VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommittedResource(
		&HeapProp(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
		&BufferDesc(m_u32Size * VeRendererD3D12::FRAME_COUNT),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&m_pkResource)), S_OK);

	VE_ASSERT_GE(m_pkResource->Map(0, nullptr, &m_pvMappedBuffer), S_OK);
	VE_ASSERT(!(((VeSizeT)m_pvMappedBuffer) & 0xf));
	if (VE_MASK_HAS_ANY(m_u16Useage, USEAGE_CB))
	{
		m_pkCBView = VeAlloc(CBufferView, VeRendererD3D12::FRAME_COUNT);
		for (VeUInt32 i(0); i < VeRendererD3D12::FRAME_COUNT; ++i)
		{
			m_pkCBView[i].m_u32Offset = kRenderer.m_kSRVHeap.Alloc();
			m_pkCBView[i].m_kCPUHandle.ptr = kRenderer.m_kSRVHeap.GetCPUStart().ptr + m_pkCBView[i].m_u32Offset;
			m_pkCBView[i].m_kGPUHandle.ptr = kRenderer.m_kSRVHeap.GetGPUStart().ptr + m_pkCBView[i].m_u32Offset;
			D3D12_CONSTANT_BUFFER_VIEW_DESC kCBVDesc = { m_pkResource->GetGPUVirtualAddress() + i * m_u32Size, m_u32Size };
			kRenderer.m_pkDevice->CreateConstantBufferView(&kCBVDesc, m_pkCBView[i].m_kCPUHandle);
		}
	}
	kRenderer.m_kRenderBufferList.attach_back(m_kNode);
}
//--------------------------------------------------------------------------
void VeDynamicBufferD3D12::Term() noexcept
{
	if (m_kNode.is_attach())
	{
		VeRendererD3D12& kRenderer = GetRenderer();
		if (m_pkCBView)
		{
			for (VeUInt32 i(VeRendererD3D12::FRAME_COUNT - 1); i < VeRendererD3D12::FRAME_COUNT; --i)
			{
				kRenderer.m_kSRVHeap.Free(m_pkCBView[i].m_u32Offset);
			}
			VeFree(m_pkCBView);
			m_pkCBView = nullptr;
		}		
		m_pvMappedBuffer = nullptr;
		m_pkResource->Unmap(0, nullptr);
		VE_SAFE_RELEASE(m_pkResource);
		m_kNode.detach();
	}
}
//--------------------------------------------------------------------------
void* VeDynamicBufferD3D12::Map() noexcept
{
	if ((++m_u32Active) >= VeRendererD3D12::FRAME_COUNT)
		m_u32Active -= VeRendererD3D12::FRAME_COUNT;
	return GetActiveBuffer();
}
//--------------------------------------------------------------------------
void VeDynamicBufferD3D12::Unmap() noexcept
{

}
//--------------------------------------------------------------------------
void VeDynamicBufferD3D12::UpdateSync(void* pvData) noexcept
{
	if ((++m_u32Active) >= VeRendererD3D12::FRAME_COUNT)
		m_u32Active -= VeRendererD3D12::FRAME_COUNT;
	VeCrazyCopy(GetActiveBuffer(), pvData, m_u32Size);
}
//--------------------------------------------------------------------------
void* VeDynamicBufferD3D12::GetActiveBuffer() noexcept
{
	return (VeByte*)m_pvMappedBuffer + m_u32Size * m_u32Active;
}
//--------------------------------------------------------------------------
