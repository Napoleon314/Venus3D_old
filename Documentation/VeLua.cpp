////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeLua.cpp
//  Created:     2015/03/03 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

extern "C"
{
#	include <lua.h>
#	include <lauxlib.h>
#	include <lualib.h>
}

//--------------------------------------------------------------------------
static VeInt32 LuaPanic(lua_State* L) noexcept
{
	ve_log.LogFormat(VeLog::TYPE_ERROR, "VeLua",
		"PANIC: unprotected error in call to Lua API (%s)",
		lua_tostring(L, -1));
	return 0;
}
//--------------------------------------------------------------------------
static void* LuaAlloc(void *ud, void *ptr, size_t osize,
	size_t nsize) noexcept
{
	if (nsize == 0)
	{
		if(ptr) VeFree(ptr);
		return nullptr;
	}
	else if (ptr)
	{
		return VeRealloc(ptr, nsize);
	}
	else
	{
		return VeMalloc(nsize);
	}
}
//--------------------------------------------------------------------------
static VeInt32 LuaPrint(lua_State* L) noexcept
{
	ve_log.Begin();
	VeInt32 n = lua_gettop(L);
	VeInt32 i;
	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++)
	{
		const VeChar8 *s;
		VeSizeT l;
		lua_pushvalue(L, -1);
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		s = lua_tolstring(L, -1, &l);
		if (s == nullptr)
			return luaL_error(L, "'tostring' must return a string to 'print'");
		if (i>1) ve_log.Append("\t");
		ve_log.Append(s);
		lua_pop(L, 1);
	}
	ve_log.End(VeLog::TYPE_INFO, "VeLua");
	return 0;
}
//--------------------------------------------------------------------------
VeLua::VeLua() noexcept
{
	m_pkState = lua_newstate(&LuaAlloc, nullptr);
	VE_ASSERT(m_pkState);
	lua_atpanic(m_pkState, &LuaPanic);
	luaL_openlibs(m_pkState);
	if (ve_sys.GetType() != VeSystem::TYPE_LUA_DEBUG)
	{
		lua_pushcfunction(m_pkState, &LuaPrint);
		lua_setglobal(m_pkState, "print");
	}
}
//--------------------------------------------------------------------------
VeLua::~VeLua() noexcept
{
	VE_ASSERT(m_pkState);
	lua_close(m_pkState);
	m_pkState = nullptr;
}
//--------------------------------------------------------------------------
VeInt32 VeLua::upvalueindex(VeInt32 index) noexcept
{
	return lua_upvalueindex(index);
}
//--------------------------------------------------------------------------
VeInt32 VeLua::pushvalue(lua_State *L, const VeChar8 *str) noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	VeStrcpy(acBuffer, str);
	VeChar8* pcContext;
	VeInt32 i32Top = lua_gettop(L);
	lua_pushglobaltable(L);
	VeChar8* pcTemp = VeStrtok(acBuffer, ".", &pcContext);
	while (pcTemp)
	{
		if (!(*pcTemp))
		{
			lua_settop(L, i32Top);
			return 0;
		}
		lua_pushstring(L, pcTemp);
		lua_gettable(L, -2);
		if (lua_isnil(L, -1))
		{
			lua_settop(L, i32Top);
			return 0;
		}
		pcTemp = VeStrtok<VeChar8>(nullptr, ".", &pcContext);
	}
	lua_replace(L, i32Top + 1);
	lua_settop(L, i32Top + 1);
	return 1;
}
//--------------------------------------------------------------------------
VeInt32 VeLua::dostring(lua_State *L, const VeChar8 *str) noexcept
{
	return luaL_dostring(L, str);
}
//--------------------------------------------------------------------------
VeInt32 VeLua::dofile(lua_State *L, const VeChar8 *path) noexcept
{
	if (ve_sys.GetType() == VeSystem::TYPE_LUA_DEBUG)
	{
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeStrcpy(acBuffer, path);
		VeChar8* pcContext;
		VeChar8* pcTemp = VeStrtok(acBuffer, "#", &pcContext);
		if (pcContext && (!VeStrcmp(pcTemp, VeFilePath::GetTypeName())))
		{
			return luaL_dofile(L, pcContext);
		}
		else if (!pcContext)
		{
			return luaL_dofile(L, path);
		}
	}
	VeBinaryIStreamPtr spInput = ve_res_mgr.CreateStream(path);
	if (spInput)
	{
		VeSizeT stLen = spInput->RemainingLength();
		VeChar8* pcData = VeStackAlloc(VeChar8, stLen);
		VE_ASSERT_EQ(spInput->Read(pcData, stLen), stLen);
		VeInt32 i32Res = luaL_loadbuffer(L, pcData, stLen, path)
			|| lua_pcall(L, 0, LUA_MULTRET, 0);
		VeStackFree(pcData);
		return i32Res;
	}
	else
	{
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeSprintf(acBuffer, "cannot open %s: No such file", path);
		lua_pushstring(L, acBuffer);
		return LUA_ERRERR;
	}
	return 0;
}
//--------------------------------------------------------------------------
VeInt32 VeLua::pcall(lua_State *L, VeInt32 nargs, VeInt32 nresults,
	VeInt32 msgh) noexcept
{
	return lua_pcall(L, nargs, nresults, msgh);
}
//--------------------------------------------------------------------------
void VeLua::settop(lua_State *L, int index) noexcept
{
	lua_settop(L, index);
}
//--------------------------------------------------------------------------
VeInt32 VeLua::gettop(lua_State *L) noexcept
{
	return lua_gettop(L);
}
//--------------------------------------------------------------------------
void VeLua::pop(lua_State *L, VeInt32 n) noexcept
{
	lua_pop(L, n);
}
//--------------------------------------------------------------------------
VeInt32 VeLua::type(lua_State *L, VeInt32 index) noexcept
{
	return lua_type(L, index);
}
//--------------------------------------------------------------------------
bool VeLua::isboolean(lua_State *L, VeInt32 index) noexcept
{
	return lua_isboolean(L, index);
}
//--------------------------------------------------------------------------
bool VeLua::iscfunction(lua_State *L, VeInt32 index) noexcept
{
	return lua_iscfunction(L, index) ? true : false;
}
//--------------------------------------------------------------------------
bool VeLua::isfunction(lua_State *L, VeInt32 index) noexcept
{
	return lua_isfunction(L, index);
}
//--------------------------------------------------------------------------
bool VeLua::isinteger(lua_State *L, VeInt32 index) noexcept
{
	return lua_isinteger(L, index) ? true : false;
}
//--------------------------------------------------------------------------
bool VeLua::islightuserdata(lua_State *L, VeInt32 index) noexcept
{
	return lua_islightuserdata(L, index);
}
//--------------------------------------------------------------------------
bool VeLua::isnil(lua_State *L, VeInt32 index) noexcept
{
	return lua_isnil(L, index);
}
//--------------------------------------------------------------------------
bool VeLua::isnone(lua_State *L, VeInt32 index) noexcept
{
	return lua_isnone(L, index);
}
//--------------------------------------------------------------------------
bool VeLua::isnoneornil(lua_State *L, VeInt32 index) noexcept
{
	return lua_isnoneornil(L, index);
}
//--------------------------------------------------------------------------
bool VeLua::isnumber(lua_State *L, VeInt32 index) noexcept
{
	return lua_isnumber(L, index) ? true : false;
}
//--------------------------------------------------------------------------
bool VeLua::isstring(lua_State *L, VeInt32 index) noexcept
{
	return lua_isstring(L, index) ? true : false;
}
//--------------------------------------------------------------------------
bool VeLua::istable(lua_State *L, VeInt32 index) noexcept
{
	return lua_istable(L, index);
}
//--------------------------------------------------------------------------
bool VeLua::isthread(lua_State *L, VeInt32 index) noexcept
{
	return lua_isthread(L, index);
}
//--------------------------------------------------------------------------
bool VeLua::isuserdata(lua_State *L, VeInt32 index) noexcept
{
	return lua_isuserdata(L, index) ? true : false;
}
//--------------------------------------------------------------------------
void VeLua::pushboolean(lua_State *L, bool val) noexcept
{
	lua_pushboolean(L, val);
}
//--------------------------------------------------------------------------
void VeLua::pushcclosure(lua_State *L, CFunc fn, VeInt32 n) noexcept
{
	lua_pushcclosure(L, fn, n);
}
//--------------------------------------------------------------------------
void VeLua::pushcfunction(lua_State *L, CFunc f) noexcept
{
	lua_pushcfunction(L, f);
}
//--------------------------------------------------------------------------
const VeChar8 *VeLua::pushfstring(lua_State *L,
	const VeChar8 *fmt, ...) noexcept
{
	const VeChar8 *ret;
	va_list argp;
	va_start(argp, fmt);
	ret = lua_pushvfstring(L, fmt, argp);
	va_end(argp);
	return ret;
}
//--------------------------------------------------------------------------
void VeLua::pushglobaltable(lua_State *L) noexcept
{
	lua_pushglobaltable(L);
}
//--------------------------------------------------------------------------
void VeLua::pushinteger(lua_State *L, VeInt64 n) noexcept
{
	lua_pushinteger(L, n);
}
//--------------------------------------------------------------------------
void VeLua::pushlightuserdata(lua_State *L, void *p) noexcept
{
	lua_pushlightuserdata(L, p);
}
//--------------------------------------------------------------------------
const VeChar8 *VeLua::pushlstring(lua_State *L,
	const VeChar8 *s, VeSizeT len) noexcept
{
	return lua_pushlstring(L, s, len);
}
//--------------------------------------------------------------------------
void VeLua::pushnil(lua_State *L) noexcept
{
	lua_pushnil(L);
}
//--------------------------------------------------------------------------
void VeLua::pushnumber(lua_State *L, VeFloat64 n) noexcept
{
	lua_pushnumber(L, n);
}
//--------------------------------------------------------------------------
const VeChar8 *VeLua::pushstring(lua_State *L,
	const VeChar8 *s) noexcept
{
	return lua_pushstring(L, s);
}
//--------------------------------------------------------------------------
VeInt32 VeLua::pushthread(lua_State *L) noexcept
{
	return lua_pushthread(L);
}
//--------------------------------------------------------------------------
void VeLua::pushvalue(lua_State *L, VeInt32 index) noexcept
{
	lua_pushvalue(L, index);
}
//--------------------------------------------------------------------------
const VeChar8 *VeLua::pushvfstring(lua_State *L,
	const VeChar8 *fmt, va_list argp) noexcept
{
	return lua_pushvfstring(L, fmt, argp);
}
//--------------------------------------------------------------------------
bool VeLua::toboolean(lua_State *L, VeInt32 index) noexcept
{
	return lua_toboolean(L, index) ? true : false;
}
//--------------------------------------------------------------------------
VeLua::CFunc VeLua::tocfunction(lua_State *L, VeInt32 index) noexcept
{
	return lua_tocfunction(L, index);
}
//--------------------------------------------------------------------------
VeInt64 VeLua::tointeger(lua_State *L, VeInt32 index) noexcept
{
	return lua_tointeger(L, index);
}
//--------------------------------------------------------------------------
VeInt64 VeLua::tointegerx(lua_State *L, VeInt32 index,
	VeInt32 *isnum) noexcept
{
	return lua_tointegerx(L, index, isnum);
}
//--------------------------------------------------------------------------
const VeChar8 *VeLua::tolstring(lua_State *L, VeInt32 index,
	VeSizeT *len) noexcept
{
	return lua_tolstring(L, index, len);
}
//--------------------------------------------------------------------------
VeFloat64 VeLua::tonumber(lua_State *L, VeInt32 index) noexcept
{
	return lua_tonumber(L, index);
}
//--------------------------------------------------------------------------
VeFloat64 VeLua::tonumberx(lua_State *L, VeInt32 index,
	VeInt32 *isnum) noexcept
{
	return lua_tonumberx(L, index, isnum);
}
//--------------------------------------------------------------------------
const void *VeLua::topointer(lua_State *L, VeInt32 index) noexcept
{
	return lua_topointer(L, index);
}
//--------------------------------------------------------------------------
const VeChar8 *VeLua::tostring(lua_State *L, VeInt32 index) noexcept
{
	return lua_tostring(L, index);
}
//--------------------------------------------------------------------------
lua_State *VeLua::tothread(lua_State *L, VeInt32 index) noexcept
{
	return lua_tothread(L, index);
}
//--------------------------------------------------------------------------
void *VeLua::touserdata(lua_State *L, VeInt32 index) noexcept
{
	return lua_touserdata(L, index);
}
//--------------------------------------------------------------------------
void VeLua::createtable(lua_State *L, VeInt32 narr, VeInt32 nrec) noexcept
{
	lua_createtable(L, narr, nrec);
}
//--------------------------------------------------------------------------
void VeLua::newtable(lua_State *L) noexcept
{
	lua_newtable(L);
}
//--------------------------------------------------------------------------
void VeLua::settable(lua_State *L, VeInt32 index) noexcept
{
	lua_settable(L, index);
}
//--------------------------------------------------------------------------
VeInt32 VeLua::gettable(lua_State *L, VeInt32 index) noexcept
{
	return lua_gettable(L, index);
}
//--------------------------------------------------------------------------
void VeLua::setmetatable(lua_State *L, VeInt32 index) noexcept
{
	lua_setmetatable(L, index);
}
//--------------------------------------------------------------------------
VeInt32 VeLua::getmetatable(lua_State *L, VeInt32 index) noexcept
{
	return lua_getmetatable(L, index);
}
//--------------------------------------------------------------------------
VeInt32 VeLua::rawequal(lua_State *L, VeInt32 index1,
	VeInt32 index2) noexcept
{
	return lua_rawequal(L, index1, index2);
}
//--------------------------------------------------------------------------
VeInt32 VeLua::rawget(lua_State *L, VeInt32 index) noexcept
{
	return lua_rawget(L, index);
}
//--------------------------------------------------------------------------
VeInt32 VeLua::rawgeti(lua_State *L, VeInt32 index, VeInt64 n) noexcept
{
	return lua_rawgeti(L, index, n);
}
//--------------------------------------------------------------------------
VeInt32 VeLua::rawgetp(lua_State *L, VeInt32 index, const void *p) noexcept
{
	return lua_rawgetp(L, index, p);
}
//--------------------------------------------------------------------------
VeSizeT VeLua::rawlen(lua_State *L, VeInt32 index) noexcept
{
	return lua_rawlen(L, index);
}
//--------------------------------------------------------------------------
void VeLua::rawset(lua_State *L, VeInt32 index) noexcept
{
	return lua_rawset(L, index);
}
//--------------------------------------------------------------------------
void VeLua::rawseti(lua_State *L, VeInt32 index, VeInt64 i) noexcept
{
	return lua_rawseti(L, index, i);
}
//--------------------------------------------------------------------------
void VeLua::rawsetp(lua_State *L, VeInt32 index, const void *p) noexcept
{
	return lua_rawsetp(L, index, p);
}
//--------------------------------------------------------------------------
void* VeLua::newuserdata(lua_State *L, VeSizeT size) noexcept
{
	return lua_newuserdata(L, size);
}
//--------------------------------------------------------------------------
