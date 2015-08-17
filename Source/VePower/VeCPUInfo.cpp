////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeCPUInfo.cpp
//  Created:     2015/08/17 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"
#if defined(_MSC_VER) && (_MSC_VER >= 1500) && (defined(_M_IX86) || defined(_M_X64))
#include <intrin.h>
#ifndef _WIN64
#define __MMX__
#define __3dNOW__
#endif
#define __SSE__
#define __SSE2__
#elif defined(__MINGW64_VERSION_MAJOR)
#include <intrin.h>
#else
#ifdef __ALTIVEC__
#if HAVE_ALTIVEC_H && !defined(__APPLE_ALTIVEC__)
#include <altivec.h>
#undef pixel
#endif
#endif
#ifdef __MMX__
#include <mmintrin.h>
#endif
#ifdef __3dNOW__
#include <mm3dnow.h>
#endif
#ifdef __SSE__
#include <xmmintrin.h>
#endif
#ifdef __SSE2__
#include <emmintrin.h>
#endif
#endif

//--------------------------------------------------------------------------
VeInt32 VeCPUInfo::ms_i32CPUCount = 0;
//--------------------------------------------------------------------------
VeInt32 VeCPUInfo::GetCount() noexcept
{
	if (!ms_i32CPUCount)
	{
#		ifdef VE_PLATFORM_WIN
		SYSTEM_INFO kInfo;
		GetSystemInfo(&kInfo);
		ms_i32CPUCount = kInfo.dwNumberOfProcessors;
#		endif

		if (ms_i32CPUCount <= 0)
		{
			ms_i32CPUCount = 1;
		}
	}
	return ms_i32CPUCount;
}
//--------------------------------------------------------------------------
