////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeJSON.h
//  Created:     2015/08/24 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <rapidjson/document.h>

typedef rapidjson::GenericDocument<rapidjson::UTF8<> > VeJSONDoc;
typedef rapidjson::GenericValue<rapidjson::UTF8<> > VeJSONValue;

template <class _Ty>
inline _Ty VeToEnum(VeJSONValue& kValue, const VeChar8* pcName,
	VeStringMap<_Ty>& kMap, _Ty tDefault) noexcept
{
	auto iter = kValue.FindMember(pcName);
	if (iter != kValue.MemberEnd() && iter->value.IsString())
	{
		auto iterVal = kMap.find(iter->value.GetString());
		if (iterVal != kMap.end())
		{
			return iterVal->second;
		}
	}
	return tDefault;
}

template <class _Ty>
inline _Ty VeToEnum(VeJSONValue& kValue, VeStringMap<_Ty>& kMap, _Ty tDefault) noexcept
{
	if (kValue.IsString())
	{
		auto iterVal = kMap.find(kValue.GetString());
		if (iterVal != kMap.end())
		{
			return iterVal->second;
		}
	}
	return tDefault;
}

//inline const VeChar8* VeToString(VeJSONValue& kValue, const VeChar8* pcName,
//	const VeChar8* pcDefault = "") noexcept
//{
//	auto iter = kValue.FindMember(pcName);
//	if (iter != kValue.MemberEnd() && iter->value.IsString())
//	{
//		return iter->value.GetString();
//	}
//	return pcDefault;
//}

//inline bool VeToBoolean(VeJSONValue& kValue, const VeChar8* pcName,
//	bool bDefault = false) noexcept
//{
//	auto iter = kValue.FindMember(pcName);
//	if (iter != kValue.MemberEnd() && iter->value.IsBool())
//	{
//		return iter->value.GetBool();
//	}
//	return bDefault;
//}

//inline VE_BOOL VeToBool(VeJSONValue& kValue, const VeChar8* pcName,
//	VE_BOOL bDefault = VE_FALSE) noexcept
//{
//	auto iter = kValue.FindMember(pcName);
//	if (iter != kValue.MemberEnd() && iter->value.IsBool())
//	{
//		return iter->value.GetBool() ? VE_TRUE : VE_FALSE;
//	}
//	return bDefault;
//}

#define VE_JSON_EXT { "json", "plist" }

namespace rapidjson
{
	template <class _Ty>
	struct EnumTranslator
	{
		static_assert(std::is_enum<_Ty>::value, "_Ty has to be a enum");

		template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
		static _Ty Trans(GenericValue<Encoding, Allocator>& val, _Ty def) noexcept
		{
			if (val.IsString())
			{
				return VeEnumParser<_Ty>::GetSingleton().EnumToStr(val.GetString(), def);
			}
			else if (val.IsUint())
			{
				return VeEnumParser<_Ty>::GetSingleton().ValueToEnum(val.GetUint(), def);
			}
			return def;
		}
	};

	template <class _Ty>
	struct NormalTranslator
	{
		template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
		static _Ty Trans(GenericValue<Encoding, Allocator>& val, _Ty def) noexcept
		{
			if (val.IsString())
			{
				return ve_parser.Parse<_Ty>(val.GetString(), def);
			}
			return def;
		}
	};

	template <>
	struct NormalTranslator<VeUInt32>
	{
		template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
		static VeUInt32 Trans(GenericValue<Encoding, Allocator>& val, VeUInt32 def) noexcept
		{
			if (val.IsUint())
			{
				return val.GetUint();
			}
			return def;
		}
	};

	template <>
	struct NormalTranslator<VeUInt64>
	{
		template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
		static VeUInt64 Trans(GenericValue<Encoding, Allocator>& val, VeUInt64 def) noexcept
		{
			if (val.IsUint64())
			{
				return val.GetUint64();
			}
			return def;
		}
	};

	template <>
	struct NormalTranslator<VeInt32>
	{
		template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
		static VeInt32 Trans(GenericValue<Encoding, Allocator>& val, VeInt32 def) noexcept
		{
			if (val.IsInt())
			{
				return val.GetInt();
			}
			return def;
		}
	};

	template <>
	struct NormalTranslator<VeInt64>
	{
		template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
		static VeInt64 Trans(GenericValue<Encoding, Allocator>& val, VeInt64 def) noexcept
		{
			if (val.IsInt64())
			{
				return val.GetInt64();
			}
			return def;
		}
	};

	template <>
	struct NormalTranslator<VeFloat32>
	{
		template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
		static VeFloat32 Trans(GenericValue<Encoding, Allocator>& val, VeFloat32 def) noexcept
		{
			if (val.IsDouble())
			{
				return (VeFloat32)val.GetDouble();
			}
			return def;
		}
	};

	template <>
	struct NormalTranslator<VeFloat64>
	{
		template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
		static VeFloat64 Trans(GenericValue<Encoding, Allocator>& val, VeFloat64 def) noexcept
		{
			if (val.IsDouble())
			{
				return val.GetDouble();
			}
			return def;
		}
	};

	template <>
	struct NormalTranslator<bool>
	{
		template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
		static bool Trans(GenericValue<Encoding, Allocator>& val, bool def) noexcept
		{
			if (val.IsBool())
			{
				return val.GetBool();
			}
			return def;
		}
	};

	template <>
	struct NormalTranslator<const VeChar8*>
	{
		template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
		static const VeChar8* Trans(GenericValue<Encoding, Allocator>& val, const VeChar8* def) noexcept
		{
			if (val.IsString())
			{
				return val.GetString();
			}
			return def;
		}
	};

	template <>
	struct NormalTranslator<VeFixedString>
	{
		template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
		static VeFixedString Trans(GenericValue<Encoding, Allocator>& val, VeFixedString def) noexcept
		{
			if (val.IsString())
			{
				return val.GetString();
			}
			return def;
		}
	};

	template <class _Ty>
	struct ValueTranslator : std::_If <
		std::is_enum<_Ty>::value,
		EnumTranslator<_Ty>,
		NormalTranslator<_Ty >>::type {};
}

int VeJSON() noexcept;
