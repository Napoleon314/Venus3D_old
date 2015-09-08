/**********************************************************************
*<
FILE: IPFOperatorPhysXWorld.h

DESCRIPTION: Maxscript access to PFOperatorPhysXWorld functionality

CREATED BY: Oleg Bayborodin

HISTORY: created 2008-01-06

*>	Copyright Orbaz Technologies (c) 2008, All Rights Reserved.
**********************************************************************/

#pragma once

#include "PFExport.h"
#include "../ifnpub.h"
#include "../object.h"

PFExport Object* GetPFObject(Object* obj);
// interface ID
#define PFOPERATORPHYSXWORLD_INTERFACE Interface_ID(0x160CC7AE, 0xDB564182) 
#define GetPFOperatorPhysXWorldInterface(obj) ((IPFOperatorPhysXWorld*)((GetPFObject(obj))->GetInterface(PFOPERATORPHYSXWORLD_INTERFACE))) 

// forward declares
class IObject;
class IPFSystemWorld;

class IPFOperatorPhysXWorld : public FPMixinInterface
{
public:

	// function IDs
	enum {	
		kAddCollisionGroupExclusionPair,
		kRemoveCollisionGroupExclusionPair,
		kHasCollisionGroupExclusionPair,
		kBakeSimulation,
		kRemoveBakedSimulation,
		kPlayback,
		kCreateNewDriver
	};

	// Function Map for Function Publish System 
	//***********************************
	BEGIN_FUNCTION_MAP

		FN_2(kAddCollisionGroupExclusionPair, TYPE_bool, AddCollisionGroupExclusionPair, TYPE_INT, TYPE_INT);
		FN_2(kRemoveCollisionGroupExclusionPair, TYPE_bool, RemoveCollisionGroupExclusionPair, TYPE_INT, TYPE_INT);
		FN_2(kHasCollisionGroupExclusionPair, TYPE_bool, HasCollisionGroupExclusionPair, TYPE_INT, TYPE_INT);
		FN_0(kBakeSimulation, TYPE_bool, BakeSimulation);
		VFN_0(kRemoveBakedSimulation, RemoveBakedSimulation);
		VFN_0(kPlayback, Playback);
		VFN_0(kCreateNewDriver, CreateNewDriver);

	END_FUNCTION_MAP

		virtual bool AddCollisionGroupExclusionPair(int group0, int group1) = 0;
	virtual bool RemoveCollisionGroupExclusionPair(int group0, int group1) = 0;
	virtual bool HasCollisionGroupExclusionPair(int group0, int group1) const = 0;
	virtual bool BakeSimulation(void) = 0;
	virtual void RemoveBakedSimulation(void) = 0;
	virtual void Playback(void) {}
	virtual bool SupressExpressSave(void) const { return false; }
	virtual void CreateNewDriver(void) {}

	virtual IPFSystemWorld*	GetSystemWorld(IObject* pCont)	const = 0;

	FPInterfaceDesc* GetDesc() { return GetDescByID(PFOPERATORPHYSXWORLD_INTERFACE); }
};

inline IPFOperatorPhysXWorld* PFOperatorPhysXWorldInterface(Object* obj) {
	return ((obj == NULL) ? NULL : GetPFOperatorPhysXWorldInterface(obj));
};

inline IPFOperatorPhysXWorld* PFOperatorPhysXWorldInterface(INode* node) {
	return ((node == NULL) ? NULL : GetPFOperatorPhysXWorldInterface(node->GetObjectRef()));
};

