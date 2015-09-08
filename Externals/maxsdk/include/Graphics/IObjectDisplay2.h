//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "Types.h"
#include "RenderItemHandleArray.h"
#include "UpdateDisplayContext.h"
#include "UpdateNodeContext.h"
#include "UpdateViewContext.h"

#include "../BaseInterface.h"

/** Interface id of IObjectDisplay.
*/
#define IOBJECT_DISPLAY2_INTERFACE_ID Interface_ID(0x471404b6, 0x71ee66a1)

namespace MaxSDK { namespace Graphics {

class IObjectDisplay2Impl;

/** This enum contains flags that can be combined together to specify whether or 
	not certain functions of IObjectDisplay2 will be supported.
*/
enum ObjectDisplayRequirement
{
	/** Check if this plugin contains legacy display code and want legacy code 
		to be called as well. Signal this bit if contains legacy display code. 
		Default	value for this bit is false.
	*/
	ObjectDisplayRequireLegacyDisplayMode = 0x00000001,
	/** Trying to update items on per view basis is very time consuming. So only 
		signal this bit	if there are actual per view-dependent render items to 
		update. Otherwise call to UpdatePerViewItems will be ignored.
		Default value for this bit is false.
	*/
	ObjectDisplayRequireUpdatePerViewItems = 0x00000002,
};

/** To give object plugins better control of display, now object plugins can expose 
	IObjectDisplay2 interface. This interface allows object plugins to provide 
	different RenderItem for different nodes and different views. 

	If an object plugin provides both IObjectDisplay2 and IObjectDisplay interface, 
	only IObjectDisplay2 will be used.  IObjectDisplay interface is still supported 
	by Nitrous, however, we don’t suggest plugins to use it anymore. In the future 
	it will be obsoleted.
*/
class IObjectDisplay2 : public BaseInterface
{
public:
	GraphicsDriverAPI IObjectDisplay2();
	GraphicsDriverAPI virtual ~IObjectDisplay2();

	/** Return the object display requirement of this object.
	*/
	GraphicsDriverAPI virtual unsigned long GetObjectDisplayRequirement() const;

	/** An object could be referenced by multiple nodes at the same time, while 
	different node may requires different display of the object (backface cull vs. 
	non-backface cull, vertex color shading vs. material shading, etc.). 
	IObjectDisplay2::PrepareDisplay gives object plugins a chance to prepare display 
	data for all nodes, thus avoid duplicate display data generation for each node
	or each view. 
	If an object contains a Mesh or MNMesh and wants to display it, then Mesh::PrepareDisplay
	or MNMesh::PrepareDisplay must be called in this function.
	\param prepareDisplayContext The display context, which contains all the requirements for 
	plugin to generate render items.
	\return true if prepare successfully, false otherwise.
	*/
	virtual bool PrepareDisplay(
		const UpdateDisplayContext& prepareDisplayContext) = 0;

	/** This function will be called for each owner nodes of a given object plugin. 
		Plugins can follow the input node properties, and create appropriate RenderItem. 
		For example, if an object wants to display high-resolution mesh when node
		is selected, and display low-resolution mesh when node is not selected, 
		it can use this function to create different DisplayItem for different notes.
		\param updateDisplayContext The display context, which contains all the requirements for 
				plugin to generate render items.
		\param nodeContext which contains all infos required for adding per node
				relevant render items.
		\param targetRenderItemContainer the target render item container to which this 
		plugin object will be added.
		\return true if successful or false otherwise
	*/
	virtual bool UpdatePerNodeItems(
		const UpdateDisplayContext& updateDisplayContext,
		UpdateNodeContext& nodeContext,
		IRenderItemContainer& targetRenderItemContainer) = 0;

	/** This function will be called for each viewport and for each owner nodes 
		of a given object plugin.  If plugin wants to display / hide certain RenderItem
		for a given view property, plugin can override this function and update 
		RenderItem based on the input view properties.
		\note that this function is optional. If plugin doesn’t have view-dependent 
		RenderItem, it doesn’t need to override this function.
		\param pView the view for which render items will be updated
		\param updateDisplayContext The display context, which contains all the requirements for 
				plugin to generate render items.
		\param nodeContext which contains all infos required for adding per node
				relevant render items.
		\param targetRenderItemContainer the target render item container to which this 
				plugin object will be added.
		\return true if the plugin has updated any render items for hTargetNode 
			or false otherwise. We STRONGLY suggest return false for performance 
			sake when no any render item has been actually updated.
	*/
	GraphicsDriverAPI virtual bool UpdatePerViewItems(
		const UpdateDisplayContext& updateDisplayContext,
		UpdateNodeContext& nodeContext,
		UpdateViewContext& viewContext,
		IRenderItemContainer& targetRenderItemContainer);

	// from BaseInterface
	GraphicsDriverAPI virtual Interface_ID GetID();

	GraphicsDriverAPI virtual BaseInterface* GetInterface(Interface_ID id);

private:
	IObjectDisplay2Impl* mpImpl;
};

} } // end namespace
