//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "RenderItemHandleArray.h"
#include "GenerateMeshRenderItemsContext.h"
#include "UpdateNodeContext.h"

#include "../BaseInterface.h"

/** Interface id of IMeshDisplay2.
*/
#define IMesh_DISPLAY2_INTERFACE_ID Interface_ID(0x177b2945, 0x776a1f62)

namespace MaxSDK { namespace Graphics {

class IMeshDisplay2Impl;

/** Use this interface to query the per node related render items from a mesh or mnesh.
	One can directly query this interface from a mesh or mnmesh object.
*/
class IMeshDisplay2 : public BaseInterface
{
public:
	GraphicsDriverAPI IMeshDisplay2();
	GraphicsDriverAPI virtual ~IMeshDisplay2();

	virtual void PrepareDisplay(const GenerateMeshRenderItemsContext& generateRenderItemsContext) = 0;

	/** This function is usually called by IObjectDisplay2::UpdatePerNodeItems() to 
		get accurate render items specified by generateRenderItemsContext to match
		the accurate per-node requirements for the current node's specific render items
		
		\param generateRenderItemsContext contains all the information that is used to generate all
				the required render items. 
		\param nodeContext which contains all infos required for adding per node
				relevant render items.
		\param targetRenderItemContainer the target render item container.
		\note DO NOT modify any GeometryRenderItemHandle which are returned in 
				targetRenderItemContainer. We cache all GeometryRenderItemHandle 
				internally which could be shared by other render nodes. So modify 
				them directly may cause unwanted behaviors. If you do have a request 
				to modify any GeometryRenderItemHandle, please first manually clone 
				them by using another GeometryRenderItemHandle or a RenderItemHandleDecorator.
		\return true if successful or false otherwise
	*/
	virtual bool GetRenderItems(
		const GenerateMeshRenderItemsContext& generateRenderItemsContext,
		UpdateNodeContext& nodeContext,
		IRenderItemContainer& targetRenderItemContainer) = 0;

	// from BaseInterface
	GraphicsDriverAPI virtual Interface_ID GetID();

	GraphicsDriverAPI virtual BaseInterface* GetInterface(Interface_ID id);

private:
	IMeshDisplay2Impl* mpImpl;
};

} } // end namespace
