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
	m_kData.m_kNode.m_Content = this;
}
//--------------------------------------------------------------------------
VeInt32 VeWindow::GetWindowDisplayIndex() noexcept
{
	VE_ASSERT(ve_video_ptr);
	VeInt32 i32DisplayNum = (VeInt32)(ve_video_ptr->m_kDisplayList.size());

	VeInt32 displayIndex;
	VeInt32 i, dist;
	VeInt32 closest = -1;
	VeInt32 closest_dist = 0x7FFFFFFF;
	VePoint center;
	VePoint delta;
	VeRect rect;

	if (VE_WINDOWPOS_ISUNDEFINED(m_kData.x)
		|| VE_WINDOWPOS_ISCENTERED(m_kData.x))
	{
		displayIndex = (m_kData.x & 0xFFFF);
		if (displayIndex >= i32DisplayNum)
		{
			displayIndex = 0;
		}
		return displayIndex;
	}
	if (VE_WINDOWPOS_ISUNDEFINED(m_kData.y)
		|| VE_WINDOWPOS_ISCENTERED(m_kData.y))
	{
		displayIndex = (m_kData.y & 0xFFFF);
		if (displayIndex >= i32DisplayNum)
		{
			displayIndex = 0;
		}
		return displayIndex;
	}

	for (i = 0; i < i32DisplayNum; ++i)
	{
		VeVideoDisplay* display = &ve_video_ptr->m_kDisplayList[i];

		if (display->m_spFullscreenWindow == this)
		{
			return i;
		}
	}
	center.x = m_kData.x + (m_kData.w >> 1);
	center.y = m_kData.y + (m_kData.h >> 1);
	for (i = 0; i < i32DisplayNum; ++i)
	{
		ve_video_ptr->GetDisplayBounds(i, &rect);
		if (VeRect::EnclosePoints(nullptr, &center, 1, &rect))
		{
			return i;
		}

		delta.x = center.x - (rect.x + (rect.w >> 2));
		delta.y = center.y - (rect.y + (rect.h >> 2));
		dist = (delta.x * delta.x + delta.y * delta.y);
		if (dist < closest_dist)
		{
			closest = i;
			closest_dist = dist;
		}
	}
	if (closest < 0)
	{
		VeDebugOutputCore("Couldn't find any displays");
	}
	return closest;
}
//--------------------------------------------------------------------------
VeVideoDisplay* VeWindow::GetDisplayForWindow() noexcept
{
	VE_ASSERT(ve_video_ptr);
	VeInt32 displayIndex = GetWindowDisplayIndex();
	if (displayIndex >= 0)
	{
		return &ve_video_ptr->m_kDisplayList[displayIndex];
	}
	else {
		return nullptr;
	}
}
//--------------------------------------------------------------------------
VeWindow* VeWindow::Cast(Data* pkData) noexcept
{
	return VeMemberCast(&VeWindow::m_kData, pkData);
}
//--------------------------------------------------------------------------
