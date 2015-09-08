 //
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "SmartHandle.h"
#include "BaseMaterialHandle.h"
#include "Matrix44.h"
#include <WTypes.h>

class Box3;
class INode;

namespace MaxSDK { namespace Graphics {

/** This class describes a graphics node. A graphics node is the container of 
render items. It also contains a material to shade all those render items. 
One render item could be added to multiple node to share the graphics data.
*/
class RenderNodeHandle : public SmartHandle
{
public:
	GraphicsDriverAPI RenderNodeHandle();
	GraphicsDriverAPI RenderNodeHandle(const RenderNodeHandle& from);
	GraphicsDriverAPI RenderNodeHandle& operator = (const RenderNodeHandle& from);
	GraphicsDriverAPI virtual ~RenderNodeHandle();

	/** Initialize this graphics node. 
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool Initialize();

public:

	/** Returns the world matrix of this node.
	\return the world matrix.
	*/
	GraphicsDriverAPI virtual const Matrix44& GetWorldMatrix() const;

	/** Set the world matrix of this node.
	\param matWorld the world matrix of this node.
	*/
	GraphicsDriverAPI void SetWorldMatrix(const Matrix44& matWorld);
	
	/** Set the bounding box of this node in its own space.
	\param bbox the bounding box of this node in its own space.
	*/
	GraphicsDriverAPI void SetObjectBox(const Box3& bbox);

public:

	/** Returns the max node.
	\return the max node.
	*/
	GraphicsDriverAPI INode* GetMaxNode() const;

	/** Returns whether this node would be visible to cameras in the scene.
	\return true if this node is visible to cameras in the scene, false otherwise.
	*/
	GraphicsDriverAPI bool GetPrimaryVisibility() const;

	/** Returns whether this node would appear in rendered reflections and refractions.
	\return true if this node would appear in rendered reflections and refractions, false otherwise.
	*/
	GraphicsDriverAPI bool GetSecondaryVisibility() const;

	/** Returns whether this node could receive shadows.
	\return true if this node could receiver shadows, false otherwise.
	*/
	GraphicsDriverAPI bool GetReceiveShadows() const;

	/** Returns whether this node could case shadows.
	\return true if this node could case shadows, false otherwise.
	*/
	GraphicsDriverAPI bool GetCastShadows() const;

	/** Returns whether this node should not be culled when rendering.
	\return true if this node should not be culled when rendering, false otherwise.
	*/
	GraphicsDriverAPI bool GetNoCulling() const;

	/** Returns whether this node is selected.
	\return true if this node is selected, false otherwise.
	*/
	GraphicsDriverAPI bool GetSelected() const;

	/** Returns whether this node is in bounding box mode.
	A node is in bounding box mode means that only the bounding box of this node would
	be shown in the view port.
	\return true if this node is in bounding box mode, false otherwise.
	*/
	GraphicsDriverAPI bool GetBoxMode() const;

	/** Returns whether back face cull is enabled on this node.
	\return true if back face cull is enabled on this node, false otherwise.
	*/
	GraphicsDriverAPI bool GetBackFaceCull() const;

	/** Returns the wire color of this node.
	\return the wire color of this node.
	*/
	GraphicsDriverAPI DWORD GetWireColor() const;

	/** Returns the opacity of this node.
	\return the opacity of this node.
	*/
	GraphicsDriverAPI float GetOpacity() const;

	/** Get the number of render items that this node contains.
	\return the number of render items that this node contains.
	*/
	GraphicsDriverAPI size_t GetNumberOfRenderItems() const;

	/** Get the i-th render item of this node.
	This function will not modify the reference count of the returned render item.
	\param i the index to the render item.
	\return the i-th render item of this node.
	*/
	GraphicsDriverAPI RenderItemHandle GetRenderItem(size_t i) const;

	/** Add a render item to this node. 
	This function will add reference to the input render item.
	\param pRenderItem the render item to add.
	*/
	GraphicsDriverAPI void AddRenderItem(const RenderItemHandle& pRenderItem);

	/** Remove the render item with the specified index from this node.
	\param i the index of the render item to be removed.
	*/
	GraphicsDriverAPI void RemoveRenderItem(size_t i);

	/** Set the default material for the given render item. 
	The material for a render item could be determined by three parameters: 
		- the override material of the render item (highest priority).
		- the "default" material of the render item (medium priority).
		- the node material (lowest priority).

	According to this priority, the "default" material will be used, if the render 
	item doesn't have an override material.

	Different render item could has different "default" material for the same node. 
	For example: the default material for wireframe render item could be a solid 
	white material.

	\param i the index of the render item.
	\param hMaterial the default material of that render item.
	*/
	GraphicsDriverAPI void SetRenderItemDefaultMaterial(
		size_t i, 
		const BaseMaterialHandle& hMaterial);

	/** Get the solid material assigned to the current node. 
		\note A custom solid material could be specified when updating per node items.
		This material would be passed to the inner mesh whose solid mesh render item will
		be assign this material.
	*/
	GraphicsDriverAPI BaseMaterialHandle GetSolidMaterial() const;

	/** Set the solid material. 
	*/
	GraphicsDriverAPI void SetSolidMaterial(const BaseMaterialHandle& solidMaterial);

	/** Get the wireframe material assigned to the current node. 
	*/
	GraphicsDriverAPI BaseMaterialHandle GetWireframeMaterial() const;

	/** Set the wirefrmame material. 
	*/
	GraphicsDriverAPI void SetWireframeMaterial(const BaseMaterialHandle& wireframeMaterial);

	/** Remove all render items from this node. Also release reference to those 
	render items.
	*/
	GraphicsDriverAPI void ClearAllRenderItems();
};

} } // namespaces
