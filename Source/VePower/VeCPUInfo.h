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

protected:
	static VeInt32 ms_i32CPUCount;

};
