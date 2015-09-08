// Copyright 2013 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../../point2.h"
#include "../../box2.h"

namespace MaxSDK { namespace Graphics {

/** Note: The render region can set the clipping rectangle,offset and scale of render target in the screen space of render view.
 * \see the function SetRenderRegion of Class IRenderView
*/
struct RenderRegion: public MaxHeapOperators 
{
	inline RenderRegion() 
	{
		mbScalingBeforeClipping			= true;
		mScreenSpaceScale				= Point2(1.0,1.0);
		mPostRenderClippingRegion.pmin	= Point2(0.0,0.0);
		mPostRenderClippingRegion.pmax	= Point2(1.0,1.0);
		mScreenSpaceOffset				= Point2(0.0,0.0);	
		
	}
	
	inline RenderRegion(
		bool						bScalingBeforeClipping,
		const Point2&				scale,
		const FBox2&				rect,
		const Point2&				offset)
	{
		this->mbScalingBeforeClipping	= bScalingBeforeClipping;
		this->mScreenSpaceScale			= scale;
		this->mPostRenderClippingRegion	= rect;
		this->mScreenSpaceOffset		= offset;		
	}
	
	inline RenderRegion(const RenderRegion& from)
	{
		this->mbScalingBeforeClipping	= from.mbScalingBeforeClipping;
		this->mScreenSpaceScale			= from.mScreenSpaceScale;
		this->mPostRenderClippingRegion	= from.mPostRenderClippingRegion;
		this->mScreenSpaceOffset		= from.mScreenSpaceOffset;		
	}

	inline RenderRegion& operator = (const RenderRegion& from)
	{
		this->mbScalingBeforeClipping	= from.mbScalingBeforeClipping;
		this->mScreenSpaceScale			= from.mScreenSpaceScale;
		this->mPostRenderClippingRegion	= from.mPostRenderClippingRegion;
		this->mScreenSpaceOffset		= from.mScreenSpaceOffset;
		
		return *this;
	}

	inline ~RenderRegion()
	{
		// do nothing.
	}
	
	/** the flag can indicate the order of scaling and clipping.The offset order does not affect the final result.*/
	bool	mbScalingBeforeClipping;				// Its default value is true, that means scaling operation before the clipping operation.

	/** the scale value for the cropped render target after it is positioned in the screen space of the view.
	Its X and Y are radios that take the render target's width and height as base. 
	The scaling operation take the center of the render target as reference point.*/
	Point2 mScreenSpaceScale;						// Its default value is IPoint2(1.0,1.0), that means no scale.

	/** indicate the clipping rectangle of the render target. */
	FBox2 mPostRenderClippingRegion;	// Its default value indicates the full region of the render target.

	/** the offset value of the render target in the screen space of the view. 
	They are ratios that take the view's width and height as base. */
	Point2 mScreenSpaceOffset;						// Its default value is (0.0,0.0), that means no offset in the screen space.
};

} }// namespaces
