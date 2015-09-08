//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "DataBridgeExport.h"
#include "Types.h"
#include "GeometryEnums.h"
#include "../maxtypes.h"
#include "../MaxHeap.h"
#include "../tab.h"

namespace MaxSDK { namespace Graphics {

/** This struct describes a single stream element that will be used by the hardware 
shader of a material. More specifically, it defines one input parameter of vertex shader. 
Based on the structure we know exactly how to bind a max mesh channel to a vertex 
shader input parameter.
*/
struct MaterialRequiredStreamElement : public MaxHeapOperators
{
public:
	DataBridgeAPI MaterialRequiredStreamElement();
	DataBridgeAPI MaterialRequiredStreamElement(const MaterialRequiredStreamElement& from);
	DataBridgeAPI MaterialRequiredStreamElement& operator = (const MaterialRequiredStreamElement& from);
	DataBridgeAPI ~MaterialRequiredStreamElement();

	/** Compare two MaterialRequiredStreamElement. 
	\param rhs the "right-hand-side" MaterialRequiredStreamElement of the compare function.
	\return 1 if this object is greater than rhs, -1 if this object is less 
	than rhs, zero otherwise.
	*/
	DataBridgeAPI int Compare(const MaterialRequiredStreamElement& rhs) const;

	/** Get the max channel id which record where is the element come from.
	\remarks this function is used for channels related to texture coordinates 
	like uv, tangent and bitangent. Since a mesh may have multiple texture 
	coordinates, this id is used to differentiate those coordinates. 
	For other vertex buffers like position and normal, the channel id is zero.
	\return the max channel id.
	*/
	inline int GetChannelID() const;

	/** Get the vertex field type of the stream element.
	\return the vertex field type.
	*/
	DataBridgeAPI VertexFieldType GetType() const;

	/** Get the channel category of the stream element.
	\return the vertex channel category.
	*/
	inline MeshChannelCategory GetChannelCategory() const;

	/** Get the usage index of the stream element.
	\return the usage index.
	*/
	inline unsigned short GetUsageIndex() const;
	
	/** Set the max channel id which record where is the element come from.
	\param maxMapChannel the channel id.
	*/
	inline void SetChannelID(int maxMapChannel);

	/** Set the vertex element type.
	\param type the vertex element type.
	*/
	DataBridgeAPI void SetType(VertexFieldType type);

	/** Set the channel category of the element.
	\param channelCategory the channel category of the element.
	*/
	inline void SetChannelCategory(MeshChannelCategory channelCategory);

	/** Set the usage index of the stream element.
	\param usageIndex the usage index of the stream element.
	*/
	inline void SetUsageIndex(unsigned short usageIndex);
	
	/** Get whether this stream contains instance data
	\return true if it's a instance data stream
	*/
	DataBridgeAPI bool GetIsInstanceStream() const;
	
	/** Set whether this stream contains instance data. Only used when the stream
	is used as instance data
	Note: only last stream of a vertex buffer array can be used as instance stream
	\param value true to indicate this is a instance stream
	*/
	DataBridgeAPI void SetIsInstanceStream(bool value);

	DataBridgeAPI void SetOffset(unsigned short offset);

	DataBridgeAPI unsigned short GetOffset() const;

	DataBridgeAPI void SetStreamIndex(unsigned char index);

	DataBridgeAPI unsigned char GetStreamIndex() const;


	inline bool operator == (const MaterialRequiredStreamElement& rhs) const;
	inline bool operator != (const MaterialRequiredStreamElement& rhs) const;
	inline bool operator < (const MaterialRequiredStreamElement& rhs) const;
	inline bool operator <= (const MaterialRequiredStreamElement& rhs) const;
	inline bool operator > (const MaterialRequiredStreamElement& rhs) const;
	inline bool operator >= (const MaterialRequiredStreamElement& rhs) const;

protected:
#pragma warning(disable:4201)
	union DataUnion
	{
		unsigned __int64 mKey;
		struct
		{
			char mChannelId;
			unsigned char mBarrier;
			unsigned short mOffset;
			unsigned char mUsageIndex;
			unsigned char mStreamIndex;
			unsigned char mFieldType; // VertexFieldType
			unsigned char mChannelCategory; // MeshChannelCategory
		};
	};
#pragma warning(default:4201)

	DataUnion mData;
};

/** This enum defines requirement flags of material additional to mesh stream 
requirements of MaterialRequiredStreams.
*/
enum MaterialRequirementFlags
{
	/** Plug-in needs to split mesh to sub meshes according to face material id. 
	This flag is on when the material is multi/sub material.
	*/
	MaterialRequirementSplitByMaterialID        = 0x00000001, 
};

class MaterialRequiredStreamsImpl;

/** This class is used for describing the mesh stream requirements of a material. 
By default, 3ds Max automatically generates MaterialRequiredStreams for all materials. 
However if a material plugin wants to provide customized hardware shaders, it must  
provide a MaterialRequiredStreams as well. Other plugins like the object plugin, 
will provide render items that suffice the requirement.

We create this class for two reasons: 

First, a mesh contains a lot of channels however a material may be interested in a 
few channels of that mesh (eg. Position and the first texture coordinate channel). 
Converting a mesh channel to vertex buffer is a time and memory consuming job, 
therefore the material provide a MaterialRequiredStreams to indicate which channel is 
used by the material to minimize the number of vertex buffers we created.

Second, after we converted mesh channels to vertex buffers, it is necessary to 
indicate details like how is the vertex buffers are organized in hardware streams 
and the usage index of the channels. The usage index is used by hardware shaders 
of the material to differentiate the channels with the same type and usage. 
For example, if a material wants uv map channel 99 be treated as TEX0 in hardware 
shaders and uv map channel 32 may be treated as TEX1. Then the following code may 
be necessary: 

\code
	MaterialRequiredStreams requirement;

	// add position, normal and other streams.
	...

	MaterialRequiredStreamElement texCoord99;
	texCoord99.SetType(EVertexType_Float3);
	texCoord99.SetUsage(EVertexUsage_Texcoord);
	texCoord99.SetChannelID(99); // set the max channel id
	texCoord99.SetUsageIndex(0);
	requirement.AddStream(texCoord99); // uv channel 99 is TEX0 in shaders

	MaterialRequiredStreamElement texCoord32;
	texCoord32.SetType(EVertexType_Float3);
	texCoord32.SetUsage(EVertexUsage_Texcoord);
	texCoord32.SetChannelID(32); // set the max channel id
	texCoord99.SetUsageIndex(1);
	requirement.AddStream(texCoord32); // uv channel 32 is TEX1 in shaders
\endcode
*/
class MaterialRequiredStreams : public MaxHeapOperators
{
	friend class VirtualDeviceImp;
public:
	DataBridgeAPI MaterialRequiredStreams();
	DataBridgeAPI MaterialRequiredStreams(const MaterialRequiredStreams& from);
	DataBridgeAPI MaterialRequiredStreams& operator = (const MaterialRequiredStreams& from);
	DataBridgeAPI ~MaterialRequiredStreams();

	/** Remove all streams from this requirement.
	*/
	DataBridgeAPI void Clear();

	/** Add a stream to this requirement. Simply return existing stream index if 
	the stream to be added is already in the array.
	\param streamElement the stream element to add.
	\return the newly added stream index, or the existing stream index.
	*/
	DataBridgeAPI size_t AddStream(
		const MaterialRequiredStreamElement& streamElement);

	/** Find the index of a stream element
	\param streamElement the element to find
	\return the index to the found element, if not found, returns (size_t)-1
	*/
	DataBridgeAPI size_t FindStream(
		const MaterialRequiredStreamElement& streamElement) const;

	/** Remove the stream element specified by the given index from this requirement.
	\param index the index to the stream element.
	*/
	DataBridgeAPI void RemoveStream(size_t index);

	/** Get the number of streams of the requirement.
	\return the number of streams.
	*/
	DataBridgeAPI size_t GetNumberOfStreams() const;

	/** Get the field of the i-th stream.
	\param i the index to the stream.
	\return the field of the i-th stream.
	*/
	DataBridgeAPI const MaterialRequiredStreamElement& GetStreamElement(size_t i) const;

	/** Compare two MaterialRequiredStreams. 
	\param rhs the "right-hand-side" MaterialRequiredStreams of the compare function.
	\return 1 if this object is greater than rhs, -1 if this object is less 
	than rhs, zero otherwise.
	*/
	DataBridgeAPI int Compare(const MaterialRequiredStreams& rhs) const;

	/** Get extra flags of this requirement. The extra flag should be a combination 
	of MaterialRequirementFlags.
	\return the extra flags. 
	*/
	DataBridgeAPI unsigned long GetFlags() const;

	/** Set extra flags of this requirement. The extra flag should be a combination 
	of MaterialRequirementFlags.
	\param flags extra flags.
	*/
	DataBridgeAPI void SetFlags(unsigned long flags);

	/** Swap another MaterialRequiredStreams with this one
	\param from another MaterialRequiredStreams.
	*/
	DataBridgeAPI void Swap(MaterialRequiredStreams& from);

	/** Set the max material ID of a multi-sub material. 
	\param value the max material ID of a multi-sub material. 
	*/
	DataBridgeAPI void SetMaxMaterialID(MtlID value);

	/** Get the max material ID of a multi-sub material.
	\returns the max material ID of a multi-sub material. Default value is 0.
	*/
	DataBridgeAPI MtlID GetMaxMaterialID() const;

	inline bool operator == (const MaterialRequiredStreams& rhs) const;
	inline bool operator != (const MaterialRequiredStreams& rhs) const;
	inline bool operator < (const MaterialRequiredStreams& rhs) const;
	inline bool operator <= (const MaterialRequiredStreams& rhs) const;
	inline bool operator > (const MaterialRequiredStreams& rhs) const;
	inline bool operator >= (const MaterialRequiredStreams& rhs) const;

protected:
	MaterialRequiredStreamsImpl* mpImpl;
};

inline int MaterialRequiredStreamElement::GetChannelID() const
{
	return (int)((unsigned int)(mData.mChannelId));
}

inline MeshChannelCategory MaterialRequiredStreamElement::GetChannelCategory() const
{
	return (MeshChannelCategory)(mData.mChannelCategory);
}

inline unsigned short MaterialRequiredStreamElement::GetUsageIndex() const
{
	return mData.mUsageIndex;
}

inline void MaterialRequiredStreamElement::SetChannelID(int maxMapChannel)
{
	mData.mChannelId = (char)maxMapChannel;
}

inline void MaterialRequiredStreamElement::SetType(VertexFieldType type)
{
	mData.mFieldType = (unsigned char)type;
}

inline void MaterialRequiredStreamElement::SetChannelCategory(MeshChannelCategory channelCategory)
{
	mData.mChannelCategory = (unsigned char)channelCategory;
}

inline void MaterialRequiredStreamElement::SetUsageIndex(unsigned short usageIndex)
{
	mData.mUsageIndex = (unsigned char)usageIndex;
}

inline bool MaterialRequiredStreamElement::operator == (
	const MaterialRequiredStreamElement& rhs) const
{
	return Compare(rhs) == 0;
}

inline bool MaterialRequiredStreamElement::operator != (
	const MaterialRequiredStreamElement& rhs) const
{
	return Compare(rhs) != 0;
}

inline bool MaterialRequiredStreamElement::operator < (
	const MaterialRequiredStreamElement& rhs) const
{
	return Compare(rhs) < 0;
}

inline bool MaterialRequiredStreamElement::operator <= (
	const MaterialRequiredStreamElement& rhs) const
{
	return Compare(rhs) <= 0;
}

inline bool MaterialRequiredStreamElement::operator > (
	const MaterialRequiredStreamElement& rhs) const
{
	return Compare(rhs) > 0;
}

inline bool MaterialRequiredStreamElement::operator >= (
	const MaterialRequiredStreamElement& rhs) const
{
	return Compare(rhs) >= 0;
}

inline bool MaterialRequiredStreams::operator == (const MaterialRequiredStreams& rhs) const
{
	return Compare(rhs) == 0;
}

inline bool MaterialRequiredStreams::operator != (const MaterialRequiredStreams& rhs) const
{
	return Compare(rhs) != 0;
}

inline bool MaterialRequiredStreams::operator < (const MaterialRequiredStreams& rhs) const
{
	return Compare(rhs) < 0;
}

inline bool MaterialRequiredStreams::operator <= (const MaterialRequiredStreams& rhs) const
{
	return Compare(rhs) <= 0;
}

inline bool MaterialRequiredStreams::operator > (const MaterialRequiredStreams& rhs) const
{
	return Compare(rhs) > 0;
}

inline bool MaterialRequiredStreams::operator >= (const MaterialRequiredStreams& rhs) const
{
	return Compare(rhs) >= 0;
}

} } // namespace
