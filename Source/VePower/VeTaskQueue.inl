////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeBGQueue.inl
//  Created:     2015/03/06 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline void VeTaskQueue::SetTickOnce(bool bEnable) noexcept
{
	m_bTickOnce = bEnable;
}
//--------------------------------------------------------------------------
inline bool VeTaskQueue::IsTickOnce() noexcept
{
	return m_bTickOnce;
}
//--------------------------------------------------------------------------
