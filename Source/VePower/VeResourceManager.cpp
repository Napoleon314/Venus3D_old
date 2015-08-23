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
