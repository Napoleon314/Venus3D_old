////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Framework
//  File name:   VeApplication.cpp
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

#include "stdafx.h"

//--------------------------------------------------------------------------
VeApplication::VeApplication() noexcept
{

}
//--------------------------------------------------------------------------
VeApplication::~VeApplication() noexcept
{
	
}
//--------------------------------------------------------------------------
void VeApplication::Init() noexcept
{
	m_spRenderer = VeRenderer::Create(VE_RENDER_D3D12);
	VE_ASSERT(m_spRenderer);
	VE_TRY_CALL(m_spRenderer->Init());
	const char* pcCaption = venus3d.GetConfig("caption");
	m_kCaption = pcCaption ? pcCaption : "D3D12RenderWindow";
	uint32_t w(1024), h(768);
	const char* pcResolution = venus3d.GetConfig("resolution");
	if (pcResolution)
	{
		char acBuf[64];
		VeStrcpy(acBuf, pcResolution);
		char* pcHeight;
		char* pcWidth = vtd::strtok(acBuf, "x", &pcHeight);
		if (pcWidth) w = atoi(pcWidth);
		if (pcHeight) h = atoi(pcHeight);
	}
	m_spWindow = m_spRenderer->CreateRenderWindow(m_kCaption, w, h);
	VE_ASSERT(m_spWindow);
	OnInit();
}
//--------------------------------------------------------------------------
void VeApplication::Term() noexcept
{
	OnTerm();
	m_spWindow = nullptr;
	VE_TRY_CALL(m_spRenderer->Term());
	m_spRenderer = nullptr;
}
//--------------------------------------------------------------------------
void VeApplication::ProcessEvents() noexcept
{
	VE_ASSERT(venus3d.GetVideo());
	venus3d.GetVideo()->PumpEvents();
}
//--------------------------------------------------------------------------
void VeApplication::Update() noexcept
{
	venus3d.GetTime().Update();
	{
		static float s_f32TimeCount(0);
		static uint32_t s_u32FrameCount(0);

		s_f32TimeCount += venus3d.GetTime().GetDeltaTime();
		++s_u32FrameCount;

		if (s_f32TimeCount > 1.0f)
		{
			char s_acFPS[64];
			VeSprintf(s_acFPS, "%s [FPS:%d]", m_kCaption.c_str(), s_u32FrameCount);
			m_spWindow->SetTitle(s_acFPS);
			s_f32TimeCount -= floorf(s_f32TimeCount);
			s_u32FrameCount = 0;
		}
	}
	OnUpdate();
}
//--------------------------------------------------------------------------
void VeApplication::Render() noexcept
{
	m_spWindow->Begin();
	OnRender();	
	m_spWindow->End();
}
//--------------------------------------------------------------------------
void VeApplication::Loop() noexcept
{
	while (m_bLoop)
	{
		ProcessEvents();
		if (m_spWindow && m_spWindow->IsValid())
		{
			Update();
			Render();
		}
		else
		{
			Quit();
		}
	}
}
//--------------------------------------------------------------------------
void VeApplication::Go() noexcept
{
	Init();
	Loop();
	Term();
}
//--------------------------------------------------------------------------
