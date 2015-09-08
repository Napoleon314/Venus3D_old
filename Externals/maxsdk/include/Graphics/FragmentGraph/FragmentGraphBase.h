//
// Copyright 2013 Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
//
//

#pragma once

#include "../../maxheap.h"

#ifdef FRAGMENT_GRAPH_MODULE
#	define FragmentGraphAPI __declspec(dllexport)
#else
#	define FragmentGraphAPI __declspec(dllimport)
#endif
