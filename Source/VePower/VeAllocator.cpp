////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeAllocator.cpp
//  Created:     2015/03/03 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeStackAllocator::VeStackAllocator(VeSizeT stSize) noexcept
	: m_stSize(stSize)
{
	m_pbyBuffer = (VeByte*)VeAlignedMalloc(stSize, 16);
	m_pbyCurrent = m_pbyBuffer;
}
//--------------------------------------------------------------------------
VeStackAllocator::~VeStackAllocator() noexcept
{
	VE_ASSERT(m_pbyBuffer == m_pbyCurrent && m_kStack.empty());
	VeAlignedFree(m_pbyBuffer);
	m_pbyBuffer = nullptr;
	m_pbyCurrent = nullptr;
}
//--------------------------------------------------------------------------
void* VeStackAllocator::Allocate(VeSizeT stSizeInBytes) noexcept
{
	stSizeInBytes = (stSizeInBytes + 0xF) & 0xFFFFFFF0;
	void* pvRes = m_pbyCurrent;
	m_pbyCurrent += stSizeInBytes;
	m_kStack.push(stSizeInBytes);
	return pvRes;
}
//--------------------------------------------------------------------------
void VeStackAllocator::Deallocate() noexcept
{
	m_pbyCurrent -= m_kStack.pop();
}
//--------------------------------------------------------------------------
VePoolAllocator::VePoolAllocator(VeSizeT stUnitSize, VeSizeT stUnitPerChunk,
	VeSizeT stAlign) noexcept : m_stUnitSize(stUnitSize)
	, m_stUnitPerChunk(stUnitPerChunk), m_stAlign(stAlign)
{
	VE_ASSERT(stUnitSize >= sizeof(VeSizeT) && stUnitSize >= stAlign
		&& stUnitPerChunk && stAlign);
	_AddChunk();
}
//--------------------------------------------------------------------------
VePoolAllocator::~VePoolAllocator() noexcept
{
#	ifdef VE_MEM_DEBUG
	VE_ASSERT(!m_u32UnitCount);
#	endif
	while (m_pkChunkHead)
	{
		Chunk* pkPre = m_pkChunkHead->m_pvPre;
		VeAlignedFree((void*)(VeSizeT(m_pkChunkHead->m_pvEnd) -
			m_stUnitSize * m_stUnitPerChunk));
		VeFree(m_pkChunkHead);
		m_pkChunkHead = pkPre;
	}
}
//--------------------------------------------------------------------------
void VePoolAllocator::_AddChunk() noexcept
{
#	ifdef VE_MEM_DEBUG
	++m_u32ChunkCount;
#	endif
	Chunk* pkChunk = VeAlloc(Chunk, 1);
	pkChunk->m_pvFreeStart = VeAlignedMalloc(
		m_stUnitSize * m_stUnitPerChunk, m_stAlign);
	pkChunk->m_pvEnd = (void*)(VeSizeT(pkChunk->m_pvFreeStart) +
		m_stUnitSize * m_stUnitPerChunk);
	pkChunk->m_pvPre = m_pkChunkHead;
	m_pkChunkHead = pkChunk;
}
//--------------------------------------------------------------------------
void* VePoolAllocator::_Allocate() noexcept
{
	VE_ASSERT(m_pkChunkHead);
#	ifdef VE_MEM_DEBUG
	++m_u32UnitCount;
#	endif
	while (true)
	{
		if (m_pkChunkHead->m_pvFreeStart < m_pkChunkHead->m_pvEnd)
		{
			void* pvRes = m_pkChunkHead->m_pvFreeStart;
			m_pkChunkHead->m_pvFreeStart =
				(void*)(VeSizeT(m_pkChunkHead->m_pvFreeStart) + m_stUnitSize);
			return pvRes;
		}

		if (m_pvFreeHead)
		{
			void* pvRes = m_pvFreeHead;
			m_pvFreeHead = *(void**)m_pvFreeHead;
			return pvRes;
		}

		_AddChunk();
	}
}
//--------------------------------------------------------------------------
void VePoolAllocator::_Deallocate(void* pvMemory) noexcept
{
#	ifdef VE_MEM_DEBUG
	--m_u32UnitCount;
#	endif
	*(void**)pvMemory = m_pvFreeHead;
	m_pvFreeHead = pvMemory;
}
//--------------------------------------------------------------------------
const VePoolAllocatorPtr& VePoolAllocator::GetAllocator(
	VeSizeT stUnitSize) noexcept
{
	return ve_sys.GetPoolAllocator(stUnitSize);
}
//--------------------------------------------------------------------------
