// Copyright (c) 1998-2011 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.

#pragma once

#if defined(_MSC_VER) // Microsoft Visual Studio

	#define MAX_DEPRECATED				__declspec(deprecated)
	#ifdef _MSC_EXTENSIONS
		#define MAX_SEALED				sealed
	#endif

#elif defined(__GNUG__) // GNU C++ compiler

	#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
	#if (GCC_VERSION >= 40200)
		#define MAX_DEPRECATED				__attribute__((__deprecated__))
		// Helper macros to replace __declspec(import) and __declspec(export)
		#define __declspec(method)			__declspec_##method
		#define _declspec(method)			__declspec_##method
		#define __declspec_dllexport		__attribute__ ((__visibility__("default")))
		#define __declspec_dllimport		//__attribute__ ((dllimport))  // Is this not needed? When compiling I get warning "dllimport attribute ignored"   
		#define __cdecl						//__attribute__ ((cdecl)) 
		#define nullptr						0
		#define MAX_SEALED					// sealed keyword is not supported under GCC
	#else
		#error Unsupported GCC version. Must be GCC 4.2 or higher.
	#endif

	// Some features are turned off when compiling with GCC
	#ifdef _ADESK_3DSMAX_MAC_
		#define MAX_MAC_NO_HEAP_COUNTER
	#else
		#error _ADESK_3DSMAX_MAC_ not defined; define it in the xcode configuration file.
	#endif
#else
	#error Unsupported compiler. Must be Microsoft Visual Studio or GCC 
#endif
