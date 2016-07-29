////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      RenderTest
//  File name:   RenderTest.cpp
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

#include "RenderTest.h"

//--------------------------------------------------------------------------
RenderTest::RenderTest() noexcept
	: VeApplication("com.venus3d.RenderTest", VE_MAKE_VERSION(0, 1))
{

}
//--------------------------------------------------------------------------
RenderTest::~RenderTest() noexcept
{

}
//--------------------------------------------------------------------------
void RenderTest::OnInit() noexcept
{
	
}
//--------------------------------------------------------------------------
void RenderTest::OnTerm() noexcept
{

}
//--------------------------------------------------------------------------
void RenderTest::OnUpdate() noexcept
{

}
//--------------------------------------------------------------------------
void RenderTest::OnRender() noexcept
{

}
//--------------------------------------------------------------------------
VeApplicationPtr VeApplication::Create(int32_t, char * []) noexcept
{
	return VE_NEW RenderTest();
}
//--------------------------------------------------------------------------
