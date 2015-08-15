////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeString.cpp
//  Created:     2015/02/23 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeChar8* VeStrcpy(VeChar8* pcDest, VeSizeT stDestSize,
	const VeChar8* pcSrc) noexcept
{
#	if _MSC_VER >= 1400
	strcpy_s(pcDest, stDestSize, pcSrc);
	return pcDest;
#	else // #if _MSC_VER >= 1400
	VE_ASSERT(stDestSize != 0);
	VeSizeT stSrcLen = VeStrlen(pcSrc);
	VE_ASSERT(stDestSize > stSrcLen);
	VeSizeT stWrite;
	if (stDestSize <= stSrcLen) // stDestSize < stSrcLen + 1
		stWrite = stDestSize;
	else
		stWrite = stSrcLen + 1;
	VeChar8 *pcResult = strncpy(pcDest, pcSrc, stWrite);
	pcResult[stDestSize - 1] = 0;
	return pcResult;
#	endif // #if _MSC_VER >= 1400
}
//--------------------------------------------------------------------------
VeChar8* VeStrncpy(VeChar8* pcDest, VeSizeT stDestSize,
	const VeChar8* pcSrc, VeSizeT stCount) noexcept
{
#	if _MSC_VER >= 1400
	strncpy_s(pcDest, stDestSize, pcSrc, stCount);
	return pcDest;
#	else // #if _MSC_VER >= 1400
	VE_ASSERT(pcDest != 0 && stDestSize != 0);
	VE_ASSERT(stCount < stDestSize || stCount == VE_SIZET_MAX);
	if (stCount >= stDestSize)
	{
		if (stCount != VE_SIZET_MAX)
		{
			pcDest[0] = 0;
			return pcDest;
		}
		else
		{
			stCount = stDestSize - 1;
		}
	}
	VeChar8* pcResult = strncpy(pcDest, pcSrc, stCount);
	pcResult[stCount] = 0;
	return pcResult;
#	endif // #if _MSC_VER >= 1400
}
//--------------------------------------------------------------------------
VeInt32 VeSprintf(VeChar8* pcDest, VeSizeT stDestSize,
	const VeChar8* pcFormat, ...) noexcept
{
	va_list kArgs;
	va_start(kArgs, pcFormat);
	VeInt32 iRet = VeVsprintf(pcDest, stDestSize, pcFormat, kArgs);
	va_end(kArgs);
	return iRet;
}
//--------------------------------------------------------------------------
VeInt32 VeVsprintf(VeChar8* pcDest, VeSizeT stDestSize,
	const VeChar8* pcFormat, va_list kArgs) noexcept
{
	return VeVsnprintf(pcDest, stDestSize, VE_SIZET_MAX, pcFormat, kArgs);
}
//--------------------------------------------------------------------------
VeInt32 VeSnprintf(VeChar8* pcDest, VeSizeT stDestSize, VeSizeT stCount,
	const VeChar8* pcFormat, ...) noexcept
{
	va_list kArgs;
	va_start(kArgs, pcFormat);
	VeInt32 iRet = VeVsnprintf(pcDest, stDestSize,
		stCount, pcFormat, kArgs);
	va_end(kArgs);
	return iRet;
}
//--------------------------------------------------------------------------
VeInt32 VeVsnprintf(VeChar8* pcDest, VeSizeT stDestSize, VeSizeT stCount,
	const VeChar8* pcFormat, va_list kArgs) noexcept
{
	if (stDestSize == 0)
	{
		return 0;
	}

	VE_ASSERT(pcDest);
	VE_ASSERT(stCount < stDestSize || stCount == VE_SIZET_MAX);
	VE_ASSERT(pcFormat);

	pcDest[0] = 0;

	bool bTruncate = (stCount == VE_SIZET_MAX);

#if _MSC_VER >= 1400
	VeInt32 iRet = vsnprintf_s(pcDest, stDestSize, stCount, pcFormat, kArgs);
#else
	if (bTruncate)
	{
		stCount = stDestSize - 1;
	}
	VeInt32 iRet = vsnprintf(pcDest, stCount, pcFormat, kArgs);
#endif

	if (iRet == -1 && !bTruncate)
	{
		iRet = (VeInt32)stCount;
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
VeFixedString::VeFixedString() noexcept
{

}
//--------------------------------------------------------------------------
VeFixedString::VeFixedString(const VeChar8* pcString) noexcept
{
	m_kHandle = ve_str_tab.AddString(pcString);
}
//--------------------------------------------------------------------------
VeFixedString::VeFixedString(const VeFixedString& kCopy) noexcept
{
	VeStringTable::IncRefCount(
		(VeStringTable::StringHandle&)kCopy.m_kHandle);
	m_kHandle = kCopy.m_kHandle;
}
//--------------------------------------------------------------------------
VeFixedString::VeFixedString(VeFixedString&& kMove) noexcept
{
	m_kHandle = kMove.m_kHandle;
	kMove.m_kHandle = nullptr;
}
//--------------------------------------------------------------------------
VeFixedString::~VeFixedString() noexcept
{
	VeStringTable::DecRefCount(m_kHandle);
}
//--------------------------------------------------------------------------
VeFixedString::operator const VeChar8*() const noexcept
{
	return m_kHandle;
}
//--------------------------------------------------------------------------
const VeChar8* VeFixedString::GetStr() const noexcept
{
	return m_kHandle;
}
//--------------------------------------------------------------------------
bool VeFixedString::Exists() const noexcept
{
	return m_kHandle ? true : false;
}
//--------------------------------------------------------------------------
VeFixedString& VeFixedString::operator=(
	const VeFixedString& kCopy) noexcept
{
	if (m_kHandle != kCopy.m_kHandle)
	{
		VeStringTable::StringHandle kHandle = kCopy.m_kHandle;
		VeStringTable::IncRefCount(kHandle);
		VeStringTable::DecRefCount(m_kHandle);
		m_kHandle = kHandle;
	}
	return *this;
}
//--------------------------------------------------------------------------
VeFixedString& VeFixedString::operator=(VeFixedString&& kMove) noexcept
{
	m_kHandle = kMove.m_kHandle;
	kMove.m_kHandle = nullptr;
	return *this;
}
//--------------------------------------------------------------------------
VeFixedString& VeFixedString::operator=(
	const VeChar8* pcString) noexcept
{
	if (m_kHandle != pcString)
	{
		VeStringTable::StringHandle kHandle = m_kHandle;
		m_kHandle = ve_str_tab.AddString(pcString);
		VeStringTable::DecRefCount(kHandle);
	}
	return *this;
}
//--------------------------------------------------------------------------
VeUInt32 VeFixedString::GetLength() const noexcept
{
	return VeStringTable::GetLength(m_kHandle);
}
//--------------------------------------------------------------------------
VeUInt32 VeFixedString::GetRefCount() const noexcept
{
	return VeStringTable::GetRefCount(m_kHandle);
}
//--------------------------------------------------------------------------
VeUInt32 VeFixedString::GetHashCode() const noexcept
{
	return VeStringTable::GetHashValue(m_kHandle);
}
//--------------------------------------------------------------------------
bool VeFixedString::Equals(const VeChar8* pcStr) const noexcept
{
	if (m_kHandle == pcStr)
		return true;

	if (pcStr == nullptr || m_kHandle == nullptr)
		return false;

	return VeStrcmp((const VeChar8*)m_kHandle, pcStr) == 0;
}
//--------------------------------------------------------------------------
bool VeFixedString::EqualsNoCase(const VeChar8* pcStr) const noexcept
{
	if (m_kHandle == pcStr)
		return true;

	if (pcStr == nullptr || m_kHandle == nullptr)
		return false;

	return VeStricmp((const VeChar8*)m_kHandle, pcStr) == 0;
}
//--------------------------------------------------------------------------
bool VeFixedString::Contains(const VeChar8* pcStr) const noexcept
{
	if (m_kHandle == pcStr && pcStr != nullptr)
		return true;

	if (pcStr == nullptr || m_kHandle == nullptr
		|| pcStr[0] == '\0' || m_kHandle[0] == '\0')
	{
		return false;
	}

	return VeStrstr((const VeChar8*)m_kHandle, pcStr) != nullptr;
}
//--------------------------------------------------------------------------
bool VeFixedString::ContainsNoCase(const VeChar8* pcStr) const noexcept
{
	if (m_kHandle == pcStr && pcStr != nullptr)
		return true;

	if (pcStr == nullptr || m_kHandle == nullptr
		|| pcStr[0] == '\0' || m_kHandle[0] == '\0')
	{
		return false;
	}

	const VeChar8* pcMyString = m_kHandle;
	VeUInt32 u32OtherLength = (VeUInt32)VeStrlen(pcStr);
	for (VeUInt32 uiMyIdx = 0; uiMyIdx < GetLength(); uiMyIdx++)
	{
		if (VeStrnicmp(&pcMyString[uiMyIdx], pcStr, u32OtherLength) == 0)
			return true;
	}
	return false;
}
//--------------------------------------------------------------------------
bool VeFixedString::operator==(const VeFixedString& s) const noexcept
{
	return Equals(s);
}
//--------------------------------------------------------------------------
bool VeFixedString::operator!=(const VeFixedString& s) const noexcept
{
	return !Equals(s);
}
//--------------------------------------------------------------------------
bool VeFixedString::operator==(const VeChar8* s) const noexcept
{
	return Equals(s);
}
//--------------------------------------------------------------------------
bool VeFixedString::operator!=(const VeChar8* s) const noexcept
{
	return !Equals(s);
}
//--------------------------------------------------------------------------
bool operator==(const VeChar8* s1, const VeFixedString& s2) noexcept
{
	return s2.Equals(s1);
}
//--------------------------------------------------------------------------
bool operator!=(const VeChar8* s1, const VeFixedString& s2) noexcept
{
	return !(s2.Equals(s1));
}
//--------------------------------------------------------------------------
VeStringTable::VeStringTable() noexcept : m_kEmpty("")
{

}
//--------------------------------------------------------------------------
VeStringTable::~VeStringTable() noexcept
{

}
//--------------------------------------------------------------------------
const VeStringTable::StringHandle VeStringTable::AddString(
	const VeChar8* pcString) noexcept
{
	if (!pcString)
		return nullptr;
	VeSizeT stStrLen = VeStrlen(pcString);
	VE_ASSERT(stStrLen < VE_UINT16_MAX);
	VeUInt32 u32Hash = HashFunction(pcString, (VeUInt32)stStrLen);
	StringHandle kHandle = FindString(pcString,
		(VeUInt32)stStrLen, u32Hash);
	if (kHandle)
	{
		IncRefCount(kHandle);
	}
	else
	{
		VeUInt32 u32AllocLen = VeUInt32(stStrLen) + 9;
		u32AllocLen = (u32AllocLen + 3) & (~3);
		void* pvMem = VeMalloc(u32AllocLen);
		VeChar8* pcMem = (VeChar8*)pvMem + 8;
		kHandle = pcMem;
		((VeUInt16*)pvMem)[0] = 2;
		((VeUInt16*)pvMem)[1] = VeUInt16(stStrLen);
		((VeUInt32*)pvMem)[1] = u32Hash;
		VeMemoryCopy(pcMem, pcString, stStrLen + 1);
		InsertString(kHandle, u32Hash);
	}
	return kHandle;
}
//--------------------------------------------------------------------------
VeSizeT VeStringTable::GetNumString() noexcept
{
	return m_stStrNum;
}
//--------------------------------------------------------------------------
VeSizeT VeStringTable::GetBucketSize(VeSizeT stWhichBucket) noexcept
{
	VE_ASSERT(stWhichBucket <= VE_STR_TAB_MASK);
	return m_akHashArray[stWhichBucket].size();
}
//--------------------------------------------------------------------------
VeSizeT VeStringTable::GetTotalNumberOfBuckets() noexcept
{
	return VE_STR_TAB_MASK + 1;
}
//--------------------------------------------------------------------------
VeSizeT VeStringTable::GetMaxBucketSize() noexcept
{
	VE_LOCK_MUTEX(m_kMutex);
	VeSizeT stSize = 0;
	for (auto vec : m_akHashArray)
	{
		stSize = VE_MAX(stSize, vec.size());
	}
	return stSize;
}
//--------------------------------------------------------------------------
const VeStringTable::StringHandle VeStringTable::FindString(
	const VeChar8* pcString, VeUInt32 u32Len, VeUInt32 u32Hash) noexcept
{
	VE_LOCK_MUTEX(m_kMutex);
	VeVector<StringHandle>& kStrArray = m_akHashArray[u32Hash & VE_STR_TAB_MASK];
	for (auto str : kStrArray)
	{
		if (str == pcString || (GetString(str) && GetLength(str) == u32Len
			&& VeStrcmp(GetString(str), pcString) == 0))
		{
			return str;
		}
	}
	return nullptr;
}
//--------------------------------------------------------------------------
void VeStringTable::InsertString(const StringHandle& kHandle,
	VeUInt32 u32Hash) noexcept
{
	VE_ASSERT(kHandle && ValidateString(kHandle));
	VE_LOCK_MUTEX(m_kMutex);
	m_akHashArray[u32Hash & VE_STR_TAB_MASK].push_back(kHandle);
	++m_stStrNum;
}
//--------------------------------------------------------------------------
void VeStringTable::RemoveString(const StringHandle& kHandle,
	VeUInt32 u32Hash) noexcept
{
	const VeChar8* pcString = GetString(kHandle);
	VE_LOCK_MUTEX(m_kMutex);
	VeVector<StringHandle>& kStrArray = m_akHashArray[u32Hash & VE_STR_TAB_MASK];
	for (VeSizeT i(0); i < kStrArray.size(); ++i)
	{
		if (kStrArray[i] == pcString)
		{
			VeUInt16* pu16Mem = (VeUInt16*)GetRealBufferStart(kHandle);
			VE_ASSERT(GetRefCount(kHandle) > 0);
			if ((--pu16Mem[0]) == 0)
			{
				VE_ASSERT(!GetRefCount(kHandle));
				VeFree(GetRealBufferStart(kHandle));
				if (i < (kStrArray.size() - 1))
				{
					kStrArray[i] = kStrArray.back();
				}
				kStrArray.pop_back();
				--m_stStrNum;
			}
			break;
		}
	}
}
//--------------------------------------------------------------------------
VeChar8* VeStringTable::GetRealBufferStart(
	const StringHandle& kHandle) noexcept
{
	VE_ASSERT(kHandle);
	return (VeChar8*)kHandle - 8;
}
//--------------------------------------------------------------------------
VeUInt32 VeStringTable::HashFunction(const VeChar8* pcKey,
	VeUInt32 u32Len) noexcept
{
	VeUInt32 u32Hash = 0;
	VeUInt32 u32Unroll = (VeUInt32)(u32Len & ~0x3);
	for (VeUInt32 ui = 0; ui < u32Unroll; ui += 4)
	{
		VeUInt32 uiHash0 = *pcKey;
		u32Hash = (u32Hash << 5) + u32Hash + uiHash0;
		VeUInt32 uiHash1 = *(pcKey + 1);
		u32Hash = (u32Hash << 5) + u32Hash + uiHash1;
		VeUInt32 uiHash2 = *(pcKey + 2);
		u32Hash = (u32Hash << 5) + u32Hash + uiHash2;
		VeUInt32 uiHash3 = *(pcKey + 3);
		u32Hash = (u32Hash << 5) + u32Hash + uiHash3;
		pcKey += 4;
	}
	while (*pcKey)
		u32Hash = (u32Hash << 5) + u32Hash + *pcKey++;
	return u32Hash;
}
//--------------------------------------------------------------------------
void VeStringTable::IncRefCount(StringHandle& kHandle) noexcept
{
	if (kHandle)
	{
		VE_ASSERT(ValidateString(kHandle));
		VeUInt16* pu16Mem = (VeUInt16*)GetRealBufferStart(kHandle);
		VE_ASSERT(pu16Mem[0] < VE_UINT16_MAX);
		VE_LOCK_MUTEX_NAME(ve_str_tab.m_kMutex, lockMutex);
		++pu16Mem[0];
	}
}
//--------------------------------------------------------------------------
void VeStringTable::DecRefCount(StringHandle& kHandle) noexcept
{
	if (kHandle)
	{
		VE_ASSERT(ValidateString(kHandle));
		VeUInt16* pu16Mem = (VeUInt16*)GetRealBufferStart(kHandle);
		VeUInt32 u32Hash = GetHashValue(kHandle);
		{
			VE_LOCK_MUTEX_NAME(ve_str_tab.m_kMutex, lockMutex);
			if ((--pu16Mem[0]) > 1) return;
		}
		ve_str_tab.RemoveString(kHandle, u32Hash);
	}
}
//--------------------------------------------------------------------------
const VeChar8* VeStringTable::GetString(
	const StringHandle& kHandle) noexcept
{
	VE_ASSERT(kHandle && ValidateString(kHandle));
	return (const VeChar8*)kHandle;
}
//--------------------------------------------------------------------------
VeUInt32 VeStringTable::GetLength(const StringHandle& kHandle) noexcept
{
	VE_ASSERT(kHandle && ValidateString(kHandle));
	VeUInt16* pu16Mem = (VeUInt16*)GetRealBufferStart(kHandle);
	return pu16Mem[1];
}
//--------------------------------------------------------------------------
VeUInt32 VeStringTable::GetHashValue(const StringHandle& kHandle) noexcept
{
	VE_ASSERT(kHandle && ValidateString(kHandle));
	VeUInt32* pu32Mem = (VeUInt32*)GetRealBufferStart(kHandle);
	return pu32Mem[1];
}
//--------------------------------------------------------------------------
VeUInt32 VeStringTable::GetRefCount(const StringHandle& kHandle) noexcept
{
	VE_ASSERT(kHandle && ValidateString(kHandle));
	VeUInt16* pu16Mem = (VeUInt16*)GetRealBufferStart(kHandle);
	return pu16Mem[0];
}
//--------------------------------------------------------------------------
bool VeStringTable::ValidateString(const StringHandle& kHandle) noexcept
{
	if (!kHandle) return true;
	VeUInt16* pu16Mem = (VeUInt16*)GetRealBufferStart(kHandle);
	return VeSizeT(pu16Mem[1]) == VeStrlen((const VeChar8*)kHandle);
}
//--------------------------------------------------------------------------
