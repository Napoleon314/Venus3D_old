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

// 2013/4/22, Qinming.Ren Created.

#pragma once
#include "../ICustomRenderItem.h"
#include "../IConsolidationKey.h"
#include "../IConsolidationRenderItem.h"
#include "../IConsolidationStrategy.h"
#include "../VertexColorMaterialHandle.h"
#include "../VertexBufferHandle.h"
#include "../IndexBufferHandle.h"
#include "../HLSLMaterialHandle.h"
#include "../RenderStates.h"
#include "../GraphicsExport.h"
#include "../../Tab.h"
#include "../../Point3.h"
#include "../../matrix3.h"

namespace MaxSDK{namespace Graphics{namespace Utilities	{

	/** Class ID of the Marker consolidation item
	*/
	#define CONSOLIDATION_MARKER_CLASS_ID Class_ID(0x70dc9cd9, 0x886d46dd)

	/** This is a utility class for provide a key for marker consolidation item
	*/
	class MarkerItemKey : public MaxSDK::Graphics::IConsolidationKey
	{
		/** hash key which contains class ID */
		size_t mHashKey;
	public:
		DefaultRenderItemAPI MarkerItemKey();

		DefaultRenderItemAPI MarkerItemKey(MarkerItemKey&);

		DefaultRenderItemAPI virtual ~MarkerItemKey();

		DefaultRenderItemAPI MarkerItemKey& operator=(const MarkerItemKey& rhs);

		/** Get hash key of this key
		\return hash key
		*/
		DefaultRenderItemAPI virtual size_t GetHashKey();

		/** Compare to another key
		Since MarkerItemKey doesn't contain any values other than hash key, this function simply
		check whether rhs is a MarkerItemKey.
		\return true if rhs is a MarkerItemKey. false if not.
		*/
		DefaultRenderItemAPI virtual bool Equals(const IConsolidationKey& rhs) const;

		/** Get class id of the marker key
		The key is CONSOLIDATION_MARKER_CLASS_ID = Class_ID(0x70dc9cd9, 0x886d46dd)
		\return CONSOLIDATION_MARKER_CLASS_ID
		*/
		DefaultRenderItemAPI virtual const Class_ID GetClassID() const;

		/** Return a new instance of MarkerItemKey.
		\return new instance of the key
		*/
		DefaultRenderItemAPI virtual MaxSDK::Graphics::IConsolidationKey* Clone();
	};

	/** This class defines a marker item consolidation strategy.
		By default, marker item always support immediate consolidation.
		If user plugins have complex data that can't support immediate consolidation,
		they need to create own strategy with ImmediateConsolidationEnabled returns false.
	*/
	class MarkerConsolidationStrategy : public MaxSDK::Graphics::IConsolidationStrategy
	{
		MarkerConsolidationStrategy() {}
		virtual ~MarkerConsolidationStrategy() {}
	public:
		/** Get instance of the strategy. The instance is global object
		\return instance of MarkerConsolidationStrategy
		*/
		DefaultRenderItemAPI static MaxSDK::Graphics::IConsolidationStrategy& GetInstance();
		
		/** Create a new marker consolidation item
		\return a new marker consolidation item
		*/
		DefaultRenderItemAPI virtual IConsolidationRenderItem* CreateConsolidationItem(const IConsolidationKey& key);

		/** Returns whether this strategy supports immediate consolidation.
		If user plugins have complex data that can't support immediate consolidation,
		they need to create own strategy with ImmediateConsolidationEnabled returns false.
		\return for this class, always return true.
		*/
		virtual bool ImmediateConsolidationEnabled() {return true;}
	};

	/** Base class for marker render items. This class is also an alternate solution for marker functions in GraphicsWindow.
		Usually plugins needs to inherit this class and implement Realize() function to generate marker data unless the plugin
		has static data that never changes after creation.
		This class is designed for simple marker data that can be immediate consolidated. So it actually doesn't need to implement
		Display() fucntion because the display part is handled by MarkerConsolidationItem. This item simply store data in 
		system memory arrays, which has fast read performance.

		If plugin's data like particle system is too complex and can't be generated in real time, we suggest the plugin to 
		implements both Realize() and Display() functions. Then use a non-immediate consolidation strategy. So that the complex 
		marker item won't slow down other marker items. A reference complexity of the marker item is if it has more than 1K markers, we 
		suggest to disable immediate consolidation.
	*/
	class MarkerRenderItem : public MaxSDK::Graphics::ICustomRenderItem, public MaxSDK::Util::Noncopyable
	{
	protected:
		MaxSDK::Array<Point3> mPositionArray;
		MaxSDK::Array<DWORD> mColorArray;
		VertexBufferHandleArray mMarkerVB;
		bool mbIsDirty;
	public:
		DefaultRenderItemAPI MarkerRenderItem();

		DefaultRenderItemAPI virtual ~MarkerRenderItem();

		/** Return dirty status of this item
		\return true if dirty, false if not
		*/
		DefaultRenderItemAPI bool DirtyFlag(){return mbIsDirty;}
		/** Sets dirty status of this item
		*/
		DefaultRenderItemAPI void ClearDirty() {mbIsDirty = false;}

		/** Default realize function.
		Plugins should implement Realize() function to handle object changes.
		In this function, plugins need to fill position, color and index buffers, and
		set mbIsDirty to true to notify consolidation item.
		\param drawContext
		*/
		DefaultRenderItemAPI virtual void Realize(DrawContext& drawContext);

		/** Default display function
		This class is used for immediate consolidation. So it doesn't need display function.
		\param drawContext
		*/
		DefaultRenderItemAPI virtual void Display(DrawContext& drawContext);

		/** Hit Test function for this render item
		Since the markers are always rendered by consolidation item, in the render item's hit test function
		we need to build separate data to draw the item.
		\param hittestContext
		\param drawContext
		*/
		DefaultRenderItemAPI virtual void HitTest(HitTestContext& hittestContext, DrawContext& drawContext);

		/** Get primitive count of this item
		\return primitive count of this item
		*/
		DefaultRenderItemAPI virtual size_t GetPrimitiveCount() const {return mPositionArray.lengthUsed();}

		/** Return whether this item contains marker data
		Empty item will be skipped when consolidation
		\return true if there's marker data. Otherwise false.
		*/
		DefaultRenderItemAPI bool HasData() {return mPositionArray.length() > 0;}

		/** Add marker data by array
			The position array and color array must has same length.
		\param positionArray array contains marker position data
		\param colorArray array contains DWORD color data. Can be generated by Color.toRGB(). 
		\param markerTypeArray array of MarkerType.
		*/
		DefaultRenderItemAPI void AddMarkers(MaxSDK::Array<Point3>& positionArray, MaxSDK::Array<DWORD>& colorArray, MaxSDK::Array<int>& markerTypeArray);

		/** Add marker data by raw pointers with custom color.
		\param positionArray pointers of position data
		\param colorArray pointers of color data.
		\param type Marker type
		\param count number of positions to add.
		*/
		DefaultRenderItemAPI void AddMarkers(const Point3* positionArray, const Color* colorArray, const MarkerType type, size_t count);

		/** Add marker data by raw pointers, with same color for all markers.
		\param positionArray pointers of position data
		\param color reference of color data.
		\param type Marker type
		\param count number of positions to add.
		*/
		DefaultRenderItemAPI void AddMarkers(const Point3* positionArray, const Color& color, const MarkerType type, size_t count);

		/** Clear marker data
		*/
		DefaultRenderItemAPI void ClearMarkers();

		/** Get position array
		\return position array
		*/
		DefaultRenderItemAPI MaxSDK::Array<Point3>& GetPositionArray() {return mPositionArray;}

		/** Get color array
		\return color array
		*/
		DefaultRenderItemAPI MaxSDK::Array<DWORD>& GetColorArray() {return mColorArray;}
	};


	/** This class collects all marker items(both immediate and non-immediate), hardware build vertex buffer
	and index buffer in Realize() function and render them in Display() function.
	With this class, we can efficiently batch markers data and greatly reduce rendering API overheads.
	Ideally whole scene's marker objects can be rendered by a few consolidation items.
	*/
	class MarkerConsolidationItem : public MaxSDK::Graphics::IConsolidationRenderItem, public MaxSDK::Util::Noncopyable
	{
	protected:
		/** Number of primitives */
		int mPrimitiveCount;
		/** Dirty flag */
		bool mbIsDirty;

		/** Data structure used by the item
		*/
		struct ConsolidationData : public MaxHeapOperators
		{
			MarkerRenderItem* pSourceItem;
			Matrix3 WorldMatrix;
		};

		/** Array of ConsolidationData*/
		MaxSDK::Array<ConsolidationData> mMarkerDataArray;

		/** Marker data vertex buffer*/
		VertexBufferHandleArray mMarkerVB;

	public:
		MarkerConsolidationItem();
		//From ICustomRenderItem

		/** This function first check dirty flags of its own and sub items.
		If anything changes, it creates and fills hardware vertex buffer form  
		all of internal item data.
		\param drawContext draw context used by the function
		*/
		virtual void Realize(DrawContext& drawContext);

		/** This function draw markers by marker shader
		\param drawContext draw context used by the function
		*/
		virtual void Display(DrawContext& drawContext);

		/** This function returns number of markers
		\return number of markers.
		*/
		virtual size_t GetPrimitiveCount() const {return mPrimitiveCount;}

		//From IConsolidationRenderItem

		/** This function clears all marker data comes from AddSourceItem()
		*/
		virtual void ClearSourceItems();

		/** This function handle pSourceItem which has same consolidation key.
		\param pSourceItem an instance of MarkerRenderItem
		\param drawContext draw context of the incoming item
		*/
		virtual bool AddSourceItem(ICustomRenderItemPtr& pSourceItem, DrawContext& drawContext);
	};
}}};
