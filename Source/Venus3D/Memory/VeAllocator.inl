////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeAllocator.inl
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

//--------------------------------------------------------------------------
inline size_t VeStackAllocator::Space() noexcept
{
	return m_stSize;
}
//--------------------------------------------------------------------------
inline size_t VeStackAllocator::RemainSpace() noexcept
{
	return m_stSize - (m_pu8Current - m_pu8Buffer);
}
//--------------------------------------------------------------------------
inline size_t VePoolAllocator::GetUnitSize() const noexcept
{
	return m_stUnitSize;
}
//--------------------------------------------------------------------------
inline size_t VePoolAllocator::GetUnitPerChunk() const noexcept
{
	return m_stUnitPerChunk;
}
//--------------------------------------------------------------------------
inline size_t VePoolAllocator::GetAlign() const noexcept
{
	return m_stAlign;
}
//--------------------------------------------------------------------------
inline void* VePoolAllocator::Allocate() noexcept
{
	std::lock_guard<vtd::spin_lock> l(m_kLock);
	return _Allocate();
}
//--------------------------------------------------------------------------
inline void VePoolAllocator::Deallocate(void* pvMemory) noexcept
{
	std::lock_guard<vtd::spin_lock> l(m_kLock);
	return _Deallocate(pvMemory);
}
//--------------------------------------------------------------------------
