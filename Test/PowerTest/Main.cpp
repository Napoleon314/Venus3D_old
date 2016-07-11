////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      PowerTest
//  File name:   Main.cpp
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

#ifdef BUILD_PLATFORM_WIN
#	include <vld.h>
#endif
#include <Venus3D.h>

int main(/*int argc, char * argv[]*/)
{
	VeThread::Init();

	int a = 0;
	int s = 0;
	VeThread t1;
	t1.StartEntry([&]() noexcept
	{	
		while (true)
		{
			if (s)
			{
				printf("after: %d\n", ++a);
			}
			else
			{
				printf("before: %d\n", ++a);
			}
		}
	});

	std::this_thread::sleep_for(std::chrono::seconds(3));

	t1.Suspend();
	
	std::this_thread::sleep_for(std::chrono::seconds(3));

	s = 1;

	t1.Resume();

	t1.Join();

	VeThread::Term();
	
	return 0;
}
