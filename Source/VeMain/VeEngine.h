////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeEngine.h
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

VeSmartPointer(VeRenderer);

class VE_MAIN_API VeEngine : public VeSingleton<VeEngine>
{
public:
	VeEngine() noexcept;

	~VeEngine() noexcept;

	inline const VeEventQueuePtr& GetEventQueue() noexcept;

	inline const VeVideoDevicePtr& GetVideoDevice() noexcept;

	inline const VeKeyboardPtr& GetKeyboard() noexcept;

	inline const VeMousePtr& GetMouse() noexcept;

	inline const VeRendererPtr& GetRenderer() noexcept;

	void Init() noexcept;

	void Term() noexcept;

	void Update() noexcept;

protected:
	void InitEventQueue() noexcept;

	void TermEventQueue() noexcept;

	void InitVideo() noexcept;

	void TermVideo() noexcept;

	void InitInputSystem() noexcept;

	void TermInputSystem() noexcept;

	void InitRenderer() noexcept;

	void TermRenderer() noexcept;

	VeEventQueuePtr m_spEventQueue;
	VeVideoDevicePtr m_spVideo;
	VeKeyboardPtr m_spKeyboard;
	VeMousePtr m_spMouse;
	VeRendererPtr m_spRenderer;

};

#define ve_engine VeEngine::GetSingleton()
#define ve_event_queue_ptr ve_engine.GetEventQueue()
#define ve_video_ptr ve_engine.GetVideoDevice()
#define ve_keyboard_ptr ve_engine.GetKeyboard()
#define ve_mouse_ptr ve_engine.GetMouse()
#define ve_renderer_ptr ve_engine->GetRenderer()

#include "VeEngine.inl"
