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
VeResource::VeResource(const VeChar8* pcName) noexcept
{
	m_kNode.m_Content = this;
	m_kName = pcName;
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
	if (m_eState == UNLOADED || m_eState == LOADED)
	{
		auto spCache = ve_res_mgr.CacheFile(m_kName, m_spGroup);
		if (spCache)
		{
			LoadImpl(spCache);
		}
		else
		{
			OnResLoadFailed(ERR_FILE_NOT_FOUND);
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
void VeResource::LoadAsync() noexcept
{
	if (m_eState == LOADED)
	{
		Hold();
	}
	else
	{
		m_eState = LOADING;
		UpdateState();
	}
	ve_res_mgr.CacheFile(m_kName, m_spGroup, [this](
		VeResourceManager::FileCachePtr spCache) noexcept
	{
		if (spCache)
		{
			LoadImpl(spCache);
		}
		else
		{
			if (m_eState == LOADED)
			{
				Free();
			}
			else
			{
				m_eState = UNLOADED;
				UpdateState();
			}
			OnResLoadFailed(ERR_FILE_NOT_FOUND);
		}
	});
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
		OnResLoaded();
	}
}
//--------------------------------------------------------------------------
void VeResource::OnResLoaded() noexcept
{
	VE_ASSERT(!m_u32WaitNumber);
	if (m_eState != LOADED)
	{
		m_eState = LOADED;
		UpdateState();
		ve_sys.CORE.I.LogFormat("Resource[\"%s\"] is loaded.", (const VeChar8*)m_kName);
	}
	else
	{
		ve_sys.CORE.I.LogFormat("Resource[\"%s\"] is updated.", (const VeChar8*)m_kName);
	}	
}
//--------------------------------------------------------------------------
void VeResource::OnResUnloaded() noexcept
{
	VE_ASSERT(m_u32HoldCount == 0);
	m_eState = UNLOADED;
	UpdateState();
	ve_sys.CORE.I.LogFormat("Resource[\"%s\"] is unloaded.", (const VeChar8*)m_kName);
}
//--------------------------------------------------------------------------
void VeResource::OnResLoadFailed(Error eError) noexcept
{
	ve_sys.CORE.E.LogFormat("%s occurred while %s resource[\"%s\"]",
		ve_parser.EnumToStr(eError),
		m_eState == LOADED ? "updating" : "loading",
		(const VeChar8*)m_kName);
}
//--------------------------------------------------------------------------
void VeResource::LoadImpl(VeResourceManager::FileCachePtr spCache) noexcept
{
	if (m_u32WaitNumber == 0)
	{
		OnResLoaded();
	}
}
//--------------------------------------------------------------------------
void VeResource::UnloadImpl() noexcept
{
	OnResUnloaded();
}
//--------------------------------------------------------------------------
