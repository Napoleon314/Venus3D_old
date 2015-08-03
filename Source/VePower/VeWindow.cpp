////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeWindow.cpp
//  Created:     2015/07/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeWindow::VeWindow() noexcept
{
	m_kNode.m_Content = this;
}
//--------------------------------------------------------------------------
VeWindow::~VeWindow() noexcept
{

	

}
//--------------------------------------------------------------------------
VeWindow* VeWindow::Cast(Data* pkData) noexcept
{
	return VeMemberCast(&VeWindow::m_kData, pkData);
}
//--------------------------------------------------------------------------
