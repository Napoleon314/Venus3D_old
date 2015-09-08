/**********************************************************************
// 	FILE: PopulateExport.h
// 
// 	DESCRIPTION:  Dll Export for Populate
//
// 	AUTHOR: Susan Amkraut  
// 
//	HISTORY: project initiated in Nov. 2006
//
// Copyright 2010 Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
**********************************************************************/

#pragma once

#ifdef BLD_POPULATE
#define POPULATE_EXPORT __declspec( dllexport )
#else
#define POPULATE_EXPORT __declspec( dllimport )
#endif
