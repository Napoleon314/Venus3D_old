// Copyright 2014 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../Types.h"
#include "../IRefObject.h"
#include "RenderRegion.h"
#include "../ICamera.h"

class ViewExp;

namespace MaxSDK { namespace Graphics {

struct ViewParameterData;

/** This class wrap up all viewport related parameters.
	For e.g., the width and height of render view 
	render region,frame draw method etc.,.
*/
class ViewParameter : public MaxSDK::Graphics::ARefObject
{
public:
		/** Constructor.
		*/
		GraphicsDriverAPI ViewParameter();
	
		/** Destructor.
		*/
		GraphicsDriverAPI virtual ~ViewParameter();

		/** Get the render view's ViewExp
		\return the ViewExp of the render view.
		*/
		GraphicsDriverAPI ViewExp* GetViewExp() const;

		/** Set the render view's ViewExp
		\param[in] viewExp the ViewExp of the render view.
		*/
		GraphicsDriverAPI void SetViewExp(ViewExp* viewExp);

		/** Get the render view's width
		\return the width of the render view.
		*/
		GraphicsDriverAPI size_t GetWidth() const;

		/** Get the render view's height
		\return the height of the render view.
		*/
		GraphicsDriverAPI size_t GetHeight() const;

		/** Set the render view's width and height
		\param[in] viewSize the rectangle size of the render view.
		*/
		GraphicsDriverAPI void SetSize(const RectangleSize& viewSize);

		/** Set the render region of the render view.
		\param[in] renderRegion this parameter is one struct type that includes the clipping rectangle,offset and scales 
		of the original render target in the screen space of the render view.
		*/
		GraphicsDriverAPI void   SetRenderRegion(const RenderRegion& renderRegion);

		/** Get the render region of the render view.
		\return the render region that includes the scales,clipping rectangle and offset of the original 
		render target in the screen space of the render view.
		*/
		GraphicsDriverAPI RenderRegion GetRenderRegion() const;

		/** Indicate whether the render region intersection with the view after computation.
		\return true if having intersection or false otherwise.
		*/
		GraphicsDriverAPI bool IsRenderRegionVisible() const;

		/** Get the intersection position in the view coordinate if having intersection.
		\return the intersection position in the view coordinate.
		*/
		GraphicsDriverAPI const FBox2& GetClippedTargetRegion() const;

		/** Get the intersection position in the source coordinate if having intersection.
		\return the intersection position in the source coordinate.
		*/
		GraphicsDriverAPI const FBox2& GetClippedSourceRegion() const;

		/** Get the camera of the render view
		\return the camera of the render view
		*/
		GraphicsDriverAPI CameraPtr GetCamera() const;

protected:
		ViewParameterData* mpData;
};

typedef MaxSDK::Graphics::RefPtr<ViewParameter> ViewParameterPtr;

} }// namespaces