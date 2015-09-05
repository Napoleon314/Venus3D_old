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
	ve_res_mgr.LoadFile("startup$root_signatures.json");
	ve_res_mgr.LoadFile("startup$pipeline_states.json");

	VeMatrix4X3 kRot;
	kRot.SetRotateZ(VE_MATH_PI_2_F);

	VeMatrix4X3 kScale;
	kScale.SetScale(2, 2, 2);

	VeMatrix4X3 kTrans;
	kTrans.SetTranslation(10, 11, 12);

	kRot = kRot * kScale;

	VeRenderWindowPtr spRenderWindow = ve_renderer_ptr->CreateRenderWindow(
		"RenderTest", VE_WINDOWPOS_CENTERED, VE_WINDOWPOS_CENTERED, 1280, 720, VE_WINDOW_ALLOW_HIGHDPI);

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
						if (spRenderWindow && pkEvent->m_kWindow.m_u32WindowID == spRenderWindow->GetID())
						{
							spRenderWindow = nullptr;
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

			if (spRenderWindow)
			{
				static VeFloat32 s_f32TimeCount(0);
				static VeUInt32 s_u32FrameCount(0);
				static VeUInt32 s_u32FPS;

				s_f32TimeCount += ve_time.GetDeltaTime();
				++s_u32FrameCount;

				if (s_f32TimeCount > 1.0f)
				{
					s_u32FPS = s_u32FrameCount;
					VeChar8 s_acFPS[64];
					VeSprintf(s_acFPS, "RenderTest[FPS:%d]", s_u32FrameCount);
					spRenderWindow->SetTitle(s_acFPS);
					s_f32TimeCount -= VeFloorf(s_f32TimeCount);
					s_u32FrameCount = 0;
				}
				spRenderWindow->Update();
			}
		}		
	}

	VE_ASSERT(!spRenderWindow);

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
