////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   Venus3D.cpp
//  Created:     2016/07/06 by Albert
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
extern VeVideoDevicePtr CreateVideoDevice() noexcept;
//--------------------------------------------------------------------------
#ifdef VE_ENABLE_KEYBOARD
extern VeKeyboardPtr CreateKeyboard() noexcept;
#endif
//--------------------------------------------------------------------------
#ifdef VE_ENABLE_MOUSE
extern VeMousePtr CreateMouse() noexcept;
#endif
//--------------------------------------------------------------------------
Venus3D::Venus3D(const char* pcProcessName, uint32_t u32InitMask) noexcept
	: m_kProcessName(pcProcessName), CORE("Venus3D", m_kLog)
	, USER(m_kProcessName, m_kLog)

{
	Init(u32InitMask);
}
//--------------------------------------------------------------------------
Venus3D::~Venus3D() noexcept
{
	Term();
}
//--------------------------------------------------------------------------
void Venus3D::Init(uint32_t u32InitMask)
{
	VE_MASK_CONDITION(u32InitMask, VE_INIT_LOG, InitLog());
	VE_MASK_CONDITION(u32InitMask, VE_INIT_JOB, InitJob());
	VE_MASK_CONDITION(u32InitMask, VE_INIT_EVENT, InitEvent());
	VE_MASK_CONDITION(u32InitMask, VE_INIT_VIDEO, InitVideo());
	VE_MASK_ADD(m_u32ActiveMask, u32InitMask);
}
//--------------------------------------------------------------------------
void Venus3D::Term()
{
	VE_MASK_CONDITION(m_u32ActiveMask, VE_INIT_VIDEO, TermVideo());
	VE_MASK_CONDITION(m_u32ActiveMask, VE_INIT_EVENT, TermEvent());
	VE_MASK_CONDITION(m_u32ActiveMask, VE_INIT_JOB, TermJob());
	VE_MASK_CONDITION(m_u32ActiveMask, VE_INIT_LOG, TermLog());
	VE_MASK_CLEAR(m_u32ActiveMask);
}
//--------------------------------------------------------------------------
void Venus3D::InitLog() noexcept
{
	m_kLog.SetTarget(&VeLog::ConsoleOutput);
}
//--------------------------------------------------------------------------
void Venus3D::TermLog() noexcept
{
	m_kLog.SetTarget(nullptr);
}
//--------------------------------------------------------------------------
void Venus3D::InitJob() noexcept
{
	uint32_t u32CPUNum = VeThreadHardwareConcurrency();
	VeJobSystem::Create(u32CPUNum, VE_CLAMP(u32CPUNum >> 1, 1, VE_JOB_BG_BUFFER_MASK + 1));
}
//--------------------------------------------------------------------------
void Venus3D::TermJob() noexcept
{
	VeJobSystem::Destory();
}
//--------------------------------------------------------------------------
void Venus3D::InitEvent() noexcept
{
	m_spEventQueue = VE_NEW VeEventQueue();
	m_spEventQueue->Init();
#	ifdef VE_ENABLE_KEYBOARD
	if (!m_spKeyboard)
	{
		m_spKeyboard = CreateKeyboard();
		assert(m_spKeyboard);
		m_spKeyboard->Init();
	}
#	endif
#	ifdef VE_ENABLE_MOUSE
	if (!m_spMouse)
	{
		m_spMouse = CreateMouse();
		assert(m_spMouse);
		m_spMouse->Init();
	}
#	endif
}
//--------------------------------------------------------------------------
void Venus3D::TermEvent() noexcept
{
#	ifdef VE_ENABLE_MOUSE
	if (m_spMouse)
	{
		m_spMouse->Term();
		m_spMouse = nullptr;
	}
#	endif
#	ifdef VE_ENABLE_KEYBOARD
	if (m_spKeyboard)
	{
		m_spKeyboard->Term();
		m_spKeyboard = nullptr;
	}
#	endif
	m_spEventQueue->Term();
	m_spEventQueue = nullptr;
}
//--------------------------------------------------------------------------
void Venus3D::InitVideo() noexcept
{
	if (!m_spVideoDevice)
	{
		m_spVideoDevice = CreateVideoDevice();
		assert(m_spVideoDevice);
		m_spVideoDevice->Init();
	}
}
//--------------------------------------------------------------------------
void Venus3D::TermVideo() noexcept
{
	if (m_spVideoDevice)
	{
		m_spVideoDevice->Term();
		m_spVideoDevice = nullptr;
	}
}
//--------------------------------------------------------------------------
VeStackAllocator& Venus3D::GetStackAllocator() noexcept
{
	return VeThread::GetThreadLocalSingleton()->m_kAllocator;
}
//--------------------------------------------------------------------------
const VePoolAllocatorPtr& Venus3D::GetPoolAllocator(
	size_t stUnitSize) noexcept
{
	stUnitSize = (stUnitSize + 0xF) & (~0xF);
	std::lock_guard<vtd::spin_lock> l(m_kAllocatorLock);
	VePoolAllocatorPtr& spRes = m_kAllocatorMap[stUnitSize];
	if (!spRes)
	{
		spRes = VE_NEW VePoolAllocator(stUnitSize);
	}
	return spRes;
}
//--------------------------------------------------------------------------
