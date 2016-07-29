////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   Venus3D.h
//  Created:     2016/07/01 by Albert
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

#if !defined(__cplusplus)
#	error C++ compiler required.
#endif

#if defined(DEBUG) | defined(_DEBUG)
#	define VE_DEBUG
#elif defined(BUILD_SHIP)
#	define VE_SHIPPING
#else
#	define VE_RELEASE
#endif

#define VE_STRINGIZE(X) VE_DO_STRINGIZE(X)
#define VE_DO_STRINGIZE(X) #X

#define VE_JOIN(X, Y) VE_DO_JOIN(X, Y)
#define VE_DO_JOIN(X, Y) VE_DO_JOIN2(X, Y)
#define VE_DO_JOIN2(X, Y) X##Y

#if defined(__clang__)

#if __cplusplus < 201103L
#	error "-std=c++11 must be turned on."
#endif

#define VE_COMPILER_CLANG
#define VE_COMPILER_NAME "clang"

#define CLANG_VERSION VE_JOIN(__clang_major__, __clang_minor__)

#define VE_CXX11_CORE_CONSTEXPR_SUPPORT
#define VE_CXX11_CORE_NOEXCEPT_SUPPORT

#if defined(__APPLE__)
#	if CLANG_VERSION >= 40
#		define VE_COMPILER_VERSION CLANG_VERSION
#	else
#		error "Unsupported compiler version. Please install Apple clang++ 4.0 or up."
#	endif
#	define VE_CXX11_LIBRARY_REGEX_SUPPORT
#	if __cplusplus >= 201402L
#		define VE_CXX14_LIBRARY_MAKE_UNIQUE
#	endif
#	define VE_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#	define VE_SYMBOL_IMPORT
#elif defined(__MINGW32__)
#	if CLANG_VERSION >= 34
#		define VE_COMPILER_VERSION CLANG_VERSION
#	else
#		error "Unsupported compiler version. Please install clang++ 3.4 or up."
#	endif
#	include <bits/c++config.h>
#	ifdef _GLIBCXX_USE_FLOAT128
#		undef _GLIBCXX_USE_FLOAT128
#	endif
#	ifdef _GLIBCXX_USE_INT128
#		undef _GLIBCXX_USE_INT128
#	endif
#	ifdef __GLIBCXX__
#		if __GLIBCXX__ < 20130322 // g++ 4.8
#			error "Unsupported library version. Please install clang++ with g++ 4.8 or up."
#		endif
#		if !defined(_GLIBCXX_HAS_GTHREADS)
#			error "_GLIBCXX_HAS_GTHREADS must be turned on."
#		endif
#		if __GLIBCXX__ >= 20140422 // g++ 4.9
#			define VE_CXX11_LIBRARY_REGEX_SUPPORT
#			if __cplusplus > 201103L
#				define VE_TS_LIBRARY_OPTIONAL_SUPPORT
#			endif
#			if __cplusplus >= 201402L
#				define VE_CXX14_LIBRARY_MAKE_UNIQUE
#			endif
#		endif
#	endif
#	define VE_SYMBOL_EXPORT __declspec(dllexport)
#	define VE_SYMBOL_IMPORT __declspec(dllimport)
#else
#	error "Clang++ on an unknown platform. Only Apple and Windows are supported."
#endif

#elif defined(__GNUC__)

#define VE_COMPILER_GCC

#include <bits/c++config.h>
#ifdef _GLIBCXX_USE_FLOAT128
#	undef _GLIBCXX_USE_FLOAT128
#endif
#ifdef _GLIBCXX_USE_INT128
#	undef _GLIBCXX_USE_INT128
#endif

#define GCC_VERSION VE_JOIN(__GNUC__, __GNUC_MINOR__)

#if GCC_VERSION >= 48
#	define VE_COMPILER_VERSION GCC_VERSION
#else
#	error "Unsupported compiler version. Please install g++ 4.8 or up."
#endif

#if __cplusplus < 201103L
#	error "-std=c++11 or -std=c++0x must be turned on."
#endif
#if !defined(_GLIBCXX_HAS_GTHREADS)
#	error "_GLIBCXX_HAS_GTHREADS must be turned on."
#endif

#define VE_CXX11_CORE_CONSTEXPR_SUPPORT
#define VE_CXX11_CORE_NOEXCEPT_SUPPORT
#if VE_COMPILER_VERSION >= 49
#	define VE_CXX11_LIBRARY_REGEX_SUPPORT
#	if __cplusplus > 201103L
#		define VE_TS_LIBRARY_OPTIONAL_SUPPORT
#	endif
#	if __cplusplus >= 201402L
#		define VE_CXX14_LIBRARY_MAKE_UNIQUE
#	endif
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	define VE_SYMBOL_EXPORT __attribute__((__dllexport__))
#	define VE_SYMBOL_IMPORT __attribute__((__dllimport__))
#else
#	define VE_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#	define VE_SYMBOL_IMPORT
#endif

#elif defined(_MSC_VER)

#define VE_COMPILER_MSVC
#define VE_COMPILER_NAME vc

#define VE_HAS_DECLSPEC
#define VE_SYMBOL_EXPORT __declspec(dllexport)
#define VE_SYMBOL_IMPORT __declspec(dllimport)

#if _MSC_VER >= 1900
#	define VE_COMPILER_VERSION 140
#else
#	error "Unsupported compiler version. Please install vc14 or up."
#endif

#define VE_CXX11_LIBRARY_REGEX_SUPPORT
#define VE_TS_LIBRARY_FILESYSTEM_V2_SUPPORT
#define VE_CXX14_LIBRARY_MAKE_UNIQUE

#define VE_CXX11_CORE_CONSTEXPR_SUPPORT
#define VE_CXX11_CORE_NOEXCEPT_SUPPORT
#undef VE_TS_LIBRARY_FILESYSTEM_V2_SUPPORT
#define VE_TS_LIBRARY_FILESYSTEM_V3_SUPPORT

#ifdef VE_COMPILER_MSVC
#	pragma warning(disable: 4251) // STL classes are not dllexport.
#	pragma warning(disable: 4275) // Derived from non dllexport classes.
#	pragma warning(disable: 4334)
#	pragma warning(disable: 4706)
#	pragma warning(disable: 4702)
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#	define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _SCL_SECURE_NO_DEPRECATE
#	define _SCL_SECURE_NO_DEPRECATE
#endif

#else
#	error "Unknown compiler. Please install vc, g++ or clang."
#endif

#ifdef BUILD_SHARED_LIB
#	ifdef VENUS3D_EXPORTS
#		define VENUS_API VE_SYMBOL_EXPORT
#	else
#		define VENUS_API VE_SYMBOL_IMPORT
#	endif
#else
#	define VENUS_API
#endif

#if defined(BUILD_PLATFORM_WIN) || defined(BUILD_PLATFORM_LINUX) || defined(BUILD_PLATFORM_DARWIN)
#	define BUILD_PLATFORM_PC
#elif defined(BUILD_PLATFORM_IOS) || defined(BUILD_PLATFORM_ANDROID)
#	define BUILD_PLATFORM_MOBILE
#endif
#if defined(BUILD_PLATFORM_DARWIN) || defined(BUILD_PLATFORM_IOS)
#	define BUILD_PLATFORM_APPLE
#endif

#if defined(BUILD_PLATFORM_PC)
#	define VE_ENABLE_KEYBOARD
#	define VE_ENABLE_MOUSE
#endif

#ifdef BUILD_PLATFORM_WIN
#	define VE_ENABLE_D3D11
#	define VE_ENABLE_D3D12
#endif

#ifdef BUILD_PLATFORM_APPLE
#	define VE_ENABLE_METAL
#else
//#	define VE_ENABLE_VULKAN
#endif

#define VE_OK (0)
#define VE_ELF (-1)
#define VE_INFINITE (0xFFFFFFFF)

#define VE_SUCCEEDED(hr) ((hr) >= 0)
#define VE_FAILED(hr) ((hr) < 0)

#ifdef BUILD_PLATFORM_WIN
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
#	include <pthread.h>
#   if __APPLE__ && __MACH__
#       define _XOPEN_SOURCE
#       include <ucontext.h>
#   else
#       include <ucontext.h>
#   endif
#	define VE_CALLBACK
#endif
#ifdef BUILD_PLATFORM_LINUX
#	include <pthread.h>
#	include <errno.h>
#	include <signal.h>
#	include <sys/time.h>
#	include <ctype.h>
#	include <stdarg.h>
#	include <dlfcn.h>
#endif
#ifdef BUILD_PLATFORM_ANDROID
#	include <pthread.h>
#	include <errno.h>
#	include <signal.h>
#	include <sys/time.h>
#	include <ctype.h>
#	include <android/log.h>
#	include <android/asset_manager.h>
#endif
#ifdef BUILD_PLATFORM_APPLE
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
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <setjmp.h>

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
#include <condition_variable>

#include "VeAssert.h"

#ifdef BUILD_PLATFORM_PC
#	define VE_ENABLE_COROUTINE
#	include <fcontext/fcontext.h>
#endif

#ifndef BUILD_PLATFORM_APPLE
#	define VE_USE_THREAD_LOCAL
#endif

#define VE_MAX(a,b) (((a)>(b))?(a):(b))
#define VE_MIN(a,b) (((a)<(b))?(a):(b))
#define VE_CLAMP(a,l,h) (((a) < (l)) ? (l) : (((a) > (h)) ? (h) : (a)))
#define VE_ABS(x) ((x) < 0 ? -(x) : (x))

#define VE_TRUE (1)
#define VE_FALSE (0)
#define VE_BOOL uint32_t

#define VE_MASK(location) (uint32_t(0x01<<(location)))
#define VE_MASK_CLEAR(flag) (flag &= 0)
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
#define VE_MASK_CONDITION(flag, mask, run) if(VE_MASK_HAS_ALL(flag,mask)) run

#define VE_MAKE_FOURCC(ch0, ch1, ch2, ch3)								\
	((uint32_t)(ch0) | ((uint32_t)(ch1) << 8) |							\
	((uint32_t)(ch2) << 16) | ((uint32_t)(ch3) << 24 ))


inline constexpr uint32_t VeMakeVersion(uint32_t maj, uint32_t min = 0, uint32_t pch = 0) noexcept
{
	return (((maj) << 22) | ((min) << 12) | (pch));
}

#define VE_MAKE_VERSION(...) VeMakeVersion(__VA_ARGS__)

#define VE_VERSION_MAJOR(version) ((uint32_t)(version) >> 22)
#define VE_VERSION_MINOR(version) (((uint32_t)(version) >> 12) & 0x3ff)
#define VE_VERSION_PATCH(version) ((uint32_t)(version) & 0xfff)

#define VeNoCopy(cls) \
	cls(const cls&) = delete;\
	cls& operator= (const cls&) = delete

#define VeNoMove(cls) \
	cls(cls&&) = delete;\
	cls& operator= (cls&&) = delete

#if defined(BUILD_ARCH_ARM)
#	if defined(__ARM_NEON) && BUILD_INTRINSICS_LEVEL > 0
#		define _XM_ARM_NEON_INTRINSICS_
#	else
#		define _XM_NO_INTRINSICS_
#	endif
#else
#	if BUILD_INTRINSICS_LEVEL > 0
#		define _XM_SSE_INTRINSICS_
#	endif
#	if BUILD_INTRINSICS_LEVEL > 1
#		define _XM_SSE3_INTRINSICS_
#		define _XM_SSE4_INTRINSICS_
#		define _XM_AVX_INTRINSICS_
#	endif
#	if BUILD_INTRINSICS_LEVEL > 2
#		define _XM_F16C_INTRINSICS_
#		define _XM_AVX2_INTRINSICS_
#	endif
#endif
#if defined(VE_COMPILER_GCC) || defined(BUILD_PLATFORM_IOS)
#	define _XM_NO_CALL_CONVENTION_
#endif
#if defined(BUILD_PLATFORM_IOS) || defined(BUILD_PLATFORM_ANDROID)
#	define _XM_ARM_NEON_NO_ALIGN_
#endif

#include "XMath/XMath.h"
#include "XMath/XPackedVector.h"
#include "XMath/XColors.h"
#include "XMath/XCollision.h"

#include "vtd/rtti.h"
#include "vtd/vector.h"
#include "vtd/stack.h"
#include "vtd/ring_buffer.h"
#include "vtd/obj_pool.h"
#include "vtd/spin_lock.h"
#include "vtd/thread.h"
#include "vtd/intrusive_list.h"
#include "vtd/intrusive_ptr.h"
#include "vtd/string.h"
#include "vtd/point.h"
#include "vtd/rect.h"

#define VeRTTIDecl(classname,...) \
	public: \
		static const vtd::rtti::select<classname,##__VA_ARGS__>::type _rtti; \
		virtual const vtd::rtti::base* get_rtti() const noexcept { return &_rtti; } \
		bool is_exact_kind_of(const vtd::rtti::base* _Rtti) const noexcept \
		{ \
			return (get_rtti() == _Rtti); \
		} \
		void* _dynamic_cast(const vtd::rtti::base* _Rtti) const noexcept \
		{ \
			auto _Temp = _Rtti->get_path_from(get_rtti()); \
			if(_Temp == (decltype(_Temp)(-1))) \
			{ \
				return nullptr; \
			} \
			else \
			{ \
				_Temp = (decltype(_Temp))(void*)this + _Temp; \
				_Temp -= classname::_rtti.get_path_from(get_rtti()); \
				return (void*)_Temp; \
			} \
		} \
		bool is_kind_of(const vtd::rtti::base* _Rtti) const noexcept \
		{ \
			return _dynamic_cast(_Rtti) ? true : false; \
		}

#define VeRTTIImpl(classname,...) \
	const vtd::rtti::select<classname,##__VA_ARGS__>::type classname::_rtti(#classname)

#define VeIsExactKindOf vtd::rtti::base::is_exact_kind_of

#define VeIsKindOf vtd::rtti::base::is_kind_of

#define VeDynamicCast(t,p) vtd::rtti::base::_dynamic_cast<t>((t*)p)

#include "Memory/VeString.h"
#include "Memory/VeMemory.h"
#include "Memory/VeMemObject.h"
#include "Memory/VeRefObject.h"
#include "Memory/VeBlob.h"
#include "Memory/VeAllocator.h"

#include "Log/VeLog.h"
#include "Log/VeException.h"

#include "ASync/VeThread.h"
#include "ASync/VeCoroutine.h"
#include "ASync/VeJobSystem.h"

#include "System/VeSharedLib.h"
#include "System/VeTime.h"

#include "Archive/VeArchive.h"

#include "Video/VeWindow.h"
#include "Video/VeVideo.h"

struct VENUS_API VeThreadLocalSingleton : public VeMemObject
{
	VeCoenvironment m_kCoenviron;
	VeStackAllocator m_kAllocator;
};

enum VeInitMask
{
	VE_INIT_NONE		= 0x0,
	VE_INIT_LOG			= 0x1,
	VE_INIT_JOB			= 0x2,
	VE_INIT_VIDEO		= 0x4,

	VE_INIT_CONSOLE		= VE_INIT_LOG | VE_INIT_JOB,
	VE_INIT_WINDOW		= VE_INIT_CONSOLE | VE_INIT_VIDEO,

	VE_INIT_MASK		= 0xFFFFFFFF
};

struct VeInitData
{
	const char* m_pcAppName = nullptr;
	uint32_t m_u32AppVersion = 0;
	uint32_t m_u32InitMask = (uint32_t)VE_INIT_MASK;

	VeInitData(const char* pcName, uint32_t u32Version, uint32_t u32InitMask) noexcept
		: m_pcAppName(pcName), m_u32AppVersion(u32Version), m_u32InitMask(u32InitMask) {}

#ifdef BUILD_PLATFORM_WIN

	HINSTANCE m_hInstance = nullptr;
	HINSTANCE m_hPrevInstance = nullptr;
	int32_t m_i32CmdShow = 0;

	VeInitData(const char* pcName, uint32_t u32Version, uint32_t u32InitMask,
		HINSTANCE hInstance, HINSTANCE hPrevInstance, int32_t i32Show) noexcept
		: m_pcAppName(pcName), m_u32AppVersion(u32Version), m_u32InitMask(u32InitMask)
		, m_hInstance(hInstance), m_hPrevInstance(hPrevInstance), m_i32CmdShow(i32Show)
		 {}

	VeInitData(std::initializer_list<std::tuple<const char*,uint32_t,uint32_t,HINSTANCE,HINSTANCE,int32_t>> l) noexcept
	{
		m_pcAppName = std::get<0>(*l.begin());
		m_u32AppVersion = std::get<1>(*l.begin());
		m_u32InitMask = std::get<2>(*l.begin());
		m_hInstance = std::get<3>(*l.begin());
		m_hPrevInstance = std::get<4>(*l.begin());
		m_i32CmdShow = std::get<5>(*l.begin());
	}

#endif

};

class VENUS_API Venus3D : public VeSingleton<Venus3D>
{
public:
	typedef std::map <size_t, VePoolAllocatorPtr, std::less<size_t>,
		venus::allocator<std::pair<const size_t, VePoolAllocatorPtr>>>
		PoolAllocatorMap;

	static constexpr const char* ENGINE_NAME = "Venus3D";
	static constexpr uint32_t ENGINE_VERSION = VE_MAKE_VERSION(0, 1);

	Venus3D(const VeInitData& kInitData) noexcept;

	~Venus3D() noexcept;

	VeStackAllocator& GetStackAllocator() noexcept;

	const VePoolAllocatorPtr& GetPoolAllocator(size_t stUnitSize) noexcept;

	inline const VeInitData& GetInitData() noexcept;

	inline VeLog& GetLog() noexcept;

	inline VeTime& GetTime() noexcept;

	inline const VeVideoPtr& GetVideo() noexcept;

private:
	void Init();

	void Term();

	void InitLog() noexcept;

	void TermLog() noexcept;

	void InitJob() noexcept;

	void TermJob() noexcept;

	void InitVideo() noexcept;

	void TermVideo() noexcept;

	VeInitData m_kInitData;
	PoolAllocatorMap m_kAllocatorMap;
	vtd::spin_lock m_kAllocatorLock;
	VeLog m_kLog;
	VeTime m_kTime;
	VeVideoPtr m_spVideo;

public:
	VeLog::Pack CORE;
	VeLog::Pack USER;

};

#define venus3d Venus3D::Ref()
#define job_system VeJobSystem::Ref()

#define VeStackAlloc(t,s) (t*)(Venus3D::Ref().GetStackAllocator().Allocate(sizeof(t)*(s)))
#define VeStackFree(p) Venus3D::Ref().GetStackAllocator().Deallocate(); (p) = nullptr

template <class _Ty>
class VeDyanmicStack
{
	VeNoCopy(VeDyanmicStack);
	VeNoMove(VeDyanmicStack);
public:
	typedef _Ty* iterator;
	typedef const _Ty* const_iterator;

	VeDyanmicStack(size_t stNum) noexcept
	{
		m_pData = VeStackAlloc(_Ty, stNum);
		m_stNum = stNum;
	}

	VeDyanmicStack(std::initializer_list<_Ty> l) noexcept
	{
		m_stNum = l.size();
		m_pData = VeStackAlloc(_Ty, m_stNum);
		_Ty* pStart = m_pData;
		for (auto&& val : l)
		{
			*pStart++ = val;
		}
	}

	~VeDyanmicStack() noexcept
	{
		VeStackFree(m_pData);
	}

	operator _Ty* () noexcept
	{
		return m_pData;
	}

	_Ty& operator [] (size_t stIndex) noexcept
	{
#		ifdef VE_DEBUG
		VE_ASSERT(stIndex < m_stNum);
#		endif
		return m_pData[stIndex];
	}

	iterator begin() noexcept
	{
		return m_pData;
	}

	iterator end() noexcept
	{
		return m_pData + m_stNum;
	}

	_Ty* data() noexcept
	{
		return m_pData;
	}

	const_iterator begin() const noexcept
	{
		return m_pData;
	}

	const_iterator end() const noexcept
	{
		return m_pData + m_stNum;
	}

	const _Ty* data() const noexcept
	{
		return m_pData;
	}

	size_t size() const noexcept
	{
		return m_stNum;
	}

private:
	_Ty* m_pData = nullptr;
	size_t m_stNum;

};

#ifdef VE_DEBUG
#	define VeCoreDebugOutput venus3d.CORE.D.LogFormat
#	define VeDebugOutput venus3d.USER.D.LogFormat
#	define VeCoreLogD venus3d.CORE.D.Log
#	define VeUserLogD venus3d.USER.D.Log
#else
#	define VeCoreDebugOutput(...)
#	define VeDebugOutput(...)
#	define VeCoreLogD(...)
#	define VeUserLogD(...)
#endif

#define VeCoreLogI venus3d.CORE.I.Log
#define VeUserLogI venus3d.USER.I.Log
#define VeCoreLogW venus3d.CORE.W.Log
#define VeUserLogW venus3d.USER.W.Log
#define VeCoreLogE venus3d.CORE.E.Log
#define VeUserLogE venus3d.USER.E.Log

inline void VeInit(const VeInitData& kInitData) noexcept
{
	VeThread::Init();
	Venus3D::Create(kInitData);
}

inline void VeTerm() noexcept
{
	Venus3D::Destory();
	VeThread::Term();
	VeMemoryExit();
}

#include "Renderer/VeRenderWindow.h"
#include "Renderer/VeRenderState.h"
#include "Renderer/VeRenderer.h"

#include "Venus3D.inl"
