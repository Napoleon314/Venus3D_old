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
VeJSONDoc VeOpenJSON(const VeChar8* pcPath) noexcept
{
	VeJSONDoc kDoc;
	VeBinaryIStreamPtr spConfig = ve_res_mgr.CreateIStream(pcPath);
	VeSizeT stLen = spConfig->RemainingLength();
	VeChar8* pcData = VeAlloc(VeChar8, stLen + 1);
	spConfig->Read(pcData, stLen);
	pcData[stLen] = 0;
	kDoc.Parse<0>(pcData);
	VeFree(pcData);
	return kDoc;
}
//--------------------------------------------------------------------------
