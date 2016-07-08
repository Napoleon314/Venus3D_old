////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeBlob.h
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

#pragma once

class VENUS_API VeBlob : public VeRefObject
{
	VeNoCopy(VeBlob);
public:
	VeBlob() noexcept;

	VeBlob(size_t stSize) noexcept;

	VeBlob(void* pvBuffer, size_t stSize) noexcept;

	~VeBlob() noexcept;

	inline operator void*() noexcept;

	inline operator const void*() const noexcept;

	inline void* GetBuffer() noexcept;

	inline void* GetBuffer(size_t stPos) noexcept;

	inline size_t GetSize() const noexcept;

	void AddSize(size_t stSize) noexcept;

protected:
	void* m_pvBuffer = nullptr;
	size_t m_stByteSize = 0;
	bool m_bNeedFree = true;

};

VeSmartPointer(VeBlob);

#include "VeBlob.inl"
