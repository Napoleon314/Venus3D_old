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

namespace venus
{
	template<class _Ty>
	struct jton;

	template<>
	struct jton<VeUInt32>
	{
		static VeUInt32 ToNum(VeJSONValue& kValue) noexcept
		{
			return (VeUInt32)kValue.GetInt();
		}
	};

	template<>
	struct jton<VeUInt64>
	{
		static VeUInt64 ToNum(VeJSONValue& kValue) noexcept
		{
			return (VeUInt64)kValue.GetInt64();
		}
	};

	template<>
	struct jton<VeInt32>
	{
		static VeInt32 ToNum(VeJSONValue& kValue) noexcept
		{
			return kValue.GetInt();
		}
	};

	template<>
	struct jton<VeInt64>
	{
		static VeInt64 ToNum(VeJSONValue& kValue) noexcept
		{
			return kValue.GetInt64();
		}
	};

	template<>
	struct jton<VeFloat32>
	{
		static VeFloat32 ToNum(VeJSONValue& kValue) noexcept
		{
			return (VeFloat32)kValue.GetDouble();
		}
	};

	template<>
	struct jton<VeFloat64>
	{
		static VeFloat64 ToNum(VeJSONValue& kValue) noexcept
		{
			return kValue.GetDouble();
		}
	};
};

template <class _Ty>
inline _Ty VeToNumber(VeJSONValue& kValue, const VeChar8* pcName,
	_Ty tDefault) noexcept
{
	auto iter = kValue.FindMember(pcName);
	if (iter != kValue.MemberEnd() && iter->value.IsNumber())
	{
		return venus::jton<_Ty>::ToNum(iter->value);
	}
	return tDefault;
}

inline const VeChar8* VeToString(VeJSONValue& kValue, const VeChar8* pcName,
	const VeChar8* pcDefault = "") noexcept
{
	auto iter = kValue.FindMember(pcName);
	if (iter != kValue.MemberEnd() && iter->value.IsString())
	{
		return iter->value.GetString();
	}
	return pcDefault;
}

inline bool VeToBoolean(VeJSONValue& kValue, const VeChar8* pcName,
	bool bDefault = false) noexcept
{
	auto iter = kValue.FindMember(pcName);
	if (iter != kValue.MemberEnd() && iter->value.IsBool())
	{
		return iter->value.GetBool();
	}
	return bDefault;
}

inline VE_BOOL VeToBool(VeJSONValue& kValue, const VeChar8* pcName,
	VE_BOOL bDefault = VE_FALSE) noexcept
{
	auto iter = kValue.FindMember(pcName);
	if (iter != kValue.MemberEnd() && iter->value.IsBool())
	{
		return iter->value.GetBool() ? VE_TRUE : VE_FALSE;
	}
	return bDefault;
}

#define VE_JSON_EXT { "json", "plist" }

int VeJSON() noexcept;
