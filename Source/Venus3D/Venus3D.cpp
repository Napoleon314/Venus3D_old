////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   Venus3D.cpp
//  Created:     2016/07/06 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//--------------------------------------------------------------------------
Venus3D::Venus3D(const VeInitData& kInitData) noexcept
	: m_kInitData(kInitData), CORE(ENGINE_NAME, m_kLog)
	, USER(kInitData.m_pcAppName, m_kLog)
{
	Init();
}
//--------------------------------------------------------------------------
Venus3D::~Venus3D() noexcept
{
	Term();
}
//--------------------------------------------------------------------------
void Venus3D::Init()
{
	VE_MASK_CONDITION(m_kInitData.m_u32InitMask, VE_INIT_LOG, InitLog());
	VE_MASK_CONDITION(m_kInitData.m_u32InitMask, VE_INIT_JOB, InitJob());
	VE_MASK_CONDITION(m_kInitData.m_u32InitMask, VE_INIT_RES, InitResource());
	VE_MASK_CONDITION(m_kInitData.m_u32InitMask, VE_INIT_VIDEO, InitVideo());
	LoadConfig();
}
//--------------------------------------------------------------------------
void Venus3D::Term()
{
	VE_MASK_CONDITION(m_kInitData.m_u32InitMask, VE_INIT_VIDEO, TermVideo());
	VE_MASK_CONDITION(m_kInitData.m_u32InitMask, VE_INIT_RES, TermResource());
	VE_MASK_CONDITION(m_kInitData.m_u32InitMask, VE_INIT_JOB, TermJob());
	VE_MASK_CONDITION(m_kInitData.m_u32InitMask, VE_INIT_LOG, TermLog());
}
//--------------------------------------------------------------------------
void Venus3D::InitLog() noexcept
{
	m_kLog.SetTarget(&VeLog::ConsoleOutput);
}
//--------------------------------------------------------------------------
void Venus3D::TermLog() noexcept
{
	m_kLog.SetTarget(nullptr);
}
//--------------------------------------------------------------------------
void Venus3D::InitJob() noexcept
{
	uint32_t u32CPUNum = VeThreadHardwareConcurrency();
	VeJobSystem::Create(u32CPUNum, VE_CLAMP(u32CPUNum >> 1, 1, VE_JOB_BG_BUFFER_MASK + 1));
}
//--------------------------------------------------------------------------
void Venus3D::TermJob() noexcept
{
	VeJobSystem::Destory();
}
//--------------------------------------------------------------------------
void Venus3D::InitResource() noexcept
{
	VeResourceManager::Create();
	ve_res_mgr.RegistArchClasses<VeFileDir>();
}
//--------------------------------------------------------------------------
void Venus3D::TermResource() noexcept
{
	ve_res_mgr.UnregistArchClasses<VeFileDir>();
	VeResourceManager::Destory();
}
//--------------------------------------------------------------------------
#ifdef BUILD_PLATFORM_WIN
extern VeVideoPtr CreateWindowsVideo() noexcept;
#else
VeVideoPtr CreateVideo() noexcept
{
    return nullptr;
}
#endif
//--------------------------------------------------------------------------
void Venus3D::InitVideo() noexcept
{
#	ifdef BUILD_PLATFORM_WIN
	m_spVideo = CreateWindowsVideo();
#   else
    m_spVideo = CreateVideo();
#	endif
	if (m_spVideo)
	{
		VE_TRY_CALL(m_spVideo->Init());
	}
}
//--------------------------------------------------------------------------
void Venus3D::TermVideo() noexcept
{
	if (m_spVideo)
	{
		VE_TRY_CALL(m_spVideo->Term());
	}
	m_spVideo = nullptr;
}
//--------------------------------------------------------------------------
void Venus3D::LoadConfig() noexcept
{
	VeBlobPtr spBlob = ve_res_mgr.LoadArchive(m_kInitData.m_pcConfigFile);
	if (!spBlob) return;
	rapidxml::xml_document<char> doc;
	doc.parse<0>((char*)(spBlob->data()));
	auto root = doc.first_node();
	if (!root) return;
	{
		auto cfg = root->first_node(VE_STR_AND_LEN("cfg"));
		if (cfg)
		{
			auto iter = cfg->first_node();
			while (iter)
			{
				m_kConfig[iter->name()] = iter->value();
				iter = iter->next_sibling();
			}
		}
	}
	{
		auto res = root->first_node(VE_STR_AND_LEN("res"));
		if (res)
		{
			auto iter = res->first_node();
			while (iter)
			{
				VeDirectoryPtr spDir = ve_res_mgr.OpenDirectory(iter->value(), true);
				if (spDir)
				{
					ve_res_mgr.RegistArchSource(iter->name(),
						[spDir](const char* pcPath, bool bTryCreate) noexcept
					{
						return spDir->OpenSubdir(pcPath, bTryCreate);
					}, [spDir](const char* pcPath, uint32_t u32Flags) noexcept
					{
						return spDir->OpenArchive(pcPath, u32Flags);
					});
				}
				iter = iter->next_sibling();
			}
		}
	}
}
//--------------------------------------------------------------------------
VeStackAllocator& Venus3D::GetStackAllocator() noexcept
{
	return VeThread::GetThreadLocalSingleton()->m_kAllocator;
}
//--------------------------------------------------------------------------
const VePoolAllocatorPtr& Venus3D::GetPoolAllocator(
	size_t stUnitSize) noexcept
{
	stUnitSize = (stUnitSize + 0xF) & (~0xF);
	std::lock_guard<vtd::spin_lock> l(m_kAllocatorLock);
	VePoolAllocatorPtr& spRes = m_kAllocatorMap[stUnitSize];
	if (!spRes)
	{
		spRes = VE_NEW VePoolAllocator(stUnitSize);
	}
	return spRes;
}
//--------------------------------------------------------------------------
