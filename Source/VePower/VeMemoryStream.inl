////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMemoryStream.inl
//  Created:     2015/03/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeBlob::operator void*() noexcept
{
	return m_pvBuffer;
}
//--------------------------------------------------------------------------
inline VeBlob::operator const void*() const noexcept
{
	return m_pvBuffer;
}
//--------------------------------------------------------------------------
inline void* VeBlob::GetBuffer() noexcept
{
	return m_pvBuffer;
}
//--------------------------------------------------------------------------
inline void* VeBlob::GetBuffer(VeSizeT stPos) noexcept
{
	VE_ASSERT(stPos < m_stByteSize);
	return ((VeByte*)m_pvBuffer) +stPos;
}
//--------------------------------------------------------------------------
inline VeSizeT VeBlob::GetSize() const noexcept
{
	return m_stByteSize;
}
//--------------------------------------------------------------------------
inline void* VeMemoryOStream::GetData() noexcept
{
	return m_spBlob->GetBuffer();
}
//--------------------------------------------------------------------------
inline void* VeMemoryOStream::GetDataForRead() noexcept
{
	return m_spBlob->GetBuffer(m_stRead);
}
//--------------------------------------------------------------------------
inline VeSizeT VeMemoryOStream::AfterReadData(VeSizeT stBytes) noexcept
{
	stBytes = VE_MIN(RemainingLength(), stBytes);
	m_stRead += stBytes;
	return stBytes;
}
//--------------------------------------------------------------------------
inline const VeBlobPtr& VeMemoryOStream::GetBlob() noexcept
{
	return m_spBlob;
}
//--------------------------------------------------------------------------
inline void VeMemoryOStream::Reset() noexcept
{
	m_stCurr = m_stRead = 0;
}
//--------------------------------------------------------------------------
inline void VeMemoryOStream::Rewind() noexcept
{
	m_stRead = 0;
}
//--------------------------------------------------------------------------
const void* VeMemoryIStream::GetData() const noexcept
{
	return m_spBlob->GetBuffer(m_stCurr);
}
//--------------------------------------------------------------------------
const VeBlobPtr& VeMemoryIStream::GetBlob() noexcept
{
	return m_spBlob;
}
//--------------------------------------------------------------------------
void VeMemoryIStream::Reset() noexcept
{
	m_stCurr = 0;
}
//--------------------------------------------------------------------------
