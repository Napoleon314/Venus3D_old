/**********************************************************************
 *<
	FILE:			INodeValidity.h

	DESCRIPTION:	Declare BaseInterface class INodeValidity
											 
	CREATED BY:		MIchael Zyracki

	HISTORY:		created Aug 6, 2004

 *>	Copyright (c) 2004, All Rights Reserved.
 **********************************************************************/
#pragma once

#include "baseinterface.h"
#include "interval.h"

// The interface ID for class INodeValidity
#define NODEVALIDITY_INTERFACE Interface_ID(0x15ce0b65, 0x6acd24ba)

/** This interface provides access to the node's geometry pipeline, transform and whole world state validity intervals. 
Client code can retrieve this interface from an INode instance as follows:
\code
	INode* node; // variable that points to a node in the scene
	INodeValidity *nodeValidity = static_cast<INodeValidity*>(node->GetInterface(NODEVALIDITY_INTERFACE));
\endcode
*/
class INodeValidity : public BaseInterface 
{
public:
	/** Returns the cached validity interval of the node's geometry pipeline, including world space modifiers.
	Calling this method does not cause an evaluation of the node's geometry pipeline. */
	virtual Interval GetObValid() = 0;

	/** Returns the cached validity interval of the node's world state, which includes the validity of
	its geometry pipeline, transform, material and visibility controller.
	Calling this method does not cause an evaluation of the node's transform. */
	virtual Interval GetWSValid() = 0;

	/** Returns the cached validity interval of the node's transform controller.
	Calling this method does not cause an evaluation of the node's transform. */
	virtual Interval GetTMValid() = 0;
	
	// From BaseInterface
	Interface_ID GetID() { return NODEVALIDITY_INTERFACE; }
	LifetimeType LifetimeControl() { return wantsRelease; }
	void DeleteInterface() = 0;

};
