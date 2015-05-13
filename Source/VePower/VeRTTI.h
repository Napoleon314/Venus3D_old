////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeRTTI.h
//  Created:     2015/02/21 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VeRTTIBase : public VeMemObject
{
public:
	typedef std::pair<const VeRTTIBase*, VeSizeT> _Base;

	VeRTTIBase(const VeChar8* pcName) noexcept
		: m_pcName(pcName)
	{

	}

	~VeRTTIBase() noexcept
	{
		m_pcName = nullptr;
		m_stBaseNum = 0;
	}

	const _Base* begin() const noexcept
	{
		return m_pkBaseArray;
	}

	const _Base* end() const noexcept
	{
		return m_pkBaseArray + m_stBaseNum;
	}

	const VeChar8* GetName() const noexcept
	{
		return m_pcName;
	}

	VeSizeT GetBaseNum() const noexcept
	{
		return m_stBaseNum;
	}

	VeSizeT GetPathFrom(const VeRTTIBase* pkFrom, VeSizeT stBase = 0) const noexcept
	{
		if (this == pkFrom)
		{
			return stBase;
		}
		else
		{
			for (auto i : *pkFrom)
			{
				VeSizeT stTemp = GetPathFrom(i.first, stBase + i.second);
				if (stTemp != VE_SIZET_MAX)
				{
					return stTemp;
				}
			}
		}
		return VE_SIZET_MAX;
	}

	template<class _Ty1, class _Ty2>
	static inline bool IsExactKindOf(_Ty2* pkObject) noexcept
	{
		return pkObject ? pkObject->IsExactKindOf(&_Ty1::ms_kRTTI) : false;
	}

	template<class _Ty1, class _Ty2>
	static inline bool IsKindOf(_Ty2* pkObject) noexcept
	{
		return pkObject ? pkObject->IsKindOf(&_Ty1::ms_kRTTI) : false;
	}

	template<class _Ty1, class _Ty2>
	static inline _Ty1* DynamicCast(_Ty2* pkObject) noexcept
	{
		return pkObject ? (_Ty1*)pkObject->_Ty2::DynamicCast(&_Ty1::ms_kRTTI) : nullptr;
	}

protected:
	const VeChar8* m_pcName;
	_Base* m_pkBaseArray = nullptr;
	VeSizeT m_stBaseNum = 0;

};

template<class... _Types>
struct VeRTTIUnpacker;

template<>
struct VeRTTIUnpacker<>
{
	template<class _Ty>
	static void Set(VeIdentity<_Ty>, VeRTTIBase::_Base* pkDesc) noexcept
	{

	}
};

template<class _This, class... _Rest>
struct VeRTTIUnpacker<_This, _Rest...>
{
	template<class _Ty>
	static void Set(VeIdentity<_Ty> kId, VeRTTIBase::_Base* pkDesc) noexcept
	{
		static_assert(__is_base_of(_This, _Ty),
			"init rtti with wrong base");
		pkDesc->first = &_This::ms_kRTTI;
		pkDesc->second = VeBaseOffset<_This, _Ty>();
		VeRTTIUnpacker<_Rest...>::Set(kId, pkDesc + 1);
	}
};

template<class _Der, class... _Bases>
class VeRTTI : public VeRTTIBase
{
public:
	VeRTTI(const VeChar8* pcName) noexcept
		: VeRTTIBase(pcName)
	{
		static_assert(sizeof...(_Bases) > 0, "init rtti with unknown error");
		VeRTTIUnpacker<_Bases...>::Set(VeIdentity<_Der>(), m_akBaseArray);
		m_pkBaseArray = m_akBaseArray;
		m_stBaseNum = sizeof...(_Bases);
	}

private:
	_Base m_akBaseArray[sizeof...(_Bases)];

};

template<class _Der, class... _Bases>
struct VeRTTISelect : VeIf<sizeof...(_Bases) == 0, VeRTTIBase, VeRTTI<_Der, _Bases...>>
{

};

#define VeRTTIDecl(classname,...) \
	public: \
		static const VeRTTISelect<classname,##__VA_ARGS__>::type ms_kRTTI; \
		virtual const VeRTTIBase* GetRTTI() const noexcept { return &ms_kRTTI; } \
		bool IsExactKindOf(const VeRTTIBase* pkRTTI) const noexcept \
		{ \
			return (GetRTTI() == pkRTTI); \
		} \
		void* DynamicCast(const VeRTTIBase* pkRTTI) const noexcept \
		{ \
			VeSizeT stTemp = pkRTTI->GetPathFrom(GetRTTI()); \
			if(stTemp == VE_SIZET_MAX) \
			{ \
				return nullptr; \
			} \
			else \
			{ \
				stTemp = (VeSizeT)(void*)this + stTemp; \
				stTemp -= classname::ms_kRTTI.GetPathFrom(GetRTTI()); \
				return (void*)stTemp; \
			} \
		} \
		bool IsKindOf(const VeRTTIBase* pkRTTI) const noexcept \
		{ \
			return DynamicCast(pkRTTI) ? true : false; \
		}

#define VeRTTIImpl(classname,...) \
	const VeRTTISelect<classname,##__VA_ARGS__>::type classname::ms_kRTTI(#classname)

#define VeIsExactKindOf(classname, pkObject) \
	VeRTTIBase::IsExactKindOf<classname>(pkObject)

#define VeIsKindOf(classname, pkObject) \
	VeRTTIBase::IsKindOf<classname>(pkObject)

#define VeDynamicCast(classname, pkObject) \
	VeRTTIBase::DynamicCast<classname>(pkObject)
