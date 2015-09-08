/**********************************************************************
 *<
	FILE: IParticleChannelTrueFalseIterator2.h

	DESCRIPTION: Channel-generic interface for boolean type particle channels.
				The interface is an extension for IParticleChannelTrueFalseIterator
				interface. 

	CREATED BY:	Oleg Bayborodin

	HISTORY:	created 2012-03-10

 *>	Copyright (c) 2012 Orbaz Technologies, Inc. All Rights Reserved.
 **********************************************************************/

#pragma once

#include "PFExport.h"
#include "../ifnpub.h"

// generic particle channel
// interface ID
#define PARTICLECHANNELTRUEFALSEITERATOR2_INTERFACE Interface_ID(0x91cd191d, 0x1eb34500)
#define GetParticleChannelTrueFalseIterator2Interface(obj) ((IParticleChannelTrueFalseIterator2*)obj->GetInterface(PARTICLECHANNELTRUEFALSEITERATOR2_INTERFACE))


class IParticleChannelTrueFalseIterator2 : public FPMixinInterface
{
public:

	// control whether to suspend or resume invalidate if the iterator
	virtual void SuspendIteratorInvalidation(void) = 0;
	virtual void ResumeIteratorInvalidation(void) = 0;

	FPInterfaceDesc* GetDesc() { return GetDescByID(PARTICLECHANNELTRUEFALSEITERATOR2_INTERFACE); }
};


