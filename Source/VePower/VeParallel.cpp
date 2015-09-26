////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeParallel.cpp
//  Created:     2015/09/12 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeParallel::VeParallel() noexcept
{
	m_stThreadNum = VeCPUInfo::GetCount();
	m_pkLoopEventArray = VeAlloc(VeThread::Event, m_stThreadNum);
	for (VeSizeT i(0); i < m_stThreadNum; ++i)
	{
		new(m_pkLoopEventArray + i)VeThread::Event();
	}
	m_pkThreadArray = VeAlloc(std::thread, m_stThreadNum);
	for (VeSizeT i(0); i < m_stThreadNum; ++i)
	{
		new(m_pkThreadArray + i)std::thread([this,i]() noexcept
		{
			do
			{
				m_pkLoopEventArray[i].Wait();
				if (m_kTask)
				{
					m_kTask(VeUInt32(i));
				}				
				m_pkLoopEventArray[i].Reset();
				m_kEvent.Set();
			} while (m_bLoop);
		});
	}
}
//--------------------------------------------------------------------------
VeParallel::~VeParallel() noexcept
{
	m_bLoop = false;
	for (VeSizeT i(0); i < m_stThreadNum; ++i)
	{
		m_pkLoopEventArray[i].Set();
		m_pkThreadArray[i].join();
		m_pkThreadArray[i].~thread();
		m_pkLoopEventArray[i].~Event();
	}
	VeFree(m_pkThreadArray);
	m_pkThreadArray = nullptr;
	VeFree(m_pkLoopEventArray);
	m_pkLoopEventArray = nullptr;
	m_stThreadNum = 0;
}
//--------------------------------------------------------------------------
void VeParallel::Do(std::function<void(VeUInt32)> funcTask) noexcept
{
	m_kTask = funcTask;
	m_kEvent.Reset(VeUInt32(m_stThreadNum));
	for (VeSizeT i(0); i < m_stThreadNum; ++i)
	{
		m_pkLoopEventArray[i].Set();
	}
	m_kEvent.Wait();
	m_kTask = nullptr;
}
//--------------------------------------------------------------------------
