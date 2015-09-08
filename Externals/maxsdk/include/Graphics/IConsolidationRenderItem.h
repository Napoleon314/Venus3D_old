//
// Copyright 2013 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once
#include "../Graphics/IRefObject.h"
#include "../baseinterface.h"
#include "IConsolidationKey.h"
#include "ICustomRenderItem.h"
#include "IConsolidationStrategy.h"
#include "DrawContext.h"

namespace MaxSDK { namespace Graphics {

/** This class defines a consolidation render item, which is used to improve
rendering performance by batching similar render items together.
*/
class IConsolidationRenderItem : public ICustomRenderItem
{
public:
	/** This function adds a render item to the consolidation item. The consolidation
	item should build geometries of these items in the Realize() function.
	After being consolidated, the render item's Realize() and Display() functions will
	no longer be called by Max.
	\return true if the item is added successfully, otherwise false.
	*/
	virtual bool AddSourceItem(ICustomRenderItemPtr& pSourceItem, DrawContext& dc) = 0;

	/** This function clears all render items held by the consolidation item.
	There are many situations that will cause Max call this function, such as
	adding or removing a render item for a node, moving an object, changing an object's data etc...
	*/
	virtual void ClearSourceItems() = 0;
};

typedef RefPtr<IConsolidationRenderItem> IConsolidationRenderItemPtr;

} } // end namespace
