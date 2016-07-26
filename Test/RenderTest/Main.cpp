////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      RenderTest
//  File name:   Main.cpp
//  Created:     2016/07/23 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#include "RenderTest.h"

const char* g_pcPakName = "com.venus3d.RenderTest";

int32_t VeEntry(int32_t, char*[]) noexcept
{
	VeRendererPtr spD3D12 = VeRenderer::Create(VE_RENDER_VULKAN);
	if (!spD3D12) return 0;
	VE_TRY_CALL(spD3D12->Init());

	VeRenderWindowPtr spWindow = spD3D12->CreateRenderWindow("D3D12", 1024, 768);
	if (!spWindow) return 0;

	VeDesktopVideoPtr spDesktop = VeDynamicCast(VeDesktopVideo, venus3d.GetVideo());
	if (!spDesktop) return 0;

	while (true)
	{
		venus3d.GetTime().Update();
		spDesktop->PumpEvents();
		{
			static float s_f32TimeCount(0);
			static uint32_t s_u32FrameCount(0);
			static uint32_t s_u32FPS;

			s_f32TimeCount += venus3d.GetTime().GetDeltaTime();
			++s_u32FrameCount;

			if (s_f32TimeCount > 1.0f)
			{
				s_u32FPS = s_u32FrameCount;
				char s_acFPS[64];
				VeSprintf(s_acFPS, "%s[FPS:%d]", "D3D12", s_u32FrameCount);
				spWindow->SetTitle(s_acFPS);
				s_f32TimeCount -= floorf(s_f32TimeCount);
				s_u32FrameCount = 0;
			}
		}
		if (spWindow->IsValid())
		{
			spWindow->Begin();
			spWindow->End();
		}
		else
		{
			spWindow = nullptr;
			break;
		}
	}
	
	VE_TRY_CALL(spD3D12->Term());

	return 0;
}
