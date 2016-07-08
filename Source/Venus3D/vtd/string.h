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
#ifdef __APPLE__
#   include <stdlib.h>
#else
#   include <malloc.h>
#endif
#include <string.h>
#include <string>

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
		static_assert(is_char<_Ty>::value, "_Ty has to be a kind of char");
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
		static_assert(is_char<_Ty>::value, "_Ty has to be a kind of char");
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
		static_assert(is_char<_Ty>::value, "_Ty has to be a kind of char");
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
		static_assert(is_char<_Ty>::value, "_Ty has to be a kind of char");
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
		static_assert(is_char<_Ty>::value, "_Ty has to be a kind of char");
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
		static_assert(is_char<_Ty>::value, "_Ty has to be a kind of char");
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
		static_assert(is_char<_Ty>::value, "_Ty has to be a kind of char");
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
	inline _Ty* strtok(_Ty* _Str, const _Ty* _Delimit,
		_Ty** _Context) noexcept
	{
		static_assert(is_char<_Ty>::value, "_Ty has to be a kind of char");
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

	template <class _Ty>
	inline _Ty* strtrim(_Ty* _Str) noexcept
	{
		static_assert(is_char<_Ty>::value, "_Ty has to be a kind of char");
		size_t i1 = strlen(_Str);
		size_t i0(0);
		while (i0 < i1 && isspace((int)_Str[i0]))
			++i0;
		while (i1 > 0 && isspace((int)_Str[i1 - 1]))
			--i1;
		_Str[i1] = 0;
		return _Str + i0;
	}

	template <class _Ty>
	inline _Ty* strline(_Ty* _Str, _Ty** _Context) noexcept
	{
		static_assert(is_char<_Ty>::value, "_Ty has to be a kind of char");
		_Ty* _StrStart = _Str;
		if ((!_StrStart) && _Context)
		{
			_StrStart = *_Context;
		}
		if (_StrStart)
		{
			_Ty* pcTemp = _StrStart;
			while (pcTemp)
			{
				if ((*pcTemp == '\r') || (*pcTemp == '\n'))
				{
					break;
				}
				else if (*pcTemp == '\0')
				{
					pcTemp = nullptr;
					break;
				}
				else
				{
					++pcTemp;
				}
			}
			if (pcTemp)
			{
				*(pcTemp++) = '\0';
				if (_Context)
				{
					if (*pcTemp == '\n') ++pcTemp;
					*_Context = pcTemp;
				}
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

#ifndef VTD_STR_TAB_MASK
#	define VTD_STR_TAB_MASK (0x7FF)
#endif

	template <class _Ty>
	class string
	{
		static_assert(is_char<_Ty>::value, "_Ty has to be a kind of char");
	public:
		typedef _Ty value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef pointer string_handle;

		string() noexcept = default;		

		string(const char* str) noexcept
		{
			holder = add_string(str);
		}

		string(const char* str, size_t len) noexcept
		{
			holder = add_string(str, (uint32_t)len);
		}

		string(const string& copy) noexcept
		{
			if (copy.holder)
			{
				inc_refcount(copy.holder);
				holder = copy.holder;
			}			
		}

		string(string&& move) noexcept
		{
			holder = move.holder;
			move.holder = nullptr;
		}

		~string() noexcept
		{
			dec_refcount(holder);
		}

		operator const const_pointer() const noexcept
		{
			return holder;
		}

		static void clear() noexcept
		{
			for (auto& vec : hash_array)
			{
				vec.clear();
			}
		}
	
	protected:
		string_handle holder = nullptr;

	private:
		static const string_handle add_string(const_pointer str) noexcept
		{
			if (!str) return nullptr;
			uint32_t len = (uint32_t)strlen(str);
			return add_string(str, len);
		}

		static const string_handle add_string(const_pointer str, uint32_t len) noexcept
		{
			assert(len < UINT16_MAX);
			uint32_t hash = hash_func(str, len);
			string_handle handle = find_string(str, len, hash);
			if (handle)
			{
				inc_refcount(handle);
			}
			else
			{
				uint32_t alloc_len = uint32_t((len + 1) * sizeof(value_type)) + 8;
				alloc_len = (alloc_len + 7) & (~7);
				void* mem = VeMalloc(alloc_len);
				handle = (string_handle)((uint8_t*)mem + 8);
				((uint16_t*)mem)[0] = 2;
				((uint16_t*)mem)[1] = uint16_t(len);
				((uint32_t*)mem)[1] = hash;
				memcpy(handle, str, len * sizeof(value_type));
				handle[len] = 0;
				insert_string(handle, hash);
			}
			return handle;
		}

		static size_t get_num_string() noexcept
		{
			return str_num;
		}

		static size_t get_bucket_size(size_t which) noexcept
		{
			assert(which <= VTD_STR_TAB_MASK);
			return hash_array[which].size();
		}

		static constexpr size_t get_total_num_of_buckets() noexcept
		{
			return VTD_STR_TAB_MASK + 1;
		}

		static size_t get_max_bucket_size() noexcept
		{
			size_t res = 0;
			for (auto& vec : hash_array)
			{
				res = max(res, vec.size());
			}
			return res;
		}

		static const string_handle find_string(const_pointer str, uint32_t len, uint32_t hash) noexcept
		{
			auto& str_array = hash_array[hash & VTD_STR_TAB_MASK];
			for (auto s : str_array)
			{
				if (s == str || (get_string(s) && get_length(s) == len
					&& strcmp(get_string(s), str) == 0))
				{
					return s;
				}
			}
			return nullptr;
		}

		static void insert_string(const string_handle& handle, uint32_t hash) noexcept
		{
			assert(handle && validate(handle));
			hash_array[hash & VTD_STR_TAB_MASK].push_back(handle);
			++str_num;
		}

		static void remove_string(const string_handle& handle, uint32_t hash) noexcept
		{
			const_pointer s = get_string(handle);			
			auto& str_array = hash_array[hash & VTD_STR_TAB_MASK];
			for (size_t i(0); i < str_array.size(); ++i)
			{
				if (str_array[i] == s)
				{
					uint16_t* mem = (uint16_t*)get_real_start(handle);
					assert(get_refcount(handle) > 0);
					if ((--mem[0]) == 0)
					{
						assert(!get_refcount(handle));
						VeFree(get_real_start(handle));
						if (i < (str_array.size() - 1))
						{
							str_array[i] = str_array.back();
						}
						str_array.pop_back();
						--str_num;
					}
					break;
				}
			}
		}

		static pointer get_real_start(const string_handle& handle) noexcept
		{
			assert(handle);
			return (pointer)((uint8_t*)handle - 8);
		}

		static uint32_t hash_func(const_pointer str, uint32_t len) noexcept
		{
			uint32_t hash = 0;
			uint32_t unroll = (uint32_t)(len & ~0x3);
			for (uint32_t ui = 0; ui < unroll; ui += 4)
			{
				uint32_t hash0 = *str;
				hash = (hash << 5) + hash + hash0;
				uint32_t hash1 = *(str + 1);
				hash = (hash << 5) + hash + hash1;
				uint32_t hash2 = *(str + 2);
				hash = (hash << 5) + hash + hash2;
				uint32_t hash3 = *(str + 3);
				hash = (hash << 5) + hash + hash3;
				str += 4;
			}
			while (*str)
				hash = (hash << 5) + hash + *str++;
			return hash;
		}

		static void inc_refcount(string_handle& handle) noexcept
		{
			if (handle)
			{
				assert(validate(handle));
				uint16_t* mem = (uint16_t*)get_real_start(handle);
				assert(mem[0] < UINT16_MAX);
				++mem[0];
			}
		}

		static void dec_refcount(string_handle& handle) noexcept
		{
			if (handle)
			{
				assert(validate(handle));
				uint16_t* mem = (uint16_t*)get_real_start(handle);
				uint32_t hash = get_hash_value(handle);
				if ((--mem[0]) > 1) return;
				remove_string(handle, hash);
			}
		}

		static const_pointer get_string(const string_handle& handle) noexcept
		{
			assert(handle && validate(handle));
			return (const_pointer)handle;
		}

		static uint32_t get_length(const string_handle& handle) noexcept
		{
			assert(handle && validate(handle));
			uint16_t* mem = (uint16_t*)get_real_start(handle);
			return mem[1];
		}

		static uint32_t get_hash_value(const string_handle& handle) noexcept
		{
			assert(handle && validate(handle));
			uint32_t* mem = (uint32_t*)get_real_start(handle);
			return mem[1];
		}

		static uint32_t get_refcount(const string_handle& handle) noexcept
		{
			assert(handle && validate(handle));
			uint16_t* mem = (uint16_t*)get_real_start(handle);
			return mem[0];
		}

		static bool validate(const string_handle& handle) noexcept
		{
			if (!handle) return true;
			uint16_t* mem = (uint16_t*)get_real_start(handle);
			return size_t(mem[1]) == strlen(handle);
		}
	
		static size_t str_num;
		static vector<string_handle> hash_array[VTD_STR_TAB_MASK + 1];
		static spin_lock lock;

	};

	template<class _Ty> size_t string<_Ty>::str_num = 0;
	template<class _Ty> vector<_Ty*> string<_Ty>::hash_array[VTD_STR_TAB_MASK + 1];
	template<class _Ty> spin_lock string<_Ty>::lock;

#ifdef VTD_STR_TAB_MASK
#	undef VTD_STR_TAB_MASK
#endif

}
