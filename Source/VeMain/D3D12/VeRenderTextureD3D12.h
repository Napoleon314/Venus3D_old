////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderTextureD3D12.h
//  Created:     2015/09/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VeRenderTextureD3D12 : public VeRenderTexture
{
	VeNoCopy(VeRenderTextureD3D12);
	VeRTTIDecl(VeRenderTextureD3D12, VeRenderTexture);
public:
	struct SRView
	{
		VeUInt32 m_u32Offset;
		D3D12_CPU_DESCRIPTOR_HANDLE m_kCPUHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE m_kGPUHandle;
	};

	VeRenderTextureD3D12(Dimension eDim, Useage eUse, Format eFormat,
		VeUInt32 u32Width, VeUInt32 u32Height, VeUInt16 u16Depth,
		VeUInt16 u16MipLevels, VeUInt16 u16Count, VeUInt16 u16Quality) noexcept;

	virtual ~VeRenderTextureD3D12() noexcept;

	VeRendererD3D12& GetRenderer() noexcept;

	void GetSRVDesc(D3D12_SHADER_RESOURCE_VIEW_DESC& kDesc) noexcept;

	void AddSRV(VeRendererD3D12& kRenderer, const D3D12_SHADER_RESOURCE_VIEW_DESC& kDesc) noexcept;

	void Init(VeRendererD3D12& kRenderer) noexcept;

	void Term() noexcept;

	virtual void UpdateSync(void* pvData, VeSizeT stSize) noexcept override;

	VeRefNode<VeRenderTextureD3D12*> m_kNode;
	ID3D12Resource* m_pkResource = nullptr;
	VeVector<SRView> m_kSRVList;

};
