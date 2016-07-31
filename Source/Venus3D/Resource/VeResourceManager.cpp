////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Resource
//  File name:   VeResourceManager.cpp
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

#include "stdafx.h"

//--------------------------------------------------------------------------
VeReadCache::VeReadCache() noexcept
{
	m_kNode._Content = this;
}
//--------------------------------------------------------------------------
VeReadCache::~VeReadCache() noexcept
{
	if (m_pvData)
	{
		VeFree(m_pvData);
	}
}
//--------------------------------------------------------------------------
VeResourceManager::VeResourceManager() noexcept
{

}
//--------------------------------------------------------------------------
VeResourceManager::~VeResourceManager() noexcept
{

}
//--------------------------------------------------------------------------
void VeResourceManager::SetDefaultArchSource(const char* pcName) noexcept
{
	if (pcName)
	{
		auto it = m_kArchSourceMap.find(pcName);
		if (it != m_kArchSourceMap.end())
		{
			m_kDefaultArchSource = *it;
		}
	}
	else
	{
		m_kDefaultArchSource = { nullptr,{ nullptr, nullptr } };
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::RegistArchSource(const char* pcName,
	DirCreator kDirCreator, ArchCreator kArchCreator) noexcept
{
	ArchSource& kArchSrc = m_kArchSourceMap[pcName];
	kArchSrc.first = std::move(kDirCreator);
	kArchSrc.second = std::move(kArchCreator);
}
//--------------------------------------------------------------------------
void VeResourceManager::UnregistArchSource(const char* pcName) noexcept
{
	auto it = m_kArchSourceMap.find(pcName);
	if (it != m_kArchSourceMap.end())
	{
		m_kArchSourceMap.erase(it);
	}
}
//--------------------------------------------------------------------------
void VeResourceManager::UnregistAllArchSource() noexcept
{
	m_kDefaultArchSource = { nullptr, { nullptr, nullptr } };
	m_kArchSourceMap.clear();
}
//--------------------------------------------------------------------------
VeDirectoryPtr VeResourceManager::OpenDirectory(const char* pcPath,
	bool bTryCreate) noexcept
{
	if (pcPath)
	{
		const char* pcSplit = strchr(pcPath, '#');
		if (pcSplit)
		{
			vtd::string kArchSrc(pcPath, pcSplit - pcPath);
			auto it = m_kArchSourceMap.find(kArchSrc);
			if (it != m_kArchSourceMap.end())
			{
				return it->second.first(pcSplit + 1, bTryCreate);
			}
		}
		else if (m_kDefaultArchSource.second.first)
		{
			return m_kDefaultArchSource.second.first(pcPath, bTryCreate);
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeArchivePtr VeResourceManager::OpenArchive(const char* pcPath,
	uint32_t u32Flags) noexcept
{
	if (pcPath)
	{
		const char* pcSplit = strchr(pcPath, '#');
		if (pcSplit)
		{
			vtd::string kArchSrc(pcPath, pcSplit - pcPath);
			auto it = m_kArchSourceMap.find(kArchSrc);
			if (it != m_kArchSourceMap.end())
			{
				return it->second.second(pcSplit + 1, u32Flags);
			}
		}
		else if (m_kDefaultArchSource.second.second)
		{
			return m_kDefaultArchSource.second.second(pcPath, u32Flags);
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------
VeBlobPtr VeResourceManager::LoadArchive(const char* pcPath) noexcept
{
	VeArchivePtr spArch = OpenArchive(pcPath, VE_ARCH_OPEN_READ);
	if (spArch && spArch->Seek(0, VE_SEEK_END))
	{
		size_t stLen = spArch->Tell();
		if (stLen && spArch->Seek(0, VE_SEEK_SET))
		{
			VeBlobPtr spRet = VE_NEW VeBlob(stLen);
			if (spArch->Read(spRet->data(), stLen) == stLen)
			{
				return spRet;
			}
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------
