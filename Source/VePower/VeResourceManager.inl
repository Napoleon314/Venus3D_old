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
