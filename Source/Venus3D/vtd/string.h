////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      vtd
//  File name:   string.h
//  Created:     2016/04/02 by Albert D Yang
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

#include "utility.h"
#include <string.h>

namespace vtd
{
	template <class _Ty>
	struct is_char : std::false_type
	{

	};

	template <>
	struct is_char<char> : std::true_type
	{

	};

	template <>
	struct is_char<wchar_t> : std::true_type
	{

	};

	template <>
	struct is_char<char16_t> : std::true_type
	{

	};

	template <>
	struct is_char<char32_t> : std::true_type
	{

	};

	template <class _Ty>
	inline size_t strlen(const _Ty* _Str) noexcept
	{
		static_assert(is_char<_Ty>::value, "_Ty is not char");
		const _Ty* _Eos = _Str;
		while (*_Eos++);
		return (_Eos - _Str - 1);
	}

	template <>
	inline size_t strlen<char>(const char* _Str) noexcept
	{
		return ::strlen(_Str);
	}

	template <class _Ty>
	inline int strcmp(const _Ty* _Str1, const _Ty* _Str2) noexcept
	{
		static_assert(is_char<_Ty>::value, "_Ty is not char");
		int _Ret = 0;
		while (!(_Ret = unsigned(*_Str1) - unsigned(*_Str2)) && *_Str2)
			++_Str1, ++_Str2;
		if (_Ret < 0)
			_Ret = -1;
		else if (_Ret > 0)
			_Ret = 1;
		return _Ret;
	}

	template <>
	inline int strcmp<char>(const char* _Str1,
		const char* _Str2) noexcept
	{
		return ::strcmp(_Str1, _Str2);
	}

	template <class _Ty>
	inline int strncmp(const _Ty* _Str1, const _Ty* _Str2,
		size_t _Count) noexcept
	{
		static_assert(is_char<_Ty>::value, "_Ty is not char");
		for (size_t x(0); x < _Count; x++)
		{
			if (*_Str1 == 0 || *_Str1 != *_Str2)
			{
				return (unsigned(*_Str1) - unsigned(*_Str2));
			}
			++_Str1;
			++_Str2;
		}
		return 0;
	}

	template <>
	inline int strncmp<char>(const char* _Str1,
		const char* _Str2, size_t _Count) noexcept
	{
		return ::strncmp(_Str1, _Str2, _Count);
	}

	template <class _Ty>
	inline int stricmp(const _Ty* _Str1,
		const _Ty* _Str2) noexcept
	{
		int f, l;
		do
		{
			if (((f = (unsigned)(*(_Str1++))) >= 'A') && (f <= 'Z'))
				f -= ('A' - 'a');

			if (((l = (unsigned)(*(_Str2++))) >= 'A') && (l <= 'Z'))
				l -= ('A' - 'a');
		} while (f && (f == l));
		return(f - l);
	}

	template <class _Ty>
	inline int strnicmp(const _Ty* _Str1, const _Ty* _Str2,
		size_t _Count) noexcept
	{
		int f, l;
		if (_Count)
		{
			do
			{
				if (((f = (unsigned)(*(_Str1++))) >= 'A') && (f <= 'Z'))
					f -= 'A' - 'a';

				if (((l = (unsigned)(*(_Str2++))) >= 'A') && (l <= 'Z'))
					l -= 'A' - 'a';
			} while (--_Count && f && (f == l));

			return(f - l);
		}
		return(0);
	}

	template <class _Ty>
	inline const _Ty* strchr(const _Ty* _Str, _Ty _Char) noexcept
	{
		static_assert(is_char<_Ty>::value, "_Ty is not char");
		while (*_Str && *_Str != _Char)
			++_Str;

		if (*_Str == _Char)
			return _Str;
		return nullptr;
	}

	template <>
	inline const char* strchr<char>(const char* _Str,
		char _Char) noexcept
	{
		return ::strchr(_Str, _Char);
	}

	template <class _Ty>
	inline const _Ty* strrchr(const _Ty* _Str, _Ty _Char) noexcept
	{
		static_assert(is_char<_Ty>::value, "_Ty is not char");
		const _Ty* _Start = _Str;
		while (*_Str++);
		while (--_Str != _Start && *_Str != _Char);
		if (*_Str == _Char)
			return _Str;
		return nullptr;
	}

	template <>
	inline const char* strrchr<char>(const char* _Str, char _Char) noexcept
	{
		return ::strrchr(_Str, _Char);
	}

	template <class _Ty>
	inline const _Ty* strstr(const _Ty* _Str1, const _Ty* _Str2) noexcept
	{
		static_assert(is_char<_Ty>::value, "_Ty is not char");
		const _Ty* cp = _Str1;
		const _Ty *s1, *s2;

		if (!*_Str2)
			return _Str1;

		while (*cp)
		{
			s1 = cp;
			s2 = _Str2;
			while (*s1 && *s2 && !(*s1 - *s2))
				s1++, s2++;
			if (!*s2)
				return(cp);
			cp++;
		}

		return nullptr;
	}

	template <>
	inline const char* strstr<char>(const char* _Str1,
		const char* _Str2) noexcept
	{
		return ::strstr(_Str1, _Str2);
	}

	template <class _Ty>
	inline _Ty* strstr(_Ty* _Str1, const _Ty* _Str2) noexcept
	{
		static_assert(is_char<_Ty>::value, "_Ty is not char");
		_Ty* cp = _Str1;
		_Ty *s1, *s2;

		if (!*_Str2)
			return _Str1;

		while (*cp)
		{
			s1 = cp;
			s2 = _Str2;
			while (*s1 && *s2 && !(*s1 - *s2))
				s1++, s2++;
			if (!*s2)
				return(cp);
			cp++;
		}

		return nullptr;
	}

	template <>
	inline char* strstr<char>(char* _Str1,
		const char* _Str2) noexcept
	{
		return ::strstr(_Str1, _Str2);
	}

	template <class _Ty>
	inline _Ty* strtok(_Ty* _Str, const _Ty* _Delimit,
		_Ty** _Context) noexcept
	{
		static_assert(is_char<_Ty>::value, "_Ty is not char");
		_Ty* _StrStart = _Str;
		if ((!_StrStart) && _Context)
		{
			_StrStart = *_Context;
		}
		if (_StrStart)
		{
			size_t _Len = strlen(_Delimit);
			_Ty* pcTemp = strstr(_StrStart, _Delimit);
			if (pcTemp)
			{
				*pcTemp = '\0';
				if (_Context) *_Context = pcTemp += _Len;
				return _StrStart;
			}
			else if (*_StrStart)
			{
				if (_Context) *_Context = nullptr;
				return _StrStart;
			}
			else
			{
				if (_Context) *_Context = nullptr;
				return nullptr;
			}
		}
		return nullptr;
	}
}
