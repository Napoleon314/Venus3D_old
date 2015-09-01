////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeResourceManager.cpp
//  Created:     2015/03/10 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeResourceManager::VeResourceManager() noexcept
{

}
//--------------------------------------------------------------------------
VeResourceManager::~VeResourceManager() noexcept
{
	while (HasTask())
	{
		Update();
	}
	VE_ASSERT(m_kLoadingResList.empty());
	for (auto it : m_kResourceMap)
	{
		it.second->Unload();
	}
	while (HasTask())
	{
		Update();
	}
	m_kResourceMap.clear();
	VE_ASSERT(m_kLoadingFileList.empty() && m_kUsingFileList.empty());
	m_kFileCacheMap.clear();
	VE_ASSERT(m_kFreeFileList.empty());
}
//--------------------------------------------------------------------------
bool VeResourceManager::HasTask() noexcept
{
	for (auto& obj : m_akTaskQueues)
	{
		if (obj.HasTask())
		{
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------------------
void VeResourceManager::Update() noexcept
{
	for (auto& obj : m_akTaskQueues)
	{
		obj.Update();
	}
	VeUInt32 u32Tick = ve_time.GetTimeUInt();	
	while (m_kFreeFileList.size())
	{
		VeRefNode<FileCache*>* pkNode = m_kFreeFileList.get_head_node();
		VeUInt32 u32Delta = u32Tick > pkNode->m_Content->m_u32FreeTime
			? u32Tick - pkNode->m_Content->m_u32FreeTime : 0;
		if (u32Delta >= m_u32FileCacheFreeTime)
		{
			pkNode->detach();
			m_kFileCacheMap.erase(pkNode->m_Content->m_kFullPath);
		}
		else
		{
			break;
		}
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::SetDefaultDirCreator(DirCreator kCreator) noexcept
{
	m_kDefaultDirCreator = kCreator;
}
//--------------------------------------------------------------------------
void VeResourceManager::RegistDirCreator(const VeChar8* pcName,
	DirCreator kCreator) noexcept
{
	m_kDirCreatorMap[pcName] = kCreator;
}
//--------------------------------------------------------------------------
void VeResourceManager::UnregistDirCreator(const VeChar8* pcName) noexcept
{
	auto it = m_kDirCreatorMap.find(pcName);
	if (it != m_kDirCreatorMap.end())
	{
		m_kDirCreatorMap.erase(it);
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::SetDefaultStreamCreator(
	StreamCreator kCreator) noexcept
{
	m_kDefaultStreamCreator = kCreator;
}
//--------------------------------------------------------------------------
void VeResourceManager::RegistStreamCreator(const VeChar8* pcName,
	StreamCreator kCreator) noexcept
{
	m_kStreamCreatorMap[pcName] = kCreator;
}
//--------------------------------------------------------------------------
void VeResourceManager::UnregistStreamCreator(
	const VeChar8* pcName) noexcept
{
	auto it = m_kStreamCreatorMap.find(pcName);
	if (it != m_kStreamCreatorMap.end())
	{
		m_kStreamCreatorMap.erase(it);
	}
}
//--------------------------------------------------------------------------
VeDirectoryPtr VeResourceManager::CreateDir(const VeChar8* pcPath,
	bool bTryCreate) noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	VeStrcpy(acBuffer, pcPath);
	VeChar8* pcContext;
	VeChar8* pcTemp = VeStrtok(acBuffer, "#", &pcContext);
	if (pcContext)
	{
		auto it = m_kDirCreatorMap.find(pcTemp);
		if (it != m_kDirCreatorMap.end())
		{
			return it->second(pcContext, bTryCreate);
		}
	}
	else
	{
		return m_kDefaultDirCreator(pcPath, bTryCreate);
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeBinaryIStreamPtr VeResourceManager::CreateStream(
	const VeChar8* pcPath) noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	VeStrcpy(acBuffer, pcPath);
	VeChar8* pcContext;
	VeChar8* pcTemp = VeStrtok(acBuffer, "#", &pcContext);
	if (pcContext)
	{
		auto it = m_kStreamCreatorMap.find(pcTemp);
		if (it != m_kStreamCreatorMap.end())
		{
			return it->second(pcContext);
		}
	}
	else
	{
		return m_kDefaultStreamCreator(pcPath);
	}
	return nullptr;
}
//--------------------------------------------------------------------------
void VeResourceManager::RegistFileCreator(const VeChar8* pcExt,
	FileCreator kCreator, FileCreatorSync kCreatorSync) noexcept
{
	m_kFileCreatorMap[pcExt] = std::make_pair(kCreator, kCreatorSync);
}
//--------------------------------------------------------------------------
void VeResourceManager::RegistFileCreator(
	std::initializer_list<const VeChar8*> kExtList,
	FileCreator kCreator, FileCreatorSync kCreatorSync) noexcept
{
	for (auto ext : kExtList)
	{
		RegistFileCreator(ext, kCreator, kCreatorSync);
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::UnregistFileCreator(const VeChar8* pcExt) noexcept
{
	auto it = m_kFileCreatorMap.find(pcExt);
	if (it != m_kFileCreatorMap.end())
	{
		m_kFileCreatorMap.erase(it);
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::UnregistFileCreator(
	std::initializer_list<const VeChar8*> kExtList) noexcept
{
	for (auto ext : kExtList)
	{
		UnregistFileCreator(ext);
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::RegistJSONCreator(const VeChar8* pcType,
	JSONCreator kCreator, JSONCreatorSync kCreatorSync) noexcept
{
	m_kJSONCreatorMap[pcType] = std::make_pair(kCreator, kCreatorSync);
}
//--------------------------------------------------------------------------
void VeResourceManager::UnregistJSONCreator(const VeChar8* pcType) noexcept
{
	auto it = m_kJSONCreatorMap.find(pcType);
	if (it != m_kJSONCreatorMap.end())
	{
		m_kJSONCreatorMap.erase(it);
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::RegistResCreator(const VeChar8* pcType,
	ResCreator kCreator) noexcept
{
	m_kResCreatorMap[pcType] = kCreator;
}
//--------------------------------------------------------------------------
void VeResourceManager::UnregistResCreator(const VeChar8* pcType) noexcept
{
	auto it = m_kResCreatorMap.find(pcType);
	if (it != m_kResCreatorMap.end())
	{
		m_kResCreatorMap.erase(it);
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::SetupGroupFromJSON(const VeChar8* pcText) noexcept
{
	VeJSONDoc kDoc;
	if (kDoc.Parse<0>(pcText).HasParseError())
	{
		ve_sys.CORE.E.LogFormat("Parse Error in SetupGroupFromJSON.");
		return;
	}
	SetupGroup(kDoc);
}
//--------------------------------------------------------------------------
void VeResourceManager::SetupGroup(const VeJSONValue& kObj) noexcept
{
	if (kObj.IsObject())
	{
		for (auto it = kObj.MemberBegin(); it != kObj.MemberEnd(); ++it)
		{
			if (it->value.IsObject())
			{
				VeResourceGroupPtr spGroup = GetGroup(it->name.GetString());
				{
					const VeJSONValue& kRead = it->value["r"];
					if (kRead.IsArray())
					{
						for (VeUInt32 i(0); i < kRead.Size(); ++i)
						{
							const VeJSONValue& kVal = kRead[i];
							if (kVal.IsString())
							{
								spGroup->PushReadDirBack(CreateDir(kVal.GetString()));
							}
						}
					}
					else if (kRead.IsString())
					{
						spGroup->PushReadDirBack(CreateDir(kRead.GetString()));
					}
				}
				{
					const VeJSONValue& kWrite = it->value["w"];
					if (kWrite.IsString())
					{
						spGroup->SetWriteDirectory(CreateDir(kWrite.GetString()));
					}
				}
				if (VeStrcmp(it->name.GetString(), "startup") == 0)
				{
					m_spDefaultGroup = spGroup;
				}
			}
		}
	}
}
//--------------------------------------------------------------------------
VeResourceGroupPtr VeResourceManager::GetGroup(const VeChar8* pcName,
	bool bCreate) noexcept
{
	auto iter = m_kGroupMap.find(pcName);
	if (iter != m_kGroupMap.end())
	{
		return iter->second;
	}
	else if (bCreate)
	{
		VeResourceGroupPtr spGroup = VE_NEW VeResourceGroup(pcName);
		m_kGroupMap[pcName] = spGroup;
		return spGroup;
	}
	else
	{
		return nullptr;
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::SetDefaultGroup(
	const VeResourceGroupPtr& spGroup) noexcept
{
	m_spDefaultGroup = spGroup;
}
//--------------------------------------------------------------------------
VeResourcePtr VeResourceManager::GetResource(
	const VeChar8* pcFullName, bool bCreate) noexcept
{
	auto it = m_kResourceMap.find(pcFullName);
	if (it != m_kResourceMap.end())
	{
		return it->second;
	}
	if (bCreate)
	{
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeStrcpy(acBuffer, pcFullName);
		VeChar8* pcContext;
		VeChar8* pcTemp = VeStrtok(acBuffer, "@", &pcContext);
		if (pcContext)
		{
			auto itCreator = m_kResCreatorMap.find(pcContext);
			if (itCreator != m_kResCreatorMap.end())
			{
				VeResourcePtr spRes = itCreator->second(pcTemp, acBuffer);
				m_kResourceMap.insert(std::make_pair(pcFullName, spRes));
				return spRes;
			}
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeResourcePtr VeResourceManager::GetResource(const VeChar8* pcType,
	const VeChar8* pcName, bool bCreate) noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	VeSprintf(acBuffer, "%s@%s", pcName, pcType);
	auto it = m_kResourceMap.find(acBuffer);
	if (it != m_kResourceMap.end())
	{
		return it->second;
	}
	if (bCreate)
	{
		auto itCreator = m_kResCreatorMap.find(pcType);
		if (itCreator != m_kResCreatorMap.end())
		{
			VeResourcePtr spRes = itCreator->second(pcName, acBuffer);
			m_kResourceMap.insert(std::make_pair(acBuffer, spRes));
			return spRes;
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------
void VeResourceManager::LoadFile(const VeChar8* pcPath,
	LoadCallback kCallback) noexcept
{
	CacheFile(pcPath, [this,kCallback](FileCachePtr spCache) noexcept
	{
		if (spCache)
		{
			const VeChar8* pcExt = VeStrrchr(spCache->GetFileName(), '.');
			pcExt = pcExt ? pcExt + 1 : "";
			auto it = m_kFileCreatorMap.find(pcExt);
			if (it != m_kFileCreatorMap.end())
			{
				it->second.first(spCache, kCallback);
			}
		}
	});
}
//--------------------------------------------------------------------------
void VeResourceManager::LoadFile(const VeChar8* pcPath) noexcept
{
	FileCachePtr spCache = CacheFile(pcPath);
	if (spCache->GetData())
	{
		const VeChar8* pcExt = VeStrrchr(spCache->GetFileName(), '.');
		pcExt = pcExt ? pcExt + 1 : "";
		auto it = m_kFileCreatorMap.find(pcExt);
		if (it != m_kFileCreatorMap.end())
		{
			it->second.second(spCache);
		}
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::LoadGroup(const VeChar8* pcGroup) noexcept
{

}
//--------------------------------------------------------------------------
void VeResourceManager::ParseJSON(FileCachePtr spCache,
	LoadCallback kCallback) noexcept
{
	JSONCache* pkJSON = VE_NEW JSONCache(spCache, kCallback);
	VeTaskQueue& kQueue = GetTaskQueue(VeResourceManager::TASK_PROCESS);
	kQueue.AddBGTask([pkJSON](VeTaskQueue& kMgr) noexcept
	{
		pkJSON->Parse();
		kMgr.AddFGTask([pkJSON](VeTaskQueue& kMgr) noexcept
		{
			pkJSON->Create();
		});
	});
}
//--------------------------------------------------------------------------
void VeResourceManager::ParseJSON(FileCachePtr spCache) noexcept
{
	VeJSONDoc kDoc;
	kDoc.Parse<0>((const VeChar8*)(spCache->GetData()->GetBlob()->GetBuffer()));
	if (kDoc.HasParseError())
	{
		ve_sys.CORE.E.LogFormat("Parse Error in [\"%s\"].", spCache->GetFullPath());
	}
	else
	{
		VeCoreDebugOutput("[\"%s\"] is parsed.", spCache->GetFullPath());
		for (auto it = kDoc.MemberBegin(); it != kDoc.MemberEnd(); ++it)
		{
			auto itCreator = m_kJSONCreatorMap.find(it->name.GetString());
			if (itCreator != m_kJSONCreatorMap.end())
			{
				itCreator->second.second(spCache, it->value);
			}
		}
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::CacheFile(const VeChar8* pcPath,
	FileCallback kCallback) noexcept
{
	FileCachePtr spFile = GetFileCache(pcPath);
	if (spFile->m_kNode.is_attach(m_kLoadingFileList))
	{
		spFile->m_kCallbackList.push_back(kCallback);
		spFile->Load();
	}
	else
	{
		kCallback(spFile);
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::CacheFile(const VeChar8* pcFile,
	const VeResourceGroupPtr& spGroup, FileCallback kCallback) noexcept
{
	FileCachePtr spFile = GetFileCache(pcFile, spGroup);
	if (spFile->m_kNode.is_attach(m_kLoadingFileList))
	{
		spFile->m_kCallbackList.push_back(kCallback);
		spFile->Load();
	}
	else
	{
		kCallback(spFile);
	}
	
}
//--------------------------------------------------------------------------
VeResourceManager::FileCachePtr VeResourceManager::CacheFile(
	const VeChar8* pcPath) noexcept
{
	FileCachePtr spFile = GetFileCache(pcPath);
	if (spFile->m_kNode.is_attach(m_kLoadingFileList))
	{
		spFile->LoadSync();
	}
	return spFile->GetData() ? spFile : nullptr;
}
//--------------------------------------------------------------------------
VeResourceManager::FileCachePtr VeResourceManager::CacheFile(
	const VeChar8* pcFile, const VeResourceGroupPtr& spGroup) noexcept
{
	FileCachePtr spFile = GetFileCache(pcFile, spGroup);
	if (spFile->m_kNode.is_attach(m_kLoadingFileList))
	{
		spFile->LoadSync();
	}
	return spFile->GetData() ? spFile : nullptr;
}
//--------------------------------------------------------------------------
VeResourceManager::FileCachePtr VeResourceManager::GetFileCache(
	const VeChar8* pcPath) noexcept
{
	FileCachePtr spFile;
	auto it = m_kFileCacheMap.find(pcPath);
	if (it == m_kFileCacheMap.end())
	{
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeStrcpy(acBuffer, pcPath);
		VeChar8* pcContext;
		VeChar8* pcTemp = VeStrtok(acBuffer, "$", &pcContext);
		VeResourceGroupPtr spGroup = m_spDefaultGroup;
		if (pcContext)
		{
			auto it = m_kGroupMap.find(pcTemp);
			if (it != m_kGroupMap.end())
			{
				spGroup = it->second;
			}
			pcTemp = pcContext;
		}
		spFile = VE_NEW FileCache(pcPath, pcTemp, spGroup);
		m_kFileCacheMap.insert(std::make_pair(pcPath, spFile));
	}
	else
	{
		spFile = it->second;
	}
	VE_ASSERT(spFile);
	return spFile;
}
//--------------------------------------------------------------------------
VeResourceManager::FileCachePtr VeResourceManager::GetFileCache(
	const VeChar8* pcFile, const VeResourceGroupPtr& spGroup) noexcept
{
	FileCachePtr spFile;
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	VeSprintf(acBuffer, "%s$%s", spGroup->GetName(), pcFile);
	auto it = m_kFileCacheMap.find(acBuffer);
	if (it == m_kFileCacheMap.end())
	{
		spFile = VE_NEW FileCache(acBuffer, pcFile, spGroup);
		m_kFileCacheMap.insert(std::make_pair(acBuffer, spFile));
	}
	else
	{
		spFile = it->second;
	}
	VE_ASSERT(spFile);
	return spFile;
}
//--------------------------------------------------------------------------
VeResourceManager::FileCache::FileCache(const VeChar8* pcPath,
	const VeChar8* pcName, const VeResourceGroupPtr& spGroup) noexcept
{
	m_kNode.m_Content = this;
	ve_res_mgr.m_kLoadingFileList.attach_back(m_kNode);
	m_kFullPath = pcPath;
	m_kFileName = pcName;
	m_spGroup = spGroup;	
}
//--------------------------------------------------------------------------
VeResourceManager::FileCache::~FileCache() noexcept
{
	VE_ASSERT(m_kCallbackList.empty());
#	ifdef VE_DEBUG
	if (m_spData)
	{
		VeCoreDebugOutput("[\"%s\"] is uncached.", m_kFullPath);
	}
#	endif
}
//--------------------------------------------------------------------------
void VeResourceManager::FileCache::Load() noexcept
{
	VeTaskQueue& kQueue = ve_res_mgr.GetTaskQueue(VeResourceManager::TASK_QUERY);
	kQueue.AddBGTask([this](VeTaskQueue& kMgr) noexcept
	{
		m_spDir = m_spGroup->GetDir(m_kFileName);
		kMgr.AddFGTask([this](VeTaskQueue& kMgr) noexcept
		{
			if (m_spDir)
			{
				m_spDir->ReadAsync(m_kFileName, [this](const VeMemoryIStreamPtr& spData) noexcept
				{
					m_spData = spData;
					m_u32FreeTime = ve_time.GetTimeUInt();
					ve_res_mgr.m_kFreeFileList.attach_back(m_kNode);
					VeCoreDebugOutput("[\"%s\"] is cached.", m_kFullPath);
					for (auto call : m_kCallbackList)
					{
						call(this);
					}
					m_kCallbackList.clear();
				});
			}
			else
			{
				for (auto call : m_kCallbackList)
				{
					call(nullptr);
				}
				m_kCallbackList.clear();
				ve_res_mgr.m_kFileCacheMap.erase(m_kFullPath);
			}
		});
	});
}
//--------------------------------------------------------------------------
void VeResourceManager::FileCache::LoadSync() noexcept
{
	m_spDir = m_spGroup->GetDir(m_kFileName);
	if (m_spDir)
	{
		VeBinaryIStreamPtr spStream = m_spDir->OpenSync(m_kFileName);
		VE_ASSERT(spStream);
		VeSizeT stLen = spStream->RemainingLength();
		VeBlobPtr spBlob = VE_NEW VeBlob(stLen);
		spStream->Read(spBlob->GetBuffer(), stLen);
		m_spData = VE_NEW VeMemoryIStream(spBlob);
		m_u32FreeTime = ve_time.GetTimeUInt();
		ve_res_mgr.m_kFreeFileList.attach_back(m_kNode);
		VeCoreDebugOutput("[\"%s\"] is cached.", m_kFullPath);
		for (auto call : m_kCallbackList)
		{
			call(this);
		}
		m_kCallbackList.clear();
	}
	else
	{
		for (auto call : m_kCallbackList)
		{
			call(nullptr);
		}
		m_kCallbackList.clear();
		ve_res_mgr.m_kFileCacheMap.erase(m_kFullPath);
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::FileCache::Enter() noexcept
{
	if (!(m_u32EnterCount++))
	{
		ve_res_mgr.m_kUsingFileList.attach_back(m_kNode);
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::FileCache::Leave() noexcept
{
	VE_ASSERT(m_u32EnterCount);
	if (!(--m_u32EnterCount))
	{
		m_u32FreeTime = ve_time.GetTimeUInt();
		ve_res_mgr.m_kFreeFileList.attach_back(m_kNode);
	}
}
//--------------------------------------------------------------------------
VeResourceManager::JSONCache::JSONCache(const FileCachePtr& spFile,
	LoadCallback kCallback) noexcept
	: m_spFile(spFile), m_kFullPath(spFile->GetFullPath())
	, m_spGroup(spFile->GetGroup()), m_kCallback(kCallback)
{
	IncRefCount();
	m_kNode.m_Content = this;
	ve_sys.Collect(m_kNode);
	m_spFile->Enter();
}
//--------------------------------------------------------------------------
VeResourceManager::JSONCache::~JSONCache() noexcept
{
	m_kCallback();
}
//--------------------------------------------------------------------------
void VeResourceManager::JSONCache::Parse() noexcept
{
	m_kDoc.Parse<0>((const VeChar8*)(m_spFile->GetData()->GetBlob()->GetBuffer()));
	if (m_kDoc.HasParseError())
	{
		ve_sys.CORE.E.LogFormat("Parse Error in [\"%s\"].", m_spFile->GetFullPath());
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::JSONCache::Create() noexcept
{
	m_spFile->Leave();
	m_spFile = nullptr;
	if (!m_kDoc.HasParseError())
	{
		VeCoreDebugOutput("[\"%s\"] is parsed.", m_kFullPath);
		for (auto it = m_kDoc.MemberBegin(); it != m_kDoc.MemberEnd(); ++it)
		{
			auto itCreator = ve_res_mgr.m_kJSONCreatorMap.find(it->name.GetString());
			if (itCreator != ve_res_mgr.m_kJSONCreatorMap.end())
			{
				itCreator->second.first(this, it->value);
			}
		}
	}
	DecRefCount();
}
//--------------------------------------------------------------------------
