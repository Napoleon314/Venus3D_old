////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeBinaryStream.h
//  Created:     2015/03/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#define VE_BS_BUFF_MAX_SIZE (2048)
#define VE_MAX_LENGTH (0xFFFFFF)

enum VeWhence
{
	VE_SEEK_SET = 0,
	VE_SEEK_CUR = 1,
	VE_SEEK_END = 2
};

class VeBinaryIStream;

class VE_POWER_API VeBinaryOStream : virtual public VeRefObject
{
	VeNoCopy(VeBinaryOStream);
	VeRTTIDecl(VeBinaryOStream);
public:
	VeBinaryOStream() noexcept;

	virtual ~VeBinaryOStream() noexcept;

	virtual VeSizeT Write(const void* pvBuffer, VeSizeT stBytes) noexcept = 0;

	virtual VeSizeT Size() const noexcept;

	void AppendString(const VeChar8* pcData, VeSizeT stLength) noexcept;

	void AppendStringAligned4(const VeChar8* pcData,
		VeSizeT stLength) noexcept;

	void Transfer(VeBinaryIStream& kFrom, VeSizeT stLength) noexcept;

	virtual void AddBlob(const void* pvBlob, VeSizeT stSize) noexcept;

	template <class _Ty>
	void InsertRaw(const _Ty& t) noexcept
	{
		VE_ASSERT_EQ(Write(&t, sizeof(_Ty)), sizeof(_Ty));
	}

	void WritePackedInt(VeSizeT stValue) noexcept;

	void WriteStringLength(VeSizeT stValue) noexcept;

	inline static VeSizeT CalculatePackedIntSize(VeSizeT stValue) noexcept;

};

VeSmartPointer(VeBinaryOStream);

class VE_POWER_API VeBinaryIStream : virtual public VeRefObject
{
	VeNoCopy(VeBinaryIStream);
	VeRTTIDecl(VeBinaryIStream);
public:
	VeBinaryIStream() noexcept;

	virtual ~VeBinaryIStream() noexcept;

	virtual VeSizeT Read(void* pvBuffer, VeSizeT stBytes) noexcept = 0;

	virtual VeResult Seek(VePtrDiff pdOffset, VeWhence eWhence) noexcept = 0;

	virtual VeSizeT Tell() noexcept = 0;

	virtual void* Skip(VeSizeT stBytes) noexcept = 0;

	virtual void Restart() noexcept = 0;

	virtual VeSizeT RemainingLength() const noexcept = 0;

	virtual VeChar8 Peek() noexcept = 0;

	virtual void Finish() noexcept;

	void GetStringAligned4(VeFixedString& kStr) noexcept;

	VeFixedString GetStringAligned4() noexcept;

	template <class _Ty>
	_Ty Get() noexcept
	{
		_Ty tRes;
		Read(&tRes, sizeof(_Ty));
		return tRes;
	}

	template <class _Ty>
	_Ty* To() noexcept
	{
		return (_Ty*)Skip(sizeof(_Ty));
	}

	template <class _Ty>
	void ExtractRaw(_Ty& t)
	{
		VE_ASSERT_EQ(Read(&t, sizeof(_Ty)), sizeof(_Ty));
	}

	VeSizeT ReadPackedInt() noexcept;

	inline VeSizeT ReadStringLength() noexcept;

	inline bool IsError() const noexcept;

	static VeChar8 ms_abErrBuf[VE_BS_BUFF_MAX_SIZE];

protected:
	inline void SetError(bool bError) noexcept;

	bool m_bError;

};

VeSmartPointer(VeBinaryIStream);

typedef union
{
	VeUInt8 u8;
	VeInt8 i8;
	VeChar8 c8;
	bool b;
} VeNetByte;

typedef union
{
	VeUInt16 u16;
	VeInt16 i16;
} VeNetShort;


typedef union
{
	VeUInt32 u32;
	VeInt32 i32;
	VeFloat32 f32;
} VeNetLong;


typedef union
{
	VeUInt64 u64;
	VeInt64 i64;
	VeFloat64 f64;
} VeNetLongLong;

#ifdef VE_BIG_ENDIAN

inline VeUInt16 VE_HTONS(VeUInt16 x) noexcept;

inline VeUInt32 VE_HTONL(VeUInt32 x) noexcept;

inline VeUInt64 VE_HTONLL(VeUInt64 x) noexcept;

inline VeFloat32 VE_HTONF(VeFloat32 f) noexcept;

inline void VE_HTONF_ASSIGN(VeFloat32& dest, VeFloat32 f) noexcept;

inline void VE_HTON3_ASSIGN(VeChar8* pcDest, const VeChar8* pcData) noexcept;

inline void VE_PACK3(VeChar8* pcDest, VeUInt32 u32Src) noexcept;

inline VeUInt32 VE_UNPACK3(const VeChar8* pcData) noexcept;

#else

#define VE_HTONS(x) (x)

#define VE_HTONL(x) (x)

#define VE_HTONLL(x) (x)

#define VE_HTONF(x) (x)

#define VE_HTONF_ASSIGN(dest,x) (dest=x)

inline void VE_HTON3_ASSIGN(VeChar8* pcDest, const VeChar8* pcData) noexcept;

inline void VE_PACK3(VeChar8* pcDest, VeUInt32 u32Src) noexcept;

inline VeUInt32 VE_UNPACK3(const VeChar8* pcData) noexcept;

#endif

#define VE_NTOHS(x) VE_HTONS(x)
#define VE_NTOHL(x) VE_HTONL(x)
#define VE_NTOHLL(x) VE_HTONLL(x)
#define VE_NTOHF(x) VE_HTONF(x)
#define VE_NTOHF_ASSIGN(dest,x) VE_HTONF_ASSIGN(dest,x)
#define VE_NTOH3_ASSIGN(dest,data) VE_HTON3_ASSIGN(dest,data)

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeNetByte kData) noexcept
{
	static_assert(sizeof(VeNetByte) == 1, "Stream: wrong byte size");
	VE_ASSERT_EQ(kOut.Write(&kData, sizeof(kData)), sizeof(kData));
	return kOut;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeNetShort kData) noexcept
{
	static_assert(sizeof(VeNetShort) == 2, "Stream: wrong short size");
	kData.u16 = VE_HTONS(kData.u16);
	VE_ASSERT_EQ(kOut.Write(&kData, sizeof(kData)), sizeof(kData));
	return kOut;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeNetLong kData) noexcept
{
	static_assert(sizeof(VeNetLong) == 4, "Stream: wrong long size");
	kData.u32 = VE_HTONL(kData.u32);
	VE_ASSERT_EQ(kOut.Write(&kData, sizeof(kData)), sizeof(kData));
	return kOut;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeNetLongLong kData) noexcept
{
	static_assert(sizeof(VeNetLongLong) == 8,
		"Stream: wrong longlong size");
	kData.u64 = VE_HTONLL(kData.u64);
	VE_ASSERT_EQ(kOut.Write(&kData, sizeof(kData)), sizeof(kData));
	return kOut;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeUInt8 x) noexcept
{
	VeNetByte n;
	n.u8 = x;
	return kOut << n;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeUInt16 x) noexcept
{
	VeNetShort n;
	n.u16 = x;
	return kOut << n;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeUInt32 x) noexcept
{
	VeNetLong n;
	n.u32 = x;
	return kOut << n;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeUInt64 x) noexcept
{
	VeNetLongLong n;
	n.u64 = x;
	return kOut << n;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeInt8 x) noexcept
{
	VeNetByte n;
	n.i8 = x;
	return kOut << n;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeInt16 x) noexcept
{
	VeNetShort n;
	n.i16 = x;
	return kOut << n;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeInt32 x) noexcept
{
	VeNetLong n;
	n.i32 = x;
	return kOut << n;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeInt64 x) noexcept
{
	VeNetLongLong n;
	n.i64 = x;
	return kOut << n;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	bool x) noexcept
{
	VeNetByte n;
	n.b = x;
	return kOut << n;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeFloat32 x) noexcept
{
	VeNetLong n;
	n.f32 = x;
	return kOut << n;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeFloat64 x) noexcept
{
	VeNetLongLong n;
	n.f64 = x;
	return kOut << n;
}

#ifndef VE_BIG_ENDIAN

template <class _Ty>
inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	const _Ty& t) noexcept
{
	kOut.InsertRaw(t);
	return kOut;
}

#endif

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	const VeFixedString& kStr) noexcept
{
	kOut.AppendString(kStr, kStr.GetLength());
	return kOut;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	VeChar8* pcStr) noexcept
{
	kOut.AppendString(pcStr, VeStrlen(pcStr));
	return kOut;
}

inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	const VeChar8* pcStr) noexcept
{
	kOut.AppendString(pcStr, VeStrlen(pcStr));
	return kOut;
}

template<class _Ty1, class _Ty2>
inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	const std::pair<_Ty1, _Ty2>& data) noexcept
{
	kOut << data.first;
	kOut << data.second;
	return kOut;
}

template <class _Ty>
inline VeBinaryOStream& operator << (VeBinaryOStream& kOut,
	const VeVector<_Ty>& data) noexcept
{
	kOut << VeUInt32(data.size());
	for (VeUInt32 i(0); i < data.size(); ++i)
		kOut << data[i];
	return kOut;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeNetByte& data) noexcept
{
	static_assert(sizeof(VeNetByte) == 1, "Stream: wrong byte size");
	VE_ASSERT_EQ(kIn.Read(&data, sizeof(data)), sizeof(data));
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeNetShort& data) noexcept
{
	static_assert(sizeof(VeNetShort) == 2, "Stream: wrong short size");
	VE_ASSERT_EQ(kIn.Read(&data, sizeof(data)), sizeof(data));
	data.u16 = VE_NTOHS(data.u16);
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeNetLong& data) noexcept
{
	static_assert(sizeof(VeNetLong) == 4, "Stream: wrong long size");
	VE_ASSERT_EQ(kIn.Read(&data, sizeof(data)), sizeof(data));
	data.u32 = VE_NTOHL(data.u32);
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeNetLongLong& data) noexcept
{
	static_assert(sizeof(VeNetLongLong) == 8,
		"Stream: wrong longlong size");
	VE_ASSERT_EQ(kIn.Read(&data, sizeof(data)), sizeof(data));
	data.u64 = VE_NTOHLL(data.u64);
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeUInt8& x) noexcept
{
	VeNetByte n;
	kIn >> n;
	x = n.u8;
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeUInt16& x) noexcept
{
	VeNetShort n;
	kIn >> n;
	x = n.u16;
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeUInt32& x) noexcept
{
	VeNetLong n;
	kIn >> n;
	x = n.u32;
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeUInt64& x) noexcept
{
	VeNetLongLong n;
	kIn >> n;
	x = n.u64;
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeInt8& x) noexcept
{
	VeNetByte n;
	kIn >> n;
	x = n.i8;
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeInt16& x) noexcept
{
	VeNetShort n;
	kIn >> n;
	x = n.i16;
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeInt32& x) noexcept
{
	VeNetLong n;
	kIn >> n;
	x = n.i32;
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeInt64& x) noexcept
{
	VeNetLongLong n;
	kIn >> n;
	x = n.i64;
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	bool& x) noexcept
{
	VeNetByte n;
	kIn >> n;
	x = n.b;
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeFloat32& x) noexcept
{
	VeNetLong n;
	kIn >> n;
	x = n.f32;
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeFloat64& x) noexcept
{
	VeNetLongLong n;
	kIn >> n;
	x = n.f64;
	return kIn;
}

#ifndef VE_BIG_ENDIAN

template <class _Ty>
inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	_Ty& t) noexcept
{
	kIn.ExtractRaw(t);
	return kIn;
}

#endif

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeFixedString& kStr) noexcept
{
	VeSizeT stLength = kIn.ReadPackedInt();
	if (stLength)
	{
		VeChar8* pcBuffer = VeStackAlloc(VeChar8, stLength + 1);
		VE_ASSERT_EQ(kIn.Read(pcBuffer, stLength), stLength);
		pcBuffer[stLength] = 0;
		kStr = pcBuffer;
		VeStackFree(pcBuffer);
	}
	else
	{
		kStr = "";
	}
	return kIn;
}

inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeChar8* pcStr) noexcept
{
	VeSizeT stLength = kIn.ReadPackedInt();
	if (stLength)
	{
		VE_ASSERT_EQ(kIn.Read(pcStr, stLength), stLength);
	}
	else
	{
		*pcStr = 0;
	}
	return kIn;
}

template<class _Ty1, class _Ty2>
inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	std::pair<_Ty1, _Ty2>& data) noexcept
{
	kIn >> data.first;
	kIn >> data.second;
	return kIn;
}

template <class _Ty>
inline VeBinaryIStream& operator>>(VeBinaryIStream& kIn,
	VeVector<_Ty>& data) noexcept
{
	VeUInt32 u32Size;
	kIn >> u32Size;
	data.resize(u32Size);
	for (VeUInt32 i(0); i < u32Size; ++i)
	{
		kIn >> data[i];
	}
	return kIn;
}

#include "VeBinaryStream.inl"
