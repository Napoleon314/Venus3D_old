//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "RenderItemHandle.h"
#include "Matrix44.h"

namespace MaxSDK { namespace Graphics {

/** This is a decorator class for RenderItemHandle. RenderItemHandleDecorator is 
	mainly used when we want to create an additional instance of an existing 
	RenderItemHandle, namely a decorator. The decorator will share the same render
	geometry of the original render item but it will have some different visual 
	appearance. For e.g, it would have different material assigned, different world 
	position or visibility group, etc. In such cases, there's no need to completely 
	copy the original render item of which we can simply create a decorator. And
	we can specify the different visual appearance/behavior for the decorator.
*/
class RenderItemHandleDecorator : public RenderItemHandle
{
public:
	GraphicsDriverAPI RenderItemHandleDecorator();
	GraphicsDriverAPI RenderItemHandleDecorator(const RenderItemHandleDecorator& from);
	GraphicsDriverAPI RenderItemHandleDecorator& operator = (const RenderItemHandleDecorator& from);
	GraphicsDriverAPI virtual ~RenderItemHandleDecorator();

	/** Initialize the decorator.
		\param itemToDecorate the original render item to be decorated.
		\return true if itemToDecorate can be decorated or false otherwise. Note
		CustomRenderItemHandle can't be decorated. 
	*/
	GraphicsDriverAPI bool Initialize(const RenderItemHandle& itemToDecorate);

	/** Get the decorated render item.
		\returns the original render item to be decorated.
	*/
	GraphicsDriverAPI RenderItemHandle GetDecoratedRenderItem() const;

	/** Set the offset matrix for the decorator. The world position of this decorator
		would be an offset from the world position of the node owning it.
		\note Calling this function will simply override the inner state resulted
			  from any previous call to SetAbsoluteMatrix()
		\param offsetMatrix the offset matrix
	*/
	GraphicsDriverAPI void SetOffsetMatrix(const Matrix44& offsetMatrix);
	/** Get the offset matrix
		\param[out] offsetMatrix use this parameter to get the offset matrix 
		\return true if an offset matrix has been assign to this decorator or false
		otherwise
	*/
	GraphicsDriverAPI bool GetOffsetMatrix(Matrix44& offsetMatrix) const;

	/** Set the absolute matrix for the decorator. The world position of this decorator
		would be directly determined by absoluteMatrix.
		\note Calling this function will simply override the inner state resulted
		from any previous call to SetOffsetMatrix()
		\param absoluteMatrix the absoluteMatrix matrix
	*/
	GraphicsDriverAPI void SetAbsoluteMatrix(const Matrix44& absoluteMatrix);
	/** Get the absolute matrix
		\param[out] absoluteMatrix use this parameter to get the absolute matrix 
		\return true if an absolute matrix has been assign to this decorator or false
		otherwise
	*/
	GraphicsDriverAPI bool GetAbsoluteMatrix(Matrix44& absoluteMatrix) const;
};

} } // end namespace
