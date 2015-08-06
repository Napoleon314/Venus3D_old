////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeWindow.cpp
//  Created:     2015/07/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeWindow::VeWindow() noexcept
{
	m_kData.m_kNode.m_Content = &m_kData;
}
//--------------------------------------------------------------------------
VeWindow::~VeWindow() noexcept
{
	Destory();
}
//--------------------------------------------------------------------------
bool VeWindow::Create(const VeChar8* pcTitle, VeInt32 x, VeInt32 y,
	VeInt32 w, VeInt32 h, VeUInt32 u32Flags) noexcept
{
	if (!ve_video_ptr) return false;
	if (IsValid()) return false;
	return ve_video_ptr->CreateWindowBy(&m_kData, pcTitle, x, y, w, h, u32Flags);
}
//--------------------------------------------------------------------------
void VeWindow::Destory()
{
	if (ve_video_ptr && IsValid())
	{
		ve_video_ptr->DestroyWindow(&m_kData);
	}
}
//--------------------------------------------------------------------------
VeWindow* VeWindow::Cast(Data* pkData) noexcept
{
	return VeMemberCast(&VeWindow::m_kData, pkData);
}
//--------------------------------------------------------------------------
