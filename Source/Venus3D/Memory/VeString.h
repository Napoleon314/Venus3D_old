////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeString.h
//  Created:     2016/07/14 by Albert
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

VENUS_API char* VeStrcpy(char* pcDest, size_t stDestSize,
	const char* pcSrc) noexcept;

VENUS_API char* VeStrncpy(char* pcDest, size_t stDestSize,
	const char* pcSrc, size_t stCount) noexcept;

VENUS_API int32_t VeSprintf(char* pcDest, size_t stDestSize,
	const char* pcFormat, ...) noexcept;

VENUS_API int32_t VeVsprintf(char* pcDest, size_t stDestSize,
	const char* pcFormat, va_list kArgs) noexcept;

VENUS_API int32_t VeSnprintf(char* pcDest, size_t stDestSize,
	size_t stCount, const char* pcFormat, ...) noexcept;

VENUS_API int32_t VeVsnprintf(char* pcDest, size_t stDestSize,
	size_t stCount, const char* pcFormat, va_list kArgs) noexcept;

template <size_t s>
char* VeStrcpy(char(&pcDest)[s], const char* pcSrc) noexcept
{
	return VeStrcpy(pcDest, s, pcSrc);
}

template<size_t s>
char* VeStrncpy(char(&pcDest)[s], const char* pcSrc,
	size_t stCount) noexcept
{
	return VeStrncpy(pcDest, s, pcSrc, stCount);
}

template<size_t s>
int32_t VeSprintf(char(&pcDest)[s],
	const char* pcFormat, ...) noexcept
{
	assert(pcFormat);
	va_list kArgs;
	va_start(kArgs, pcFormat);
	int iRet = VeVsprintf(pcDest, s, pcFormat, kArgs);
	va_end(kArgs);
	return iRet;
}

template <class _Ty>
inline const char* VeSprintFormat() noexcept
{
	return "%s";
}

template <>
inline const char* VeSprintFormat<bool>() noexcept
{
	return "%d";
}

template <>
inline const char* VeSprintFormat<int8_t>() noexcept
{
	return "%d";
}

template <>
inline const char* VeSprintFormat<int16_t>() noexcept
{
	return "%d";
}

template <>
inline const char* VeSprintFormat<int32_t>() noexcept
{
	return "%d";
}

template <>
inline const char* VeSprintFormat<uint8_t>() noexcept
{
	return "%lu";
}

template <>
inline const char* VeSprintFormat<uint16_t>() noexcept
{
	return "%lu";
}

template <>
inline const char* VeSprintFormat<uint32_t>() noexcept
{
	return "%lu";
}

template <>
inline const char* VeSprintFormat<int64_t>() noexcept
{
	return "%ll";
}

template <>
inline const char* VeSprintFormat<uint64_t>() noexcept
{
	return "%llu";
}

template <>
inline const char* VeSprintFormat<float>() noexcept
{
	return "%f";
}

template <>
inline const char* VeSprintFormat<double>() noexcept
{
	return "%lf";
}

template <class _Ty>
int32_t VeTSprintf(char* pcDest, size_t stDestSize, _Ty _Param) noexcept
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
	static void Append(char*, size_t) noexcept
	{

	}
};

template <class _This, class... _Params>
struct  VeStrLinker<_This, _Params...>
{
	static void Append(char* pcBuffer, size_t stMax, _This _Cur, _Params... _Pak) noexcept
	{
		int32_t i32Res = VeTSprintf(pcBuffer, stMax, _Cur);
		if (!(sizeof...(_Params))) return;
		pcBuffer += i32Res;
		stMax -= i32Res;
		VeStrLinker<_Params...>::Append(pcBuffer, stMax, _Pak...);
	}
};
