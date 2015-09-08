//**************************************************************************/
// Copyright (c) 1998-2006 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Defines an interface for a container of indirect references to
// reference targets. All actions performed instances of this class are undoable.
// AUTHOR: Attila.Szabo - created March.14.2006
//***************************************************************************/
#pragma once

#include "plugapi.h"
#include "IIndirectRefMaker.h"
#include "IRefTargContainer.h"

//! The class id of the indirect reference container
//! \note SClass_ID is REF_TARGET_CLASS_ID
#define INDIRECT_REFTARG_CONTAINER_CLASS_ID Class_ID(0x28de7aca, 0x78236a6f)

//! Id for IIndirectRefTargContainer interface via GetInterface(DWORD in_interfaceID)
#define IID_IINDIRECT_REFTARG_CONTAINER					0x28de7aca

//! \brief The interface for a container of indirect references
/* Instances of this class store a variable number of indirect references, and 
zero regular (direct) references. Reference notification messages sent by the
ReferenceTargets indirectly referenced by this container are ignored by the 
container and are not propagated to the container's dependents. Consequently, 
the container does not report itself nor its dependents as dependents of the 
indirect references stored by it (see ReferenceTarget::DoEnumDependents).
The indirect references stored in this container to ReferenceTargets do not 
cause those ReferenceTargets to be persisted when this container is persisted. 

This type of container is implemented by 3ds Max. Instances of it can be created 
via Interface::CreateInstance(SClass_ID superID, Class_ID classID) or by the 
static CreateInstance() method.
Plugins that need to store a variable number of indirect references to other
plugin objects should create an instance of this container type and make a 
reference to it. 

3ds Max's implementation of this container type persists itself and its operations 
support being undone\redone. 

3ds Max's implementation of this container type holds only indirect references, so
the weak reference setting from IRefTargContainer has no impact on its behavior.

* A REFMSG_CONTAINER_ELEMENT_NULLED notification is sent to the immediate dependents of the 
* IIndirectRefTargContainer instance when a monitored ReferenceTarget is deleted.
* A REFMSG_CONTAINER_ELEMENT_ADDED notification is sent to the immediate dependents of the 
* IIndirectRefTargContainer instance when an item slot is added.
* A REFMSG_CONTAINER_ELEMENT_REMOVED notification is sent to the immediate dependents of the 
* IIndirectRefTargContainer instance when an item slot is removed. 
* A REFMSG_CONTAINER_ELEMENT_SET notification is sent to the immediate dependents of the 
* IIndirectRefTargContainer instance when a monitored ReferenceTarget is set. 
* When processing notifications, the GetChangedRefTarget() method can be used to identify which item triggered 
* the notification, and GetChangedRefTargetIndex() which item index.
\see IIndirectReferenceMaker, IRefTargContainer, ReferenceTarget
*/
class IIndirectRefTargContainer : public IRefTargContainer, public IIndirectReferenceMaker
{
public:
	virtual SClass_ID SuperClassID() { return REF_TARGET_CLASS_ID; }
	virtual Class_ID ClassID() { return INDIRECT_REFTARG_CONTAINER_CLASS_ID; }	

	// Animatable
	CoreExport virtual void* GetInterface(DWORD in_interfaceID);
	// InterfaceServer
	CoreExport virtual BaseInterface* GetInterface( Interface_ID id );

	/*! \brief Create instance of 3ds Max's implementation of IIndirectRefTargContainer class
	 *
	 * \return The created instance. */
	CoreExport static IIndirectRefTargContainer* CreateInstance();
};

