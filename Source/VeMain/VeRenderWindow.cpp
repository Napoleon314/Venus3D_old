////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderWindow.cpp
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderWindow);
//--------------------------------------------------------------------------
VeRenderWindow::VeRenderWindow(const VeWindowPtr& spWindow) noexcept
	: m_spTargetWindow(spWindow)
{

}
//--------------------------------------------------------------------------
VeRenderWindow::~VeRenderWindow() noexcept
{
	if (m_bNeedDestory)
	{
		m_spTargetWindow->Destory();
	}
	m_spTargetWindow = nullptr;
}
//--------------------------------------------------------------------------
bool VeRenderWindow::IsValid() noexcept
{
	return m_spTargetWindow ? m_spTargetWindow->IsValid() : false;
}
//--------------------------------------------------------------------------
