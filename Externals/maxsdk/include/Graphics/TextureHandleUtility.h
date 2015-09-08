//
// Copyright 2013 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "GraphicsExport.h"
#include "TextureHandle.h"
#include "../MaxHeap.h"
#include "../maxtypes.h"

class Texmap;

namespace MaxSDK { namespace Graphics {

	/** This is utility class to create TextureHandle from Texmap.
	\see TextureHandle for details about the usage of TextureHandle.
	How to use:
	\code
	TextureHandle textureHandle = TextureHandleUtility::CreateTextureHandle(pTexmap, t);
	\endcode
	*/
	class TextureHandleUtility : public MaxHeapOperators
	{
	public:

		/** Create texture handle from Texmap.
		The size of texture is controlled by procedural texture size of viewport setting
		\param[in] pTexmap A pointer to a Texmap.
		\param[in] t the time.
		\return TextureHandle of Texmap.
		*/
		MaxGraphicsObjectAPI static TextureHandle CreateTextureHandle(Texmap* pTexmap, TimeValue t);
	};

}}