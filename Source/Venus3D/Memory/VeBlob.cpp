////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeBlob.cpp
//  Created:     2016/07/09 by Albert
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
VeBlob::VeBlob() noexcept
{

}
//--------------------------------------------------------------------------
VeBlob::VeBlob(size_t stSize) noexcept
{
	if (stSize)
	{
		m_pvBuffer = VeMalloc(stSize + 1);
		((char*)m_pvBuffer)[stSize] = 0;
		m_stByteSize = stSize;
	}
}
//--------------------------------------------------------------------------
VeBlob::VeBlob(void* pvBuffer, size_t stSize) noexcept
{
	if (stSize)
	{
		m_pvBuffer = pvBuffer;
		m_stByteSize = stSize;
		m_bNeedFree = false;
	}
}
//--------------------------------------------------------------------------
VeBlob::~VeBlob() noexcept
{
	if (m_pvBuffer && m_bNeedFree)
	{
		VeFree(m_pvBuffer);
	}
}
//--------------------------------------------------------------------------
void VeBlob::AddSize(size_t stSize) noexcept
{
	assert(m_bNeedFree && stSize);
	if (m_pvBuffer)
	{
		m_stByteSize += stSize;
		m_pvBuffer = VeRealloc(m_pvBuffer, m_stByteSize + 1);
	}
	else
	{
		m_stByteSize = stSize;
		m_pvBuffer = VeMalloc(m_stByteSize + 1);
	}
	((char*)m_pvBuffer)[m_stByteSize] = 0;
}
//--------------------------------------------------------------------------
