//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "SmartHandle.h"
#include "IDisplayCallback.h"
#include "TargetHandle.h"
#include "../color.h"

namespace MaxSDK { namespace Graphics {

/** ImmediateFragment is a class which let application render primitives outside
the render loop. Application can create this fragment instance like mouse proc, 
render primitives onto a separate target and then present it to a separate window 
or draw it to viewport via retain mode.
Typical usage should be:
ImmediateFragment renderFrag;
renderFrag.Initialize();
if (renderFrag.Begin())
{
	renderFrag.SetTarget();
	renderFrag.Clear();
	renderFrag.DrawCallback();
	renderFrag.End();
}
*/
class ImmediateFragment : public SmartHandle
{
public:
	GraphicsDriverAPI ImmediateFragment();
	GraphicsDriverAPI ImmediateFragment(const ImmediateFragment& from);
	GraphicsDriverAPI ImmediateFragment& operator = (const ImmediateFragment& from);
	GraphicsDriverAPI virtual ~ImmediateFragment();
	/** Initialize the handle. Must be called before using it.
	\return true if success.
	*/
	GraphicsDriverAPI virtual bool Initialize();
	/** Begin a frame
		Application must call ImmediateFragment::Begin() before any other methods and call 
		ImmediateFragment::End() when rendering is complete. ImmediateFragment::Begin()/End() 
		pair can't nest. If the function return false, other methods also fail and won't do 
		anything. And no need to call ImmediateFragment::End()
	*/
	GraphicsDriverAPI virtual bool Begin();

	/** End a frame
		Application must call ImmediateFragment::End() before calling ImmediateFragment::Begin()
		again. This method sends jobs to internal render queue. And application must call 
		this method before presenting a target.
	*/
	GraphicsDriverAPI virtual void End();
	/** Sets a target to the device, any drawing method will render primitives on this target
		The viewport will be set to full size of this target.
		Application must set a target to the device. There's no default render target after
		ImmediateFragment::Begin(). This because current rendering viewport is undefined outside
		the nitrous thread.
		Internally, there will be a same size depth target set with the target. Application
		can control depth behavior via IPrimitiveRenderer.
		/param[in] target can either be TargetHandle or PresentableTargetHandle
	*/
	GraphicsDriverAPI virtual void SetTarget(const TargetHandle& target);
	/**
		Gets current target. Default target is NULL after ImmediateFragment::Begin(). This method
		/returns target set by previous SetTarget() function.
	*/
	GraphicsDriverAPI virtual TargetHandle GetTarget() const;
	/** Clear current target by color
	    Internally this method also clears the depth buffer attached with the color buffer.
		/param[in] color The color after target is cleared. Typically it's Color(0,0,0).
		/return true if successfully cleared the target
	*/
	GraphicsDriverAPI virtual bool Clear(const Color& color);
	/** Calls IDisplayCallback::Display() function to do the render jobs
		Note the callback doesn't relate to any of the viewport. So all members of DisplayCallbackContext
		in DoDisplay() function remain default value. You can't get ViewExp and CurrentNode in the ImmediateFragment
		/param[in] user define IDisplayCallback class instance.
		/return false if any error like not calling ImmediateFragment::Begin() or ImmediateFragment::SetTarget()
	*/
	GraphicsDriverAPI virtual bool DrawCallback(const IDisplayCallbackPtr& pCallback);
};

} } // end namespace
