//
// Copyright 2013 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once
#include "RenderEnums.h"
#include "../maxheap.h"

namespace MaxSDK { namespace Graphics {

	/** This struct is used to store some device related information.
	*/
	struct DeviceCaps : public MaxHeapOperators
	{
		/**The current viewport virtual device's feature level
		*/
		GraphicFeatureLevel FeatureLevel;
	};
} }// end namespace 
