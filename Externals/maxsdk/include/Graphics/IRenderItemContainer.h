//
// Copyright [2011] Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
//

#pragma once

#include "../MaxHeap.h"
#include "RenderItemHandle.h"

namespace MaxSDK { namespace Graphics {

/** A render item container, it provides various methods to access the contained render items.
*/
 class IRenderItemContainer : public MaxHeapOperators
{
protected:
	virtual ~IRenderItemContainer() {}

public:
	/** Get the number of render items contained in this container.
	\return the number of render items contained in this container.
	*/
	virtual size_t GetNumberOfRenderItems() const = 0;

	/** Get the i-th render item in this container.
	\param i the index of the render item to be retrieved.
	\return the i-th render item in this container.
	*/
	virtual RenderItemHandle GetRenderItem(size_t i) const = 0;

	/** Find a render item with the specified mesh element description.
	\param descriptionBits the unique ID associated with the specified mesh element 
			description.
	\sa MeshElementDescription::GetDescriptionBits()
	\return the render item with the mesh element description.
	*/
	GraphicsDriverAPI RenderItemHandle FindRenderItem(unsigned long descriptionBits) const;

	/** Find all render items with the specified mesh element description.
	\param descriptionBits the unique ID associated with the specified mesh element 
			description.
	\sa MeshElementDescription::GetDescriptionBits()
	\return the render item array with the mesh element description.
	*/
	GraphicsDriverAPI RenderItemHandleArray FindAllRenderItems(unsigned long descriptionBits) const;
	/** Add a render item into this container. 
	\param pRenderItem the render item to add.
	*/
	virtual void AddRenderItem(const RenderItemHandle& pRenderItem) = 0;

	/** Add all render items from the specified render item container into this container.
	\param renderItemContainer the render item container that the render items in which 
	would be added into this container.
	*/
	virtual void AddRenderItems(const IRenderItemContainer& renderItemContainer) = 0;

	/** Remove the render item with the specified index from the container.
	\param i the index of the render item to be removed.
	*/
	virtual void RemoveRenderItem(size_t i) = 0;

	/** Remove all render items from this container.
	*/
	virtual void ClearAllRenderItems() = 0;

};

} } // end namespace
