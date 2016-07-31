////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      System
//  File name:   VeFile.h
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

#pragma once

class VeFileDir;

class VENUS_API VeFile : public VeArchive
{
	VeNoCopy(VeFile);
	VeRTTIDecl(VeFile, VeArchive);
public:
	virtual ~VeFile() noexcept;

	virtual bool Seek(ptrdiff_t pdOffset, VeWhence eWhence) noexcept;

	virtual size_t Tell() noexcept;

	virtual size_t Read(void* pvOutput, size_t stBytes) noexcept;

	virtual size_t Write(void* pvOutput, size_t stBytes) noexcept;

	

private:
	friend class VeFileDir;
	VeFile(FILE* hFile, uint32_t u32Flags) noexcept;

	FILE* m_hFile = nullptr;

};

class VENUS_API VeFileDir : public VeDirectory
{
	VeNoCopy(VeFileDir);
	VeRTTIDecl(VeFileDir, VeDirectory);
public:
	virtual ~VeFileDir() noexcept;

	virtual bool Access(const char* pcPath, uint32_t u32Flags) noexcept;

	virtual ArchDataPtr FindFirst(const char* pcDesc = nullptr) noexcept;

	virtual VeDirectoryPtr OpenSubdir(const char* pcPath, bool bTryCreate) noexcept;

	virtual VeArchivePtr OpenArchive(const char* pcPath, uint32_t u32Flags) noexcept;

	static inline const char* Name() noexcept;

	static bool TestPath(std::pair<uint32_t, time_t>& kOut, const char* pcPath) noexcept;

	static bool CreatePath(const char* pcPath) noexcept;

	static VeDirectoryPtr Create(const char* pcPath = "", bool bTryCreate = true) noexcept;

	static VeArchivePtr Open(const char* pcPath, uint32_t u32Flags) noexcept;

private:
	VeFileDir(const char* pcPath) noexcept;

	vtd::string m_kFileDirPath;

};

#include "VeFile.inl"
