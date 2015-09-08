// Copyright 2013 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "IView.h"
#include "RenderRegion.h"

namespace MaxSDK { namespace Graphics {

/** This class is a representation of a render view which provides some common 
	functions to manipulate a viewport. For e.g., invalidate or render the view 
	port, save or load the view fragment graph etc.,.
	To get access to a IRenderView instance, please call ViewExp18::GetRenderView()
	\sa class ViewExp18
 */
class IRenderView : public IView
{
public:
	/** Load the view fragment graph from a specified xml file
	\param[in] fileName the file name of the xml file
	\return true if succeed or false otherwise.
	*/
	virtual bool LoadXML(const MCHAR* fileName) = 0;

	/** Unload the view fragment graph
	\return true if succeed or false otherwise.
	*/
	virtual bool UnloadXML() = 0;

	/** Save the view fragment graph to a specified xml file
	\param[in] fileName the file name of the xml file
	\return true if succeed or false otherwise.
	*/
	virtual bool SaveXML(const MCHAR* fileName) = 0;

	/** Set the render view's width and height
	\param[in] width the width of the view
	\param[in] height the height of the view
	*/
	virtual void SetViewSize(const size_t width, const size_t height) = 0;

	/** Get the render view's width
	\return the width of the render view.
	*/
	virtual size_t GetViewWidth() const = 0;

	/** Get the render view's height
	\return the height of the render view.
	*/
	virtual size_t GetViewHeight() const = 0;

	/** Set the render region of the render view.
	At the beginning, the rendering target's size,left-top position and right-bottom position are as same as the view. 	They are fully overlapped.
	Once the render region is set for one render view, it means how the render region is computed in the view that includes 3 operations 
	for render target: scale,crop and offset.
	The member mbScalingBeforeClipping of RenderRegion can indicate the order of scaling and clipping.The offset order does not affect the final result.
	The member mScreenSpaceScale of RenderRegion includes the wide and height's scales by taking the render target's width and height as base.
	The member mPostRenderClippingRegion of RenderRegion includes the left-top and right-bottom positions of clipping rectangle 
	that are ratios by taking the rendering target's width and height as base.
	The member mScreenSpaceOffset of RenderRegion will horizontally and vertically move the render target in the screen space of the render view.
	At last, if the processed render target intersects with the view, the intersection region will become the render region.
	\param[in] renderRegion this parameter is one struct type that includes the clipping rectangle,offset and scales 
	of the original render target in the screen space of the render view.
	*/
	virtual void SetRenderRegion(const RenderRegion& renderRegion) = 0;

	/** Get the render region of the render view.
	\return the render region that includes the scales,clipping rectangle and offset of the original render target in the screen space of the render view.
	*/
	virtual const RenderRegion& GetRenderRegion() const = 0;
};

} }// namespaces
