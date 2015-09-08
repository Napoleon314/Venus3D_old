//
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../../../maxsdk/include/Graphics/RenderNodeHandle.h"
#include "../../../maxsdk/include/box3.h"

namespace MaxSDK { namespace Graphics {

struct RenderNodeIteratorImp;

/**	RenderNodeIterator is the iterator class to iterate all render nodes in the
	current render world.
*/
class RenderNodeIterator
{
public:
	GraphicsDriverAPI RenderNodeIterator(RenderNodeIteratorImp&);
	GraphicsDriverAPI RenderNodeIterator(const RenderNodeIterator& rhs);
	GraphicsDriverAPI ~RenderNodeIterator();

	/** Advances the node iterator by one position. This function is similar as
		operator ++.
		\return the iterator itself
	*/
	GraphicsDriverAPI RenderNodeIterator& Increment();

	/** Copy assignment of this iterator from anther iterator
		\param[in] right the iterator to be copied
		\return the iterator itself
	*/
	GraphicsDriverAPI RenderNodeIterator& operator = (const RenderNodeIterator &right);

	/** Check if this iterator is not equal with another one
		\param[in] right the iterator to be compared against
		\return true if both iterator are not same or false otherwise
	*/
	GraphicsDriverAPI bool operator != (const RenderNodeIterator& right);

	/** Check if this iterator is equal with another one
		\param[in] right the iterator to be compared against
		\return true if both iterator are same or false otherwise
	*/
	GraphicsDriverAPI bool operator == (const RenderNodeIterator& right);

	/** Dereferencing the iterator to a pointer to the current render node
		\return the pointer to the current render node.
	 */

	GraphicsDriverAPI RenderNodeHandle* operator -> ();
	/** Dereferencing the iterator to a reference to the current render node
	 * \return the reference to the current render node.
	 */
	GraphicsDriverAPI RenderNodeHandle& operator * ();
private:
	RenderNodeIteratorImp* mpImp;
};

/**	RenderWorldHandle is the base wrapper class of the nitrous world.
*/
class RenderWorldHandle : public SmartHandle
{
public:
	GraphicsDriverAPI RenderWorldHandle();
	GraphicsDriverAPI RenderWorldHandle(const RenderWorldHandle& from);
	GraphicsDriverAPI RenderWorldHandle& operator = (const RenderWorldHandle& from);
	GraphicsDriverAPI virtual ~RenderWorldHandle();

	/** Initialize this render world. 
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool Initialize();
	/** Call Reset() to clear all render nodes.
	*/
	GraphicsDriverAPI void Reset();

	/** Add a render node to the world
	\param[in] node the render node to be added
	\return true if successfully added, false otherwise.
	*/
	GraphicsDriverAPI bool AddNode(const RenderNodeHandle& node);
	/** Remove a render node from the world
	\param[in] node the render node to be removed
	*/
	GraphicsDriverAPI void RemoveNode(const RenderNodeHandle& node);
	/** Synchronize changes to all render nodes from client side to server side.
	*/
	GraphicsDriverAPI void UpdateNodes();

	/** Get count of all render nodes in the world.
	\return the count of nodes
	*/
	GraphicsDriverAPI size_t GetNumberOfNodes() const;
	/** Returns an iterator referring to the first node in the render world.
	\return an iterator referring to the first node in the render world.
	*/
	GraphicsDriverAPI RenderNodeIterator NodeBegin();
	/** Returns an iterator referring to the past-the-end node in the render world.
	\return an iterator referring to the past-the-end node in the render world.
	*/
	GraphicsDriverAPI RenderNodeIterator NodeEnd();

	/** Get the bounding box of all render nodes in the world
	\return the bounding box of all render nodes in the world
	*/
	GraphicsDriverAPI const Box3& GetWorldBox();
	/** Get the bounding box of all render nodes casting shadows in the world
	\return the bounding box of all render nodes casting shadows in the world
	*/
	GraphicsDriverAPI Box3 GetShadowWorldBox();
};

} } // namespaces

