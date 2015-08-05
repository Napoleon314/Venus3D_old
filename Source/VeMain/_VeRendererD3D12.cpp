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
#include <d3d12.h>
//#include <dxgi1_4.h>
//#include <D3Dcompiler.h>
//#include <DirectXMath.h>


//--------------------------------------------------------------------------
#ifdef VE_ENABLE_D3D12
//--------------------------------------------------------------------------
VeRendererD3D12::VeRendererD3D12() noexcept
{
	m_eType = API_D3D12;
}
//--------------------------------------------------------------------------
VeRendererD3D12::~VeRendererD3D12() noexcept
{

}
//--------------------------------------------------------------------------
void VeRendererD3D12::Init() noexcept
{
	m_spLib = VE_NEW VeSharedLib(VE_SHARED_LIB_NAME("d3d12"));
	m_spLib->Load();
}
//--------------------------------------------------------------------------
void VeRendererD3D12::Term() noexcept
{
	m_spLib = nullptr;
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
