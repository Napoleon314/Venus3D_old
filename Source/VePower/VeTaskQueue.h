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
	VeNoCopy(VeTaskQueue);
public:
	typedef std::function<void(VeTaskQueue&)> DoTask;

	VeTaskQueue() noexcept;

	virtual ~VeTaskQueue() noexcept;

	inline void SetTickOnce(bool bEnable) noexcept;

	inline bool IsTickOnce() noexcept;

	void AddBGTask(DoTask funcTask) noexcept;

	void AddFGTask(DoTask funcTask) noexcept;

	virtual void Run() noexcept override;

	void Update() noexcept;

	bool HasTask() noexcept;

protected:
	VeSpinLock m_kBGMutex;
	VeSpinLock m_kFGMutex;
	VeList<DoTask> m_kBGTaskList[2];
	VeList<DoTask> m_kFGTaskList[2];
	VeUInt32 m_u32ActiveBGTask = 0;
	VeUInt32 m_u32ActiveFGTask = 0;
	bool m_bTickOnce = false;

};

VeSmartPointer(VeTaskQueue);

#include "VeTaskQueue.inl"
