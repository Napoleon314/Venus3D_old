//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "GraphicsExport.h"
#include "GeometryEnums.h"
#include "RenderEnums.h"
#include "../MaxHeap.h"
// forward declarations
class Point4;

namespace MaxSDK { namespace Graphics {

	
	/** IRenderStateManager controls primitive renderer's current render style.
		It defines set-up states for all kinds of vertex and pixel processing. 
		Some render states set up vertex processing, and some set up pixel processing.
		This interface is acquired from IPrimitiveRenderer::GetRenderStates()
	*/
	class IRenderStateManager : public MaxHeapOperators
	{
		friend class IPrimitiveRenderer;
	protected:
		GraphicsDriverAPI virtual ~IRenderStateManager(){}
	public:

		/** @name Alpha blending methods
		* Methods for controlling the alpha blending algorithm
		*/
		//@{
		/**
			Set alpha blend status
		    The blend state controls how a primitive mixing with current back-buffer.
			When disabling blend, the primitive's pixel will write to back-buffer without
			any additional operation. When enable it, the final color will be computed via
			SourceBlend, DestintaionBlend and BlendOperation.
			\remarks
			An example, when SourceBlend is BlendSelector_SourceColor, DestinationBlend is
			BlendSelector_DestinationColor and BlendOperation is BlendOperation_Add, then
			the FinalColor = PrimitiveColor + BackBufferColor.
		\param bEnable true to enable alpha blend, false to disable alpha blend
		*/

		GraphicsDriverAPI virtual void SetBlendEnable(bool bEnable) = 0;

		/** Get alpha blend status
		*/
		GraphicsDriverAPI virtual bool GetBlendEnable() = 0;

		/** Set source element of alpha blend
		\param value source element
		*/
		GraphicsDriverAPI virtual void SetSourceBlend(BlendSelector value) = 0;

		/** Get source element
		*/
		GraphicsDriverAPI virtual BlendSelector GetSourceBlend() = 0;

		/** Set destination element of alpha blend
		\param value destination element
		*/
		GraphicsDriverAPI virtual void SetDestinationBlend(BlendSelector value) = 0;

		/** Get destination element
		*/
		GraphicsDriverAPI virtual BlendSelector GetDestinationBlend() = 0;

		/** Set alpha blend method
		    The alpha blend operations controls how to mix the alpha value with back-buffer's alpha.
		\param value type of alpha operation
		*/
		GraphicsDriverAPI virtual void SetBlendOperation(BlendOperation value) = 0;

		/** Get alpha blend method
		*/
		GraphicsDriverAPI virtual BlendOperation GetBlendOperation() = 0;

		/** Set source element of alpha value blend
		\param value source element
		*/
		GraphicsDriverAPI virtual void SetAlphaSourceBlend(BlendSelector value) = 0;

		/** Get source element
		*/
		GraphicsDriverAPI virtual BlendSelector GetAlphaSourceBlend() = 0;

		/** Set destination element of alpha value blend
		\param value destination element
		*/
		GraphicsDriverAPI virtual void SetAlphaDestBlend(BlendSelector value) = 0;

		/** Get destination element
		*/
		GraphicsDriverAPI virtual BlendSelector GetAlphaDestBlend() = 0;

		/** Set alpha value blend method
		\param value type of alpha operation
		*/
		GraphicsDriverAPI virtual void SetAlphaBlendOperation(BlendOperation value) = 0;

		/** Get alpha blend method
		*/
		GraphicsDriverAPI virtual BlendOperation GetAlphaBlendOperation() = 0;

		/** Set alpha value blend factor
		\param factor blend factor
		*/
		GraphicsDriverAPI virtual void SetBlendFactor(const Point4& factor) = 0;

		/** Get blend factor
		*/
		GraphicsDriverAPI virtual Point4 GetBlendFactor() = 0;

		//@}
		/**@name Rasterizer state methods
		* Set basic rasterizing states such as culling
		*/
		//@{
		/** Set polygon cull method
		\param value the cull mode
		*/
		GraphicsDriverAPI virtual void SetCullMode(CullMode value) = 0;

		/** Get polygon cull method
		*/
		GraphicsDriverAPI virtual CullMode GetCullMode() = 0;

		/** Set polygon fill mode
		\param value the fill mode
		*/
		GraphicsDriverAPI virtual void SetFillMode(FillMode value) = 0;

		/** Get current polygon fill method
		*/
		GraphicsDriverAPI virtual FillMode GetFillMode() = 0;

		/** Set point size when using point primitive.
		\param value size of the point in pixel.
		*/
		GraphicsDriverAPI virtual void SetPointSize(float value) = 0;

		/** Get current point size
		*/
		GraphicsDriverAPI virtual float GetPointSize() = 0;
		//@}

		/** @name Depth state methods
		* Enable and set depth based methods
		*/
		//@{
		/** Set whether enable depth compare and culling
		\param bEnable true to enable Z buffer function, false to disable Z buffer.
		*/
		GraphicsDriverAPI virtual void SetDepthEnable(bool bEnable) = 0;

		/** Get current depth culling status.
		*/
		GraphicsDriverAPI virtual bool GetDepthEnable() = 0;

		/** Set depth compare function
		\param value compare function
		*/
		GraphicsDriverAPI virtual void SetDepthFunction(CompareFunction value) = 0;

		/** Get compare function
		*/
		GraphicsDriverAPI virtual CompareFunction GetDepthFunction() = 0;

		/** Set whether write Z value when drawing primitives
		\param bEnable true to allow writing z values. false to disable it.
		*/
		GraphicsDriverAPI virtual void SetDepthWriteEnable(bool bEnable) = 0;

		/** Get current depth write status
		*/
		GraphicsDriverAPI virtual bool GetDepthWriteEnable() = 0;
		//@}

		/** @name Render state management
		* Method for controlling the life time of render states
		*/
		//@{
		/** Push all render states into internal stack. You can then change the states 
			and restore them by calling PopRenderStates.
			If there's only a few states to change, it's better to manage them by yourself.
		*/
		GraphicsDriverAPI virtual void PushRenderStates() = 0;

		/** Restore render states to internal stack's topmost render states and pop it out of stack
			you can use this function to restore previous pushed states.
		*/
		GraphicsDriverAPI virtual void PopRenderStates() = 0;

		/** Apply current render states to real render device. 
		\code
			A typical usage of render stack may like this:
			PushRenderStates()...
			SetRenderState()
			...
			ApplyRenderStates()
			DrawPrimitive()...
			DrawPrimitive()...
			PopRenderStates()...
		\endcode
		*/
		GraphicsDriverAPI virtual void ApplyRenderStates() = 0;
		//@}
	};

} }
