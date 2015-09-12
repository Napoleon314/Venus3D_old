////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeLog.cpp
//  Created:     2015/03/02 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeLog::VeLog()
{

}
//--------------------------------------------------------------------------
VeLog::~VeLog()
{

	

}
//--------------------------------------------------------------------------
void VeLog::LogFormat(Type eType, const VeChar8* pcTag,
	const VeChar8* pcFormat, ...) noexcept
{
	va_list kArgs;
	va_start(kArgs, pcFormat);
	LogFormat(eType, pcTag, pcFormat, kArgs);
	va_end(kArgs);
}
//--------------------------------------------------------------------------
void VeLog::LogFormat(Type eType, const VeChar8* pcTag,
	const VeChar8* pcFormat, va_list kArgs) noexcept
{
	VeChar8 acBuffer[VE_LOG_BUFFER_SIZE];
	VeVsprintf(acBuffer, VE_LOG_BUFFER_SIZE, pcFormat, kArgs);
	Output(eType, pcTag, acBuffer);
}
//--------------------------------------------------------------------------
void VeLog::Output(Type eType, const VeChar8* pcTag,
	const VeChar8* pcContent) noexcept
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
