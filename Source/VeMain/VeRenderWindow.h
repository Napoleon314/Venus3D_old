////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderWindow.h
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeRenderWindow : public VeRefObject
{
	VE_NO_COPY(VeRenderWindow);
	VeRTTIDecl(VeRenderWindow);
public:
	inline const VeWindowPtr& GetTargetWindow() noexcept;

	virtual bool IsValid() noexcept = 0;


protected:
	VeRenderWindow() noexcept;

	virtual ~VeRenderWindow() noexcept;

	VeWindowPtr m_spTargetWindow;

};

VeSmartPointer(VeRenderWindow);

#include "VeRenderWindow.inl"
