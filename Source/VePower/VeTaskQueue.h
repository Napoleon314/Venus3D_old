////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeBGQueue.h
//  Created:     2015/03/06 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeTaskQueue : public VeThread
{
public:
	typedef std::function<void(VeTaskQueue&)> DoTask;

	VeTaskQueue(VeUInt32 u32Priority = VE_THREAD_PRIORITY_NORMAL,
		VeSizeT stStackSize = 32768) noexcept;

	~VeTaskQueue() noexcept;

	inline void SetTickOnce(bool bEnable) noexcept;

	inline bool IsTickOnce() noexcept;

	void AddBGTask(VeRefNode<DoTask>& kTask) noexcept;

	void AddFGTask(VeRefNode<DoTask>& kTask) noexcept;

	void RemoveTask(VeRefNode<DoTask>& kTask) noexcept;

	virtual void Run() noexcept;

	void Update() noexcept;

	bool HasTask() noexcept;

protected:
	VeThread::Mutex m_kMutex;
	VeRefList<DoTask> m_kBGTaskList;
	VeRefList<DoTask> m_kFGTaskList;
	bool m_bTickOnce = false;

};

#include "VeTaskQueue.inl"
