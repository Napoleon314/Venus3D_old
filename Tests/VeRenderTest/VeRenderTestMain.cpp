////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeRenderTest
//  File name:   VeRenderTestMain.cpp
//  Created:     2015/08/04 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include <VePowerPch.h>
#include <vld.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	VeConsoleInit("Render", false);

	ve_sys.Init();

	//VeUInt32 style = 113901568;//440 125 1040 807

	ve_video_ptr->CreateWindowBy("Test", VE_WINDOWPOS_CENTERED, VE_WINDOWPOS_CENTERED,
		1024, 768, 0);

	while (true)
	{
		ve_video_ptr->_PumpEvents();
		VeSleep(10);
	}

	ve_sys.Term();

	VeConsoleTerm();

	return 0;
}
