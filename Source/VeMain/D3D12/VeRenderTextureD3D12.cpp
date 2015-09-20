////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderTextureD3D12.cpp
//  Created:     2015/09/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "../VeMainPch.h"
#include "VeRendererD3D12.h"
#include "VeRenderTextureD3D12.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderTextureD3D12, VeRenderTexture);
//--------------------------------------------------------------------------
VeRenderTextureD3D12::VeRenderTextureD3D12(Dimension eDim, Useage eUse,
	Format eFormat, VeUInt32 u32Width, VeUInt32 u32Height, VeUInt16 u16Depth,
	VeUInt16 u16MipLevels, VeUInt16 u16Count, VeUInt16 u16Quality) noexcept
	: VeRenderTexture(eDim, eUse, eFormat, u32Width, u32Height, u16Depth,
		u16MipLevels, u16Count, u16Quality)
{
	m_kNode.m_Content = this;
}
//--------------------------------------------------------------------------
VeRenderTextureD3D12::~VeRenderTextureD3D12() noexcept
{
	Term();
}
//--------------------------------------------------------------------------
VeRendererD3D12& VeRenderTextureD3D12::GetRenderer() noexcept
{
	VE_ASSERT(m_kNode.is_attach());
	return *VeMemberCast(&VeRendererD3D12::m_kRenderTextureList,
		m_kNode.get_list());
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::GetSRVDesc(
	D3D12_SHADER_RESOURCE_VIEW_DESC& kDesc) noexcept
{
	kDesc = {};
	kDesc.Format = (DXGI_FORMAT)m_eFormat;
	kDesc.ViewDimension = D3D12_SRV_DIMENSION_UNKNOWN;
	kDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	switch (m_eDimension)
	{
	case VeRenderResource::DIMENSION_TEXTURE1D:
		if (m_u16Depth == 1)
		{
			kDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
			kDesc.Texture1D.MipLevels = m_u16MipLevels;

		}
		else if(m_u16Depth > 1)
		{
			kDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
			kDesc.Texture1DArray.MipLevels = m_u16MipLevels;
			kDesc.Texture1DArray.ArraySize = m_u16Depth;
		}
		break;
	case VeRenderResource::DIMENSION_TEXTURE2D:
		if (VE_MASK_HAS_ANY(m_eUseage, USEAGE_CUBE)
			&& (m_u16Depth % 6 == 0))
		{
			if (m_u16Depth == 6)
			{
				kDesc.ViewDimension =  D3D12_SRV_DIMENSION_TEXTURECUBE;
				kDesc.TextureCube.MipLevels = m_u16MipLevels;
			}
			else if (m_u16Depth > 6)
			{
				kDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
				kDesc.TextureCubeArray.MipLevels = m_u16MipLevels;
				kDesc.TextureCubeArray.NumCubes = m_u16Depth / 6;
			}
		}
		else if (m_u16Count == 1)
		{
			if (m_u16Depth == 1)
			{
				kDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				kDesc.Texture2D.MipLevels = m_u16MipLevels;
			}
			else if (m_u16Depth > 1)
			{
				kDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				kDesc.Texture2DArray.MipLevels = m_u16MipLevels;
				kDesc.Texture2DArray.ArraySize = m_u16Depth;
			}
		}
		else if (m_u16Count > 1)
		{
			if (m_u16Depth == 1)
			{
				kDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
			}
			else if (m_u16Depth > 1)
			{
				kDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
				kDesc.Texture2DMSArray.ArraySize = m_u16Depth;
			}
		}
		break;
	case VeRenderResource::DIMENSION_TEXTURE3D:
		kDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
		kDesc.Texture3D.MipLevels = m_u16MipLevels;
		break;
	default:
		break;
	}
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::AddSRV(VeRendererD3D12& kRenderer,
	const D3D12_SHADER_RESOURCE_VIEW_DESC& kDesc) noexcept
{
	m_kSRVList.resize(m_kSRVList.size() + 1);
	SRView& kView = m_kSRVList.back();
	kView.m_u32Offset = kRenderer.m_kSRVHeap.Alloc();
	kView.m_kCPUHandle.ptr = kRenderer.m_kSRVHeap.GetCPUStart().ptr + kView.m_u32Offset;
	kView.m_kGPUHandle.ptr = kRenderer.m_kSRVHeap.GetGPUStart().ptr + kView.m_u32Offset;
	kRenderer.m_pkDevice->CreateShaderResourceView(m_pkResource, &kDesc, kView.m_kCPUHandle);
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::Init(VeRendererD3D12& kRenderer) noexcept
{
	if (!m_kNode.is_attach())
	{
		D3D12_RESOURCE_DESC kDesc = {};
		kDesc.Dimension = (D3D12_RESOURCE_DIMENSION)m_eDimension;
		kDesc.Width = m_u32Width;
		kDesc.Height = m_u32Height;
		kDesc.DepthOrArraySize = m_u16Depth;
		kDesc.MipLevels = m_u16MipLevels;
		kDesc.Format = (DXGI_FORMAT)m_eFormat;
		kDesc.SampleDesc = { m_u16Count, m_u16Quality };
		kDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		if (!VE_MASK_HAS_ANY(m_eUseage, USEAGE_SRV))
			kDesc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
		if (VE_MASK_HAS_ANY(m_eUseage, USEAGE_RTV))
			kDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		if (VE_MASK_HAS_ANY(m_eUseage, USEAGE_DSV))
			kDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		if (VE_MASK_HAS_ANY(m_eUseage, USEAGE_UAV))
			kDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		D3D12_RESOURCE_STATES eState = D3D12_RESOURCE_STATE_COMMON;
		if (VE_MASK_HAS_ANY(m_eUseage, USEAGE_SRV))
			eState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		else if (VE_MASK_HAS_ANY(m_eUseage, USEAGE_RTV))
			eState = D3D12_RESOURCE_STATE_RENDER_TARGET;
		else if (VE_MASK_HAS_ANY(m_eUseage, USEAGE_DSV))
			eState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		else if (VE_MASK_HAS_ANY(m_eUseage, USEAGE_UAV))
			eState = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommittedResource(
			&HeapProp(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			&kDesc, eState, nullptr, IID_PPV_ARGS(&m_pkResource)), S_OK);
		kRenderer.m_kRenderTextureList.attach_back(m_kNode);
		if (VE_MASK_HAS_ANY(m_eUseage, USEAGE_SRV))
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC kSRVDesc = {};
			GetSRVDesc(kSRVDesc);
			if (kSRVDesc.ViewDimension)
			{
				AddSRV(kRenderer, kSRVDesc);
			}
		}
	}
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::Term() noexcept
{
	if (m_kNode.is_attach())
	{
		VeRendererD3D12& kRenderer = GetRenderer();
		for (auto it = m_kSRVList.rbegin(); it != m_kSRVList.rend(); ++it)
		{
			kRenderer.m_kSRVHeap.Free(it->m_u32Offset);
		}
		m_kSRVList.clear();
		VE_SAFE_RELEASE(m_pkResource);
		m_kNode.detach();
	}
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::UpdateSync(void* pvData, VeSizeT stSize) noexcept
{
	if (m_kNode.is_attach())
	{
		VeRendererD3D12& kRenderer = GetRenderer();
		SubResDesc kSubResDesc[D3D12_REQ_MIP_LEVELS];
		VeSizeT stTargetSize = GetSubResDesc(kSubResDesc);
		if (stTargetSize == stSize)
		{
			D3D12_RESOURCE_DESC kDesc = m_pkResource->GetDesc();
			D3D12_PLACED_SUBRESOURCE_FOOTPRINT akFootPrint[D3D12_REQ_MIP_LEVELS];
			VeUInt64 u64RequiredSize = 0;
			kRenderer.m_pkDevice->GetCopyableFootprints(&kDesc, 0,
				kDesc.MipLevels, 0, akFootPrint, nullptr, nullptr, &u64RequiredSize);

			ID3D12Resource* pkIntermediate = nullptr;
			VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommittedResource(
				&HeapProp(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
				&BufferDesc(u64RequiredSize), D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr, IID_PPV_ARGS(&pkIntermediate)), S_OK);
			void* pvDst;
			VE_ASSERT_GE(pkIntermediate->Map(0, nullptr, &pvDst), S_OK);

			VeByte* pbySrc = (VeByte*)pvData;

			for (VeUInt16 i(0); i < kDesc.MipLevels; ++i)
			{
				VeByte* pbyDst = (VeByte*)pvDst + akFootPrint[i].Offset;
				if (kSubResDesc[i].m_u32RowPitch == akFootPrint[i].Footprint.RowPitch)
				{					
					VeCrazyCopy(pbyDst, pbySrc, kSubResDesc[i].m_u32SlicePitch);
					pbySrc += kSubResDesc[i].m_u32SlicePitch;					
				}
				else
				{			
					for (VeUInt16 j(0); j < kSubResDesc[i].m_u32RowNum; ++j)
					{					
						VeCrazyCopy(pbyDst, pbySrc, kSubResDesc[i].m_u32RowPitch);
						pbyDst += akFootPrint[i].Footprint.RowPitch;
						pbySrc += kSubResDesc[i].m_u32RowPitch;
					}
				}
			}
			
			pkIntermediate->Unmap(0, nullptr);

			kRenderer.m_pkCopyCommandList->ResourceBarrier(1, &BarrierTransition(
				m_pkResource, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
			for (VeUInt16 i = 0; i < kDesc.MipLevels; ++i)
			{
				D3D12_TEXTURE_COPY_LOCATION kDst =
				{
					m_pkResource, D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX, i
				};
				D3D12_TEXTURE_COPY_LOCATION kSrc =
				{
					pkIntermediate, D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT, akFootPrint[i]
				};
				kRenderer.m_pkCopyCommandList->CopyTextureRegion(&kDst, 0, 0, 0, &kSrc, nullptr);
			}
			kRenderer.m_pkCopyCommandList->ResourceBarrier(1, &BarrierTransition(
				m_pkResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON));
			kRenderer.m_kCopyResList.push_back(pkIntermediate);
			m_pkResource->AddRef();
			kRenderer.m_kCopyResList.push_back(m_pkResource);
		}
	}
}
//--------------------------------------------------------------------------
