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

//--------------------------------------------------------------------------
VeAbortState VeReportAssertion(const char* pcCond, const char* pcFile,
	int32_t i32Line, const char* pcFunc) noexcept
{
	auto& spVideo = Venus3D::Ref().GetVideo();
	if (spVideo)
	{
		char acBuffer[VE_MAX_LOG_MESSAGE];
		VeSprintf(acBuffer,
			"Assertion failed!\n\n"
			"File: %s\n"
			"Line: %d\n"
			"Function: %s\n\n"
			"Expression: %s\n\n"
			"For information on how your program can cause an assertion failure\n\n"
			"(Press Retry to debug the application)",
			pcFile, i32Line, pcFunc, pcCond);
		switch (spVideo->MessageBoxSync(VE_ASSERT_CAPTION, acBuffer,
			VE_MB_ABORTRETRYIGNORE | VE_MB_ERROR))
		{
		case VE_MB_IDABORT:
			return VE_AS_STOP;
		case VE_MB_IDRETRY:
			return VE_AS_BREAK;
		case VE_MB_IDIGNORE:
			return VE_AS_IGNORE;
		default:
			return VE_AS_RETRY;
		};
	}
	else
	{
		VeCoreLogE("*****************************************************");
		VeCoreLogE("Assertion failed!");
		VeCoreLogE("File: ", pcFile);
		VeCoreLogE("Line: ", i32Line);
		VeCoreLogE("Function: ", pcFunc);
		VeCoreLogE("Expression: ", pcCond);
		VeCoreLogE("*****************************************************");
		return VE_AS_BREAK;
	}
}
//--------------------------------------------------------------------------
