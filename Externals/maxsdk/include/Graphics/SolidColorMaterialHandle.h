//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "BaseMaterialHandle.h"
#include "../acolor.h"


namespace MaxSDK { namespace Graphics {

/** SolidColorMaterialHandle is a material class to support solid color.
This class is used for viewport display and Quicksilver.
It's used by RenderItemHandle::SetCustomMaterial() to describe the solid color surface.
How to use:
\code
SolidColorMaterialHandle hMaterial;
hMaterial.Initialize();
pItem->SetCustomMaterial(hMaterial); // pItem is an instance of RenderItemHandle
\endcode
*/
class SolidColorMaterialHandle : public BaseMaterialHandle
{
public:
	GraphicsDriverAPI SolidColorMaterialHandle();
	GraphicsDriverAPI SolidColorMaterialHandle(const SolidColorMaterialHandle& from);
	GraphicsDriverAPI SolidColorMaterialHandle& operator = (const SolidColorMaterialHandle& from);
	GraphicsDriverAPI virtual ~SolidColorMaterialHandle();
	
	/** Initialize the instance of solid color material.
	A SolidColorMaterialHandle should be initialized before it's used.
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

	/** Set color for solid color material.
	By default, the color of solid color material is AColor(1.0, 1.0, 1.0, 1.0).
	When a color is set, the solid color material will use this color.
	\param color The color of solid color material.
	*/
	GraphicsDriverAPI void SetColor(const AColor& color);

	/** Get the color for solid color material.
	\return the color for solid color material.
	*/
	GraphicsDriverAPI const AColor& GetColor() const;
};

}}