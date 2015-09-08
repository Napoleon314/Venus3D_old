/*! \file IPFActionExt.h
\brief Action-generic interface IPFActionExt.
Used as an extension for IPFAction interface
without breaking the SDK. The interface is NOT
a part of every PF Operator & Test, only for those
that need it.
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
#define PFACTIONEXT_INTERFACE Interface_ID(0x0e789fcb, 0x1eb34500) 
#define GetPFActionExtInterface(obj) ((IPFActionExt*)((GetPFObject(obj))->GetInterface(PFACTIONEXT_INTERFACE))) 

class IPFActionExt : public FPMixinInterface
{
public:

	// function IDs
	enum {	kInteruptUpdate, 
			kSetFinalUpdateTime
	}; 

	// Function Map for Function Publish System 
	//***********************************
	BEGIN_FUNCTION_MAP

		FN_1(kInteruptUpdate, TYPE_bool, InteruptUpdate, TYPE_IOBJECT);
		FN_1(kSetFinalUpdateTime, TYPE_bool, SetFinalUpdateTime, TYPE_TIMEVALUE);

	END_FUNCTION_MAP

		/** @defgroup IPFActionExt IPFActionExt.h
		*  @{
		*/

		/*! \fn virtual bool InteruptUpdate() { return false; }
		*  \brief The method can be used to signal the a container update for a
		particle group has to be interrupted on this action. Only after all 
		particle groups in the system are updated (with possible interruptions as
		well), the container update can be continued. This is done for operators
		that are instanced across a PFlow system to ensure the real instant snapshot
		of the whole system to guarantee the proper interaction between particles
		at the same instant moment.
		*/
		virtual bool InteruptUpdate(IObject* pCont) const {  UNUSED_PARAM(pCont); return false; }

		/*! \fn virtual void AllowUpdateInterruption(bool allow);
		\brief defines whether is it allowed to interrupt an update
		for an action. If the update interruption is not allowed
		then InterruptUpdate cannot return true
		*/
		virtual void AllowUpdateInterruption(bool allow) {  UNUSED_PARAM(allow); }

		/*! \fn virtual bool SetFinalUpdateTime(TimeValue t) = 0;
		\brief inform the action what would be a final time for the series of update calls
		*/
		virtual bool SetFinalUpdateTime(TimeValue t) {  UNUSED_PARAM(t); return true; }


	/*! \fn FPInterfaceDesc* GetDesc() { return GetDescByID(PFACTIONEXT_INTERFACE); }
	*  \brief 
	*/
	FPInterfaceDesc* GetDesc() { return GetDescByID(PFACTIONEXT_INTERFACE); }

};

/*! \fn inline IPFActionExt* PFActionExtInterface(Object* obj) 
*  \brief 
*/
inline IPFActionExt* PFActionExtInterface(Object* obj) {
	return ((obj == NULL) ? NULL : GetPFActionExtInterface(obj));
};

/*! \fn inline IPFActionExt* PFActionExtInterface(INode* node)
*  \brief 
*/
inline IPFActionExt* PFActionExtInterface(INode* node) {
	return ((node == NULL) ? NULL : PFActionExtInterface(node->GetObjectRef()));
};

/*@}*/


