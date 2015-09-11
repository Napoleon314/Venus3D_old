////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeResource.cpp
//  Created:     2015/08/23 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeResource);
//--------------------------------------------------------------------------
VeResource::VeResource() noexcept
{
	m_kNode.m_Content = this;
	m_spGroup = ve_res_mgr.m_spDefaultGroup;
	UpdateState();
}
//--------------------------------------------------------------------------
VeResource::~VeResource() noexcept
{

}
//--------------------------------------------------------------------------
void VeResource::SetGroup(const VeChar8* pcGroup) noexcept
{
	m_spGroup = ve_res_mgr.GetGroup(pcGroup, false);
}
//--------------------------------------------------------------------------
void VeResource::SetGroup(const VeResourceGroupPtr& spGroup) noexcept
{
	m_spGroup = spGroup;
}
//--------------------------------------------------------------------------
void VeResource::Hold() noexcept
{
	++m_u32HoldCount;
	UpdateState();
}
//--------------------------------------------------------------------------
void VeResource::Free() noexcept
{
	VE_ASSERT(m_u32HoldCount);
	--m_u32HoldCount;
	UpdateState();
}
//--------------------------------------------------------------------------
void VeResource::Load() noexcept
{
	if (m_spGroup)
	{
		if (m_eState == UNLOADED)
		{
			m_eState = LOADING;
			UpdateState();
			VeChar8 acBuffer[VE_MAX_PATH_LEN];
			VeSprintf(acBuffer, "%s.%s", (const VeChar8*)m_kName, GetTypeName());
			ve_res_mgr.CacheFile(acBuffer, m_spGroup, [this](
				VeResourceManager::FileCachePtr spCache) noexcept
			{
				if (spCache)
				{
					LoadImpl(spCache);
				}
				else
				{
					m_eState = UNLOADED;
					UpdateState();
					ve_sys.CORE.W.LogFormat("File not found occurred while loading resource[\"%s\"]", (const VeChar8*)m_kFullName);
				}
			});
		}
		else if (m_eState == LOADED)
		{
			Hold();
			VeChar8 acBuffer[VE_MAX_PATH_LEN];
			VeSprintf(acBuffer, "%s.%s", (const VeChar8*)m_kName, GetTypeName());
			ve_res_mgr.CacheFile(acBuffer, m_spGroup, [this](
				VeResourceManager::FileCachePtr spCache) noexcept
			{
				if (spCache)
				{
					UpdateImpl(spCache);
				}
				else
				{
					Free();
					ve_sys.CORE.W.LogFormat("File not found occurred while updating resource[\"%s\"].", (const VeChar8*)m_kFullName);
				}
			});
		}
	}	
}
//--------------------------------------------------------------------------
void VeResource::Unload() noexcept
{
	if (m_eState == LOADED && m_u32HoldCount == 0)
	{
		UnloadImpl();
	}
}
//--------------------------------------------------------------------------
void VeResource::UpdateState() noexcept
{
	if (m_eState == LOADING)
	{
		if (!m_kNode.is_attach(ve_res_mgr.m_kLoadingResList))
		{
			ve_res_mgr.m_kLoadingResList.attach_back(m_kNode);
		}
	}
	else if (m_u32HoldCount)
	{
		if (!m_kNode.is_attach(ve_res_mgr.m_kUsingResList))
		{
			ve_res_mgr.m_kUsingResList.attach_back(m_kNode);
		}
	}
	else
	{
		if (!m_kNode.is_attach(ve_res_mgr.m_kFreeResList))
		{
			ve_res_mgr.m_kFreeResList.attach_back(m_kNode);
			m_u32FreeTime = ve_time.GetTimeUInt();
		}
	}
}
//--------------------------------------------------------------------------
void VeResource::Wait(VeResource* pkRes) noexcept
{
	if (pkRes != this && pkRes->m_eState == LOADING)
	{
		pkRes->m_kWaitList.push_back(this);
		++m_u32WaitNumber;
	}
}
//--------------------------------------------------------------------------
void VeResource::WaitResLoaded(VeResource* pkRes) noexcept
{
	--m_u32WaitNumber;
	if (m_u32WaitNumber == 0)
	{
		if (m_eState == LOADING)
		{
			OnResLoaded();
		}
		else
		{
			OnResUpdated();
		}		
	}
}
//--------------------------------------------------------------------------
void VeResource::OnResLoaded() noexcept
{
	VE_ASSERT(!m_u32WaitNumber);
	m_eState = LOADED;
	UpdateState();
	ve_sys.CORE.I.LogFormat("Resource[\"%s\"] is loaded.", (const VeChar8*)m_kFullName);
}
//--------------------------------------------------------------------------
void VeResource::OnResUpdated() noexcept
{
	VE_ASSERT(!m_u32WaitNumber);
	m_eState = LOADED;
	UpdateState();
	ve_sys.CORE.I.LogFormat("Resource[\"%s\"] is updated.", (const VeChar8*)m_kFullName);
}
//--------------------------------------------------------------------------
void VeResource::OnResUnloaded() noexcept
{
	VE_ASSERT(m_u32HoldCount == 0);
	m_eState = UNLOADED;
	UpdateState();
	ve_sys.CORE.I.LogFormat("Resource[\"%s\"] is unloaded.", (const VeChar8*)m_kFullName);
}
//--------------------------------------------------------------------------
void VeResource::LoadImpl(VeResourceManager::FileCachePtr spCache) noexcept
{
	ParseStream(spCache->GetData());
	if (m_u32WaitNumber == 0)
	{
		OnResLoaded();
	}
}
//--------------------------------------------------------------------------
void VeResource::UpdateImpl(VeResourceManager::FileCachePtr spCache) noexcept
{
	ParseStream(spCache->GetData());
	if (m_u32WaitNumber == 0)
	{
		OnResUpdated();
	}
}
//--------------------------------------------------------------------------
void VeResource::UnloadImpl() noexcept
{
	Clear();
	OnResUnloaded();
}
//--------------------------------------------------------------------------
