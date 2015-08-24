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
