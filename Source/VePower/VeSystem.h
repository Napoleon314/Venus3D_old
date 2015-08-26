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

class VE_POWER_API VeSystem : public VeSingleton<VeSystem>
{
	VeNoCopy(VeSystem);
public:
	enum Type
	{
		TYPE_DEFAULT,
		TYPE_CONSOLE,
		TYPE_MAX
	};

	typedef std::map <VeSizeT, VePoolAllocatorPtr, std::less<VeSizeT>,
		venus::allocator<std::pair<const VeSizeT, VePoolAllocatorPtr>>>
		PoolAllocatorMap;

	VeSystem(Type eType, const VeChar8* pcPakName) noexcept;

	~VeSystem() noexcept;

	inline Type GetType() const noexcept;

	inline const VeChar8* GetPakName() const noexcept;

	inline VeLog& GetLog() noexcept;

	inline void Collect(VeRefNode<VeRefObject*>& kNode) noexcept;

	inline const VeStackAllocatorPtr& GetStackAllocator() noexcept;

	inline VeTime& GetTime() noexcept;

	const VePoolAllocatorPtr& GetPoolAllocator(VeSizeT stUnitSize) noexcept;

	void Init() noexcept;

	void Term() noexcept;

	void Update() noexcept;

private:
	void InitLog() noexcept;

	void TermLog() noexcept;

	void InitResMgr() noexcept;

	void TermResMgr() noexcept;

	const Type m_eType;
	VeFixedString m_kPakName;
	VeLog m_kLog;
	VeRefList<VeRefObject*> m_kObjCollector;
	VeStackAllocatorPtr m_spMainStack;
	PoolAllocatorMap m_kAllocatorMap;
	VeTimePtr m_spTime;

public:
	VeLog::Pack CORE;
	VeLog::Pack USER;

};

#define ve_sys VeSystem::GetSingleton()
#define ve_log ve_sys.GetLog()
#define ve_time ve_sys.GetTime()
#define VeStackMalloc(...) (ve_sys.GetStackAllocator()->Allocate(__VA_ARGS__))
#define VeStackAlloc(t,s) (t*)(ve_sys.GetStackAllocator()->Allocate(s))
#define VeStackFree(p) ve_sys.GetStackAllocator()->Deallocate(); p = nullptr

#ifdef VE_DEBUG
#	define VeDebugOutput ve_sys.USER.D.LogFormat
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
