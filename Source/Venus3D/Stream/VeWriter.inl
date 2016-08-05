////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Stream
//  File name:   VeWriter.inl
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
inline size_t VeMemWriter::write(void* pvBuffer, size_t stBytes) noexcept
{
	if (stBytes > capacity())
	{
		expand(m_stSize);
	}
	memcpy((char*)m_pvBuffer + m_stWritten, pvBuffer, stBytes);
	m_stWritten += stBytes;
	return stBytes;
}
//--------------------------------------------------------------------------
inline size_t VeMemWriter::capacity() noexcept
{
	VE_ASSERT(m_stSize >= m_stWritten);
	return m_stSize - m_stWritten;
}
//--------------------------------------------------------------------------
inline void VeMemWriter::clear() noexcept
{
	m_stWritten = 0;
}
//--------------------------------------------------------------------------
inline void* VeMemWriter::data() noexcept
{
	return m_pvBuffer;
}
//--------------------------------------------------------------------------
inline size_t VeMemWriter::size() noexcept
{
	return m_stWritten;
}
//--------------------------------------------------------------------------
inline size_t VeBlobWriter::write(void* pvBuffer, size_t stBytes) noexcept
{
	VE_ASSERT(m_spBlob);
	if (stBytes > capacity())
	{
		m_spBlob->expand(vtd::max(m_spBlob->size(), VE_WRITER_UNIT));
	}
	memcpy((char*)m_spBlob->data() + m_stWritten, pvBuffer, stBytes);
	m_stWritten += stBytes;
	return stBytes;
}
//--------------------------------------------------------------------------
inline size_t VeBlobWriter::capacity() noexcept
{
	VE_ASSERT(m_spBlob->size() >= m_stWritten);
	return m_spBlob->size() - m_stWritten;
}
//--------------------------------------------------------------------------
inline void VeBlobWriter::clear() noexcept
{
	m_stWritten = 0;
}
//--------------------------------------------------------------------------
inline const VeBlobPtr& VeBlobWriter::blob() noexcept
{
	return m_spBlob;
}
//--------------------------------------------------------------------------
