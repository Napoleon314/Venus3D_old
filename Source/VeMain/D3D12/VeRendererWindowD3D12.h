////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRendererWindowD3D12.h
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeRendererD3D12.h"

class VeRenderWindowD3D12 : public VeRenderWindow
{
	VE_NO_COPY(VeRenderWindowD3D12);
	VeRTTIDecl(VeRenderWindowD3D12, VeRenderWindow);
public:
	VeRenderWindowD3D12() noexcept;

	virtual ~VeRenderWindowD3D12() noexcept;

	virtual bool IsValid() noexcept;

	VeRefNode<VeRenderWindowD3D12*> m_kNode;
	ComPtr<ID3D12CommandQueue> m_cpCommandQueue;
	ComPtr<IDXGISwapChain> m_cpSwapChain;

};
