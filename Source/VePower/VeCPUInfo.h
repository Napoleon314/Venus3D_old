////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeCPUInfo.h
//  Created:     2015/08/17 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#define VE_CACHELINE_SIZE 128

class VE_POWER_API VeCPUInfo : public VeMemObject
{
public:
	static VeInt32 GetCount() noexcept;

	static VeInt32 GetCacheLineSize() noexcept;

	static bool HasRDTSC() noexcept;

	static bool HasMMX() noexcept;

	static bool Has3DNow() noexcept;

	static bool HasSSE() noexcept;

	static bool HasSSE2() noexcept;

	static bool HasSSE3() noexcept;

	static bool HasSSE41() noexcept;

	static bool HasSSE42() noexcept;

	static bool HasAVX() noexcept;

	static VeInt32 GetSystemRAM() noexcept;

protected:
	static VeUInt32 GetCPUFeatures() noexcept;

	static VeInt32 ms_i32CPUCount;
	static VeInt32 ms_i32CPUFeatures;
	static VeInt32 ms_i32SystemRAM;

};
