////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderBufferD3D12.h
//  Created:     2015/09/11 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VeRenderBufferD3D12 : public VeRenderBuffer
{
	VeNoCopy(VeRenderBufferD3D12);
	VeRTTIDecl(VeRenderBufferD3D12, VeRenderBuffer);
public:
	struct CBufferView
	{
		VeUInt32 m_u32Offset;
		D3D12_CPU_DESCRIPTOR_HANDLE m_kCPUHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE m_kGPUHandle;
	};

	VeRenderBufferD3D12(Type eType, Useage eUse, VeUInt32 u32Size) noexcept;

	virtual ~VeRenderBufferD3D12() noexcept;

	VeRendererD3D12& GetRenderer() noexcept;

	virtual void Init(VeRendererD3D12& kRenderer) noexcept = 0;

	virtual void Term() noexcept = 0;

	VeRefNode<VeRenderBufferD3D12*> m_kNode;
	ID3D12Resource* m_pkResource = nullptr;

};

class VeStaticBufferD3D12 : public VeRenderBufferD3D12
{
	VeNoCopy(VeStaticBufferD3D12);
	VeRTTIDecl(VeStaticBufferD3D12, VeRenderBufferD3D12);
public:
	VeStaticBufferD3D12(Useage eUse, VeUInt32 u32Size) noexcept;

	virtual ~VeStaticBufferD3D12() noexcept;

	virtual void Init(VeRendererD3D12& kRenderer) noexcept override;

	virtual void Term() noexcept override;

	virtual void* Map() noexcept override { return nullptr; }

	virtual void Unmap() noexcept override {}

	virtual void UpdateSync(void* pvData, VeSizeT stSize) noexcept override;

	CBufferView* m_pkCBView = nullptr;

};

class VeDynamicBufferD3D12 : public VeRenderBufferD3D12
{
	VeNoCopy(VeDynamicBufferD3D12);
	VeRTTIDecl(VeDynamicBufferD3D12, VeRenderBufferD3D12);
public:
	VeDynamicBufferD3D12(Useage eUse, VeUInt32 u32Size) noexcept;

	virtual ~VeDynamicBufferD3D12() noexcept;

	virtual void Init(VeRendererD3D12& kRenderer) noexcept override;

	virtual void Term() noexcept override;

	virtual void* Map() noexcept override;

	virtual void Unmap() noexcept override;

	virtual void UpdateSync(void* pvData, VeSizeT stSize) noexcept override;

	void* GetActiveBuffer() noexcept;

	void* m_pvMappedBuffer = nullptr;	
	CBufferView* m_pkCBView = nullptr;
	VeUInt32 m_u32Active = VeRendererD3D12::FRAME_COUNT - 1;

};
