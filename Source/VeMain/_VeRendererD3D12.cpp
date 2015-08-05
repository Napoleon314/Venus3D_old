////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   _VeRendererD3D12.cpp
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"
#include "_VeRendererD3D12.h"


//#include <D3Dcompiler.h>
//#include <DirectXMath.h>


//--------------------------------------------------------------------------
#ifdef VE_ENABLE_D3D12
//--------------------------------------------------------------------------
VeRendererD3D12::VeRendererD3D12() noexcept
{
	m_eType = API_D3D12;

	D3D12GetDebugInterface = nullptr;
	D3D12CreateDevice = nullptr;

	CreateDXGIFactory1 = nullptr;
}
//--------------------------------------------------------------------------
VeRendererD3D12::~VeRendererD3D12() noexcept
{
	
}
//--------------------------------------------------------------------------
bool VeRendererD3D12::Init() noexcept
{
	m_spD3D12 = VE_NEW VeSharedLib(VE_SHARED_LIB_NAME("d3d12"));
	if (!m_spD3D12->Load()) return false;
	m_spDXGI = VE_NEW VeSharedLib(VE_SHARED_LIB_NAME("dxgi"));
	if (!m_spDXGI->Load()) return false;

	D3D12GetDebugInterface = (decltype(D3D12GetDebugInterface))
		m_spD3D12->GetProc("D3D12GetDebugInterface");
	if (!D3D12GetDebugInterface) return false;

	D3D12CreateDevice = (decltype(D3D12CreateDevice))
		m_spD3D12->GetProc("D3D12CreateDevice");
	if (!D3D12CreateDevice) return false;

	CreateDXGIFactory1 = (decltype(CreateDXGIFactory1))
		m_spDXGI->GetProc("CreateDXGIFactory1");
	if (!CreateDXGIFactory1) return false;

#	ifdef VE_DEBUG
	{
		ComPtr<ID3D12Debug> cpDebugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&cpDebugController))))
		{
			cpDebugController->EnableDebugLayer();
		}
	}
#	endif

	ComPtr<IDXGIFactory4> cpFactory;
	if (VE_FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&cpFactory))))
		return false;

	ComPtr<IDXGIAdapter> cpAdapter;
	if (VE_FAILED(cpFactory->EnumWarpAdapter(IID_PPV_ARGS(&cpAdapter))))
		return false;

	if (VE_FAILED(D3D12CreateDevice(cpAdapter.Get(),
		D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_cpDevice))))
		return false;

	VeDebugOutputCore("VeRendererD3D12 renderer is created.");

	return true;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::Term() noexcept
{
	m_cpDevice = nullptr;
	D3D12GetDebugInterface = nullptr;
	m_spD3D12 = nullptr;
	m_spDXGI = nullptr;
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
