////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      System
//  File name:   VeTime.cpp
//  Created:     2016/07/20 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//--------------------------------------------------------------------------
#ifndef BUILD_PLATFORM_WIN
//--------------------------------------------------------------------------
#include <sys/time.h>
//--------------------------------------------------------------------------
void VeTimeOfDay(uint64_t& u64Time)
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
#ifdef BUILD_PLATFORM_WIN
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_u64Frequency);
	QueryPerformanceCounter((LARGE_INTEGER *)&m_u64CurrentCount);
#else
	m_u64Frequency = 1000000;
	VeTimeOfDay(m_u64CurrentCount);
#endif
	m_f64InvFrequency = 1.0 / double(m_u64Frequency);
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

#ifdef BUILD_PLATFORM_WIN
	QueryPerformanceCounter((LARGE_INTEGER*)&m_u64CurrentCount);
#else
	VeTimeOfDay(m_u64CurrentCount);
#endif

	uint64_t u64DeltaCount = m_u64CurrentCount - m_u64LastRecordCount;

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

	double f64LastTime = m_f64Time;

	uint64_t u64Elapsed = m_u64CurrentCount - m_u64StartRecordCount;
	m_f64Time = double(u64Elapsed) * m_f64InvFrequency;
	m_f64DeltaTime = m_f64Time - f64LastTime;
}
//--------------------------------------------------------------------------
