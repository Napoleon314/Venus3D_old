//**************************************************************************/
// Copyright (c) 2011 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/

#pragma once

/*! This file contains windows version constants for 3ds Max. 
 *	The earliest version of Windows that 3ds Max supports is defined below
 *	For other system requirements, see www.autodesk.com for details.
 */

#ifndef WINVER				
#define WINVER 0x0502		//!< 3ds Max 2013 specifically targets Windows XP 2 or higher.
#endif

#ifndef _WIN32_WINNT		
#define _WIN32_WINNT 0x0502	 //!< 3ds Max 2013 specifically targets Windows XP 2 or higher.
#endif						

#ifndef _WIN32_WINDOWS		
#define _WIN32_WINDOWS 0x0502 //!< 3ds Max 2013 specifically targets Windows XP 2 or higher.
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0800	//!< 3ds Max 2013 specifically requires Internet Explorer version 8.0 or higher
#endif

