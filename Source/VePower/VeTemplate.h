////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeTemplate.h
//  Created:     2015/02/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef VE_PLATFORM_APPLE
typedef std::nullptr_t nullptr_t;
#endif

template <class _Ty>
inline _Ty VeClamp(_Ty a, _Ty l, _Ty h) noexcept
{
	return a < l ? l : (a > h ? h : a);
}

template <class _Class, class _Member>
inline _Class& VeMemberCast(_Member _Class::*offset, const _Member* child) noexcept
{
	return *(_Class*)(void*)((VeSizeT)(child) - (VeSizeT)(&((*(const _Class*)0).*offset)));
}

template <class _Base, class _Der>
inline VeSizeT VeBaseOffset() noexcept
{
	return (VeSizeT)(void*)static_cast<_Base*>(((_Der*)1)) - 1;
}

template<class _Ty>
struct VeIdentity
{
	typedef _Ty type;
};

template<bool, class _Ty1, class _Ty2>
struct VeIf
{
	typedef _Ty2 type;
};

template<class _Ty1, class _Ty2>
struct VeIf<true, _Ty1, _Ty2>
{
	typedef _Ty1 type;
};

template<class _Ty, VeSizeT _Num = 32>
class VeStack : public VeMemObject
{
public:
	VeStack() noexcept
	{

	}

	void push(_Ty _Element) noexcept
	{
		VE_ASSERT(_Pointer < _Num);
		_Stack[_Pointer++] = _Element;
	}

	_Ty pop() noexcept
	{
		VE_ASSERT(_Pointer > 0);
		return _Stack[--_Pointer];
	}

	VeSizeT size() noexcept
	{
		return _Pointer;
	}

	bool empty() noexcept
	{
		return size() == 0;
	}

private:
	_Ty _Stack[_Num];
	VeSizeT _Pointer = 0;

};
