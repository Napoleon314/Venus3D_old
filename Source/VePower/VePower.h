////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VePower.h
//  Created:     2015/02/18 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#if defined(VE_PLATFORM_WIN) && defined(VE_DYNAMIC_LIB)
#	ifdef VE_POWER_EXPORTS
#	define VE_POWER_API __declspec (dllexport)
#	else
#	define VE_POWER_API __declspec (dllimport)
#	endif
#else
#	define VE_POWER_API
#endif

#if defined(VE_PLATFORM_WIN) || defined(VE_PLATFORM_LINUX) || defined(VE_PLATFORM_OSX)
#	define VE_PLATFORM_PC
#elif defined(VE_PLATFORM_IOS) || defined(VE_PLATFORM_ANDROID)
#	define VE_PLATFORM_MOBILE
#endif
#if defined(VE_PLATFORM_OSX) || defined(VE_PLATFORM_IOS)
#	define VE_PLATFORM_APPLE
#endif

#ifdef VE_PLATFORM_WIN
#	define VE_ENABLE_D3D11
#	define VE_ENABLE_D3D12
#endif
#ifdef VE_PLATFORM_ANDROID
#	define VE_USE_SLES
#else
#	define VE_USE_OAL
#endif
#if defined(VE_PLATFORM_PC)
#	define VE_ENABLE_GL
#	define VE_ENABLE_KEYBOARD
#	define VE_ENABLE_MOUSE
#elif defined(VE_PLATFORM_MOBILE)
#	define VE_ENABLE_GLES2
#endif

#ifdef VE_ENABLE_SSE
#	include <xmmintrin.h>
#endif

#define VE_OK (0)
#define VE_ELF (-1)
#define VE_INFINITE (0xFFFFFFFF)

enum VeResult
{
	VE_S_OK = 0x0,
	VE_E_FAIL = 0x80004005,

	VE_E_FILE_ACCESS,
	VE_E_FILE_CREATE,
	VE_E_FILE_WRITE
};

#define VE_SUCCEEDED(hr) ((hr)==0)
#define VE_FAILED(hr) ((hr)!=0)

#ifdef VE_PLATFORM_WIN
#	include <SDKDDKVer.h>
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <process.h>
#	include <direct.h>
#	include <io.h>
#	define VE_CALLBACK __stdcall
#else
#	include <dirent.h>
#	include <unistd.h>
#	include <sys/stat.h>
#	define VE_CALLBACK
#endif
#ifdef VE_PLATFORM_LINUX
#	include <pthread.h>
#	include <errno.h>
#	include <signal.h>
#	include <sys/time.h>
#	include <ctype.h>
#include <stdarg.h>
#endif
#ifdef VE_PLATFORM_ANDROID
#	include <pthread.h>
#	include <errno.h>
#	include <signal.h>
#	include <sys/time.h>
#	include <ctype.h>
#	include <android/log.h>
#	include <android/asset_manager.h>
#endif
#ifdef VE_PLATFORM_APPLE
#	include <libkern/OSAtomic.h>
#	include <pthread.h>
#	include <errno.h>
#	include <sys/time.h>
#   include <CoreFoundation/CoreFoundation.h>
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <functional>
#include <tuple>
#include <array>
#include <stack>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <mutex>

#if defined(VE_PLATFORM_WIN)

typedef __int8 VeInt8;
typedef unsigned __int8 VeUInt8;
typedef __int16 VeInt16;
typedef unsigned __int16 VeUInt16;
typedef __int32 VeInt32;
typedef unsigned __int32 VeUInt32;
typedef __int64 VeInt64;
typedef unsigned __int64 VeUInt64;

#elif defined(VE_PLATFORM_LINUX)

typedef int8_t VeInt8;
typedef u_int8_t VeUInt8;
typedef int16_t VeInt16;
typedef u_int16_t VeUInt16;
typedef int32_t VeInt32;
typedef u_int32_t VeUInt32;
typedef int64_t VeInt64;
typedef u_int64_t VeUInt64;

#elif defined(VE_PLATFORM_OSX)

typedef int8_t VeInt8;
typedef uint8_t VeUInt8;
typedef int16_t VeInt16;
typedef uint16_t VeUInt16;
typedef int32_t VeInt32;
typedef uint32_t VeUInt32;
typedef int64_t VeInt64;
typedef uint64_t VeUInt64;

#else

typedef __int8_t VeInt8;
typedef __uint8_t VeUInt8;
typedef __int16_t VeInt16;
typedef __uint16_t VeUInt16;
typedef __int32_t VeInt32;
typedef __uint32_t VeUInt32;
typedef __int64_t VeInt64;
typedef __uint64_t VeUInt64;

#endif

typedef size_t VeSizeT;
typedef ptrdiff_t VePtrDiff;
typedef time_t VeTimeT;
typedef long VeLong;
typedef unsigned long VeULong;
typedef char VeChar8;
typedef char16_t VeChar16;
typedef char32_t VeChar32;
typedef VeUInt8 VeByte;
typedef VeUInt16 VeWord;
typedef VeUInt32 VeDWord;
typedef float VeFloat32;
typedef double VeFloat64;

#define VE_MAX(a,b) (((a)>(b))?(a):(b))
#define VE_MIN(a,b) (((a)<(b))?(a):(b))
#define VE_CLAMP(a,l,h) ((a < l) ? l : ((a > h) ? h : a))

#define VE_TRUE (1)
#define VE_FALSE (0)
#define VE_BOOL VeUInt32

#define VE_INT8_MIN (-128)
#define VE_INT8_MAX (127)
#define VE_UINT8_MAX (0xffu)
#define VE_CHAR_MIN VE_INT8_MIN
#define VE_CHAR_MAX VE_INT8_MAX
#define VE_INT16_MIN (-32768)
#define VE_INT16_MAX (32767)
#define VE_UINT16_MAX (0xffffu)
#define VE_INT32_MIN (-2147483648)
#define VE_INT32_MAX (2147483647)
#define VE_UINT32_MAX (0xffffffffu)
#define VE_INT64_MAX (9223372036854775807i64)
#define VE_INT64_MIN (-9223372036854775808i64)
#define VE_UINT64_MAX (0xffffffffffffffffui64)
#define VE_SIZET_MAX ((VeSizeT)-1)

#define VE_TMIN(t) ((t)0)
#define VE_TMAX(t) ((t)VE_UINT32_MAX)

#define VE_MAX_PATH_LEN (1024)

#define VE_MASK(location) (VeUInt32(0x01<<(location)))
#define VE_MASK_HAS_ANY(flag,mask) (((flag)&(mask))!=0)
#define VE_MASK_HAS_ALL(flag,mask) (((flag)&(mask))==(mask))
#define VE_MASK_ADD(flag,mask) ((flag)|=(mask))
#define VE_MASK_DEL(flag,mask) ((flag)&=(~(mask)))
#define VE_MASK_REMOVE(flag,mask) ((flag)&(~(mask)))
#define VE_MASK_CHANGE(location,flag)									\
	if(VE_MASK_HAS_ANY(flag,VE_MASK(location)))							\
		VE_MASK_DEL(flag,VE_MASK(location));							\
	else																\
		VE_MASK_ADD(flag,VE_MASK(location));

#define VE_MAKE_ID(ch0, ch1, ch2, ch3)									\
	((VeDWord)(VeByte)(ch0) | ((VeDWord)(VeByte)(ch1) << 8) |			\
	((VeDWord)(VeByte)(ch2) << 16) | ((VeDWord)(VeByte)(ch3) << 24 ))

#define VeAbort abort

#define VeMemorySet(dst,val,len) (memset(dst,val,len))
#define VeZero(dst) (memset(dst,0,sizeof(dst)))
#define VeZeroMemory(dst,len) (memset((dst),0,(len)))
#define VeZeroMemoryEx(dst,size,count) (VeZeroMemory((dst),(size)*(count)))
#define VeMemoryCopy(dst,src,len) (memcpy((dst),(src),(len)))
#define VeCrazyCopy(dst,src,len) (memcpy((dst),(src),(len)))
#define VeCrazyCopyEx(dst,src,size,count) (VeCrazyCopy((dst),(src),(size)*(count)))

#ifdef VE_DEBUG
#	include <assert.h>
#	define VE_ASSERT assert
#	define VE_ASSERT_EQ(exp,val) VE_ASSERT(exp==val)
#	define VE_ASSERT_NE(exp,val) VE_ASSERT(exp!=val)
#	define VE_ASSERT_GT(exp,val) VE_ASSERT(exp>val)
#	define VE_ASSERT_LT(exp,val) VE_ASSERT(exp<val)
#	define VE_ASSERT_GE(exp,val) VE_ASSERT(exp>=val)
#	define VE_ASSERT_LE(exp,val) VE_ASSERT(exp<=val)
#else
#	define VE_ASSERT(x)
#	define VE_ASSERT_EQ(exp,val) (exp)
#	define VE_ASSERT_NE(exp,val) (exp)
#	define VE_ASSERT_GT(exp,val) (exp)
#	define VE_ASSERT_LT(exp,val) (exp)
#	define VE_ASSERT_GE(exp,val) (exp)
#	define VE_ASSERT_LE(exp,val) (exp)
#endif

#define VeNoCopy(cls) \
	cls(const cls&) = delete;\
	cls& operator= (const cls&) = delete

VE_POWER_API void VeConsoleInit(const VeChar8* pcPakName) noexcept;

VE_POWER_API void VeConsoleTerm() noexcept;
