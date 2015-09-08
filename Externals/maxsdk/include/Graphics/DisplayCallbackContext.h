//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../maxtypes.h"
#include "../MaxHeap.h"
#include "GraphicsExport.h"

namespace MaxSDK { namespace Graphics {

/** This class contains context which can be queried in IDisplayCallback
*/
class DisplayCallbackContext : public MaxHeapOperators
{
protected:
	GraphicsDriverAPI DisplayCallbackContext();
	GraphicsDriverAPI virtual ~DisplayCallbackContext();

protected:
	void* mpImpl;
};

} } // namespaces
