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
	struct ThreadData
	{
		ThreadData(std::function<void()> funcEntry) noexcept
			: m_kThread(funcEntry) {}

		std::thread m_kThread;
		VeThread::Event m_kEvent;
	};

	VeParallel() noexcept;

	virtual ~VeParallel() noexcept;

	void Do(std::function<void(VeUInt32)> funcTask) noexcept;

protected:
	std::function<void(VeUInt32)> m_kTask;
	VeSizeT m_stThreadNum = 0;
	VeThread::Event* m_pkLoopEventArray;
	std::thread* m_pkThreadArray = nullptr;
	VeThread::Event m_kEvent;



	bool m_bLoop = true;

};

#define ve_paral VeParallel::GetSingleton()
