////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeBGQueue.cpp
//  Created:     2015/03/06 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeTaskQueue::VeTaskQueue() noexcept
{

}
//--------------------------------------------------------------------------
VeTaskQueue::~VeTaskQueue() noexcept
{
	VE_ASSERT(!IsRunning());
	VE_ASSERT(m_kBGTaskList[0].empty() && m_kFGTaskList[0].empty());
	VE_ASSERT(m_kBGTaskList[1].empty() && m_kFGTaskList[1].empty());
}
//--------------------------------------------------------------------------
void VeTaskQueue::AddBGTask(DoTask funcTask) noexcept
{
	VE_LOCK_MUTEX(m_kBGMutex);	
	m_kBGTaskList[m_u32ActiveBGTask].push_back(funcTask);
}
//--------------------------------------------------------------------------
void VeTaskQueue::AddFGTask(DoTask funcTask) noexcept
{
	VE_LOCK_MUTEX(m_kFGMutex);	
	m_kFGTaskList[m_u32ActiveFGTask].push_back(funcTask);
}
//--------------------------------------------------------------------------
void VeTaskQueue::Run() noexcept
{
	VeList<DoTask>& kTaskList = m_kBGTaskList[!m_u32ActiveBGTask];
	while (kTaskList.size())
	{
		auto task = kTaskList.begin();
		(*task)(*this);
		kTaskList.erase(task);
	}
	VE_ASSERT(kTaskList.empty());
}
//--------------------------------------------------------------------------
void VeTaskQueue::Update() noexcept
{
	if (!IsRunning())
	{
		VE_LOCK_MUTEX(m_kBGMutex);
		VeSizeT stSize = m_kBGTaskList[m_u32ActiveBGTask].size();
		if (stSize)
		{
			m_u32ActiveBGTask = !m_u32ActiveBGTask;
			VE_ASSERT(m_kBGTaskList[m_u32ActiveBGTask].empty());
			Start();
		}
	}
	{
		VE_LOCK_MUTEX(m_kFGMutex);
		if (m_kFGTaskList[!m_u32ActiveFGTask].empty())
		{
			m_u32ActiveFGTask = !m_u32ActiveFGTask;
		}
	}
	VeList<DoTask>& kTaskList = m_kFGTaskList[!m_u32ActiveFGTask];
	if (kTaskList.size())
	{
		do 
		{
			auto task = kTaskList.begin();
			(*task)(*this);
			kTaskList.erase(task);
		}
		while (kTaskList.size() && (!m_bTickOnce));
	}
}
//--------------------------------------------------------------------------
bool VeTaskQueue::HasTask() noexcept
{
	if (IsRunning())
	{
		return true;
	}
	else
	{
		VE_LOCK_MUTEX(m_kBGMutex);
		VE_LOCK_MUTEX(m_kFGMutex);
		if (m_kBGTaskList[m_u32ActiveBGTask].size())
		{
			return true;
		}
		if (m_kFGTaskList[m_u32ActiveFGTask].size())
		{
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------------------
