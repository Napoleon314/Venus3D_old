////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeFramework
//  File name:   VeEntry.cpp
//  Created:     2015/09/25 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeFrameworkPch.h"

//--------------------------------------------------------------------------
extern VeInt32 VeEntry(const VeChar8* pcCmdLine) noexcept;
//--------------------------------------------------------------------------
void VeEngineStart() noexcept
{
	VE_NEW VeStringTable();
	VE_NEW VeSystem(VeSystem::TYPE_DEFAULT, "com.VenusIE.VeRenderTest");
	VE_NEW VeEngine();
}
//--------------------------------------------------------------------------
void VeEngineEnd() noexcept
{
	VeEngine::Destory();
	VeSystem::Destory();
	VeStringTable::Destory();
#	ifdef VE_MEM_DEBUG
	_VeMemoryExit();
#	endif
}
//--------------------------------------------------------------------------
#ifdef VE_PLATFORM_WIN
//--------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	VeEngineStart();
	int iRes = VeEntry(lpCmdLine);
	VeEngineEnd();
	return iRes;
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
