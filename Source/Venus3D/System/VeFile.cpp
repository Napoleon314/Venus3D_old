////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      System
//  File name:   VeFile.cpp
//  Created:     2016/07/30 by Albert
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

#include "stdafx.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeFile, VeArchive);
//--------------------------------------------------------------------------
VeFile::VeFile(FILE* hFile, uint32_t u32Flags) noexcept
{
	VE_ASSERT(hFile);
	m_hFile = hFile;
	m_u32Flags = u32Flags;
}
//--------------------------------------------------------------------------
VeFile::~VeFile() noexcept
{
	if (m_hFile)
	{
		fclose(m_hFile);
		m_hFile = nullptr;
	}
}
//--------------------------------------------------------------------------
bool VeFile::Seek(ptrdiff_t pdOffset, VeWhence eWhence) noexcept
{
	return _fseeki64(m_hFile, pdOffset, eWhence) == 0;
}
//--------------------------------------------------------------------------
size_t VeFile::Tell() noexcept
{
	return _ftelli64(m_hFile);
}
//--------------------------------------------------------------------------
size_t VeFile::Read(void* pvOutput, size_t stBytes) noexcept
{
	return fread(pvOutput, 1, stBytes, m_hFile);
}
//--------------------------------------------------------------------------
size_t VeFile::Write(void* pvOutput, size_t stBytes) noexcept
{
	return fwrite(pvOutput, 1, stBytes, m_hFile);
}
//--------------------------------------------------------------------------
VeRTTIImpl(VeFileDir, VeDirectory);
//--------------------------------------------------------------------------
VeFileDir::VeFileDir(const char* pcPath) noexcept
	: m_kFileDirPath(pcPath)
{

}
//--------------------------------------------------------------------------
VeFileDir::~VeFileDir() noexcept
{

}
//--------------------------------------------------------------------------
bool VeFileDir::Access(const char* pcPath, uint32_t u32Flags) noexcept
{
	char acPath[VE_MAX_PATH_LEN];
	VeSprintf(acPath, "%s/%s", m_kFileDirPath.c_str(), pcPath);
	return _access(acPath, u32Flags) == 0;
}
//--------------------------------------------------------------------------
struct FindData : VeDirectory::Child
{
	virtual VeDirectory::ChildData data() noexcept override
	{
		return{ m_kData.name, m_kData.size, m_kData.time_write, m_kData.attrib };
	}

	virtual bool next() noexcept
	{
		return _findnext(m_hFind, &m_kData) != EOF;
	}

	intptr_t m_hFind;
	_finddata_t m_kData;
};
//--------------------------------------------------------------------------
VeDirectory::ChildPtr VeFileDir::FindFirst(const char* pcDesc) noexcept
{
	vtd::intrusive_ptr<FindData> spData = VE_NEW FindData();
	if (pcDesc)
	{
		char acBuffer[VE_MAX_PATH_LEN];
		VeSprintf(acBuffer, "%s/%s", m_kFileDirPath.c_str(), pcDesc);
		spData->m_hFind = _findfirst(acBuffer, &(spData->m_kData));
	}
	else
	{
		spData->m_hFind = _findfirst(m_kFileDirPath, &(spData->m_kData));
	}
	if (spData->m_hFind != EOF)
	{
		return spData;
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeDirectoryPtr VeFileDir::OpenSubdir(const char* pcPath,
	bool bTryCreate) noexcept
{
	char acPath[VE_MAX_PATH_LEN];
	VeSprintf(acPath, "%s/%s", m_kFileDirPath.c_str(), pcPath);
	return Create(acPath, bTryCreate);
}
//--------------------------------------------------------------------------
VeArchivePtr VeFileDir::OpenArchive(const char* pcPath,
	uint32_t u32Flags) noexcept
{
	char acPath[VE_MAX_PATH_LEN];
	VeSprintf(acPath, "%s/%s", m_kFileDirPath.c_str(), pcPath);
	return Open(acPath, u32Flags);
}
//--------------------------------------------------------------------------
bool VeFileDir::TestPath(std::pair<uint32_t, time_t>& kOut,
	const char* pcPath) noexcept
{
	_finddata32i64_t kData;
	size_t stHandle = _findfirst32i64(pcPath, &kData);
	if (stHandle != VE_ELF)
	{

		kOut.first = kData.attrib & 0xf0;
		kOut.second = kData.time_write;
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------
bool VeFileDir::CreatePath(const char* pcPath) noexcept
{
	size_t stLen = strlen(pcPath);
	char acBuffer[VE_MAX_PATH_LEN];
	{
		bool bSingleRelative(true);
		const char* pcStart = pcPath;
		while (*pcStart)
		{
			if ((*pcStart) == '/' || (*pcStart) == '\\')
			{
				bSingleRelative = false;
				break;
			}
			else
			{
				++pcStart;
			}
		}
		if (bSingleRelative)
		{
			VeSprintf(acBuffer, "./%s", pcPath);
		}
		else
		{
			memcpy(acBuffer, pcPath, stLen + 1);
		}
	}
	char* pcTemp = acBuffer;
	if ((*pcTemp) == '/') ++pcTemp;
	char* pcEnd = acBuffer + stLen;
	bool bAllowCreate(false);
	while (pcTemp < pcEnd)
	{
		while (true)
		{
			if ((*pcTemp) == '\0')
			{
				break;
			}
			if ((*pcTemp) == '/' || (*pcTemp) == '\\')
			{
				(*pcTemp) = 0;
				break;
			}
			else
			{
				++pcTemp;
			}
		}

		if (VE_SUCCEEDED(_access(acBuffer, 2)))
		{
			bAllowCreate = true;
			if (pcTemp < pcEnd)
			{
				(*pcTemp) = '/';
				++pcTemp;
			}

		}
		else if (VE_SUCCEEDED(_access(acBuffer, 0)))
		{
			bAllowCreate = false;
			if (pcTemp < pcEnd)
			{
				(*pcTemp) = '/';
				++pcTemp;
			}
		}
		else if (bAllowCreate)
		{
			if (VE_SUCCEEDED(_mkdir(acBuffer)))
			{
				if (pcTemp < pcEnd)
				{
					(*pcTemp) = '/';
					++pcTemp;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}
//--------------------------------------------------------------------------
VeDirectoryPtr VeFileDir::Create(const char* pcPath,
	bool bTryCreate) noexcept
{
	std::pair<uint32_t, time_t> kInfo;
	if (TestPath(kInfo, pcPath))
	{
		if (VE_MASK_HAS_ALL(kInfo.first, VE_PATTR_SUBDIR))
		{
			return VE_NEW VeFileDir(pcPath);
		}
		else if (VE_MASK_HAS_ALL(kInfo.first, VE_PATTR_ARCH))
		{
			return nullptr;
		}
	}
	else if (bTryCreate && CreatePath(pcPath))
	{
		return VE_NEW VeFileDir(pcPath);
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeArchivePtr VeFileDir::Open(const char* pcPath, uint32_t u32Flags) noexcept
{
	FILE* hFile = nullptr;
	if (VE_MASK_HAS_ALL(u32Flags, VE_ARCH_OPEN_CREATE))
	{
		switch (u32Flags & VE_ARCH_OPEN_WMOD_MASK)
		{
		case VE_ARCH_OPEN_WRITE:
			hFile = fopen(pcPath, VE_MASK_HAS_ALL(u32Flags, VE_ARCH_OPEN_READ)
				? "wb+" : "wb");
			break;
		case VE_ARCH_OPEN_APPEND:
			hFile = fopen(pcPath, VE_MASK_HAS_ALL(u32Flags, VE_ARCH_OPEN_READ)
				? "ab+" : "ab");
			break;
		default:
			break;
		}
	}
	else if (VE_MASK_HAS_ALL(u32Flags, VE_ARCH_OPEN_READ))
	{
		hFile = fopen(pcPath, VE_MASK_HAS_ALL(u32Flags, VE_ARCH_OPEN_WRITE)
			? "rb+" : "rb");
	}
	if (hFile)
	{
		VE_MASK_CONDITION(u32Flags, VE_ARCH_OPEN_READ, VE_MASK_ADD(u32Flags, VE_ARCH_READ));
		VE_MASK_CONDITION(u32Flags, VE_ARCH_OPEN_WRITE, VE_MASK_ADD(u32Flags, VE_ARCH_WRITE));
		VE_MASK_CONDITION(u32Flags, VE_ARCH_OPEN_APPEND, VE_MASK_ADD(u32Flags, VE_ARCH_APPEND));
		u32Flags |= VE_ARCH_SEEK | VE_ARCH_SEEK_BACKWARD | VE_ARCH_FILE;
		return VE_NEW VeFile(hFile, u32Flags);
	}
	return nullptr;
}
//--------------------------------------------------------------------------
