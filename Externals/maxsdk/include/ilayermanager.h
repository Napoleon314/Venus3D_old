/**********************************************************************
 *<
	FILE: ILayerManager.h

	DESCRIPTION: Declaration of the ILayerManager interface

	CREATED BY:	Peter Sauerbrei

	HISTORY: Created 19 October 1998

 *>	Copyright (c) 1998-99, All Rights Reserved.
 **********************************************************************/

#pragma once
#include "ref.h"

class ILayer;
class LayerIterator;
class ConstLayerIterator;

//! \brief This class is an interface to the layer manager.
/*! Note that some methods of this class are not functional in 3ds Max (only in 3D Studio VIZ).
To get a pointer to the global instance of this class use the following code:
\code
	ILayerManager* manager = GetCOREInterface13()-\>GetLayerManager();
\endcode
\sa  Class ReferenceTarget, Class ILayer, Class Interface13
*/
class ILayerManager : public ReferenceTarget
{
public:
	//! The super class ID of the layer manager interface
	static const SClass_ID kLayerManagerSuperClassID;

	// From Animatable
	SClass_ID SuperClassID() { return kLayerManagerSuperClassID; }

	// local methods ---------------------------
	/*! \remarks Adds the specified layer.
	\param layer - Points to the layer to add.
	\return  Returns true if the layer was added; false if not. */
	virtual bool AddLayer(ILayer * layer) = 0;
	/*! \remarks Creates and adds a layer. The name is based on the incremented layer
	count. 
	\return  The new layer */
	virtual ILayer * CreateLayer(void) = 0;
	/*! \remarks Creates and adds a layer.  
	\param name - The name of the new layer. The name must be case-insensitive unique
	\return  The new layer if a layer with the specified name could be created, null if not.*/
	virtual ILayer * CreateLayer(MSTR& name) = 0;
	/*! \remarks Deletes the layer whose name is passed. 
	\param name - The name for the layer.
	\return  TRUE if the layer was deleted, otherwise FALSE. A layer cannot be deleted
	if it is the current layer or contains nodes.*/
	virtual BOOL DeleteLayer(const MSTR & name) = 0;
	/*! \remarks Sets the layer whose name is passed as current.
	\param name - The name for the new current layer. */
	virtual void SetCurrentLayer(const MSTR & name) = 0;
	/*! \remarks Sets the current layer based on the selection set (the common
	layer). */
	virtual void SetCurrentLayer(void) = 0;
	/*! \remarks Returns an interface to the current layer. */
	virtual ILayer * GetCurrentLayer(void) const = 0;
	/*! \remarks Edits the layer whose name is passed. Note: This method does
	nothing in 3ds Max.
	\param name - The name of the layer to edit. */
	virtual void EditLayer(const MSTR & name) = 0;
	/*! \remarks Brings up the layer property dialog. Note: This method does
	nothing in 3ds Max.
	\param hWnd - The parent window handle. */
	virtual void DoLayerPropDialog(HWND hWnd) = 0;
	/*! \remarks Returns the number of layers. */
	virtual int GetLayerCount(void) = 0;
	/*! \remarks Gets the specified layer by index
	\param i - The index of the layer to get. 
	\return  The layer if the layer index is valid, null if not.*/
	virtual ILayer* GetLayer(int i) const = 0;
	/*! \remarks Returns a pointer to a layer interface for the named layer.
	\param name - The name of the layer to get. 
	\return  The layer if a layer with the specified name exists, null if not.*/
	virtual ILayer* GetLayer(const MSTR & name) const = 0;
	/*! \remarks Sets up the dropdown list in the Layer toolbar.
	\param hWnd - The dropdown list window handle. 
	\param hParent - The dropdown list's parent window handle. */
	virtual void SetupToolList2(HWND hWnd, HWND hParent) = 0;
	/*! \remarks Gets the layer saved in the right menu stack. Note: This method does
	nothing in 3ds Max.
	\param i - The index of the layer to get. 
	\return  The layer name if the layer index is valid, null string if not.*/
	virtual MSTR GetSavedLayer(int i) const = 0;
	/*! \remarks Returns an interface to the 0 layer. */
	virtual ILayer * GetRootLayer() const = 0;
	/*! \remarks Resets the layer manager.
	\param fileReset - This parameter is ignored. */
	virtual void Reset(BOOL fileReset = FALSE) = 0;

	/*! \remarks Sets whether Unhide/Unfreeze of objects is automatically propagated to hidden/frozen layers.
	\param prop - The propagation behavior. Valid values are:
		\li -2 - ask user whether to propagate, remember result for remainder of session
		\li -1 - use remembered result
		\li  0 - don't automatically propagate
		\li  1 - do automatically propagate
	*/
	virtual void SetPropagateToLayer(int prop) = 0;
	/*! \remarks gets whether Unhide/Unfreeze of objects is automatically propagated to hidden/frozen layers. */
	virtual int GetPropagateToLayer(void) = 0;
};

//! \brief This class is an extended interface to the layer manager.
/*!To get a pointer to the global instance of this class use the following code:
\code
	ILayerManagerEx* manager = (ILayerManagerEx*)GetCOREInterface13()-\>GetLayerManager();
\endcode
*/
class ILayerManagerEx : public ILayerManager
{
public:
	enum LayerMergeType
	{
		MergeContentLayersToRoot,		// merge content layers (layers containing merged nodes) into root
		MergeHierarchyToRoot,			// merge content layers and hierarchy (parent layer of content layers, recursively) to root
		MergeContentToDefault,			// merge content (merged nodes) to default layer (does not merge layers)
		MergeHierarchyToNew,				// merge content layers and hierarchy to new layer
		MergeHierarchyToMatching,		// merge content layers and hierarchy (parent layer of content layers, recursively) to root or matching layer hierarchy
		NumLayerMergeTypes				// must always be last - the number of layer merge types
	};

	/*! \remarks Returns the behavior of handling layers when merging a scene file. */
	virtual LayerMergeType GetLayerMergeType() const = 0;
	/*! \remarks Sets the behavior of handling layers when merging a scene file.
	\param layerMergeType - The merge type behavior.
	\param persist - Whether to persist the new setting to 3dsmax.ini */
	virtual void SetLayerMergeType(LayerMergeType layerMergeType, bool persist) = 0;
	/*! \remarks Returns whether to rename the merged layer or combine its content when an
	existing layer of the same name already exists. */
	virtual bool GetLayerMergeRenameDups() const = 0;
	/*! \remarks Sets whether to rename the merged layer or combine its content when an
	existing layer of the same name already exists.
	\param layerMergeRenameDups - If true, renames the merged layer if an existing layer with the same name already exists.
	\param persist - Whether to persist the new setting to 3dsmax.ini */
	virtual void SetLayerMergeRenameDups(bool layerMergeRenameDups, bool persist) = 0;

	/*! \remarks Sets the layer passed as the current layer.
	\param layer - The new current layer. */
	virtual void SetCurrentLayer(ILayer* layer) = 0;

	/*! \remarks Tests if the specified layer can be deleted. A layer can be deleted 
	if it is not the current layer, it is not layer "0", and the layer contains no nodes
	\param layer - The layer to test. 
	\return true if the specified layer can be deleted. */
	virtual bool CanDeleteLayer(ILayer* layer) const = 0;
	/*! \remarks Deletes the specified layer.
	\param layer - The layer to delete. 
	\return true if the layer was deleted, otherwise false. */
	virtual bool DeleteLayer(ILayer* layer) = 0;
	/*! \remarks Deletes the specified layer and its child layers.
	\param layer - The layer to test. 
	\param forceDelete - If false, any layers containing nodes, and their parent layers recursively, are not deleted. 
	If true, any nodes found are moved to the parent of the specified layer, or layer "0" if no
	parent layer, and the layers are deleted.
	\return true if the layer was deleted, otherwise false. */
	virtual bool DeleteLayerHierarchy(ILayer* layer, bool forceDelete = false) = 0;
	/*! \remarks Tests if the specified layer, or one of its descendant layers, contains a node.
	\param layer - The layer to test. 
	\return true if the specified layer, or one of its descendant layers, contains a node. */
	virtual bool DoesLayerHierarchyContainNodes(ILayer* layer) = 0;
};