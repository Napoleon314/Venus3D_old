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
				return ve_parser.ParseEnum(val.GetString(), def);
			}
			else if (val.IsUint())
			{
				return ve_parser.ValueToEnum(val.GetUint(), def);
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
				return ve_parser.Parse(val.GetString(), def);
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
	struct ValueTranslator : std::conditional <
		std::is_enum<_Ty>::value,
		EnumTranslator<_Ty>,
		NormalTranslator<_Ty >>::type {};

	template <typename _Ty, typename Encoding, typename Allocator = MemoryPoolAllocator<>>
	_Ty To(GenericValue<Encoding, Allocator>& val, _Ty def) noexcept
	{
		return ValueTranslator<_Ty>::Trans(val, def);
	}

	template <typename Encoding, typename Allocator = MemoryPoolAllocator<>>
	VeVector<std::pair<const VeChar8*, const VeChar8*>> ToStrPairs(
		GenericValue<Encoding, Allocator>& val) noexcept
	{
		VeVector<std::pair<const VeChar8*, const VeChar8*>> kRes;
		if (val.IsObject())
		{
			for (auto it = val.MemberBegin(); it != val.MemberEnd(); ++it)
			{
				if (it->value.IsString())
				{
					kRes.push_back(std::make_pair(it->name.GetString(), it->value.GetString()));
				}
			}
		}
		return kRes;
	}

	template <typename Encoding, typename Allocator = MemoryPoolAllocator<>>
	VeVector<std::pair<const VeChar8*, const VeChar8*>> GetStrPairs(
		GenericValue<Encoding, Allocator>& val, const VeChar8* pcName) noexcept
	{
		auto it = val.FindMember(pcName);
		if (it != val.MemberEnd())
		{
			return ToStrPairs(it->value);
		}
		return VeVector<std::pair<const VeChar8*, const VeChar8*>>();
	}
}

#define VeTo(val,def) rapidjson::To(val,def)
#define VeToStrPairs(val) rapidjson::ToStrPairs(val)
#define VeGetStrPairs(val,n) rapidjson::GetStrPairs(val,n)

VE_POWER_API VeJSONDoc operator "" _C(const VeChar8* pcStr, VeSizeT stNum) noexcept;
