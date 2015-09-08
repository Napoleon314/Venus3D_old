//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once
#include "../Graphics/IRefObject.h"
#include "../baseinterface.h"
#include "../MaxHeap.h"
#include "IConsolidationKey.h"

namespace MaxSDK { namespace Graphics {
class IConsolidationRenderItem;

/** This class defines a strategy to consolidate one type of consolidation key.
*/
class IConsolidationStrategy : public MaxHeapOperators
{
public:	
	/** Create a new consolidation item based on the key
	\return a new consolidation render item
	*/
	virtual IConsolidationRenderItem* CreateConsolidationItem(const IConsolidationKey& key) = 0;

	/** Returns whether this strategy supports immediate consolidation.
	Standard consolidation works on a 3-seconds-rule. This means a render item will be sent to
	a corresponding consolidation item after it doesn't change for 3 seconds.
	Immediate consolidation works on a frame-by-frame basis. It's useful for many small animated
	objects, such as bones. Rendering these objects is very slow due to poor batching performance. 
	With immediate consolidation, items are sent to a consolidation item immediately after being added to the world.
	\return true if the strategy supports immediate consolidation, otherwise false.
	*/
	virtual bool ImmediateConsolidationEnabled() = 0;
};

} } // end namespace
