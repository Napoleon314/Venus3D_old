/*! \file IParticleChannelExt2.h
    \brief Channel-generic interfaces ParticleChannelExt2
				This is a part of a particle channel that
				has extended interface for advanced funtionality;
				effectively it's an extension for IParticleChannelExt
				interface; it's not included in IParticleChannelExt
				to avoid max SDK break
*/
/**********************************************************************
 *<
	CREATED BY: Oleg Bayborodin

	HISTORY: created 03-10-2012

 *>	Copyright (c) Orbaz Technologies, Inc. 2012, All Rights Reserved.
 **********************************************************************/

#pragma once

#include "../ifnpub.h"

// interface ID
#define PARTICLECHANNELEXT2_INTERFACE Interface_ID(0x12dd1706, 0x1eb34400)

#define GetParticleChannelExt2Interface(obj) ((IParticleChannelExt2*)obj->GetInterface(PARTICLECHANNELEXT2_INTERFACE))


class IParticleChannelExt2 : public FPMixinInterface
{
public:

	/*! \fn virtual bool	Deglobalize(void) = 0; 
	*  \brief Make the data not to be global in the particle channel.
	This is mostly done to allow multi-threaded read/write access to the data
	Returns true if the operation is successful. It may return false if
	there is zero or one data item in the channel.
	*/
	virtual bool	Deglobalize(void) = 0; 

	// relevant for Shape particle channel only
	virtual void SuspendSharedDataShrinkage(void) {}
	virtual void ResumeSharedDataShrinkage(bool doDataShrink=false) { UNREFERENCED_PARAMETER(doDataShrink); }

      /*! \fn FPInterfaceDesc* GetDesc() { return GetDescByID(PARTICLECHANNELEXT2_INTERFACE); }
      *  \brief 
      */
	FPInterfaceDesc* GetDesc() { return GetDescByID(PARTICLECHANNELEXT2_INTERFACE); }
};

