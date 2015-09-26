////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeFramework
//  File name:   VeApplication.cpp
//  Created:     2015/09/25 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeFrameworkPch.h"

//--------------------------------------------------------------------------
VeApplication::VeApplication(const VeChar8* pcConfig) noexcept
	: m_kString(pcConfig)
{

}
//--------------------------------------------------------------------------
VeApplication::~VeApplication() noexcept
{
	
}
//--------------------------------------------------------------------------
void VeApplication::Init() noexcept
{
	ve_sys.Init();
	ve_engine.Init();
	VeJSONDoc kDoc = VeOpenJSON(m_kString);
	auto it = kDoc.FindMember("resource");
	if (it != kDoc.MemberEnd())
	{
		ve_res_mgr.SetupGroup(it->value);
	}
	it = kDoc.FindMember("render");
	if (it != kDoc.MemberEnd())
	{
		ve_renderer_ptr->PreCache(ve_res_mgr.CreateDir(it->value("cache", "")),
			ve_res_mgr.CreateDir(it->value("shader", ""), false),
			ve_res_mgr.CreateDir(it->value("script", ""), false));
	}
	it = kDoc.FindMember("window");
	if (it != kDoc.MemberEnd())
	{
		m_kString = it->value("title", "");
		m_spMainWindow = ve_renderer_ptr->CreateRenderWindow(
			m_kString, VE_WINDOWPOS_CENTERED, VE_WINDOWPOS_CENTERED,
			it->value("width", 640), it->value("height", 480), 0);
		m_spMainWindow->SetSync(it->value("sync", false));
	}
	VE_ASSERT(m_spMainWindow);
	ve_renderer_ptr->BeginSyncCopy();
	OnInit();
	ve_renderer_ptr->EndSyncCopy();
}
//--------------------------------------------------------------------------
void VeApplication::Term() noexcept
{
	OnTerm();
	VE_ASSERT(!m_spMainWindow);
	ve_engine.Term();
	ve_sys.Term();
}
//--------------------------------------------------------------------------
void VeApplication::ProcessEvents() noexcept
{
	ve_video_ptr->PeekEvents(m_kEventCache);
	for (auto pkEvent : m_kEventCache)
	{
		VE_ASSERT(pkEvent->m_u32Type);
		switch (pkEvent->m_u32Type)
		{
		case VE_WINDOWEVENT:
			if (pkEvent->m_kWindow.m_u8Event == VE_WINDOWEVENT_CLOSE)
			{
				if (m_spMainWindow && pkEvent->m_kWindow.m_u32WindowID
					== m_spMainWindow->GetID())
				{
					m_spMainWindow = nullptr;
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
	ve_sys.Update();
	ve_engine.Update();
	ve_renderer_ptr->BeginSyncCopy();	
	OnUpdate();
	ve_renderer_ptr->EndSyncCopy();
}
//--------------------------------------------------------------------------
void VeApplication::Render() noexcept
{
	m_spMainWindow->Begin();
	OnRender();	
	m_spMainWindow->End();
}
//--------------------------------------------------------------------------
void VeApplication::Loop() noexcept
{
	while (m_bLoop)
	{
		ProcessEvents();
		if (m_spMainWindow)
		{
			Update();
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
