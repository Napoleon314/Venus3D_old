//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../maxtypes.h"
#include "../baseinterface.h"
#include "../MaxHeap.h"
#include "../Point2.h"
#include "../Box2.h"

#include "GraphicsExport.h"
#include "RenderEnums.h"
#include "VertexBufferHandle.h"
#include "IndexBufferHandle.h"
#include "TargetHandle.h"

#include "RenderStates.h"

class AColor;

namespace MaxSDK { namespace Graphics {

	/**	This class provides low level access to the GPU device. It wraps the variable 3D API into an unified interface.
		This means you do not need to know whether the real device is actually Direct3D9, Direct3D11 or OpenGL.
		Similar to the device interface in Direct3D, this interface provides rendering functions such as Draw() and 
		DrawInstanced(),
		and resource functions like SetVertexStreams(), SetIndexBuffer() and so on. You can also change render states
		via Get/SetBlendState(), Get/SetRasterizerState(), and Get/SetDepthStencilState().
		For example, to change the Z-Bias of the next draw call, use RasterizerState::SetDepthBias() or 
		RasterizerState::SetSlopeScaledDepthBias().

		\sa
		For information about how to obtain the draw context and work with
		the Nitrous API, see ~{ Low-level Device API }~ and ~{ About RenderItem }~.
		You should read all the topics on ~{ Nitrous }~ before developing Nitrous plugins.
        
        For an example of code that implements this interface, see the sample project in <b>howto/Graphiccs/GPUParticle</b>.

		\par Example Code:

		Typical usage of rendering geometry data may look like this:
		\code
		class SampleItemDX9 : public ICustomRenderItem
        {
            ...
        };
        void SampleItemDX9::Display(DrawContext& dc)
        {
            IVirtualDevice& vd = drawContext.GetVirtualDevice();
            BaseMaterialHandle& material = const_cast<BaseMaterialHandle&>(drawContext.GetMaterial()); //Use draw context’s material
            vd.SetStreamFormat(const_cast<MaterialRequiredStreams&>(*material.GetRequiredStreams()));
            vd.SetVertexStreams(mVertexBuffer);
            vd.SetIndexBuffer(&mIndexBuffer);
            
            material.Activate(drawContext);
            unsigned int passCount = material.GetPassCount(drawContext);
            for (int pass = 0; pass < passCount; ++pass) //Loop through all passes
            {
                material.ActivatePass(drawContext, pass);
                vd.Draw(PrimitiveTriangleList, 0, (int)mPrimitiveCount);
            }
            material.Terminate(drawContext);
        }
		\endcode

		Typical usage of stream-out data may look like this:
		\code
		//Turn off Z-Buffer
		vd.SetDepthStencilState(mDepthOffState);
		streamOutEffect.Activate(drawContext);
		streamOutEffect.ActivatePass(drawContext, 0);
		vd.SetStreamFormat(streamOutEffect.GetStreamFormat());
		vd.SetIndexBuffer(nullptr);
		vd.SetVertexStreams(mRawDataBuffers);
		vd.SetOutputStreams(&mStreamOutBuffers);
		vd.Draw(PrimitivePointList, 0, (int)vertexCount);
		vd.SetOutputStreams(nullptr);
		streamOutEffect.PassesFinished(drawContext);
		streamOutEffect.Terminate();
		//Restore Z-Buffer state
		vd.SetDepthStencilState(mBackupState);
		\endcode
	*/
	class IVirtualDevice : public MaxHeapOperators
	{
	protected:
		virtual ~IVirtualDevice(){}
	public:
		/** Returns the thread id of the virtual device.
		\return Return 0 for DirectX9, the real thread id for DirectX11.
		*/
		virtual unsigned int ThreadID() = 0;
		
		/** Gets the status of the device.
		Note: When IsValid is false, all device-related resources may be invalid.
		Sometimes plugins may attempt to dispose after Nitrous has already done so.
		At this time, any calls to
		device resources may cause a crash. Therefore, plugins should check this flag to make
		sure the device is valid before releasing the resources.
		\return true if the device is valid. false if not.
		*/
		virtual bool IsValid() const = 0;

		/** Begins a frame draw. Should be paired with EndFrame().
		Brackets sets of drawing calls, only 1 level deep allowed.
		*/
		virtual bool BeginFrame() = 0;

		/** Ends a frame draw. Should be paired with BeginFrame().
		*/
		virtual bool EndFrame() = 0;

		/** Begins a scene draw.
		Brackets sets of drawing calls. Only 1 level deep allowed.
		*/
		virtual bool BeginScene() = 0;

		/** Ends a scene draw.
		*/
		virtual bool EndScene() = 0;

		
		/** Gets the current rasterizer state.
		\return The rasterizer state handle.
		*/
		virtual const RasterizerState& GetRasterizerState() = 0;


		/** Sets the current rasterizer state.
		\param[in] newState the new rasterizer state.
		*/
		virtual void SetRasterizerState(const RasterizerState& newState) = 0;

		
		/** Gets the current depth-stencil state.
		\return The depth stencil state handle.
		*/
		virtual const DepthStencilState& GetDepthStencilState() = 0;

		
		/** Sets the current depth stencil state.
		\param[in] newState The new depth stencil state handle.
		*/
		virtual void SetDepthStencilState(const DepthStencilState& newState) = 0;
		
		/** Gets the current blend state.
		\return The current blend state handle.
		*/
		virtual const BlendState& GetBlendState() = 0;

		/** Sets the current depth stencil state.
		\param[in] newState The new blend state handle.
		*/
		virtual void SetBlendState(const BlendState& newState) = 0;


		/** Gets the pixel coordinate viewport for the drawing.
		\return The viewport's {0,0,w,h}, or {0,0,0,0} if no target is defined.
		*/
		virtual const Box2 Viewport() = 0;

		/** Sets the pixel coordinate viewport for the drawing.
		\param[in] vp The desired viewport size, or `nullptr` which means "use the full screen".
		*/
		virtual void Viewport(const Box2* vp) = 0;
				
		/** Gets the depth range.
		\return The depth range, normally between 0.0 to 1.0. 
		*/
		virtual const Point2 DepthRange() const = 0;
		
		/** Sets the depth range.
		\param[in] depthRange The depth range, depthRange.x means near distance, depthRange.y means far distance.
		Normally the values are in the range 0.0 to 1.0.
		*/
		virtual void DepthRange(const Point2& depthRange) = 0;

		/** Gets scissor box in pixels.
		\return The scissor box. The default value is the current render target size.
		*/
		virtual const Box2 ScissorBox() = 0;

		/** Sets the scissor box in pixels.
		\param[in] box The scissor box, `nullptr` means no scissor clip.
		*/
		virtual void ScissorBox(const Box2* box) = 0;

		/** Gets the number of vertex buffer streams.
		\return The current number of active streams.
		*/
		virtual size_t VertexStreamCount() const = 0;
		
		/** Gets the specified vertex buffer stream.
		\param[in] nStream The stream index of the vertex buffer.
		\return The vertex buffer handle at nStream.
		*/
		virtual VertexBufferHandle GetVertexStream(int nStream) const= 0;

		/** Sets vertex buffer streams.
		\param[in] bufferArray The vertex buffer array.
		*/
		virtual void SetVertexStreams(VertexBufferHandleArray& bufferArray) = 0;

		/* Sets the stream format of the vertex buffer.
		\param[in] format The stream format of the vertex buffer.
		*/
		virtual void SetStreamFormat(const MaxSDK::Graphics::MaterialRequiredStreams& format) = 0;

		/** Gets the number of current output vertex buffer streams.
		\return The number of current output vertex buffer streams.
		*/
		virtual size_t OutputStreamCount() const = 0;
		
		/** Gets the specified output vertex buffer stream.
		Note: this function only works for DX11 devices.
		\param[in] nStream The index of the stream.
		\return The vertex buffer handle at output nStream. For dx9 devices (feature level < LEVEL_4_0),
		an invalid handle is returned. Use VertexBufferHandle::IsValid() to check the validity.
		*/
		virtual VertexBufferHandle GetOutputStream(int nStream) const= 0;
		
		/** Sets the current output vertex buffer stream.
		Note: The max number of output streams is 0 for DX9 devices, 4 for DX11 devices.
		\param[in] bufferArray Sets output streams. Use `nullptr` to clear output streams.
		*/
		virtual void SetOutputStreams(VertexBufferHandleArray* bufferArray) = 0;

		/** Sets the render target at index 0.
		\param[in] target A pointer for the render target handle.
		\param[in] zBuffer A pointer for the z buffer handle. The size of zBuffer must be equal to or larger 
		than the target. Can be `nullptr`. 
		*/
		virtual void SetTarget(TargetHandle* target, TargetHandle* zBuffer) = 0;

		/** Sets multiple render targets.
		\param[in] targetArray The render targets to be set. All targets must have the same size.
		\param[in] zBuffer A pointer to the z buffer handle. The size of zBuffer must be equal to or larger 
		than the render targets. Can be `nullptr`. 
		*/
		virtual void SetTargets(TargetHandleArray& targetArray, TargetHandle* zBuffer) = 0;

		/** Gets the specified render target
		\param[in] index The index of the target. For DX9, the maximum number of the index is 4, for DX11, it's 8. 
		\return The target handle. The handle might be invalid if the corresponding index doesn't exist.
		*/
		virtual TargetHandle GetTarget(int index) = 0;

		/** Gets the current render target count.
		\return The current target count.
		*/
		virtual size_t TargetCount() = 0;

		/** Gets the current z buffer.
		\return The current z buffer. The handle might be invalid if the z buffer doesn't exist.
		*/
		virtual TargetHandle GetZBuffer() = 0;
		
		/** Gets the current index buffer.
		\return The current index buffer handle.
		*/
		virtual IndexBufferHandle GetIndexBuffer() const = 0;
		
		/** Sets the current index buffer.
		\param[in] indexBuffer The index buffer handle. Pass `nullptr` to clear the current index buffer.
		*/
		virtual void SetIndexBuffer(IndexBufferHandle* indexBuffer) = 0;

		/** Draws part of a set of primitives.
		\param[in] type The primitive type.
		\param[in] start The start vertex index in the current vertex buffer.
		\param[in] primCount The number of primitives to draw.
		*/
		virtual void Draw(PrimitiveType type, int start, int primCount) = 0;

		/** Draws part of a set of primitives with an instance.
		Note: you need call VertexBufferHandle::SetInstanceCount() to set the instance count of the instance data stream.
		\param[in] type The primitive type.
		\param[in] instanceCount The number of instances.
		\param[in] start The start vertex index in the current vertex buffer.
		\param[in] primCount The number of primitives to draw.
		*/
		virtual void DrawInstanced(PrimitiveType type, int instanceCount, int start, int primCount) = 0;

		/** Performs a full clear: target( nTargets ) to a given color, z to a given z, stencil to a given stencil.
			The target (or MRT targets), instead of the viewport, is cleared.
			The clear operation is not affected by the scissor box.
		\param[in] flags The components to clear. 
		\param[in] color The color after the clear. Available when ::ClearBackbuffer or ::ClearAll is set.
		\param[in] z The z value after the clear. Available when ::ClearDepthBuffer or ::ClearAll is set.
		\param[in] stencil The stencil value after clear. Available when ::ClearStencilBuffer or ::ClearAll is set.
		*/
		virtual void Clear(BufferClearFlag flags, const AColor& color, float z, int stencil) = 0;

		/** Performs a partial clear: a sub-region of the target/z/stencil is cleared.
			The region is clipped to the bounds of the viewport rectangle, 
			and the clear operation is not affected by the scissor box.
		\param[in] rect The region to clear.
		\param[in] flags The components to clear. 
		\param[in] color The color after the clear. Available when ::ClearBackbuffer or ::ClearAll is set.
		\param[in] z The z value after the clear. Available when ::ClearDepthBuffer or ::ClearAll is set.
		\param[in] stencil The stencil value after the clear. Available when ::ClearStencilBuffer or ::ClearAll is set.
		*/
		virtual void ClearRegion(
			const Box2& rect,
			BufferClearFlag flags, 
			const AColor& color,  
			float z,  
			int stencil
			) = 0;
	};

} }
