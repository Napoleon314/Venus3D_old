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

	inline const VeRendererPtr& GetRenderer() noexcept;

	void Init() noexcept;

	void Term() noexcept;

protected:
	void InitRenderer() noexcept;

	void TermRenderer() noexcept;

	VeRendererPtr m_spRenderer;

};

#define ve_engine VeEngine::GetSingleton()
#define ve_renderer_ptr ve_engine->GetRenderer()

#include "VeEngine.inl"
