////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Stream
//  File name:   VeReader.inl
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

//--------------------------------------------------------------------------
inline size_t VeMemReader::read(void* pvBuffer, size_t stBytes) noexcept
{
	stBytes = vtd::min(remaining(), stBytes);
	memcpy(pvBuffer, (char*)m_pvData + m_stPointer, stBytes);
	m_stPointer += stBytes;
	return stBytes;
}
//--------------------------------------------------------------------------
inline void VeMemReader::skip(size_t stBytes) noexcept
{
	m_stPointer = std::min(m_stPointer + stBytes, m_stSize);
}
//--------------------------------------------------------------------------
inline size_t VeMemReader::remaining() noexcept
{
	VE_ASSERT(m_stPointer <= m_stSize);
	return m_stSize - m_stPointer;
}
//--------------------------------------------------------------------------
inline void VeMemReader::restart() noexcept
{
	m_stPointer = 0;
}
//--------------------------------------------------------------------------
inline size_t VeBlobReader::read(void* pvBuffer, size_t stBytes) noexcept
{
	VE_ASSERT(m_spBlob);
	stBytes = vtd::min(remaining(), stBytes);
	memcpy(pvBuffer, (char*)m_spBlob->data() + m_stPointer, stBytes);
	m_stPointer += stBytes;
	return stBytes;
}
//--------------------------------------------------------------------------
inline void VeBlobReader::skip(size_t stBytes) noexcept
{
	VE_ASSERT(m_spBlob);
	m_stPointer = std::min(m_stPointer + stBytes, m_spBlob->size());
}
//--------------------------------------------------------------------------
inline size_t VeBlobReader::remaining() noexcept
{
	VE_ASSERT(m_spBlob && m_stPointer <= m_spBlob->size());
	return m_spBlob->size() - m_stPointer;
}
//--------------------------------------------------------------------------
inline void VeBlobReader::restart() noexcept
{
	m_stPointer = 0;
}
//--------------------------------------------------------------------------
