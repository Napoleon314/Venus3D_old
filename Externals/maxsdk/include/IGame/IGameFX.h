/**********************************************************************
*<
FILE: IGameFX.h

DESCRIPTION: D3D Effect interfaces for IGame

CREATED BY: Neil Hazzard,  Autodesk Ltd

HISTORY: created 06/25/05

IGame Version: 1.122

*>	Copyright (c) 2005, All Rights Reserved.
**********************************************************************/
#pragma once

#include "../maxheap.h"
#include "../BuildWarnings.h"
#include "../strclass.h"

// forward declarations
class IGameNode;
class IGameProperty;
class IGameFXProperty;
class IGameFXTechnique;
class IGameFXFunction;
namespace MaxSDK
{
	namespace AssetManagement
	{
		class AssetUser;
	}
}
class Mtl;

//! Main interface for parsing a generic effect file
/*! This interface breaks down an effect file into a table of parameters, techniques and code.  It provides a non format and compiler specific 
summary of an D3D %Effect file.  This allow the developer to break up an effect for recompiling and regenerating on another platform.
\deprecated From 3ds Max 2015 IGameFX is deprecated.  It no longer supports the newer Shader models or Nitrous.  Please use the regular 
Microsoft DirectX SDK and supporting APIs to extract shader data.
*/
MAX_DEPRECATED class IGameFX: public MaxHeapOperators
{

public:

	//! Define the source of the effect data.
	enum EffectSource{
		kFile,			/*!<The effect is defined in a file*/
		kBuffer			/*!<The effect is defined in memory*/
	};

	//! The compile time option for the profile used in the shader
	enum IGameFXProfile{
		kVSPS_20,		/*!<Vertex and Pixel shader version 2.0*/
		kVSPS_2X,		/*!<Vertex and Pixel shader version 2.A*/
		kVSPS_30		/*!<Vertex and Pixel shader version 3.0*/
	};
	//! \brief Destructor 
	MAX_DEPRECATED ~IGameFX();

	//! Initialise the interface with the effect to be decompiled
	MAX_DEPRECATED bool Initialize(const MaxSDK::AssetManagement::AssetUser& effect, EffectSource input);

	//! Parse the effect file, providing the host material as a source for parameter hook up
	MAX_DEPRECATED bool ParseData(Mtl * effectMtl);

	MAX_DEPRECATED const MSTR GetError();

	//! The total number of properties in the effect
	MAX_DEPRECATED int GetNumberOfProperties();

	//! Access to a specific parameter.
	MAX_DEPRECATED IGameFXProperty * GetIGameFXProperty(int index);

	//! The total number of techniques in the effect
	MAX_DEPRECATED int GetNumberOfTechniques();

	//! Access to a specifc technique.
	MAX_DEPRECATED IGameFXTechnique* GetIGameFXTechnique(int index);

	//! the total number of functions on the effect
	MAX_DEPRECATED int GetNumberOfFunctions() ;

	//! Get the function from the index supplied
	MAX_DEPRECATED IGameFXFunction * GetIGameFXFunction(int index) ;

	//!  Access the filename of the effect used.  This will only provide a valid filename if kFile is used in Initialize
	MAX_DEPRECATED const MaxSDK::AssetManagement::AssetUser & GetEffectFile() ;
};


//! An interface that holds the data for an individual parameter in the D3D effect.  This will include global parameters, samplers and transform data
/*! The IGameFX interface will try to match any parameter with its host data.  For the DxMaterial this would be the paramblock that drives basic parameters.
For the DxStdMtl the parameters are driven by the host StdMtl2, but all lighting data is patched in so a "binding" node can be access.  A binding node is a max
node that drives the data.  For a light this could be position, direction, color, falloff and hotspot.  The semantic will define the usage.  A binding token will 
eventually be used to define by name where the parameter lives in the scenegraph
\deprecated From 3ds Max 2015 IGameFXProperty is deprecated.  It no longer supports the newer Shader models or Nitrous.  Please use the regular 
Microsoft DirectX SDK and supporting APIs to extract shader data.
*/

MAX_DEPRECATED class IGameFXProperty: public MaxHeapOperators
{
public:

	enum IGameFXPropUsage{
		IGAMEFX_GENERAL,	/*!<General property, such a a float or transform param*/
		IGAMEFX_SAMPLER,	/*!<A texture sampler, will propabaly have a pipe state*/
		IGAMEFX_LIGHT,		/*!<A light property - a binding node is used*/
		IGAMEFX_TEXTURE		/*!<A Texture property - this would be a Bitmap in max.*/
	};
	//! \brief Destructor 
	MAX_DEPRECATED ~IGameFXProperty();

	//! Get the IGameProperty of the effect property
	/*! The IGameFx interface will try to extract all relevant data between the host material and the effect.  Any paramblock data that is used to drive
	the effect will be stored in an IGameProperty
	*/
	MAX_DEPRECATED IGameProperty * GetIGameProperty();

	//! This defines what the property is - General means a basic parameter, Sampler means a texture sampler.  you can use this to determine whether pipe state exist
	MAX_DEPRECATED IGameFXPropUsage GetPropertyUsage();

	//! Get the semantic for the parameter
	MAX_DEPRECATED const MCHAR * GetFXSemantic();

	//! The number of annotations for the parameter.
	MAX_DEPRECATED int GetNumberOfFXAnnotations();

	//! A key/Value pair defining the name of value of the annotation.  The High level language type id also provided.
	MAX_DEPRECATED bool GetFXAnnotation(int index, MSTR * languageType, MSTR * key, MSTR * value);

	//! the default value of the parameter if supplied
	MAX_DEPRECATED const MCHAR * GetFXDefaultValue();

	//! The number of render states values the parameter has.  This would typicaly be based on a Sampler.
	MAX_DEPRECATED int GetNumberofFXPipeStates();

	//!The key/value pair of any pipeline states supported in the effect.  At the parameter level this will usually be a Sampler
	MAX_DEPRECATED bool GetPipeStates(int index, MSTR * key, MSTR *value);

	//!The name of the property fround in the effect file
	MAX_DEPRECATED const MCHAR * GetPropertyName();

	//! The type of the property e.g. float4, int, bool etc..
	MAX_DEPRECATED const MCHAR * GetPropertyType();

	//! Get the Binding Node
	MAX_DEPRECATED IGameNode * GetBindingNode() ;
};

//! Interface that holds per pass information.  Each technique will be made up of one or more passes.
/*! A pass will contain pipe state information and also vertex or pixel shader programs
\deprecated From 3ds Max 2015 IGameFXPass is deprecated.  It no longer supports the newer Shader models or Nitrous.  Please use the regular 
Microsoft DirectX SDK and supporting APIs to extract shader data.
*/
MAX_DEPRECATED class IGameFXPass: public MaxHeapOperators{
public:
	//! \brief Destructor 
	MAX_DEPRECATED ~IGameFXPass() ;

	//! The name of the pass
	MAX_DEPRECATED const MCHAR * GetPassName();

	//!The number of annotations associated with the pass
	MAX_DEPRECATED int GetNumberOfFXAnnotations();

	//! Access the key/value pair for the annotation
	MAX_DEPRECATED bool GetFXAnnotation(int index, MSTR * languageType, MSTR * key, MSTR * value);

	//! the number of render pipe states
	MAX_DEPRECATED int GetNumberofFXPipeStates();

	//!Acces the specific key/value pair for the state.  This would typically be render state information
	MAX_DEPRECATED bool GetPipeStates(int index, MSTR * key, MSTR *value);

	//! Access the vertex shader for the current pass
	/*!
	\param entry A buffer to receive the entry name for the function
	\param code A buffer to receive the actual code fragment
	\return True if successful, or false if no code was found
	*/
	MAX_DEPRECATED bool GetVertexShader(MSTR * entry, MSTR * code);
	
	//! Access the pixel shader for the current pass
	/*!
	\param entry A buffer to receive the entry name for the function
	\param code A buffer to receive the actual code fragment
	\return True if successful, or false if no code was found
	*/
	MAX_DEPRECATED bool GetPixelShader(MSTR * entry, MSTR * code);

	//! Access the profile used for the current pass.
	MAX_DEPRECATED IGameFX::IGameFXProfile GetTargetProfile();
};

//! An interface holding technique information for the effect.  Each technqiue is made up of one or more passes
/*
\deprecated From 3ds Max 2015 IGameFXTechnique is deprecated.  It no longer supports the newer Shader models or Nitrous.  Please use the regular 
Microsoft DirectX SDK and supporting APIs to extract shader data.
*/
MAX_DEPRECATED class IGameFXTechnique: public MaxHeapOperators
{
public:
	//! \brief Destructor 
	MAX_DEPRECATED ~IGameFXTechnique();

	//! The number of passes making up the technique
	MAX_DEPRECATED int GetNumberOfPasses();

	//!The technique name
	MAX_DEPRECATED const MCHAR * GetTechniqueName();

	//!The number of annotations assigned to the technique
	MAX_DEPRECATED int GetNumberOfFXAnnotations();

	//! Access the key/value pair
	MAX_DEPRECATED bool GetFXAnnotation(int index, MSTR * languageType, MSTR * key, MSTR * value);

	//! Access the actual pass
	MAX_DEPRECATED IGameFXPass * GetIGameFXPass(int index);
};

//! Access function data contained in the effect file
/*
\deprecated From 3ds Max 2015 IGameFXFunction is deprecated.  It no longer supports the newer Shader models or Nitrous.  Please use the regular 
Microsoft DirectX SDK and supporting APIs to extract shader data.
*/
MAX_DEPRECATED class IGameFXFunction: public MaxHeapOperators
{
public:
	//! \brief Destructor 
	MAX_DEPRECATED ~IGameFXFunction();

	//! Access the code that makes the function
	MAX_DEPRECATED const MCHAR * GetFunctionBody();

	//! Access the name of the function
	MAX_DEPRECATED const MCHAR * GetFunctionName();
};
