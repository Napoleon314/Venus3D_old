////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeAllocator.h
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

#pragma once

#define VE_STACK_ALLOCATOR_DEFAULT_SIZE (128 * 1024)

class VENUS_API VeStackAllocator : public VeMemObject
{
	VeNoCopy(VeStackAllocator);
public:
	VeStackAllocator(size_t stSize = VE_STACK_ALLOCATOR_DEFAULT_SIZE) noexcept;

	~VeStackAllocator() noexcept;

	inline size_t Space() noexcept;

	inline size_t RemainSpace() noexcept;

	void* Allocate(size_t stSizeInBytes) noexcept;

	void Deallocate() noexcept;

protected:
	const size_t m_stSize;
	uint8_t* m_pu8Buffer = nullptr;
	uint8_t* m_pu8Current = nullptr;
	vtd::stack<size_t> m_kStack;

};

#include "VeAllocator.inl"
