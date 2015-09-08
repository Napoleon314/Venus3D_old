//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once
#include "../maxapi.h"
#include "GraphicsEnums.h"

#define IVIEWPORT_SETTINGS_INTERFACE_ID Interface_ID(0x761840a9, 0x437f4090)

namespace MaxSDK { namespace Graphics {

	/** \brief Viewport Setting For The Nitrous Graphics Viewport
		\note To access this interface from a ViewExp object, please call 
		ViewExp::GetInterface(IVIEWPORT_SETTINGS_INTERFACE_ID).
	*/
	class IViewportViewSetting : public FPMixinInterface
	{
	public:
		/** Enable/Disable progressive rendering.
		*/
		virtual void SetProgressiveRenderingEnabled(bool enabled) = 0;
		/** Get the enable state of progressive rendering
			\return true if enabled or false otherwise
		*/
		virtual bool GetProgressiveRenderingEnabled() const = 0;

		/** Enable/Disable show edged faces.
		*/
		virtual void SetShowEdgedFaces(bool bShowEdgeFaces) = 0;
		/** Get the state of showing edged faces
			\return true if edged faces is on or false otherwise
		*/
		virtual bool GetShowEdgedFaces() const = 0;

		/** Enable/Disable use texture.
		*/
		virtual void SetUseTexture(bool bUseTexture) = 0;
		/** Get the state of 'use texture'
			\return true if use texture or false otherwise
		*/
		virtual bool GetUseTexture() const = 0;

		/** Enable/Disable display selected with edged faces.
		*/
		virtual void SetSelectedEdgedFaces(bool bSelEdgedFaces) = 0;
		/** Get the state of 'display selected with edged faces'
			\return true if this option is on or false otherwise
		*/
		virtual bool GetSelectedEdgedFaces() const = 0;

		/** Enable/Disable use selection brackets.
		*/
		virtual void SetShowSelectionBrackets(bool bShowSelectionBrackets) = 0;
		/** Get the state of show selection brackets.
			\return true if this option is on or false otherwise
		*/
		virtual bool GetShowSelectionBrackets() const = 0;

		/** Enable/Disable shade selected faces.
		*/
		virtual void SetShadeSelectedFaces(bool bShadeSelFaces) = 0;
		/** Get the state of shade selected edged faces.
			\return true if this option is on or false otherwise
		*/
		virtual bool GetShadeSelectedFaces() const = 0;

		/** Set true to disable viewport or false otherwise.
		*/
		virtual void SetViewportDisable(bool bViewportDisable) = 0;
		/** Get the state of viewport disable
			\return true if viewport is currently disabled or false otherwise
		*/
		virtual bool GetViewportDisable() const = 0;

		/** Enable/Disable viewport clipping.
		*/
		virtual void SetViewportClipping(bool bVptClipping) = 0;
		/** Get the state of viewport clipping
			\return true if this option is on or false otherwise
		*/
		virtual bool GetViewportClipping() const = 0;

		/** Enable/Disable use environment background color.
		*/
		virtual void SetUseEnvironmentBackgroundColor(bool bUseEnvColor) = 0;
		/** Get the state of use environment background color
			\return true if this option is on or false otherwise
		*/
		virtual bool GetUseEnvironmentBackgroundColor() const = 0;

		/** Enable/Disable use viewport background.
		*/
		virtual void SetUseViewportBackground(bool bValue) = 0;
		/** Get the state of use viewport background
			\return true if this option is on or false otherwise
		*/
		virtual bool GetUseViewportBackground() const = 0;

		/** Enable/Disable show highlight.
		*/
		virtual void SetShowHighLight(bool bShowHighLight) = 0;
		/** Get the state of show highlight
			\return true if this option is on or false otherwise
		*/
		virtual bool GetShowHighlight() const = 0;

		/** Set the current view style.
		*/
		virtual void SetViewportVisualStyle(VisualStyle visualStyle) = 0;
		/** Get the viewport visual style of current viewport
			\return the visual style
		*/
		virtual VisualStyle GetViewportVisualStyle() const = 0;

		/** Enable/Disable shade selected objects.
		*/
		virtual void SetShadeSelectedObjects(bool bShadeSelObj) = 0;
		/** Get the state of shade selected objects
			\return true if this option is on or false otherwise
		*/
		virtual bool GetShadeSelectedObjects() const = 0;

		/** Enable/Disable auto display selected light.
		*/
		virtual void SetAutoDisplaySelectedLight(bool bAutoDisSelLight) = 0;
		/** Get the state of auto display selected light
			\return true if this option is on or false otherwise
		*/
		virtual bool GetAutoDisplaySelectedLight() const = 0;

		/** Enable/Disable ambient occlusion.
		*/
		virtual void SetAmbientOcclusionEnabled(bool bAmbientOcclusionEnabled) = 0;
		/** Get the enabled state of ambient occlusion
			\return true if ambient occlusion is enabled or false otherwise
		*/
		virtual bool GetAmbientOcclusionEnabled() const = 0;

		/** Enable/Disable shadow.
		*/
		virtual void SetShadowsEnabled(bool bShadowsEnabled) = 0;
		/** Get the state of display shadow
			\return true if shadow is on or false otherwise
		*/
		virtual bool GetShadowsEnabled() const = 0;

		/** Enable/Disable degrade to default lights.
		*/
		virtual void SetAdaptiveDegradeAlwaysDegradeLights(bool bDegradeLights) = 0;
		/** Get the state of degrade to default lights
			\return true if this option is on or false otherwise
		*/
		virtual bool GetAdaptiveDegradeAlwaysDegradeLights() const = 0;

		/** Enable/Disable draw back faces for wireframe mesh in adaptive degradation.
		*/
		virtual void SetAdaptiveDegradeDrawBackfaces(bool bDrawBackfaces) = 0;
		/** Get the state of draw backfaces
			\return true if this option is on or false otherwise
		*/
		virtual bool GetAdaptiveDegradeDrawBackfaces() const = 0;

		/** Set true to never degrade geometry or false otherwise.
		*/
		virtual void SetAdaptiveDegradeNeverDegradeGeometry(bool bNeverDegradeGeometry) = 0;
		/** Get the state of never degrade geometry
			\return true if this option is on or false otherwise
		*/
		virtual bool GetAdaptiveDegradeNeverDegradeGeometry() const = 0;

	protected:
		virtual ~IViewportViewSetting(){}

	};
} }// namespaces


