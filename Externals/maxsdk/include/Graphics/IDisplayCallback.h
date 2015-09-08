//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../baseinterface.h"
#include "GraphicsExport.h"
#include "IRefObject.h"
#include "IDisposable.h"
#include "DisplayCallbackContext.h"

namespace MaxSDK { namespace Graphics {

class IPrimitiveRenderer;
class MaterialRequiredStreams;

/** A developer would implement this interface to allow immediate mode drawing of primitives, text and markers.  
The display system will query for this interface and the Display method will be called.
\remarks The pointer to the IPrimitiveRenderer should not be stored as its life time is managed by the 
graphics driver
\brief A callback used to render items using IPrimitiveRenderer.
*/
class IDisplayCallback : 
	public ARefObject, 
	public IDisposable
{
public:
	GraphicsDriverAPI IDisplayCallback();
	GraphicsDriverAPI virtual ~IDisplayCallback();

	/** A wrapper of DoDisplay() function.
		This function additional checks if current object is disposed.
	   */
	GraphicsDriverAPI void Display(
		TimeValue t, 
		IPrimitiveRenderer& renderer, 
		const DisplayCallbackContext& displayContext);

protected:
	/** Prototype of user defined render procedure.
	   User should implement this method, and this method is called when rendering.
	Note: this interface has reference counter, after creation you need to call AcquireInterface, 
	when you want to release it, call ReleaseInterface.
	\param[in] t Current time
	\param[in] renderer Instance of IPrimitiveRender. User can use this class 
		to draw geometries, markers, and texts.
	\param[in] displayContext The display context. 
	*/
	virtual void DoDisplay(
		TimeValue t, 
		IPrimitiveRenderer& renderer, 
		const DisplayCallbackContext& displayContext) = 0;
};

typedef RefPtr<IDisplayCallback> IDisplayCallbackPtr;

} } // namespaces
