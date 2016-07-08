////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeMemory.cpp
//  Created:     2016/07/08 by Albert
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
#ifdef VE_MEM_DEBUG
//--------------------------------------------------------------------------
#ifdef VE_MEM_TRACK
//--------------------------------------------------------------------------
struct VeMallocInfo
{
	size_t m_stSize;
	const char* m_pcFile;
	int32_t m_i32Line;
	const char* m_pcFunc;
};
static std::map<size_t, VeMallocInfo> s_kMallocMap;
static std::map<size_t, std::pair<VeMallocInfo, size_t>> s_kAlignedMallocMap;
static vtd::spin_lock s_kLock;
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
static size_t s_stMallocCount(0);
static size_t s_stAlignedMallocCount(0);
//--------------------------------------------------------------------------
extern std::vector<void(*)()> g_kClassInitList;
//--------------------------------------------------------------------------
extern std::vector<void(*)()> g_kClassTermList;
//--------------------------------------------------------------------------
void _VeMemoryExit() noexcept
{
	assert(!s_stMallocCount);
	assert(!s_stAlignedMallocCount);
}
//--------------------------------------------------------------------------
void* _VeMalloc(size_t stSizeInBytes,
#ifdef VE_MEM_TRACK
	const char* pcSourceFile,
	int32_t iSourceLine,
	const char* pcFunction
#else
	const char*, int32_t, const char*
#endif
) noexcept
{
#	ifdef VE_MEM_TRACK
	std::lock_guard<vtd::spin_lock> lck(s_kLock);
	++s_stMallocCount;
	void* pvRes = malloc(stSizeInBytes);
	VeMallocInfo& kInfo = s_kMallocMap[(size_t)pvRes];
	kInfo.m_stSize = stSizeInBytes;
	kInfo.m_pcFile = pcSourceFile;
	kInfo.m_i32Line = iSourceLine;
	kInfo.m_pcFunc = pcFunction;
	return pvRes;
#	else
	++s_stMallocCount;
	return malloc(stSizeInBytes);
#	endif
}
//--------------------------------------------------------------------------
void* _VeAlignedMalloc(size_t stSizeInBytes, size_t stAlignment,
#ifdef VE_MEM_TRACK
	const char* pcSourceFile,
	int32_t iSourceLine,
	const char* pcFunction
#else
	const char*, int32_t, const char*
#endif
) noexcept
{
#	ifdef VE_MEM_TRACK
	std::lock_guard<vtd::spin_lock> lck(s_kLock);
	++s_stAlignedMallocCount;
	void* pvRes = aligned_malloc(stSizeInBytes, stAlignment);
	std::pair<VeMallocInfo, size_t>& kInfo = s_kAlignedMallocMap[(size_t)pvRes];
	kInfo.first.m_stSize = stSizeInBytes;
	kInfo.first.m_pcFile = pcSourceFile;
	kInfo.first.m_i32Line = iSourceLine;
	kInfo.first.m_pcFunc = pcFunction;
	kInfo.second = stAlignment;
	return pvRes;
#	else
	++s_stAlignedMallocCount;
	return aligned_malloc(stSizeInBytes, stAlignment);
#	endif
}
//--------------------------------------------------------------------------
void* _VeRealloc(void* pvMemblock, size_t stSizeInBytes,
#ifdef VE_MEM_TRACK
	const char* /*pcSourceFile*/,
	int32_t /*iSourceLine*/,
	const char* /*pcFunction*/
#else
	const char*, int32_t, const char*
#endif
) noexcept
{
#	ifdef VE_MEM_TRACK
	std::lock_guard<vtd::spin_lock> lck(s_kLock);
	void* pvRes = realloc(pvMemblock, stSizeInBytes);
	auto it = s_kMallocMap.find((size_t)pvMemblock);
	assert(it != s_kMallocMap.end());
	VeMallocInfo kInfo = it->second;
	s_kMallocMap.erase(it);
	assert(s_kMallocMap.find((size_t)pvRes) == s_kMallocMap.end());
	s_kMallocMap[(size_t)pvRes] = kInfo;
	return pvRes;
#	else
	return realloc(pvMemblock, stSizeInBytes);
#	endif
}
//--------------------------------------------------------------------------
void _VeFree(void* pvMemory,
#ifdef VE_MEM_TRACK
	const char* /*pcSourceFile*/,
	int32_t /*iSourceLine*/,
	const char* /*pcFunction*/
#else
	const char*, int32_t, const char*
#endif
) noexcept
{
#	ifdef VE_MEM_TRACK
	std::lock_guard<vtd::spin_lock> lck(s_kLock);
	--s_stMallocCount;
	free(pvMemory);
	auto it = s_kMallocMap.find((size_t)pvMemory);
	assert(it != s_kMallocMap.end());
	s_kMallocMap.erase(it);
#	else
	--s_stMallocCount;
	free(pvMemory);
#	endif
}
//--------------------------------------------------------------------------
void _VeAlignedFree(void* pvMemory,
#ifdef VE_MEM_TRACK
	const char* /*pcSourceFile*/,
	int32_t /*iSourceLine*/,
	const char* /*pcFunction*/
#else
	const char*, int32_t, const char*
#endif
) noexcept
{
#	ifdef VE_MEM_TRACK
	std::lock_guard<vtd::spin_lock> lck(s_kLock);
	--s_stAlignedMallocCount;
	aligned_free(pvMemory);
	auto it = s_kAlignedMallocMap.find((size_t)pvMemory);
	assert(it != s_kAlignedMallocMap.end());
	s_kAlignedMallocMap.erase(it);
#	else
	--s_stAlignedMallocCount;
	return aligned_free(pvMemory);
#	endif
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
