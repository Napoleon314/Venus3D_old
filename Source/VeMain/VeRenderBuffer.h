////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderBuffer.h
//  Created:     2015/09/10 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeRenderBuffer : public VeRenderResource
{
	VeNoCopy(VeRenderBuffer);
	VeRTTIDecl(VeRenderBuffer, VeRenderResource);
public:
	enum Type
	{
		TYPE_STATIC,
		TYPE_DYNAMIC,
		TYPE_MAX
	};

	enum Useage
	{
		USEAGE_CB = 0x1,
		USEAGE_VB = 0x2,
		USEAGE_IB = 0x4,		
		USEAGE_MASK = 0xf
	};

	inline Type GetType() noexcept;

	inline Useage GetUseage() noexcept;

	inline VeUInt32 GetSize() noexcept;

	inline bool CanUse(Useage eUse) noexcept;

	virtual void* Map() noexcept = 0;

	virtual void Unmap() noexcept = 0;

	virtual void UpdateSync(void* pvData) noexcept = 0;

protected:
	VeRenderBuffer(Type eType, Useage eUse, VeUInt32 u32Size) noexcept
		: VeRenderResource(DIMENSION_BUFFER), m_u16Type(eType)
		, m_u16Useage(eUse), m_u32Size(u32Size) {}

	VeUInt16 m_u16Type;
	VeUInt16 m_u16Useage;
	VeUInt32 m_u32Size;

};

VeSmartPointer(VeRenderBuffer);

#include "VeRenderBuffer.inl"
