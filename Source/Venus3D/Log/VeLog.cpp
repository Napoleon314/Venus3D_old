////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Log
//  File name:   VeLog.cpp
//  Created:     2016/07/14 by Albert
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
VeLog::VeLog()
{

}
//--------------------------------------------------------------------------
VeLog::~VeLog()
{



}
//--------------------------------------------------------------------------
void VeLog::LogFormat(Type eType, const char* pcTag,
	const char* pcFormat, ...) noexcept
{
	va_list kArgs;
	va_start(kArgs, pcFormat);
	LogFormat(eType, pcTag, pcFormat, kArgs);
	va_end(kArgs);
}
//--------------------------------------------------------------------------
void VeLog::LogFormat(Type eType, const char* pcTag,
	const char* pcFormat, va_list kArgs) noexcept
{
	char acBuffer[VE_MAX_LOG_MESSAGE];
	VeVsprintf(acBuffer, VE_MAX_LOG_MESSAGE, pcFormat, kArgs);
	Output(eType, pcTag, acBuffer);
}
//--------------------------------------------------------------------------
void VeLog::Output(Type eType, const char* pcTag,
	const char* pcContent) noexcept
{
	if (m_funcTarget)
	{
#		ifdef VE_RELEASE
		if (VeUInt32(eType) < VeUInt32(m_eLevel)) return;
#		endif
		m_funcTarget(eType, pcTag, pcContent);
	}
}
//--------------------------------------------------------------------------
