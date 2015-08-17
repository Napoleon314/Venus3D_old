////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMemoryStream.h
//  Created:     2015/03/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeBlob : public VeRefObject
{
	VE_NO_COPY(VeBlob);
public:
	VeBlob() noexcept;

	VeBlob(VeSizeT stSize) noexcept;

	VeBlob(void* pvBuffer, VeSizeT stSize) noexcept;

	~VeBlob() noexcept;

	inline operator void*() noexcept;

	inline void* GetBuffer() noexcept;

	inline void* GetBuffer(VeSizeT stPos) noexcept;

	inline VeSizeT GetSize() noexcept;

	void AddSize(VeSizeT stSize) noexcept;

protected:
	void* m_pvBuffer = nullptr;
	VeSizeT m_stByteSize = 0;
	bool m_bNeedFree = true;

};

VeSmartPointer(VeBlob);

class VE_POWER_API VeMemoryOStream
	: public VeBinaryOStream, public VeBinaryIStream
{
	VE_NO_COPY(VeMemoryOStream);
	VeRTTIDecl(VeMemoryOStream, VeBinaryOStream, VeBinaryIStream);
public:
	VeMemoryOStream(VeSizeT stSize = 64) noexcept;

	VeMemoryOStream(const VeBlobPtr& spBlob) noexcept;

	VeMemoryOStream(VeMemoryOStream&& kMove) noexcept;

	virtual ~VeMemoryOStream() noexcept;

	VeMemoryOStream& operator=(VeMemoryOStream&& kMove) noexcept;

	inline void* GetData() noexcept;

	inline void* GetDataForRead() noexcept;

	inline VeSizeT AfterReadData(VeSizeT stBytes) noexcept;

	inline const VeBlobPtr& GetBlob() noexcept;

	inline void Reset() noexcept;

	inline void Rewind() noexcept;

	virtual VeSizeT Write(const void* pvBuffer, VeSizeT stBytes) noexcept;

	virtual VeSizeT Size() const noexcept;

	virtual VeSizeT Read(void* pvBuffer, VeSizeT stBytes) noexcept;

	virtual VeResult Seek(VePtrDiff pdOffset, VeWhence eWhence) noexcept;

	virtual VeSizeT Tell() noexcept;

	virtual void* Skip(VeSizeT stBytes) noexcept;

	virtual void Restart() noexcept;

	virtual VeSizeT RemainingLength() const noexcept;

	virtual VeChar8 Peek() noexcept;

protected:
	VeBlobPtr m_spBlob;
	VeSizeT m_stCurr = 0;
	VeSizeT m_stRead = 0;

};

VeSmartPointer(VeMemoryOStream);

class VE_POWER_API VeMemoryIStream : public VeBinaryIStream
{
	VE_NO_COPY(VeMemoryIStream);
	VeRTTIDecl(VeMemoryIStream, VeBinaryIStream);
public:
	VeMemoryIStream(const VeBlobPtr& spBlob) noexcept;

	virtual ~VeMemoryIStream() noexcept;

	inline const void* GetData() const noexcept;

	inline const VeBlobPtr& GetBlob() noexcept;

	inline void Reset() noexcept;

	virtual VeSizeT Read(void* pvBuffer, VeSizeT stBytes) noexcept;

	virtual VeResult Seek(VePtrDiff pdOffset, VeWhence eWhence) noexcept;

	virtual VeSizeT Tell() noexcept;

	virtual void* Skip(VeSizeT stBytes) noexcept;

	virtual void Restart() noexcept;

	virtual VeSizeT RemainingLength() const noexcept;

	virtual VeChar8 Peek() noexcept;

protected:
	VeBlobPtr m_spBlob;
	VeSizeT m_stCurr = 0;

};

VeSmartPointer(VeMemoryIStream);

#include "VeMemoryStream.inl"
