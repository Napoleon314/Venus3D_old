//
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#include <Graphics/IRenderGeometry.h>
#include <Graphics/DrawContext.h>
#include <Graphics/MaterialRequiredStreams.h>
#include <Graphics/VertexBufferHandle.h>
#include <Graphics/IndexBufferHandle.h>
#include <export.h>

namespace MaxSDK { namespace Graphics {

struct SimpleRenderGeometryImpl;

/** SimpleRenderGeometry is a simple implementation of IRenderGeometry. SimpleRenderGeometry
	just contains the vertex buffer array, the index buffer, the primitive type,
	the vertex count and the stream format. 3rd party developer can directly use
	this class for simple mesh whose geometry could be defined using vertex buffer
	and index buffer.
*/
class SimpleRenderGeometry : public IRenderGeometry
{
public:
	DllExport SimpleRenderGeometry();
	DllExport virtual ~SimpleRenderGeometry();

	/** Display the geometry defined by the index buffer and vertex buffer
		\param[in] drawContext the context for display
		\param[in] start start primitive to render
		\param[in] count primitive count to render
		\param[in] lod current lod value from AD system
	*/
	DllExport void Display(DrawContext& drawContext, int start, int count, int lod);

	/** Get the type of primitives in the geometry.
	\return the geometry's primitive type
	*/
	DllExport PrimitiveType GetPrimitiveType();
	/** Sets type of primitives in the geometry.
		\param[in] type the geometry's primitive type
	*/
	DllExport void SetPrimitiveType(PrimitiveType type);

	/** Number of primitives the mesh represents.
		\return geometry's primitive count
	*/
	DllExport size_t GetPrimitiveCount();
	/** Set the number of primitives in the geometry.
		\param[in] count the number of primitives
	*/
	DllExport void SetPrimitiveCount(size_t count);
	
	/** Number of vertices in the mesh.
		\return number of vertices in the mesh.
	*/
	DllExport virtual size_t GetVertexCount();
	
	/** Get the stream requirement of this render geometry.
		\return the stream requirement which this geometry built with.
	*/
	DllExport MaterialRequiredStreams& GetSteamRequirement();
	/** Set the stream requirement of this render geometry.
		\param[in] streamFormat the stream requirement which this geometry built with.
	*/
	DllExport void SetSteamRequirement(const MaterialRequiredStreams& streamFormat);

	/** Get index buffer of this geometry.
		\return index buffer of this geometry. 
	*/
	DllExport IndexBufferHandle& GetIndexBuffer();
	/** Set index buffer of this geometry.
		\param[in] indexBuffer index buffer of this geometry. 
	*/
	DllExport void SetIndexBuffer(const IndexBufferHandle& indexBuffer);

	/** Add a vertex buffer to this geometry.
		\param[in] vertexBuffer the vertex buffer to be added. 
	*/
	DllExport void AddVertexBuffer(const VertexBufferHandle& vertexBuffer);
	/** Remove the index-th vertex buffer.
		\param[in] index the index of the vertex buffer to be removed
	*/
	DllExport void RemoveVertexBuffer(size_t index);

	/** Get the number of vertex buffers.
		\return	the number of vertex buffers.
	*/
	DllExport size_t GetVertexBufferCount() const;
	/** Get the index-th vertex buffer.
		\param[in] index the index of the vertex buffer
		\return the index-th vertex buffer.
	*/
	DllExport VertexBufferHandle GetVertexBuffer(size_t index) const;

private:
	virtual VertexBufferHandleArray& GetVertexBuffers();
	
	SimpleRenderGeometryImpl* mpImpl;
};

}} // namespace