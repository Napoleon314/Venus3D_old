//**************************************************************************/
// Copyright (c) 1998-2009 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/

#pragma once

#include "../ScripterExport.h"
#include "../../plugapi.h"

/** 
 * \brief The 3ds Max file versions the maxscript save commands 
 * (saveMaxFile and saveNodes) work with.
 * These values are converted by GetMaxVerFromMxsMaxFileVer() to 
 * the ones in \ref VersionMacros such as MAX_RELEASE, etc
*/
enum EMxsMaxFileVersion 
{
	//! \brief Unsupported 3ds Max version, corresponds to MAX_RELEASE_UNSUPPORTED
	kMxsMaxFileVersion_Unsuported = -1,
	//! \brief A file version was not specified, 
	//! maxscript save commands default to saving to current file version (MAX_RELASE)
	kMxsMaxFileVersion_Unspecified = 0,
	//! \brief The current 3ds Max version, corresponds to MAX_RELEASE
	kMxsMaxFileVersion_Current = 1,

	//! \brief 3ds Max 2013 file version, corresponds to MAX_RELEASE_R15
	kMxsMaxFileVersion_2013 = 2013,
	//! \brief 3ds Max 2014 file version, corresponds to MAX_RELEASE_R16
	kMxsMaxFileVersion_2014 = 2014,
	//! \brief 3ds Max 2015 file version, corresponds to MAX_RELEASE_R17
	kMxsMaxFileVersion_2015 = 2015,
	//! \brief 3ds Max 2016 file version, corresponds to MAX_RELEASE_R18
	kMxsMaxFileVersion_2016 = 2016,
};

/** 
 * \brief Converts a "human readable" 3ds Max version to \ref VersionMacros.
 * \param mxsMaxFileVer 3ds Max version as it occurs in the product name
 * \return A value in \ref VersionMacros (such as MAX_RELEASE),
 * or MAX_RELEASE_UNSUPPORTED if a non-supported 3ds Max version is specified.
*/
ScripterExport long GetMaxVerFromMxsMaxFileVer(EMxsMaxFileVersion mxsMaxFileVer);

/** 
 * \brief Converts a given 3ds Max version number to a EMxsMaxFileVersion.
 * \param maxVersion 3ds Max version number
 * \return The version number as it occurs in the product name ,
 * or kMxsMaxFileVersion_Unsuported if a non-supported 3ds Max version is specified.
*/
ScripterExport EMxsMaxFileVersion GetMxsMaxFileVerFromMaxVer(long maxVersion);
