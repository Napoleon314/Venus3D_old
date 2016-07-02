////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Venus3D
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   Venus3D.h
//  Created:     2016/07/01 by Albert
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

#pragma once

#if !defined(__cplusplus)
#	error C++ compiler required.
#endif

#if defined(DEBUG) | defined(_DEBUG)
#	define VE_DEBUG
#elif defined(BUILD_SHIP)
#	define VE_SHIPPING
#else
#	define VE_RELEASE
#endif

#define VE_STRINGIZE(X) VE_DO_STRINGIZE(X)
#define VE_DO_STRINGIZE(X) #X

#define VE_JOIN(X, Y) VE_DO_JOIN(X, Y)
#define VE_DO_JOIN(X, Y) VE_DO_JOIN2(X, Y)
#define VE_DO_JOIN2(X, Y) X##Y

#if defined(__clang__)

#if __cplusplus < 201103L
#	error "-std=c++11 must be turned on."
#endif

#define VE_COMPILER_CLANG
#define VE_COMPILER_NAME "clang"

#define CLANG_VERSION VE_JOIN(__clang_major__, __clang_minor__)

#define VE_CXX11_CORE_CONSTEXPR_SUPPORT
#define VE_CXX11_CORE_NOEXCEPT_SUPPORT

#if defined(__APPLE__)
#	if CLANG_VERSION >= 40
#		define VE_COMPILER_VERSION CLANG_VERSION
#	else
#		error "Unsupported compiler version. Please install Apple clang++ 4.0 or up."
#	endif
#	define VE_CXX11_LIBRARY_REGEX_SUPPORT
#	if __cplusplus >= 201402L
#		define VE_CXX14_LIBRARY_MAKE_UNIQUE
#	endif
#	define VE_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#	define VE_SYMBOL_IMPORT
#elif defined(__MINGW32__)
#	if CLANG_VERSION >= 34
#		define VE_COMPILER_VERSION CLANG_VERSION
#	else
#		error "Unsupported compiler version. Please install clang++ 3.4 or up."
#	endif
#	include <bits/c++config.h>
#	ifdef _GLIBCXX_USE_FLOAT128
#		undef _GLIBCXX_USE_FLOAT128
#	endif
#	ifdef _GLIBCXX_USE_INT128
#		undef _GLIBCXX_USE_INT128
#	endif
#	ifdef __GLIBCXX__
#		if __GLIBCXX__ < 20130322 // g++ 4.8
#			error "Unsupported library version. Please install clang++ with g++ 4.8 or up."
#		endif
#		if !defined(_GLIBCXX_HAS_GTHREADS)
#			error "_GLIBCXX_HAS_GTHREADS must be turned on."
#		endif
#		if __GLIBCXX__ >= 20140422 // g++ 4.9
#			define VE_CXX11_LIBRARY_REGEX_SUPPORT
#			if __cplusplus > 201103L
#				define VE_TS_LIBRARY_OPTIONAL_SUPPORT
#			endif
#			if __cplusplus >= 201402L
#				define VE_CXX14_LIBRARY_MAKE_UNIQUE
#			endif
#		endif
#	endif
#	define VE_SYMBOL_EXPORT __declspec(dllexport)
#	define VE_SYMBOL_IMPORT __declspec(dllimport)
#else
#	error "Clang++ on an unknown platform. Only Apple and Windows are supported."
#endif

#elif defined(__GNUC__)

#define VE_COMPILER_GCC

#include <bits/c++config.h>
#ifdef _GLIBCXX_USE_FLOAT128
#	undef _GLIBCXX_USE_FLOAT128
#endif
#ifdef _GLIBCXX_USE_INT128
#	undef _GLIBCXX_USE_INT128
#endif

#define GCC_VERSION KFL_JOIN(__GNUC__, __GNUC_MINOR__)

#if GCC_VERSION >= 48
#	define VE_COMPILER_VERSION GCC_VERSION
#else
#	error "Unsupported compiler version. Please install g++ 4.8 or up."
#endif

#if __cplusplus < 201103L
#	error "-std=c++11 or -std=c++0x must be turned on."
#endif
#if !defined(_GLIBCXX_HAS_GTHREADS)
#	error "_GLIBCXX_HAS_GTHREADS must be turned on."
#endif

#define VE_CXX11_CORE_CONSTEXPR_SUPPORT
#define VE_CXX11_CORE_NOEXCEPT_SUPPORT
#if VE_COMPILER_VERSION >= 49
#	define VE_CXX11_LIBRARY_REGEX_SUPPORT
#	if __cplusplus > 201103L
#		define VE_TS_LIBRARY_OPTIONAL_SUPPORT
#	endif
#	if __cplusplus >= 201402L
#		define VE_CXX14_LIBRARY_MAKE_UNIQUE
#	endif
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	define VE_SYMBOL_EXPORT __attribute__((__dllexport__))
#	define VE_SYMBOL_IMPORT __attribute__((__dllimport__))
#else
#	define VE_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#	define VE_SYMBOL_IMPORT
#endif

#elif defined(_MSC_VER)

#define VE_COMPILER_MSVC
#define VE_COMPILER_NAME vc

#define VE_HAS_DECLSPEC
#define VE_SYMBOL_EXPORT __declspec(dllexport)
#define VE_SYMBOL_IMPORT __declspec(dllimport)

#if _MSC_VER >= 1900
#	define VE_COMPILER_VERSION 140
#else
#	error "Unsupported compiler version. Please install vc14 or up."
#endif

#define VE_CXX11_LIBRARY_REGEX_SUPPORT
#define VE_TS_LIBRARY_FILESYSTEM_V2_SUPPORT
#define VE_CXX14_LIBRARY_MAKE_UNIQUE

#define VE_CXX11_CORE_CONSTEXPR_SUPPORT
#define VE_CXX11_CORE_NOEXCEPT_SUPPORT
#undef VE_TS_LIBRARY_FILESYSTEM_V2_SUPPORT
#define VE_TS_LIBRARY_FILESYSTEM_V3_SUPPORT

#ifdef BUILD_SHARED_LIB
#	pragma warning(disable: 4251) // STL classes are not dllexport.
#	pragma warning(disable: 4275) // Derived from non dllexport classes.
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#	define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _SCL_SECURE_NO_DEPRECATE
#	define _SCL_SECURE_NO_DEPRECATE
#endif

#else
#	error "Unknown compiler. Please install vc, g++ or clang."
#endif

#ifdef BUILD_SHARED_LIB
#	ifdef VENUS3D_EXPORTS
#		define VENUS_API VE_SYMBOL_EXPORT
#	else
#		define VENUS_API VE_SYMBOL_IMPORT
#	endif
#else
#	define VENUS_API
#endif

#if defined(BUILD_PLATFORM_WIN) || defined(BUILD_PLATFORM_LINUX) || defined(BUILD_PLATFORM_DARWIN)
#	define BUILD_PLATFORM_PC
#elif defined(BUILD_PLATFORM_IOS) || defined(BUILD_PLATFORM_ANDROID)
#	define BUILD_PLATFORM_MOBILE
#endif
#if defined(BUILD_PLATFORM_DARWIN) || defined(BUILD_PLATFORM_IOS)
#	define BUILD_PLATFORM_APPLE
#endif

#ifdef BUILD_PLATFORM_WIN
#	define VE_ENABLE_DIRECTX
#endif

#ifdef BUILD_PLATFORM_APPLE
#	define VE_ENABLE_METAL
#else
#	define VE_ENABLE_VULKAN
#endif

#define VE_SUCCEEDED(hr) ((hr)==0)
#define VE_FAILED(hr) ((hr)!=0)

#ifdef BUILD_PLATFORM_WIN
#	include <SDKDDKVer.h>
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <process.h>
#	include <direct.h>
#	include <io.h>
#	define VE_CALLBACK __stdcall
#	define VE_VECTORCALL __vectorcall
#else
#	include <dirent.h>
#	include <unistd.h>
#	include <sys/stat.h>
#	define VE_CALLBACK
#	define VE_VECTORCALL
#endif
#ifdef BUILD_PLATFORM_LINUX
#	include <pthread.h>
#	include <errno.h>
#	include <signal.h>
#	include <sys/time.h>
#	include <ctype.h>
#include <stdarg.h>
#endif
#ifdef BUILD_PLATFORM_ANDROID
#	include <pthread.h>
#	include <errno.h>
#	include <signal.h>
#	include <sys/time.h>
#	include <ctype.h>
#	include <android/log.h>
#	include <android/asset_manager.h>
#endif
#ifdef BUILD_PLATFORM_APPLE
#	include <libkern/OSAtomic.h>
#	include <pthread.h>
#	include <errno.h>
#	include <sys/time.h>
#   include <CoreFoundation/CoreFoundation.h>
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <math.h>
#include <float.h>

#define BUILD_INTRINSICS_LEVEL 2		//supposed to move in predefines.

#if defined(BUILD_ARCH_X86) || defined(BUILD_ARCH_X64)
#	if BUILD_INTRINSICS_LEVEL > 0
#		define VE_SSE_INTRINSICS
#		if BUILD_INTRINSICS_LEVEL > 1
#			define VE_AVX2_INTRINSICS
#		endif
#	else
#		define VE_NO_INTRINSICS
#	endif
#	ifdef VE_COMPILER_MSVC
#		include <intrin.h>
#	endif
#	ifdef VE_SSE_INTRINSICS
#		include <xmmintrin.h>
#		include <emmintrin.h>
#		include <pmmintrin.h>
#		include <smmintrin.h>
#	endif
#	ifdef VE_AVX2_INTRINSICS
#		include <immintrin.h>
#	endif
#elif defined(BUILD_ARCH_ARM)
#	if BUILD_INTRINSICS_LEVEL > 0
#		define VE_NEON_INTRINSICS
#	else
#		define VE_NO_INTRINSICS
#	endif
#else
#	define VE_NO_INTRINSICS
#endif

#include <functional>
#include <tuple>
#include <array>
#include <stack>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <mutex>

#define VE_MAX(a,b) (((a)>(b))?(a):(b))
#define VE_MIN(a,b) (((a)<(b))?(a):(b))
#define VE_CLAMP(a,l,h) ((a < l) ? l : ((a > h) ? h : a))

#define VE_INT8_MIN (-128)
#define VE_INT8_MAX (127)
#define VE_UINT8_MAX (0xffu)
#define VE_CHAR_MIN VE_INT8_MIN
#define VE_CHAR_MAX VE_INT8_MAX
#define VE_INT16_MIN (-32768)
#define VE_INT16_MAX (32767)
#define VE_UINT16_MAX (0xffffu)
#define VE_INT32_MIN (-2147483648)
#define VE_INT32_MAX (2147483647)
#define VE_UINT32_MAX (0xffffffffu)
#define VE_INT64_MAX (9223372036854775807i64)
#define VE_INT64_MIN (-9223372036854775808i64)
#define VE_UINT64_MAX (0xffffffffffffffffui64)

#define VE_MASK(location) (VeUInt32(0x01<<(location)))
#define VE_MASK_HAS_ANY(flag,mask) (((flag)&(mask))!=0)
#define VE_MASK_HAS_ALL(flag,mask) (((flag)&(mask))==(mask))
#define VE_MASK_ADD(flag,mask) ((flag)|=(mask))
#define VE_MASK_DEL(flag,mask) ((flag)&=(~(mask)))
#define VE_MASK_REMOVE(flag,mask) ((flag)&(~(mask)))
#define VE_MASK_CHANGE(location,flag)									\
	if(VE_MASK_HAS_ANY(flag,VE_MASK(location)))							\
		VE_MASK_DEL(flag,VE_MASK(location));							\
	else																\
		VE_MASK_ADD(flag,VE_MASK(location));

#define VE_MAKE_FOURCC(ch0, ch1, ch2, ch3)								\
	((VeDWord)(VeByte)(ch0) | ((VeDWord)(VeByte)(ch1) << 8) |			\
	((VeDWord)(VeByte)(ch2) << 16) | ((VeDWord)(VeByte)(ch3) << 24 ))

#define VeNoCopy(cls) \
	cls(const cls&) = delete;\
	cls& operator= (const cls&) = delete

#include "CPU/VeCPUInfo.h"
#include "CPU/VeSIMDMath.h"
