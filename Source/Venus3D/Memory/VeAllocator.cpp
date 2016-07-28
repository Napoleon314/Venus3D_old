////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeAllocator.cpp
//  Created:     2016/07/20 by Albert
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
VeStackAllocator::VeStackAllocator(size_t stSize) noexcept
	: m_stSize(stSize)
{
	m_pu8Buffer = (uint8_t*)VeAlignedMalloc(stSize, 16);
	m_pu8Current = m_pu8Buffer;
}
//--------------------------------------------------------------------------
VeStackAllocator::~VeStackAllocator() noexcept
{
	VE_ASSERT(m_pu8Buffer == m_pu8Current && m_kStack.empty());
	VeAlignedFree(m_pu8Buffer);
	m_pu8Buffer = nullptr;
	m_pu8Current = nullptr;
}
//--------------------------------------------------------------------------
void* VeStackAllocator::Allocate(size_t stSizeInBytes) noexcept
{
	stSizeInBytes = (stSizeInBytes + 0xF) & 0xFFFFFFF0;
	VE_ASSERT((m_stSize - (m_pu8Current - m_pu8Buffer)) >= stSizeInBytes);
	void* pvRes = m_pu8Current;
	m_pu8Current += stSizeInBytes;
	m_kStack.push(stSizeInBytes);
	return pvRes;
}
//--------------------------------------------------------------------------
void VeStackAllocator::Deallocate() noexcept
{
	m_pu8Current -= m_kStack.pop();
}
//--------------------------------------------------------------------------
VePoolAllocator::VePoolAllocator(size_t stUnitSize, size_t stUnitPerChunk,
	size_t stAlign) noexcept : m_stUnitSize(stUnitSize)
	, m_stUnitPerChunk(stUnitPerChunk), m_stAlign(stAlign)
{
	VE_ASSERT(stUnitSize >= sizeof(size_t) && stUnitSize >= stAlign
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
		VeAlignedFree((void*)(size_t(m_pkChunkHead->m_pvEnd)
			- m_stUnitSize * m_stUnitPerChunk));
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
	pkChunk->m_pvEnd = (void*)(size_t(pkChunk->m_pvFreeStart) +
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
				(void*)(size_t(m_pkChunkHead->m_pvFreeStart) + m_stUnitSize);
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
	size_t stUnitSize) noexcept
{
	return Venus3D::Ref().GetPoolAllocator(stUnitSize);
}
//--------------------------------------------------------------------------
