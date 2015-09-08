// Copyright (c) 1998-2011 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.

#pragma once
#include "3dsmaxconfig.h"
 
// The recommended way to disable\enable warnings, or to change their level
// is via the following files 
// maxsdk\ProjectSettings\AdditionalCompilerOptions.txt - for x32 build targets
// maxsdk\ProjectSettings\AdditionalCompilerOptions64.txt - for x64 build targets

// Macro used to avoid the C4100 compiler warning. Use this for function parameters
// that are not used.
#define UNUSED_PARAM(x) (x)

