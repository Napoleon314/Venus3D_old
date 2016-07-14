////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Log
//  File name:   VeAssert.cpp
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

static VeAssertData *triggered_assertions = nullptr;

//static void AddAssertionToReport(VeAssertData *data)
//{
//	/* (data) is always a static struct defined with the assert macros, so
//	we don't have to worry about copying or allocating them. */
//	data->trigger_count++;
//	if (data->trigger_count == 1) {  /* not yet added? */
//		data->next = triggered_assertions;
//		triggered_assertions = data;
//	}
//}

//VeAssertState VeReportAssertion(VeAssertData *data, const char *func,\
//	const char *file, int line) noexcept
//{
//	static int assertion_running = 0;
//	static vtd::spin_lock spinlock;
//	VeAssertState state = assertION_IGNORE;
//
//	/* doing this because Visual C is upset over assigning in the macro. */
//	if (data->trigger_count == 0) {
//		data->function = func;
//		data->filename = file;
//		data->linenum = line;
//	}
//
//	AddAssertionToReport(data);
//
//	assertion_running++;
//	if (assertion_running > 1) {   /* assert during assert! Abort. */
//		if (assertion_running == 2) {
//			SDL_AbortAssertion();
//		}
//		else if (assertion_running == 3) {  /* Abort asserted! */
//			SDL_ExitProcess(42);
//		}
//		else {
//			while (1) { /* do nothing but spin; what else can you do?! */ }
//		}
//	}
//
//	if (!data->always_ignore) {
//		state = assertion_handler(data, assertion_userdata);
//	}
//
//	switch (state)
//	{
//	case SDL_ASSERTION_ABORT:
//		SDL_AbortAssertion();
//		return SDL_ASSERTION_IGNORE;  /* shouldn't return, but oh well. */
//
//	case SDL_ASSERTION_ALWAYS_IGNORE:
//		state = SDL_ASSERTION_IGNORE;
//		data->always_ignore = 1;
//		break;
//
//	case SDL_ASSERTION_IGNORE:
//	case SDL_ASSERTION_RETRY:
//	case SDL_ASSERTION_BREAK:
//		break;  /* macro handles these. */
//	}
//
//	assertion_running--;
//	SDL_UnlockMutex(assertion_mutex);
//
//	return state;
//}
