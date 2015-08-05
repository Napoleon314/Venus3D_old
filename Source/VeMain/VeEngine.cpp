////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeEngine.cpp
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"
#include "_VeRendererD3D12.h"

//--------------------------------------------------------------------------
VeEngine::VeEngine() noexcept
{

	

}
//--------------------------------------------------------------------------
VeEngine::~VeEngine() noexcept
{

}
//--------------------------------------------------------------------------
void VeEngine::Init() noexcept
{
	InitRenderer();
}
//--------------------------------------------------------------------------
void VeEngine::Term() noexcept
{
	TermRenderer();
}
//--------------------------------------------------------------------------
void VeEngine::InitRenderer() noexcept
{
#	ifdef VE_ENABLE_D3D12
	if (!m_spRenderer)
	{
		m_spRenderer = VE_NEW VeRendererD3D12();
		if (!m_spRenderer->Init())
		{
			m_spRenderer->Term();
			m_spRenderer = nullptr;
		}
	}
#	endif
}
//--------------------------------------------------------------------------
void VeEngine::TermRenderer() noexcept
{
	if (m_spRenderer)
	{
		m_spRenderer->Term();
		m_spRenderer = nullptr;
	}
}
//--------------------------------------------------------------------------
