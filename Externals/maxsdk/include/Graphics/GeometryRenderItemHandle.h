//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once
#include "RenderItemHandle.h"
#include "IRenderGeometry.h"

namespace MaxSDK { namespace Graphics {

/** This is a special render item which used only by Mesh/MNMesh. User can custom a IRenderGeometry
to define what the mesh geometry looks like. Mesh and MNMesh also can generate default render geometry
for this render item.
*/
class GeometryRenderItemHandle : public RenderItemHandle
{
public:
	GraphicsDriverAPI GeometryRenderItemHandle();
	GraphicsDriverAPI GeometryRenderItemHandle(const GeometryRenderItemHandle& from);
	GraphicsDriverAPI GeometryRenderItemHandle& operator = (const GeometryRenderItemHandle& from);
	GraphicsDriverAPI virtual ~GeometryRenderItemHandle();

	GraphicsDriverAPI void Initialize();

	/** Set render item's render geometry. Render items without render geometry will be 
	ignored when rendering.
	\param renderGeometry the render geometry instance
	*/
	GraphicsDriverAPI void SetRenderGeometry(IRenderGeometryPtr renderGeom);

	/** Get current render geometry
	\return reference current render geometry
	*/
	GraphicsDriverAPI const IRenderGeometryPtr GetRenderGeometry() const;

	/** Set the material id of the render item. By default the material id of 
	a tuple mesh render item is zero. However if the parent node of the render 
	item has a multi/sub material, then it is necessary to specify which sub 
	material is used by this render item.
	\param materialId the material id of the render item. It is actually an index 
	to a sub material and starts from zero.
	*/
	GraphicsDriverAPI void SetMaterialID(MtlID materialId);

	/** Get the material id of the render item.
	*/
	GraphicsDriverAPI MtlID GetMaterialID() const;
};

} } // end namespace
