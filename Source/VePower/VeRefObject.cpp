////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeRefObject.cpp
//  Created:     2015/02/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
std::atomic_uint VeRefObject::ms_u32Objects = 0;
//--------------------------------------------------------------------------
VeRefObject::VeRefObject() noexcept
{
	m_u32RefCount = 0;
	++ms_u32Objects;
}
//--------------------------------------------------------------------------
VeRefObject::~VeRefObject() noexcept
{
	--ms_u32Objects;
}
//--------------------------------------------------------------------------
void VeRefObject::DeleteThis() noexcept
{
	VE_DELETE(this);
}
//--------------------------------------------------------------------------
void VeRefObject::IncRefCount() noexcept
{
	++m_u32RefCount;
}
//--------------------------------------------------------------------------
void VeRefObject::DecRefCount() noexcept
{
	if ((--m_u32RefCount) == 0)
		DeleteThis();
}
//--------------------------------------------------------------------------
