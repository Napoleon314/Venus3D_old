////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMemObject.cpp
//  Created:     2015/02/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
#ifdef VE_MEM_DEBUG
//--------------------------------------------------------------------------
struct VeDeleteCallParams
{
	const VeChar8* m_pcSourceFile;
	VeInt32 m_i32SourceLine;
	const VeChar8* m_pcFunction;
};
//--------------------------------------------------------------------------
static std::vector<VeDeleteCallParams> s_kDeleteCallParamsStack;
//--------------------------------------------------------------------------
static volatile VeThreadID s_hDeleteThreadID = 0;
//--------------------------------------------------------------------------
static VeThread::Mutex s_kDeleteMutex;
//--------------------------------------------------------------------------
void VeMemObject::PushDeleteCallParams(const VeChar8* pcSourceFile,
	VeInt32 i32SourceLine, const VeChar8* pcFunction) noexcept
{
	if (s_hDeleteThreadID == 0)
	{
		VE_ASSERT(s_kDeleteCallParamsStack.empty());
		s_kDeleteMutex.Lock();
		s_hDeleteThreadID = VeGetLocalThread();
	}
	else if (s_hDeleteThreadID != VeGetLocalThread())
	{
		s_kDeleteMutex.Lock();
		VE_ASSERT(s_kDeleteCallParamsStack.empty());
		VE_ASSERT(s_hDeleteThreadID == 0);
		s_hDeleteThreadID = VeGetLocalThread();
	}
	else
	{
		VE_ASSERT(s_kDeleteCallParamsStack.size());
	}
	VeDeleteCallParams kCurrent = { pcSourceFile, i32SourceLine, pcFunction };
	s_kDeleteCallParamsStack.push_back(kCurrent);
}
//--------------------------------------------------------------------------
void VeMemObject::PopDeleteCallParams() noexcept
{
	VE_ASSERT(s_hDeleteThreadID == VeGetLocalThread());
	s_kDeleteCallParamsStack.pop_back();
	if (s_kDeleteCallParamsStack.empty())
	{
		s_hDeleteThreadID = 0;
		s_kDeleteMutex.Unlock();
	}
}
//--------------------------------------------------------------------------
void* VeMemObject::operator new(VeSizeT stSize, const VeChar8* pcSourceFile,
	VeInt32 i32SourceLine, const VeChar8* pcFunction) noexcept
{
	return _VeMalloc(stSize, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
void* VeMemObject::operator new [](VeSizeT stSize, const VeChar8* pcSourceFile,
VeInt32 i32SourceLine, const VeChar8* pcFunction) noexcept
{
	return _VeMalloc(stSize, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
void VeMemObject::operator delete [](void* pvMem, const VeChar8* pcSourceFile,
VeInt32 i32SourceLine, const VeChar8* pcFunction) noexcept
{
	_VeFree(pvMem, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
void VeMemObject::operator delete(void* pvMem, const VeChar8* pcSourceFile,
VeInt32 i32SourceLine, const VeChar8* pcFunction) noexcept
{
	_VeFree(pvMem, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
void VeMemObject::operator delete (void* pvMem) noexcept
{
	VeDeleteCallParams& kParams = s_kDeleteCallParamsStack.back();
	_VeFree(pvMem, kParams.m_pcSourceFile, kParams.m_i32SourceLine, kParams.m_pcFunction);
}
//--------------------------------------------------------------------------
void VeMemObject::operator delete [](void* pvMem) noexcept
{
	VeDeleteCallParams& kParams = s_kDeleteCallParamsStack.back();
	_VeFree(pvMem, kParams.m_pcSourceFile, kParams.m_i32SourceLine, kParams.m_pcFunction);
}
//--------------------------------------------------------------------------
void* VeMemA16Object::operator new (VeSizeT stSize,
	const VeChar8* pcSourceFile, VeInt32 i32SourceLine,
	const VeChar8* pcFunction) noexcept
{
	return _VeAlignedMalloc(stSize, 16, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
void* VeMemA16Object::operator new[](VeSizeT stSize,
	const VeChar8* pcSourceFile, VeInt32 i32SourceLine,
	const VeChar8* pcFunction) noexcept
{
	return _VeAlignedMalloc(stSize, 16, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
void VeMemA16Object::operator delete (void* pvMem,
	const VeChar8* pcSourceFile, VeInt32 i32SourceLine,
	const VeChar8* pcFunction) noexcept
{
	_VeAlignedFree(pvMem, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
void VeMemA16Object::operator delete[](void* pvMem,
	const VeChar8* pcSourceFile, VeInt32 i32SourceLine,
	const VeChar8* pcFunction) noexcept
{
	_VeAlignedFree(pvMem, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
void VeMemA16Object::operator delete (void* pvMem) noexcept
{
	VeDeleteCallParams& kParams = s_kDeleteCallParamsStack.back();
	_VeAlignedFree(pvMem, kParams.m_pcSourceFile, kParams.m_i32SourceLine, kParams.m_pcFunction);
}
//--------------------------------------------------------------------------
void VeMemA16Object::operator delete [](void* pvMem) noexcept
{
	VeDeleteCallParams& kParams = s_kDeleteCallParamsStack.back();
	_VeAlignedFree(pvMem, kParams.m_pcSourceFile, kParams.m_i32SourceLine, kParams.m_pcFunction);
}
#else
//--------------------------------------------------------------------------
void VeMemObject::operator delete (void* pvMem) noexcept
{
	_VeFree(pvMem);
}
//--------------------------------------------------------------------------
void VeMemObject::operator delete [](void* pvMem) noexcept
{
	_VeFree(pvMem);
}
//--------------------------------------------------------------------------
void VeMemA16Object::operator delete (void* pvMem) noexcept
{
	_VeAlignedFree(pvMem);
}
//--------------------------------------------------------------------------
void VeMemA16Object::operator delete [](void* pvMem) noexcept
{
	_VeAlignedFree(pvMem);
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
void* VeMemObject::operator new (VeSizeT stSize) noexcept
{
	return VeAlignedMalloc(stSize, 16);
}
//--------------------------------------------------------------------------
void* VeMemObject::operator new (VeSizeT stSize, void* pvMemory) noexcept
{
	return pvMemory;
}
//--------------------------------------------------------------------------
void* VeMemObject::operator new [](VeSizeT stSize) noexcept
{
	return VeAlignedMalloc(stSize, 16);
}
//--------------------------------------------------------------------------
void* VeMemA16Object::operator new (VeSizeT stSize) noexcept
{
	return VeAlignedMalloc(stSize, 16);
}
//--------------------------------------------------------------------------
void* VeMemA16Object::operator new (VeSizeT stSize, void* pvMemory) noexcept
{
	return pvMemory;
}
//--------------------------------------------------------------------------
void* VeMemA16Object::operator new [](VeSizeT stSize) noexcept
{
	return VeAlignedMalloc(stSize, 16);
}
//--------------------------------------------------------------------------
