//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "Types.h"
#include "DataBridgeExport.h"

namespace MaxSDK { namespace Graphics {

	/** types available for vertex fields, e.g. Float4
	*/
	enum VertexFieldType
	{
		/** unknown field
		*/
		VertexFieldTypeUnknown, 

		/** single float field
		*/
		VertexFieldFloat1, 

		/** double float field. x,y
		*/
		VertexFieldFloat2, 

		/** triple float field
		*/
		VertexFieldFloat3, 

		/** quad float field
		*/
		VertexFieldFloat4, 

		/** color field
		*/
		VertexFieldColor, 

		VertexFieldTypeCount, 
	};

	/** Indicates the usage of a vertex field of a vertex buffer. This enum is 
	used to make hardware shaders understand the usage of vertex buffer.
	*/
	enum VertexFieldUsage
	{
		/** Unknown usage.
		*/
		VertexFieldUsageUnknown, 

		/** Position
		*/
		VertexFieldUsagePosition, 

		/** Normal
		*/
		VertexFieldUsageNormal, 

		/** Vertex diffuse color.
		*/
		VertexFieldUsageDiffuse, 

		/** Vertex specular color.
		*/
		VertexFieldUsageSpecular, 

		/** Texture coordinate
		*/
		VertexFieldUsageTexcoord, 

		/** tangent
		*/
		VertexFieldUsageTangent, 

		/** bitangent
		*/
		VertexFieldUsageBitangent, 

		VertexFieldUsageCount, 
	};

	/** Indicates the data source of a vertex field of a mesh vertex buffer. e.g. 
	Position, Normal, Texture, etc. 
	*/
	enum MeshChannelCategory
	{
		/** Unknown data source.
		*/
		MeshChannelUnknown, 

		/** position
		*/
		MeshChannelPosition, 

		/** vertex normal components
		*/
		MeshChannelVertexNormal, 

		/** texture coordinate components
		*/
		MeshChannelTexcoord, 

		/** vertex color components
		*/
		MeshChannelVertexColor, 

		/** tangent components
		*/
		MeshChannelTangent, 

		/** bitangent components
		*/
		MeshChannelBitangent, 

		/** face map components
		*/
		MeshChannelFaceMap, 

		MeshChannelCategoryCount, 
	};

	/** Buffers are composed of arrays of elements all of the same type.
	This enum describes the size and interpretation of the elements in the buffer.
	Index buffers can have ushort or uint elements. 
	*/
	enum IndexType
	{
		/** not defined
		*/
		IndexTypeUnknown,

		/** unsigned 16 bit short integer, used for index buffers.
		*/
		IndexTypeShort, 

		/** unsigned 32 bit integer, used for index buffers.
		*/
		IndexTypeInt, 

		IndexTypeCount, 
	};

	/** Get the vertex stride by a given vertex type, in bytes. 
	\param type the vertex type.
	\return the vertex stride, in bytes.
	*/
	DataBridgeAPI size_t GetVertexStride(const VertexFieldType type);

	/** Get the index stride by a given index type, in bytes. 
	\param type the index type.
	\return the index stride, in bytes.
	*/
	DataBridgeAPI size_t GetIndexStride(const IndexType type);

	/** BufferUsageType Types for OGS buffer usage.
	*/
	enum BufferUsageType
	{
		/** Unknown usage type.
		*/
		BufferUsageTypeNone,
		/** Static buffer.
		*/
		BufferUsageStatic,
		/** Dynamic buffer.
		*/
		BufferUsageDynamic,

		/*for anding off the high bits
		*/
		BufferUsageMask = 0xf,       

		/** Usage for stream output
			for vb & ibs, may be used as a texture
		*/
		BufferUsageStreamTexture = 0x10,

		/** Usage for render target, it means the resouce created from video memory and
		    can be used as render-target
		    for vb & ibs & textures, may be used as a render target/stream output buffer
		*/
		BufferUsageTarget = 0x20,

		/** Usage for unordered access of the resource.
			This allows the resource to be both read and written by a shader, through the use of
			atomic functions.  Check the device capabilities to determine support.
		*/
		BufferUsageUnorderedAccess = 0x80,

		BufferUsageStructure = 0x100
	};

} } // end namespace

