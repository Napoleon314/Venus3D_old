////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeTime.h
//  Created:     2015/08/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeTime : public VeRefObject
{
public:
	VeTime() noexcept;

	virtual ~VeTime() noexcept;

	inline VeFloat32 GetDeltaTime() noexcept;

	inline VeFloat64 GetDeltaTime64() noexcept;

	inline VeFloat64 GetTime() noexcept;

	inline VeUInt32 GetTimeUInt() noexcept;

	inline VeUInt64 GetCurrentCount() noexcept;

	inline void Pause() noexcept;

	inline void Resume() noexcept;

	inline bool GetPaused() noexcept;

	inline void SetInvert(bool bInv = false) noexcept;

	inline bool GetInvert() noexcept;

	void Reset() noexcept;

	void Update() noexcept;

protected:
	VeUInt64 m_u64Frequency;
	VeFloat64 m_f64InvFrequency;
	VeUInt64 m_u64StartRecordCount;
	VeUInt64 m_u64LastRecordCount;
	VeUInt64 m_u64CurrentCount;

	VeFloat64 m_f64Time;
	VeFloat64 m_f64DeltaTime;
	bool m_bNeedSkip;
	bool m_bPaused;
	bool m_bInvert;


};

VeSmartPointer(VeTime);

#include "VeTime.inl"
