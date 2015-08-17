////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeLua.inl
//  Created:     2015/03/04 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeLua::operator lua_State* () noexcept
{
	return m_pkState;
}
//--------------------------------------------------------------------------
inline lua_State* VeLua::GetLuaState() noexcept
{
	return m_pkState;
}
//--------------------------------------------------------------------------
inline VeInt32 VeLua::DoString(const VeChar8* pcStr) noexcept
{
	return dostring(m_pkState, pcStr);
}
//--------------------------------------------------------------------------
inline VeInt32 VeLua::DoFile(const VeChar8* pcPath) noexcept
{
	return dofile(m_pkState, pcPath);
}
//--------------------------------------------------------------------------
inline void VeLua::SetTop(VeInt32 i32Index) noexcept
{
	settop(m_pkState, i32Index);
}
//--------------------------------------------------------------------------
inline VeInt32 VeLua::GetTop() noexcept
{
	return gettop(m_pkState);
}
//--------------------------------------------------------------------------
inline void VeLua::Pop(VeInt32 i32Num) noexcept
{
	pop(m_pkState, i32Num);
}
//--------------------------------------------------------------------------
inline VeLua::ValueType VeLua::GetType(VeInt32 i32Index) noexcept
{
	return (ValueType)type(m_pkState, i32Index);
}
//--------------------------------------------------------------------------
inline VeInt32 VeLua::PushValue(const VeChar8* pcName) noexcept
{
	return pushvalue(m_pkState, pcName);
}
//--------------------------------------------------------------------------
inline VeInt32 VeLua::Call(VeInt32 i32NumArgs,
	VeInt32 i32NumResults) noexcept
{
	return pcall(m_pkState, i32NumArgs, i32NumResults);
}
//--------------------------------------------------------------------------
