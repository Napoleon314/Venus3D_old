////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Archive
//  File name:   VeArchive.h
//  Created:     2016/07/29 by Albert
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

enum VeWhence
{
	VE_SEEK_SET				= 0,
	VE_SEEK_CUR				= 1,
	VE_SEEK_END				= 2
};

enum VeAccessFlag
{
	VE_ACCESS_E_OK			= 0x0,
	VE_ACCESS_W_OK			= 0x2,
	VE_ACCESS_R_OK			= 0x4
};

enum VePathAttrFlag
{
	VE_PATTR_NORMAL			= 0x00,
	VE_PATTR_RDONLY			= 0x01,
	VE_PATTR_HIDDEN			= 0x02,
	VE_PATTR_SUBDIR			= 0x10,
	VE_PATTR_ARCH			= 0x20
};

enum VeArchOpenFlag
{
	VE_ARCH_OPEN_CREATE		= 0x01000000,
	VE_ARCH_OPEN_READ		= 0x02000000,
	VE_ARCH_OPEN_WRITE		= 0x10000000,
	VE_ARCH_OPEN_APPEND		= 0x20000000,
	VE_ARCH_OPEN_WMOD_MASK	= 0xF0000000,
	VE_ARCH_OPEN_MASK		= 0xFF000000
};

enum VeArchPermission
{
	VE_ARCH_READ			= 0x010000,
	VE_ARCH_WRITE			= 0x020000,
	VE_ARCH_APPEND			= 0x040000,
	VE_ARCH_SEEK			= 0x080000,
	VE_ARCH_SEEK_BACKWARD	= 0x100000,
	VE_ARCH_PERM_MASK		= 0xFF0000
};

VeSmartPointer(VeArchive);

class VENUS_API VeArchive : public VeRefObject
{
	VeNoCopy(VeArchive);
	VeRTTIDecl(VeArchive);
public:
	VeArchive() noexcept = default;

	virtual ~VeArchive() noexcept = default;

	virtual bool Seek(ptrdiff_t pdOffset, VeWhence eWhence) noexcept = 0;

	virtual size_t Tell() noexcept = 0;

	virtual size_t Read(void* pvOutput, size_t stBytes) noexcept = 0;

	virtual size_t Write(void* pvOutput, size_t stBytes) noexcept = 0;

	inline uint32_t GetFlags() noexcept;

	inline bool TestFlags(uint32_t u32Flags) noexcept;

protected:
	uint32_t m_u32Flags = 0;

};

VeSmartPointer(VeDirectory);

class VENUS_API VeDirectory : public VeRefObject
{
	VeNoCopy(VeDirectory);
	VeRTTIDecl(VeDirectory);
public:
	struct ArchData : public VeRefObject
	{
		virtual ~ArchData() noexcept = default;

		virtual ArchData* Next() noexcept = 0;

		const char* m_pcName;
		size_t m_stSize;
		time_t m_ttLastWrite;
	};

	typedef vtd::intrusive_ptr<ArchData> ArchDataPtr;

	class iterator
	{
	public:
		iterator(const ArchDataPtr& _Data) noexcept
			: data(_Data) {}

		ArchData& operator*() noexcept
		{
			return *data;
		}

		iterator& operator++() noexcept
		{
			if (data)
			{
				data = data->Next();
			}
			return *this;
		}

	private:
		ArchDataPtr data = nullptr;

	};

	VeDirectory() noexcept = default;

	virtual ~VeDirectory() noexcept = default;

	virtual bool Access(const char* pcPath, uint32_t u32Flags) noexcept = 0;

	virtual ArchDataPtr FindFirst(const char* pcDesc = nullptr) noexcept = 0;

	virtual VeDirectoryPtr OpenSubdir(const char* pcPath, bool bTryCreate = true) noexcept = 0;

	virtual VeArchivePtr OpenArchive(const char* pcPath, uint32_t u32Flags) noexcept = 0;
	
	inline iterator begin() noexcept;

	inline iterator end() noexcept;

};

#include "VeArchive.inl"
