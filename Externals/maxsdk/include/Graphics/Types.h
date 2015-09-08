//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include <crtdefs.h>

namespace MaxSDK { namespace Graphics {

	/** This is the unique id type of our internal graphics objects. 
	*/
	typedef unsigned __int64 Identifier;

	/** Invalid id value.
	*/
	const Identifier NIL_ID = (Identifier)-1;

	class IObjectDisplay;
	class IRenderItemContainer;

	class SmartHandle;
	class StreamFormatHandle;
	class RenderItemHandle;
	class RenderNodeHandle;

	class IRenderStateManager;
	class IDisplayCallback;
	class IVirtualDevice;
	class IViewportViewSetting;

	class BaseMaterialHandle;
	class MaterialRequiredStreams;

	struct ShadowParameters;


	/** the struct that defines the width and height of one rectangle.
	*/
	struct RectangleSize
	{
		/** the width of one rectangle.
		*/
		size_t cx;
		/** the height of one rectangle.
		*/
		size_t cy;
	};

	/** record the dirty flags of render view. See the enumeration RenderViewDirtyFlag.
	*/
	typedef unsigned long Bitfields;
} } // namespaces
