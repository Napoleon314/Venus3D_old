/*! \file IParticleGroupExt.h
\brief Interface for ParticleGroup object.
Used as an extension for IParticleGroup interface
without breaking the SDK.
*/
/**********************************************************************
*<
CREATED BY: Oleg Bayborodin

HISTORY: created 2008-01-04

*>	Copyright (c) 2008, All Rights Reserved.
**********************************************************************/

#pragma once

#include "PFExport.h"
#include "../ifnpub.h"
#include "../object.h"

PFExport Object* GetPFObject(Object* obj);
// interface ID
#define PARTICLEGROUPEXT_INTERFACE Interface_ID(0x320fbd26, 0x7bc54cb0) 
#define GetParticleGroupExtInterface(obj) ((IParticleGroupExt*)((GetPFObject(obj))->GetInterface(PARTICLEGROUPEXT_INTERFACE))) 

class IParticleGroupExt : public FPMixinInterface
{

public:
	/** @defgroup IParticleGroupExt IParticleGroupExt.h
	*  @{
	*/

	/*! \fn virtual bool UpdateIsFinished();
	\brief identifies whether the update associated with the call
	Update(TimeValue t) was finished. If the update was not finished
	then the method Update(..) has to be called again (with the same
	TimeValue t) until it is finished
	*/
	virtual bool UpdateIsFinished() const { return true; }

	/*! \fn virtual void AllowUpdateInterruption(bool allow);
	\brief defines whether is it allowed to interrupt an update
	for a particle group. If the update interruption is not allowed
	then UpdateIsFinished should return true
	*/
	virtual void AllowUpdateInterruption(bool allow) {  UNUSED_PARAM(allow); }

	/*! \fn FPInterfaceDesc* GetDesc() { return GetDescByID(PARTICLEGROUPEXT_INTERFACE); }
	\brief perform invalidation that was delayed
	*/
	FPInterfaceDesc* GetDesc() { return GetDescByID(PARTICLEGROUPEXT_INTERFACE); }
};

inline IParticleGroupExt* ParticleGroupExtInterface(Object* obj) {
	return ((obj == NULL) ? NULL : GetParticleGroupExtInterface(obj));
};

inline IParticleGroupExt* ParticleGroupExtInterface(INode* node) {
	return ((node == NULL) ? NULL : ParticleGroupExtInterface(node->GetObjectRef()));
};
