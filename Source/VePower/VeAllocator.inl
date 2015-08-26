////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeAllocator.inl
//  Created:     2015/03/04 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeSizeT VeStackAllocator::Space() noexcept
{
	return m_stSize;
}
//--------------------------------------------------------------------------
inline VeSizeT VeStackAllocator::RemainSpace() noexcept
{
	VE_LOCK_MUTEX(m_kLock);
	return m_stSize - (m_pbyCurrent - m_pbyBuffer);
}
//--------------------------------------------------------------------------
inline VeSizeT VePoolAllocator::GetUnitSize() const noexcept
{
	return m_stUnitSize;
}
//--------------------------------------------------------------------------
inline VeSizeT VePoolAllocator::GetUnitPerChunk() const noexcept
{
	return m_stUnitPerChunk;
}
//--------------------------------------------------------------------------
inline VeSizeT VePoolAllocator::GetAlign() const noexcept
{
	return m_stAlign;
}
//--------------------------------------------------------------------------
inline void* VePoolAllocator::Allocate() noexcept
{
	VE_LOCK_MUTEX(m_kLock);
	return _Allocate();
}
//--------------------------------------------------------------------------
inline void VePoolAllocator::Deallocate(void* pvMemory) noexcept
{
	VE_LOCK_MUTEX(m_kLock);
	return _Deallocate(pvMemory);
}
//--------------------------------------------------------------------------
