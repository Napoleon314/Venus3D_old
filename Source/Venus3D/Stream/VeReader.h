////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Stream
//  File name:   VeReader.h
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

class VeMemReader : public VeStackObject
{
public:
	VeMemReader(void* pvData, size_t stSize) noexcept;

	~VeMemReader() noexcept;

	inline size_t read(void* pvBuffer, size_t stBytes) noexcept;

	inline void skip(size_t stBytes) noexcept;

	inline size_t remaining() noexcept;

	inline void restart() noexcept;

	template <class _Ty>
	inline bool peek(_Ty& o) noexcept
	{
		if (remaining() < sizeof(_Ty)) return false;
		o = *(_Ty*)((char*)m_pvData + m_stPointer);
		return true;
	}

protected:
	void* m_pvData = nullptr;
	size_t m_stSize = 0;
	size_t m_stPointer = 0;

};

class VeBlobReader : public VeStackObject
{
public:
	VeBlobReader(const VeBlobPtr& spBlob) noexcept;

	~VeBlobReader() noexcept;

	inline size_t read(void* pvBuffer, size_t stBytes) noexcept;

	inline void skip(size_t stBytes) noexcept;

	inline size_t remaining() noexcept;

	inline void restart() noexcept;

protected:
	VeBlobPtr m_spBlob;
	size_t m_stPointer = 0;

};

#include "VeReader.inl"
