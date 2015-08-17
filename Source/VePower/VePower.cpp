////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VePower.cpp
//  Created:     2015/02/18 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
void VeConsoleInit(const VeChar8* pcPakName) noexcept
{
	VE_NEW VeStringTable();
	VE_NEW VeSystem(VeSystem::TYPE_CONSOLE, pcPakName);
	ve_sys.Init();
}
//--------------------------------------------------------------------------
void VeConsoleTerm() noexcept
{
	ve_sys.Term();
	VeSystem::Destory();
	VeStringTable::Destory();
#	ifdef VE_MEM_DEBUG
	_VeMemoryExit();
#	endif
}
//--------------------------------------------------------------------------
