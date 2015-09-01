////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeResourceManager.inl
//  Created:     2015/03/10 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeTaskQueue& VeResourceManager::GetTaskQueue(Task eTask) noexcept
{
	VE_ASSERT(eTask < TASK_NUM);
	return m_akTaskQueues[eTask];
}
//--------------------------------------------------------------------------
inline void VeResourceManager::SetFileFreeTime(
	VeUInt32 u32TimeUInt) noexcept
{
	m_u32FileCacheFreeTime = u32TimeUInt;
}
//--------------------------------------------------------------------------
inline VeUInt32 VeResourceManager::GetFileFreeTime() noexcept
{
	return m_u32FileCacheFreeTime;
}
//--------------------------------------------------------------------------
inline const VeChar8*
VeResourceManager::FileCache::GetFullPath() const noexcept
{
	return m_kFullPath;
}
//--------------------------------------------------------------------------
inline const VeChar8*
VeResourceManager::FileCache::GetFileName() const noexcept
{
	return m_kFileName;
}
//--------------------------------------------------------------------------
inline const VeChar8*
VeResourceManager::FileCache::GetGroupName() const noexcept
{
	return m_spGroup ? m_spGroup->GetName() : "";
}
//--------------------------------------------------------------------------
inline const VeResourceGroupPtr&
VeResourceManager::FileCache::GetGroup() const noexcept
{
	return m_spGroup;
}
//--------------------------------------------------------------------------
inline const VeMemoryIStreamPtr&
VeResourceManager::FileCache::GetData() noexcept
{
	return m_spData;
}
//--------------------------------------------------------------------------
inline const VeChar8* VeResourceManager::JSONCache::GetFullPath() noexcept
{
	return m_kFullPath;
}
//--------------------------------------------------------------------------
inline const VeChar8* VeResourceManager::JSONCache::GetGroupName() noexcept
{
	return m_spGroup->GetName();
}
//--------------------------------------------------------------------------
inline const VeResourceGroupPtr& VeResourceManager::JSONCache::GetGroup() noexcept
{
	return m_spGroup;
}
//--------------------------------------------------------------------------
