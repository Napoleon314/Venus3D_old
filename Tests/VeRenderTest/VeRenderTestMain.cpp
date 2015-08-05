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

	VeWindowPtr spWindow = ve_video_ptr->CreateWindowBy("Render Test",
		VE_WINDOWPOS_CENTERED, VE_WINDOWPOS_CENTERED, 1024, 768, 0);

	bool bLoop(true);
	while (bLoop)
	{
		VeVector<VeEvent*> kEventCache;
		ve_video_ptr->PeekEvents(kEventCache);
		for (auto kEvent : kEventCache)
		{
			VE_ASSERT(kEvent->m_u32Type);
			switch (kEvent->m_u32Type)
			{
			case VE_QUIT:
				bLoop = false;
				break;
			default:
				break;
			}
		}
	}

	spWindow = nullptr;

	ve_sys.Term();

	VeConsoleTerm();

	return 0;
}
