////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeJSON.cpp
//  Created:     2015/08/25 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeJSONDoc operator "" _C(const VeChar8* pcStr, VeSizeT stNum) noexcept
{
	VeJSONDoc kDoc;
	kDoc.Parse<0>(pcStr);
	return kDoc;
}
//--------------------------------------------------------------------------
