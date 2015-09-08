//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../GraphicsTypes.h"
#include "../baseinterface.h"
#include "../acolor.h"
#include "../box2.h"
#include "../MaxHeap.h"

#include "RenderEnums.h"
#include "GraphicsExport.h"
#include "VertexBufferHandle.h"
#include "IndexBufferHandle.h"
#include "DisplayCallbackContext.h"
#include "Matrix44.h"

class Matrix3;
class IPoint2;
class IPoint3;
class Point2;
class Point3;
class Point4;

namespace MaxSDK { namespace Graphics {

	class Matrix44;
	class IRenderStateManager;
	/**
	\brief This structure is used to define vertex's texture coordinate information
	*/
	struct TextureCoordStream : public MaxHeapOperators
	{
		TextureCoordStream()
			: Dimension(0)
			, Data(NULL)
		{
		}
		//!Number of floats in a texture coordinate(from 0 to 4). "0" means no data
		int Dimension;	
		//!Pointer of the texture coordinate array. Should be same size as vertices
		float* Data; 
	};

	/** Used by DrawPrimitive and DrawIndexedPrimitive for vertex data.
		It is used for all types of primitive.  The pointers should point to arrays in system memory.
		\remark The size of the arrays should be at least the same as "primitiveCount" passed in DrawPrimitive
		and DrawIndexedPrimitive.  Simply set an array to NULL if it is not needed, however Positions should
		never be NULL, it must always have valid data.
		\brief This structure is used to define a simple vertex stream
		\sa IPrimitiveRenderer::DrawPrimitive() IPrimitiveRenderer::DrawIndexedPrimitive()

	*/
	struct SimpleVertexStream : public MaxHeapOperators
	{
		enum {
			/** The maximum number of texture streams */
			MaximalTextureStreams = 4, 
		};
		//! Position array
		Point3* Positions;
		//! Normals array
		Point3* Normals;
		//! Vertex Color array
		Point4* Colors;

		/** Texture coordinate array. The number of TextureCoordStreams is defined by "MaximalTextureStreams".
		*/
		TextureCoordStream TextureStreams[MaximalTextureStreams];
		SimpleVertexStream()
			: Positions(NULL)
			, Normals(NULL)
			, Colors(NULL)
		{
		}
	};

	/**	\par Description
		The class provides 	various ways to draw objects.  Many functions are self contained and handle
		much of the code for you by using SimpleVertexStream, whilst others give you complete control 
		over how the primitive is drawn.  For example DrawPrimitive() allows the developer to create 
		vertex and index buffers and define the actual vertex format to be used by providing a
		StreamFormatHandle which provides formating information for vertex data like Position, UVs and Normals.
		\remark This class is running in graphic driver's thread. So sharing data between max thread and 
		driver thread should be designed carefully ensuring thread safety. Users can get an instance of 
		IPrimitiveRenderer in IDisplayCallback::Display().
		\brief This class is used to draw simple primitives, markers, text 	with given material 
		and transformations.
		\sa IDisplayCallback IVertexBuffer IIndexBuffer
	*/
	class IPrimitiveRenderer : public MaxHeapOperators
	{
	protected:
		virtual ~IPrimitiveRenderer(){}
	public:
		GraphicsDriverAPI static Color DefaultMarkerColor;

		/** This method gets current row-major world transform matrix
		\return current world transform matrix
		 */
		virtual const Matrix44& GetWorldMatrix() const = 0;

		/** This method sets current row-major world transform matrix
		    for example, world matrix with translation should be:
			|  1  0  0  0 |
			|  0  1  0  0 |
			|  0  0  1  0 |
			|  x  y  z  1 |
		\param[in] worldMatrix  the new world transform matrix
		 */
		virtual void SetWorldMatrix(const Matrix44& worldMatrix) = 0;

		/** This method gets current row-major  view transform matrix
		\return current view transform matrix
		 */
		virtual const Matrix44& GetViewMatrix() const = 0;

		/** This method sets current row-major view transform matrix
		pseudo view matrix should like this:
			zaxis = normal(At - Eye)
			xaxis = normal(cross(Up, zaxis))
			yaxis = cross(zaxis, xaxis)

			|	xaxis.x           yaxis.x           zaxis.x          0	|
			|	xaxis.y           yaxis.y           zaxis.y          0	|
			|	xaxis.z           yaxis.z           zaxis.z          0	|
			|	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye) 1	|

		\param[in] viewMatrix the new view transform matrix
		 */
		virtual void SetViewMatrix(const Matrix44& viewMatrix) = 0;

		/** This method gets current row-major  projection transform matrix
		\return current 4x4 projection matrix
		 */
		virtual const Matrix44& GetProjectionMatrix() const = 0;

		/** This method sets current row-major  projection transform matrix
		typical perspective project matrix should like this:
			|	2*zNear/width		0				0					0	|
			|	0				2*zNear/height		0					0	|
			|	0					0       zFar/(zFar-zNear)			1	|
			|	0					0       zNear*zFar/(zNear-zFar)		0	|

		\param[in] projMatrix projection matrix.
		 */
		virtual void SetProjectionMatrix(const Matrix44& projMatrix) = 0;

		/** This method gets current viewport rectangle
		\return current viewport
		 */
		virtual Rect GetViewportRect() = 0;

		/** This method gets current depth range
		\return current depth range
		 */
		virtual Point2 GetDepthRange() = 0;

		/** This method sets current viewport rectangle and depth range
		\param[in] r viewport area
		\param[in] depth viewport depth range, x is near plane(default 0), y is far plane(default 1)
		 */
		virtual void SetScreenSpace(const Rect& r, const Point2& depth) = 0;
		
		/** This method clears screen
		\param[in] flags  the element to clear (Target | Stencil | ZBuffer)
		\param[in] color  the Target's clear color
		\param[in] z	  the value of depth buffer after clear
		\param[in] stencil the stencil value after clear
		 */
		virtual void Clear(
			const BufferClearFlag flags, 
			const AColor& color, 
			const float z, 
			const int stencil) = 0;

		/** This method clears sub-region of screen
		\param[in] upperleft  upper left of clear rect
		\param[in] bottomright  bottom right of clear rect
		\param[in] flags  the element to clear (Target | Stencil | ZBuffer)
		\param[in] color  the Target's clear color
		\param[in] z	  the value of depth buffer after clear
		\param[in] stencil the stencil value after clear
		 */
		virtual void Clear(
			const IPoint2& upperleft, 
			const IPoint2& bottomright,
			const BufferClearFlag flags, 
			const AColor& color, 
			const float z, 
			const int stencil) = 0;

		/** This method draws indexed primitives to screen
		\param[in] primitiveType	type of primitive (line, triangle)
		\param[in] vertices			the primitive data pointer. In system memory.
		\param[in] primitiveCount	the number of primitives.
		\param[in] indices			the vertices index array 
								A index array stores a lot of integers. Which indicates how the primitive 
								been constructed. For example, a triangle list index "0, 2, 5" means vertex
								v0, v2 and v5 consist one triangle. A line strip index "0, 3, 1" means vertex
								v0, v3, v1 consist two lines: one is v0-v3, the other is v3-v1.
		\param[in] indexCount	    size of the index buffer
		 */
		virtual void DrawIndexedPrimitiveUP(
			PrimitiveType primitiveType,
			const SimpleVertexStream& vertices, 
			int primitiveCount, 
			const int* indices, 
			int indexCount) = 0;

		/** This method draws primitives to screen
		\param[in] primitiveType	type of primitive (point, line, triangle)
		\param[in] vertices			the primitive data pointer. In system memory.
		\param[in] primitiveCount	the number of primitives.
		 */
		virtual void DrawPrimitiveUP(
			PrimitiveType primitiveType, 
			const SimpleVertexStream& vertices, 
			int primitiveCount) = 0;

		/** This method draws primitives based on the current vertex and index buffers
		\sa SetVertexBuffer SetIndexBuffer SetStreamFormat
		\param[in] primitiveType	type of primitive (point, line, triangle)
		\param[in] vertexStart		the 1st primitives vertex position in vertex buffer
		\param[in] primitiveCount	the number of primitives.
		 */
		virtual void DrawPrimitive(
			MaxSDK::Graphics::PrimitiveType primitiveType, 
			int vertexStart, 
			int primitiveCount) = 0;

		/** This method draws multiple primitives instance from vertex buffer and index buffer.
		The instance data should be saved in one of the vertex streams.
		For example, the layout of transform instance stream should be:
		\code
		first instance: float4, float4, float4, float4 //Four rows of transform matrix
		second instance: float4, float4, float4, float4 //Four rows of transform matrix
		etc.
		\endcode
		When drawing the primitives, each instance's matrix will apply to the primitive. So
		you can draw multiple instances in one call.
		\sa SetVertexBuffer SetIndexBuffer SetStreamFormat
		\param[in] primitiveType	type of primitive (point, line, triangle)
		\param[in] vertexStart		the 1st primitives vertex position in vertex buffer
		\param[in] primitiveCount	the number of primitives.
		\param[in] instanceCount	the number of instances in the stream
		 */
		virtual void DrawPrimitiveInstanced(
			MaxSDK::Graphics::PrimitiveType primitiveType, 
			int vertexStart, 
			int primitiveCount, 
			int instanceCount) = 0;

		/** Draw a given render item.
		\param[in] displayContext the current display context.
		\param[in] hRenderItem the render item to be draw.
		*/
		virtual void DrawRenderItem(
			const DisplayCallbackContext& displayContext, 
			const RenderItemHandle& hRenderItem) = 0;

		/** This method draws a quad covering the entire current target set
		\param[in] z	z value of this quad
		 */
		virtual void DrawScreenQuad(float z = 0.0f) = 0;

		/** This method draws a string at given world position
		\param[in] xyz	world position of the string
		\param[in] s	the text
		 */
		virtual void DrawText(const Point3& xyz, const MCHAR *s) = 0;

		/** This method draws a string at given screen position (upper left is the origin)
		\param[in] xy	screen position of the string
		\param[in] s	the text
		 */
		virtual void DrawText(const IPoint2& xy, const MCHAR *s) = 0;

		/** This method should be called before AddMarkers. It can be used to draw markers. The 
		system will create the appropriate buffer which will be filled up with calls to AddMarker
		\return True if successful.
		 */
		virtual bool BeginMarker() = 0;

		/** This allows drawing batches of markers at a time.  
		\remarks Only call this function between BeginMarker and EndMarker pair
		\param[in] type		Marker type
		\param[in] xyz		Pointer to a Point3 buffer which contains marker's position in world space
		\param[in] count	Number of markers
		\param[in] color	Marker's color
		\return True if successful.  If false, then the buffer has potentially overflowed. Try using EndMarker 
		and start another sequence
		*/
		virtual bool AddMarkers(
			MarkerType type, 
			const Point3 *xyz, 
			size_t count, 
			const Color& color = DefaultMarkerColor) = 0;

		/** This allows drawing batches of screen-space markers at a time.  
		needed to be drawn.
		\remarks Only call this function between BeginMarker and EndMarker pair
		\param[in] type		Marker type
		\param[in] xyz		Pointer to a Point3 buffer which contains marker's position in screen space.
							z value's range is [0, 1]. "0" is front most and "1" is back most. It is used for comparing depth
							with other geometries.
		\param[in] count	Number of markers
		\param[in] color	Marker's color
		\return True if successful.  If false, then the buffer has potentially overflowed. Try using EndMarker 
		and start another sequence
		*/
		virtual bool AddScreenSpaceMarkers(
			MarkerType type, 
			const Point3 *xyz, 
			size_t count, 
			const Color& color = DefaultMarkerColor) = 0;

		/** Ends the marker drawing sequence.  This will draw and flush the internal buffers.
		*/
		virtual void EndMarker() = 0;

		/** This method sets current material used by DrawPrimitive and DrawIndexedPrimitive
		\param material the new materials' pointer 
		*/
		virtual bool SetMaterial(const BaseMaterialHandle& material) = 0;

		/** This method gets current material
		\return current material handle
		*/
		virtual BaseMaterialHandle& GetMaterial() = 0;
		/** This method sets vertex buffers used by DrawPrimitive
		\remarks The number of steams is set by the StreamFormatHandle
		\param count    number of vertex buffers, range is 0 to max number of streams - 1
		\param pBuffers	vertex buffer array
		*/
		virtual void SetVertexBuffer(size_t count, const VertexBufferHandle* pBuffers) = 0;

		/** This method sets index buffer used by DrawPrimitive
		\param pIndexBuffer  A pointer to the index buffer object
		*/
		virtual void SetIndexBuffer(const IndexBufferHandle& pIndexBuffer) = 0;

		/** This method sets vertex format of vertex buffer.
		You can set multiple streams with different usage.
		Instance data must be a separate stream.
		\param format format of vertex buffer
		*/
		virtual void SetStreamFormat(const StreamFormatHandle& format) = 0;

		/** This method retrieves current render states object.  IRenderStateManager allows the developer
		to set various graphics states, whilst performing state management.
		\return A reference of IRenderStateManager object
		*/
		virtual IRenderStateManager& GetRenderStates() = 0;
	};

	/**	\par Description
		This is a utility class for automatically restoring the world matrix 
		when we finished rendering a render item.
	*/
	class AutoRestoreWorldMatrix : public MaxHeapOperators
	{
	public:
		/** The default ctor. 
		\param render The primitive render that is to restore its world matrix
		*/
		AutoRestoreWorldMatrix(IPrimitiveRenderer* render) : 
			mpRender(render),
			mOldWorldMatrix(render->GetWorldMatrix())
		{ }

		/** The default dtor. It will restore the world matrix
		*/
		~AutoRestoreWorldMatrix()
		{
			mpRender->SetWorldMatrix(mOldWorldMatrix);
		}
	private:
		MaxSDK::Graphics::Matrix44 mOldWorldMatrix;
		IPrimitiveRenderer* mpRender;
	};
} }
