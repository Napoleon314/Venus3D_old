////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeParallel.h
//  Created:     2015/09/12 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeParallel : public VeSingleton<VeParallel>
{
public:
	VeParallel() noexcept;

	virtual ~VeParallel() noexcept;

	void Do(std::function<void()> funcTask) noexcept;

protected:
	std::function<void()> m_kTask;
	VeSizeT m_stThreadNum = 0;
	std::thread* m_pkThreadArray = nullptr;
	std::mutex m_kLoopMutex;
	std::condition_variable m_kLoopCondition;
	std::mutex m_kJoinMutex;
	std::condition_variable m_kJoinCondition;
	std::atomic_uint m_u32Count;
	bool m_bEndFlag = false;
	bool m_bLoop = false;

};

#define ve_parallel VeParallel::GetSingleton()
