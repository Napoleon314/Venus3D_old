////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeFramework
//  File name:   VeApplication.h
//  Created:     2015/09/25 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VeApplication : public VeRefObject
{
public:
	inline const VeRenderWindowPtr& GetWindow() noexcept;

	virtual void Init() noexcept;

	virtual void Term() noexcept;	

	virtual void ProcessEvents() noexcept;

	virtual void Update() noexcept;

	virtual void Render() noexcept;

	virtual void Loop() noexcept;

	virtual void Go() noexcept;

	virtual void OnInit() noexcept {}

	virtual void OnTerm() noexcept {}

	virtual void OnUpdate() noexcept {}

	virtual void OnRender() noexcept {}

protected:
	VeApplication(const VeChar8* pcConfig) noexcept;

	virtual ~VeApplication() noexcept;

	VeFixedString m_kString;
	VeRenderWindowPtr m_spMainWindow;
	VeVector<VeEvent*> m_kEventCache;
	bool m_bLoop = true;

};

#include "VeApplication.inl"
