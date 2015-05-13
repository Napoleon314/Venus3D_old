////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeThread.inl
//  Created:     2015/02/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline void VeThread::Start() noexcept
{
	if (!m_u32State)
	{
		VE_AUTO_LOCK_MUTEX(m_kMutex);
		++m_u32State;
		m_pkParams->m_kEventLoop.Set();
		m_pkParams->m_kEvent.Reset();
	}
}
//--------------------------------------------------------------------------
inline bool VeThread::IsRunning() noexcept
{
	return m_u32State > 0;
}
//--------------------------------------------------------------------------
inline void VeThread::Wait(Event& kEvent) noexcept
{
	VeThreadEventWait(&(kEvent.m_kEvent));
}
//--------------------------------------------------------------------------
inline void VeThread::Wait(Event& kEvent, VeUInt32 u32Milliseconds) noexcept
{
	VeThreadEventWait(&(kEvent.m_kEvent), u32Milliseconds);
}
//--------------------------------------------------------------------------
inline void VeThread::Join(VeThread& kThread) noexcept
{
	Wait(kThread.m_pkParams->m_kEvent);
}
//--------------------------------------------------------------------------
