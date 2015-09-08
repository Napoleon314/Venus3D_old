//
// Copyright 2009 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.  
//
//

// Most source files should not include this file directly.  Instead, they
// should include dllutilities.h.

#pragma once
#include "WindowsDefines.h"
#include "utilexp.h"
#include "strclass.h"

namespace MaxSDK
{

/**
 * \brief Extract a resource from a module's string table.
 *
 * GetResourceString is essentially a thread-safe wrapper around the Win32 API's LoadString,
 * but it saves client code from the bother of maintaining a buffer and 
 * reduces duplicated code. Optimally, GetResourceString should be called
 * through the single-parameter version from dllutilities.h.
 *
 * \param hinstance Handle to the module whose string table will be queried
 * for the resource.
 * \param resourceId Identifier for the desired string resource within the
 * calling module's string table.
 * \return An MSTR containing the resource string, empty if the requested 
 * resource is not found.
 */
UtilExport MSTR GetResourceStringAsMSTR(HINSTANCE hinstance, UINT resourceId);

/**
 * \brief Extract a resource from a module's string table.
 *
 * GetResourceString is essentially a thread-safe wrapper around the Win32 API's LoadString,
 * but it saves client code from the bother of maintaining a buffer and 
 * reduces duplicated code. Optimally, GetResourceString should be called
 * through the single-parameter version from dllutilities.h.
 *
 * \param hinstance Handle to the module whose string table will be queried
 * for the resource.
 * \param resourceId Identifier for the desired string resource within the
 * calling module's string table.
 * \param resourceString Updated to contain the resource string if the requested 
 * resource is not found.
 * \return true if the requested resource is found, false if not
 */
UtilExport bool GetResourceStringAsMSTR(HINSTANCE hinstance, UINT resourceId, MSTR& resourceString);

}
