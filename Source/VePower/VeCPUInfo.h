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

class VE_POWER_API VeCPUInfo : public VeMemObject
{
public:
	static constexpr VeInt32 CACHELINE_SIZE = 128;

	static VeInt32 GetCPUCount() noexcept;


};
