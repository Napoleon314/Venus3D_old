////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Resource
//  File name:   VeResourceManager.h
//  Created:     2016/07/31 by Albert
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

class VENUS_API VeReadCache : public VeMemObject
{
public:
	~VeReadCache() noexcept;

private:
	friend class VeResourceManager;
	VeReadCache() noexcept;

	vtd::intrusive_node<VeReadCache*> m_kNode;
	//uint32_t m_u32FreeTime;
	//std::atomic<uint32_t> m_u32RefCount;
	vtd::string m_kFullPath;
	vtd::string m_kExt;
	void* m_pvData = nullptr;
	//size_t m_stSize = 0;

};

class VENUS_API VeResourceManager : public VeSingleton<VeResourceManager>
{
	VeNoCopy(VeResourceManager);
public:
	template<class... _Types>
	struct unpacker;

	template<class _This>
	struct unpacker<_This>
	{
		static void reg_arch_src(VeResourceManager&) noexcept {}

		static void unreg_arch_src(VeResourceManager&) noexcept {}
	};

	template<class _This, class... _Rest>
	struct unpacker<_This, _Rest...>
	{
		static void reg_arch_src(VeResourceManager& mgr) noexcept
		{
			mgr.RegistArchSource(_This::Name(), &_This::Create, &_This::Open);
			unpacker<_Rest...>::reg_arch_src(mgr);
		}

		static void unreg_arch_src(VeResourceManager& mgr) noexcept
		{
			mgr.UnregistArchSource(_This::Name());
			unpacker<_Rest...>::unreg_arch_src(mgr);
		}
	};

	typedef std::function<VeDirectoryPtr(const char*, bool)> DirCreator;

	typedef std::function<VeArchivePtr(const char*, uint32_t)> ArchCreator;

	typedef std::pair<DirCreator, ArchCreator> ArchSource;

	VeResourceManager() noexcept;

	virtual ~VeResourceManager() noexcept;

	void SetDefaultArchSource(const char* pcName) noexcept;

	void RegistArchSource(const char* pcName, DirCreator kDirCreator, ArchCreator kArchCreator) noexcept;

	void UnregistArchSource(const char* pcName) noexcept;

	void UnregistAllArchSource() noexcept;

	template <class _This, class... _Rest>
	void RegistArchClasses() noexcept
	{
		unpacker<_This, _Rest...>::reg_arch_src(*this);
		SetDefaultArchSource(_This::Name());
	}

	template <class _This, class... _Rest>
	void UnregistArchClasses() noexcept
	{
		SetDefaultArchSource(nullptr);
		unpacker<_This, _Rest...>::unreg_arch_src(*this);
	}

	VeDirectoryPtr OpenDirectory(const char* pcPath, bool bTryCreate = false) noexcept;

	VeArchivePtr OpenArchive(const char* pcPath, uint32_t u32Flags = VE_ARCH_OPEN_READ) noexcept;

	VeBlobPtr LoadArchive(const char* pcPath) noexcept;

private:
	friend class VeReadCache;
	std::pair<vtd::string, ArchSource> m_kDefaultArchSource;
	VeStringMap<ArchSource> m_kArchSourceMap;

	vtd::spin_lock m_kReadCacheLock;
	VeStringMap<VeReadCache*> m_kReadCacheMap;
	vtd::intrusive_list<VeReadCache*> m_kReadingCache;
	VeStringMap<std::pair<uint32_t, vtd::intrusive_list<VeReadCache*>>> m_kSpecificFree;
	vtd::intrusive_list<VeReadCache*> m_kGeneralFree;
	//uint32_t m_u32ReadCacheRemaining;

};

#define ve_res_mgr VeResourceManager::Ref()

#include "VeResourceManager.inl"
