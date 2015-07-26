////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   WindowsVideo.cpp
//  Created:     2015/07/27 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "../VePowerPch.h"
#include "WindowsVideo.h"

//--------------------------------------------------------------------------
VeVideoDevicePtr CreateVideoDevice() noexcept
{
	return VE_NEW WindowsVideoDevice();
}
//--------------------------------------------------------------------------
WindowsVideoDevice::WindowsVideoDevice() noexcept
{
	m_pcName = "windows";
	m_pcDesc = "Venus3D Windows Video Driver";
}
//--------------------------------------------------------------------------
WindowsVideoDevice::~WindowsVideoDevice() noexcept
{

}
//--------------------------------------------------------------------------
void WindowsVideoDevice::Init() noexcept
{
	InitModes();
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::Term() noexcept
{

}
//--------------------------------------------------------------------------
void WindowsVideoDevice::InitModes() noexcept
{
	
}
//--------------------------------------------------------------------------
