//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../GraphicsTypes.h"
#include "../baseinterface.h"
#include "../MaxHeap.h"
#include "../Point4.h"

#include "../Graphics/RenderEnums.h"


namespace MaxSDK { namespace Graphics {

	/**	This class encapsulates a complete rasterizer state, like fill mode, 
		cull mode, point size, etc.
	*/
	class GraphicsDriverAPI RasterizerState: public MaxHeapOperators
	{
	public:
		RasterizerState();
		virtual ~RasterizerState() {}
		
		/** Set the raster fill mode. Default value is FillSolid
		/param value new mode
		*/
		virtual void SetFillMode(FillMode value);
		/** Get the raster fill mode
		/return the fill mode
		*/
		virtual FillMode GetFillMode() const;
		/** Set culling mode, front face or back face. Default value is CullModeCCW
		/param value new cull mode
		*/
		virtual void SetCullMode(CullMode value);
		/** Get culling mode
		/return the culling mode
		*/
		virtual CullMode GetCullMode() const;

		/**	Sets whether color registers are interpolated between vertices.
			If disabled, this will provide flat shading for effects that use the color registers;
			otherwise Gouraud (smooth) shading is performed. Default value is true.
		/param value true to enable color interpolation, false to disable
		*/
		virtual void SetColorInterpolationEnable(bool value);
		/**	Gets whether color registers are interpolated between vertices.
		/return true if the color interpolation is enabled, false if disabled.
		*/
		virtual bool GetColorInterpolationEnable() const;

		/** Set z buffer depth bias value by float
		/param value float depth bias
		*/
		virtual void SetDepthBias(float value);
		/** Set z buffer depth bias value by int
		/param value int depth bias
		*/
		virtual void SetDepthBias(int value);

		/** Get depth bias is float or int
		/return true if using float depth bias, false if it's int.
		*/
		virtual bool IsFloatDepthBias();
		/** Get float depth bias
		/return depth bias
		*/
		virtual float GetDepthBiasFloat() const;
		/** Get int depth bias
		/return depth bias
		*/
		virtual int GetDepthBiasInt() const;

		/** Set the z buffer depth bias clamp value. Default value is 0
		/param value depth bias clamp
		*/
		virtual void SetDepthBiasClamp(float value);

		/** get the depth bias clamp value
		/return depth bias clamp
		*/
		virtual float GetDepthBiasClamp() const;

		/** Set the slope scaled depth bias value. Default value is 0
		/param value scaled depth bias value
		*/
		virtual void SetSlopeScaledDepthBias(float value);

		/** Get the slope scaled depth bias value
		/return slope scaled depth bias value
		*/
		virtual float GetSlopeScaledDepthBias() const;

		/** Set if depth clipping is enabled. Default value is true.
			If enabled, depth values out of the 0-1 range are clipped. If false,
			the GPU will first clamp the depth value to 0-1, then clip any values
			where the w < 0. This is mostly used by shadow volume techniques to avoid
			near and far clipping problems. It could also be used by a renderer where
			one object is behind everything (e.g. an environment hemisphere) that would
			normally be clipped by the far clip plane; turning off clipping would instead
			clamp the z-depth of this object to 1.0 and so display it behind everything.
			In summary, this value should very rarely be set to false.
		/param value true to enable depth clipe, false to disable.
		*/
		virtual void SetDepthClipEnabled(bool value);
		
		/** Get if depth clipping is enabled
		/return true if enabled, false if not.
		*/
		virtual bool GetDepthClipEnabled() const;

		/** Set scissor clipping state. Default value is false
		/param value true to enable scissor clipping, false to disable
		*/
		virtual void SetScissorEnabled(bool value);

		/** Get scissor clipping state.
		/return true if scissor is enabled, false if not.
		*/
		virtual bool GetScissorEnabled() const;

		/** MultiSample state. Default value is false
		/param value true to enable multi-sample, false to disable
		*/
		virtual void SetMultiSampleEnabled(bool value);
		
		/** Get MultiSample State
		/return true if multi-sample enabled, false if not.
		*/
		virtual bool GetMultiSampleEnabled() const;
		/** Set vertex point size. Default value is 1.0
			Note this property only works for DirectX9 device
		/param value the point size.
		*/
		virtual void SetPointSize(float value);

		/** Get vertex point size. 
		/return vertex point size
		*/
		virtual float GetPointSize() const;

	protected:
		/** The raster fill mode.
		Default value is FillSolid
		*/
		FillMode mFillMode;

		/** The culling mode, front face or back face.
		Default value is CullModeCCW
		*/
		CullMode mCullMode;

		/**	Sets whether color registers are interpolated between vertices.
			If disabled, this will provide flat shading for effects that use the color registers;
			otherwise Gouraud (smooth) shading is performed. Default value is true.
		*/
		bool mColorInterpolationEnabled;

		/** The z buffer depth bias type: float or int.
		Default value is false
		*/
		bool mIsFloatDepthBias;

		/** the z buffer depth bias clamp value.
		Default value is 0
		*/
		float mDepthBiasClamp;

		/** the slope scaled depth bias value.
		Default value is 0
		*/
		float mSlopeScaledDepthBias;

		/** Indicate if depth clipping is enabled.
			If enabled, depth values out of the 0-1 range are clipped. If false,
			the GPU will first clamp the depth value to 0-1, then clip any values
			where the w < 0. This is mostly used by shadow volume techniques to avoid
			near and far clipping problems. It could also be used by a renderer where
			one object is behind everything (e.g. an environment hemisphere) that would
			normally be clipped by the far clip plane; turning off clipping would instead
			clamp the z-depth of this object to 1.0 and so display it behind everything.
			In summary, this value should very rarely be set to false.
		*/
		bool mDepthClipEnabled;

		/** scissor clipping state
		Default value is false
		*/
		bool mScissorEnabled;
		/** Multisample state
		Default value is false
		*/
		bool mMultiSampleEnabled;
		/** Vertex point size
			Note this property only works for DirectX9 device
			Default value is 1.0
		*/
		float mPointSize;

		/** The z buffer depth bias value
		Default value is 0
		*/
		union DepthBias
		{
			float fVal;
			int   iVal;
		};
		DepthBias mDepthBias;
	};

	/** This class holds the blend state for single render target.
	*/
	class GraphicsDriverAPI TargetBlendState : public MaxHeapOperators
	{
		friend class BlendState;
	protected:		
		TargetBlendState();
		virtual ~TargetBlendState() {}
	public:
		/** Set the blending is enabled or not. Default value is false.
		/param value true to enable blending, false to disable
		*/
		virtual void SetBlendEnabled(bool value);

		/** Get the blending is enabled or not. 
		/return true if enabled, false if not.
		*/
		virtual bool GetBlendEnabled() const;
		
		/** Set the blend factor for source color. Default value is BlendSelectorOne.
		/param value source blend factor
		*/
		virtual void SetSourceBlend(BlendSelector value);

		/** Set the blend factor for source color.
		/return current source blend factor
		*/
		virtual BlendSelector GetSourceBlend() const;

		/** Set the blend factor for destination color. Default value is BlendSelectorZero.
		/param value destination blend factor
		*/
		virtual void SetDestinationBlend(BlendSelector value);
		
		/** Get the blend factor for destination color
		/return current destination blend factor
		*/
		virtual BlendSelector GetDestinationBlend() const;

		/** Set the operation for color blending. Default value is BlendOperationAdd.
		/param value the operation for color blending
		*/
		virtual void SetColorBlendOperation(BlendOperation value);

		/** Get the operation for color blending.
		/return the operation for color blending.
		*/
		virtual BlendOperation GetColorBlendOperation() const;
		
		/** Set the blend factor for source alpha. Default value is BlendSelectorOne.
		/param value the blend factor for source alpha
		*/
		virtual void SetAlphaSourceBlend(BlendSelector value);

		/** Get the blend factor for source alpha. 
		/return the blend factor for source alpha
		*/
		virtual BlendSelector GetAlphaSourceBlend() const;

		/** Set the blend factor for destination alpha. Default value is BlendSelectorZero.
		/param value blend factor for destination alpha
		*/
		virtual void SetAlphaDestinationBlend(BlendSelector value);

		/** Get the blend factor for destination alpha.
		/return the blend factor for destination alpha.
		*/
		virtual BlendSelector GetAlphaDestinationBlend() const;

		/** Set the operation for alpha blending. Default value is BlendOperationAdd.
		/param value the operation for alpha blending
		*/
		virtual void SetAlphaBlendOperation(BlendOperation value);

		/** Get the operation for alpha blending.
		/return the operation for alpha blending.
		*/
		virtual BlendOperation GetAlphaBlendOperation() const;

		/** Indicates what color components(red, green, blue, alpha) are writable.
			Default value is ColorWriteEnableRedGreenBlueAlpha.
		/param mask the write mask
		*/
		virtual void SetRenderTargetWriteMask(ColorWriteEnable mask);
		/** Get color write mask
		/return current mask
		*/
		virtual ColorWriteEnable GetRenderTargetWriteMask() const;
	protected:
		/** The blending is enabled or not.
			By default it is false.
		*/
		bool                mBlendEnabled;
		
		/** The blend factor for source color.
			By default it is BlendSelectorOne.
		*/
		BlendSelector       mSourceBlend;
		
		/** The blend factor for destination color.
			By default it is BlendSelectorZero.
		*/
		BlendSelector       mDestinationBlend;

		/** The operation for color blending.
			By default it is BlendOperationAdd.
		*/
		BlendOperation     mColorBlendOperation;
		
		/** The blend factor for source alpha.
			By default it is BlendSelectorOne.
		*/
		BlendSelector      mAlphaSourceBlend;

		/** The blend factor for destination alpha.
			By default it is BlendSelectorZero.		
		*/
		BlendSelector      mAlphaDestinationBlend;
		
		/** The operation for alpha blending.
			By default it is BlendOperationAdd.
		*/
		BlendOperation     mAlphaBlendOperation;

		
		/** Indicates what color components(red, green, blue, alpha) are writable.
			By default it is ColorWriteEnableRedGreenBlueAlpha.
		*/
		ColorWriteEnable   mRenderTargetWriteMask;	
	};

	#define MAX_TARGETS_SLOT 8
	/** This class holds the blend state for all render targets.
	*/
	class GraphicsDriverAPI BlendState : public MaxHeapOperators
	{
	public:
		BlendState();
		virtual ~BlendState() {}
		/** Set the AlphaToCoverage state. Default value is false
			Note: "AlphaToCoverage" is only valid for multi-sampling.
		/param value true to enable alpha-to-coverage, false to disable.
		*/
		virtual void SetAlphaToCoverageEnabled(bool value);

		/** Get the AlphaToCoverage state.
		return true if alpha-to-coverage is enabled, false if not
		*/
		virtual bool GetAlphaToCoverageEnabled() const;

		/** Set independent blend state
			If independent blend is enabled, each target has its own blend state;
			Otherwise all targets share one blend state.
			Default value is false
		/param value true to enable independent blend, false to disable
		*/
		virtual void SetIndependentBlendEnabled(bool value);

		/** Get independent blend state
		/return true if independent blend is enabled, false if not.
		*/
		virtual bool GetIndependentBlendEnabled() const;

		/** Set the color blending factor. Default value is Point4(1.0f, 1.0f , 1.0f, 1.0f);
		/param factor color blending factor
		*/
		virtual void SetBlendFactor(Point4& factor);

		/** Get the color blending factor.
		/return the factor
		*/
		virtual Point4 GetBlendFactor() const;

		/** Set multi-sample mask. Default value is 0xffffffff
		/param value the mask
		*/
		virtual void SetMultiSampleMask(unsigned int mask);

		/** Get multi-sample mask
		/return the mask
		*/
		virtual unsigned int GetMultiSampleMask() const;

		/** Get individual target blend state.
		/param index index of the target. Max value is MAX_TARGETS_SLOT.
		/return target blend state handle
		*/
		virtual TargetBlendState& GetTargetBlendState(size_t index);

	protected:
		/** The AlphaToCoverage state
			Note: "AlphaToCoverage" is only valid for multi-sampling.
			Default value is false
		*/
		bool mAlphaToCoverageEnabled;
		
		/** Independent blend state
			If independent blend is enabled, each target has its own blend state;
			Otherwise all targets share one blend state.
			Default value is false
		*/
		bool mIndependentBlendEnabled;

		/** The color blending factor.
		*/
		Point4 mBlendFactor;

		/** Multisample mask
		Default value is 0xffffffff
		*/
		unsigned int mMultiSampleMask;
		TargetBlendState mBlendStates[ MAX_TARGETS_SLOT ];
	};

		/** This class encapsulates a complete set of stencil buffer operations for stencil test.
	*/
	class GraphicsDriverAPI StencilOperation : public MaxHeapOperators
	{
		friend class DepthStencilState;
	protected:
		StencilOperation();
		virtual ~StencilOperation() {}
	public:
		/** Set stencil operation when stencil test fails.
			By default, it is StencilOperationKeep.
		/param op operation when stencil test fails.
		*/
		virtual void SetStencilFailOperation(StencilOperationType op);

		/** Get stencil fail operation
		/return stencil fail operation
		*/
		virtual StencilOperationType GetStencilFailOperation() const;

		/** Set stencil operation when stencil test passes and depth test fails.
			By default, it is StencilOperationKeep.
		/param op operation when depth fail
		*/
		virtual void SetStencilDepthFailOperation(StencilOperationType op);

		/** Get stencil operation when stencil test passes and depth test fails.
		/return depth fail operation
		*/
		virtual StencilOperationType GetStencilDepthFailOperation() const;
		
		/** Set stencil operation when stencil test and depth test all pass.
			By default, it is StencilOperationKeep.
		/param op operation when stencil pass
		*/
		virtual void SetStencilPassOperation(StencilOperationType op);

		/** Get stencil operation when stencil test and depth test all pass.
		/return stencil and depth pass operation
		*/
		virtual StencilOperationType GetStencilPassOperation() const;

		/** Set compare function for stencil test.
			By default, it is CompareFunctionAlways.
		/param func compare function for stencil test.
		*/
		virtual void SetStencilFunction(CompareFunction func);

		/** Get compare function for stencil test.
		/return compare function for stencil test.
		*/
		virtual CompareFunction GetStencilFunction() const;

	protected:
		/** Stencil operation when stencil test fails.
			By default, it is StencilOperationKeep.
		*/
		StencilOperationType mStencilFailOp;
		
		/** Stencil operation when stencil test passes and depth test fails.
			By default, it is StencilOperationKeep.
		*/
		StencilOperationType mStencilDepthFailOp;
		
		/** Stencil operation when stencil test and depth test all pass.
			By default, it is StencilOperationKeep.
		*/
		StencilOperationType mStencilPassOp;

		/** Compare function for stencil test.
			By default, it is CompareFunctionAlways.
		*/
		CompareFunction mStencilFunc;
	};

	/** This class encapsulates a complete depth buffer and stencil buffer state.
	*/
	class GraphicsDriverAPI DepthStencilState : public MaxHeapOperators
	{
	public:
		DepthStencilState();
		virtual ~DepthStencilState() {}

		/** Set depth test state. Default value is true
		/param value true to enable depth test, false to disable
		*/
		virtual void SetDepthEnabled(bool value);
		/** Get depth test state.
		/return true if enabled, false if not.
		*/
		virtual bool GetDepthEnabled() const;

		/** Set whether depth write is allowed. Default value is true
		/param value true to allow depth write, false to turn off
		*/
		virtual void SetDepthWriteEnabled(bool value);

		/** Get whether depth write is allowed. Default value is true
		/return true if depth write is enabled, false if not
		*/
		virtual bool GetDepthWriteEnabled() const;

		/** Set depth buffer compare function. Default value is CompareFunctionLess
		/param value depth compare function
		*/
		virtual void SetDepthFunction(CompareFunction value);

		/** Get depth buffer compare function.
		/return current depth compare function
		*/
		virtual CompareFunction GetDepthFunction() const;

		/** Set stencil buffer state. Default value is false
		/param value true to enable stencil buffer, false to disable
		*/
		virtual void SetStencilEnabled(bool value);
		/** Get stencil buffer state
		/return stencil buffer state
		*/
		virtual bool GetStencilEnabled() const;

		/** Set stencil buffer read mask. Default value is 0xFF
		/param value stencil read mask
		*/
		virtual void SetStencilReadMask(unsigned char mask);

		/** Get stencil buffer read mask. 
		/return current stencil read mask
		*/
		virtual unsigned char GetStencilReadMask() const;
		
		/** Set stencil buffer write mask. Default value is 0xFF
		/param value stencil write mask
		*/
		virtual void SetStencilWriteMask(unsigned char mask);

		/** Get stencil buffer write mask. 
		/return current stencil write mask
		*/
		virtual unsigned char GetStencilWriteMask() const;
		
		/** Get front face stencil operation 
		/return front face stencil operation
		*/
		virtual StencilOperation& GetFrontFaceOperation();

		/** Get front back stencil operation 
		/return front back stencil operation
		*/
		virtual StencilOperation& GetBackFaceOperation();

		/** Set stencil reference value. Default value is 0.
		/param value stencil reference value
		*/
		virtual void SetStencilReference(int value);

		/** Get stencil reference value.
		/return stencil reference value
		*/
		virtual int GetStencilReference() const;
	protected:
		/** Depth test state
		Default value is true
		*/
		bool mDepthEnabled;

		/** Indicates whether depth write is allowed
		Default value is true
		*/
		bool mDepthWriteEnabled;

		/** Depth buffer compare function.
		Default value is CompareFunctionLess
		*/
		CompareFunction mDepthFunction;

		/** Stencil buffer state
		Default value is false
		*/
		bool mStencilEnable;

		/** Stencil buffer read mask
		Default value is 0xFF
		*/
		unsigned char mStencilReadMask;

		/** Stencil buffer write mask
		Default value is 0xFF
		*/
		unsigned char mStencilWriteMask;

		/** Front face stencil operation
		*/
		StencilOperation mFrontFaceOp;

		/** Back face stencil operation
		*/
		StencilOperation mBackFaceOp;

		/** Stencil reference value
		Default value is 0
		*/
		int mReferenceStencil;
	};

	
} }
