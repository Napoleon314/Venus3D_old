/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _SDL_config_h
#define _SDL_config_h

#include "SDL_platform.h"

/**
 *  \file SDL_config.h
 */

/* Add any platform that doesn't build using the configure system. */
#ifdef USING_PREMAKE_CONFIG_H
#include "SDL_config_premake.h"
#elif defined(__WIN32__)
#include "SDL_config_windows.h"
#elif defined(__WINRT__)
#include "SDL_config_winrt.h"
#elif defined(__MACOSX__)
#include "SDL_config_macosx.h"
#elif defined(__IPHONEOS__)
#include "SDL_config_iphoneos.h"
#elif defined(__ANDROID__)
#include "SDL_config_android.h"
#elif defined(__PSP__)
#include "SDL_config_psp.h"
#else
/* This is a minimal configuration just to get SDL running on new platforms */
#include "SDL_config_minimal.h"
#endif /* platform config */

#ifdef USING_GENERATED_CONFIG_H
#error Wrong SDL_config.h, check your include path?
#endif

#ifndef SDL_DEPRECATED
#  if (__GNUC__ >= 4)  /* technically, this arrived in gcc 3.1, but oh well. */
#    define SDL_DEPRECATED __attribute__((deprecated))
#  else
#    define SDL_DEPRECATED
#  endif
#endif

#ifndef SDL_UNUSED
#  ifdef __GNUC__
#    define SDL_UNUSED __attribute__((unused))
#  else
#    define SDL_UNUSED
#  endif
#endif

/* Some compilers use a special export keyword */
#ifndef SDL_SYMBOL_EXPORT
#	ifdef BUILD_PLATFORM_WIN
#		define SDL_SYMBOL_EXPORT __declspec(dllexport)
#	else
#		define SDL_SYMBOL_EXPORT
#	endif
#endif

#ifndef SDL_SYMBOL_IMPORT
#	ifdef BUILD_PLATFORM_WIN
#		define SDL_SYMBOL_IMPORT __declspec(dllimport)
#	else
#		define SDL_SYMBOL_IMPORT
#	endif
#endif

#ifndef SDL_API
#	ifdef BUILD_SHARED_LIB
#		ifdef SDL_EXPORTS
#			define SDL_API SDL_SYMBOL_EXPORT
#		else
#			define SDL_API SDL_SYMBOL_IMPORT
#		endif
#	else
#		define SDL_API
#	endif
#endif

/* By default SDL uses the C calling convention */
#ifndef SDLCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define SDLCALL __cdecl
#else
#define SDLCALL
#endif
#endif /* SDLCALL */

/* Removed DECLSPEC on Symbian OS because SDL cannot be a DLL in EPOC */
#ifdef __SYMBIAN32__
#undef DECLSPEC
#define DECLSPEC
#endif /* __SYMBIAN32__ */

/* Force structure packing at 4 byte alignment.
This is necessary if the header is included in code which has structure
packing set to an alternate value, say for loading structures from disk.
The packing is reset to the previous value in close_code.h
*/
#if defined(_MSC_VER) || defined(__MWERKS__) || defined(__BORLANDC__)
#ifdef _MSC_VER
#pragma warning(disable: 4103)
#endif
#ifdef __BORLANDC__
#pragma nopackwarning
#endif
#ifdef _M_X64
/* Use 8-byte alignment on 64-bit architectures, so pointers are aligned */
#pragma pack(push,8)
#else
#pragma pack(push,4)
#endif
#endif /* Compiler needs structure packing set */

#ifndef SDL_INLINE
#if defined(__GNUC__)
#define SDL_INLINE __inline__
#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      defined(__DMC__) || defined(__SC__) || \
      defined(__WATCOMC__) || defined(__LCC__) || \
      defined(__DECC)
#define SDL_INLINE __inline
#ifndef __inline__
#define __inline__ __inline
#endif
#else
#define SDL_INLINE inline
#ifndef __inline__
#define __inline__ inline
#endif
#endif
#endif /* SDL_INLINE not defined */

#ifndef SDL_FORCE_INLINE
#if defined(_MSC_VER)
#define SDL_FORCE_INLINE __forceinline
#elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#define SDL_FORCE_INLINE __attribute__((always_inline)) static __inline__
#else
#define SDL_FORCE_INLINE static SDL_INLINE
#endif
#endif /* SDL_FORCE_INLINE not defined */

/* Apparently this is needed by several Windows compilers */
#if !defined(__MACH__)
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */
#endif /* ! Mac OS X - breaks precompiled headers */

#endif /* _SDL_config_h */
