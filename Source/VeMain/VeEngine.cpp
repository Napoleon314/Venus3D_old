////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeEngine.cpp
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
extern VeVideoDevicePtr CreateVideoDevice() noexcept;
//--------------------------------------------------------------------------
#ifdef VE_ENABLE_KEYBOARD
extern VeKeyboardPtr CreateKeyboard() noexcept;
#endif
//--------------------------------------------------------------------------
#ifdef VE_ENABLE_MOUSE
extern VeMousePtr CreateMouse() noexcept;
#endif
//--------------------------------------------------------------------------
#ifdef VE_ENABLE_D3D12
extern VeRendererPtr CreateRendererD3D12() noexcept;
#endif
//--------------------------------------------------------------------------
VeEngine::VeEngine() noexcept
{

	

}
//--------------------------------------------------------------------------
VeEngine::~VeEngine() noexcept
{

}
//--------------------------------------------------------------------------
void VeEngine::Init() noexcept
{
	InitEventQueue();
	InitInputSystem();
	InitVideo();
	InitRenderer();
}
//--------------------------------------------------------------------------
void VeEngine::Term() noexcept
{
	TermRenderer();
	TermVideo();
	TermInputSystem();
	TermEventQueue();
}
//--------------------------------------------------------------------------
void VeEngine::Update() noexcept
{
	m_spEventQueue->FlushEvents();
}
//--------------------------------------------------------------------------
void VeEngine::InitEventQueue() noexcept
{
	m_spEventQueue = VE_NEW VeEventQueue();
	m_spEventQueue->Init();
}
//--------------------------------------------------------------------------
void VeEngine::TermEventQueue() noexcept
{
	m_spEventQueue->Term();
	m_spEventQueue = nullptr;
}
//--------------------------------------------------------------------------
void VeEngine::InitVideo() noexcept
{
	if (!m_spVideo)
	{
		m_spVideo = CreateVideoDevice();
		VE_ASSERT(m_spVideo);
		m_spVideo->Init();
	}
}
//--------------------------------------------------------------------------
void VeEngine::TermVideo() noexcept
{
	if (m_spVideo)
	{
		m_spVideo->Term();
		m_spVideo = nullptr;
	}
}
//--------------------------------------------------------------------------
void VeEngine::InitInputSystem() noexcept
{
#	ifdef VE_ENABLE_KEYBOARD
	if (!m_spKeyboard)
	{
		m_spKeyboard = CreateKeyboard();
		VE_ASSERT(m_spKeyboard);
		m_spKeyboard->Init();
	}
#	endif
#	ifdef VE_ENABLE_MOUSE
	if (!m_spMouse)
	{
		m_spMouse = CreateMouse();
		VE_ASSERT(m_spMouse);
		m_spMouse->Init();
	}
#	endif
}
//--------------------------------------------------------------------------
void VeEngine::TermInputSystem() noexcept
{
#	ifdef VE_ENABLE_KEYBOARD
	if (m_spKeyboard)
	{
		m_spKeyboard->Term();
		m_spKeyboard = nullptr;
	}
#	endif
#	ifdef VE_ENABLE_MOUSE
	if (m_spMouse)
	{
		m_spMouse->Term();
		m_spMouse = nullptr;
	}
#	endif
}
//--------------------------------------------------------------------------
void VeEngine::InitRenderer() noexcept
{
#	ifdef VE_ENABLE_D3D12
	if (!m_spRenderer)
	{
		m_spRenderer = CreateRendererD3D12();
		if (!m_spRenderer->Init())
		{
			m_spRenderer->Term();
			m_spRenderer = nullptr;
		}
	}
#	endif
	if (m_spRenderer)
	{
		m_spRenderer->RegistResTypes();
	}
}
//--------------------------------------------------------------------------
void VeEngine::TermRenderer() noexcept
{
	if (m_spRenderer)
	{
		m_spRenderer->UnregistResTypes();
		m_spRenderer->Term();
		m_spRenderer = nullptr;
	}
}
//--------------------------------------------------------------------------
