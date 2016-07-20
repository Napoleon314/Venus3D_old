////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeAllocator.cpp
//  Created:     2016/07/20 by Albert
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
VeStackAllocator::VeStackAllocator(size_t stSize) noexcept
	: m_stSize(stSize)
{
	m_pu8Buffer = (uint8_t*)VeAlignedMalloc(stSize, 16);
	m_pu8Current = m_pu8Buffer;
}
//--------------------------------------------------------------------------
VeStackAllocator::~VeStackAllocator() noexcept
{
	assert(m_pu8Buffer == m_pu8Current && m_kStack.empty());
	VeAlignedFree(m_pu8Buffer);
	m_pu8Buffer = nullptr;
	m_pu8Current = nullptr;
}
//--------------------------------------------------------------------------
void* VeStackAllocator::Allocate(size_t stSizeInBytes) noexcept
{
	stSizeInBytes = (stSizeInBytes + 0xF) & 0xFFFFFFF0;
	assert(m_pu8Current - m_pu8Buffer <= ptrdiff_t(stSizeInBytes));
	void* pvRes = m_pu8Current;
	m_pu8Current += stSizeInBytes;
	m_kStack.push(stSizeInBytes);
	return pvRes;
}
//--------------------------------------------------------------------------
void VeStackAllocator::Deallocate() noexcept
{
	m_pu8Current -= m_kStack.pop();
}
//--------------------------------------------------------------------------
