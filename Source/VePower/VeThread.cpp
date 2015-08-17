////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeThread.cpp
//  Created:     2015/02/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeThread::VeThread() noexcept
{
    m_u32State = 0;
	m_kJoin.Set();
	m_kCore = std::thread([this]() noexcept
	{
		this->Callback();
	});
}
//--------------------------------------------------------------------------
VeThread::~VeThread() noexcept
{
	while (IsRunning())
		;
	m_u32State = 2;
	m_kLoop.Set();
	m_kCore.join();
}
//--------------------------------------------------------------------------
void VeThread::Start() noexcept
{
	if (m_u32State == 0)
	{
		m_u32State = 1;
		m_kLoop.Set();
		m_kJoin.Reset();
	}
}
//--------------------------------------------------------------------------
void VeThread::Join() noexcept
{
	m_kJoin.Wait();
}
//--------------------------------------------------------------------------
void VeThread::Callback() noexcept
{
	do
	{
		m_kLoop.Wait();
		if (m_u32State == 1)
		{
			Run();
			m_u32State = 0;
			m_kLoop.Reset();
			m_kJoin.Set();
		}
	}
	while (m_u32State < 2);
}
//--------------------------------------------------------------------------
