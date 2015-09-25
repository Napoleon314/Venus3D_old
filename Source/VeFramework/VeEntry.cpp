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
#ifdef VE_PLATFORM_WIN
//--------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	return VeEntry(lpCmdLine);
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
