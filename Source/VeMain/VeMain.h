////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeMain.h
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once


#if defined(VE_PLATFORM_WIN) && defined(VE_DYNAMIC_LIB)
#	ifdef VE_MAIN_EXPORT
#		define VE_MAIN_API __declspec (dllexport)
#	else
#		define VE_MAIN_API __declspec (dllimport)
#	endif
#else
#	define VE_MAIN_API
#endif

VE_MAIN_API void VeMain();
