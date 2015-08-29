////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeJSON.h
//  Created:     2015/08/24 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <rapidjson/document.h>

typedef rapidjson::GenericDocument<rapidjson::UTF8<> > VeJSONDoc;
typedef rapidjson::GenericValue<rapidjson::UTF8<> > VeJSONValue;

#define VE_JSON_EXT { "json", "plist" }

int VeJSON() noexcept;
