////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMemory.cpp
//  Created:     2015/02/19 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
#ifdef VE_PLATFORM_APPLE
void* memalign(VeSizeT stAlignment, VeSizeT stByteSize) noexcept
{
	void* pvPointer(nullptr);
	if (VE_SUCCEEDED(posix_memalign(&pvPointer, stAlignment, stByteSize)))
	{
		return pvPointer;
	}
	else
	{
		return nullptr;
	}
}
#endif
//--------------------------------------------------------------------------
#ifdef VE_MEM_DEBUG
//--------------------------------------------------------------------------
#ifdef VE_MEM_TRACK
//--------------------------------------------------------------------------
struct VeMallocInfo
{
	VeSizeT m_stSize;
	const VeChar8* m_pcFile;
	VeInt32 m_i32Line;
	const VeChar8* m_pcFunc;
};
static std::map<VeSizeT, VeMallocInfo> s_kMallocMap;
static std::map<VeSizeT, std::pair<VeMallocInfo,VeSizeT>> s_kAlignedMallocMap;
static VeSpinLock s_kMutex;
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
static VeSizeT s_stMallocCount(0);
static VeSizeT s_stAlignedMallocCount(0);
//--------------------------------------------------------------------------
extern std::vector<void(*)()> g_kClassInitList;
//--------------------------------------------------------------------------
extern std::vector<void(*)()> g_kClassTermList;
//--------------------------------------------------------------------------
void _VeMemoryExit() noexcept
{
	VE_ASSERT(!s_stMallocCount);
	VE_ASSERT(!s_stAlignedMallocCount);
}
//--------------------------------------------------------------------------
void* _VeMalloc(VeSizeT stSizeInBytes, const VeChar8* pcSourceFile,
	VeInt32 iSourceLine, const VeChar8* pcFunction) noexcept
{
#	ifdef VE_MEM_TRACK
	VE_LOCK_MUTEX(s_kMutex);
	++s_stMallocCount;
	void* pvRes = VeExternalMalloc(stSizeInBytes);
	VeMallocInfo& kInfo = s_kMallocMap[(VeSizeT)pvRes];
	kInfo.m_stSize = stSizeInBytes;
	kInfo.m_pcFile = pcSourceFile;
	kInfo.m_i32Line = iSourceLine;
	kInfo.m_pcFunc = pcFunction;
	return pvRes;
#	else
	++s_stMallocCount;
	return VeExternalMalloc(stSizeInBytes);
#	endif
}
//--------------------------------------------------------------------------
void* _VeAlignedMalloc(VeSizeT stSizeInBytes, VeSizeT stAlignment,
	const VeChar8* pcSourceFile, VeInt32 iSourceLine,
	const VeChar8* pcFunction) noexcept
{
#	ifdef VE_MEM_TRACK
	VE_LOCK_MUTEX(s_kMutex);
	++s_stAlignedMallocCount;
	void* pvRes = VeExternalAlignedMalloc(stSizeInBytes, stAlignment);
	std::pair<VeMallocInfo,VeSizeT>& kInfo = s_kAlignedMallocMap[(VeSizeT)pvRes];
	kInfo.first.m_stSize = stSizeInBytes;
	kInfo.first.m_pcFile = pcSourceFile;
	kInfo.first.m_i32Line = iSourceLine;
	kInfo.first.m_pcFunc = pcFunction;
	kInfo.second = stAlignment;
	return pvRes;
#	else
	++s_stAlignedMallocCount;
	return VeExternalAlignedMalloc(stSizeInBytes, stAlignment);
#	endif
}
//--------------------------------------------------------------------------
void* _VeRealloc(void* pvMemblock, VeSizeT stSizeInBytes,
	const VeChar8* pcSourceFile, VeInt32 iSourceLine,
	const VeChar8* pcFunction) noexcept
{
#	ifdef VE_MEM_TRACK
	VE_LOCK_MUTEX(s_kMutex);
	void* pvRes = VeExternalRealloc(pvMemblock, stSizeInBytes);
	auto it = s_kMallocMap.find((VeSizeT)pvMemblock);
	VE_ASSERT(it != s_kMallocMap.end());
	VeMallocInfo kInfo = it->second;
	s_kMallocMap.erase(it);
	VE_ASSERT(s_kMallocMap.find((VeSizeT)pvRes) == s_kMallocMap.end());
	s_kMallocMap[(VeSizeT)pvRes] = kInfo;
	return pvRes;
#	else
	return VeExternalRealloc(pvMemblock, stSizeInBytes);
#	endif
}
//--------------------------------------------------------------------------
void _VeFree(void* pvMemory, const VeChar8* pcSourceFile,
	VeInt32 iSourceLine, const VeChar8* pcFunction) noexcept
{
#	ifdef VE_MEM_TRACK
	VE_LOCK_MUTEX(s_kMutex);
	--s_stMallocCount;
	VeExternalFree(pvMemory);
	auto it = s_kMallocMap.find((VeSizeT)pvMemory);
	VE_ASSERT(it != s_kMallocMap.end());
	s_kMallocMap.erase(it);
#	else
	--s_stMallocCount;
	VeExternalFree(pvMemory);
#	endif
}
//--------------------------------------------------------------------------
void _VeAlignedFree(void* pvMemory, const VeChar8* pcSourceFile,
	VeInt32 iSourceLine, const VeChar8* pcFunction) noexcept
{
#	ifdef VE_MEM_TRACK
	VE_LOCK_MUTEX(s_kMutex);
	--s_stAlignedMallocCount;
	VeExternalAlignedFree(pvMemory);
	auto it = s_kAlignedMallocMap.find((VeSizeT)pvMemory);
	VE_ASSERT(it != s_kAlignedMallocMap.end());
	s_kAlignedMallocMap.erase(it);
#	else
	--s_stAlignedMallocCount;
	return VeExternalAlignedFree(pvMemory);
#	endif
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
