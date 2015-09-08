//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "RenderItemHandle.h"
#include "IDisplayCallback.h"

namespace MaxSDK { namespace Graphics {

/** This class is used to wrap user defined IDisplayCallback class.
Application which needs immediate mode should add this class to mRenderItemHandles in
UpdateDisplay() function. 
*/
class ImmediateRenderItemHandle : public RenderItemHandle
{
public:
	GraphicsDriverAPI ImmediateRenderItemHandle();
	GraphicsDriverAPI ImmediateRenderItemHandle(const ImmediateRenderItemHandle& from);
	GraphicsDriverAPI ImmediateRenderItemHandle& operator = (const ImmediateRenderItemHandle& from);
	GraphicsDriverAPI virtual ~ImmediateRenderItemHandle();
	/** Initialize the handle. Must be called before use it.
	\return true if success.
	*/
	GraphicsDriverAPI virtual bool Initialize();
	/** Set user defined display callback. pCallback is a user class which inherits 
	    from MaxSDK::Graphics::IDisplayCallback. 
	*/
	GraphicsDriverAPI virtual void SetDisplayCallback(const IDisplayCallbackPtr& pCallback);
};

} } // end namespace
