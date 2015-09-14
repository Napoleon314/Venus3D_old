////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderResource.cpp
//  Created:     2015/09/14 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderTexture, VeRenderResource);
//--------------------------------------------------------------------------
VeRenderTexture::VeRenderTexture(Dimension eDim, Format eFormat,
	VeUInt32 u32Width, VeUInt32 u32Height, VeUInt16 u16Depth,
	VeUInt16 u16MipLevels, VeUInt16 u16Count, VeUInt16 u16Qulity) noexcept
	: VeRenderResource(eDim), m_eFormat(eFormat), m_u32Width(u32Width)
	, m_u16Depth(u16Depth), m_u16MipLevels(u16MipLevels)
{
	m_kSampleDesc.m_u16Count = u16Count;
	m_kSampleDesc.m_u16Qulity = u16Qulity;
}
//--------------------------------------------------------------------------
