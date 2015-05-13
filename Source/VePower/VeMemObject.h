////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMemObject.h
//  Created:     2015/02/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeMemObject
{
public:

#ifdef VE_MEM_DEBUG

	static void* operator new (VeSizeT stSize, const VeChar8* pcSourceFile, VeInt32 i32SourceLine, const VeChar8* pcFunction) noexcept;

	static void* operator new[](VeSizeT stSize, const VeChar8* pcSourceFile, VeInt32 i32SourceLine, const VeChar8* pcFunction) noexcept;

	static void operator delete (void* pvMem, const VeChar8* pcSourceFile, VeInt32 i32SourceLine, const VeChar8* pcFunction) noexcept;

	static void operator delete[](void* pvMem, const VeChar8* pcSourceFile, VeInt32 i32SourceLine, const VeChar8* pcFunction) noexcept;

	static void PushDeleteCallParams(const VeChar8* pcSourceFile, VeInt32 i32SourceLine, const VeChar8* pcFunction) noexcept;

	static void PopDeleteCallParams() noexcept;

#endif

	static void* operator new (VeSizeT stSize) noexcept;

	static void* operator new[](VeSizeT stSize) noexcept;

	static void* operator new (VeSizeT stSize, void* pvMemory) noexcept;

	static void operator delete (void* pvMem) noexcept;

	static void operator delete[](void* pvMem) noexcept;

};
