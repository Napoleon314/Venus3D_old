//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "Types.h"
#include "GraphicsExport.h"
#include "../MaxHeap.h"
#include "../maxtypes.h"

class ViewExp;

namespace MaxSDK { namespace Graphics {

class UpdateViewContextImpl;

/** This class contains view dependent information for calling IObjectDisplay2::UpdatePerViewItems()
*/
class UpdateViewContext : public MaxHeapOperators
{
public:
	GraphicsDriverAPI UpdateViewContext();
	GraphicsDriverAPI virtual ~UpdateViewContext();

	/** Get the current view for updating per view items which would be used by
		IObjectDisplay2::UpdatePerViewItems(). And during the execution of 
		IObjectDisplay2::UpdatePerNodeItems(), NULL would be returned for this function.
		\return the current view for updating per view items.
	*/
	GraphicsDriverAPI ViewExp* GetView() const;

	/** Set the current view for updating per view items.
		\param pView the current view for updating per view items.
	*/
	GraphicsDriverAPI void SetView(ViewExp* pView);

protected:
	UpdateViewContextImpl* mpImpl;
};

} } // end namespaces
