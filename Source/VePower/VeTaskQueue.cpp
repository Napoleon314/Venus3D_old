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
void VeTaskQueue::AddBGTask(VeRefNode<DoTask>& kTask) noexcept
{
	VE_LOCK_MUTEX(m_kBGMutex);
	m_kBGTaskList[m_u32ActiveBGTask].attach_back(kTask);
}
//--------------------------------------------------------------------------
void VeTaskQueue::AddFGTask(VeRefNode<DoTask>& kTask) noexcept
{
	VE_LOCK_MUTEX(m_kFGMutex);
	m_kFGTaskList[m_u32ActiveFGTask].attach_back(kTask);
}
//--------------------------------------------------------------------------
void VeTaskQueue::RemoveTask(VeRefNode<DoTask>& kTask) noexcept
{
	VE_LOCK_MUTEX(m_kBGMutex);
	VE_LOCK_MUTEX(m_kFGMutex);
	kTask.detach();
}
//--------------------------------------------------------------------------
void VeTaskQueue::Run() noexcept
{
	VeRefList<DoTask>& kTaskList = m_kBGTaskList[!m_u32ActiveBGTask];
	for (auto func : kTaskList)
	{
		VeRefNode<DoTask>* pkNode = kTaskList.get_iter_node();
		VE_ASSERT(pkNode);
		pkNode->detach();
		pkNode->m_Content(*this);
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
		m_kBGMutex.Unlock();
	}

	{
		VE_LOCK_MUTEX(m_kFGMutex);
		if (m_kFGTaskList[!m_u32ActiveFGTask].empty())
		{
			m_u32ActiveFGTask = !m_u32ActiveFGTask;
		}
	}
	VeRefList<DoTask>& kTaskList = m_kFGTaskList[!m_u32ActiveFGTask];
	if (m_bTickOnce)
	{
		if (kTaskList.size())
		{
			VeRefNode<DoTask>* pkHead = kTaskList.get_head_node();
			pkHead->detach();
			pkHead->m_Content(*this);
		}
	}
	else
	{
		for (auto func : kTaskList)
		{
			VeRefNode<DoTask>* pkNode = kTaskList.get_iter_node();
			VE_ASSERT(pkNode);
			pkNode->detach();
			pkNode->m_Content(*this);
		}
		VE_ASSERT(kTaskList.empty());
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
