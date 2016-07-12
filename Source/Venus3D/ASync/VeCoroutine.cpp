////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      ASync
//  File name:   VeCoroutine.cpp
//  Created:     2016/07/11 by Albert
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
static thread_local VeCoroutineEnvPtr ms_spEnv_local;
//--------------------------------------------------------------------------
VeCoroutineBase::VeCoroutineBase(Entry pfuncEntry,
	uint32_t u32Stack) noexcept
	: m_u32StackSize(u32Stack ? u32Stack : VE_CO_DEFAULT_STACK)
{
	Restart(pfuncEntry);
}
//--------------------------------------------------------------------------
VeCoroutineBase::~VeCoroutineBase() noexcept
{
#ifdef BUILD_PLATFORM_WIN
	if (m_hFiber)
	{
		DeleteFiber(m_hFiber);
		m_hFiber = NULL;
	}
#endif
}
//--------------------------------------------------------------------------
void VeCoroutineBase::Push() noexcept
{
	m_eStatus = VE_CO_RUNNING;
	m_spPrevious = m_spEnv->m_spRunning;
	m_spEnv->m_spRunning = this;
#ifdef BUILD_PLATFORM_WIN
	SwitchToFiber(m_hFiber);
	if (m_eStatus == VE_CO_DEAD)
	{
		DeleteFiber(m_hFiber);
		m_hFiber = NULL;
	}
#endif
}
//--------------------------------------------------------------------------
void VeCoroutineBase::Resume() noexcept
{
	if (m_eStatus == VE_CO_READY || m_eStatus == VE_CO_SUSPENDED)
	{
		if (!ms_spEnv_local)
		{
			ms_spEnv_local = VE_NEW VeCoroutineEnv();
		}
		assert(ms_spEnv_local);
		if (!m_spEnv)
		{
			m_spEnv = ms_spEnv_local;
		}
		if (m_spEnv == ms_spEnv_local && !m_spPrevious)
		{
			Push();
		}
	}
}
//--------------------------------------------------------------------------
void VeCoroutineBase::Restart(Entry pfuncEntry) noexcept
{
#ifdef BUILD_PLATFORM_WIN
    assert(m_eStatus == VE_CO_DEAD && !m_hFiber);
	m_hFiber = CreateFiber((SIZE_T)m_u32StackSize, pfuncEntry, this);
#endif
	m_eStatus = VE_CO_READY;
    pfuncEntry = nullptr;
}
//--------------------------------------------------------------------------
VeCoroutineEnv::VeCoroutineEnv() noexcept
{
	m_spMain = VE_NEW VeCoroutineBase();
	m_spMain->m_eStatus = VE_CO_RUNNING;
#ifdef BUILD_PLATFORM_WIN
	m_spMain->m_hFiber = ConvertThreadToFiber(NULL);
#endif
	m_spRunning = m_spMain;
}
//--------------------------------------------------------------------------
void VeCoroutineEnv::Suspend() noexcept
{
	assert(m_spRunning);
	if (m_spRunning->m_spPrevious)
	{
		{
			VeCoroutineBasePtr spTop = m_spRunning;
			m_spRunning = spTop->m_spPrevious;
			spTop->m_spPrevious = nullptr;
			spTop->m_eStatus = VE_CO_SUSPENDED;
		}
#ifdef BUILD_PLATFORM_WIN
		SwitchToFiber(m_spRunning->m_hFiber);
#endif
	}
}
//--------------------------------------------------------------------------
void VeCoroutineEnv::Close() noexcept
{
	assert(m_spRunning);
	if (m_spRunning->m_spPrevious)
	{
		{
			VeCoroutineBasePtr spTop = m_spRunning;
			m_spRunning = spTop->m_spPrevious;
			spTop->m_spPrevious = nullptr;
			spTop->m_eStatus = VE_CO_DEAD;
		}
#ifdef BUILD_PLATFORM_WIN
		SwitchToFiber(m_spRunning->m_hFiber);
#endif
	}
}
//--------------------------------------------------------------------------
VeCoroutineEnv* VeCoroutineEnv::GetCurrent() noexcept
{
	return ms_spEnv_local;
}
//--------------------------------------------------------------------------
