//
// Copyright 2013 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../baseInterface.h"
#include "HLSLMaterialHandle.h"
#include "RenderEnums.h"

/** Interface ID of class IHLSLMaterialTranslator. */
#define IHLSL_MATERIAL_TRANSLATOR_INTERFACE_ID Interface_ID(0x7de9426c, 0x308028da)

namespace MaxSDK { namespace Graphics {

	/** Facilitates translation of material parameter values to their shaders representation.
	Materials can use programmable shaders to describe their appearance when displayed in the 
	Nitrous viewport and rendered with Quicksilver. Programmable shaders allow for a more realistic 
	and complex visual appearance than ISimpleMaterial does. Besides MetaSL shader ( See IShaderManager 
	and IParameterTranslatorfor more details), 3ds Max 2014 starts to support HLSL shader.

	IHLSLMaterialTranslator give a proper chance in UpdateHLSLMaterial() to update parameters of  
	HLSLMaterialHandle, such as parameter values, map channel values. See HLSLMaterialHandle for more details. 

	Plug-ins derive from IHLSLMaterialTranslator. They rather need to create instances of HLSLMaterialHandle. 
	3ds Max will query the plug-in for its IHLSLMaterialTranslator via a request for the interface identified 
	by IHLSL_MATERIAL_TRANSLATOR_INTERFACE_ID. Then get HLSLMaterialHandle via GetHLSLMaterialHandle() of IHLSLMaterialTranslator.
	Typically a plug-in would implement GetHLSLMaterialHandle of IHLSLMaterialTranslator in response to this request
	and its override of Animatable::GetInterface(Interface_ID) for IHLSLMaterialTranslator

	The following code snippet illustrates how a material plug-in may implement IHLSLMaterialTranslator:
	\code
	#include <./graphics/IHLSLMaterialTranslator.h>
	// A material plug-in that has HLSLMaterialHandle and needs to update HLSLMaterial parameters.
	class MyMtlPlugin : public MtlBase, public MaxSDK::Graphics::IHLSLMaterialTranslator {
		// HLSLMaetrialHandle omitted for brevity
	private:
		HLSLMaetrialHandle mHLSLMaterialHandle;
	public:
		virtual bool UpdateHLSLMaterial(const TimeValue t, GraphicFeatureLevel featureLevel) {
			mHLSLMaterialHandle.SetFloatParameter(_M("myFloatShaderParam"), floatValue);
			mHLSLMaterialHandle.SetIntParameter(_M("myIntShaderParam"), intValue);
			return true;
		}

		virtual const HLSLMaterialHandle& GetHLSLMaterialHandle(GraphicFeatureLevel featureLevel) {
			return mHLSLMaterialHandle;
		}

		BaseInterface* GetInterface(Interface_ID iid) {
			if (IHLSL_MATERIAL_TRANSLATOR_INTERFACE_ID == iid)
				return static_cast<IHLSLMaterialTranslator*>(this);
			else
				return MtlBase::GetInterface(iid);
		}
	}
	\endcode
	*/
	class IHLSLMaterialTranslator : public BaseInterface
	{
	public:

		/** Retrieves the interface ID of IHLSLMaterialTranslator.
		*/
		virtual Interface_ID GetID() { return IHLSL_MATERIAL_TRANSLATOR_INTERFACE_ID; }

		/** Update HLSLMaterial parameters
		\param [in] t The time at which to update the plug-in's parameter value
		\param [in] featureLevel The device's feature level
		\return Returns true if successful, false otherwise.
		*/
		virtual bool UpdateHLSLMaterial(const TimeValue t, GraphicFeatureLevel featureLevel) = 0;

		/** Get HLSLMaterialHandle
		\param [in] featureLevel The device's feature level
		\return Returns the HLSLMaterialHandle.
		*/
		virtual const HLSLMaterialHandle& GetHLSLMaterialHandle(GraphicFeatureLevel featureLevel) = 0;
	};


} } // end namespace