////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VePipelineState.cpp
//  Created:     2015/09/01 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeMainPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VePipelineState, VeResource);
//--------------------------------------------------------------------------
VePipelineState::VePipelineState(const VeChar8* pcName,
	const VeChar8* pcFullName) noexcept
{
	m_kName = pcName;
	m_kFullName = pcFullName;
}
//--------------------------------------------------------------------------
VePipelineState::~VePipelineState() noexcept
{

}
//--------------------------------------------------------------------------
const VeChar8* VePipelineState::GetTypeName() const noexcept
{
	return VePipelineState::GetName();
}
//--------------------------------------------------------------------------
void VePipelineState::LoadDirect(
	const VeRenderer::PipelineStatePtr& spObj) noexcept
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
void VePipelineState::Clear() noexcept
{
	m_spObject = nullptr;
}
//--------------------------------------------------------------------------
void VePipelineState::Regist() noexcept
{
	ve_res_mgr.RegistResCreator(VePipelineState::GetName(), [](
		const VeChar8* pcName, const VeChar8* pcFullName) noexcept
	{
		return VE_NEW VePipelineState(pcName, pcFullName);
	});
	ve_res_mgr.RegistJSONCreator(VePipelineState::GetName(),
		[](VeResourceManager::JSONCachePtr spCache,
			VeJSONValue& kValue) noexcept
	{
		
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
void VePipelineState::Unregist() noexcept
{
	ve_res_mgr.UnregistJSONCreator(VeRootSignature::GetName());
	ve_res_mgr.UnregistResCreator(VeRootSignature::GetName());
}
//--------------------------------------------------------------------------
void VePipelineState::Parse(const VeResourceManager::FileCachePtr& spCache,
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
		VeRenderer::PipelineStatePtr spObj = ve_renderer_ptr->CreatePipelineState(*pkValue);
		if (!spObj) return;
		VePipelineStatePtr spResource = ve_res_mgr.Get<VePipelineState>(
			VePipelineState::GetName(), pcName);
		spResource->SetGroup(spCache->GetGroup());
		spResource->LoadDirect(spObj);
	}
	else
	{
		ve_sys.CORE.E.LogFormat("Pipeline State script error in [\"%s\"].",
			spCache->GetFullPath());
	}
}
//--------------------------------------------------------------------------
