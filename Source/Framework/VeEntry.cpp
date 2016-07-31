////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Framework
//  File name:   VeEntry.cpp
//  Created:     2016/07/23 by Albert
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
#ifdef BUILD_PLATFORM_WIN
//--------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int argc(0);
	char* argv[256];
	{
		char* pcContent;
		char* pcTemp = vtd::strtok(lpCmdLine, " ", &pcContent);
		while (pcTemp)
		{
			argv[argc++] = pcTemp;
			pcTemp = vtd::strtok<CHAR>(NULL, " ", &pcContent);
		}
	}
	VeInitData kData =
	{
		VeApplication::Name(),
		VeApplication::Version(),
		VE_INIT_WINDOW,
		"startup.cfg"
	};
	kData.m_hInstance = hInstance;
	kData.m_hPrevInstance = hPrevInstance;
	kData.m_i32CmdShow = nCmdShow;
	kData.m_i32Argc = argc;
	kData.m_ppcArgv = argv;
	VeInit(kData);
	{
		VeApplicationPtr spApp = VeApplication::Create();
		VE_ASSERT(spApp);
		spApp->Go();
	}
	VeTerm();
	return 0;
}
//--------------------------------------------------------------------------
#elif defined(BUILD_PLATFORM_ANDROID)
//--------------------------------------------------------------------------
#else
//--------------------------------------------------------------------------
int main(int32_t argc, char * argv[])
{
	VeInitData kData;
	FillInitDataCommon(kData, argc, argv);
    VeInit(kData);
    VeTerm();
    return 0;
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
