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
#include "../acolor.h"

namespace MaxSDK { namespace Graphics {

class DrawContext;

/**This is base graphics material class. 
A material describes how to render the surface of an object. 
This class, and all its derived classes, are used for viewport display and Quicksilver. 
The difference between BaseMaterialHandle and ISimpleMaterial is that 
BaseMaterialHandle is used by RenderItemHandle::SetCustomMaterial() to describes the surface and
ISimpleMaterial is used for Mtl and Texmap to display.
*/
class BaseMaterialHandle : public SmartHandle
{
public:
	GraphicsDriverAPI BaseMaterialHandle();
	GraphicsDriverAPI BaseMaterialHandle(const BaseMaterialHandle& from);
	GraphicsDriverAPI BaseMaterialHandle& operator = (const BaseMaterialHandle& from);
	GraphicsDriverAPI virtual ~BaseMaterialHandle();

	/** Returns the material required streams to tell how to setup mesh data.
	When a BaseMaterialHandle is used by RenderItemHandle::SetCustomMaterial(), 
	it also needs to know about the material requirement streams to setup corresponding 
	mesh data. It could be used by GenerateRenderItems::GenerateRenderItems() for RenderItemHandle.
	\return the material required streams.
	*/
	GraphicsDriverAPI virtual const MaterialRequiredStreams* GetRequiredStreams() const;

	/** Activates the material handle with the specified pipeline context. 
	\param[in] drawContext The pipeline context.
	*/
	GraphicsDriverAPI void Activate(DrawContext& drawContext);

	/** Terminates the material handle, performing any cleanup required by the material handle. 
	*/
	GraphicsDriverAPI void Terminate();

	/** Retrieves the number of passes required to render the material handle.  
	Each geometry to be drawn with the effect must be drawn that number of times.
	This pass count can only be retrieved while the effect is active.
	\param[in] drawContext The pipeline context.
	\return Returns pass count.
	*/
	GraphicsDriverAPI unsigned int GetPassCount(DrawContext& drawContext) const;

	/** Activates the specified pass of the material handle, at which point it may render geometry as necessary for the pass.
	\param[in] drawContext The pipeline context.
	\param[in] pass The index of pass.
	*/
	GraphicsDriverAPI void ActivatePass(DrawContext& drawContext, unsigned int pass);

	/** Called after all of the passes for the material handle have been drawn. 
	\param[in] drawContext The pipeline context.
	*/
	GraphicsDriverAPI void PassesFinished(DrawContext& drawContext);
};

} } // end namespace
