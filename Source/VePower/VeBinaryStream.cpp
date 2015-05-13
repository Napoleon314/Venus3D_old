////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeBinaryStream.cpp
//  Created:     2015/03/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeBinaryOStream);
//--------------------------------------------------------------------------
VeBinaryOStream::VeBinaryOStream() noexcept
{

}
//--------------------------------------------------------------------------
VeBinaryOStream::~VeBinaryOStream() noexcept
{

}
//--------------------------------------------------------------------------
VeSizeT VeBinaryOStream::Size() const noexcept
{
	return 0;
}
//--------------------------------------------------------------------------
void VeBinaryOStream::AppendString(const VeChar8* pcData,
	VeSizeT stLength) noexcept
{
	WritePackedInt(stLength);
	AddBlob(pcData, stLength);
}
//--------------------------------------------------------------------------
void VeBinaryOStream::AppendStringAligned4(const VeChar8* pcData,
	VeSizeT stLength) noexcept
{
	WritePackedInt(stLength);
	AddBlob(pcData, stLength);
	VeSizeT stTemp = (stLength >= 255) ? 4 : 1;
	stTemp += stLength;
	stTemp = ((stTemp + 3) & (~3)) - stTemp;
	if (stTemp)
	{
		VeUInt32 u32Zero = 0;
		AddBlob(&u32Zero, stTemp);
	}
}
//--------------------------------------------------------------------------
void VeBinaryOStream::Transfer(VeBinaryIStream& kFrom,
	VeSizeT stLength) noexcept
{
	if (stLength)
	{
		void* pvBuffer = VeStackMalloc(stLength);
		VE_ASSERT_EQ(kFrom.Read(pvBuffer, stLength), stLength);
		AddBlob(pvBuffer, stLength);
		VeStackFree(pvBuffer);
	}
}
//--------------------------------------------------------------------------
void VeBinaryOStream::AddBlob(const void* pvBlob,
	VeSizeT stSize) noexcept
{
	if (stSize)
	{
		VE_ASSERT_EQ(Write(pvBlob, stSize), stSize);
	}
}
//--------------------------------------------------------------------------
void VeBinaryOStream::WritePackedInt(VeSizeT stValue) noexcept
{
	VE_ASSERT(stValue <= VE_MAX_LENGTH);
	if (stValue >= 255)
	{
		(*this) << (VeUInt8)0xFF;
		VeChar8 acValue[3];
		VE_PACK3(acValue, (VeUInt32)stValue);
		VE_ASSERT_EQ(Write(acValue, sizeof(acValue)), sizeof(acValue));
	}
	else
	{
		(*this) << (VeUInt8)stValue;
	}
}
//--------------------------------------------------------------------------
void VeBinaryOStream::WriteStringLength(VeSizeT stValue) noexcept
{
	WritePackedInt(stValue);
}
//--------------------------------------------------------------------------
VeRTTIImpl(VeBinaryIStream);
//--------------------------------------------------------------------------
VeBinaryIStream::VeBinaryIStream() noexcept : m_bError(false)
{

}
//--------------------------------------------------------------------------
VeBinaryIStream::~VeBinaryIStream() noexcept
{

}
//--------------------------------------------------------------------------
void VeBinaryIStream::Finish() noexcept
{

}
//--------------------------------------------------------------------------
void VeBinaryIStream::GetStringAligned4(VeFixedString& kStr) noexcept
{
	VeSizeT	stLength = ReadPackedInt();
	if (stLength)
	{
		VeChar8* pcBuffer = VeStackAlloc(VeChar8, stLength + 1);
		VE_ASSERT_EQ(Read(pcBuffer, stLength), stLength);
		pcBuffer[stLength] = 0;
		kStr = pcBuffer;
		VeStackFree(pcBuffer);
	}
	else
	{
		kStr = "";
	}
	VeSizeT stTemp = (stLength >= 255) ? 4 : 1;
	stTemp += stLength;
	stTemp = ((stTemp + 3) & (~3)) - stTemp;
	if (stTemp)
	{
		Skip(stTemp);
	}
}
//--------------------------------------------------------------------------
VeFixedString VeBinaryIStream::GetStringAligned4() noexcept
{
	VeFixedString kRes;
	GetStringAligned4(kRes);
	return kRes;
}
//--------------------------------------------------------------------------
VeSizeT VeBinaryIStream::ReadPackedInt() noexcept
{
	VeUInt8 u8Len;
	VE_ASSERT_EQ(Read(&u8Len, 1), 1);
	VeSizeT stValue = u8Len;
	if (stValue == 0xFF)
	{
		VeChar8 acBuffer[3];
		VE_ASSERT_EQ(Read(acBuffer, 3), 3);
		stValue = VE_UNPACK3(acBuffer);
	}
	return stValue;
}
//--------------------------------------------------------------------------
