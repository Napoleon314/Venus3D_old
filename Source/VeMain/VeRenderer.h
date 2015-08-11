////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderer.h
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeRenderer : public VeRefObject
{
public:
	enum API
	{
		API_NONE = 0x0,
		API_D3D11 = 0x1,
		API_D3D12 = 0x2,
		API_OGL = 0x4,
		API_OGLES2 = 0x8,
		API_OGLES3 = 0x10,
		API_MASK = 0xFF
	};

	inline API GetAPI() const noexcept;

	virtual bool Init() noexcept = 0;

	virtual void Term() noexcept = 0;

protected:
	VeRenderer() noexcept;

	virtual ~VeRenderer() noexcept;

	API m_eType = API_NONE;

};

#include "VeRenderer.inl"
