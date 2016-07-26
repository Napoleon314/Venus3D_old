////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      System
//  File name:   VeSharedLib.cpp
//  Created:     2016/07/22 by Albert
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
#ifdef VE_SHARED_LIB
//--------------------------------------------------------------------------
VeSharedLib::VeSharedLib(const char* pcLibName) noexcept
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
		m_hSharedLib = nullptr;
	}
}
//--------------------------------------------------------------------------
const char* VeSharedLib::GetName() noexcept
{
	return m_kName;
}
//--------------------------------------------------------------------------
void* VeSharedLib::GetProc(const char* pcProcName) noexcept
{
	return (void*)VE_SHARED_LIB_GET_PROC(m_hSharedLib, pcProcName);
}
//--------------------------------------------------------------------------
#ifdef BUILD_PLATFORM_DARWIN
//--------------------------------------------------------------------------
CFBundleRef VE_SHARED_LIB_LOAD(const char* pcName)
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
void* VE_SHARED_LIB_GET_PROC(CFBundleRef hLib, const char* pcProc)
{
	CFStringRef hProc = CFStringCreateWithCString(kCFAllocatorDefault, pcProc, kCFStringEncodingASCII);
	void* pvRet = CFBundleGetFunctionPointerForName(hLib, hProc);
	CFRelease(hProc);
	return pvRet;
}
//--------------------------------------------------------------------------
uint32_t VE_SHARED_LIB_UNLOAD(CFBundleRef hLib)
{
	CFRelease(hLib);
	return VE_OK;
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
