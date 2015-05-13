////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMemory.h
//  Created:     2015/02/19 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef VE_DEBUG
#	define VE_MEM_DEBUG
#	define VE_MEM_TRACK
#endif

#ifndef VE_PLATFORM_APPLE
#	include <malloc.h>
#else
void* memalign(VeSizeT stAlignment, VeSizeT stByteSize) noexcept;
#endif

#define VeExternalNew new
#define VeExternalDelete delete
#define VeExternalAlloc(T, count) ((T*)malloc(sizeof(T)*(count)))
#define VeExternalMalloc malloc
#define VeExternalRealloc realloc
#define VeExternalFree free
#define VeExternalCalloc calloc

#if defined(VE_PLATFORM_WIN)
#	define VeExternalAlignedMalloc(s,a) _aligned_malloc(s,a)
#	define VeExternalAlignedFree _aligned_free
#else
#	define VeExternalAlignedMalloc(s,a) memalign(a,s)
#	define VeExternalAlignedFree free
#endif

#include <new>

#ifdef VE_MEM_DEBUG

VE_POWER_API void _VeMemoryExit() noexcept;

VE_POWER_API void* _VeMalloc(VeSizeT stSizeInBytes, const VeChar8* pcSourceFile, VeInt32 iSourceLine, const VeChar8* pcFunction) noexcept;

VE_POWER_API void* _VeAlignedMalloc(VeSizeT stSizeInBytes, VeSizeT stAlignment, const VeChar8* pcSourceFile, VeInt32 iSourceLine, const VeChar8* pcFunction) noexcept;

VE_POWER_API void* _VeRealloc(void* pvMemblock, VeSizeT stSizeInBytes, const VeChar8* pcSourceFile, VeInt32 iSourceLine, const VeChar8* pcFunction) noexcept;

VE_POWER_API void _VeFree(void* pvMemory, const VeChar8* pcSourceFile, VeInt32 iSourceLine, const VeChar8* pcFunction) noexcept;

VE_POWER_API void _VeAlignedFree(void* pvMemory, const VeChar8* pcSourceFile, VeInt32 iSourceLine, const VeChar8* pcFunction) noexcept;

#define VeAlloc(T, count) ((T*)_VeMalloc(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))

#define VeMalloc(size) _VeMalloc(size, __FILE__, __LINE__, __FUNCTION__)

#define VeAlignedMalloc(size,align) _VeAlignedMalloc(size, align, __FILE__, __LINE__, __FUNCTION__)

#define VeRealloc(mem,size) _VeRealloc(mem, size, __FILE__, __LINE__, __FUNCTION__)

#define VeFree(mem) _VeFree(mem, __FILE__, __LINE__, __FUNCTION__)

#define VeAlignedFree(mem) _VeAlignedFree(mem, __FILE__, __LINE__, __FUNCTION__)

#define VE_NEW new(__FILE__, __LINE__, __FUNCTION__)

#define VE_DELETE(p) VeMemObject::PushDeleteCallParams(__FILE__, __LINE__, __FUNCTION__); delete p; VeMemObject::PopDeleteCallParams()

#define VE_DELETE_ARRAY(p) VeMemObject::PushDeleteCallParams(__FILE__, __LINE__, __FUNCTION__); delete [] p; VeMemObject::PopDeleteCallParams()

#else

inline void* _VeMalloc(VeSizeT stSizeInBytes) noexcept;

inline void* _VeAlignedMalloc(VeSizeT stSizeInBytes, VeSizeT stAlignment) noexcept;

inline void* _VeRealloc(void* pvMemblock, VeSizeT stSizeInBytes) noexcept;

inline void _VeFree(void* pvMemory) noexcept;

inline void _VeAlignedFree(void* pvMemory) noexcept;

#define VeAlloc(T, count) ((T*)_VeMalloc(sizeof(T)*(count)))

#define VeMalloc(size) _VeMalloc(size)

#define VeAlignedMalloc(size,align) _VeAlignedMalloc(size, align)

#define VeRealloc(mem,size) _VeRealloc(mem, size)

#define VeFree(mem) _VeFree(mem)

#define VeAlignedFree(mem) _VeAlignedFree(mem)

#define VE_NEW new

#define VE_DELETE delete

#define VE_DELETE_ARRAY VE_DELETE []

#endif

#define VE_SAFE_DELETE(p)			{ if(p) { VE_DELETE (p); (p) = nullptr; } }

#define VE_SAFE_DELETE_ARRAY(p)		{ if(p) { VE_DELETE_ARRAY(p); (p) = nullptr; } }

#define VE_SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p) = nullptr; } }

#include "VeMemory.inl"
