////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeLuaBind.cpp
//  Created:     2015/03/17 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
static inline void GetScope(lua_State *L) noexcept
{
	VE_ASSERT(VeLua::type(L, -1) == VeLua::VAL_TTABLE);
	if (VeLua::getmetatable(L, -1))
	{
		VeLua::pushstring(L, "__index");
		VeLua::rawget(L, -2);
	}
	else
	{
		VeLua::newtable(L);
		VeLua::pushvalue(L, -1);
		VeLua::setmetatable(L, -3);
		VeLua::newtable(L);
		VeLua::pushstring(L, "__index");
		VeLua::pushvalue(L, -2);
		VeLua::rawset(L, -4);
	}
	VE_ASSERT(VeLua::type(L, -1) == VeLua::VAL_TTABLE);
	VE_ASSERT(VeLua::type(L, -2) == VeLua::VAL_TTABLE);
	VE_ASSERT(VeLua::type(L, -3) == VeLua::VAL_TTABLE);
}
//--------------------------------------------------------------------------
static inline void GetScope(lua_State *L, const VeChar8* pcName) noexcept
{
	VeLua::pushstring(L, pcName);
	if (VeLua::rawget(L, -2) <= 0)
	{
		VeLua::pop(L, 1);
		VeLua::newtable(L);

		VeLua::pushstring(L, pcName);
		VeLua::pushvalue(L, -2);
		VeLua::rawset(L, -4);
	}
	GetScope(L);
}
//--------------------------------------------------------------------------
venus::regist::regist() noexcept
{

}
//--------------------------------------------------------------------------
venus::regist::~regist() noexcept
{

}
//--------------------------------------------------------------------------
venus::scope::scope() noexcept
{

}
//--------------------------------------------------------------------------
venus::scope::scope(const registration_ptr& reg) noexcept
	: m_spChain(reg)
{

}
//--------------------------------------------------------------------------
venus::scope::scope(const scope& other) noexcept
{
	m_spChain = other.m_spChain;
	const_cast<scope&>(other).m_spChain = nullptr;
}
//--------------------------------------------------------------------------
venus::scope::~scope() noexcept
{

}
//--------------------------------------------------------------------------
venus::scope& venus::scope::operator=(const scope& other) noexcept
{
	m_spChain = other.m_spChain;
	const_cast<scope&>(other).m_spChain = nullptr;
	return *this;
}
//--------------------------------------------------------------------------
venus::scope& venus::scope::operator,(scope s) noexcept
{
	if (!m_spChain)
	{
		m_spChain = s.m_spChain;
		s.m_spChain = nullptr;
		return *this;
	}

	for (registration_ptr r = m_spChain; ; r = r->m_spNext)
	{
		if (!r->m_spNext)
		{
			r->m_spNext = s.m_spChain;
			s.m_spChain = 0;
			break;
		}
	}

	return *this;
}
//--------------------------------------------------------------------------
void venus::scope::_register(lua_State* L) const noexcept
{
	for (registration_ptr r = m_spChain; r != 0; r = r->m_spNext)
	{
		VE_LUA_STACK_CHECKER(L);
		r->_register(L);
	}
}
//--------------------------------------------------------------------------
namespace venus
{
	class registration_ : public regist
	{
	public:
		registration_(const VeChar8* pcName)
			: m_pcName(pcName)
		{

		}

		virtual void _register(lua_State *L) const noexcept
		{
			VE_LUA_STACK_HOLDER(L);
			if (m_pcName && *m_pcName)
			{
				GetScope(L, m_pcName);
			}
			m_kScope._register(L);
		}

		const VeChar8* m_pcName;
		venus::scope m_kScope;

	};
}
//--------------------------------------------------------------------------
venus::namespace_::namespace_(const VeChar8* pcName) noexcept
{
	m_spChain = VE_NEW registration_(pcName);
}
//--------------------------------------------------------------------------
venus::namespace_& venus::namespace_::operator[](scope s) noexcept
{
	VeSmartPointerCast(registration_, m_spChain)->m_kScope.operator,(s);
	return *this;
}
//--------------------------------------------------------------------------
venus::module::module(lua_State *L, const VeChar8* pcName) noexcept
	: m_pkState(L), m_pcName(pcName)
{

}
//--------------------------------------------------------------------------
void venus::module::operator[](scope s) noexcept
{
	VE_LUA_STACK_HOLDER(m_pkState);
	VeLua::pushglobaltable(m_pkState);
	GetScope(m_pkState);
	if (m_pcName && *m_pcName)
	{
		GetScope(m_pkState, m_pcName);
	}
	s._register(m_pkState);
}
//--------------------------------------------------------------------------
