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

#ifndef assert_LEVEL
#ifdef VE_DEFAULT_ASSERT_LEVEL
#define assert_LEVEL VE_DEFAULT_ASSERT_LEVEL
#elif defined(VE_DEBUG)
#define SDL_ASSERT_LEVEL 2
#else
#define SDL_ASSERT_LEVEL 1
#endif
#endif /* assert_LEVEL */

#if defined(_MSC_VER)
/* Don't include intrin.h here because it contains C++ code */
extern void __cdecl __debugbreak(void);
#define VE_DEBUG_BREAK() __debugbreak()
#elif (!defined(__NACL__) && defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)))
#define VE_DEBUG_BREAK() __asm__ __volatile__ ( "int $3\n\t" )
#elif defined(HAVE_SIGNAL_H)
#include <signal.h>
#define VE_DEBUG_BREAK() raise(SIGTRAP)
#else
/* How do we trigger breakpoints on this platform? */
#define VE_DEBUG_BREAK() abort()
#endif

/* "while (0,0)" fools Microsoft's compiler's /W4 warning level into thinking
this condition isn't constant. And looks like an owl's face! */
#ifdef _MSC_VER  /* stupid /W4 warnings. */
#define VE_NULL_WHILE_LOOP_CONDITION (0,0)
#else
#define VE_NULL_WHILE_LOOP_CONDITION (0)
#endif

#define VE_DISABLED_ASSERT(condition) \
    do { (void) sizeof ((condition)); } while (VE_NULL_WHILE_LOOP_CONDITION)

enum VeAssertState
{
	VE_ASSERTION_RETRY,
	VE_ASSERTION_BREAK,
	VE_ASSERTION_ABORT,
	VE_ASSERTION_IGNORE,
	VE_ASSERTION_ALWAYS_IGNORE
};

struct VeAssertData
{
	int32_t always_ignore;
	uint32_t trigger_count;
	const char *condition;
	const char *filename;
	int32_t linenum;
	const char *function;
	const VeAssertData *next;
};

#if (SDL_ASSERT_LEVEL > 0)


VENUS_API VeAssertState VeReportAssertion(VeAssertData* data,
	const char *func, const char *file, int line) noexcept;

#if defined(__clang__)
#if __has_feature(attribute_analyzer_noreturn)
__attribute__((analyzer_noreturn))
#endif
#endif
#endif

typedef VeAssertState (*VeAssertionHandler)(
	const VeAssertData* data, void* userdata);

