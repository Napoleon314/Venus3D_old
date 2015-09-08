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
#include "DrawContext.h"
#include "HittestContext.h"

namespace MaxSDK { namespace Graphics {

/** This class is used to customize the behavior of a render item. It can be set via CustormRenderItemHandle::SetImplementation.
The main behavior of a render item is defined in 2 functions: Realize() and Display().
Realize() is called once per frame. It is used to prepare render data for the Display() function. To improve performance, it's
recommended to build and store geometry data in this function. You should build vertex streams to at least meet material requirements 
in the pipeline context.
Display() might be called more than once. You should follow material requirements in the pipeline context to draw the geometry.
In some cases, the render item may looks different between viewports (for example, billboard). So you may build geometry dynamically 
in the Display() function. The vertex stream should always follow material requirements in the pipeline context.
*/
class ICustomRenderItem : public ARefObject, public BaseInterfaceServer
{
public:
	/** Realize() might be called once per frame. However, if the render item is culled by Max, it won't call Realize() any more.
	Realize() is designed to build necessary data for the Display() function. Most of the requirement and pipeline states are stored in the
	PiplineContext. So you should at least build vertex buffers to meet the material requirements.
	Since this function may be called every frame, it's better to have some cache mechanism to avoid unnecessary geometry rebuilding.
	\param[in] drawContext context of the pipeline.
	*/
	virtual void Realize(DrawContext& drawContext) = 0;

	/** Display() might be called more than once per frame. Users can get a virtual device or legacy graphics window from the
	pipeline context. It's your job to activate a material (either from the pipeline context or using a custom material), loop all passes,
	set the vertex format, set the vertex stream, index and call VirtualDevice::Draw()/DrawInstanced() to render geometries. Always make sure
	the vertex format should be same as pipeline context's material requirement (if it exists). And the vertex stream layout should meet the
	format.
	\param[in] drawContext context of the pipeline.
	*/
	virtual void Display(DrawContext& drawContext) = 0;
	
	/** Get the number of primitives in this render item. Sometimes a render item doesn't have an explicit primitive count (such as a procedure
	item). In these cases, returning a non-accurate count is acceptable.
	This primitive count is mainly used for BSP cell creation.
	\return number of primitives in this render item.
	*/
	virtual size_t GetPrimitiveCount() const = 0;

	/** This function is called in a hit test pass. All custom render items which use a different render code path need to implement this interface.
	Users can get hit test information from the HitTestContext and use the draw context	to do a hardware hit test. Max uses occlusion query-based hit test. 
	Each render node is associated with an occlusion query. So a typical approach in this HitTest() is call Display() to render the item. Max 
	will get the render result to find out which nodes are hit.*/
	virtual void HitTest(HitTestContext& /*hittestContext*/, DrawContext& drawContext)
	{
		Display(drawContext);
	}
	
	/** This function will be called when the render item is hit. Some objects (like cameras) may need special operations rather than selection of the node
	when specific components are hit. 
	For this scenario, write a separate render item and override the OnHit() function to do the those actions.
	*/
	virtual void OnHit(HitTestContext& /*hittestContext*/, DrawContext& /*drawContext*/) {}
};

typedef RefPtr<ICustomRenderItem> ICustomRenderItemPtr;

} } // end namespace
