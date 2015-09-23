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
		D3D12_CPU_DESCRIPTOR_HANDLE m_hCPUHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE m_hGPUHandle;
	};

	struct RTView
	{
		VeUInt32 m_u32Offset;
		D3D12_CPU_DESCRIPTOR_HANDLE m_hCPUHandle;
		VeUInt32 m_u32Width;
		VeUInt32 m_u32Height;
		VeUInt32 m_u32Depth;
		VeUInt32 m_u32SubResource;
	};

	struct DSView
	{
		VeUInt32 m_u32Offset;
		D3D12_CPU_DESCRIPTOR_HANDLE m_hCPUHandle;
		VeUInt32 m_u32Width;
		VeUInt32 m_u32Height;
		VeUInt32 m_u32Depth;
		VeUInt32 m_u32SubResource;
	};

	VeRenderTextureD3D12(Dimension eDim, Useage eUse, Format eFormat,
		VeUInt32 u32Width, VeUInt32 u32Height, VeUInt16 u16Depth,
		VeUInt16 u16MipLevels, VeUInt16 u16Count, VeUInt16 u16Quality) noexcept;

	virtual ~VeRenderTextureD3D12() noexcept;

	VeRendererD3D12& GetRenderer() noexcept;

	void GetSRVDesc(D3D12_SHADER_RESOURCE_VIEW_DESC& kDesc) noexcept;

	void GetRTVDesc(D3D12_RENDER_TARGET_VIEW_DESC& kDesc, VeUInt32 u32Mip) noexcept;

	void GetDSVDesc(D3D12_DEPTH_STENCIL_VIEW_DESC& kDesc, VeUInt32 u32Mip) noexcept;

	void Init(VeRendererD3D12& kRenderer) noexcept;

	void Term() noexcept;

	virtual void UpdateSync(void* pvData, VeSizeT stSize) noexcept override;

	virtual VeUInt32 GetSRVNum() noexcept override;

	virtual void ClearSRV() noexcept override;

	virtual void SetSRVNum(VeUInt32 u32Num) noexcept override;

	virtual void SetSRV(VeUInt32 u32Index, SRVType eType, Format eFormat,
		VeUInt32 p0, VeUInt32 p1) noexcept override;

	virtual VeUInt32 GetRTVNum() noexcept override;

	virtual void ClearRTV() noexcept override;

	virtual void SetRTVNum(VeUInt32 u32Num) noexcept override;

	virtual void SetRTV(VeUInt32 u32Index, RTVType eType, Format eFormat,
		VeUInt32 p0, VeUInt32 p1) noexcept override;

	virtual VeUInt32 GetDSVNum() noexcept override;

	virtual void ClearDSV() noexcept override;

	virtual void SetDSVNum(VeUInt32 u32Num) noexcept override;

	virtual void SetDSV(VeUInt32 u32Index, DSVType eType, Format eFormat,
		VeUInt32 p0, VeUInt32 p1) noexcept override;

	VeRefNode<VeRenderTextureD3D12*> m_kNode;
	ID3D12Resource* m_pkResource = nullptr;
	VeVector<SRView> m_kSRVList;
	VeVector<RTView> m_kRTVList;
	VeVector<DSView> m_kDSVList;

};
