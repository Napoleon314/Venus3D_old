////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeTime.cpp
//  Created:     2015/08/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
#ifndef VE_PLATFORM_WIN
//--------------------------------------------------------------------------
#include <sys/time.h>
//--------------------------------------------------------------------------
void VeTimeOfDay(VeUInt64& u64Time)
{
	timeval kTime;
	gettimeofday(&kTime, nullptr);
	u64Time = kTime.tv_sec * 1000000ull + kTime.tv_usec;
}
#endif
//--------------------------------------------------------------------------
VeTime::VeTime() noexcept
{
	Reset();
}
//--------------------------------------------------------------------------
VeTime::~VeTime() noexcept
{

}
//--------------------------------------------------------------------------
void VeTime::Reset() noexcept
{
#ifdef VE_PLATFORM_WIN
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_u64Frequency);
	QueryPerformanceCounter((LARGE_INTEGER *)&m_u64CurrentCount);
#else
	m_u64Frequency = 1000000;
	VeTimeOfDay(m_u64CurrentCount);
#endif
	m_f64InvFrequency = 1.0 / VeFloat64(m_u64Frequency);
	m_u64StartRecordCount = m_u64CurrentCount;
	m_u64LastRecordCount = m_u64CurrentCount;
	m_f64Time = 0;
	m_f64DeltaTime = 0;
	m_bPaused = false;
	m_bInvert = false;
}
//--------------------------------------------------------------------------
void VeTime::Update() noexcept
{
	m_u64LastRecordCount = m_u64CurrentCount;

#ifdef VE_PLATFORM_WIN
	QueryPerformanceCounter((LARGE_INTEGER*)&m_u64CurrentCount);
#else
	VeTimeOfDay(m_u64CurrentCount);
#endif

	VeUInt64 u64DeltaCount = m_u64CurrentCount - m_u64LastRecordCount;

	if (m_bPaused)
	{
		m_u64StartRecordCount += u64DeltaCount;
	}
	else if (m_bInvert)
	{
		m_u64StartRecordCount += u64DeltaCount << 1;
		if (m_u64StartRecordCount > m_u64CurrentCount)
		{
			m_u64StartRecordCount = m_u64CurrentCount;
		}
	}

	VeFloat64 f64LastTime = m_f64Time;

	VeUInt64 u64Elapsed = m_u64CurrentCount - m_u64StartRecordCount;
	m_f64Time = VeFloat64(u64Elapsed) * m_f64InvFrequency;
	m_f64DeltaTime = m_f64Time - f64LastTime;
}
//--------------------------------------------------------------------------
