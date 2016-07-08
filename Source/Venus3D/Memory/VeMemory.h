////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeMemory.h
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

#pragma once

#ifdef VE_DEBUG
//#	define VE_MEM_TRACK
#endif

#ifndef VE_SHIPPING
#	define VE_MEM_DEBUG
#endif

#ifndef BUILD_PLATFORM_APPLE
#	include <malloc.h>
#endif

inline void* aligned_malloc(size_t _Size, size_t _Alignment) noexcept
{
#	ifdef BUILD_PLATFORM_WIN
	return _aligned_malloc(_Size, _Alignment);
#	else
	void* _Pointer;
	if (VE_SUCCEEDED(posix_memalign(&_Pointer, _Alignment, _Size)))
	{
		return _Pointer;
	}
	else
	{
		return nullptr;
	}
#	endif
}

inline void aligned_free(void* _Block) noexcept
{
#	ifdef BUILD_PLATFORM_WIN
	_aligned_free(_Block);
#	else
	free(_Block);
#	endif
}

#ifdef VE_MEM_DEBUG

VENUS_API void _VeMemoryExit() noexcept;

VENUS_API void* _VeMalloc(size_t stSizeInBytes, const char* pcSourceFile, int32_t iSourceLine, const char* pcFunction) noexcept;

VENUS_API void* _VeAlignedMalloc(size_t stSizeInBytes, size_t stAlignment, const char* pcSourceFile, int32_t iSourceLine, const char* pcFunction) noexcept;

VENUS_API void* _VeRealloc(void* pvMemblock, size_t stSizeInBytes, const char* pcSourceFile, int32_t iSourceLine, const char* pcFunction) noexcept;

VENUS_API void _VeFree(void* pvMemory, const char* pcSourceFile, int32_t iSourceLine, const char* pcFunction) noexcept;

VENUS_API void _VeAlignedFree(void* pvMemory, const char* pcSourceFile, int32_t iSourceLine, const char* pcFunction) noexcept;

#define VeAlloc(T, count) ((T*)_VeMalloc(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))

#define VeMalloc(size) _VeMalloc(size, __FILE__, __LINE__, __FUNCTION__)

#define VeAlignedMalloc(size,align) _VeAlignedMalloc(size, align, __FILE__, __LINE__, __FUNCTION__)

#define VeRealloc(mem,size) _VeRealloc(mem, size, __FILE__, __LINE__, __FUNCTION__)

#define VeFree(mem) _VeFree(mem, __FILE__, __LINE__, __FUNCTION__)

#define VeAlignedFree(mem) _VeAlignedFree(mem, __FILE__, __LINE__, __FUNCTION__)

#define VE_NEW new(__FILE__, __LINE__, __FUNCTION__)

#define VE_DELETE(p) std::remove_pointer<decltype(p)>::type::operator delete (p, __FILE__, __LINE__, __FUNCTION__)

#define VE_DELETE_ARRAY(p) std::remove_pointer<decltype(p)>::type::operator delete[] (p, __FILE__, __LINE__, __FUNCTION__)

#else

inline void* _VeMalloc(size_t stSizeInBytes) noexcept;

inline void* _VeAlignedMalloc(size_t stSizeInBytes, size_t stAlignment) noexcept;

inline void* _VeRealloc(void* pvMemblock, size_t stSizeInBytes) noexcept;

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

#define VE_DELETE_ARRAY delete []

#endif

#define VE_SAFE_DELETE(p)			{ if(p) { VE_DELETE (p); (p) = nullptr; } }

#define VE_SAFE_DELETE_ARRAY(p)		{ if(p) { VE_DELETE_ARRAY(p); (p) = nullptr; } }

#define VE_SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p) = nullptr; } }

#include "VeMemory.inl"
