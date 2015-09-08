//
// Copyright 2013 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//
#pragma once
#include "../MaxHeap.h"
// forward declaration
class GraphicsWindow;

namespace MaxSDK { namespace Graphics {
	/**	This is a hit test context class. It contains functions to get/release
	a special GraphicsWindow instance, which is used for CPU based hit test.
	*/
	class HitTestContext : public MaxHeapOperators
	{
	public:
		/** Acquire an new HitTest graphics window instance. Application can
		acquire multiple graphics window instances for multithread algorithm.
		The graphics window instance can only perform hit test related operation.
		Any rendering features are ignored.
		*/
		virtual GraphicsWindow* AcquireHitTestGW() = 0;

		/** Application should always release the instance acquired from AcquireHitTestGW().
		Otherwise the context will create an new instance in the next AcquireHitTestGW() call.
		*/
		virtual void ReleaseHitTestGW(GraphicsWindow* gw) = 0;
	};
} }
