////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Parser
//  File name:   VeStringParser.h
//  Created:     2016/08/12 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#pragma once

namespace venus
{
	template <class _Ty>
	struct str_to_i32
	{
		static bool transfer(const char* _Str, _Ty& _Out) noexcept
		{
			if (_Str && isalnum(*_Str))
			{
				_Out = (_Ty)atoi(_Str);
				return true;
			}
			return false;
		}
	};

	template <class _Ty>
	struct str_to_i64
	{
		static bool transfer(const char* _Str, _Ty& _Out) noexcept
		{
			if (_Str && isalnum(*_Str))
			{
				_Out = (_Ty)atoll(_Str);
				return true;
			}
			return false;
		}
	};

	template <class _Ty>
	struct str_to_float
	{
		static bool transfer(const char* _Str, _Ty& _Out) noexcept
		{
			if (_Str && (isalnum(*_Str) || (*_Str == '.')))
			{
				_Out = (_Ty)atof(_Str);
				return true;
			}
			return false;
		}
	};

	struct str_to_bool
	{
		static bool transfer(const char* _Str, bool& _Out) noexcept
		{
			if (_Str)
			{
				if (isalnum(*_Str))
				{
					_Out = atoi(_Str) != 0;
					return true;
				}
				else if (!vtd::stricmp(_Str, "true"))
				{
					_Out = true;
					return true;
				}
				else if (!vtd::stricmp(_Str, "false"))
				{
					_Out = false;
					return true;
				}
			}
			return false;
		}
	};
	
	template <class _Ty>
	struct str_to_arithmetic : std::conditional <
		std::is_same<_Ty, bool>::value, str_to_bool,
		typename std::conditional<std::is_floating_point<_Ty>::value,
		str_to_float<_Ty>, typename std::conditional<sizeof(_Ty) <= 4,
		str_to_i32<_Ty>, str_to_i64<_Ty>>::type>::type>::type
	{

	};

	struct str_to_str
	{
		static bool transfer(const char* _Str, const char*& _Out) noexcept
		{
			if (_Str)
			{
				_Out = _Str;
				return true;
			}
			return false;
		}
	};

	template<class _Ty,
		class _Hasher = vtd::string::hasher,
		class _Keyeq = std::equal_to<vtd::string>>
		class strmap : public std::unordered_map<vtd::string, _Ty, _Hasher, _Keyeq> {};

	template <class _Ty>
	struct enum_str_map : strmap<_Ty>
	{
		enum_str_map() noexcept;
	};

	template <class _Ty>
	struct str_to_enum
	{
		static bool transfer(const char* _Str, _Ty& _Out) noexcept
		{
			auto it = enum_map.find(_Str);
			if (it != enum_map.end())
			{
				_Out = it->second;
				return true;
			}
			return false;
		}

		static enum_str_map<_Ty> enum_map;
	};

	template<class _Ty> enum_str_map<_Ty> str_to_enum<_Ty>::enum_map;

	template <class _Ty>
	struct str_to_unknown_type;

	template <class _Ty>
	struct str_to_type : std::conditional <
		std::is_enum<_Ty>::value, str_to_enum<_Ty>,
		typename std::conditional <std::is_same<_Ty, const char*>::value,
		str_to_str, typename std::conditional <std::is_arithmetic<_Ty>::value,
		str_to_arithmetic<_Ty>, str_to_unknown_type<_Ty>>::type>::type>::type
	{

	};

	template <class _Ty>
	_Ty convert_str(const char* _Str, _Ty _Default) noexcept
	{
		_Ty _Val;
		if (str_to_type<_Ty>::transfer(_Str, _Val))
		{
			return _Val;
		}
		else
		{
			return _Default;
		}
	}

	template <class _Ty>
	bool transfer_str(const char* _Str, _Ty& _Out) noexcept
	{
		return str_to_type<_Ty>::transfer(_Str, _Out);
	}
}

#define VE_PARSER_ENUM(t) template <> venus::enum_str_map<t>::enum_str_map() noexcept
#define VE_PARSER_ENUM_ITEM(n,v) insert(std::make_pair(n, v));
