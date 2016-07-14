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
static const char* s_apcLogTypeNames[VeLog::TYPE_MAX] =
{
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR"
};
//--------------------------------------------------------------------------
static void OutputToConsole(VeLog::Type eType, const char* pcTag,
	const char* pcText) noexcept
{
#	if defined(BUILD_PLATFORM_ANDROID)
	const android_LogPriority aeLogTypeMap[VeLog::TYPE_MAX] =
	{
		ANDROID_LOG_DEBUG,
		ANDROID_LOG_INFO,
		ANDROID_LOG_WARN,
		ANDROID_LOG_ERROR
	};
	__android_log_print(aeLogTypeMap[eType], pcTag, "%s", pcText);
#	elif defined(BUILD_PLATFORM_WIN)
	char acLogBuffer[VE_MAX_LOG_MESSAGE + 64];
	VeSprintf(acLogBuffer, "%s,%s: %s\n", s_apcLogTypeNames[eType], pcTag, pcText);
	static vtd::spin_lock lock;
	{
		std::lock_guard<vtd::spin_lock> l(lock);
		OutputDebugStringA(acLogBuffer);
		printf(acLogBuffer);
	}
#	else
	printf("%s,%s: %s\n", s_apcLogTypeNames[eType], pcTag, pcText);
#	endif
}
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
	VE_MASK_ADD(m_u32ActiveMask, u32InitMask);
}
//--------------------------------------------------------------------------
void Venus3D::Term()
{
	VE_MASK_CONDITION(m_u32ActiveMask, VE_INIT_LOG, TermLog());
	VE_MASK_CLEAR(m_u32ActiveMask);
}
//--------------------------------------------------------------------------
void Venus3D::InitLog() noexcept
{
	m_kLog.SetTarget(&OutputToConsole);
}
//--------------------------------------------------------------------------
void Venus3D::TermLog() noexcept
{
	m_kLog.SetTarget(nullptr);
}
//--------------------------------------------------------------------------
