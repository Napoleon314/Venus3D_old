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

	VeTaskQueue() noexcept;

	virtual ~VeTaskQueue() noexcept;

	inline void SetTickOnce(bool bEnable) noexcept;

	inline bool IsTickOnce() noexcept;

	void AddBGTask(VeRefNode<DoTask>& kTask) noexcept;

	void AddFGTask(VeRefNode<DoTask>& kTask) noexcept;

	void RemoveTask(VeRefNode<DoTask>& kTask) noexcept;

	virtual void Run() noexcept override;

	void Update() noexcept;

	bool HasTask() noexcept;

protected:
	VeMutex m_kBGMutex;
	VeMutex m_kFGMutex;
	VeRefList<DoTask> m_kBGTaskList[2];
	VeRefList<DoTask> m_kFGTaskList[2];
	volatile VeUInt32 m_u32ActiveBGTask = 0;
	volatile VeUInt32 m_u32ActiveFGTask = 0;
	bool m_bTickOnce = false;

};

#include "VeTaskQueue.inl"
