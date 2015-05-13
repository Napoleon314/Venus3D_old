////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeLua.h
//  Created:     2015/03/03 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

struct lua_State;

class VE_POWER_API VeLua : public VeRefObject
{
public:
	enum ValueType
	{
		VAL_TNONE = -1,
		VAL_TNIL = 0,
		VAL_TBOOLEAN,
		VAL_TLIGHTUSERDATA,
		VAL_TNUMBER,
		VAL_TSTRING,
		VAL_TTABLE,
		VAL_TFUNCTION,
		VAL_TUSERDATA,
		VAL_TTHREAD,
		VAL_MAX
	};

	typedef VeInt32(*CFunc) (lua_State *L);

	VeLua() noexcept;

	~VeLua() noexcept;

	inline operator lua_State* () noexcept;

	inline lua_State* GetLuaState() noexcept;

	inline VeInt32 DoString(const VeChar8* pcStr) noexcept;

	inline VeInt32 DoFile(const VeChar8* pcPath) noexcept;

	inline void SetTop(VeInt32 i32Index) noexcept;

	inline VeInt32 GetTop() noexcept;

	inline void Pop(VeInt32 i32Num = 1) noexcept;

	inline ValueType GetType(VeInt32 i32Index) noexcept;

	inline VeInt32 PushValue(const VeChar8* pcName) noexcept;

	inline VeInt32 Call(VeInt32 i32NumArgs,
		VeInt32 i32NumResults = -1) noexcept;

	static VeInt32 upvalueindex(VeInt32 index) noexcept;

	static VeInt32 pushvalue(lua_State *L, const VeChar8 *str) noexcept;

	static VeInt32 dostring(lua_State *L, const VeChar8 *str) noexcept;

	static VeInt32 dofile(lua_State *L, const VeChar8 *path) noexcept;

	static VeInt32 pcall(lua_State *L, VeInt32 nargs, VeInt32 nresults = -1,
		VeInt32 msgh = 0) noexcept;

	static void settop(lua_State *L, int index) noexcept;

	static VeInt32 gettop(lua_State *L) noexcept;

	static void pop(lua_State *L, VeInt32 n) noexcept;

	static VeInt32 type(lua_State *L, VeInt32 index) noexcept;

	static bool isboolean(lua_State *L, VeInt32 index) noexcept;

	static bool iscfunction(lua_State *L, VeInt32 index) noexcept;

	static bool isfunction(lua_State *L, VeInt32 index) noexcept;

	static bool isinteger(lua_State *L, VeInt32 index) noexcept;

	static bool islightuserdata(lua_State *L, VeInt32 index) noexcept;

	static bool isnil(lua_State *L, VeInt32 index) noexcept;

	static bool isnone(lua_State *L, VeInt32 index) noexcept;

	static bool isnoneornil(lua_State *L, VeInt32 index) noexcept;

	static bool isnumber(lua_State *L, VeInt32 index) noexcept;

	static bool isstring(lua_State *L, VeInt32 index) noexcept;

	static bool istable(lua_State *L, VeInt32 index) noexcept;

	static bool isthread(lua_State *L, VeInt32 index) noexcept;

	static bool isuserdata(lua_State *L, VeInt32 index) noexcept;

	static void pushboolean(lua_State *L, bool val) noexcept;

	static void pushcclosure(lua_State *L, CFunc fn, VeInt32 n) noexcept;

	static void pushcfunction(lua_State *L, CFunc f) noexcept;

	static const VeChar8 *pushfstring(lua_State *L,
		const VeChar8 *fmt, ...) noexcept;

	static void pushglobaltable(lua_State *L) noexcept;

	static void pushinteger(lua_State *L, VeInt64 n) noexcept;

	static void pushlightuserdata(lua_State *L, void *p) noexcept;

	static const VeChar8 *pushlstring(lua_State *L,
		const VeChar8 *s, VeSizeT len) noexcept;

	static void pushnil(lua_State *L) noexcept;

	static void pushnumber(lua_State *L, VeFloat64 n) noexcept;

	static const VeChar8 *pushstring(lua_State *L,
		const VeChar8 *s) noexcept;

	static VeInt32 pushthread(lua_State *L) noexcept;

	static void pushvalue(lua_State *L, VeInt32 index) noexcept;

	static const VeChar8 *pushvfstring(lua_State *L,
		const VeChar8 *fmt, va_list argp) noexcept;

	static bool toboolean(lua_State *L, VeInt32 index) noexcept;

	static CFunc tocfunction(lua_State *L, VeInt32 index) noexcept;

	static VeInt64 tointeger(lua_State *L, VeInt32 index) noexcept;

	static VeInt64 tointegerx(lua_State *L, VeInt32 index,
		VeInt32 *isnum) noexcept;

	static const VeChar8 *tolstring(lua_State *L, VeInt32 index,
		VeSizeT *len) noexcept;

	static VeFloat64 tonumber(lua_State *L, VeInt32 index) noexcept;

	static VeFloat64 tonumberx(lua_State *L, VeInt32 index,
		VeInt32 *isnum) noexcept;

	static const void *topointer(lua_State *L, VeInt32 index) noexcept;

	static const VeChar8 *tostring(lua_State *L, VeInt32 index) noexcept;

	static lua_State *tothread(lua_State *L, VeInt32 index) noexcept;

	static void *touserdata(lua_State *L, VeInt32 index) noexcept;

	static void createtable(lua_State *L, VeInt32 narr, VeInt32 nrec) noexcept;

	static void newtable(lua_State *L) noexcept;

	static void settable(lua_State *L, VeInt32 index) noexcept;

	static VeInt32 gettable(lua_State *L, VeInt32 index) noexcept;

	static void setmetatable(lua_State *L, VeInt32 index) noexcept;

	static VeInt32 getmetatable(lua_State *L, VeInt32 index) noexcept;

	static VeInt32 rawequal(lua_State *L, VeInt32 index1, VeInt32 index2) noexcept;

	static VeInt32 rawget(lua_State *L, VeInt32 index) noexcept;

	static VeInt32 rawgeti(lua_State *L, VeInt32 index, VeInt64 n) noexcept;

	static VeInt32 rawgetp(lua_State *L, VeInt32 index, const void *p) noexcept;

	static VeSizeT rawlen(lua_State *L, VeInt32 index) noexcept;

	static void rawset(lua_State *L, VeInt32 index) noexcept;

	static void rawseti(lua_State *L, VeInt32 index, VeInt64 i) noexcept;

	static void rawsetp(lua_State *L, VeInt32 index, const void *p) noexcept;

	static void* newuserdata(lua_State *L, VeSizeT size) noexcept;

private:
	lua_State* m_pkState = nullptr;

};

VeSmartPointer(VeLua);

#include "VeLua.inl"
