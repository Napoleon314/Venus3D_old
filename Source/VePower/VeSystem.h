////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeSystem.h
//  Created:     2015/03/03 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#define VE_STACK_SIZE (524288)

VeSmartPointer(VeResourceManager);

class VE_POWER_API VeSystem : public VeSingleton<VeSystem>
{
public:
	enum Type
	{
		TYPE_DEFAULT,
		TYPE_CONSOLE,
		TYPE_LUA_DEBUG,
		TYPE_MAX
	};

	typedef std::map <VeSizeT, VePoolAllocatorPtr, std::less<VeSizeT>,
		venus::allocator<std::pair<const VeSizeT, VePoolAllocatorPtr>>>
		PoolAllocatorMap;

	VeSystem(Type eType, const VeChar8* pcPakName) noexcept;

	~VeSystem() noexcept;

	inline Type GetType() const noexcept;

	inline VeLog& GetLog() noexcept;

	inline VeLua& GetLua() noexcept;

	inline void Collect(VeRefNode<VeRefObject*>& kNode) noexcept;

	inline const VeStackAllocatorPtr& GetStackAllocator() noexcept;

	const VePoolAllocatorPtr& GetPoolAllocator(VeSizeT stUnitSize) noexcept;

	void Init() noexcept;

	void Term() noexcept;

private:
	void InitVideo() noexcept;

	void TermVideo() noexcept;

	const Type m_eType;
	VeFixedString m_kPakName;
	VeLog m_kLog;
	VeLuaPtr m_spLua;
	VeRefList<VeRefObject*> m_kObjCollector;
	VeStackAllocatorPtr m_spMainStack;
	PoolAllocatorMap m_kAllocatorMap;
	VeVideoDevicePtr m_spVideo;

public:
	VeLog::Pack CORE;
	VeLog::Pack USER;

};

#define ve_sys VeSystem::GetSingleton()
#define ve_log ve_sys.GetLog()
#define ve_lua ve_sys.GetLua()
#define VeStackMalloc(s) (ve_sys.GetStackAllocator()->Allocate(s))
#define VeStackAlloc(t,s) (t*)(ve_sys.GetStackAllocator()->Allocate(s))
#define VeStackFree(p) ve_sys.GetStackAllocator()->Deallocate(); p = nullptr

#ifdef VE_DEBUG
#	define VeDebugOutput ve_sys.CORE.D.LogFormat
#	define VeCoreLogD ve_sys.CORE.D.Log
#	define VeUserLogD ve_sys.USER.D.Log
#else
#	define VeDebugOutput(...)
#	define VeCoreLogD(...)
#	define VeUserLogD(...)
#endif

#define VeCoreLogI ve_sys.CORE.I.Log
#define VeUserLogI ve_sys.USER.I.Log
#define VeCoreLogW ve_sys.CORE.W.Log
#define VeUserLogW ve_sys.USER.W.Log
#define VeCoreLogE ve_sys.CORE.E.Log
#define VeUserLogE ve_sys.USER.E.Log

#include "VeSystem.inl"
