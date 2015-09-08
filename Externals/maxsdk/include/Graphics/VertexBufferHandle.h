//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "SmartHandle.h"
#include "GeometryEnums.h"
#include "RenderEnums.h"
#include "../Containers/Array.h"

namespace MaxSDK { namespace Graphics {

/** VertexBufferHandle is a memory buffer that contain vertex data. Vertex buffers can 
contain any vertex type - transformed or untransformed, lit or unlit - that can 
be rendered through the use of the rendering methods. 
*/
class VertexBufferHandle : public SmartHandle
{
public:
	GraphicsDriverAPI VertexBufferHandle();
	GraphicsDriverAPI VertexBufferHandle(const VertexBufferHandle& from);
	GraphicsDriverAPI VertexBufferHandle& operator = (const VertexBufferHandle& from);
	GraphicsDriverAPI virtual ~VertexBufferHandle();

public:
	/** Initialize this vertex buffer. 
	\param stride The stride in bytes of each vertex of the newly created buffer. 
	\param bThreadSafe True if this vertex buffer is thread safe or false otherwise
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool Initialize(size_t stride, bool bThreadSafe);

	/** \deprecated Deprecated method in terms of implementation as of 3ds Max 2016. 
	Please use the above function instead
	*/ 
	GraphicsDriverAPI bool Initialize(size_t stride);

	/** Clear all vertices in the buffer and set the size of the buffer to zero.
	*/
	GraphicsDriverAPI void Clear();

	/** Get the size of each vertex of this buffer. Measured in bytes.
	\return the size in bytes of each vertex.
	*/
	GraphicsDriverAPI size_t GetVertexStride() const;

	/** Get the real number of vertices in the buffer
	\return the number of vertices.
	*/
	GraphicsDriverAPI size_t GetNumberOfVertices() const;

	/** Sets the real number of vertices in the buffer.
	\param numberOfVertices the new number of vertices.
	*/
	GraphicsDriverAPI void SetNumberOfVertices(size_t numberOfVertices);

	/** Get the capacity of the buffer
	\return the capacity
	*/
	GraphicsDriverAPI size_t GetCapacity() const;

	/** Sets the reserved capacity of the buffer.
	\param capacity capacity of the buffer
	*/
	GraphicsDriverAPI void SetCapacity(size_t capacity);

	/** Sets the number of instance contained in the buffer.
	\param count instance count
	*/
	GraphicsDriverAPI void SetInstanceCount(size_t count);

	/** Get the number of instance contained in the buffer.
	\return instance count
	*/
	GraphicsDriverAPI size_t GetInstanceCount();

	/** Set start offset of this vertex buffer
	Sometimes geometry data may store in different position when using multiple vertex buffers.
	For example, assuming VertexBuffer1 stores geometry1 and geometry2's position data, and
	VertexBuffer2 stores geometry2's normal. When you want to render geometry2, you need 
	VertexBuffer1's start offset to correct value, otherwise you'll use geometry1's position and
	geometry2's normal.
	Another situation is steam-out. When you need to stream-out data to specific position of a
	vertex buffer, you need to set start offset to that output buffer.
	\param offset the start offset of this vertex buffer
	*/
	GraphicsDriverAPI void SetStartOffset(size_t offset);

	/** Get start offset of this vertex buffer
	\return start offset of this vertex buffer
	*/
	GraphicsDriverAPI size_t GetStartOffset();

	/** This function is used for reading/writing data from/into the vertex buffer.

	\remarks if the input parameter is invalid (eg. vertexIndex + numberOfVertices 
	is greater than the total number of vertices of the buffer), the function returns 
	NULL.

	\param vertexIndex the vertex index that the lock operation starts
	\param numberOfVertices the number of vertices that this function will lock.
	\param accessType behavior of the locking. Default is write access.
	\return the buffer for developers to read/write vertex data. The size 
	of the buffer is numberOfVertices * vertexStride.
	*/
	GraphicsDriverAPI unsigned char* Lock(size_t vertexIndex, size_t numberOfVertices, MaxSDK::Graphics::AccessType accessType = MaxSDK::Graphics::WriteAcess);

	/** Unlock the buffer to update. This function must be called after Lock. 
	Once this function is called, the buffer that returned by Lock become invalid 
	and should not be used any more.
	*/
	GraphicsDriverAPI void Unlock();

	/** Sets the buffer usage type.
	\param bufferType buffer usage type
	*/
	GraphicsDriverAPI void SetBufferUsageType(BufferUsageType bufferType);
};

typedef MaxSDK::Array<VertexBufferHandle> VertexBufferHandleArray;

} } // end namespace
