////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   VeJobSystem.cpp
//  Created:     2016/07/15 by Albert
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
VeJobSystem::~VeJobSystem() noexcept
{
	Term();
}
//--------------------------------------------------------------------------
void VeJobSystem::Init(size_t stFGNum, size_t stBGNum)
{
	Term();
	if (stFGNum)
	{
		m_pkFGThreads = VeAlloc(VeThread, stFGNum);
		for (size_t i(0); i < stFGNum; ++i)
		{
			vtd::allocator<VeThread>::construct(m_pkFGThreads + i, VE_JOB_FG_PRIORITY);
		}
		m_stNumFGThreads = stFGNum;
	}
	if (stBGNum)
	{
		m_pkBGThreads = VeAlloc(VeThread, stBGNum);
		for (size_t i(0); i < stBGNum; ++i)
		{
			vtd::allocator<VeThread>::construct(m_pkBGThreads + i, VE_JOB_BG_PRIORITY);
		}
		m_stNumBGThreads = stBGNum;
	}
}
//--------------------------------------------------------------------------
void VeJobSystem::Term()
{
	if (m_pkFGThreads)
	{
		for (size_t i(0); i < m_stNumFGThreads; ++i)
		{
			vtd::allocator<VeThread>::destroy(m_pkFGThreads + i);
		}
		VeFree(m_pkFGThreads);
		m_pkFGThreads = nullptr;
		m_stNumFGThreads = 0;
	}
	if (m_stNumBGThreads)
	{
		for (size_t i(0); i < m_stNumBGThreads; ++i)
		{
			vtd::allocator<VeThread>::destroy(m_pkBGThreads + i);
		}
		VeFree(m_pkBGThreads);
		m_pkBGThreads = nullptr;
		m_stNumBGThreads = 0;
	}
}
//--------------------------------------------------------------------------
