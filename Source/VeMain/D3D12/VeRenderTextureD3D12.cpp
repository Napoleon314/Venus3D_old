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
void VeRenderTextureD3D12::GetRTVDesc(D3D12_RENDER_TARGET_VIEW_DESC& kDesc,
	VeUInt32 u32Mip) noexcept
{
	kDesc = {};
	kDesc.Format = (DXGI_FORMAT)m_eFormat;
	kDesc.ViewDimension = D3D12_RTV_DIMENSION_UNKNOWN;
	switch (m_eDimension)
	{
	case VeRenderResource::DIMENSION_TEXTURE1D:
		if (m_u16Depth == 1)
		{
			kDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
			kDesc.Texture1D.MipSlice = u32Mip;
		}
		else if (m_u16Depth > 1)
		{
			kDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
			kDesc.Texture1DArray.MipSlice = u32Mip;
			kDesc.Texture1DArray.ArraySize = m_u16Depth;
		}
		break;
	case VeRenderResource::DIMENSION_TEXTURE2D:
		if (VE_MASK_HAS_ANY(m_eUseage, USEAGE_CUBE)
			&& (m_u16Depth % 6 == 0))
		{
			kDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
			kDesc.Texture2DArray.MipSlice = u32Mip;
			kDesc.Texture2DArray.ArraySize = m_u16Depth;
		}
		else if (m_u16Count == 1)
		{
			if (m_u16Depth == 1)
			{
				kDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
				kDesc.Texture2D.MipSlice = u32Mip;
			}
			else if (m_u16Depth > 1)
			{
				kDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
				kDesc.Texture2DArray.MipSlice = u32Mip;
				kDesc.Texture2DArray.ArraySize = m_u16Depth;
			}
		}
		else if (m_u16Count > 1)
		{
			if (m_u16Depth == 1)
			{
				kDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
			}
			else if (m_u16Depth > 1)
			{
				kDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
				kDesc.Texture2DMSArray.ArraySize = m_u16Depth;
			}
		}
		break;
	case VeRenderResource::DIMENSION_TEXTURE3D:
		kDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
		kDesc.Texture3D.MipSlice = u32Mip;
		kDesc.Texture3D.WSize = m_u16Depth;
		break;
	default:
		break;
	}
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::GetDSVDesc(D3D12_DEPTH_STENCIL_VIEW_DESC& kDesc,
	VeUInt32 u32Mip) noexcept
{
	kDesc = {};
	kDesc.Format = (DXGI_FORMAT)m_eFormat;
	kDesc.ViewDimension = D3D12_DSV_DIMENSION_UNKNOWN;
	switch (m_eDimension)
	{
	case VeRenderResource::DIMENSION_TEXTURE1D:
		if (m_u16Depth == 1)
		{
			kDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
			kDesc.Texture1D.MipSlice = u32Mip;
		}
		else if (m_u16Depth > 1)
		{
			kDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
			kDesc.Texture1DArray.MipSlice = u32Mip;
			kDesc.Texture1DArray.ArraySize = m_u16Depth;
		}
		break;
	case VeRenderResource::DIMENSION_TEXTURE2D:
		if (VE_MASK_HAS_ANY(m_eUseage, USEAGE_CUBE)
			&& (m_u16Depth % 6 == 0))
		{
			kDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
			kDesc.Texture2DArray.MipSlice = u32Mip;
			kDesc.Texture2DArray.ArraySize = m_u16Depth;
		}
		else if (m_u16Count == 1)
		{
			if (m_u16Depth == 1)
			{
				kDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				kDesc.Texture2D.MipSlice = u32Mip;
			}
			else if (m_u16Depth > 1)
			{
				kDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
				kDesc.Texture2DArray.MipSlice = u32Mip;
				kDesc.Texture2DArray.ArraySize = m_u16Depth;
			}
		}
		else if (m_u16Count > 1)
		{
			if (m_u16Depth == 1)
			{
				kDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
			}
			else if (m_u16Depth > 1)
			{
				kDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
				kDesc.Texture2DMSArray.ArraySize = m_u16Depth;
			}
		}
		break;
	default:
		break;
	}
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
		kDesc.Format = (DXGI_FORMAT)VeRenderResource::ToTypeless(m_eFormat);
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
	}
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::Term() noexcept
{
	if (m_kNode.is_attach())
	{
		VeRendererD3D12& kRenderer = GetRenderer();
		ClearSRV();
		ClearRTV();
		ClearDSV();
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
		D3D12_RESOURCE_DESC kDesc = m_pkResource->GetDesc();
		VeUInt32 u32SliceNum = kDesc.MipLevels * kDesc.DepthOrArraySize;
		VeVector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> kFootPrints;
		VeVector<VeUInt32> kNumRows;
		VeVector<VeUInt64> kRowSizeInBytes;
		kFootPrints.resize(u32SliceNum);
		kNumRows.resize(u32SliceNum);
		kRowSizeInBytes.resize(u32SliceNum);
		VeUInt64 u64RequiredSize = 0;
		kRenderer.m_pkDevice->GetCopyableFootprints(&kDesc, 0,
			u32SliceNum, 0, &kFootPrints.front(), &kNumRows.front(),
			&kRowSizeInBytes.front(), &u64RequiredSize);

		VeSizeT stTargetSize = 0;
		for (VeUInt32 i(0); i < u32SliceNum; ++i)
		{
			stTargetSize += kNumRows[i] * kRowSizeInBytes[i];
		}

		if (stTargetSize == stSize)
		{
			ID3D12Resource* pkIntermediate = nullptr;
			VE_ASSERT_GE(kRenderer.m_pkDevice->CreateCommittedResource(
				&HeapProp(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
				&BufferDesc(u64RequiredSize), D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr, IID_PPV_ARGS(&pkIntermediate)), S_OK);

			void* pvDst;
			VE_ASSERT_GE(pkIntermediate->Map(0, nullptr, &pvDst), S_OK);
			
			VeByte* pbySrc = (VeByte*)pvData;

			for (VeUInt32 i(0); i < u32SliceNum; ++i)
			{
				VeByte* pbyDst = (VeByte*)pvDst + kFootPrints[i].Offset;
				if (kRowSizeInBytes[i] == kFootPrints[i].Footprint.RowPitch)
				{
					VeSizeT stSlicePitch = (VeSizeT)(kRowSizeInBytes[i] * kNumRows[i]);
					VeCrazyCopy(pbyDst, pbySrc, stSlicePitch);					
					pbySrc += stSlicePitch;
				}
				else
				{
					for (VeUInt32 j(0); j < kNumRows[i]; ++j)
					{
						VeCrazyCopy(pbyDst, pbySrc, kRowSizeInBytes[i]);
						pbyDst += kFootPrints[i].Footprint.RowPitch;
						pbySrc += kRowSizeInBytes[i];
					}
				}
			}

			pkIntermediate->Unmap(0, nullptr);

			kRenderer.m_pkCopyCommandList->ResourceBarrier(1, &BarrierTransition(
				m_pkResource, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
			for (VeUInt32 i = 0; i < u32SliceNum; ++i)
			{
				D3D12_TEXTURE_COPY_LOCATION kDst =
				{
					m_pkResource, D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX, i
				};
				D3D12_TEXTURE_COPY_LOCATION kSrc =
				{
					pkIntermediate, D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT, kFootPrints[i]
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
VeUInt32 VeRenderTextureD3D12::GetSRVNum() noexcept
{
	return (VeUInt32)m_kSRVList.size();
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::ClearSRV() noexcept
{
	VeRendererD3D12& kRenderer = GetRenderer();
	for (auto it = m_kSRVList.rbegin(); it != m_kSRVList.rend(); ++it)
	{
		kRenderer.m_kSRVHeap.Free(it->m_u32Offset);
	}
	m_kSRVList.clear();
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::SetSRVNum(VeUInt32 u32Num) noexcept
{
	if (CanUse(VeRenderTexture::USEAGE_SRV))
	{
		VeRendererD3D12& kRenderer = GetRenderer();
		if (u32Num > m_kSRVList.size())
		{
			VeSizeT i = m_kSRVList.size();
			m_kSRVList.resize(u32Num);
			for (; i < m_kSRVList.size(); ++i)
			{
				SRView& kView = m_kSRVList[i];
				kView.m_u32Offset = kRenderer.m_kSRVHeap.Alloc();
				kView.m_hCPUHandle.ptr = kRenderer.m_kSRVHeap.GetCPUStart().ptr + kView.m_u32Offset;
				kView.m_hGPUHandle.ptr = kRenderer.m_kSRVHeap.GetGPUStart().ptr + kView.m_u32Offset;
			}
		}
		else if (u32Num < m_kSRVList.size())
		{
			for (VeSizeT i(m_kSRVList.size() - 1); i >= u32Num; --i)
			{
				SRView& kView = m_kSRVList[i];
				kRenderer.m_kSRVHeap.Free(kView.m_u32Offset);
			}
			m_kSRVList.resize(u32Num);
		}
	}
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::SetSRV(VeUInt32 u32Index, SRVType eType,
	Format eFormat, VeUInt32 p0, VeUInt32 p1) noexcept
{
	if (CanUse(VeRenderTexture::USEAGE_SRV)
		&& u32Index < m_kSRVList.size())
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC kDesc;
		switch (eType)
		{
		case VeRenderResource::SRV_DEFAULT:
			GetSRVDesc(kDesc);
			break;
		default:
			return;
		}
		if (eFormat) kDesc.Format = (DXGI_FORMAT)eFormat;
		VeRendererD3D12& kRenderer = GetRenderer();
		kRenderer.m_pkDevice->CreateShaderResourceView(
			m_pkResource, &kDesc, m_kSRVList[u32Index].m_hCPUHandle);
	}
}
//--------------------------------------------------------------------------
VeUInt32 VeRenderTextureD3D12::GetRTVNum() noexcept
{
	return (VeUInt32)m_kRTVList.size();
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::ClearRTV() noexcept
{
	VeRendererD3D12& kRenderer = GetRenderer();
	for (auto it = m_kRTVList.rbegin(); it != m_kRTVList.rend(); ++it)
	{
		kRenderer.m_kRTVHeap.Free(it->m_u32Offset);
	}
	m_kRTVList.clear();
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::SetRTVNum(VeUInt32 u32Num) noexcept
{
	if (CanUse(VeRenderTexture::USEAGE_RTV))
	{
		VeRendererD3D12& kRenderer = GetRenderer();
		if (u32Num > m_kRTVList.size())
		{
			VeSizeT i = m_kRTVList.size();
			m_kRTVList.resize(u32Num);
			for (; i < m_kRTVList.size(); ++i)
			{
				RTView& kView = m_kRTVList[i];
				kView.m_u32Offset = kRenderer.m_kRTVHeap.Alloc();
				kView.m_hCPUHandle.ptr = kRenderer.m_kRTVHeap.GetCPUStart().ptr + kView.m_u32Offset;
				kView.m_u32Width = 0;
				kView.m_u32Height = 0;
				kView.m_u32Depth = 0;
				kView.m_u32SubResource = 0;
			}
		}
		else if (u32Num < m_kRTVList.size())
		{
			for (VeSizeT i(m_kRTVList.size() - 1); i >= u32Num; --i)
			{
				RTView& kView = m_kRTVList[i];
				kRenderer.m_kRTVHeap.Free(kView.m_u32Offset);
			}
			m_kRTVList.resize(u32Num);
		}
	}
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::SetRTV(VeUInt32 u32Index, RTVType eType,
	Format eFormat, VeUInt32 p0, VeUInt32 p1) noexcept
{
	if (CanUse(VeRenderTexture::USEAGE_RTV)
		&& u32Index < m_kRTVList.size())
	{
		D3D12_RENDER_TARGET_VIEW_DESC kDesc;
		switch (eType)
		{
		case VeRenderResource::RTV_DEFAULT:
			GetRTVDesc(kDesc, p0);
			m_kRTVList[u32Index].m_u32Width = (m_u32Width >> p0) ? (m_u32Width >> p0) : 1;
			m_kRTVList[u32Index].m_u32Height = (m_u32Height >> p0) ? (m_u32Height >> p0) : 1;
			m_kRTVList[u32Index].m_u32Depth = m_u16Depth;
			m_kRTVList[u32Index].m_u32SubResource = p1;
			break;
		default:
			return;
		}
		if (eFormat) kDesc.Format = (DXGI_FORMAT)eFormat;
		VeRendererD3D12& kRenderer = GetRenderer();
		kRenderer.m_pkDevice->CreateRenderTargetView(
			m_pkResource, &kDesc, m_kRTVList[u32Index].m_hCPUHandle);
	}
}
//--------------------------------------------------------------------------
VeUInt32 VeRenderTextureD3D12::GetDSVNum() noexcept
{
	return (VeUInt32)m_kDSVList.size();
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::ClearDSV() noexcept
{
	VeRendererD3D12& kRenderer = GetRenderer();
	for (auto it = m_kDSVList.rbegin(); it != m_kDSVList.rend(); ++it)
	{
		kRenderer.m_kDSVHeap.Free(it->m_u32Offset);
	}
	m_kDSVList.clear();
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::SetDSVNum(VeUInt32 u32Num) noexcept
{
	if (CanUse(VeRenderTexture::USEAGE_DSV))
	{
		VeRendererD3D12& kRenderer = GetRenderer();
		if (u32Num > m_kDSVList.size())
		{
			VeSizeT i = m_kDSVList.size();
			m_kDSVList.resize(u32Num);
			for (; i < m_kDSVList.size(); ++i)
			{
				DSView& kView = m_kDSVList[i];
				kView.m_u32Offset = kRenderer.m_kDSVHeap.Alloc();
				kView.m_hCPUHandle.ptr = kRenderer.m_kDSVHeap.GetCPUStart().ptr + kView.m_u32Offset;
				kView.m_u32Width = 0;
				kView.m_u32Height = 0;
				kView.m_u32Depth = 0;
				kView.m_u32SubResource = 0;
			}
		}
		else if (u32Num < m_kDSVList.size())
		{
			for (VeSizeT i(m_kDSVList.size() - 1); i >= u32Num; --i)
			{
				DSView& kView = m_kDSVList[i];
				kRenderer.m_kDSVHeap.Free(kView.m_u32Offset);
			}
			m_kDSVList.resize(u32Num);
		}
	}
}
//--------------------------------------------------------------------------
void VeRenderTextureD3D12::SetDSV(VeUInt32 u32Index, DSVType eType,
	Format eFormat, VeUInt32 p0, VeUInt32 p1) noexcept
{
	if (CanUse(VeRenderTexture::USEAGE_DSV)
		&& u32Index < m_kDSVList.size())
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC kDesc;
		switch (eType)
		{
		case VeRenderResource::DSV_DEFAULT:
			GetDSVDesc(kDesc, p0);
			m_kDSVList[u32Index].m_u32Width = (m_u32Width >> p0) ? (m_u32Width >> p0) : 1;
			m_kDSVList[u32Index].m_u32Height = (m_u32Height >> p0) ? (m_u32Height >> p0) : 1;
			m_kDSVList[u32Index].m_u32Depth = m_u16Depth;
			m_kDSVList[u32Index].m_u32SubResource = p1;
			break;
		default:
			return;
		}
		if (eFormat) kDesc.Format = (DXGI_FORMAT)eFormat;
		VeRendererD3D12& kRenderer = GetRenderer();
		kRenderer.m_pkDevice->CreateDepthStencilView(
			m_pkResource, &kDesc, m_kDSVList[u32Index].m_hCPUHandle);
	}
}
//--------------------------------------------------------------------------
