////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeBinaryStream.inl
//  Created:     2015/03/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeSizeT VeBinaryOStream::CalculatePackedIntSize(
	VeSizeT stValue) noexcept
{
	VE_ASSERT(stValue <= VE_MAX_LENGTH);
	return (stValue >= 0xFF) ? 4 : 1;
}
//--------------------------------------------------------------------------
inline VeSizeT VeBinaryIStream::ReadStringLength() noexcept
{
	return ReadPackedInt();
}
//--------------------------------------------------------------------------
inline bool VeBinaryIStream::IsError() const noexcept
{
	return m_bError;
}
//--------------------------------------------------------------------------
inline void VeBinaryIStream::SetError(bool bError) noexcept
{
	m_bError = bError;
}
//--------------------------------------------------------------------------
#ifdef VE_BIG_ENDIAN
//--------------------------------------------------------------------------
inline VeUInt16 VE_HTONS(VeUInt16 x) noexcept
{
	VeUInt16 res = ((x & 0x00ff) << 8) | ((x & 0xff00) >> 8);
	return res;
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_HTONL(VeUInt32 x) noexcept
{
	VeUInt32 res = ((x & 0x000000ff) << 24) | ((x & 0x0000ff00) << 8)
		| ((x & 0x00ff0000) >> 8) | ((x & 0xff000000) >> 24);
	return res;
}
//--------------------------------------------------------------------------
inline VeUInt64 VE_HTONLL(VeUInt64 x) noexcept
{
	VeUInt64 res =
		((x & 0x00000000000000ffULL) << 56) |
		((x & 0x000000000000ff00ULL) << 40) |
		((x & 0x0000000000ff0000ULL) << 24) |
		((x & 0x00000000ff000000ULL) << 8) |
		((x & 0x000000ff00000000ULL) >> 8) |
		((x & 0x0000ff0000000000ULL) >> 24) |
		((x & 0x00ff000000000000ULL) >> 40) |
		((x & 0xff00000000000000ULL) >> 56);
	return res;
}
//--------------------------------------------------------------------------
inline VeFloat32 VE_HTONF(VeFloat32 f) noexcept
{
	VeNetLong n;
	n.f32 = f;
	n.u32 = VE_HTONL(n.u32);
	return n.f32;
}
//--------------------------------------------------------------------------
inline void VE_HTONF_ASSIGN(VeFloat32& dest, VeFloat32 f) noexcept
{
	VeNetLong temp;
	temp.f32 = f;
	temp.u32 = VE_HTONL(temp.u32);
	dest = temp.f32;
}
//--------------------------------------------------------------------------
inline void VE_HTON3_ASSIGN(VeChar8* pcDest, const VeChar8* pcData) noexcept
{
	pcDest[0] = pcData[2];
	pcDest[1] = pcData[1];
	pcDest[2] = pcData[0];
}
//--------------------------------------------------------------------------
inline void VE_PACK3(VeChar8* pcDest, VeUInt32 u32Src) noexcept
{
	pcDest[0] = (VeChar8)(u32Src >> 16);
	pcDest[1] = (VeChar8)(u32Src >> 8);
	pcDest[2] = (VeChar8)u32Src;
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_UNPACK3(const VeChar8* pcData) noexcept
{
	const VeUInt8* pu8Data = (const VeUInt8*)pcData;
	return (pu8Data[0] << 16) | (pu8Data[1] << 8) | pu8Data[2];
}
//--------------------------------------------------------------------------
#else
//--------------------------------------------------------------------------
inline void VE_HTON3_ASSIGN(VeChar8* pcDest, const VeChar8* pcData) noexcept
{
	pcDest[0] = pcData[0];
	pcDest[1] = pcData[1];
	pcDest[2] = pcData[2];
}
//--------------------------------------------------------------------------
inline void VE_PACK3(VeChar8* pcDest, VeUInt32 u32Src) noexcept
{
	pcDest[0] = (VeChar8)u32Src;
	pcDest[1] = (VeChar8)(u32Src >> 8);
	pcDest[2] = (VeChar8)(u32Src >> 16);
}
//--------------------------------------------------------------------------
inline VeUInt32 VE_UNPACK3(const VeChar8* pcData) noexcept
{
	const VeUInt8* pu8Data = (const VeUInt8*)pcData;
	return pu8Data[0] | (pu8Data[1] << 8) | (pu8Data[2] << 16);
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
