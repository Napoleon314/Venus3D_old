////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderer.cpp
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderer);
//--------------------------------------------------------------------------
VeRenderer::VeRenderer(API eType) noexcept
	: m_eType(eType)
{

}
//--------------------------------------------------------------------------
VeRenderer::~VeRenderer() noexcept
{

}
//--------------------------------------------------------------------------
void VeRenderer::RegistResTypes() noexcept
{
	VeShader::Regist();
}
//--------------------------------------------------------------------------
void VeRenderer::UnregistResTypes() noexcept
{
	VeShader::Unregist();
}
//--------------------------------------------------------------------------
VeRenderWindowPtr VeRenderer::CreateRenderWindow(const VeChar8* pcTitle,
	VeInt32 x, VeInt32 y, VeInt32 w, VeInt32 h, VeUInt32 u32Flags) noexcept
{
	VeWindowPtr spWindow = VE_NEW VeWindow();
	spWindow->Create(pcTitle, x, y, w, h, u32Flags);
	VeRenderWindowPtr spRes = CreateRenderWindow(spWindow);
	spWindow = nullptr;
	spRes->m_bNeedDestory = true;
	return spRes;
}
//--------------------------------------------------------------------------
