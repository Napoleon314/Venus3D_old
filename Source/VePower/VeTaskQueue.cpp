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
VeTaskQueue::VeTaskQueue(VeUInt32 u32Priority,
	VeSizeT stStackSize) noexcept
	: VeThread(u32Priority, stStackSize)
{

}
//--------------------------------------------------------------------------
VeTaskQueue::~VeTaskQueue() noexcept
{
	VE_ASSERT(!IsRunning());
	VE_ASSERT(m_kBGTaskList.empty() && m_kFGTaskList.empty());
}
//--------------------------------------------------------------------------
void VeTaskQueue::AddBGTask(VeRefNode<DoTask>& kTask) noexcept
{
	VE_AUTO_LOCK_MUTEX(m_kMutex);
	m_kBGTaskList.attach_back(kTask);
}
//--------------------------------------------------------------------------
void VeTaskQueue::AddFGTask(VeRefNode<DoTask>& kTask) noexcept
{
	VE_AUTO_LOCK_MUTEX(m_kMutex);
	m_kBGTaskList.attach_back(kTask);
}
//--------------------------------------------------------------------------
void VeTaskQueue::RemoveTask(VeRefNode<DoTask>& kTask) noexcept
{
	VE_AUTO_LOCK_MUTEX(m_kMutex);
	VE_ASSERT(kTask.is_attach(m_kBGTaskList) || kTask.is_attach(m_kFGTaskList));
	kTask.detach();
}
//--------------------------------------------------------------------------
void VeTaskQueue::Run() noexcept
{
	while (true)
	{
		DoTask funcTask(nullptr);
		{
			VE_AUTO_LOCK_MUTEX(m_kMutex);
			if (m_kBGTaskList.size())
			{
				VeRefNode<DoTask>* pkNode = m_kBGTaskList.get_head_node();
				funcTask = pkNode->m_Content;
				pkNode->detach();
			}
		}
		if (funcTask)
		{
			funcTask(*this);
		}
		else
		{
			break;
		}
	}
}
//--------------------------------------------------------------------------
void VeTaskQueue::Update() noexcept
{
	if (!IsRunning())
	{
		VE_AUTO_LOCK_MUTEX(m_kMutex);
		if (m_kBGTaskList.size()) Start();
	}

	while (true)
	{
		DoTask funcTask(nullptr);
		{
			VE_AUTO_LOCK_MUTEX(m_kMutex);
			if (m_kFGTaskList.size())
			{
				VeRefNode<DoTask>* pkNode = m_kBGTaskList.get_head_node();
				funcTask = pkNode->m_Content;
				pkNode->detach();
			}
		}
		if (funcTask)
		{
			funcTask(*this);
		}
		else
		{
			break;
		}
		if (m_bTickOnce)
		{
			break;
		}
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
		VE_AUTO_LOCK_MUTEX(m_kMutex);
		return m_kBGTaskList.size() || m_kFGTaskList.size();
	}
}
//--------------------------------------------------------------------------
