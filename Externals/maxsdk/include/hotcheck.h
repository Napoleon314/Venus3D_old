/**********************************************************************
 *<
	FILE:  hotcheck.h

	DESCRIPTION:  Video Color check utilities

	CREATED BY: Dan Silva

	HISTORY: created 26 December 1995

 *>	Copyright (c) 1995, All Rights Reserved.
 **********************************************************************/
#pragma once

#include "coreexp.h"
// forward declarations
struct Color48;

#define VID_NTSC 0
#define VID_PAL 1

// Methods
#define HOT_FLAG 0
#define HOT_SCALE_LUM 1
#define HOT_SCALE_SAT 2


CoreExport void  BuildHotTable( int video_type = VID_NTSC );
// do video color check
/*! This method is called to perform the video color check for each pixel. This function can be used to correct a pixel with RGB values that will give "unsafe" values of chrominance signal or composite signal amplitude when encoded into an NTSC or PAL color signal. This happens for certain high-intensity high-saturation colors that are rare in real scenes, but can be present in computer generated images. 
\param thepix The pixel to check and correct if necessary. 
\param method One of these values:
- HOT_FLAG: Flag the pixel as black.
- HOT_SCALE_LUM: Correct by scaling the luminance.
- HOT_SCALE_SAT: Correct by scaling the saturation.
\return A non-zero value if the color is corrected. Otherwise, zero.
 */
CoreExport int HotLimit(Color48* thepix, int method = HOT_SCALE_LUM);

