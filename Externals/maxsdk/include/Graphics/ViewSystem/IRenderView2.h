// Copyright 2014 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "IRenderView.h"
#include "IFixedViewportPipeline.h"
#include "../FragmentGraph/FragmentGraph.h"

namespace MaxSDK { namespace Graphics {

/** IRenderView2 provide the access to IFixedViewportPipeline with which 3rd party 
	developer can add some extended behavior to the fixed render pipeline. For e.g.,  
	developer may specify their own background fragment, shading fragment for the viewport. 
	Even developer can override the current view whole fragment graph with a customized
	fragment graph.
	To get access to a IRenderView2 instance, please use ViewExp18::GetRenderView().
	\code
	ViewExp18& viewExp18 = static_cast<ViewExp18&>(GetCOREInterface7()->getViewExp(0));
	IRenderView2* pRenderView = viewExp18.GetRenderView();
	\endcode
	\sa class ViewExp18
 */
class IRenderView2 : public IRenderView
{
public:
	/** Set the override view fragment graph for the current viewport.
	\param[in] pGraph when pGraph is a non-null-pointer, it will override the default
			   fragment graph or an existing override graph of the render view. 
			   When pGraph is a null-pointer, the render view will restore the 
			   default fragment graph.
	\return true if a new override graph is assigned to the render view or the render
			view succeed to restore the default fragment graph. Or false otherwise.
	\note the override graph must have at least one output(with output index 0) whose
	type id must be PresentableTargetHandle::ClassID(). Or false would be returned.
	*/
	virtual bool SetOverrideViewGraph(FragmentGraph* pGraph) = 0;
	/** Get the override view fragment graph for the current viewport.
	*/
	virtual FragmentGraph* GetOverrideViewGraph() const = 0;
	/** Get the fixed viewport render pipeline instance for the current viewport.
	\note use IFixedViewportPipeline to add extended fragments to the current viewport 
	render pipeline.
	\sa class IFixedViewportPipeline
	*/
	virtual IFixedViewportPipeline* GetFixedViewportPipeline() = 0;
};

} } // namespaces
