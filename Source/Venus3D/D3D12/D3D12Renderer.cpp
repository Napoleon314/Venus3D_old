////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      D3D12
//  File name:   VeRendererD3D12.cpp
//  Created:     2016/07/22 by Albert
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
#include "D3D12Renderer.h"
#include "D3D12RenderWindow.h"
#include "D3D12RenderState.h"

//--------------------------------------------------------------------------
#ifdef VE_ENABLE_D3D12
//--------------------------------------------------------------------------
#ifdef THROW
#	undef THROW
#endif
#define THROW(...) VE_THROW("D3D12Renderer Error", __VA_ARGS__)
//--------------------------------------------------------------------------
VeRTTIImpl(D3D12Renderer);
//--------------------------------------------------------------------------
D3D12Renderer::D3D12Renderer() noexcept
{

}
//--------------------------------------------------------------------------
D3D12Renderer::~D3D12Renderer() noexcept
{

}
//--------------------------------------------------------------------------
void D3D12Renderer::Init()
{
	m_spD3D12 = VE_NEW VeSharedLib(LIB_D3D12);
	if (!m_spD3D12->Load())
	{
		THROW("Failed to load " LIB_D3D12 ".");
	}
	m_spDXGI = VE_NEW VeSharedLib(LIB_DXGI);
	if (!m_spDXGI->Load())
	{
		THROW("Failed to load " LIB_DXGI ".");
	}
	m_spD3DCompiler = VE_NEW VeSharedLib(LIB_D3DCOMPLIER);
	if (!m_spD3DCompiler->Load())
	{
		THROW("Failed to load " LIB_D3DCOMPLIER ".");
	}

	D3D12GetDebugInterface = (decltype(D3D12GetDebugInterface))
		m_spD3D12->GetProc("D3D12GetDebugInterface");
	if (!D3D12GetDebugInterface)
	{
		THROW("Failed to fetch function D3D12GetDebugInterface.");
	}

	D3D12CreateDevice = (decltype(D3D12CreateDevice))
		m_spD3D12->GetProc("D3D12CreateDevice");
	if (!D3D12CreateDevice)
	{
		THROW("Failed to fetch function D3D12CreateDevice.");
	}

	D3D12SerializeRootSignature = (decltype(D3D12SerializeRootSignature))
		m_spD3D12->GetProc("D3D12SerializeRootSignature");
	if (!D3D12SerializeRootSignature)
	{
		THROW("Failed to fetch function D3D12SerializeRootSignature.");
	}

	CreateDXGIFactory1 = (decltype(CreateDXGIFactory1))
		m_spDXGI->GetProc("CreateDXGIFactory1");
	if (!CreateDXGIFactory1)
	{
		THROW("Failed to fetch function CreateDXGIFactory1.");
	}

	D3DCompile = (decltype(D3DCompile))
		m_spD3DCompiler->GetProc("D3DCompile");
	if (!D3DCompile)
	{
		THROW("Failed to fetch function D3DCompile.");
	}

#	ifdef VE_DEBUG
	{
		ID3D12Debug* pkDebugController(nullptr);
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pkDebugController))))
		{
			pkDebugController->EnableDebugLayer();
		}
		VE_SAFE_RELEASE(pkDebugController);
	}
#	endif

	if (VE_FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&m_pkDXGIFactory))))
	{
		THROW("Failed to create DXGI factory.");
	}

	if (m_pkDXGIFactory->EnumAdapters1(0, &m_pkDefaultAdapter) == DXGI_ERROR_NOT_FOUND)
	{
		THROW("Failed to get main adapter.");
	}

	if (VE_FAILED(D3D12CreateDevice(m_pkDefaultAdapter,
		D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pkDevice))))
	{
		THROW("Failed to create d3d12 device.");
	}

	m_kRTVHeap.Init(m_pkDevice);
	m_kDSVHeap.Init(m_pkDevice);
	m_kSRVHeap.Init(m_pkDevice);

	VeCoreLogI("D3D12Renderer created successfully.");
}
//--------------------------------------------------------------------------
void D3D12Renderer::Term()
{
	for (auto pkWindow : m_kRenderWindowList)
	{
		pkWindow->Term();
	}
	VE_ASSERT(m_kRenderWindowList.empty());

	m_kSRVHeap.Term();
	m_kDSVHeap.Term();
	m_kRTVHeap.Term();

	VE_SAFE_RELEASE(m_pkDevice);
	VE_SAFE_RELEASE(m_pkDefaultAdapter);
	VE_SAFE_RELEASE(m_pkDXGIFactory);

	D3D12GetDebugInterface = nullptr;
	D3D12CreateDevice = nullptr;
	D3D12SerializeRootSignature = nullptr;
	CreateDXGIFactory1 = nullptr;
	D3DCompile = nullptr;

	m_spD3D12 = nullptr;
	m_spDXGI = nullptr;
	m_spD3DCompiler = nullptr;
}
//--------------------------------------------------------------------------
void D3D12Renderer::BeginSyncCopy() noexcept
{

}
//--------------------------------------------------------------------------
void D3D12Renderer::EndSyncCopy() noexcept
{

}
//--------------------------------------------------------------------------
VeRenderWindowPtr D3D12Renderer::CreateRenderWindow(
	const VeWindowPtr& spWindow) noexcept
{
	D3D12RenderWindow* pkRenderWindow = VE_NEW D3D12RenderWindow(spWindow);
	VE_ASSERT(pkRenderWindow);
	pkRenderWindow->Init(*this);
	return pkRenderWindow;
}
//--------------------------------------------------------------------------
VeInputLayoutPtr D3D12Renderer::CreateInputLayout(
	const VeInputLayout::ElementDesc* pkDescs, size_t stNum) noexcept
{
	return VE_NEW D3D12InputLayout(pkDescs, stNum);
}
//--------------------------------------------------------------------------
VeRendererPtr CreateD3D12Renderer() noexcept
{
	return VE_NEW D3D12Renderer();
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
