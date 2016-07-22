////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      System
//  File name:   VeSharedLib.h
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

#pragma once

#if defined(BUILD_PLATFORM_WIN)
#	define VE_SHARED_LIB HMODULE
#	define VE_SHARED_LIB_NAME(name) (name##".dll")
#	define VE_SHARED_LIB_LOAD(name) LoadLibraryA(name)
#	define VE_SHARED_LIB_GET_PROC(lib,proc) GetProcAddress(lib,proc)
#	define VE_SHARED_LIB_UNLOAD(lib) (FreeLibrary(lib) ? VE_OK : VE_ELF)
#elif defined(BUILD_PLATFORM_LINUX)
#	define VE_SHARED_LIB void*
#	define VE_SHARED_LIB_NAME(name) (name##".so")
#	define VE_SHARED_LIB_LOAD(name) dlopen(name, RTLD_LAZY | RTLD_GLOBAL)
#	define VE_SHARED_LIB_GET_PROC(lib,proc) dlsym(lib,proc)
#	define VE_SHARED_LIB_UNLOAD(lib) dlclose(lib)
#elif defined(BUILD_PLATFORM_DARWIN)
#	define VE_SHARED_LIB CFBundleRef
#	define VE_SHARED_LIB_NAME(name) (name##".framework")
	CFBundleRef VE_SHARED_LIB_LOAD(const char* pcName);
	void* VE_SHARED_LIB_GET_PROC(CFBundleRef hLib, const char* pcProc);
	VeUInt32 VE_SHARED_LIB_UNLOAD(CFBundleRef hLib);
#endif

#ifdef VE_SHARED_LIB

class VENUS_API VeSharedLib : public VeRefObject
{
	VeNoCopy(VeSharedLib);
public:
	VeSharedLib(const char* pcLibName) noexcept;

	virtual ~VeSharedLib() noexcept;

	bool Load() noexcept;

	void Unload() noexcept;

	const char* GetName() noexcept;

	void* GetProc(const char* pcProcName) noexcept;

protected:
	vtd::string m_kName;
	VE_SHARED_LIB m_hSharedLib = 0;

};

VeSmartPointer(VeSharedLib);

#endif
