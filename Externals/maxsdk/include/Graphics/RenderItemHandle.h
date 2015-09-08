//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../containers/Array.h"
#include "../export.h"

#include "SmartHandle.h"
#include "GeometryEnums.h"
#include "VertexBufferHandle.h"

class Mesh;
class MNMesh;
class Matrix3;
class Point3;

namespace MaxSDK { namespace Graphics {

class RenderItemHandleArray;
class MaterialRequiredStreams;
class IConsolidationStrategy;
class IConsolidationKey;

enum ZBiasPresets
{
	ZBiasPresets_Default = -1, //Auto z bias set by system
	ZBiasPresets_Shaded = 0,
	ZBiasPresets_Wireframe = 1,
	ZBiasPresets_Gizmo = 2
};

/** This enum defines in what group a render item is visible. 
*/
enum RenderItemVisibilityGroup
{
	/** By default, all render items are in default visible group. That means 
	the system will determine when the render item will be displayed based 
	on the render item traits. If the render item is mesh, it will be displayed 
	in shaded/realistic mode. If the render item is lines, it will be displayed 
	in wireframe/edged faces mode. Any other render items including display 
	callbacks will be displayed as gizmos.
	*/
	RenderItemVisible_Unknown, 

	/** This kind of render item will only be visible if visual style is 
	shaded or realistic. 
	*/
	RenderItemVisible_Shaded, 

	/** This kind of render item is visible in wireframe visual style, 
	or edged faces is on, or "wireframe selected" is on and the node is selected.
	*/
	RenderItemVisible_Wireframe, 

	/** This kind of render item is always visible, no matter what the visual 
	style is.
	*/
	RenderItemVisible_Gizmo, 
};

/** This a structure that holds parameters used for render item consolidation
*/
struct ConsolidationData : public MaxHeapOperators
{
	/** Pointer to a IConsolidationStrategy instance.
		Plug-ins should manage the Strategy's life time.
	*/
	IConsolidationStrategy* Strategy;
	/** Pointer to a IConsolidationKey instance
		Plug-ins should manage the Key's life time.
	*/
	IConsolidationKey* Key;

	GraphicsDriverAPI ConsolidationData();
};

/** This is the base wrapper class for all viewport render items. A render item 
is the primary drawable element that holds the geometry. A plugin object that 
wants to be displayed in the viewport needs to create and maintain a set of 
concrete derived render items like TupleMeshRenderItem and expose those render 
items to max via the IObjectDisplay interface. 
\sa Class IObjectDisplay
*/
class RenderItemHandle : public SmartHandle
{
public:
	GraphicsDriverAPI RenderItemHandle();
	GraphicsDriverAPI RenderItemHandle(const RenderItemHandle& from);
	GraphicsDriverAPI RenderItemHandle& operator = (const RenderItemHandle& from);
	GraphicsDriverAPI virtual ~RenderItemHandle();

	/** Set the visibility group of this render item.
	\param group the visibility group of this render item.
	*/
	GraphicsDriverAPI void SetVisibilityGroup(RenderItemVisibilityGroup group);

	/** Get the visibility group of this render item.
	\return the visibility group of this render item.
	*/
	GraphicsDriverAPI RenderItemVisibilityGroup GetVisibilityGroup() const;

	/**Set custom material for render item.
	By default, render item uses the material from node. When a custom material 
	is set, the render item will use the set material.
	\param hMaterial The material handle.
	*/
	GraphicsDriverAPI void SetCustomMaterial(const BaseMaterialHandle& hMaterial);

	/**Get custom material for render item.
	\return the custom material.
	*/
	GraphicsDriverAPI BaseMaterialHandle GetCustomMaterial() const;

	/**  Get the description bits of this render item. The description bits correspond
	to a specific type of mesh element description. Developers may use it to query 
	a render item from an IRenderItemContainer.
	\sa MeshElementDescription::GetDescriptionBits()
	\return the description bits
	*/
	GraphicsDriverAPI unsigned long GetDescriptionBits() const;

	/** Set the unique ID that corresponds to the mesh element description of this
		render item. 
		\param descriptionBits the description bits
	*/
	GraphicsDriverAPI void SetDescriptionBits(unsigned long descriptionBits);

	/** Get the transparent hint of the current render item. This hint could helper decide
	whether to place an render item in transparent pass or opaque pass.
	\return the transparent hint of current render item.
	\sa BaseMaterialHandle::GetTransparentHint()
	*/
	GraphicsDriverAPI bool GetTransparentHint() const;
	/** Set the transparent hint of the current render item. 
	\param bTransparentHint The transparent hint.
	*/
	GraphicsDriverAPI void SetTransparentHint(bool bTransparentHint);

	/** Set consolidation strategy and key used for consolidate this render item.
	\param[in] data reference to a ConsolidationData structure
	*/
	GraphicsDriverAPI void SetConsolidationData(const ConsolidationData& data);

	/** Returns consolidation data of this render item
	\return consolidation data of this render item
	*/
	GraphicsDriverAPI const ConsolidationData& GetConsolidationData();

	/** Set the item's Z bias
		Can be any int value. Each visibility group has default z bias sets. See ZBiasPresets.
	\param[in] item z bias
	*/
	GraphicsDriverAPI void SetZBias(int bias);

	/** returns the item's z bias
	\return item z bias
	*/
	GraphicsDriverAPI int GetZBias() const;
};

/** Generate the instance data from a Matrix3 array
\remarks The input matrix array should contain object-to-world matrices. 
		Each matrix is used to render an mesh instance. The return object VertexBufferHandle is needed
		by GenerateInstanceRenderItem(). Multiple instance render items can sharing the same 
		VertexBufferHandle.
		The returned vertex buffer contains world matrices and inverse transpose world matrices. 
		This function automatically compute inverse transpose world matrices from world matrices 
		and then fill the buffer.
	\param worldTransforms pointer to world transform matrices. Each matrix is a instance of Mesh.
	\param count the number of the instances
	\return the instance data handle.
*/
GraphicsDriverAPI VertexBufferHandle GenerateInstanceData(
	Matrix3* worldTransforms,
	size_t count);

/** Generate the instance data from a texture coordinates array and a Matrix3 array
\remarks The input matrix array should contain object-to-world matrices. 
		Each matrix is used to render an mesh instance which can have different texture coordinate. 
		The return object VertexBufferHandle is needed
		by GenerateInstanceRenderItem(). Multiple instance render items can sharing the same 
		VertexBufferHandle.
		The returned vertex buffer contains texture coordinate, world matrices and inverse transpose world matrices. 
		This function automatically compute inverse transpose world matrices from world matrices 
		and then fill the buffer.
	\param UVs pointer to texture coordinate array.Each texture coordinate pair is for an instance of Mesh.
	\param worldTransforms pointer to world transform matrices. Each matrix is an instance of Mesh.
	\param count the number of the instances
	\return the instance data handle.
*/
GraphicsDriverAPI VertexBufferHandle GenerateInstanceData(
	Point3* UVs,
	Matrix3* worldTransforms,
	size_t count);

/** Generate the instance render item from a tuple mesh and a Matrix3 array
	\remarks Instance rendering is a way to draw many objects that use 
	the same geometry. These objects shares same materials and modifiers. The only difference
	between objects is world transformation. You can specify a matrix array for the mesh,
	then the mesh will be drawn for many times. Each corresponds to a matrix in the array.
	Typical usage of these instance functions should like following:
	\code
	//Sample code fragment in IDisplay::UpdateDisplay()
	//1. Generate render items from mesh.
	mesh.GenerateRenderItems(tempRenderItemArray, generateRenderItemsContext);

	//2. Generate instance data. The data can be shared by multiply instance render items.
	MaxSDK::Graphics::VertexBufferHandle hInstanceData = 
	MaxSDK::Graphics::GenerateInstanceData(Matrices, NumberOfMatrices);

	//3. Check all of the render items and covert them to instance render item.
	for (size_t i = 0; i < tempRenderItemArray.length(); ++i)
	{
		MaxSDK::Graphics::RenderItemHandle hInstanceItem;
		//4. Try to generate instance render item. Only tuple mesh render item can convert to instance render item
		if (MaxSDK::Graphics::GenerateInstanceRenderItem(hInstanceItem, tempRenderItemArray[i], hInstanceData))
		{
			//If the function successed, add the generated instance render item to final render item array(mRenderItemHandles).
			mRenderItemHandles.append(hInstanceItem) ;
		}
		else
		{
			//If the function failed, add original render item to the final array(mRenderItemHandles).
			//Typically this render item is other render item type like immediate render item.
			mRenderItemHandles.append(tempRenderItemArray[i]);
		}
	}
	\endcode
	\param hInstanceRenderItem the instance render item handle which can replace original tuple mesh render item.
	\param hTupleMeshHandle handle of a tuple mesh render item
	\param hInstanceData handle generated by GenerateInstanceData
	\return true if successful created the instance render item.
*/
GraphicsDriverAPI bool GenerateInstanceRenderItem(
	RenderItemHandle& hInstanceRenderItem,
	const RenderItemHandle& hTupleMeshHandle,
	const VertexBufferHandle& hInstanceData);

} } // end namespace
