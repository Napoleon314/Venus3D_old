////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeAsset.cpp
//  Created:     2015/03/12 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
#ifdef VE_PLATFORM_ANDROID
//--------------------------------------------------------------------------
static AAssetManager* s_pkAssetManager = nullptr;
//--------------------------------------------------------------------------
void SetAssetManager(AAssetManager* pkManager)
{
	VE_ASSERT(pkManager);
	s_pkAssetManager = pkManager;
}
//--------------------------------------------------------------------------
VeRTTIImpl(VeAssetIStream, VeBinaryIStream);
//--------------------------------------------------------------------------
VeAssetIStream::VeAssetIStream(const VeChar8* pcAssetName) noexcept
{
	VE_ASSERT(s_pkAssetManager);
	m_pkAsset = AAssetManager_open(s_pkAssetManager,
		pcAssetName, AASSET_MODE_UNKNOWN);
	VE_ASSERT(m_pkAsset);
}
//--------------------------------------------------------------------------
VeAssetIStream::~VeAssetIStream() noexcept
{
	Finish();
}
//--------------------------------------------------------------------------
VeSizeT VeAssetIStream::Read(void* pvBuffer, VeSizeT stBytes) noexcept
{
	return AAsset_read(m_pkAsset, pvBuffer, stBytes);
}
//--------------------------------------------------------------------------
VeResult VeAssetIStream::Seek(VePtrDiff pdOffset, VeWhence eWhence) noexcept
{
	return VE_SUCCEEDED(AAsset_seek(m_pkAsset, pdOffset, eWhence))
		? VE_S_OK : VE_E_FAIL;
}
//--------------------------------------------------------------------------
VeSizeT VeAssetIStream::Tell() noexcept
{
	return AAsset_getLength(m_pkAsset) - AAsset_getRemainingLength(m_pkAsset);
}
//--------------------------------------------------------------------------
void* VeAssetIStream::Skip(VeSizeT stBytes) noexcept
{
	stBytes = VE_MIN(stBytes, RemainingLength());
	AAsset_seek(m_pkAsset, stBytes, SEEK_CUR);
	return nullptr;
}
//--------------------------------------------------------------------------
void VeAssetIStream::Restart() noexcept
{
	AAsset_seek(m_pkAsset, 0, SEEK_SET);
}
//--------------------------------------------------------------------------
VeSizeT VeAssetIStream::RemainingLength() const noexcept
{
	return AAsset_getRemainingLength(m_pkAsset);
}
//--------------------------------------------------------------------------
VeChar8 VeAssetIStream::Peek() noexcept
{
	if (RemainingLength() <= 0)
	{
		SetError(true);
		return -1;
	}
	VeChar8 c8Res(-1);
	VE_ASSERT_EQ(AAsset_read(m_pkAsset, &c8Res, 1), 1);
	AAsset_seek(m_pkAsset, -1, SEEK_CUR);
	return c8Res;
}
//--------------------------------------------------------------------------
void VeAssetIStream::Finish() noexcept
{
	if (m_pkAsset)
	{
		AAsset_close(m_pkAsset);
		m_pkAsset = nullptr;
	}
}
//--------------------------------------------------------------------------
VeRTTIImpl(VeAssetPath, VeDirectory);
//--------------------------------------------------------------------------
const VeChar8 * const VeAssetPath::ms_pcType = "asset";
//--------------------------------------------------------------------------
VeAssetPath::VeAssetPath(const VeChar8* pcPath) noexcept
	: m_kPath(pcPath)
{

}
//--------------------------------------------------------------------------
VeAssetPath::~VeAssetPath() noexcept
{

}
//--------------------------------------------------------------------------
const VeChar8* VeAssetPath::GetTypeName() noexcept
{
	return ms_pcType;
}
//--------------------------------------------------------------------------
const VeChar8* VeAssetPath::GetType() noexcept
{
	return GetTypeName();
}
//--------------------------------------------------------------------------
const VeChar8* VeAssetPath::GetName() noexcept
{
	return m_kPath;
}
//--------------------------------------------------------------------------
bool VeAssetPath::Access(VeUInt32 u32Flag) const noexcept
{
	return !VE_MASK_HAS_ALL(u32Flag, VeDirectory::ACCESS_W_OK);
}
//--------------------------------------------------------------------------
#define PATH_CAT(buf,c,n)									\
	if(c.GetLength()) VeSprintf(buf,"%s/%s",c.GetStr(),n);	\
	else VeStrcpy(buf,n)
//--------------------------------------------------------------------------
bool VeAssetPath::HasFile(const VeChar8* pcFile) const noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	PATH_CAT(acBuffer, m_kPath, pcFile);
	AAsset* pkAsset = AAssetManager_open(s_pkAssetManager,
		acBuffer, AASSET_MODE_UNKNOWN);
	if (pkAsset)
	{
		AAsset_close(pkAsset);
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------------
bool VeAssetPath::HasSubDir(const VeChar8* pcDir) const noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	PATH_CAT(acBuffer, m_kPath, pcDir);
	AAssetDir* pkDir = AAssetManager_openDir(s_pkAssetManager, acBuffer);
	if (pkDir)
	{
		AAssetDir_close(pkDir);
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------------
void VeAssetPath::FindFileList(const VeChar8* pcDesc,
	VeVector<VeFixedString>& kOut) const noexcept
{
	const VeChar8* pcFile = VeStrrchr(pcDesc, '/');
	pcFile = pcFile ? (pcFile + 1) : pcDesc;
	AAssetDir* pkDir(nullptr);
	if (pcFile == pcDesc)
	{
		pkDir = AAssetManager_openDir(s_pkAssetManager, m_kPath);
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
		pkDir = AAssetManager_openDir(s_pkAssetManager, acBuffer);
	}
	if (pkDir)
	{
		const VeChar8* pcEntry;
		while ((pcEntry = AAssetDir_getNextFileName(pkDir)))
		{
			if (VeWildNameMatch(pcEntry, pcFile))
			{
				kOut.push_back(pcEntry);
			}
		}
		AAssetDir_close(pkDir);
	}
}
//--------------------------------------------------------------------------
VeDirectoryPtr VeAssetPath::Open(const VeChar8* pcPath,
	bool bTryCreate) const noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	PATH_CAT(acBuffer, m_kPath, pcPath);
	return Create(acBuffer, bTryCreate);
}
//--------------------------------------------------------------------------
VeBinaryIStreamPtr VeAssetPath::OpenSync(
	const VeChar8* pcFile) const noexcept
{
	if (HasFile(pcFile))
	{
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		PATH_CAT(acBuffer, m_kPath, pcFile);
		return VE_NEW VeAssetIStream(acBuffer);
	}
	return nullptr;
}
//--------------------------------------------------------------------------
void VeAssetPath::ReadAsync(const VeChar8* pcFile,
	VeRefNode<ReadCallback>& kCallback) const noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	PATH_CAT(acBuffer, m_kPath, pcFile);
	VE_NEW ReadTask(acBuffer, kCallback);
}
//--------------------------------------------------------------------------
void VeAssetPath::WriteAsync(const VeChar8* pcFileName,
	const VeMemoryOStreamPtr& spContent,
	VeRefNode<WriteCallback>& kDelegate,
	bool bAppend) const noexcept
{
	kDelegate.m_Content(VE_E_FILE_ACCESS);
}
//--------------------------------------------------------------------------
VeDirectoryPtr VeAssetPath::Create(const VeChar8* pcPath,
	bool bTryCreate) noexcept
{
	AAssetDir* pkDir = AAssetManager_openDir(s_pkAssetManager, pcPath);
	if (pkDir)
	{
		AAssetDir_close(pkDir);
		return VE_NEW VeAssetPath(pcPath);
	}
	else
	{
		return nullptr;
	}
}
//--------------------------------------------------------------------------
VeBinaryIStreamPtr VeAssetPath::CreateStream(const VeChar8* pcPath) noexcept
{
	AAsset* pkAsset = AAssetManager_open(s_pkAssetManager,
		pcPath, AASSET_MODE_UNKNOWN);
	if (pkAsset)
	{
		AAsset_close(pkAsset);
		return VE_NEW VeAssetIStream(pcPath);
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeAssetPath::ReadTask::ReadTask(const VeChar8* pcFullPath,
	VeRefNode<ReadCallback>& kCallback) noexcept
	: m_kFullPath(pcFullPath)
{
	IncRefCount();
	m_kNode.m_Content = this;
	ve_sys.Collect(m_kNode);
	m_kCallback.attach_back(kCallback);
	m_kTask.m_Content = [this](VeTaskQueue& kMgr) noexcept
	{
		AAsset* pkAsset = AAssetManager_open(s_pkAssetManager,
			m_kFullPath, AASSET_MODE_UNKNOWN);
		if (pkAsset)
		{
			VeInt32 i32Len = AAsset_getLength(pkAsset);
			if (i32Len)
			{
				VeBlobPtr spBlob = VE_NEW VeBlob(i32Len);
				VeInt32 i32Read = AAsset_read(pkAsset,
					spBlob->GetBuffer(), i32Len);
				if (i32Read == i32Len)
				{
					m_spData = VE_NEW VeMemoryIStream(spBlob);
				}
			}
			AAsset_close(pkAsset);
		}
		m_kTask.m_Content = [this](VeTaskQueue& kMgr) noexcept
		{
			for (auto call : m_kCallback)
			{
				call(m_spData);
			}
			DecRefCount();
		};
		kMgr.AddFGTask(m_kTask);
	};
	ve_res_mgr.GetTaskQueue(VeResourceManager::TASK_FILE).AddBGTask(m_kTask);
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
