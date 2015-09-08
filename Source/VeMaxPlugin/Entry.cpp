////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   Entry.cpp
//  Created:     2015/09/08 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//--------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
	}
	return TRUE;
}
//--------------------------------------------------------------------------
__declspec(dllexport) const TCHAR* LibDescription()
{
	return VE_MAX_LIB_DESC;
}
//--------------------------------------------------------------------------
__declspec(dllexport) int LibNumberClasses()
{
	return 0;
}
//--------------------------------------------------------------------------
__declspec(dllexport) ClassDesc* LibClassDesc(int i)
{
	return nullptr;
}
//--------------------------------------------------------------------------
__declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}
//--------------------------------------------------------------------------
__declspec(dllexport) int LibInitialize(void)
{
	return TRUE;
}
//--------------------------------------------------------------------------
__declspec(dllexport) int LibShutdown(void)
{
	return TRUE;
}
//--------------------------------------------------------------------------
