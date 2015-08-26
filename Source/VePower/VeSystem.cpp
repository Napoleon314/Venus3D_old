////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeSystem.cpp
//  Created:     2015/03/03 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
static const VeChar8* s_apcLogTypeNames[VeLog::TYPE_MAX] =
{
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR"
};
//--------------------------------------------------------------------------
static void OutputDebug(VeLog::Type eType, const VeChar8* pcTag,
	const VeChar8* pcText) noexcept
{
#ifdef VE_PLATFORM_ANDROID

	const android_LogPriority aeLogTypeMap[VeLog::TYPE_MAX] =
	{
		ANDROID_LOG_DEBUG,
		ANDROID_LOG_INFO,
		ANDROID_LOG_WARN,
		ANDROID_LOG_ERROR
	};
	__android_log_print(aeLogTypeMap[eType], pcTag, "%s", pcText);

#else

#	ifdef VE_PLATFORM_WIN
	VeChar8 acLogBuffer[VE_LOG_BUFFER_SIZE + 64];
	VeSprintf(acLogBuffer, "%s,%s: %s\n", s_apcLogTypeNames[eType], pcTag, pcText);
	OutputDebugStringA(acLogBuffer);
#	else
	printf("%s,%s: %s\n", s_apcLogTypeNames[eType], pcTag, pcText);
#	endif

#endif
}
//--------------------------------------------------------------------------
static void OutputConsole(VeLog::Type eType, const VeChar8* pcTag,
	const VeChar8* pcText) noexcept
{
#ifdef VE_PLATFORM_ANDROID

	const android_LogPriority aeLogTypeMap[VeLog::TYPE_MAX] =
	{
		ANDROID_LOG_DEBUG,
		ANDROID_LOG_INFO,
		ANDROID_LOG_WARN,
		ANDROID_LOG_ERROR
	};
	__android_log_print(aeLogTypeMap[eType], pcTag, "%s", pcText);

#else

	printf("%s,%s: %s\n", s_apcLogTypeNames[eType], pcTag, pcText);

#endif
}
//--------------------------------------------------------------------------
VeSystem::VeSystem(Type eType, const VeChar8* pcPakName) noexcept
	: m_eType(eType), m_kPakName(pcPakName), CORE("Venus3D", m_kLog)
	, USER(m_kPakName, m_kLog)
{
	m_spMainStack = VE_NEW VeStackAllocator(VE_STACK_SIZE);
	m_spTime = VE_NEW VeTime();
	VeResourceManager::Create();
}
//--------------------------------------------------------------------------
VeSystem::~VeSystem() noexcept
{
	VeResourceManager::Destory();
	m_spTime = nullptr;
	for (auto obj : m_kObjCollector)
	{
		obj->DecRefCount();
	}
	m_spMainStack = nullptr;
}
//--------------------------------------------------------------------------
const VePoolAllocatorPtr& VeSystem::GetPoolAllocator(
	VeSizeT stUnitSize) noexcept
{
	stUnitSize = (stUnitSize + 0xF) & (~0xF);
	VePoolAllocatorPtr& spRes = m_kAllocatorMap[stUnitSize];
	if (!spRes)
	{
		spRes = VE_NEW VePoolAllocator(stUnitSize);
	}
	return spRes;
}
//--------------------------------------------------------------------------
void VeSystem::Init() noexcept
{
	InitLog();
	InitResMgr();
}
//--------------------------------------------------------------------------
void VeSystem::Term() noexcept
{
	TermResMgr();
	TermLog();
}
//--------------------------------------------------------------------------
void VeSystem::Update() noexcept
{
	ve_time.Update();
	ve_res_mgr.Update();
}
//--------------------------------------------------------------------------
void VeSystem::InitLog() noexcept
{
	switch (m_eType)
	{
	case TYPE_DEFAULT:
		m_kLog.SetTarget(&OutputDebug);
		break;
	case TYPE_CONSOLE:
		m_kLog.SetTarget(&OutputConsole);
		break;
	default:
		break;
	}
}
//--------------------------------------------------------------------------
void VeSystem::TermLog() noexcept
{
	m_kLog.SetTarget(nullptr);
}
//--------------------------------------------------------------------------
void VeSystem::InitResMgr() noexcept
{
#	ifdef VE_PLATFORM_ANDROID
	ve_res_mgr.RegistDirectory<VeFilePath>();
	ve_res_mgr.RegistDirectory<VeAssetPath>(true);
#	else
	ve_res_mgr.RegistDirectory<VeFilePath>(true);
#	endif
	ve_res_mgr.RegistFileCreator(VE_JSON_EXT,
		[this](VeResourceManager::FileCachePtr spCache) noexcept
	{
		ve_res_mgr.ParseJSON(spCache);
	});
}
//--------------------------------------------------------------------------
void VeSystem::TermResMgr() noexcept
{
	ve_res_mgr.UnregistFileCreator(VE_JSON_EXT);
	ve_res_mgr.SetDefaultDirCreator(nullptr);
	ve_res_mgr.SetDefaultStreamCreator(nullptr);
#	ifdef VE_PLATFORM_ANDROID
	ve_res_mgr.UnregistDirectory<VeFilePath>();
	ve_res_mgr.UnregistDirectory<VeAssetPath>();
#	else
	ve_res_mgr.UnregistDirectory<VeFilePath>();
#	endif
}
//--------------------------------------------------------------------------
