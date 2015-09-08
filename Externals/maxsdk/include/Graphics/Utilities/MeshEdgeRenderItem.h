//
// Copyright 2013 Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
//
//

// 2013/3/21, Qinming.Ren Created.

#pragma once
#include "../ICustomRenderItem.h"
#include "../IConsolidationKey.h"
#include "../IConsolidationRenderItem.h"
#include "../IConsolidationStrategy.h"
#include "../VertexBufferHandle.h"
#include "../IndexBufferHandle.h"
#include "../HLSLMaterialHandle.h"
#include "../VertexColorMaterialHandle.h"
#include "../RenderStates.h"
#include "ItemConsts.h"
#include "../../mesh.h"
#include "../../containers/Array.h"
#include "../../Point3.h"
#include "../../matrix3.h"

namespace MaxSDK{namespace Graphics{namespace Utilities	{

	/** Class ID of the mesh edge consolidation item
	*/
	#define CONSOLIDATION_MESH_EDGE_CLASS_ID Class_ID(0x4d5090aa, 0x16e4e23)

	/** This is a utility class for provide a key for MeshEdgeRenderItem
	*/
	class MeshEdgeKey : public MaxSDK::Graphics::IConsolidationKey
	{
	private:
		size_t mHashKey;
		bool mbDirty;
		bool mbFixedSize;
		bool mbBackfaceCull;
		void BuildHashKey();
	public:
		DefaultRenderItemAPI MeshEdgeKey();

		DefaultRenderItemAPI MeshEdgeKey(MeshEdgeKey& from);

		DefaultRenderItemAPI ~MeshEdgeKey();

		DefaultRenderItemAPI MeshEdgeKey* operator=(const MeshEdgeKey& rhs);
		/** Set whether the mesh item is fixed size on screen
		\param value true means the mesh is fixed size. Otherwise false.
		*/
		DefaultRenderItemAPI virtual void SetFixedSize(bool value);

		/** Return if the mesh item is fixed size on screen
		\return true if it's fixed size. Otherwise false.
		*/
		DefaultRenderItemAPI virtual bool GetFixedSize();

		/** Sets whether the mesh item needs back-face cull
		\param value true means it needs back-face cull. Otherwise false.
		*/
		DefaultRenderItemAPI virtual void SetBackfaceCull(bool value);
		/**
		\return true if the item needs back-face cull. Otherwise false.
		*/
		DefaultRenderItemAPI virtual bool GetBackfaceCull();

		/** Create an new instance of the key
		\return new instance of the key which contains same value.
		*/
		DefaultRenderItemAPI virtual IConsolidationKey* Clone();

		/** Get hash key that combines properties of this key.
		\return hash key
		*/
		DefaultRenderItemAPI virtual size_t GetHashKey();
		/** Compares another key
		\param rhs another consolidation key which has same hash key
		\return true if both values are same. Otherwise false.
		*/
		DefaultRenderItemAPI virtual bool Equals(const IConsolidationKey& rhs) const;
		/** Get the key's class ID
		\return class ID of the key
		*/
		DefaultRenderItemAPI virtual const Class_ID GetClassID() const;
	};

	/** This class defines an immediate mesh edge render item consolidation strategy.
		If user plugins have complex mesh data that can't support immediate consolidation,
		they need to create own strategy with ImmediateConsolidationEnabled returns false.
	*/
	class MeshEdgeConsolidationStrategy : public MaxSDK::Graphics::IConsolidationStrategy
	{
		MeshEdgeConsolidationStrategy(MeshEdgeConsolidationStrategy&from);
		DefaultRenderItemAPI MeshEdgeConsolidationStrategy() {}
		DefaultRenderItemAPI virtual ~MeshEdgeConsolidationStrategy() {}
	public:	
		/** Get instance of the strategy. The instance is global object
		\return instance of MeshEdgeConsolidationStrategy
		*/
		DefaultRenderItemAPI static MaxSDK::Graphics::IConsolidationStrategy& GetInstance();

		/** Create an new mesh edge consolidation item
		\return an new mesh edge consolidation item
		*/
		DefaultRenderItemAPI virtual IConsolidationRenderItem* CreateConsolidationItem(const IConsolidationKey& key);

		/** Returns whether this strategy supports immediate consolidation
		If user plugins have complex mesh data that can't support immediate consolidation,
		they need to create own strategy with ImmediateConsolidationEnabled returns false.
		\return for this class, always return true.
		*/
		virtual bool ImmediateConsolidationEnabled() {return true;}
	};

	
	/** This class is designed for small meshes that only renders edge element.
		This use case is typically used by gizmos like dummy, lights which originally uses mesh.render 
		with GW_EDGES_ONLY flag to draw edges.
		The back-face cull calculation is done by software under DX9 and using hardware shader under DX11.
	*/
	class MeshEdgeRenderItem : public MaxSDK::Graphics::ICustomRenderItem, public MaxSDK::Util::Noncopyable
	{
		friend class MeshEdgeConsolidationItemDX11;
		Mesh* mpMesh;
		Edge* mpEdgeList;
		Color mColor;
		size_t mEdgeListCount;
		DWORD mdwDirtyFlags;
		bool mbBackfaceCull;
		bool mbFixedSize;
		bool mbMakeMeshCopy;
		MaxSDK::Array<int> mEdgeArray; //Edge array reserved for hit test.
		MaxSDK::Array<UINT> mCullEdge0; //Faces that has 1 edge invisible
		MaxSDK::Array<UINT> mCullEdge01; //Faces that has 2 edges invisible
		MaxSDK::Array<UINT> mNoCulling; //Faces that all edges are visible

		MaxSDK::Graphics::VertexBufferHandleArray mMeshVB;
		MaxSDK::Graphics::IndexBufferHandle mMeshIB;
		DefaultRenderItemAPI void AppendFace(Face& face, DWORD flag, bool reverse);
		DefaultRenderItemAPI void BuildVertexBuffer(DrawContext& drawContext);
		DefaultRenderItemAPI void BuildIndexBuffer();
		DefaultRenderItemAPI void DrawMeshEdge(DrawContext& drawContext);
	public:
		/** Constructor
		\param pMesh mesh to display
		\param fixedSize indicate whether this item's mesh has fixed size on the screen
		\param copyMesh whether to make a copy of the mesh. 
		If true, the item will manage the copied new mesh's life time. If the mesh is created on the stack and
		released after constructing the item, plugin needs to set this flag true to make sure the mesh is still
		available when realizing.
		*/
		DefaultRenderItemAPI MeshEdgeRenderItem(Mesh* pMesh, bool fixedSize, bool copyMesh);
		DefaultRenderItemAPI virtual ~MeshEdgeRenderItem();
		/** Return value of the flag
		\param flag specifies the flag to set
		*/
		DefaultRenderItemAPI DWORD CheckFlags(DWORD flag);
		/** Set the specified flag to 0
		\param flag the flag to clear
		*/
		DefaultRenderItemAPI void ClearFlags(DWORD flag);
		/** Set the specified flag to 1
		\param flag the flag to set
		*/
		DefaultRenderItemAPI void SetFlags(DWORD flag);

		/** Set back-face cull status of the item
		Note this value must same as key's back-face cull value. Otherwise this item might
		be consolidated by incorrect consolidation item.
		\param value true to enable back-face cull. false to disable.
		*/
		DefaultRenderItemAPI void SetBackfaceCull(bool value);
		/** Get back-face cull status
		\return back-face cull status of the item
		*/
		DefaultRenderItemAPI bool GetBackfaceCull();

		/** Get mesh edge list
		\param[out] pEdgeOut edge list of the mesh
		\param[out] count number of data in the array
		*/
		DefaultRenderItemAPI void GetEdgeList(Edge*& pEdgeOut, size_t& count);

		/** Set color of the edges
		\param color color the the edges
		*/
		DefaultRenderItemAPI void SetColor(Color& color);
		/** Get the color of edges
		\return color of the edge
		*/
		DefaultRenderItemAPI Color GetColor();

		/** Get the mesh used by render item
		\return pointer of the mesh
		*/
		DefaultRenderItemAPI Mesh* GetMesh();

		/** Build mesh data used by consolidation
		*/
		DefaultRenderItemAPI virtual void Realize(DrawContext& drawContext);

		/** Dummy Display() function
		Since MeshEdgeRenderItem is a immediate consolidation item, display is handled by consolidation item. 
		This function should never be called.
		*/
		DefaultRenderItemAPI virtual void Display(DrawContext& drawContext);

		/** Hit Test function for this render item
		Since the meshes are always rendered by consolidation item, in the render item's hit test function
		we need to build separate data to draw the item.
		\param hittestContext
		\param drawContext
		*/
		DefaultRenderItemAPI virtual void HitTest(HitTestContext& hittestContext, DrawContext& drawContext);

		/** Get number of primitives contained by this item
		\return edge count of the mesh
		*/
		DefaultRenderItemAPI virtual size_t GetPrimitiveCount() const {return mpMesh->getNumFaces();} //Assume each face has 2 edged. This doesn't accurate, but enough for building BSP tree.
	};

	/** This class collects mesh edge render items and build hardware buffers for them.
		It also uses a shader to do hardware edge culling. With this shader, we can do
		efficient edge back-face cull.
	*/
	class MeshEdgeConsolidationItemDX11 : public MaxSDK::Graphics::IConsolidationRenderItem, public MaxSDK::Util::Noncopyable
	{
	protected:
		/** number of triangles that have edge 0 invisible */
		size_t mCull0Count;
		/** number of triangles that have edge 0 and 1 invisible */
		size_t mCull01Count;
		/** number of triangles that all edges are visible*/
		size_t mNoCullingCount;
		/** number of triangle count in total*/
		size_t mPrimitiveCount;
		/** whether the item needs to recalculate mesh size every frame*/
		bool mbFixedSize;
		/** whether enable back-face cull*/
		bool mbBackfaceCull;
		/** dirty flag*/
		bool mbIsDirty;

		struct ConsolidationData : public MaxHeapOperators
		{
			MeshEdgeRenderItem* pSourceItem;
			Matrix3 WorldMatrix;
		};

		MaxSDK::Array<ConsolidationData> mDataArray;
		MaterialRequiredStreams mFormat;
		VertexBufferHandleArray mMeshVB;
		/** index array which contains triangles that all edges are visible*/
		IndexBufferHandle mNoCullingIdx;
		/** index array which contains triangles that only edge 0 is invisible*/
		IndexBufferHandle mCull0Idx;
		/** index array which contains triangles that edge 0 and 1 are invisible*/
		IndexBufferHandle mCull01Idx;
		/** wireframe render state*/
		MaxSDK::Graphics::RasterizerState mRasterState;

		DefaultRenderItemAPI virtual void BuildIndexBuffer();
		DefaultRenderItemAPI virtual void BuildVertexBuffer(DrawContext& drawContext);
	public:
		/** Constructor
		\param bFixedSize whether this item recalculate mesh size frame-by-frame
		\param bBackfaceCull whether this item enables back-face cull
		*/
		DefaultRenderItemAPI MeshEdgeConsolidationItemDX11(bool bFixedSize, bool bBackfaceCull);

		DefaultRenderItemAPI virtual ~MeshEdgeConsolidationItemDX11(){}
		//From ICustomRenderItem
		/** This function creates hardware buffers if any of the items are dirty.
		\param drawContext draw context used by the function
		*/
		DefaultRenderItemAPI virtual void Realize(DrawContext& drawContext);
		
		/** This function renders the mesh edges
		\param drawContext draw context used by the functions
		*/
		DefaultRenderItemAPI virtual void Display(DrawContext& drawContext);

		/** Return number of triangles(not edges) in this item.
		*/
		DefaultRenderItemAPI virtual size_t GetPrimitiveCount() const {return mPrimitiveCount;}
		//From IConsolidationRenderItem

		/** This function clears all line data comes from AddSourceItem()
		*/
		DefaultRenderItemAPI virtual void ClearSourceItems();
		/** This function handle pSourceItem which has same consolidation key.
		\param pSourceItem an instance of MeshRenderItem
		\param dc draw context of the incoming item
		*/
		DefaultRenderItemAPI virtual bool AddSourceItem(ICustomRenderItemPtr& pSourceItem, DrawContext& dc);
	};

	class MeshEdgeConsolidationItemDX9 : public MeshEdgeConsolidationItemDX11
	{
	protected:

		MaxSDK::Graphics::VertexColorMaterialHandle mVertexColorMaterial;
		/** Edge index array*/
		IndexBufferHandle mMeshIB;

		DefaultRenderItemAPI virtual void BuildIndexBuffer(DrawContext& drawContext);

	public:
		/** Constructor
		\param bFixedSize whether this item recalculate mesh size frame-by-frame
		\param bBackfaceCull whether this item enables back-face cull
		*/
		DefaultRenderItemAPI MeshEdgeConsolidationItemDX9(bool bFixedSize, bool bBackfaceCull);
		//From ICustomRenderItem
		/** This function creates hardware buffers if any of the item is dirty.
		\param drawContext draw context used by the function
		*/
		DefaultRenderItemAPI virtual void Realize(DrawContext& drawContext);
		
		/** This function renders the mesh edges
		\param drawContext draw context used by the functions
		*/
		DefaultRenderItemAPI virtual void Display(DrawContext& drawContext);
	};
}}};
