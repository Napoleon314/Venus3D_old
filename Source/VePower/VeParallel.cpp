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
	m_stThreadNum = std::thread::hardware_concurrency();
	m_pkThreadArray = VeAlloc(std::thread, m_stThreadNum);
	for (VeSizeT i(0); i < m_stThreadNum; ++i)
	{
		new(m_pkThreadArray + i)std::thread([this]() noexcept
		{
			do 
			{
				{
					std::unique_lock<std::mutex> kLock(m_kLoopMutex);
					m_bLoop = true;
					m_kLoopCondition.wait(kLock);
				}
				if (m_kTask)
				{
					m_kTask();
				}
				if (m_u32Count.fetch_add(1, std::memory_order_relaxed) + 1 == m_stThreadNum)
				{
					std::lock_guard<std::mutex> kLock(m_kJoinMutex);
					m_bEndFlag = true;
					m_kJoinCondition.notify_all();
				}
			}
			while (m_bLoop);
		});
	}
}
//--------------------------------------------------------------------------
VeParallel::~VeParallel() noexcept
{
	{
		while (!m_bLoop);
		std::lock_guard<std::mutex> kLock(m_kLoopMutex);
		m_bLoop = false;
		m_kLoopCondition.notify_all();
	}
	for (VeSizeT i(0); i < m_stThreadNum; ++i)
	{
		m_pkThreadArray[i].join();
		m_pkThreadArray[i].~thread();
	}
	VeFree(m_pkThreadArray);
	m_pkThreadArray = nullptr;
	m_stThreadNum = 0;
}
//--------------------------------------------------------------------------
void VeParallel::Do(std::function<void()> funcTask) noexcept
{
	m_kTask = funcTask;
	m_u32Count.store(0, std::memory_order_relaxed);
	m_bEndFlag = false;
	{
		while (!m_bLoop);
		std::lock_guard<std::mutex> kLock(m_kLoopMutex);
		m_kLoopCondition.notify_all();
	}
	{
		std::unique_lock<std::mutex> kLock(m_kJoinMutex);
		if (!m_bEndFlag)
		{
			m_kJoinCondition.wait(kLock);
		}
	}
	VE_ASSERT(m_bEndFlag);
	m_kTask = nullptr;
}
//--------------------------------------------------------------------------
