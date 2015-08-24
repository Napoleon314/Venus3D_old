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
	VeUInt32 u32Tick = ve_time_ptr->GetTimeUInt();	
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
void VeResourceManager::SetupGroupFromJSON(const VeChar8* pcText) noexcept
{
	VeJSONDoc kDoc;
	if (kDoc.Parse<0>(pcText).HasParseError())
	{
		VeCoreLogE("SetupGroupFromJSON: Parse Error");
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
void VeResourceManager::CacheFile(const VeChar8* pcPath,
	FileCallback kCallback) noexcept
{
	FileCachePtr spFile;
	{
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
			spFile = VE_NEW FileCache(this, pcPath, pcTemp, spGroup);
			m_kFileCacheMap.insert(std::make_pair(pcPath, spFile));
		}
	}
	VE_ASSERT(spFile);
	if (spFile->m_kNode.is_attach(m_kLoadingFileList))
	{
		spFile->m_kCallbackList.push_back(kCallback);
	}
	else
	{
		kCallback(spFile);
	}
}
//--------------------------------------------------------------------------
VeResourceManager::FileCache::FileCache(VeResourceManager* pkParent,
	const VeChar8* pcPath, const VeChar8* pcName,
	VeResourceGroupPtr& spGroup) noexcept
{
	m_kNode.m_Content = this;
	m_pkParent = pkParent;
	VE_ASSERT(m_pkParent);
	m_pkParent->m_kLoadingFileList.attach_back(m_kNode);
	m_kFullPath = pcPath;
	m_kFileName = pcName;
	m_spGroup = spGroup;
	VeTaskQueue& kQueue = ve_res_mgr_ptr->GetTaskQueue(VeResourceManager::TASK_PROCESS);
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
					m_u32FreeTime = ve_time_ptr->GetTimeUInt();
					m_pkParent->m_kFreeFileList.attach_back(m_kNode);
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
				m_pkParent->m_kFileCacheMap.erase(m_kFullPath);
			}
		});
	});
}
//--------------------------------------------------------------------------
VeResourceManager::FileCache::~FileCache() noexcept
{
	VE_ASSERT(m_kCallbackList.empty());
}
//--------------------------------------------------------------------------
void VeResourceManager::FileCache::Enter() noexcept
{
	if (!(m_u32EnterCount++))
	{
		m_pkParent->m_kUsingFileList.attach_back(m_kNode);
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::FileCache::Leave() noexcept
{
	VE_ASSERT(m_u32EnterCount);
	if (!(--m_u32EnterCount))
	{
		m_u32FreeTime = ve_time_ptr->GetTimeUInt();
		m_pkParent->m_kFreeFileList.attach_back(m_kNode);
	}
}
//--------------------------------------------------------------------------
