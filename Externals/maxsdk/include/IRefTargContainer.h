/**********************************************************************
 *<
	FILE:  IRefTargContainer.h

	DESCRIPTION:  Defines IRefTargContainer class
		A Reference Maker class that stores a variable list of ReferenceTarget* items.
		All actions performed on this class are undoable.

	CREATED BY: Larry Minton

	HISTORY: created 5/19/04

 *>	Copyright (c) 2004, All Rights Reserved.
 **********************************************************************/
#pragma once

#include "ref.h"
#include "coreexp.h"

//! The RefTargContainer ClassID  
//! \note SClass_ID is REF_TARGET_CLASS_ID
#define REFTARG_CONTAINER_CLASS_ID Class_ID(0xa1692edf, 0xd681173f)

//! Id for IRefTargContainer interface via GetInterface(DWORD in_interfaceID)
#define IID_IREFTARG_CONTAINER					0xa1692edf

/*! \brief Container that handles references to ReferenceTargets.
 *
 * This is the interface for a container that references a variable number of 
 * ReferenceTarget objects.
 *  
 * 3ds Max's implementation of this interface persists itself and its operations 
 * support being undone/redone. 
 * Plugins that need to store a variable number of references to other plugin objects 
 * should create an instance of this container type and make a reference to it.
 *
 * The code below shows how to create an instance of 3ds Max's implementation of this interface:
 * \code 
 * IRefTargContainer* myRefTargets = static_cast<IRefTargContainer*>(Interface::CreateInstance(REF_TARGET_CLASS_ID, REFTARG_CONTAINER_CLASS_ID));
 * \endcode
 *
 * Or, you can create an instance via the static IRefTargContainer::CreateInstance() method.
 *
 * \note When this container points to a ReferenceTarget and that ReferenceTarget is deleted,
 * the corresponding reference slot is nulled out, but not deleted. Therefore the number of 
 * items reported by NumRefs() and GetNumItems()) will not shrink.
 * For example: If ten ReferenceTargets are added to the container, and then each ReferenceTarget is deleted, 
 * the container will still report ten references. The only way to shrink the count of the real 
 * number of references used is to call RemoveItem().  
 * A REFMSG_CONTAINER_ELEMENT_NULLED notification is sent to the immediate dependents of the 
 * IRefTargContainer instance when a ReferenceTarget stored in the container is deleted.
 * A REFMSG_CONTAINER_ELEMENT_ADDED notification is sent to the immediate dependents of the 
 * IRefTargContainer instance when an item slot is added.
 * A REFMSG_CONTAINER_ELEMENT_REMOVED notification is sent to the immediate dependents of the 
 * IRefTargContainer instance when an item slot is removed. 
 * A REFMSG_CONTAINER_ELEMENT_SET notification is sent to the immediate dependents of the 
 * IRefTargContainer instance when an item slot is set. 
 * When processing notifications, the GetChangedRefTarget() method can be used to identify which item triggered 
 * the notification, and GetChangedRefTargetIndex() which item index.
 * \see IIndirectRefTargContainer, IRefTargMonitorClass, INodeMonitor
*/
class IRefTargContainer : public ReferenceTarget
{
public:
	/*! \brief Returns the number of items in the container
	 *
	 * \return The number of items in the container. */
	virtual int GetNumItems() const =0; 

	/*! \brief Returns the i-th item, NULL if i is out of range
	 *
	 * \param i The index of the item to return. Must be in range [0, GetNumItems()-1]
	 * \return The i-th item, or NULL if index is out of range. */
	virtual ReferenceTarget *GetItem(int i) const =0;  
	
	/*! \brief Adds an item at the end of the container
	 *
	 * \param ref The item to be added
	 * \return The index of the newly added item. */
	virtual int AppendItem(ReferenceTarget *ref)=0; 

	/*! \brief Sets the value of the i-th container item.
	 * Can be used to append more than one item to the container at a time.
	 * For example: It is possible to call this method on an
	 * empty container, passing in an integer value of, say 10. 
	 * After doing this, the container will return 11 from the GetNumItems() and NumRefs() method.
	 * \param i - The index of the item to set. Must be >= 0. If i is greater than or equal 
	 * to the current number of items, the container will be resized, with NULL items added as necessary.
	 * \param ref - The item to set as the i-th item. */
	virtual void SetItem(int i, ReferenceTarget *ref)=0; 

	/*! \brief Inserts specified item at i-th place into the container.
	 * 
	 * \param i - The index where the item should be inserted. Must be in range 
	 * [0, GetNumItems()], otherwise i is truncated to fit in this range. When i is 
	 * GetNumItems(), the item is appended to the end of the container.
	 * \param ref - The item to be inserted.
	 * \return Index where the item has been inserted. */
	virtual int InsertItem(int i, ReferenceTarget *ref)=0; 

	/*! \brief Removes the i-th item from the container.
	 *
	 * De-references the i-th item and removes it from the container, thus effectively
	 * changing the number of items the container reports to have via GetNumItems().
	 * \note When removing more than one item from the container, iterate from high index to low index. 
	 * \param i - The index of the item to be removed. Must be in range [0, GetNumItems()-1].
	*/
	virtual void RemoveItem(int i)=0; 

	//! \brief Finds the first occurrence of a given reference target in the container
	/* \param [in] refTargToFind - the reference target to look for. Can be NULL.
	\return the zero based index of the found reference target within the container. 
	-1 if the item wasn't found.
	*/
	virtual int FindFirstItem(ReferenceTarget* refTargToFind) const = 0;

	//! \brief Returns whether the references are held as weak references
	//! \sa ~{ Kinds of References }~
	virtual bool GetHoldAsWeakReferences() const = 0;

	//! \brief Sets whether the references are held as weak references
	//! \param [in] holdAsWeakReferences - true if references are to be held as weak references
	//! \note For containers where the references are held as indirect references, the references will always be treated as weak references 
	//! \sa ~{ Kinds of References }~
	virtual void SetHoldAsWeakReferences(bool holdAsWeakReferences) = 0;

	//! \brief When a NotifyRefChanged is received from an IRefContainer, this method can be called to retrieve the index of the item that generated the notification
	//! \note The value returned by this method is valid only while processing notifications from the IRefTargContainer
	virtual int GetChangedRefTargetIndex() = 0;

	//! \brief When a NotifyRefChanged is received from an IRefContainer, this method can be called to retrieve the item that generated the notification
	//! \note The value returned by this method is valid only while processing notifications from the IRefTargContainer
	virtual ReferenceTarget* GetChangedRefTarget() = 0;

	// Animatable
	CoreExport virtual void* GetInterface(DWORD in_interfaceID);
	// InterfaceServer
	CoreExport virtual BaseInterface* GetInterface( Interface_ID id );

	/*! \brief Create an instance of 3ds Max's implementation of IRefTargContainer class
	 * \param holdAsWeakReferences Whether to hold the references as weak references
	 * \return The created instance. 
	 * \sa ~{ Kinds of References }~
	 */	
	CoreExport static IRefTargContainer* CreateInstance(bool holdAsWeakReferences = false);
};

