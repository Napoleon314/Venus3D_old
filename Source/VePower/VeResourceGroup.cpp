////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeResourceGroup.cpp
//  Created:     2015/08/23 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeResourceGroup::VeResourceGroup(const VeChar8* pcName) noexcept
	: m_kName(pcName)
{

}
//--------------------------------------------------------------------------
VeResourceGroup::~VeResourceGroup() noexcept
{

}
//--------------------------------------------------------------------------
void VeResourceGroup::PushReadDirBack(const VeDirectoryPtr& spDir) noexcept
{
	VE_ASSERT(spDir && spDir->Access(VeDirectory::ACCESS_R_OK));
	m_kReadDirList.push_back(spDir);
}
//--------------------------------------------------------------------------
void VeResourceGroup::PushReadDirFront(const VeDirectoryPtr& spDir) noexcept
{
	VE_ASSERT(spDir && spDir->Access(VeDirectory::ACCESS_R_OK));
	m_kReadDirList.push_front(spDir);
}
//--------------------------------------------------------------------------
void VeResourceGroup::PopReadDirBack() noexcept
{
	m_kReadDirList.pop_back();
}
//--------------------------------------------------------------------------
void VeResourceGroup::PopReadDirFront() noexcept
{
	m_kReadDirList.pop_front();
}
//--------------------------------------------------------------------------
void VeResourceGroup::SetWriteDirectory(const VeDirectoryPtr& spDir) noexcept
{
	VE_ASSERT(spDir && spDir->Access(VeDirectory::ACCESS_W_OK));
	m_spWriteDir = spDir;
}
//--------------------------------------------------------------------------
void VeResourceGroup::Clear() noexcept
{
	m_kReadDirList.clear();
	m_spWriteDir = nullptr;
}
//--------------------------------------------------------------------------
