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

#include <VeMainPch.h>
#ifdef VE_PLATFORM_WIN
#	include<vld.h>
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{

	VE_NEW VeStringTable();
	VE_NEW VeSystem(VeSystem::TYPE_DEFAULT, "com.VenusIE.VeRenderTest");
	VE_NEW VeResourceManager();
	VE_NEW VeEngine();

	ve_sys.Init();
	ve_engine.Init();

	VeWindowPtr spWindow = ve_video_ptr->CreateWindowBy("Render Test",
		VE_WINDOWPOS_CENTERED, VE_WINDOWPOS_CENTERED, 1024, 768, 0);

	{
		bool bLoop(true);
		VeVector<VeEvent*> kEventCache;
		while (bLoop)
		{
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
			kEventCache.clear();
			VeSleep(10);
		}
	}

	spWindow = nullptr;

	ve_engine.Term();
	ve_sys.Term();

	VeEngine::Destory();
	VeResourceManager::Destory();
	VeSystem::Destory();
	VeStringTable::Destory();
#	ifdef VE_MEM_DEBUG
	_VeMemoryExit();
#	endif

	return 0;
}
