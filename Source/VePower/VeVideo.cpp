////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeVideo.cpp
//  Created:     2015/07/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
void VeVideoDevice::GetDisplayBounds(VeInt32 i32DisplayIndex,
	VeRect* pkRect) noexcept
{
	VE_ASSERT(i32DisplayIndex >= 0
		&& i32DisplayIndex < VeInt32(m_kDisplayList.size()));
	if (pkRect)
	{
		VeVideoDisplay* pkDisplay = &m_kDisplayList[i32DisplayIndex];

		if (!_GetDisplayBounds(pkDisplay, pkRect))
		{
			if (i32DisplayIndex == 0)
			{
				pkRect->x = 0;
				pkRect->y = 0;
			}
			else
			{
				GetDisplayBounds(i32DisplayIndex - 1, pkRect);
				pkRect->x += pkRect->w;
			}
			pkRect->w = pkDisplay->m_kCurrentMode.m_i32Width;
			pkRect->h = pkDisplay->m_kCurrentMode.m_i32Height;
		}
	}
}
//--------------------------------------------------------------------------
