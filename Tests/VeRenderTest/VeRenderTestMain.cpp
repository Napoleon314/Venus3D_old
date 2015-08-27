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
	VE_NEW VeEngine();

	ve_sys.Init();
	ve_engine.Init();

	ve_res_mgr.SetupGroupFromJSON("{\"startup\":{\"r\":[\"file#shaders/cache\",\"file#shaders\"],\"w\":\"file#shaders/cache\"}}");

	ve_res_mgr.LoadFile("startup$shaders.json");
	//VeShaderPtr spShader = ve_res_mgr.Get<VeShader>("test@vs");

	//spShader->Load();

	VeWindowPtr spWindow1 = VE_NEW VeWindow();
	spWindow1->Create("Render Test1", 80, VE_WINDOWPOS_CENTERED, 800, 600, 0);
		
	VeWindowPtr spWindow2 = VE_NEW VeWindow();
	spWindow2->Create("Render Test2", 960, VE_WINDOWPOS_CENTERED, 800, 600, 0);

	//ve_mouse_ptr->SetRelativeMode(true);

	{
		bool bLoop(true);
		VeVector<VeEvent*> kEventCache;
		while (bLoop)
		{
			ve_video_ptr->PeekEvents(kEventCache);
			for (auto pkEvent : kEventCache)
			{
				VE_ASSERT(pkEvent->m_u32Type);
				switch (pkEvent->m_u32Type)
				{
				case VE_WINDOWEVENT:
					if (pkEvent->m_kWindow.m_u8Event == VE_WINDOWEVENT_CLOSE)
					{
						if (spWindow1 && pkEvent->m_kWindow.m_u32WindowID == spWindow1->GetID())
						{
							spWindow1->Destory();
							spWindow1 = nullptr;
						}
						if (spWindow2 && pkEvent->m_kWindow.m_u32WindowID == spWindow2->GetID())
						{
							spWindow2->Destory();
							spWindow2 = nullptr;
						}
					}
					else if (pkEvent->m_kWindow.m_u8Event == VE_WINDOWEVENT_MOVED)
					{
						VeDebugOutput("Moved: %d, %d, %d",
							pkEvent->m_kWindow.m_u32TimeStamp,
							pkEvent->m_kWindow.m_i32Data1,
							pkEvent->m_kWindow.m_i32Data2);
					}
					else if (pkEvent->m_kWindow.m_u8Event == VE_WINDOWEVENT_RESIZED)
					{
						VeDebugOutput("Resized: %d, %d, %d",
							pkEvent->m_kWindow.m_u32TimeStamp,
							pkEvent->m_kWindow.m_i32Data1,
							pkEvent->m_kWindow.m_i32Data2);
					}
					break;
				case VE_QUIT:
					bLoop = false;
					break;
				default:
					break;
				}
			}
			kEventCache.clear();
			ve_sys.Update();
			ve_engine.Update();
			//VeSleep(1);
			//VeDebugOutput("Time:%f", ve_time_ptr->GetTime());
		}
	}

	VE_ASSERT(!(spWindow1 || spWindow2));

	ve_engine.Term();
	ve_sys.Term();

	VeEngine::Destory();
	VeSystem::Destory();
	VeStringTable::Destory();
#	ifdef VE_MEM_DEBUG
	_VeMemoryExit();
#	endif

	return 0;
}
