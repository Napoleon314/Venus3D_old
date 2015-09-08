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
#include "../VertexColorMaterialHandle.h"
#include "../VertexBufferHandle.h"
#include "../IndexBufferHandle.h"
#include "../HLSLMaterialHandle.h"
#include "../../containers/Array.h"
#include "../../Point3.h"
#include "../../matrix3.h"
#include "ItemConsts.h"

namespace MaxSDK{namespace Graphics{namespace Utilities	{

	/** Class ID of the spline consolidation item
	*/
	#define CONSOLIDATION_SPLINE_CLASS_ID Class_ID(0x13e7706e, 0x5bfb48bd)

	/** This is a utility class for provide a key for spline consolidation item
	*/
	class SplineItemKey : public MaxSDK::Graphics::IConsolidationKey
	{
		/** hash key which contains class ID */
		size_t mHashKey;
	public:
		DefaultRenderItemAPI SplineItemKey();

		DefaultRenderItemAPI SplineItemKey(SplineItemKey&);

		DefaultRenderItemAPI virtual ~SplineItemKey();

		DefaultRenderItemAPI SplineItemKey& operator=(const SplineItemKey& rhs);

		/** Get hash key of this key
		\return hash key
		*/
		DefaultRenderItemAPI virtual size_t GetHashKey();

		/** Compare to another key
		Since SplineItemKey doesn't contain any values other than hash key, this function simply
		check whether rhs is a SplineItemKey.
		\return true if rhs is a SplineItemKey. false if not.
		*/
		DefaultRenderItemAPI virtual bool Equals(const IConsolidationKey& rhs) const;

		/** Get class id of the spline key
		The key is CONSOLIDATION_SPLINE_CLASS_ID = Class_ID(0x4d5090aa, 0x16e4e23)
		\return CONSOLIDATION_SPLINE_CLASS_ID
		*/
		DefaultRenderItemAPI virtual const Class_ID GetClassID() const;

		/** Return a new instance of SplineItemKey.
		\return new instance of the key
		*/
		DefaultRenderItemAPI virtual MaxSDK::Graphics::IConsolidationKey* Clone();
	};

	/** This class defines a spline item consolidation strategy.
		By default, spline item always support immediate consolidation.
		If user plugins have complex data that can't support immediate consolidation,
		they need to create own strategy with ImmediateConsolidationEnabled returns false.
	*/
	class SplineConsolidationStrategy : public MaxSDK::Graphics::IConsolidationStrategy
	{
	protected:
		SplineConsolidationStrategy() {}
		virtual ~SplineConsolidationStrategy() {}
	public:

		/** Get instance of the strategy. The instance is global object
		\return instance of SplineConsolidationStrategy
		*/
		DefaultRenderItemAPI static MaxSDK::Graphics::IConsolidationStrategy& GetInstance();
		
		/** Create a new spline consolidation item
		\return a new spline consolidation item
		*/
		DefaultRenderItemAPI virtual IConsolidationRenderItem* CreateConsolidationItem(const IConsolidationKey& key);

		/** Returns whether this strategy supports immediate consolidation.
		If user plugins have complex data that can't support immediate consolidation,
		they need to create own strategy with ImmediateConsolidationEnabled returns false.
		\return for this class, always return true.
		*/
		virtual bool ImmediateConsolidationEnabled() {return true;}
	};

	/** Base class for spline render items. This class is also an alternate solution for polyline functions in GraphicsWindow.
		Usually plugins needs to inherit this class and implement Realize() function to generate line data unless the plugin
		has static data that never changes after creation.
		This class is designed for simple line data that can be immediate consolidated. So it actually doesn't need to implement
		Display() fucntion because the display part is handled by SplineConsolidationItem. This item simply store data in 
		system memory arrays, which has fast read performance.

		If plugin's data is too complex and can't be generated in real time, we suggest the plugin to implements both Realize() 
		and Display() functions. Then use a non-immediate consolidation strategy. So that the complex line item won't slow down 
		other line items. A reference complexity of the line item is if it has more than 10K primitives, we suggest to disable 
		immediate consolidation.
	*/
	class SplineRenderItem : public MaxSDK::Graphics::ICustomRenderItem, public MaxSDK::Util::Noncopyable
	{
	protected:
		size_t mPrimitiveCount;
		MaxSDK::Array<Point3> mLinePositionArray;
		MaxSDK::Array<DWORD> mLineColorArray;
		MaxSDK::Array<int> mLineIndexArray;

		MaxSDK::Array<int> mHitTestIndexArray;
		MaxSDK::Graphics::VertexBufferHandleArray mLineVB;
		MaxSDK::Graphics::IndexBufferHandle mLineIB;
		MaxSDK::Graphics::IndexBufferHandle mHitTestIB;
		
		DWORD mdwDirtyFlags;
	public:
		DefaultRenderItemAPI SplineRenderItem();

		DefaultRenderItemAPI virtual ~SplineRenderItem();

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
		Since the splines are always rendered by consolidation item, in the render item's hit test function
		we need to build separate data to draw the item.
		\param hittestContext context which contains hit test related information
		\param drawContext context which contains rendering related information
		*/
		DefaultRenderItemAPI virtual void HitTest(HitTestContext& hittestContext, DrawContext& drawContext);

		/** Get primitive count of this item
		\return primitive count of this item
		*/
		DefaultRenderItemAPI virtual size_t GetPrimitiveCount() const;

		/** Return whether this item contains line data
		Empty item will be skipped when consolidation
		\return true if there's line data. Otherwise false.
		*/
		DefaultRenderItemAPI bool HasLineData() {return mLineIndexArray.length() > 0;}

		/** Add line data by array
			The position array and color array must has same length.
			The index array must contain zero-based line list data.
			This function automatically increase the index based on existing vertex counts.
		\param positionArray array contains line position data
		\param colorArray array contains DWORD color data. Can be generated by Color.toRGB(). 
		\param indexArray array contains zero-based line list data
		\param isHitTestPrimitive indicate whether these primitives are also used for hit test.
		*/
		DefaultRenderItemAPI void AddLineList(
			MaxSDK::Array<Point3>& positionArray, 
			MaxSDK::Array<DWORD>& colorArray, 
			MaxSDK::Array<int>& indexArray, 
			bool isHitTestPrimitive);
	
		/** Add line strip data by raw pointers with custom vertex color.
			Internally the function converts line strip to line list. And build index data for the strip.
		\param positionArray pointers of position data
		\param colorArray pointers of color data. The function will automatically handle color shift in DX11 mode.
		\param count number of positions to add.
		\param closed indicate whether this line strip is closed. The function will build index for closed strip.
		\param isHitTestPrimitive indicate whether these primitives are also used for hit test.
		*/
		DefaultRenderItemAPI void AddLineStrip(
			const Point3* positionArray, 
			const Color* colorArray,
			size_t count, 
			bool closed, 
			bool isHitTestPrimitive);

		/** Add line strip data by raw pointers, with same color for all vertices.
			Internally the function converts line strip to line list. And build index data for the strip.
		\param positionArray pointers of position data
		\param color reference of color data. The function will automatically handle color shift in DX11 mode.
		\param count number of positions to add.
		\param closed indicate whether this line strip is closed. The function will build index for closed strip.
		\param isHitTestPrimitive indicate whether these primitives are also used for hit test.
		*/
		DefaultRenderItemAPI void AddLineStrip(
			const Point3* positionArray, 
			const Color& color, 
			size_t count, 
			bool closed, 
			bool isHitTestPrimitive);

		/** Clear line data
		*/
		DefaultRenderItemAPI void ClearLines();

		/** Get position array
		\return position array
		*/
		DefaultRenderItemAPI MaxSDK::Array<Point3>& GetLinePositionArray() {return mLinePositionArray;}

		/** Get color array
		\return color array
		*/
		DefaultRenderItemAPI MaxSDK::Array<DWORD>& GetLineColorArray() {return mLineColorArray;}

		/** Get line list index array
		\return line list index array
		*/
		DefaultRenderItemAPI MaxSDK::Array<int>& GetLineIndexArray() {return mLineIndexArray;}
	};


	/** This class collects all spline items(both immediate and non-immediate), hardware build vertex buffer
	and index buffer in Realize() function and render them in Display() function.
	With this class, we can efficiently batch small line data and greatly reduce rendering API overheads.
	Ideally whole scene's line objects can be rendered by a few consolidation items.
	*/
	class SplineConsolidationItem : public MaxSDK::Graphics::IConsolidationRenderItem, public MaxSDK::Util::Noncopyable
	{
	protected:
		/** Number of primitives */
		int mPrimitiveCount;
		/** Dirty flag */
		bool mbIsDirty;

		/** Data structure used by the item
		*/
		struct ConsolidationData  : public MaxHeapOperators
		{
			SplineRenderItem* pSourceItem;
			Matrix3 WorldMatrix;
		};

		/** Array of ConsolidationData*/
		MaxSDK::Array<ConsolidationData> mLineDataArray;
		/** Hardware vertex buffer handle*/
		VertexBufferHandleArray mLineVB;
		/** Hardware index buffer handle*/
		IndexBufferHandle mLineIB;

	public:
		DefaultRenderItemAPI SplineConsolidationItem();
		//From ICustomRenderItem

		/** This function first check dirty flags of its own and sub line items.
		If anything changes, it creates and fills hardware vertex/index buffer form  
		all of internal line item data.
		\param drawContext draw context used by the function
		*/
		DefaultRenderItemAPI virtual void Realize(DrawContext& drawContext);

		/** This function draw lines by vertex color material
		\param drawContext draw context used by the function
		*/
		DefaultRenderItemAPI virtual void Display(DrawContext& drawContext);

		/** This function returns number of lines
		\return number of lines.
		*/
		DefaultRenderItemAPI virtual size_t GetPrimitiveCount() const {return mPrimitiveCount;}

		//From IConsolidationRenderItem

		/** This function clears all line data comes from AddSourceItem()
		*/
		DefaultRenderItemAPI virtual void ClearSourceItems();

		/** This function handle pSourceItem which has same consolidation key.
		\param pSourceItem an instance of SplineRenderItem
		\param drawContext draw context of the incoming item
		*/
		DefaultRenderItemAPI virtual bool AddSourceItem(ICustomRenderItemPtr& pSourceItem, DrawContext& drawContext);
	};
}}};
