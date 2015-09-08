//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "IRenderItemContainer.h"

namespace MaxSDK { namespace Graphics {

/** This is a utility class. It is used for storing a set of render items.
*/
class RenderItemHandleArray : public IRenderItemContainer
{
public:
	GraphicsDriverAPI RenderItemHandleArray();
	GraphicsDriverAPI RenderItemHandleArray(const RenderItemHandleArray& from);
	GraphicsDriverAPI RenderItemHandleArray& operator = (const RenderItemHandleArray& from);
	GraphicsDriverAPI virtual ~RenderItemHandleArray();

	/** Get the number of render items contained in this container.
	\return the number of render items contained in this container.
	*/
	GraphicsDriverAPI size_t GetNumberOfRenderItems() const;

	/** Get the i-th render item in this container.
	\param i the index of the render item to be retrieved.
	\return the i-th render item in this container.
	*/
	GraphicsDriverAPI RenderItemHandle GetRenderItem(size_t i) const;

	/** Add a render item into this container. 
	\param pRenderItem the render item to add.
	*/
	GraphicsDriverAPI void AddRenderItem(const RenderItemHandle& pRenderItem);

	/** Add all render items from the specified render item container into this array.
	\param renderItemContainer the render item container that the render items in which 
	would be added into this array.
	*/
	GraphicsDriverAPI void AddRenderItems(const IRenderItemContainer& renderItemContainer);

	/** Remove the render item with the specified index from the container.
	\param i the index of the render item to be removed.
	*/
	GraphicsDriverAPI void RemoveRenderItem(size_t i);

	/** Remove all render items from this container.
	*/
	GraphicsDriverAPI void ClearAllRenderItems();

private:
	MaxSDK::Array<RenderItemHandle> mRenderItems;
};

} } // end namespace
