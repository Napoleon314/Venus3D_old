////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeRefObject.h
//  Created:     2015/02/20 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeRefObject : public VeMemObject
{
	VeNoCopy(VeRefObject);
public:
	VeRefObject() noexcept;

	virtual ~VeRefObject() noexcept;

	void IncRefCount() noexcept;

	void DecRefCount() noexcept;

	inline VeUInt32 GetRefCount() const noexcept;

	inline static VeUInt32 GetTotalObjectCount() noexcept;

protected:
	void DeleteThis() noexcept;

private:
	std::atomic_uint m_u32RefCount;
	static std::atomic_uint ms_u32Objects;

};

#include "VeRefObject.inl"
