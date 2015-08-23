////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRendererWindowD3D12.cpp
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "../VeMainPch.h"
#include "VeRendererWindowD3D12.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderWindowD3D12, VeRenderWindow);
//--------------------------------------------------------------------------
VeRenderWindowD3D12::VeRenderWindowD3D12() noexcept
{
	m_kNode.m_Content = this;
}
//--------------------------------------------------------------------------
VeRenderWindowD3D12::~VeRenderWindowD3D12() noexcept
{

}
//--------------------------------------------------------------------------
bool VeRenderWindowD3D12::IsValid() noexcept
{
	return m_kNode.is_attach();
}
//--------------------------------------------------------------------------
