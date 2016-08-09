////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Stream
//  File name:   VeWriter.cpp
//  Created:     2016/08/05 by Albert
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
VeMemWriter::VeMemWriter() noexcept
{

}
//--------------------------------------------------------------------------
VeMemWriter::VeMemWriter(size_t stReserve) noexcept
{
	expand(stReserve);
}
//--------------------------------------------------------------------------
VeMemWriter::~VeMemWriter() noexcept
{
	if (m_pvBuffer)
	{
		VeFree(m_pvBuffer);
		m_pvBuffer = nullptr;
		m_stSize = 0;
	}
}
//--------------------------------------------------------------------------
void VeMemWriter::expand(size_t stSize) noexcept
{
	stSize = vtd::max((stSize + 0xF) & (~0xF), VE_WRITER_UNIT);
	if (m_pvBuffer)
	{
		m_stSize += stSize;
		m_pvBuffer = VeRealloc(m_pvBuffer, m_stSize);
	}
	else
	{
		m_stSize = stSize;
		m_pvBuffer = VeMalloc(m_stSize);
	}
}
//--------------------------------------------------------------------------
VeBlobWriter::VeBlobWriter() noexcept
{
	m_spBlob = VE_NEW VeBlob();
}
//--------------------------------------------------------------------------
VeBlobWriter::VeBlobWriter(const VeBlobPtr& spBlob) noexcept
{
	m_spBlob = spBlob;
}
//--------------------------------------------------------------------------
VeBlobWriter::~VeBlobWriter() noexcept
{

}
//--------------------------------------------------------------------------
