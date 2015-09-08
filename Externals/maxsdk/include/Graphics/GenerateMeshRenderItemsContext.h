//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "RenderItemHandle.h"
#include "MeshElementDescription.h"
#include "MaterialRequiredStreams.h"

namespace MaxSDK { namespace Graphics {

class UpdateDisplayContext;

/** This class contains all information that will be passed to Mesh::GenerateRenderItems()
or MNMesh::GenerateRenderItems() to update its display data. All the value of this class are set by 
system and plugin developers should build render items to satisfy those requirements.
*/
class GenerateMeshRenderItemsContext : public MaxHeapOperators
{
public:
	GraphicsDriverAPI GenerateMeshRenderItemsContext();
	GraphicsDriverAPI ~GenerateMeshRenderItemsContext();

	/** Get the required streams. Required streams is for generating mesh render items. 
	\sa MaterialRequiredStreams
	*/
	GraphicsDriverAPI const MaterialRequiredStreams& GetRequiredStreams() const;

	/** Get the required streams. Required streams is for generating mesh render items. 
	\sa MaterialRequiredStreams
	*/
	GraphicsDriverAPI MaterialRequiredStreams& GetRequiredStreams();

	/** Get the count of mesh element descriptions required to be generated from
		the given mesh
		\return the count of mesh elements.
	*/
	GraphicsDriverAPI size_t GetNumMeshElementDescriptions() const;

	/** Get the i-th mesh element description
		\param[in] i the index of the mesh element description
		\return the i-th mesh element description.
	*/
	GraphicsDriverAPI const MeshElementDescription& GetMeshElementDescription(size_t i) const;

	/** Find the mesh element description based on the given descriptionBits
	\param[in] descriptionBits the description bits of the mesh element description
	\sa MeshElementDescription::GetDescriptionBits()
	\return the required mesh element description if found or NULL otherwise.
	*/
	GraphicsDriverAPI const MeshElementDescription* FindMeshElementDescription(
		unsigned long descriptionBits) const;

	/** Find the mesh element description based on the given type and mesh part.
	\param[in] type mesh element type
	\param[in] part mesh part
	\return the required mesh element description if found or NULL otherwise.
	*/
	GraphicsDriverAPI const MeshElementDescription* FindMeshElementDescription(
		MeshElementType type, MeshPart part) const;

	/** Add a new mesh element description
		\param[in] meshElementDescription the new mesh element description.
	*/
	GraphicsDriverAPI void AddMeshElementDescription(
		const MeshElementDescription& meshElementDescription);

	/** Add a built-in mesh element description
		\param[in] type the type of built-in mesh element description
	*/
	GraphicsDriverAPI void AddBuiltInMeshElementDescription(
		BuiltInMeshElementDescription type);

	/** Clear all mesh element descriptions
	*/
	GraphicsDriverAPI void ClearMeshElementDescriptions();

	/** Developer may simply call this function when objectComponentRequirements
	contains the requirements for all the render items that require to be generated.
	\remark If you require some additional render items which are usually displayed
	when mesh is edited in any sub-object level. Please still use AddMeshElementDescription
	to specify the required mesh element.
	\param[in] objectComponentRequirements the object component requirements from
	the update display context.
	*/
	GraphicsDriverAPI void GenerateDefaultMeshElementDescriptions(unsigned long objectComponentRequirements);

	/** Developer may simply call this function when updateDisplayContext contains 
		the requirements for all render items and  material streams to be generated.
	\param[in] updateDisplayContext the update display context
	*/
	GraphicsDriverAPI void GenerateDefaultContext(const UpdateDisplayContext& updateDisplayContext);

	/** There are some render items which are invisible in a max node by default
		with regard to the current state of this node. For e.g., selected edges 
		is not visible in an unselected node or digonals is not visible in a node 
		whose 'Edge Only' property is true, etc. Developers may call this function
		to remove the descriptions of those invisible render items of a given node.
	\param[in] hRenderNode the render node for which invisible descriptions will 
				be removed
	*/
	GraphicsDriverAPI void RemoveInvisibleMeshElementDescriptions(const RenderNodeHandle& hRenderNode);
	
	/** Add some new material required streams to the context. 
	\param[in] materialRequiredStreams the material required streams to add. 
	*/
	GraphicsDriverAPI void AddRequiredStreams(const MaterialRequiredStreams& materialRequiredStreams);

protected:
	MaxSDK::Array<MeshElementDescription> mMeshElementsDescriptions;
	MaterialRequiredStreams mRequiredStreams;
};

} } // end namespace
