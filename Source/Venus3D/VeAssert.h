////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Log
//  File name:   VeAssert.h
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

#pragma once

#define VE_ASSERT_CAPTION ("Venus3D Assertion")

#ifndef VE_ASSERT_LEVEL
#ifdef VE_DEFAULT_ASSERT_LEVEL
#define VE_ASSERT_LEVEL VE_DEFAULT_ASSERT_LEVEL
#elif defined(VE_DEBUG)
#define VE_ASSERT_LEVEL 2
#else
#define VE_ASSERT_LEVEL 1
#endif
#endif

#if defined(_MSC_VER)
extern void __cdecl __debugbreak(void);
#define VE_DEBUG_BREAK() __debugbreak()
#elif (!defined(__NACL__) && defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)))
#define VE_DEBUG_BREAK() __asm__ __volatile__ ( "int $3\n\t" )
#elif defined(HAVE_SIGNAL_H)
#include <signal.h>
#define VE_DEBUG_BREAK() raise(SIGTRAP)
#else
#define VE_DEBUG_BREAK() abort()
#endif

enum VeAbortState
{
	VE_AS_RETRY,
	VE_AS_STOP,
	VE_AS_BREAK,
	VE_AS_IGNORE,
	VE_AS_MAX
};

VENUS_API VeAbortState VeReportAssertion(const char* pcCond,
	const char* pcFile, int32_t i32Line, const char* pcFunc) noexcept;

#if defined(__clang__)
#if __has_feature(attribute_analyzer_noreturn)
__attribute__((analyzer_noreturn))
#endif
#endif

#define VE_ENABLED_ASSERT(condition)												\
	while (!(condition))															\
	{																				\
		switch (VeReportAssertion(#condition, __FILE__, __LINE__, __FUNCTION__))	\
		{																			\
		case VE_AS_RETRY:															\
			continue;																\
		case VE_AS_STOP:															\
			::exit(0);break;														\
		case VE_AS_BREAK:															\
			VE_DEBUG_BREAK();break;													\
		default:																	\
			break;																	\
		}																			\
		break;																		\
	}
#define VE_ASSERT_ALWAYS(condition) VE_ENABLED_ASSERT(condition)
#define VE_DISABLED_ASSERT(condition)

#if VE_ASSERT_LEVEL == 0
#   define VE_ASSERT(condition) VE_DISABLED_ASSERT(condition)
#   define VE_ASSERT_RELEASE(condition) VE_DISABLED_ASSERT(condition)
#   define VE_ASSERT_PARANOID(condition) VE_DISABLED_ASSERT(condition)
#elif VE_ASSERT_LEVEL == 1
#   define VE_ASSERT(condition) VE_DISABLED_ASSERT(condition)
#   define VE_ASSERT_RELEASE(condition) VE_ENABLED_ASSERT(condition)
#   define VE_ASSERT_PARANOID(condition) VE_DISABLED_ASSERT(condition)
#elif VE_ASSERT_LEVEL == 2
#   define VE_ASSERT(condition) VE_ENABLED_ASSERT(condition)
#   define VE_ASSERT_RELEASE(condition) VE_ENABLED_ASSERT(condition)
#   define VE_ASSERT_PARANOID(condition) VE_DISABLED_ASSERT(condition)
#elif VE_ASSERT_LEVEL == 3
#   define VE_ASSERT(condition) VE_ENABLED_ASSERT(condition)
#   define VE_ASSERT_RELEASE(condition) VE_ENABLED_ASSERT(condition)
#   define VE_ASSERT_PARANOID(condition) VE_ENABLED_ASSERT(condition)
#else
#   error Unknown assertion level.
#endif

#if VE_ASSERT_LEVEL >= 2
#	define VE_ASSERT_EQ(e,c) VE_ASSERT(e == c)
#	define VE_ASSERT_NE(e,c) VE_ASSERT(e != c)
#	define VE_ASSERT_LT(e,c) VE_ASSERT(e < c)
#	define VE_ASSERT_GT(e,c) VE_ASSERT(e > c)
#	define VE_ASSERT_LE(e,c) VE_ASSERT(e <= c)
#	define VE_ASSERT_GE(e,c) VE_ASSERT(e >= c)
#else
#	define VE_ASSERT_EQ(e,c) (e)
#	define VE_ASSERT_NE(e,c) (e)
#	define VE_ASSERT_LT(e,c) (e)
#	define VE_ASSERT_GT(e,c) (e)
#	define VE_ASSERT_LE(e,c) (e)
#	define VE_ASSERT_GE(e,c) (e)
#endif

#ifdef assert
#	undef assert
#endif
#define assert(e) VE_ASSERT(e)
#define assert_eq(e,c) VE_ASSERT_EQ(e,c)
#define assert_ne(e,c) VE_ASSERT_NE(e,c) 
#define assert_lt(e,c) VE_ASSERT_LT(e,c) 
#define assert_gt(e,c) VE_ASSERT_GT(e,c)
#define assert_le(e,c) VE_ASSERT_LE(e,c)
#define assert_ge(e,c) VE_ASSERT_GE(e,c)
