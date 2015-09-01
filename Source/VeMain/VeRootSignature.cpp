////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRootSignature.cpp
//  Created:     2015/08/31 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeRootSignature, VeResource);
//--------------------------------------------------------------------------
VeRootSignature::VeRootSignature(const VeChar8* pcName,
	const VeChar8* pcFullName) noexcept
{
	m_kName = pcName;
	m_kFullName = pcFullName;
}
//--------------------------------------------------------------------------
VeRootSignature::~VeRootSignature() noexcept
{

}
//--------------------------------------------------------------------------
const VeChar8* VeRootSignature::GetTypeName() const noexcept
{
	return VeRootSignature::GetName();
}
//--------------------------------------------------------------------------
void VeRootSignature::LoadDirect(
	const VeRenderer::RootSignaturePtr& spObj) noexcept
{
	if (m_eState == UNLOADED)
	{
		m_spObject = spObj;
		OnResLoaded();
	}
	else if (m_eState == LOADED)
	{
		m_spObject = spObj;
		OnResUpdated();
	}
}
//--------------------------------------------------------------------------
void VeRootSignature::Clear() noexcept
{
	m_spObject = nullptr;
}
//--------------------------------------------------------------------------
void VeRootSignature::Regist() noexcept
{
	ve_res_mgr.RegistResCreator(VeRootSignature::GetName(), [](
		const VeChar8* pcName, const VeChar8* pcFullName) noexcept
	{
		return VE_NEW VeRootSignature(pcName, pcFullName);
	});
	ve_res_mgr.RegistJSONCreator(VeRootSignature::GetName(),
		[](VeResourceManager::JSONCachePtr spCache,
		VeJSONValue& kValue) noexcept
	{
		if (kValue.IsArray())
		{
			for (auto it = kValue.Begin(); it != kValue.End(); ++it)
			{
				if (it->IsArray())
				{
					VE_NEW JSONParser(spCache, *it);
				}
			}
		}
	},
		[](const VeResourceManager::FileCachePtr& spCache,
			VeJSONValue& kValue) noexcept
	{
		if (kValue.IsArray())
		{
			for (auto it = kValue.Begin(); it != kValue.End(); ++it)
			{
				if (it->IsArray())
				{
					Parse(spCache, *it);
				}
			}
		}
	});
}
//--------------------------------------------------------------------------
void VeRootSignature::Unregist() noexcept
{
	ve_res_mgr.UnregistJSONCreator(VeRootSignature::GetName());
	ve_res_mgr.UnregistResCreator(VeRootSignature::GetName());
}
//--------------------------------------------------------------------------
void VeRootSignature::Parse(const VeResourceManager::FileCachePtr& spCache,
	VeJSONValue& lkValue) noexcept
{
	VeJSONValue* pkValue = nullptr;
	const VeChar8* pcName = nullptr;
	for (auto it = lkValue.Begin(); it != lkValue.End(); ++it)
	{
		if (it->IsObject())
		{
			auto p = it->FindMember("platform");
			if (p != it->MemberEnd())
			{
				if (p->value.IsString())
				{
					if (!ve_renderer_ptr->IsSupported(p->value.GetString()))
					{
						continue;
					}
				}
				else
				{
					continue;
				}
			}
			auto n = it->FindMember("name");
			if (n != it->MemberEnd() && n->value.IsString())
			{
				pcName = n->value.GetString();
			}
			
			if (pcName) pkValue = &(*it);
			break;
		}
	}
	if (pkValue)
	{
		VE_ASSERT(pcName);
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeSprintf(acBuffer, "%s.rso", pcName);
		VeResourceManager::FileCachePtr spFile = ve_res_mgr.CacheFile(
			acBuffer, spCache->GetGroup());
		if (spFile)
		{
			VeRenderer::RootSignaturePtr spObj = ve_renderer_ptr->CreateRootSignature(spFile->GetData()->GetBlob());
			if (!spObj) return;
			VeRootSignaturePtr spRootSignature = ve_res_mgr.Get<VeRootSignature>(
				VeRootSignature::GetName(), pcName);
			spRootSignature->SetGroup(spCache->GetGroup());
			spRootSignature->LoadDirect(spObj);
		}
		else
		{
			VeBlobPtr spBlob = ve_renderer_ptr->SerializeRootSignature(
				pcName, *pkValue, spCache->GetGroup());
			if (!spBlob) return;
			VeRenderer::RootSignaturePtr spObj = ve_renderer_ptr->CreateRootSignature(spBlob);
			if (!spObj) return;
			VeRootSignaturePtr spRootSignature = ve_res_mgr.Get<VeRootSignature>(
				VeRootSignature::GetName(), pcName);
			spRootSignature->SetGroup(spCache->GetGroup());
			spRootSignature->LoadDirect(spObj);
		}
	}
	else
	{
		ve_sys.CORE.E.LogFormat("Root Signature script error in [\"%s\"].",
			spCache->GetFullPath());
	}
}
//--------------------------------------------------------------------------
VeRootSignature::JSONParser::JSONParser(
	const VeResourceManager::JSONCachePtr& spCache,
	VeJSONValue& lkValue) noexcept
{
	IncRefCount();
	m_kNode.m_Content = this;
	ve_sys.Collect(m_kNode);
	m_spCache = spCache;
	VeTaskQueue& kQueue = ve_res_mgr.GetTaskQueue(VeResourceManager::TASK_PROCESS);
	kQueue.AddBGTask([this, &lkValue](VeTaskQueue& kMgr) noexcept
	{
		for (auto it = lkValue.Begin(); it != lkValue.End(); ++it)
		{
			if (it->IsObject())
			{
				auto p = it->FindMember("platform");
				if (p != it->MemberEnd())
				{
					if (p->value.IsString())
					{
						if (!ve_renderer_ptr->IsSupported(p->value.GetString()))
						{
							continue;
						}
					}
					else
					{
						continue;
					}
				}
				auto n = it->FindMember("name");
				if (n != it->MemberEnd() && n->value.IsString())
				{
					m_pcName = n->value.GetString();
				}

				if (m_pcName) m_pkValue = &(*it);
				break;
			}
		}
		if (m_pkValue)
		{
			VE_ASSERT(m_pcName);
			VeChar8 acBuffer[VE_MAX_PATH_LEN];
			VeSprintf(acBuffer, "%s.rso", m_pcName);
			ve_res_mgr.CacheFile(acBuffer, [this](VeResourceManager::FileCachePtr spFile) noexcept
			{
				if (spFile)
				{
					m_spFileCache = spFile;
					VeTaskQueue& kQueue = ve_res_mgr.GetTaskQueue(VeResourceManager::TASK_PROCESS);
					kQueue.AddBGTask([this](VeTaskQueue& kMgr) noexcept
					{
						m_spObject = ve_renderer_ptr->CreateRootSignature(m_spFileCache->GetData()->GetBlob());
						if (m_spObject)
						{
							kMgr.AddFGTask([this](VeTaskQueue& kMgr) noexcept
							{
								UpdateRootSignature();
								DecRefCount();
							});
						}
						else
						{
							DecRefCount();
						}
					});
				}
				else
				{
					VeTaskQueue& kQueue = ve_res_mgr.GetTaskQueue(VeResourceManager::TASK_PROCESS);
					kQueue.AddBGTask([this](VeTaskQueue& kMgr) noexcept
					{
						VeBlobPtr spBlob = ve_renderer_ptr->SerializeRootSignature(
							m_pcName, *m_pkValue, m_spCache->GetGroup());
						if (spBlob)
						{
							m_spObject = ve_renderer_ptr->CreateRootSignature(spBlob);
							if (m_spObject)
							{
								kMgr.AddFGTask([this](VeTaskQueue& kMgr) noexcept
								{
									UpdateRootSignature();
									DecRefCount();
								});
							}
							else
							{
								DecRefCount();
							}
						}
						else
						{
							DecRefCount();
						}
					});
					

				}
			});


		}
		else
		{
			ve_sys.CORE.E.LogFormat("Root Signature script error in [\"%s\"].",
				m_spCache->GetFullPath());
			DecRefCount();
		}
	});
}
//--------------------------------------------------------------------------
VeRootSignature::JSONParser::~JSONParser() noexcept
{

}
//--------------------------------------------------------------------------
void VeRootSignature::JSONParser::UpdateRootSignature() noexcept
{
	VE_ASSERT(m_spObject);
	VeRootSignaturePtr spRootSignature = ve_res_mgr.Get<VeRootSignature>(
		VeRootSignature::GetName(), m_pcName);
	VE_ASSERT(spRootSignature);
	spRootSignature->SetGroup(m_spCache->GetGroup());
	spRootSignature->LoadDirect(m_spObject);
}
//--------------------------------------------------------------------------
