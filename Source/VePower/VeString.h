////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeString.h
//  Created:     2015/02/23 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#define VeIsAlpha isalpha
#define VeIsSpace isspace
#define VeIsAlnum isalnum
#define VeToLower tolower
#define VeToSupper toupper

VE_POWER_API VeChar8* VeStrcpy(VeChar8* pcDest, VeSizeT stDestSize,
	const VeChar8* pcSrc) noexcept;

VE_POWER_API VeChar8* VeStrncpy(VeChar8* pcDest, VeSizeT stDestSize,
	const VeChar8* pcSrc, VeSizeT stCount) noexcept;

VE_POWER_API VeInt32 VeSprintf(VeChar8* pcDest, VeSizeT stDestSize,
	const VeChar8* pcFormat, ...) noexcept;

VE_POWER_API VeInt32 VeVsprintf(VeChar8* pcDest, VeSizeT stDestSize,
	const VeChar8* pcFormat, va_list kArgs) noexcept;

VE_POWER_API VeInt32 VeSnprintf(VeChar8* pcDest, VeSizeT stDestSize,
	VeSizeT stCount, const VeChar8* pcFormat, ...) noexcept;

VE_POWER_API VeInt32 VeVsnprintf(VeChar8* pcDest, VeSizeT stDestSize,
	VeSizeT stCount, const VeChar8* pcFormat, va_list kArgs) noexcept;

template<VeInt32 size>
VeChar8* VeStrcpy(VeChar8(&pcDest)[size], const VeChar8* pcSrc) noexcept
{
	return VeStrcpy(pcDest, size, pcSrc);
}

template<VeInt32 size>
VeChar8* VeStrncpy(VeChar8(&pcDest)[size], const VeChar8* pcSrc,
	VeSizeT stCount) noexcept
{
	return VeStrncpy(pcDest, size, pcSrc, stCount);
}

template<VeInt32 size>
VeInt32 VeSprintf(VeChar8(&pcDest)[size],
	const VeChar8* pcFormat, ...) noexcept
{
	VE_ASSERT(pcFormat);
	va_list kArgs;
	va_start(kArgs, pcFormat);
	int iRet = VeVsprintf(pcDest, size, pcFormat, kArgs);
	va_end(kArgs);
	return iRet;
}

template <class _Ty>
inline const VeChar8* VeSprintFormat() noexcept
{
	return "%s";
}

template <>
inline const VeChar8* VeSprintFormat<bool>() noexcept
{
	return "%d";
}

template <>
inline const VeChar8* VeSprintFormat<VeInt8>() noexcept
{
	return "%d";
}

template <>
inline const VeChar8* VeSprintFormat<VeInt16>() noexcept
{
	return "%d";
}

template <>
inline const VeChar8* VeSprintFormat<VeInt32>() noexcept
{
	return "%d";
}

template <>
inline const VeChar8* VeSprintFormat<VeUInt8>() noexcept
{
	return "%lu";
}

template <>
inline const VeChar8* VeSprintFormat<VeUInt16>() noexcept
{
	return "%lu";
}

template <>
inline const VeChar8* VeSprintFormat<VeUInt32>() noexcept
{
	return "%lu";
}

template <>
inline const VeChar8* VeSprintFormat<VeInt64>() noexcept
{
	return "%ll";
}

template <>
inline const VeChar8* VeSprintFormat<VeUInt64>() noexcept
{
	return "%llu";
}

template <>
inline const VeChar8* VeSprintFormat<VeFloat32>() noexcept
{
	return "%f";
}

template <>
inline const VeChar8* VeSprintFormat<VeFloat64>() noexcept
{
	return "%lf";
}

template <class _Ty>
VeInt32 VeTSprintf(VeChar8* pcDest, VeSizeT stDestSize,
	_Ty _Param) noexcept
{
	return VeSprintf(pcDest, stDestSize,
		VeSprintFormat<typename std::remove_cv<typename std::remove_reference<_Ty>::type>::type>(),
		_Param);
}

template <class... _Params>
struct  VeStrLinker;

template <>
struct  VeStrLinker<>
{
	static void Append(VeChar8* pcBuffer, VeSizeT stMax) noexcept
	{

	}
};

template <class _This, class... _Params>
struct  VeStrLinker<_This, _Params...>
{
	static void Append(VeChar8* pcBuffer, VeSizeT stMax, _This _Cur, _Params... _Pak) noexcept
	{
		VeInt32 i32Res = VeTSprintf(pcBuffer, stMax, _Cur);
		if (!(sizeof...(_Params))) return;
		pcBuffer += i32Res;
		stMax -= i32Res;
		if (stMax < 2) return;
		*pcBuffer = ',';
		++pcBuffer;
		--stMax;
		VeStrLinker<_Params...>::Append(pcBuffer, stMax, _Pak...);
	}
};

template <class _Ty>
struct VeIsChar : std::false_type
{

};

template <>
struct VeIsChar<VeChar8> : std::true_type
{

};

template <>
struct VeIsChar<VeChar16> : std::true_type
{

};

template <>
struct VeIsChar<VeChar32> : std::true_type
{

};

template <class _Ty>
inline VeSizeT VeStrlen(const _Ty* _Str) noexcept
{
	static_assert(VeIsChar<_Ty>::value,
		"_Ty is not char");
	const _Ty* _Eos = _Str;
	while (*_Eos++);
	return (_Eos - _Str - 1);
}

template <>
inline VeSizeT VeStrlen<VeChar8>(const VeChar8* _Str) noexcept
{
	return strlen(_Str);
}

template <class _Ty>
inline VeInt32 VeStrcmp(const _Ty* _Str1, const _Ty* _Str2) noexcept
{
	static_assert(VeIsChar<_Ty>::value,
		"_Ty is not char");
	VeInt32 _Ret = 0;
	while (!(_Ret = VeUInt32(*_Str1) - VeUInt32(*_Str2)) && *_Str2)
		++_Str1, ++_Str2;
	if (_Ret < 0)
		_Ret = -1;
	else if (_Ret > 0)
		_Ret = 1;
	return _Ret;
}

template <>
inline VeInt32 VeStrcmp<VeChar8>(const VeChar8* _Str1,
	const VeChar8* _Str2) noexcept
{
	return strcmp(_Str1, _Str2);
}

template <class _Ty>
inline VeInt32 VeStrncmp(const _Ty* _Str1, const _Ty* _Str2,
	VeSizeT _Count) noexcept
{
	static_assert(VeIsChar<_Ty>::value,
		"_Ty is not char");
	for (VeSizeT x(0); x < _Count; x++)
	{
		if (*_Str1 == 0 || *_Str1 != *_Str2)
		{
			return (VeUInt32(*_Str1) - VeUInt32(*_Str2));
		}
		++_Str1;
		++_Str2;
	}
	return 0;
}

template <>
inline VeInt32 VeStrncmp<VeChar8>(const VeChar8* _Str1,
	const VeChar8* _Str2, VeSizeT _Count) noexcept
{
	return strncmp(_Str1, _Str2, _Count);
}

template <class _Ty>
inline VeInt32 VeStricmp(const _Ty* _Str1,
	const _Ty* _Str2) noexcept
{
	VeInt32 f, l;
	do
	{
		if (((f = (VeUInt32)(*(_Str1++))) >= 'A') && (f <= 'Z'))
			f -= ('A' - 'a');

		if (((l = (VeUInt32)(*(_Str2++))) >= 'A') && (l <= 'Z'))
			l -= ('A' - 'a');
	} while (f && (f == l));
	return(f - l);
}

template <class _Ty>
inline VeInt32 VeStrnicmp(const _Ty* _Str1, const _Ty* _Str2,
	VeInt32 _Count) noexcept
{
	VeInt32 f, l;
	if (_Count)
	{
		do
		{
			if (((f = (VeUInt32)(*(_Str1++))) >= 'A') && (f <= 'Z'))
				f -= 'A' - 'a';

			if (((l = (VeUInt32)(*(_Str2++))) >= 'A') && (l <= 'Z'))
				l -= 'A' - 'a';
		} while (--_Count && f && (f == l));

		return(f - l);
	}

	return(0);
}

template <class _Ty>
inline const _Ty* VeStrchr(const _Ty* _Str, _Ty _Char) noexcept
{
	static_assert(VeIsChar<_Ty>::value,
		"_Ty is not char");
	while (*_Str && *_Str != _Char)
		++_Str;

	if (*_Str == _Char)
		return _Str;
	return nullptr;
}

template <>
inline const VeChar8* VeStrchr<VeChar8>(const VeChar8* _Str,
	VeChar8 _Char) noexcept
{
	return strchr(_Str, _Char);
}

template <class _Ty>
inline const _Ty* VeStrrchr(const _Ty* _Str, _Ty _Char) noexcept
{
	static_assert(VeIsChar<_Ty>::value,
		"_Ty is not char");
	const _Ty* _Start = _Str;
	while (*_Str++);
	while (--_Str != _Start && *_Str != _Char);
	if (*_Str == _Char)
		return _Str;

	return nullptr;
}

template <>
inline const VeChar8* VeStrrchr<VeChar8>(const VeChar8* _Str,
	VeChar8 _Char) noexcept
{
	return strrchr(_Str, _Char);
}

template <class _Ty>
inline const _Ty* VeStrstr(const _Ty* _Str1, const _Ty* _Str2) noexcept
{
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
inline const VeChar8* VeStrstr<VeChar8>(const VeChar8* _Str1,
	const VeChar8* _Str2) noexcept
{
	return strstr(_Str1, _Str2);
}

template <class _Ty>
inline _Ty* VeStrstr(_Ty* _Str1, const _Ty* _Str2) noexcept
{
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
inline VeChar8* VeStrstr<VeChar8>(VeChar8* _Str1,
	const VeChar8* _Str2) noexcept
{
	return strstr(_Str1, _Str2);
}

template <class _Ty>
inline _Ty* VeStrtok(_Ty* _Str, const _Ty* _Delimit,
	_Ty** _Context) noexcept
{
	_Ty* _StrStart = _Str;
	if ((!_StrStart) && _Context)
	{
		_StrStart = *_Context;
	}
	if (_StrStart)
	{
		VeSizeT _Len = VeStrlen(_Delimit);
		_Ty* pcTemp = VeStrstr(_StrStart, _Delimit);
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

//typedef std::basic_string < VeChar8, std::char_traits<VeChar8>,
//	venus::allocator < VeChar8 >> VeString;
//typedef std::basic_string < VeChar16, std::char_traits<VeChar16>,
//	venus::allocator < VeChar16 >> VeString16;
//typedef std::basic_string < VeChar32, std::char_traits<VeChar32>,
//	venus::allocator < VeChar32 >> VeString32;

class VE_POWER_API VeFixedString : public VeMemObject
{
public:
	struct Hash
	{
		size_t operator()(const VeFixedString& _Keyval) const
		{
			 return _Keyval.GetHashCode();
		}
	};

	typedef VeChar8* StringHandle;

	VeFixedString() noexcept;

	VeFixedString(const VeChar8* pcString) noexcept;

	VeFixedString(const VeFixedString& kCopy) noexcept;

	VeFixedString(VeFixedString&& kMove) noexcept;

	~VeFixedString() noexcept;

	operator const VeChar8*() const noexcept;

	const VeChar8* GetStr() const noexcept;

	bool Exists() const noexcept;

	VeFixedString& operator=(const VeFixedString& kCopy) noexcept;

	VeFixedString& operator=(VeFixedString&& kMove) noexcept;

	VeFixedString& operator=(const VeChar8* pcString) noexcept;

	VeUInt32 GetLength() const noexcept;

	VeUInt32 GetRefCount() const noexcept;

	VeUInt32 GetHashCode() const noexcept;

	bool Equals(const VeChar8* pcStr) const noexcept;

	bool EqualsNoCase(const VeChar8* pcStr) const noexcept;

	bool Contains(const VeChar8* pcStr) const noexcept;

	bool ContainsNoCase(const VeChar8* pcStr) const noexcept;

	bool operator==(const VeFixedString& s) const noexcept;

	bool operator!=(const VeFixedString& s) const noexcept;

	bool operator==(const VeChar8* s) const noexcept;

	bool operator!=(const VeChar8* s) const noexcept;

protected:
	StringHandle m_kHandle = nullptr;

};

VE_POWER_API bool operator==(const VeChar8* s1,
	const VeFixedString& s2) noexcept;

VE_POWER_API bool operator!=(const VeChar8* s1,
	const VeFixedString& s2) noexcept;

#define VE_STR_TAB_MASK (0x7FF)

class VE_POWER_API VeStringTable : public VeSingleton<VeStringTable>
{
public:
	typedef VeChar8* StringHandle;

	VeStringTable() noexcept;

	~VeStringTable() noexcept;

	const StringHandle AddString(const VeChar8* pcString) noexcept;

	VeSizeT GetNumString() noexcept;

	VeSizeT GetBucketSize(VeSizeT stWhichBucket) noexcept;

	VeSizeT GetTotalNumberOfBuckets() noexcept;

	VeSizeT GetMaxBucketSize() noexcept;

	const StringHandle FindString(const VeChar8* pcString,
		VeUInt32 u32Len, VeUInt32 u32Hash) noexcept;

	void InsertString(const StringHandle& kHandle,
		VeUInt32 u32Hash) noexcept;

	void RemoveString(const StringHandle& kHandle,
		VeUInt32 u32Hash) noexcept;

	static VeChar8* GetRealBufferStart(
		const StringHandle& kHandle) noexcept;

	static VeUInt32 HashFunction(const VeChar8* pcKey,
		VeUInt32 u32Len) noexcept;

	static void IncRefCount(StringHandle& kHandle) noexcept;

	static void DecRefCount(StringHandle& kHandle) noexcept;

	static const VeChar8* GetString(const StringHandle& kHandle) noexcept;

	static VeUInt32 GetLength(const StringHandle& kHandle) noexcept;

	static VeUInt32 GetHashValue(const StringHandle& kHandle) noexcept;

	static VeUInt32 GetRefCount(const StringHandle& kHandle) noexcept;

	static bool ValidateString(const StringHandle& kHandle) noexcept;

private:
	VeSizeT m_stStrNum = 0;
	VeVector<StringHandle> m_akHashArray[VE_STR_TAB_MASK+1];
	VeThread::Mutex m_kMutex;
	VeFixedString m_kEmpty;

};

template<class _Ty,
class _Hasher = VeFixedString::Hash,
class _Keyeq = std::equal_to<VeFixedString>,
class _Alloc = venus::allocator<std::pair<const VeFixedString, _Ty>>>
class VeStringMap : public std::unordered_map<VeFixedString, _Ty, _Hasher, _Keyeq, _Alloc> {};

#define ve_str_tab VeStringTable::GetSingleton()
