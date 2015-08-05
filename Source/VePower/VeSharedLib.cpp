////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeSharedLib.cpp
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
#ifdef VE_SHARED_LIB
//--------------------------------------------------------------------------
VeSharedLib::VeSharedLib(const VeChar8* pcLibName) noexcept
	: m_kName(pcLibName)
{

}
//--------------------------------------------------------------------------
VeSharedLib::~VeSharedLib() noexcept
{
	if (m_hSharedLib)
	{
		Unload();
	}
	VE_ASSERT(!m_hSharedLib);
}
//--------------------------------------------------------------------------
bool VeSharedLib::Load() noexcept
{
	if (!m_hSharedLib)
	{
		m_hSharedLib = VE_SHARED_LIB_LOAD(m_kName);
	}
	return m_hSharedLib ? true : false;
}
//--------------------------------------------------------------------------
void VeSharedLib::Unload() noexcept
{
	if (m_hSharedLib)
	{
		VE_ASSERT_EQ(VE_SHARED_LIB_UNLOAD(m_hSharedLib), VE_OK);
		m_hSharedLib = NULL;
	}
}
//--------------------------------------------------------------------------
const VeChar8* VeSharedLib::GetName() noexcept
{
	return m_kName;
}
//--------------------------------------------------------------------------
void* VeSharedLib::GetProc(const VeChar8* pcProcName) noexcept
{
	return (void*)VE_SHARED_LIB_GET_PROC(m_hSharedLib, pcProcName);
}
//--------------------------------------------------------------------------
#ifdef VE_PLATFORM_OSX
//--------------------------------------------------------------------------
CFBundleRef VE_SHARED_LIB_LOAD(const VeChar8* pcName)
{
	CFStringRef hName = CFStringCreateWithCString(kCFAllocatorDefault, pcName, kCFStringEncodingASCII);
	CFURLRef hURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, hName, kCFURLPOSIXPathStyle, true);
	CFBundleRef hLib = CFBundleCreate(kCFAllocatorDefault, hURL);
	VE_ASSERT(hLib);
	CFRelease(hURL);
	CFRelease(hName);
	return hLib;
}
//--------------------------------------------------------------------------
void* VE_SHARED_LIB_GET_PROC(CFBundleRef hLib, const VeChar8* pcProc)
{
	CFStringRef hProc = CFStringCreateWithCString(kCFAllocatorDefault, pcProc, kCFStringEncodingASCII);
	void* pvRet = CFBundleGetFunctionPointerForName(hLib, hProc);
	CFRelease(hProc);
	return pvRet;
}
//--------------------------------------------------------------------------
VeUInt32 VE_SHARED_LIB_UNLOAD(CFBundleRef hLib)
{
	CFRelease(hLib);
	return VE_OK;
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
