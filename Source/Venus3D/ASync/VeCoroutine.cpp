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
#ifdef VE_ENABLE_COROUTINE
//--------------------------------------------------------------------------
VeCoroutine::VeCoroutine(size_t stStackSize) noexcept
	: m_kStack({nullptr,0}), m_hContext(nullptr), m_eState(STATE_DEAD)
{
	if (stStackSize)
	{
		m_kStack = create_fcontext_stack(stStackSize);
	}
}
//--------------------------------------------------------------------------
VeCoroutine::~VeCoroutine() noexcept
{
	destroy_fcontext_stack(&m_kStack);
}
//--------------------------------------------------------------------------
void VeCoroutine::prepare() noexcept
{
	assert(m_eState == STATE_DEAD && (!m_hContext));
	m_hContext = make_fcontext(m_kStack.sptr, m_kStack.ssize, &VeCoenvironment::Entry);
	m_eState = STATE_READY;
}
//--------------------------------------------------------------------------
void* VeCoroutine::start(Entry pfuncEntry, void* pvUserData) noexcept
{
	assert(m_eState == STATE_READY);
	VeCoenvironment::GetCurrent()->m_pfuncUserEntry = pfuncEntry;
	return _resume(pvUserData);
}
//--------------------------------------------------------------------------
void* VeCoroutine::resume(void* pvUserData) noexcept
{
	assert(m_eState == STATE_SUSPENDED);
	return _resume(pvUserData);
}
//--------------------------------------------------------------------------
void* VeCoroutine::_resume(void* pvUserData) noexcept
{
	VeCoenvironment* pkEnv = VeCoenvironment::GetCurrent();
	m_eState = STATE_RUNNING;
	m_pkPrevious = pkEnv->m_pkRunning;
	pkEnv->m_pkRunning = this;
	fcontext_transfer_t next = jump_fcontext(m_hContext, pvUserData);
	if (m_eState == STATE_DEAD)
	{
		m_hContext = nullptr;
	}
	else
	{
		assert(m_eState == STATE_SUSPENDED);
		m_hContext = next.ctx;
	}
	return next.data;
}
//--------------------------------------------------------------------------
#ifdef BUILD_PLATFORM_APPLE
extern pthread_key_t g_keyCurEnv;
#else
extern thread_local VeCoenvironment* g_pkCurEnv;
#endif
//--------------------------------------------------------------------------
VeCoenvironment::VeCoenvironment() noexcept
{
	m_kMain.m_eState = VeCoroutine::STATE_RUNNING;
	m_pkRunning = &m_kMain;
}
//--------------------------------------------------------------------------
VeCoenvironment::~VeCoenvironment() noexcept
{
	assert(m_pkRunning == &m_kMain);
	m_pkRunning = nullptr;
}
//--------------------------------------------------------------------------
void* VeCoenvironment::yield(void* pvUserData) noexcept
{
	VeCoenvironment* pkEnv = GetCurrent();
	assert(pkEnv->m_pkRunning != &(pkEnv->m_kMain));
	VeCoroutine* pkSuspended = pkEnv->m_pkRunning;
	pkSuspended->m_eState = VeCoroutine::STATE_SUSPENDED;
	pkEnv->m_pkRunning = pkSuspended->m_pkPrevious;
	pkSuspended->m_pkPrevious = nullptr;
	fcontext_transfer_t trans = jump_fcontext(pkEnv->m_pkRunning->m_hContext, pvUserData);
	pkEnv->m_pkRunning->m_pkPrevious->m_hContext = trans.ctx;
	return trans.data;
}
//--------------------------------------------------------------------------
void VeCoenvironment::Entry(fcontext_transfer_t trans) noexcept
{
	VeCoenvironment* pkEnv = GetCurrent();
	pkEnv->m_pkRunning->m_pkPrevious->m_hContext = trans.ctx;
	void* pvRes = pkEnv->m_pfuncUserEntry(trans.data);
	VeCoroutine* pkDead = pkEnv->m_pkRunning;
	pkDead->m_eState = VeCoroutine::STATE_DEAD;
	pkEnv->m_pkRunning = pkDead->m_pkPrevious;
	pkDead->m_pkPrevious = nullptr;
	jump_fcontext(pkEnv->m_pkRunning->m_hContext, pvRes);
}
//--------------------------------------------------------------------------
VeCoenvironment* VeCoenvironment::GetCurrent() noexcept
{
#ifdef BUILD_PLATFORM_APPLE
	return (VeCoenvironment*)pthread_getspecific(g_keyCurEnv);
#else
	assert(g_pkCurEnv);
	return g_pkCurEnv;
#endif
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
