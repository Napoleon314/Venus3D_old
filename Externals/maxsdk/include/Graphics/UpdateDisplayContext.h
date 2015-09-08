//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "Types.h"
#include "GraphicsExport.h"
#include "MaterialRequiredStreams.h"
#include "../MaxHeap.h"
#include "../maxtypes.h"

class INode;

namespace MaxSDK { namespace Graphics {

/** This enum contains flags that can be combined together to specify which part 
	is required by the node when updating render items.
*/
enum ObjectComponentRequirement
{
	/** Require solid mesh, it could be: vertex color mesh or general mesh. This 
	will be displayed in any viewport that requires smooth mesh.
	*/
	ObjectComponentSolidMesh = 0x00000001,
	/** Require wireframe mesh, this layer includes the edges. This will be displayed
	 in viewport that has "wireframe" or "hidden line" on. 
	*/
	ObjectComponentWireframe = 0x00000002,
	/** Require vertex ticks. This will be displayed when the node has "Vertex Ticks" 
	display option on or the vertex tick sub-object be selected.
	*/
	ObjectComponentVertexTicks = 0x00000004,
	/** Require diagonals. This is the dotted edge lines when user enabled "show 
	edges" of a node and enable the wireframe mode of viewport.
	*/
	ObjectComponentDiagonal = 0x00000008,
	/** one or more of the node is selected
	*/
	ObjectComponentSelected = 0x00000010,
	/** one or more of the node is not selected
	*/
	ObjectComponentUnselected = 0x00000020,
	/** one or more of the node has the object property "Backface Cull" on.
	*/
	ObjectComponentBackfaceCull = 0x00000040, 
	/** one or more of the node has the object property "Vertex Channel Display" on
	and the active channel is soft selection
	*/
	ObjectComponentVertexColorSoftSelection = 0x00000080, 
};

/** This class contains all information that will be passed to IObjectDisplay::UpdateDisplay 
and will be used by plugin to update its display data. All the value of this class are set by 
system and plugin developers should build render items to satisfy those requirements.
*/
class UpdateDisplayContext : public MaxHeapOperators
{
public:
	GraphicsDriverAPI UpdateDisplayContext();
	GraphicsDriverAPI virtual ~UpdateDisplayContext();

	/** Get component requirement flags. Component requirement flag is decided by current 
	viewport configuration: visual style, edged faces, etc. and node display properties: 
	backface cull, vertex ticks, etc. 
	\sa ObjectComponentRequirement
	\return A combination of RenderItemCategory
	*/
	GraphicsDriverAPI unsigned long GetRequiredComponents() const;

	/** Set component requirement flags. 
	\sa ObjectComponentRequirement
	\param requiredComponent A combination of RenderItemCategory
	*/
	GraphicsDriverAPI void SetRequiredComponents(unsigned long requiredComponent);

	/** Clear the required stream of this class. 
	*/
	GraphicsDriverAPI void ClearRequiredStreams();

	/** Get the required streams. Required streams is for generating mesh render items. 
	\sa MaterialRequiredStreams
	*/
	GraphicsDriverAPI const MaterialRequiredStreams& GetRequiredStreams() const;

	/** Get the required streams. Required streams is for generating mesh render items. 
	\sa MaterialRequiredStreams
	*/
	GraphicsDriverAPI MaterialRequiredStreams& GetRequiredStreams();

	/** Add stream requirements, so that the new requirement of the UpdateDisplayContext 
	contains both old requirement and the newly input requirement.
	\sa MaterialRequiredStreams
	\param[in] requiredStreams the requirement to be added.
	*/
	GraphicsDriverAPI void AddRequiredStreams(const MaterialRequiredStreams& requiredStreams);

	/** Set the display time of this UpdateDisplayContext. 
	The display time is not necessarily the current time of 3ds max. This allows viewport 
	to display objects with different time.
	\param[in] t The current time.
	*/
	GraphicsDriverAPI void SetDisplayTime(TimeValue t);

	/** Get the display time. 
	\return the current time.
	*/
	GraphicsDriverAPI TimeValue GetDisplayTime() const;

	/** Get the number of nodes that reference the plugin object.
	\return the number of owner nodes.
	*/
	GraphicsDriverAPI size_t GetNumberOfOwnerNodes() const;

	/** Get the i-th owner node of the array.
	\param[in] i the index of the node in this context.
	\return the i-th owner node.
	*/
	GraphicsDriverAPI INode* GetOwnerNode(size_t i) const;

protected:
	void* mpImpl;
};

} } // end namespaces
