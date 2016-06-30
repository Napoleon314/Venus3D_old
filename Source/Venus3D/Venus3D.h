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

#if defined(__clang__)

#if __cplusplus < 201103L
#	error "-std=c++11 must be turned on."
#endif

#define VE_COMPILER_CLANG
#define VE_COMPILER_NAME "clang"

#define CLANG_VERSION __clang_major__##__clang_minor__

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

#include <stdio.h>

#include <thread>


#include "CPU/VeCPUInfo.h"
