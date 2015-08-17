////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeSharedLib.h
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#if defined(VE_PLATFORM_WIN)
#	define VE_SHARED_LIB HMODULE
#	define VE_SHARED_LIB_NAME(name) (name##".dll")
#	define VE_SHARED_LIB_LOAD(name) LoadLibraryA(name)
#	define VE_SHARED_LIB_GET_PROC(lib,proc) GetProcAddress(lib,proc)
#	define VE_SHARED_LIB_UNLOAD(lib) (FreeLibrary(lib) ? VE_OK : VE_ELF)
#elif defined(VE_PLATFORM_LINUX)
#	define VE_SHARED_LIB void*
#	define VE_SHARED_LIB_NAME(name) (name##".so")
#	define VE_SHARED_LIB_LOAD(name) dlopen(name, RTLD_LAZY | RTLD_GLOBAL)
#	define VE_SHARED_LIB_GET_PROC(lib,proc) dlsym(lib,proc)
#	define VE_SHARED_LIB_UNLOAD(lib) dlclose(lib)
#elif defined(VE_PLATFORM_OSX)
#	define VE_SHARED_LIB CFBundleRef
#	define VE_SHARED_LIB_NAME(name) (name##".framework")
	CFBundleRef VE_SHARED_LIB_LOAD(const VeChar8* pcName);
	void* VE_SHARED_LIB_GET_PROC(CFBundleRef hLib, const VeChar8* pcProc);
	VeUInt32 VE_SHARED_LIB_UNLOAD(CFBundleRef hLib);
#endif

#ifdef VE_SHARED_LIB

class VE_POWER_API VeSharedLib : public VeRefObject
{
	VE_NO_COPY(VeSharedLib);
public:
	VeSharedLib(const VeChar8* pcLibName) noexcept;

	virtual ~VeSharedLib() noexcept;

	bool Load() noexcept;

	void Unload() noexcept;

	const VeChar8* GetName() noexcept;

	void* GetProc(const VeChar8* pcProcName) noexcept;

protected:
	VeFixedString m_kName;
	VE_SHARED_LIB m_hSharedLib = 0;

};

VeSmartPointer(VeSharedLib);

#endif
