////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeSystem.inl
//  Created:     2015/03/03 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeSystem::Type VeSystem::GetType() const noexcept
{
	return m_eType;
}
//--------------------------------------------------------------------------
inline const VeChar8* VeSystem::GetPakName() const noexcept
{
	return m_kPakName;
}
//--------------------------------------------------------------------------
inline VeLog& VeSystem::GetLog() noexcept
{
	return m_kLog;
}
//--------------------------------------------------------------------------
inline VeLua& VeSystem::GetLua() noexcept
{
	VE_ASSERT(m_spLua);
	return *m_spLua;
}
//--------------------------------------------------------------------------
inline void VeSystem::Collect(VeRefNode<VeRefObject*>& kNode) noexcept
{
	m_kObjCollector.attach_back(kNode);
}
//--------------------------------------------------------------------------
inline const VeStackAllocatorPtr& VeSystem::GetStackAllocator() noexcept
{
	return m_spMainStack;
}
//--------------------------------------------------------------------------
inline const VeTimePtr& VeSystem::GetTime() noexcept
{
	return m_spTime;
}
//--------------------------------------------------------------------------
