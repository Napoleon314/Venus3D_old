////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeString.cpp
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

#include "stdafx.h"

//--------------------------------------------------------------------------
char* VeStrcpy(char* pcDest, size_t stDestSize, const char* pcSrc) noexcept
{
#ifdef _MSC_VER
	strcpy_s(pcDest, stDestSize, pcSrc);
	return pcDest;
#else // #if _MSC_VER >= 1400
	assert(stDestSize != 0);
	size_t stSrcLen = vtd::strlen(pcSrc);
	assert(stDestSize > stSrcLen);
	size_t stWrite;
	if (stDestSize <= stSrcLen) // stDestSize < stSrcLen + 1
		stWrite = stDestSize;
	else
		stWrite = stSrcLen + 1;
	char *pcResult = strncpy(pcDest, pcSrc, stWrite);
	pcResult[stDestSize - 1] = 0;
	return pcResult;
#endif // #if _MSC_VER >= 1400
}
//--------------------------------------------------------------------------
char* VeStrncpy(char* pcDest, size_t stDestSize,
	const char* pcSrc, size_t stCount) noexcept
{
#if _MSC_VER >= 1400
	strncpy_s(pcDest, stDestSize, pcSrc, stCount);
	return pcDest;
#else // #if _MSC_VER >= 1400
	assert(pcDest != 0 && stDestSize != 0);
	assert(stCount < stDestSize || stCount == size_t(-1));
	if (stCount >= stDestSize)
	{
		if (stCount != size_t(-1))
		{
			pcDest[0] = 0;
			return pcDest;
		}
		else
		{
			stCount = stDestSize - 1;
		}
	}
	char* pcResult = strncpy(pcDest, pcSrc, stCount);
	pcResult[stCount] = 0;
	return pcResult;
#endif // #if _MSC_VER >= 1400
}
//--------------------------------------------------------------------------
int32_t VeSprintf(char* pcDest, size_t stDestSize,
	const char* pcFormat, ...) noexcept
{
	va_list kArgs;
	va_start(kArgs, pcFormat);
	int32_t iRet = VeVsprintf(pcDest, stDestSize, pcFormat, kArgs);
	va_end(kArgs);
	return iRet;
}
//--------------------------------------------------------------------------
int32_t VeVsprintf(char* pcDest, size_t stDestSize,
	const char* pcFormat, va_list kArgs) noexcept
{
	return VeVsnprintf(pcDest, stDestSize, size_t(-1), pcFormat, kArgs);
}
//--------------------------------------------------------------------------
int32_t VeSnprintf(char* pcDest, size_t stDestSize, size_t stCount,
	const char* pcFormat, ...) noexcept
{
	va_list kArgs;
	va_start(kArgs, pcFormat);
	int32_t iRet = VeVsnprintf(pcDest, stDestSize,
		stCount, pcFormat, kArgs);
	va_end(kArgs);
	return iRet;
}
//--------------------------------------------------------------------------
int32_t VeVsnprintf(char* pcDest, size_t stDestSize, size_t stCount,
	const char* pcFormat, va_list kArgs) noexcept
{
	if (stDestSize == 0)
	{
		return 0;
	}

	assert(pcDest);
	assert(stCount < stDestSize || stCount == size_t(-1));
	assert(pcFormat);

	pcDest[0] = 0;

	bool bTruncate = (stCount == size_t(-1));

#if _MSC_VER >= 1400
	int32_t iRet = vsnprintf_s(pcDest, stDestSize, stCount, pcFormat, kArgs);
#else
	if (bTruncate)
	{
		stCount = stDestSize - 1;
	}
	int32_t iRet = vsnprintf(pcDest, stCount, pcFormat, kArgs);
#endif

	if (iRet == -1 && !bTruncate)
	{
		iRet = (int32_t)stCount;
	}

#if _MSC_VER >= 1400

#else
	if (iRet == -1)
	{
		pcDest[stDestSize - 1] = 0;
	}
	else
	{
		pcDest[iRet] = 0;
	}
#endif

	return iRet;
}
//--------------------------------------------------------------------------
