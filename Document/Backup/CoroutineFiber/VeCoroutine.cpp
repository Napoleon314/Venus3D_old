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
#ifndef BUILD_PLATFORM_WIN
	m_pvStack = VeAlignedMalloc(m_u32StackSize, 64);
#endif
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
#else
	if (m_pvStack)
	{
		VeAlignedFree(m_pvStack);
		m_pvStack = nullptr;
	}
#endif
}
//--------------------------------------------------------------------------
void VeCoroutineBase::Push() noexcept
{
#ifdef BUILD_PLATFORM_WIN
	m_eStatus = VE_CO_RUNNING;
	m_spPrevious = m_spEnv->m_spRunning;
	m_spEnv->m_spRunning = this;
	SwitchToFiber(m_hFiber);
	VeCoroutineEnv* pkEnv = m_spEnv;
	if (m_eStatus == VE_CO_DEAD)
	{
		DeleteFiber(m_hFiber);
		m_hFiber = NULL;
		m_spEnv = nullptr;
	}
	pkEnv->m_spReturnFrom = nullptr;
#else
	if (m_eStatus == VE_CO_READY)
	{
		getcontext(&m_kContext);
		m_kContext.uc_stack.ss_sp = m_pvStack;
		m_kContext.uc_stack.ss_size = m_u32StackSize;
		m_kContext.uc_link = nullptr;
		uintptr_t ptr = (uintptr_t)this;
		makecontext(&m_kContext, (void(*)(void))m_pfuncEntry, 2, (uint32_t)ptr, (uint32_t)(ptr >> 32));
	}
	m_eStatus = VE_CO_RUNNING;
	m_spPrevious = m_spEnv->m_spRunning;
	m_spEnv->m_spRunning = this;
	swapcontext(&m_spPrevious->m_kContext, &m_kContext);
	VeCoroutineEnv* pkEnv = m_spEnv;
	if (m_eStatus == VE_CO_DEAD)
	{
		m_spEnv = nullptr;
	}
	pkEnv->m_spReturnFrom = nullptr;
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
	assert(m_eStatus == VE_CO_DEAD);
#ifdef BUILD_PLATFORM_WIN
    assert(!m_hFiber);
	m_hFiber = CreateFiber((SIZE_T)m_u32StackSize, pfuncEntry, this);
#else
	assert(m_pvStack);
	m_pfuncEntry = pfuncEntry;
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
		m_spReturnFrom = m_spRunning;
		m_spRunning = m_spReturnFrom->m_spPrevious;
		m_spReturnFrom->m_spPrevious = nullptr;
		m_spReturnFrom->m_eStatus = VE_CO_SUSPENDED;
#ifdef BUILD_PLATFORM_WIN
		SwitchToFiber(m_spRunning->m_hFiber);
#else
		swapcontext(&m_spReturnFrom->m_kContext, &m_spRunning->m_kContext);
#endif
	}
}
//--------------------------------------------------------------------------
void VeCoroutineEnv::Close() noexcept
{
	assert(m_spRunning);
	if (m_spRunning->m_spPrevious)
	{
		m_spReturnFrom = m_spRunning;
		m_spRunning = m_spReturnFrom->m_spPrevious;
		m_spReturnFrom->m_spPrevious = nullptr;
		m_spReturnFrom->m_eStatus = VE_CO_DEAD;
#ifdef BUILD_PLATFORM_WIN
		SwitchToFiber(m_spRunning->m_hFiber);
#else
		swapcontext(&m_spReturnFrom->m_kContext, &m_spRunning->m_kContext);
#endif
	}
}
//--------------------------------------------------------------------------
VeCoroutineEnv* VeCoroutineEnv::GetCurrent() noexcept
{
	return ms_spEnv_local;
}
//--------------------------------------------------------------------------
