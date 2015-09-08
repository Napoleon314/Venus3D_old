//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once
#include "Types.h"
#include "RenderEnums.h"
#include "Matrix44.h"
#include "MaterialRequiredStreams.h"
#include "../GraphicsTypes.h"

class INode;
class ViewExp;

namespace MaxSDK { namespace Graphics {
	/**	Typically ICustomRenderItem::Draw() and ICustomRenderItem::HitTest()
		bring instance of this class as parameter. It contains the complete 
		state of a single item being drawn all system supplied variables are 
		contained here, for example, current virtual device and matrices.
		Developers may need these information to determine Display and HitTest
		code paths.
	*/
	class DrawContext : public MaxHeapOperators
	{
	protected:
		virtual ~DrawContext(){}
	public:
		/** Get current world matrix 
		Note: If you want change the world matrix, you must first copy it to 
		another instance, then apply the world matrix via SetWorldMatrix().
		/return current world matrix.
		*/
		virtual const Matrix44& GetWorldMatrix() const = 0;

		/** Set current world matrix
		Note: If you want change the world matrix, you must first copy it to 
		another instance, then apply the world matrix via SetWorldMatrix().
		/param matrix new world matrix.
		*/
		virtual void SetWorldMatrix(const Matrix44& matrix) = 0;
		
		/** Set current world matrix
		/param matrix new world matrix.
		*/
		virtual void SetWorldMatrix(const Matrix3& matrix) = 0;

		/** Get current view matrix 
		Note: If you want change the view matrix, you must first copy it to 
		another instance, then apply the view matrix via SetViewMatrix().
		/return current view matrix.
		*/
		virtual const Matrix44& GetViewMatrix() const = 0;

		/** Set current world matrix
		Note: If you want change the world matrix, you must first copy it to 
		another instance, then apply the world matrix via SetWorldMatrix().
		/param matrix new world matrix.
		*/
		virtual void SetViewMatrix(const Matrix44& matrix) = 0;

		/** Get current projection matrix 
		Note: If you want change the projection matrix, you must first copy it to 
		another instance, then apply the projection matrix via SetProjectionatrix().
		/return current projection matrix.
		*/
		virtual const Matrix44& GetProjectionMatrix() const = 0;

		/** Set current projection matrix
		Note: If you want change the projection matrix, you must first copy it to 
		another instance, then apply the projection matrix via SetProjectionatrix().
		/param matrix new projection matrix.
		*/
		virtual void SetProjectionMatrix(const Matrix44& matrix) = 0;

		/** Get virtual device
		Note: Don't store the virtual device on client side. You should always use the
		virtual device in the pipeline context. Because in DX11 multi-thread environment,
		the device may different in each call.
		/return virtual device.
		*/
		virtual IVirtualDevice& GetVirtualDevice() = 0;

		/** Get current INode object
		/return INode object
		*/
		virtual INode* GetCurrentNode() = 0;

		/** Get node's extended display mode
		/return extended display mode flag from node
		*/
		virtual int GetExtendedDisplayMode() const = 0;

		/** Get current ViewExp
		Note: Legacy GraphicsWindow object also available in the ViewExp. But we recommend
		to use new VirtualDevice object to do rendering task.
		/return ViewExp object
		*/
		virtual const ViewExp* GetViewExp() const = 0;

		/** Get current material handle
		/return current material handle
		Note: the handle might be invalid. Which means current item don't have material.
		*/
		virtual const MaxSDK::Graphics::BaseMaterialHandle& GetMaterial() const = 0;

		/** Get current frame stamp
		return current frame stamp
		*/
		virtual int GetFrameStamp() = 0;

		/** Get current animation time
		return current animation time
		*/
		virtual TimeValue GetTime() const = 0;
		
		/** Get the alpha test enabled flag. The default value is false (disabled).
		/return current alpha test flag. True means enabled, false means disabled.
		*/
		virtual bool GetAlphaTestEnabled() const = 0;

		/** Set the alpha test enabled flag. The default value is false (disabled).
		/param value new alpha test flag. True means enabled, false means disabled.
		*/
		virtual void SetAlphaTestEnabled(bool value) = 0;

		/** Gets the alpha test comparison function. The default value is CompareFunctionAlways.
		/return current alpha test comparison function.
		*/
		virtual CompareFunction GetAlphaTestFunction() = 0;

		/** Sets the alpha test comparison function. The default value is CompareFunctionAlways.
		/param value the new alpha test comparison function.
		*/
		virtual void SetAlphaTestFunction(CompareFunction value) = 0;

		
		/** Gets the alpha test reference value. The default value is 0.0f.
		/return alpha test reference value.
		*/
		virtual float GetAlphaTestReference() const = 0;

		/** Sets the alpha test reference value. The value must be in the range [0.0, 1.0].
		/param value new alpha test reference value
		*/
		virtual void SetAlphaTestReference(float value) = 0;

		/** Gets viewport settings
		/return viewport settings
		*/
		virtual const MaxSDK::Graphics::IViewportViewSetting* GetViewportSettings() const = 0;

		/** Get if this draw context is used for hit test
		/return true if the draw context is used for hit test pass
		*/
		virtual bool IsHitTest() const = 0;
	};
} }
