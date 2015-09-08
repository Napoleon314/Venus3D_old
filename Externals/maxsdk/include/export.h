// Copyright (c) 1998-2011 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.

#include "3dsmaxconfig.h"

#ifdef DllExport
#undef DllExport
#endif

#ifdef IMPORTING
#define DllExport	__declspec( dllimport )
#else
#define DllExport	__declspec( dllexport )
#endif

