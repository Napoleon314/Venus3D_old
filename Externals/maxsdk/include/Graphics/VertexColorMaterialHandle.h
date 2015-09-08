//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "BaseMaterialHandle.h"


namespace MaxSDK { namespace Graphics {

/** VertexColorMaterialHandle is a material class to support vertex color.
This class is used for viewport display and Quicksilver.
It's used by RenderItemHandle::SetCustomMaterial() to describe the vertex color surface.
How to use:
\code
VertexColorMaterialHandle hMaterial;
hMaterial.Initialize();
pItem->SetCustomMaterial(hMaterial); // pItem is an instance of RenderItemHandle
\endcode
*/
class VertexColorMaterialHandle : public BaseMaterialHandle
{
public:
	GraphicsDriverAPI VertexColorMaterialHandle();
	GraphicsDriverAPI VertexColorMaterialHandle(const VertexColorMaterialHandle& from);
	GraphicsDriverAPI VertexColorMaterialHandle& operator = (const VertexColorMaterialHandle& from);
	GraphicsDriverAPI virtual ~VertexColorMaterialHandle();
	
	/** Initialize the instance of vertex color material.
	A VertexColorMaterialHandle should be initialized before it's used.
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool Initialize();

	/** Returns the material required streams to tell how to setup mesh data.
	When a BaseMaterialHandle is used by RenderItemHandle::SetCustomMaterial(), 
	it also needs to know about the material requirement streams to setup corresponding 
	mesh data. It could be used by GenerateRenderItems::GenerateRenderItems() for RenderItemHandle.
	\return the material required streams.
	*/
	GraphicsDriverAPI virtual const MaterialRequiredStreams* GetRequiredStreams() const;
};

}}