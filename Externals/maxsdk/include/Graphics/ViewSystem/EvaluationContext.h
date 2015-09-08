// Copyright 2014 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../Enums.h"
#include "../Types.h"
#include "ViewParameter.h"

namespace MaxSDK { namespace Graphics {

/** This struct is a collection of elements that are necessary in a render view.
 */
struct EvaluationContext
{
	/** Constructor.
	*/
	EvaluationContext() : viewDirtyFlag(0),frameType(FrameType_Normal) {}
	
	/** view parameter including the width and height of render view, render region etc.
	*/
	ViewParameterPtr pViewParameter;

	/** record the dirty flags of render view. See the enumeration RenderViewDirtyFlag in Enum.h file.
	*/
	Bitfields viewDirtyFlag;
	
	/** indicates the type of frame.See the enumeration FrameType in Enum.h file.
	*/
	FrameType frameType;
};

} }// namespaces
