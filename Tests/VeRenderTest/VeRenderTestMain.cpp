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

	ve_res_mgr.SetupGroupFromJSON("{\"default\":{\"r\":[\"file#\"]}}");

	ve_renderer_ptr->PreCache(ve_res_mgr.CreateDir("file#scripts/cache"),
		ve_res_mgr.CreateDir("file#scripts/hlsl", false),
		ve_res_mgr.CreateDir("file#scripts", false));

	/*VeRenderBufferPtr spCB = ve_renderer_ptr->CreateBuffer(
		VeRenderBuffer::TYPE_DYNAMIC, VeRenderBuffer::USEAGE_CB, 1024);
	VE_FLOAT3 triangleVertices[] =
	{
		{ -1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f }
	};

	VeRenderBufferPtr spVB = ve_renderer_ptr->CreateBuffer(VeRenderBuffer::TYPE_STATIC, VeRenderBuffer::USEAGE_VB, sizeof(triangleVertices));
	ve_renderer_ptr->BeginSyncCopy();
	spVB->UpdateSync(triangleVertices);
	ve_renderer_ptr->EndSyncCopy();*/

	VeRenderWindowPtr spRenderWindow = ve_renderer_ptr->CreateRenderWindow(
		"RenderTest", VE_WINDOWPOS_CENTERED, VE_WINDOWPOS_CENTERED, 800, 800, 0);

	auto spRes = ve_res_mgr.GetResource("stone_d.dds");
	auto spRes1 = ve_res_mgr.GetResource("rocks_d.dds");
	auto spRes2 = ve_res_mgr.GetResource("cubeUffizi.dds");

	ve_renderer_ptr->BeginSyncCopy();
	//spRes->Load();
	//spRes1->Load();
	spRes2->Load();
	ve_renderer_ptr->EndSyncCopy();

	const VeChar8* apcList[] =
	{
		"final_output"
	};

	spRenderWindow->SetupCompositorList(apcList, sizeof(apcList) / sizeof(*apcList));

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
			ve_renderer_ptr->BeginSyncCopy();
			ve_renderer_ptr->EndSyncCopy();

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

				VE_VECTOR eye = { 500,0,500,0 };
				VE_VECTOR focus = { 0,0,0,0 };
				VE_VECTOR up = g_MathIdentityR2;
				VE_MATRIX4X3 view = VeMatrixLookAtLH(eye, focus, up);

				VE_MATRIX view4;
				view4.r[0] = view.r[0];
				view4.r[1] = view.r[1];
				view4.r[2] = view.r[2];
				view4.r[3] = g_MathIdentityR3;
				VE_MATRIX proj = VeMatrixPerspectiveFovLH(VE_MATH_PI_2_F, 720.f / 1280.f, 0.25f, 8000.f);

				view4 = VeMatrixMultiply(view4, proj);


				//static VeFloat32 s_Test = 0.0f;
				//s_Test -= ve_time.GetDeltaTime() * 0.3f;

				//VE_FLOAT4A* pvData = (VE_FLOAT4A*)spCB->Map();
				//VE_VECTOR v = { 0, s_Test, 0.5f, 0.5f };
				//VeStoreFloat4A(pvData, v);
				////VeStoreFloat4x4A(pvData, view4);
				//spCB->Unmap();

				//VE_FLOAT4X4A* pvData = (VE_FLOAT4X4A*)spCB->Map();
				//VeStoreFloat4x4A(pvData, view4);
				//spCB->Unmap();

				//spRenderWindow->Update(spCB, spVB);

				spRenderWindow->Begin();

				for (VeUInt32 i(0); i < spRenderWindow->GetRecorderNum(); ++i)
				{
					spRenderWindow->Record(i);
				}

				//spRenderWindow->Update(spCB, spVB);
				spRenderWindow->End();

			}
		}		
	}

	VE_ASSERT(!spRenderWindow);
	spRes = nullptr;
	spRes1 = nullptr;
	spRes2 = nullptr;
	//spCB = nullptr;
	//spVB = nullptr;

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
