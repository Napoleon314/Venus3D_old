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
	m_spD3D12 = VeRenderer::Create(VE_RENDER_D3D12);
	try
	{
		m_spD3D12->Init();
	}
	catch (const char* pcMsg)
	{
		VeCoreLogE(pcMsg);
		m_spD3D12 = nullptr;
	}

	m_spD3D12Window = m_spD3D12->CreateRenderWindow("D3D12",
		VE_WINDOWPOS_CENTERED, VE_WINDOWPOS_CENTERED, 1024, 768, 0);

	OnInit();
}
//--------------------------------------------------------------------------
void VeApplication::Term() noexcept
{
	OnTerm();
	try
	{
		m_spD3D12->Term();
	}
	catch (const char* pcMsg)
	{
		VeCoreLogE(pcMsg);
	}
	m_spD3D12 = nullptr;
}
//--------------------------------------------------------------------------
void VeApplication::ProcessEvents() noexcept
{
	venus3d.GetEventQueue()->PeekEvents(m_kEventCache);
	for (auto pkEvent : m_kEventCache)
	{
		assert(pkEvent->m_u32Type);
		switch (pkEvent->m_u32Type)
		{
		case VE_WINDOWEVENT:
			if (pkEvent->m_kWindow.m_u8Event == VE_WINDOWEVENT_CLOSE)
			{
				if (m_spD3D12Window && pkEvent->m_kWindow.m_u32WindowID
					== m_spD3D12Window->GetID())
				{
					m_spD3D12Window = nullptr;
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
			m_bLoop = false;
			break;
		default:
			break;
		}
	}
	m_kEventCache.clear();
}
//--------------------------------------------------------------------------
void VeApplication::Update() noexcept
{
	OnUpdate();
}
//--------------------------------------------------------------------------
void VeApplication::Render() noexcept
{
	m_spD3D12Window->Begin();
	OnRender();	
	m_spD3D12Window->End();
}
//--------------------------------------------------------------------------
void VeApplication::Loop() noexcept
{
	while (m_bLoop)
	{
		ProcessEvents();
		if (m_spD3D12Window)
		{
			//m_spD3D12->BeginSyncCopy();
			Update();
			//m_spD3D12->EndSyncCopy();
			Render();
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
