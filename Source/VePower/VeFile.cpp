////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeFile.cpp
//  Created:     2015/03/10 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeFileIStream, VeBinaryIStream);
//--------------------------------------------------------------------------
VeFileIStream::VeFileIStream(const VeChar8* pcFileName,
	VeSizeT stCache) noexcept : m_stCacheSize(stCache)
{
	if (VE_SUCCEEDED(VeAccess(pcFileName, VeDirectory::ACCESS_F_OK)))
	{
#		ifdef VE_PLATFORM_WIN
		fopen_s(&m_pkFile, pcFileName, "rb");
#		else
		m_pkFile = fopen(pcFileName, "rb");
#		endif
		if (m_pkFile)
		{
			UpdateLenth();
			VE_ASSERT(m_stCacheSize);
			m_pbyCache = VeAlloc(VeByte, m_stCacheSize);
			VE_ASSERT(m_pbyCache);
			return;
		}
	}
	SetError(true);
}
//--------------------------------------------------------------------------
VeFileIStream::~VeFileIStream() noexcept
{
	Finish();
}
//--------------------------------------------------------------------------
VeSizeT VeFileIStream::Read(void* pvBuffer, VeSizeT stBytes) noexcept
{
	if (stBytes && pvBuffer)
	{
		VeSizeT stCached = Cached();
		VeSizeT stRes(0);
		{
			VeSizeT stSpace = m_stPointer < m_stFileSize ?
				m_stFileSize - m_stPointer : 0;
			stBytes = (stSpace >= stBytes) ? stBytes : stSpace;
		}
		while (stBytes)
		{
			if (!stCached)
			{
				CacheReload();
				stCached = m_stCacheUse;
			}
			if (stBytes <= stCached)
			{
				VeCrazyCopy((VeChar8*)pvBuffer + stRes,
					(VeChar8*)m_pbyCache + m_stPointer - m_stCachePointer,
					stBytes);
				stRes += stBytes;
				m_stPointer += stBytes;
				stBytes = 0;
				break;
			}
			else
			{
				VeCrazyCopy((VeChar8*)pvBuffer + stRes,
					(VeChar8*)m_pbyCache + m_stPointer - m_stCachePointer,
					stCached);
				stRes += stCached;
				m_stPointer += stCached;
				stBytes -= stCached;
				stCached = 0;
			}
		}
		return stRes;
	}
	else
	{
		return 0;
	}
}
//--------------------------------------------------------------------------
VeResult VeFileIStream::Seek(VePtrDiff pdOffset, VeWhence eWhence) noexcept
{
	switch (eWhence)
	{
	case VE_SEEK_SET:
		if (pdOffset >= 0 && pdOffset <= VePtrDiff(m_stFileSize))
		{
			m_stPointer = pdOffset;
			return VE_S_OK;
		}
		break;
	case VE_SEEK_CUR:
	{
		VePtrDiff pdAfter = VePtrDiff(m_stPointer) + pdOffset;
		if (pdAfter >= 0 && pdAfter <= VePtrDiff(m_stFileSize))
		{
			m_stPointer = pdAfter;
			return VE_S_OK;
		}
	}
	break;
	case VE_SEEK_END:
		if (pdOffset >= 0 && pdOffset <= VePtrDiff(m_stFileSize))
		{
			m_stPointer = m_stFileSize - pdOffset;
			return VE_S_OK;
		}
		break;
	default:
		break;
	}
	return VE_E_FAIL;
}
//--------------------------------------------------------------------------
VeSizeT VeFileIStream::Tell() noexcept
{
	return m_stPointer;
}
//--------------------------------------------------------------------------
void* VeFileIStream::Skip(VeSizeT stBytes) noexcept
{
	VeSizeT stSkip = VE_MIN(stBytes, RemainingLength());
	m_stPointer += stSkip;
	return nullptr;
}
//--------------------------------------------------------------------------
void VeFileIStream::Restart() noexcept
{
	m_stPointer = 0;
}
//--------------------------------------------------------------------------
VeSizeT VeFileIStream::RemainingLength() const noexcept
{
	return m_stFileSize - m_stPointer;
}
//--------------------------------------------------------------------------
VeChar8 VeFileIStream::Peek() noexcept
{
	if (m_stPointer >= m_stFileSize)
	{
		m_bError = true;
		return -1;
	}
	if (!Cached())
	{
		CacheReload();
	}
	return *((VeChar8*)m_pbyCache + m_stPointer - m_stCachePointer);
}
//--------------------------------------------------------------------------
void VeFileIStream::Finish() noexcept
{
	m_stPointer = m_stFileSize;
	if (m_pkFile)
	{
		fclose(m_pkFile);
		m_pkFile = nullptr;
	}
	if (m_pbyCache)
	{
		VeFree(m_pbyCache);
		m_pbyCache = nullptr;
	}
}
//--------------------------------------------------------------------------
void VeFileIStream::CacheReload() noexcept
{
	fseek(m_pkFile, long(m_stPointer), SEEK_SET);
	m_stCacheUse = fread(m_pbyCache, 1, m_stCacheSize, m_pkFile);
	m_stCachePointer = m_stPointer;
}
//--------------------------------------------------------------------------
void VeFileIStream::UpdateLenth() noexcept
{
	fseek(m_pkFile, 0, SEEK_END);
	m_stFileSize = ftell(m_pkFile);
}
//--------------------------------------------------------------------------
bool VeFileIStream::IsCached() noexcept
{
	return (m_stPointer >= m_stCachePointer) &&
		(m_stPointer < (m_stCachePointer + m_stCacheUse));
}
//--------------------------------------------------------------------------
VeSizeT VeFileIStream::Cached() noexcept
{
	return IsCached() ? m_stCacheUse + m_stCachePointer - m_stPointer : 0;
}
//--------------------------------------------------------------------------
VeRTTIImpl(VeFileOStream, VeBinaryOStream);
//--------------------------------------------------------------------------
VeFileOStream::VeFileOStream(const VeChar8* pcFileName, VeSizeT stCache,
	bool bAppend) noexcept : m_stCacheSize(stCache)
{
	const VeChar8* pcMode = bAppend ? "ab" : "wb";
#	ifdef VE_PLATFORM_WIN
	fopen_s(&m_pkFile, pcFileName, pcMode);
#	else
	m_pkFile = fopen(pcFileName, pcMode);
#	endif
	VE_ASSERT(m_pkFile && m_stCacheSize);
	m_pbyCache = VeAlloc(VeByte, m_stCacheSize);
	VE_ASSERT(m_pbyCache);
}
//--------------------------------------------------------------------------
VeFileOStream::~VeFileOStream() noexcept
{
	if (m_pkFile)
	{
		WriteBackCache();
		fclose(m_pkFile);
		m_pkFile = nullptr;
	}
	if (m_pbyCache)
	{
		VeFree(m_pbyCache);
		m_pbyCache = nullptr;
	}
}
//--------------------------------------------------------------------------
VeSizeT VeFileOStream::Write(const void* pvBuffer,
	VeSizeT stBytes) noexcept
{
	if (pvBuffer && stBytes)
	{
		const VeSizeT stRes = stBytes;
		const VeChar8* pcBuffer = (const VeChar8*)pvBuffer;
		if (HasCacheUse())
		{
			VeSizeT stCache = VE_MIN(CacheCanUse(), stBytes);
			VeCrazyCopy(Cache(), pcBuffer, stCache);
			pcBuffer += stCache;
			m_stFileSize += stCache;
			stBytes -= stCache;
			WriteBackCache();
		}
		if (stBytes >= m_stCacheSize)
		{
			fwrite(pcBuffer, 1, stBytes, m_pkFile);
			m_stFileSize += stBytes;
			m_stCachePointer = m_stFileSize;
		}
		else
		{
			VeCrazyCopy(Cache(), pcBuffer, stBytes);
			m_stFileSize += stBytes;
		}
		return stRes;
	}
	else
	{
		return 0;
	}
}
//--------------------------------------------------------------------------
VeSizeT VeFileOStream::Size() const noexcept
{
	return m_stFileSize;
}
//--------------------------------------------------------------------------
bool VeFileOStream::HasCacheUse() noexcept
{
	VE_ASSERT(m_stFileSize >= m_stCachePointer);
	return m_stFileSize > m_stCachePointer;
}
//--------------------------------------------------------------------------
VeSizeT VeFileOStream::CacheUse() noexcept
{
	return m_stFileSize - m_stCachePointer;
}
//--------------------------------------------------------------------------
VeSizeT VeFileOStream::CacheCanUse() noexcept
{
	return m_stCacheSize - CacheUse();
}
//--------------------------------------------------------------------------
VeByte* VeFileOStream::Cache() noexcept
{
	return m_pbyCache + CacheUse();
}
//--------------------------------------------------------------------------
void VeFileOStream::WriteBackCache() noexcept
{
	fwrite(m_pbyCache, 1, CacheUse(), m_pkFile);
	m_stCachePointer = m_stFileSize;
}
//--------------------------------------------------------------------------
VeRTTIImpl(VeFilePath, VeDirectory);
//--------------------------------------------------------------------------
VeFilePath::VeFilePath(const VeChar8* pcPath) noexcept
	: m_kPath(pcPath)
{

}
//--------------------------------------------------------------------------
VeFilePath::~VeFilePath() noexcept
{

}
//--------------------------------------------------------------------------
const VeChar8* VeFilePath::GetType() noexcept
{
	return GetTypeName();
}
//--------------------------------------------------------------------------
const VeChar8* VeFilePath::GetName() noexcept
{
	return m_kPath;
}
//--------------------------------------------------------------------------
bool VeFilePath::Access(VeUInt32 u32Flag) const noexcept
{
	const VeChar8* pcPath = m_kPath.GetLength() ? (const VeChar8*)m_kPath : ".";
	return VE_SUCCEEDED(VeAccess(pcPath, u32Flag));
}
//--------------------------------------------------------------------------
#define PATH_CAT(buf,c,n)									\
	if(c.GetLength()) VeSprintf(buf,"%s/%s",c.GetStr(),n);	\
	else VeStrcpy(buf,n)
//--------------------------------------------------------------------------
bool VeFilePath::HasFile(const VeChar8* pcFile) const noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	PATH_CAT(acBuffer, m_kPath, pcFile);
	PathInfo kInfo;
	if (TestPath(acBuffer, kInfo))
	{
		return VE_MASK_HAS_ALL(kInfo.m_stAttrib, VE_A_ARCH);
	}
	return false;
}
//--------------------------------------------------------------------------
bool VeFilePath::HasSubDir(const VeChar8* pcDir) const noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	PATH_CAT(acBuffer, m_kPath, pcDir);
	PathInfo kInfo;
	if (TestPath(acBuffer, kInfo))
	{
		return VE_MASK_HAS_ALL(kInfo.m_stAttrib, VE_A_SUBDIR);
	}
	return false;
}
//--------------------------------------------------------------------------
void VeFilePath::FindFileList(const VeChar8* pcDesc,
	VeVector<VeFixedString>& kOut) const noexcept
{
#	ifdef VE_PLATFORM_WIN
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	PATH_CAT(acBuffer, m_kPath, pcDesc);
	_finddata_t kData;
	VeSizeT stHandle = _findfirst(acBuffer, &kData);
	if (stHandle != VE_ELF)
	{
		do
		{
			if ((kData.attrib & 0xf0) == _A_ARCH)
			{
				kOut.push_back(kData.name);
			}
		} while (VE_SUCCEEDED(_findnext(stHandle, &kData)));
		VE_ASSERT_EQ(_findclose(stHandle), VE_S_OK);
	}
#	else
	const VeChar8* pcFile = VeStrrchr(pcDesc, '/');
	pcFile = pcFile ? (pcFile + 1) : pcDesc;
	DIR* pkDir(nullptr);
	if (pcFile == pcDesc)
	{
		pkDir = opendir(m_kPath.GetLength() ? m_kPath : ".");
	}
	else
	{
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeChar8* pcTemp = acBuffer;
		if (m_kPath.GetLength())
		{
			pcTemp += VeSprintf(acBuffer, "%s/", m_kPath.GetStr());
		}
		VeStrncpy(pcTemp, VE_MAX_PATH_LEN, pcDesc, pcFile - pcDesc - 1);
		pkDir = opendir(acBuffer);
	}
	if (pkDir)
	{
		struct dirent* pkEntry;
		while ((pkEntry = readdir(pkDir)))
		{
			if (pkEntry->d_type != DT_DIR)
			{
				if (VeWildNameMatch(pkEntry->d_name, pcFile))
				{
					kOut.push_back(pkEntry->d_name);
				}

			}
		}
		VE_ASSERT_EQ(closedir(pkDir), VE_S_OK);
	}

#	endif
}
//--------------------------------------------------------------------------
VeDirectoryPtr VeFilePath::Open(const VeChar8* pcPath,
	bool bTryCreate) const noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	PATH_CAT(acBuffer, m_kPath, pcPath);
	return Create(acBuffer, bTryCreate);
}
//--------------------------------------------------------------------------
VeBinaryIStreamPtr VeFilePath::OpenIStream(
	const VeChar8* pcFile) const noexcept
{
	if (HasFile(pcFile))
	{
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		PATH_CAT(acBuffer, m_kPath, pcFile);
		return VE_NEW VeFileIStream(acBuffer);
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeBinaryOStreamPtr VeFilePath::OpenOStream(
	const VeChar8* pcFile) const noexcept
{
	if (HasFile(pcFile))
	{
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		PATH_CAT(acBuffer, m_kPath, pcFile);
		return VE_NEW VeFileOStream(acBuffer);
	}
	return nullptr;
}
//--------------------------------------------------------------------------
void VeFilePath::ReadAsync(const VeChar8* pcFile,
	ReadCallback kCallback) const noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	PATH_CAT(acBuffer, m_kPath, pcFile);
	VE_NEW ReadTask(acBuffer, kCallback);
}
//--------------------------------------------------------------------------
void VeFilePath::WriteAsync(const VeChar8* pcFile,
	const VeMemoryOStreamPtr& spContent, WriteCallback kCallback,
	bool bAppend) const noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	PATH_CAT(acBuffer, m_kPath, pcFile);
	VE_NEW WriteTask(acBuffer, spContent, kCallback, bAppend);
}
//--------------------------------------------------------------------------
bool VeFilePath::TestPath(const VeChar8* pcPath, PathInfo& kOutput) noexcept
{
#	ifdef VE_PLATFORM_WIN
	_finddata_t kData;
	VeSizeT stHandle = _findfirst(pcPath, &kData);
	if (stHandle != VE_ELF)
	{
		kOutput.m_stAttrib = kData.attrib & 0xf0;
		kOutput.m_stLastModTime = kData.time_write;
		return true;
	}
#	else
	struct stat kBuf;
	if (VE_SUCCEEDED(lstat(pcPath, &kBuf)))
	{
		kOutput.m_stAttrib = 0;
		if(S_ISREG(kBuf.st_mode))
			kOutput.m_stAttrib |= VE_A_ARCH;
		if(S_ISDIR(kBuf.st_mode))
			kOutput.m_stAttrib |= VE_A_SUBDIR;
		kOutput.m_stLastModTime = kBuf.st_mtime;
		return true;
	}
#	endif
	return false;
}
//--------------------------------------------------------------------------
bool VeFilePath::CreatePath(const VeChar8* pcPath) noexcept
{
	VeSizeT stLen = VeStrlen(pcPath);
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	{
		bool bSingleRelative(true);
		const VeChar8* pcStart = pcPath;
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
			VeMemoryCopy(acBuffer, pcPath, stLen + 1);
		}
	}
	VeChar8* pcTemp = acBuffer;
	if ((*pcTemp) == '/') ++pcTemp;
	VeChar8* pcEnd = acBuffer + stLen;
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

		if (VE_SUCCEEDED(VeAccess(acBuffer, 2)))
		{
			bAllowCreate = true;
			if (pcTemp < pcEnd)
			{
				(*pcTemp) = '/';
				++pcTemp;
			}

		}
		else if (VE_SUCCEEDED(VeAccess(acBuffer, 0)))
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
			if (VE_SUCCEEDED(VeMkDir(acBuffer)))
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
VeDirectoryPtr VeFilePath::Create(const VeChar8* pcPath,
	bool bTryCreate) noexcept
{
	PathInfo kInfo;
	if (TestPath(pcPath, kInfo))
	{
		if (VE_MASK_HAS_ALL(kInfo.m_stAttrib, VE_A_SUBDIR))
		{
			return VE_NEW VeFilePath(pcPath);
		}
		else if(VE_MASK_HAS_ALL(kInfo.m_stAttrib, VE_A_ARCH))
		{
			return nullptr;
		}
	}
	else if(bTryCreate && CreatePath(pcPath))
	{
		return VE_NEW VeFilePath(pcPath);
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeBinaryIStreamPtr VeFilePath::CreateIStream(const VeChar8* pcPath) noexcept
{
	PathInfo kInfo;
	if (TestPath(pcPath, kInfo))
	{
		if (VE_MASK_HAS_ALL(kInfo.m_stAttrib, VE_A_ARCH))
		{
			return VE_NEW VeFileIStream(pcPath);
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeBinaryOStreamPtr VeFilePath::CreateOStream(const VeChar8* pcPath) noexcept
{
	return VE_NEW VeFileOStream(pcPath);
}
//--------------------------------------------------------------------------
VeFilePath::ReadTask::ReadTask(const VeChar8* pcFullPath,
	ReadCallback kCallback) noexcept
	: m_kFullPath(pcFullPath)
{
	IncRefCount();
	m_kNode.m_Content = this;
	ve_sys.Collect(m_kNode);
	m_kCallback = kCallback;
	VeTaskQueue& kQueue = ve_res_mgr.GetTaskQueue(VeResourceManager::TASK_FILE);
	kQueue.AddBGTask([this](VeTaskQueue& kMgr) noexcept
	{
		if (VE_SUCCEEDED(VeAccess(m_kFullPath, ACCESS_R_OK)))
		{
			FILE* pkFile(nullptr);
#			ifdef VE_PLATFORM_WIN
			fopen_s(&pkFile, m_kFullPath, "rb");
#			else
			pkFile = fopen(m_kFullPath, "rb");
#			endif
			if (pkFile)
			{
				fseek(pkFile, 0, SEEK_END);
				VeSizeT stLen = ftell(pkFile);
				fseek(pkFile, 0, SEEK_SET);
				VeBlobPtr spBlob = VE_NEW VeBlob(stLen);
				VeSizeT stRead = fread(spBlob->GetBuffer(), 1, stLen, pkFile);
				fclose(pkFile);
				if (stRead == stLen)
				{
					m_spData = VE_NEW VeMemoryIStream(spBlob);
				}
			}
		}
		kMgr.AddFGTask([this](VeTaskQueue& kMgr) noexcept
		{
			m_kCallback(m_spData);
			DecRefCount();
		});
	});
}
//--------------------------------------------------------------------------
VeFilePath::WriteTask::WriteTask(const VeChar8* pcFullPath,
	const VeMemoryOStreamPtr& spContent, WriteCallback kCallback,
	bool bAppend) noexcept
	: m_kFullPath(pcFullPath), m_spData(spContent)
{
	IncRefCount();
	m_kNode.m_Content = this;
	ve_sys.Collect(m_kNode);
	m_kCallback = kCallback;
	VeTaskQueue& kQueue = ve_res_mgr.GetTaskQueue(VeResourceManager::TASK_FILE);
	kQueue.AddBGTask([this,bAppend](VeTaskQueue& kMgr) noexcept
	{
		FILE* pkFile(nullptr);
		const VeChar8* pcMode = bAppend ? "ab" : "wb";
#		ifdef VE_PLATFORM_WIN
		fopen_s(&pkFile, m_kFullPath, pcMode);
#		else
		pkFile = fopen(m_kFullPath, pcMode);
#		endif
		if (pkFile)
		{
			VeSizeT stWrite = fwrite(m_spData->GetDataForRead(),
				1, m_spData->RemainingLength(), pkFile);
			fclose(pkFile);
			if (stWrite == m_spData->RemainingLength())
			{
				m_eResult = VE_S_OK;
			}
			else
			{
				m_eResult = VE_E_FILE_WRITE;
			}
			m_spData->AfterReadData(stWrite);
		}
		else
		{
			m_eResult = VE_E_FILE_CREATE;
		}
		kMgr.AddFGTask([this](VeTaskQueue& kMgr) noexcept
		{
			if (m_kCallback)
			{
				m_kCallback(m_eResult);
			}
			DecRefCount();
		});
	});
}
//--------------------------------------------------------------------------
