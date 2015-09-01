////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeShader.cpp
//  Created:     2015/08/25 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeShader, VeResource);
//--------------------------------------------------------------------------
VeShader::VeShader(Type eType, const VeChar8* pcName,
	const VeChar8* pcFullName) noexcept
	: m_eType(eType)
{
	m_kName = pcName;
	m_kFullName = pcFullName;
}
//--------------------------------------------------------------------------
VeShader::~VeShader() noexcept
{

}
//--------------------------------------------------------------------------
const VeChar8* VeShader::GetTypeName() const noexcept
{
	return GetTypeName(m_eType);
}
//--------------------------------------------------------------------------
void VeShader::LoadDirect(const VeBlobPtr& spBlob) noexcept
{
	if (m_eState == UNLOADED)
	{
		m_spBlob = spBlob;
		OnResLoaded();
	}
	else if (m_eState == LOADED)
	{
		m_spBlob = spBlob;
		OnResUpdated();
	}
}
//--------------------------------------------------------------------------
void VeShader::ParseStream(const VeMemoryIStreamPtr& spStream) noexcept
{
	m_spBlob = spStream->GetBlob();
}
//--------------------------------------------------------------------------
void VeShader::Clear() noexcept
{
	m_spBlob = nullptr;
}
//--------------------------------------------------------------------------
void VeShader::Regist() noexcept
{
	for (VeUInt32 i(0); i < TYPE_NUM; ++i)
	{
		Type eType = (Type)i;
		ve_res_mgr.RegistResCreator(GetTypeName(eType), [eType](
			const VeChar8* pcName, const VeChar8* pcFullName) noexcept
		{
			return VE_NEW VeShader(eType, pcName, pcFullName);
		});
	}

	ve_res_mgr.RegistJSONCreator("shader",
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
void VeShader::Unregist() noexcept
{
	ve_res_mgr.UnregistJSONCreator("shader");
	for (VeUInt32 i(0); i < TYPE_NUM; ++i)
	{
		Type eType = (Type)i;
		ve_res_mgr.UnregistResCreator(GetTypeName(eType));
	}
}
//--------------------------------------------------------------------------
void VeShader::Parse(const VeResourceManager::FileCachePtr& spCache,
	VeJSONValue& lkValue) noexcept
{
	VeJSONValue* pkValue = nullptr;
	const VeChar8* pcName = nullptr;
	const VeChar8* pcFile = nullptr;
	const VeChar8* pcTarget = nullptr;
	VeShader::Type eType = TYPE_NUM;
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
			auto f = it->FindMember("file");
			if (f != it->MemberEnd() && f->value.IsString())
			{
				pcFile = f->value.GetString();
			}
			auto t = it->FindMember("target");
			if (t != it->MemberEnd() && t->value.IsString())
			{
				pcTarget = t->value.GetString();
				eType = ve_renderer_ptr->GetShaderType(pcTarget);
			}
			if (pcName && pcFile && pcTarget && eType < TYPE_NUM) pkValue = &(*it);
			break;
		}
	}
	if (pkValue)
	{
		VE_ASSERT(pcName && pcFile && pcTarget);
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeSprintf(acBuffer, "%s.%so", pcName, VeShader::GetTypeName(eType));
		VeResourceManager::FileCachePtr spFile = ve_res_mgr.CacheFile(acBuffer, spCache->GetGroup());
		if (spFile)
		{
			VeShaderPtr spShader = ve_res_mgr.Get<VeShader>(
				VeShader::GetTypeName(eType), pcName);
			spShader->SetGroup(spCache->GetGroup());
			spShader->LoadDirect(spCache->GetData()->GetBlob());
		}
		else
		{
			spFile = ve_res_mgr.CacheFile(pcFile, spCache->GetGroup());
			if (spFile)
			{
				VeBlobPtr spBlob = ve_renderer_ptr->CompileShader(
					pcName, pcTarget, spCache->GetFullPath(), *pkValue, spFile);
				if (spBlob)
				{
					VeShaderPtr spShader = ve_res_mgr.Get<VeShader>(
						VeShader::GetTypeName(eType), pcName);
					spShader->SetGroup(spCache->GetGroup());
					spShader->LoadDirect(spBlob);
				}				
			}
		}
	}
	else
	{
		ve_sys.CORE.E.LogFormat("Shader script error in [\"%s\"].",
			spCache->GetFullPath());
	}
}
//--------------------------------------------------------------------------
VeShader::JSONParser::JSONParser(
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
				auto f = it->FindMember("file");
				if (f != it->MemberEnd() && f->value.IsString())
				{
					m_pcFile = f->value.GetString();
				}
				auto t = it->FindMember("target");
				if (t != it->MemberEnd() && t->value.IsString())
				{
					m_pcTarget = t->value.GetString();
					m_eType = ve_renderer_ptr->GetShaderType(m_pcTarget);
				}
				if (m_pcName && m_pcFile && m_pcTarget && m_eType < TYPE_NUM) m_pkValue = &(*it);
				break;
			}
		}
		if (m_pkValue)
		{
			VE_ASSERT(m_pcName && m_pcFile && m_pcTarget);
			VeChar8 acBuffer[VE_MAX_PATH_LEN];
			VeSprintf(acBuffer, "%s$%s.%so", m_spCache->GetGroupName(),
				m_pcName, VeShader::GetTypeName(m_eType));
			ve_res_mgr.CacheFile(acBuffer, [this](VeResourceManager::FileCachePtr spFile) noexcept
			{
				if (spFile)
				{
					m_spBlob = spFile->GetData()->GetBlob();
					m_spGroup = spFile->GetGroup();
					UpdateShader();
					DecRefCount();
				}
				else
				{
					VeChar8 acBuffer[VE_MAX_PATH_LEN];
					VeSprintf(acBuffer, "%s$%s", m_spCache->GetGroupName(), m_pcFile);
					ve_res_mgr.CacheFile(acBuffer, [this](VeResourceManager::FileCachePtr spFile) noexcept
					{
						if (!spFile)
						{
							DecRefCount();
							return;
						}
						m_spFileCache = spFile;
						m_spFileCache->Enter();
						VeTaskQueue& kQueue = ve_res_mgr.GetTaskQueue(VeResourceManager::TASK_PROCESS);
						kQueue.AddBGTask([this](VeTaskQueue& kMgr) noexcept
						{
							m_spBlob = ve_renderer_ptr->CompileShader(
								m_pcName, m_pcTarget, m_spCache->GetFullPath(), *m_pkValue, m_spFileCache);
							if (m_spBlob)
							{
								m_spGroup = m_spFileCache->GetGroup();
								kMgr.AddFGTask([this](VeTaskQueue& kMgr) noexcept
								{
									m_spFileCache->Leave();
									m_spFileCache = nullptr;
									if (m_spBlob)
									{
										UpdateShader();
									}
									DecRefCount();
								});
							}							
						});
					});
				}
			});
		}
		else
		{
			ve_sys.CORE.E.LogFormat("Shader script error in [\"%s\"].",
				m_spCache->GetFullPath());
			DecRefCount();
		}
	});
}
//--------------------------------------------------------------------------
VeShader::JSONParser::~JSONParser() noexcept
{

}
//--------------------------------------------------------------------------
void VeShader::JSONParser::UpdateShader() noexcept
{
	VE_ASSERT(m_spBlob && m_spGroup);
	VeShaderPtr spShader = ve_res_mgr.Get<VeShader>(
		VeShader::GetTypeName(m_eType), m_pcName);
	VE_ASSERT(spShader);
	spShader->SetGroup(m_spGroup);
	spShader->LoadDirect(m_spBlob);
}
//--------------------------------------------------------------------------
