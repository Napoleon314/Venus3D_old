/*! \file IParticleChannelShapeNodeHandle.h
    \brief Channel-specific interfaces for ParticleChannelShapeNodeHandle
				 The channel is used to store handle of an INode used to
				 generate geometry shape for a particle representation
				 in mesh form for viewports and during render
				 The channel is a wrap around ParticleChannelINodeHandle
*/
/**********************************************************************
 *<
	CREATED BY: Oleg Bayborodin

	HISTORY: created 2010-03-30

 *>	Copyright (c) 2010, All Rights Reserved.
 **********************************************************************/

#pragma once

#include "IParticleChannelINodeHandle.h"

// standard particle channel "ShapeNodeHandle"
// interface ID
#define PARTICLECHANNELSHAPENODEHANDLER_INTERFACE Interface_ID(0x74f93b0d, 0x1eb34500)
#define PARTICLECHANNELSHAPENODEHANDLEW_INTERFACE Interface_ID(0x74f93b0d, 0x1eb34501)

#define GetParticleChannelShapeNodeHandleRInterface(obj) ((IParticleChannelINodeHandleR*)obj->GetInterface(PARTICLECHANNELSHAPENODEHANDLER_INTERFACE))
#define GetParticleChannelShapeNodeHandleWInterface(obj) ((IParticleChannelINodeHandleW*)obj->GetInterface(PARTICLECHANNELSHAPENODEHANDLEW_INTERFACE))

