////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeMemObject.cpp
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
struct VeDeleteCallParams
{
	const char* m_pcSourceFile;
	int32_t m_i32SourceLine;
	const char* m_pcFunction;
};
//--------------------------------------------------------------------------
static vtd::vector<VeDeleteCallParams> s_kDeleteCallParamsStack;
//--------------------------------------------------------------------------
static std::thread::id s_kDeleteThreadID;
//--------------------------------------------------------------------------
static vtd::spin_lock s_kDeleteLock;
//--------------------------------------------------------------------------
void VeMemObject::PushDeleteCallParams(const char* pcSourceFile,
	int32_t i32SourceLine, const char* pcFunction) noexcept
{
	if (s_kDeleteThreadID == std::thread::id())
	{
		assert(s_kDeleteCallParamsStack.empty());
		s_kDeleteLock.lock();
		s_kDeleteThreadID = std::this_thread::get_id();
	}
	else if (s_kDeleteThreadID != std::this_thread::get_id())
	{
		s_kDeleteLock.lock();
		assert(s_kDeleteCallParamsStack.empty());
		assert(s_kDeleteThreadID == std::thread::id());
		s_kDeleteThreadID = std::this_thread::get_id();
	}
	else
	{
		assert(s_kDeleteCallParamsStack.size());
	}
	VeDeleteCallParams kCurrent = { pcSourceFile, i32SourceLine, pcFunction };
	s_kDeleteCallParamsStack.push_back(kCurrent);
}
//--------------------------------------------------------------------------
void VeMemObject::PopDeleteCallParams() noexcept
{
	assert(s_kDeleteThreadID == std::this_thread::get_id());
	s_kDeleteCallParamsStack.pop_back();
	if (s_kDeleteCallParamsStack.empty())
	{
		s_kDeleteThreadID = std::thread::id();
		s_kDeleteLock.unlock();
	}
}
//--------------------------------------------------------------------------
void* VeMemObject::operator new(size_t stSize, const char* pcSourceFile,
	int32_t i32SourceLine, const char* pcFunction) noexcept
{
	return _VeMalloc(stSize, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
void* VeMemObject::operator new [](size_t stSize, const char* pcSourceFile,
int32_t i32SourceLine, const char* pcFunction) noexcept
{
	return _VeMalloc(stSize, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
void VeMemObject::operator delete (void* pvMem) noexcept
{
	VeDeleteCallParams& kParams = s_kDeleteCallParamsStack.back();
	_VeFree(pvMem, kParams.m_pcSourceFile, kParams.m_i32SourceLine, kParams.m_pcFunction);
}
//--------------------------------------------------------------------------
void VeMemObject::operator delete[](void* pvMem) noexcept
{
	VeDeleteCallParams& kParams = s_kDeleteCallParamsStack.back();
	_VeFree(pvMem, kParams.m_pcSourceFile, kParams.m_i32SourceLine, kParams.m_pcFunction);
}
//--------------------------------------------------------------------------
void VeMemObject::operator delete [](void* pvMem, const char* pcSourceFile,
int32_t i32SourceLine, const char* pcFunction) noexcept
{
	_VeFree(pvMem, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
void VeMemObject::operator delete(void* pvMem, const char* pcSourceFile,
int32_t i32SourceLine, const char* pcFunction) noexcept
{
	_VeFree(pvMem, pcSourceFile, i32SourceLine, pcFunction);
}
//--------------------------------------------------------------------------
#else
//--------------------------------------------------------------------------
void* VeMemObject::operator new (size_t stSize) noexcept
{
	return VeMalloc(stSize);
}
//--------------------------------------------------------------------------
void* VeMemObject::operator new [](size_t stSize) noexcept
{
	return VeAlignedMalloc(stSize, 16);
}
//--------------------------------------------------------------------------
void VeMemObject::operator delete (void* pvMem) noexcept
{
	VeFree(pvMem);
}
//--------------------------------------------------------------------------
void VeMemObject::operator delete [](void* pvMem) noexcept
{
	VeFree(pvMem);
}
//--------------------------------------------------------------------------
#endif
