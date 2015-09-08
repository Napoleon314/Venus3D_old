// Copyright 2014 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

namespace MaxSDK { namespace Graphics {

/** Indicates the type of frame.
*/
enum FrameType
{
	/** Same as REDRAW_NORMAL i.e., we will perform a complete redraw.
	*/
	FrameType_Normal, 
	/** Same as REDRAW_BEGIN which indicates the beginning of a continuous 
	render sequence(for e.g., start dragging the view cube).
	*/
	FrameType_BeginSequence, 
	/** Same as REDRAW_INTERACTIVE which indicates the continuous state of a render  
	sequence(for e.g., while we are dragging the view cube).
	*/
	FrameType_Interactive,
	/** Same as REDRAW_END which indicates the end of a render sequence 
	(for e.g.,when we stop dragging the view cube).
	*/
	FrameType_EndSequence
};

/** Indicates the dirty flag of render view.
*/
enum RenderViewDirtyFlag
{
	/** View elements such as width,height or position are changed.
	*/
	RenderViewDirtyFlagViewChanged		= 0x1,

	/** Fragment elements such as input,out or connection are changed.
	*/
	RenderViewDirtyFlagFragmentChanged	= 0x2,
};

} }// namespaces
