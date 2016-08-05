////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Stream
//  File name:   VeWriter.h
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

#pragma once

#define VE_WRITER_UNIT (1024)

class VENUS_API VeMemWriter : public VeStackObject
{
public:
	VeMemWriter() noexcept;

	VeMemWriter(size_t stReserve) noexcept;

	~VeMemWriter() noexcept;

	inline size_t write(void* pvBuffer, size_t stBytes) noexcept;

	inline size_t capacity() noexcept;

	inline void clear() noexcept;

	inline void* data() noexcept;

	inline size_t size() noexcept;

private:
	void expand(size_t stSize) noexcept;

	void* m_pvBuffer = nullptr;
	size_t m_stSize = 0;
	size_t m_stWritten = 0;

};

class VENUS_API VeBlobWriter : public VeStackObject
{
public:
	VeBlobWriter() noexcept;

	VeBlobWriter(const VeBlobPtr& spBlob) noexcept;

	~VeBlobWriter() noexcept;

	inline size_t write(void* pvBuffer, size_t stBytes) noexcept;

	inline size_t capacity() noexcept;

	inline void clear() noexcept;

	inline const VeBlobPtr& blob() noexcept;

private:
	VeBlobPtr m_spBlob;
	size_t m_stWritten = 0;

};

#include "VeWriter.inl"
