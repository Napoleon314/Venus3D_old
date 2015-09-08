//
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../../../maxsdk/include/Graphics/RenderItemHandle.h"
#include "../../../maxsdk/include/Graphics/TargetHandle.h"
#include "../../../Graphics/RealTimeShaderGenerator/SDK/ShaderGeneratorAPI/IShadingEnvironment.h"
#include "RenderWorldHandle.h"
#include "ICamera.h"

namespace MaxSDK { namespace Graphics {

/**	CustomRenderStageHandle is the base wrapper class of the custom render stage.
*/
class CustomRenderStageHandle : public SmartHandle
{
public:
	GraphicsDriverAPI CustomRenderStageHandle();
	GraphicsDriverAPI CustomRenderStageHandle(const CustomRenderStageHandle& from);
	GraphicsDriverAPI CustomRenderStageHandle& operator = (const CustomRenderStageHandle& from);
	GraphicsDriverAPI virtual ~CustomRenderStageHandle();

	/** Initialize this custom render stage. 
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI void Initialize();
	/** Run the custom render stage
	*/
	GraphicsDriverAPI void Run();

	/** Set the color target to the custom render stage
		\param[in] pColorTarget the color target
	*/
	GraphicsDriverAPI virtual void SetColorTarget(const TargetHandle& pColorTarget);
	/** Get the color target of the custom render stage
		\return the color target of the custom render stage
	*/
	GraphicsDriverAPI virtual TargetHandle GetColorTarget() const;

	/** Set the depth target to the custom render stage
		\param[in] pDepthTarget the depth target
	*/
	GraphicsDriverAPI virtual void SetDepthTarget(const TargetHandle& pDepthTarget);
	/** Get the depth target of the custom render stage
		\return the depth target of the custom render stage
	*/
	GraphicsDriverAPI virtual TargetHandle GetDepthTarget() const;

	/** Set the render world to the custom render stage
		\param[in] pRenderWorld the render world
	*/
	GraphicsDriverAPI virtual void SetRenderWorld(const RenderWorldHandle& pRenderWorld);
	/** Get the render world of the custom render stage
		\return  the render world of the custom render stage
	*/
	GraphicsDriverAPI virtual RenderWorldHandle GetRenderWorld() const;

	/** Set the render camera to the custom render stage
		\param[in] pCamera the render camera
	*/
	GraphicsDriverAPI void SetCamera(const MaxSDK::Graphics::CameraPtr& pCamera);
	/** Get the render camera of the custom render stage
		\return the render camera of the custom render stage
	*/
	GraphicsDriverAPI const MaxSDK::Graphics::CameraPtr& GetCamera() const;
	
	/** Set the visibility group to the custom render stage
		\param[in] visibilityGroup the visibility group
	*/
	GraphicsDriverAPI void SetVisibilityGroup(MaxSDK::Graphics::RenderItemVisibilityGroup visibilityGroup);
	/** Get the visibility group of the custom render stage
		\return the visibility group of the custom render stage
	*/
	GraphicsDriverAPI MaxSDK::Graphics::RenderItemVisibilityGroup GetVisibilityGroup() const;
	
	/** Set the background color to the custom render stage
		\param[in] color the background color
	*/
	GraphicsDriverAPI void SetBackgroundColor(const AColor& color);
	/** Get the background color of the custom render stage
		\return the background color of the custom render stage
	*/
	GraphicsDriverAPI const AColor& GetBackgroundColor() const;
	
	/** Set the viewport to the custom render stage
		\param[in] viewport the viewport with .xy the left-top point of the viewport 
		and .zw the right-bottom point of the viewport
	*/
	GraphicsDriverAPI void SetViewport(const Point4& viewport);
	/** Get the viewport of the custom render stage
		\return the viewport of the custom render stage
	*/
	GraphicsDriverAPI const Point4& GetViewport() const;

	/** Set whether we clear the depth buffer before running the custom render stage
		\param[in] bClear whether we clear the depth buffer
	*/
	GraphicsDriverAPI void SetClearDepth(bool bClear);
	/** Get whether we clear the depth buffer before running the custom render stage
		\return true if we clear the depth buffer before running the custom render 
		stage or false otherwise
	*/
	GraphicsDriverAPI bool GetClearDepth() const;

	/** Set whether we clear the color buffer before running the custom render stage
		\param[in] bClear whether we clear the depth buffer
	*/
	GraphicsDriverAPI void SetClearColorBuffer(bool bClear);
	/** Get whether we clear the color buffer before running the custom render stage
		\return true if we clear the color buffer before running the custom render 
		stage or false otherwise
	*/
	GraphicsDriverAPI bool GetClearColorBuffer() const;

	/** Set the override material to the custom render stage
		\param[in] pOverrideMaterial the override material
	*/
	GraphicsDriverAPI void SetOverrideMaterial(const BaseMaterialHandle& pOverrideMaterial);
	/** Get the override material of the custom render stage
		\return the override material of the custom render stage
	*/
	GraphicsDriverAPI BaseMaterialHandle GetOverrideMaterial() const;
};

} } // namespaces

