////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMemoryStream.cpp
//  Created:     2015/03/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeBlob::VeBlob() noexcept
{

}
//--------------------------------------------------------------------------
VeBlob::VeBlob(VeSizeT stSize) noexcept
{
	if (stSize)
	{
		m_pvBuffer = VeMalloc(stSize + 1);
		((VeChar8*)m_pvBuffer)[stSize] = 0;
		m_stByteSize = stSize;
	}
}
//--------------------------------------------------------------------------
VeBlob::VeBlob(void* pvBuffer, VeSizeT stSize) noexcept
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
void VeBlob::AddSize(VeSizeT stSize) noexcept
{
	VE_ASSERT(m_bNeedFree && stSize);
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
	((VeChar8*)m_pvBuffer)[m_stByteSize] = 0;
}
//--------------------------------------------------------------------------
VeRTTIImpl(VeMemoryOStream, VeBinaryOStream, VeBinaryIStream);
//--------------------------------------------------------------------------
VeMemoryOStream::VeMemoryOStream(VeSizeT stSize) noexcept
{
	m_spBlob = VE_NEW VeBlob(stSize);
}
//--------------------------------------------------------------------------
VeMemoryOStream::VeMemoryOStream(const VeBlobPtr& spBlob) noexcept
{
	m_spBlob = spBlob;
	m_stCurr = spBlob->GetSize();
	m_stRead = 0;
}
//--------------------------------------------------------------------------
VeMemoryOStream::VeMemoryOStream(VeMemoryOStream&& kMove) noexcept
{
	m_spBlob = kMove.m_spBlob;
	m_stCurr = kMove.m_stCurr;
	m_stRead = kMove.m_stRead;
	kMove.m_spBlob = nullptr;
	kMove.m_stCurr = 0;
	kMove.m_stRead = 0;
}
//--------------------------------------------------------------------------
VeMemoryOStream::~VeMemoryOStream() noexcept
{

}
//--------------------------------------------------------------------------
VeMemoryOStream& VeMemoryOStream::operator=(VeMemoryOStream&& kMove) noexcept
{
	m_spBlob = kMove.m_spBlob;
	m_stCurr = kMove.m_stCurr;
	m_stRead = kMove.m_stRead;
	kMove.m_spBlob = nullptr;
	kMove.m_stCurr = 0;
	kMove.m_stRead = 0;
	return *this;
}
//--------------------------------------------------------------------------
VeSizeT VeMemoryOStream::Write(const void* pvBuffer,
	VeSizeT stBytes) noexcept
{
	if (stBytes)
	{
		VeSizeT stNewCurr = m_stCurr + stBytes;
		if (stNewCurr > m_spBlob->GetSize())
		{
			VeSizeT stAddSize = (stNewCurr / m_spBlob->GetSize()) *
				m_spBlob->GetSize();
			m_spBlob->AddSize(stAddSize);
		}
		VeMemoryCopy(m_spBlob->GetBuffer(m_stCurr), pvBuffer, stBytes);
		m_stCurr = stNewCurr;
		return stBytes;
	}
	else
	{
		return 0;
	}
}
//--------------------------------------------------------------------------
VeSizeT VeMemoryOStream::Size() const noexcept
{
	return m_stCurr;
}
//--------------------------------------------------------------------------
VeSizeT VeMemoryOStream::Read(void* pvBuffer, VeSizeT stBytes) noexcept
{
	if (stBytes)
	{
		VeSizeT stReadBytes = VE_MIN(RemainingLength(), stBytes);
		if (stReadBytes)
		{
			VeMemoryCopy(pvBuffer, GetDataForRead(), stBytes);
			m_stRead += stReadBytes;
			return stReadBytes;
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
VeResult VeMemoryOStream::Seek(VePtrDiff pdOffset,
	VeWhence eWhence) noexcept
{
	switch (eWhence)
	{
	case VE_SEEK_SET:
		if (pdOffset >= 0 && pdOffset <= VePtrDiff(m_stCurr))
		{
			m_stRead = pdOffset;
			return VE_S_OK;
		}
		break;
	case VE_SEEK_CUR:
		{
			VePtrDiff pdAfter = VePtrDiff(m_stRead) + pdOffset;
			if (pdAfter >= 0 && pdAfter <= VePtrDiff(m_stCurr))
			{
				m_stRead = pdAfter;
				return VE_S_OK;
			}
		}
		break;
	case VE_SEEK_END:
		if (pdOffset >= 0 && pdOffset <= VePtrDiff(m_stCurr))
		{
			m_stRead = (m_stCurr - pdOffset);
			return VE_S_OK;
		}
		break;
	default:
		break;
	}
	return VE_E_FAIL;
}
//--------------------------------------------------------------------------
VeSizeT VeMemoryOStream::Tell() noexcept
{
	return m_stRead;
}
//--------------------------------------------------------------------------
void* VeMemoryOStream::Skip(VeSizeT stBytes) noexcept
{
	if (stBytes)
	{
		VeSizeT stReadBytes = VE_MIN(RemainingLength(), stBytes);
		if (stReadBytes)
		{
			void* pvBuffer = GetDataForRead();
			m_stRead += stReadBytes;
			return pvBuffer;
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------
void VeMemoryOStream::Restart() noexcept
{
	Rewind();
}
//--------------------------------------------------------------------------
VeSizeT VeMemoryOStream::RemainingLength() const noexcept
{
	return VePtrDiff(m_stCurr) - VePtrDiff(m_stRead);
}
//--------------------------------------------------------------------------
VeChar8 VeMemoryOStream::Peek() noexcept
{
	if (m_stRead >= m_stCurr)
	{
		m_bError = true;
		return -1;
	}
	return *(VeChar8*)m_spBlob->GetBuffer(m_stRead);
}
//--------------------------------------------------------------------------
VeRTTIImpl(VeMemoryIStream, VeBinaryIStream);
//--------------------------------------------------------------------------
VeMemoryIStream::VeMemoryIStream(const VeBlobPtr& spBlob) noexcept
	: m_spBlob(spBlob)
{

}
//--------------------------------------------------------------------------
VeMemoryIStream::~VeMemoryIStream() noexcept
{

}
//--------------------------------------------------------------------------
VeSizeT VeMemoryIStream::Read(void* pvBuffer, VeSizeT stBytes) noexcept
{
	if (stBytes)
	{
		VE_ASSERT(m_spBlob->GetSize() >= m_stCurr);
		VeSizeT stReadBytes =
			VE_MIN(m_spBlob->GetSize() - m_stCurr, stBytes);
		if (stReadBytes)
		{
			VeMemoryCopy(pvBuffer, m_spBlob->GetBuffer(m_stCurr), stReadBytes);
			m_stCurr += stReadBytes;
			return stReadBytes;
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
VeResult VeMemoryIStream::Seek(VePtrDiff pdOffset,
	VeWhence eWhence) noexcept
{
	switch (eWhence)
	{
	case VE_SEEK_SET:
		if (pdOffset >= 0 && pdOffset <= VePtrDiff(m_spBlob->GetSize()))
		{
			m_stCurr = pdOffset;
			return VE_S_OK;
		}
		break;
	case VE_SEEK_CUR:
		{
			VePtrDiff pdAfter = VePtrDiff(m_stCurr) + pdOffset;
			if (pdAfter >= 0 && pdAfter <= VePtrDiff(m_spBlob->GetSize()))
			{
				m_stCurr = VeSizeT(pdAfter);
				return VE_S_OK;
			}
		}
		break;
	case VE_SEEK_END:
		if (pdOffset >= 0 && pdOffset <= VePtrDiff(m_spBlob->GetSize()))
		{
			m_stCurr = m_spBlob->GetSize() - pdOffset;
			return VE_S_OK;
		}
		break;
	default:
		break;
	}
	return VE_E_FAIL;
}
//--------------------------------------------------------------------------
VeSizeT VeMemoryIStream::Tell() noexcept
{
	return m_stCurr;
}
//--------------------------------------------------------------------------
void* VeMemoryIStream::Skip(VeSizeT stBytes) noexcept
{
	if (stBytes)
	{
		VE_ASSERT(m_spBlob->GetSize() >= m_stCurr);
		VeSizeT stReadBytes =
			VE_MIN(m_spBlob->GetSize() - m_stCurr, stBytes);
		if (stReadBytes)
		{
			void* pvBuffer = m_spBlob->GetBuffer(m_stCurr);
			m_stCurr += stReadBytes;
			return pvBuffer;
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------
void VeMemoryIStream::Restart() noexcept
{
	Reset();
}
//--------------------------------------------------------------------------
VeSizeT VeMemoryIStream::RemainingLength() const noexcept
{
	VE_ASSERT(m_spBlob->GetSize() >= m_stCurr);
	return m_spBlob->GetSize() - m_stCurr;
}
//--------------------------------------------------------------------------
VeChar8 VeMemoryIStream::Peek() noexcept
{
	if (m_stCurr >= m_spBlob->GetSize())
	{
		m_bError = true;
		return -1;
	}
	return *(VeChar8*)m_spBlob->GetBuffer(m_stCurr);
}
//--------------------------------------------------------------------------
