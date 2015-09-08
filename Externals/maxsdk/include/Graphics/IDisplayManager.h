//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../ifnpub.h"
#include "../GetCOREInterface.h"
#include "DeviceCaps.h"

// forward declarations
class ViewExp;

/** Interface ID of class IDisplayManager*/
#define INITROUS_DISPLAY_MANAGER_INTERFACE_ID Interface_ID(0x71218f68, 0x52ab4a11)

namespace MaxSDK { namespace Graphics {

/** A core interface for accessing properties of the graphics library introduced in 3ds Max 2012.
The graphics library allows plug-ins to draw into the viewports using a retained mode API. Methods 
of this class can be used to determine if the graphics driver is running in retained mode which is 
important when porting plugins that draw into the viewport.
\sa Please refer to the what's new guide, and porting to the new Viewport Display SDK
*/
class IDisplayManager : public FPStaticInterface
{
public: 
	/*!Plug-ins that have a mesh representati​on and use Mesh::render(​) in their implementati​on of BaseObject::​Display() 
	will need to update their display logic in order to appear correctly in the viewport when the retained mode graphics 
	driver is active.  If a textured lit mesh is required and IsRetained​ModeEnabled(​) returns true, the calls to Mesh::render(​) 
	needs to be skipped.  If the plugin explicitly sets GW_WIREFRAME or unsets GW_LIT and GW_TEXTURE then the call to 
	mesh::render() can be kept in place.
	\code
	int MyPlugin::​Display(​TimeValue t, INode* inode, ViewExp *vpt, int flags) 
	{
		if (MaxSDK::​Graphics::​IsRetained​ModeEnabled(​)) 
		{
			return 0;
	​	}
	​	// other display code stays unchanged
	}
	\endcode
	\return True if the current viewport graphics driver runs in retained mode.
	*/
	virtual bool IsRetainedModeEnabled() const = 0;

	/*! Queries whether the system is performing hardware hit testing for a specific viewport.
	The new viewport graphics system is capable of performing hardware hit testing by rendering 
	all scene objects into a viewport-sized buffer using GPU.
	For performance sake, plug-ins that have a mesh representation and use Mesh::select() to perform 
	hit testing in their implementation of BaseObject::HitTest need to skip the Mesh::select() call 
	if they already implemented BaseObject::UpdateDisplay to render the mesh in retained mode and 
	IsHardwareHitTesting() returns true. By doing this, plugins can avoid duplicated mesh drawings, 
	and the mesh will be rendered in retained mode, which is significantly faster.
	\param vpt The specific viewport in which we want to know whether we are performing hardware hit testing.
	*/
	virtual bool IsHardwareHitTesting(ViewExp *vpt) const = 0;
	/** Get current device caps
	/param caps reference of a DeviceCaps structure
	/return true if success. Otherwise false.
	*/
	virtual bool GetDeviceCaps(DeviceCaps& caps) = 0;
};

/*! Helper function to access the IDisplayManager core interface
\return A pointer to IDisplayManager
*/
UtilExport MaxSDK::Graphics::IDisplayManager* GetIDisplayManager();

/*! Helper function to test whether the new viewport graphics system is running
\return TRUE if new viewport graphics is active
*/
inline bool IsRetainedModeEnabled()
{
	IDisplayManager* pGraphicsManager = GetIDisplayManager();
	return NULL != pGraphicsManager && pGraphicsManager->IsRetainedModeEnabled();
}

/*! Helper function to test whether the new viewport graphics system is performing hardware hit testing.
\param vpt The specific viewport in which we want to know whether we are performing hardware hit testing.
\return TRUE if new viewport graphics is performing hardware hit testing.
*/
inline bool IsHardwareHitTesting(ViewExp *vpt)
{
	IDisplayManager* pGraphicsManager = GetIDisplayManager();
	return NULL != pGraphicsManager && pGraphicsManager->IsHardwareHitTesting(vpt);
}


} }// end namespace 

