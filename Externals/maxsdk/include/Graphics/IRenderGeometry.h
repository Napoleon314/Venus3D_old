//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../GraphicsTypes.h"
#include "IRefObject.h"
#include "../baseinterface.h"
#include "RenderEnums.h"
#include "../box3.h"
#include "VertexBufferHandle.h"
#include "IndexBufferHandle.h"
#include "MaterialRequiredStreams.h"
#include "DrawContext.h"

namespace MaxSDK { namespace Graphics {

	/** IRenderGeometry is a presentation of geometry. Realize() function is used to define
	"what is" the geometry. And Display() function defines "how" the geometry looks like.
	Most of the time, inherited classes prepares vertex buffers and index buffers in the
	Realize() function, and use VirtualDevice to render these buffer in the Display() function.
	*/
	class IRenderGeometry : public ARefObject, public BaseInterfaceServer
	{
	public:
		/** This function might be called multiple times in a frame. Inherited classes need to
		use in the pipeline context ti render the geometry. It's recommend to prepare geometry
		data in the Realize() function, and only do rendering tasks in Display() function.
		Furthermore, sub-classes are not allowed to change current material parameters. So for
		multiple material instance, you should use multiple render items to store them.
		Note: the vertex buffers' format must match current stream requirement in pipeline context.
		/param drawContext the context for display
		/param start start primitive to render
		/param count primitive count to render
		/param lod current lod value from AD system
		*/
		virtual void Display(DrawContext& drawContext, int start, int count, int lod) = 0;

		/** Get the type of primitives in the geometry.
		/return the geometry's primitive type
		*/
		virtual PrimitiveType GetPrimitiveType() = 0;
		
		/** Sets type of primitives in the geometry.
		/param type the geometry's primitive type
		*/
		virtual void SetPrimitiveType(PrimitiveType type) = 0;

		/** Number of primitives the mesh represents.
		/return geometry's primitive count
		*/
		virtual size_t GetPrimitiveCount() = 0;

		/** Number of vertices in the mesh.
		/return number of vertices in the mesh.
		*/
		virtual size_t GetVertexCount() = 0;

		/** Get the stream requirement of this render geometry.
		To optimize performance, it's better to create a requirement-geometry mapping. 
		And make the render geometry read-only after created.
		\return the stream requirement which this geometry built with.
		*/
		virtual MaterialRequiredStreams& GetSteamRequirement() = 0;

		/** Get the vertex streams of this geometry
		\return vertex streams of this geometry
		*/
		virtual VertexBufferHandleArray& GetVertexBuffers() = 0;

		/** Get index buffer of this geometry.
		\return index buffer of this geometry. Might be invalid if the geometry doesn't need index buffer
		*/
		virtual IndexBufferHandle& GetIndexBuffer() = 0;
	};

	typedef RefPtr<IRenderGeometry> IRenderGeometryPtr;
} }
