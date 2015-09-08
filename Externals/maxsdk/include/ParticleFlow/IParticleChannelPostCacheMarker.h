//**************************************************************************/
// Copyright (c) 1998-2013 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/

/*! \file IParticleChannelPostCacheMarker.h
    \brief A particle channel to identify a channel container
			that undergoes a post-cache processing by either
			Cache Disk or Cache Selective PFlow operators.
			It does not carry any particle-specific information,
			it's just a marker for PFlow operators to distinguish
			the particle processing to be done during post-cache stage.
*/

#pragma once

#include "IParticleChannelInt.h"

// standard particle channel "PostCacheMarker"
// interface IDs
#define PARTICLECHANNELPOSTCACHEMARKERR_INTERFACE Interface_ID(0xc928d443, 0x1eb34500)
#define PARTICLECHANNELPOSTCACHEMARKERW_INTERFACE Interface_ID(0xc928d443, 0x1eb34501)

#define GetParticleChannelPostCacheMarkerRInterface(obj) ((IParticleChannelIntR*)obj->GetInterface(PARTICLECHANNELPOSTCACHEMARKERR_INTERFACE))
#define GetParticleChannelPostCacheMarkerWInterface(obj) ((IParticleChannelIntW*)obj->GetInterface(PARTICLECHANNELPOSTCACHEMARKERW_INTERFACE))


