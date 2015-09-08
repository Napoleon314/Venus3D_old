//
// Copyright 2013 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "BaseMaterialHandle.h"
#include "TextureHandle.h"
#include "TargetHandle.h"
#include "VertexBufferHandle.h"
#include "../strclass.h"
#include "../point4.h"
#include "../point3.h"

namespace MaxSDK { namespace Graphics {

class Matrix44;

/** HLSLMaterialHandle is a material class to support HLSL custom material.
This class is used for viewport display and Quicksilver.
It's used by RenderItemHandle::SetCustomMaterial() to describe HLSL custom material.
It can also be used for realistic material display. see IHLSLMaterialTranslator for more details.
How to use:
\code
HLSLMaterialHandle hMaterial;
hMaterial.Initialize(shaderFileName);
hMaterial.SetFloatParameter(_M("myFloatShaderParam"), floatValue);
pItem->SetCustomMaterial(hMaterial); // pItem is an instance of RenderItemHandle
\endcode
*/
class HLSLMaterialHandle : public BaseMaterialHandle
{
public:

	/** This is parameter type of HLSLMaterialHandle parameter.*/
	enum ParameterType
	{
		ParameterTypeUnknown,				/*!< unknown parameter type*/
		ParameterTypeFloat,					/*!< float type*/
		ParameterTypeInt,					/*!< int type*/
		ParameterTypeBool,					/*!< BOOL type*/
		ParameterTypePoint3,				/*!< Point3 type*/
		ParameterTypePoint4,				/*!< Point4 type*/
		ParameterTypeMatrix,				/*!< Matrix type*/
		ParameterTypeTexture,				/*!< texture type*/

		ParameterTypeFloatArray,			/*!< float array type*/
		ParameterTypeIntArray,				/*!< int array type*/
		ParameterTypeBoolArray,				/*!< BOOL array type*/
		ParameterTypePoint3Array,			/*!< Point3 array type*/
		ParameterTypePoint4Array,			/*!< Point4 array type*/
		ParameterTypeMatrixArray,			/*!< Matrix array type*/
	};

	/** This is parameter description of HLSLMaterialHandle parameter.*/
	struct ParameterDesc : public MaxHeapOperators
	{
		ParameterType parameterType;		/*!< type of parameter*/
		MSTR parameterName;					/*!< unique name of parameter in shader*/
	};

	GraphicsDriverAPI HLSLMaterialHandle();
	GraphicsDriverAPI HLSLMaterialHandle(const HLSLMaterialHandle& from);
	GraphicsDriverAPI HLSLMaterialHandle& operator = (const HLSLMaterialHandle& from);
	GraphicsDriverAPI virtual ~HLSLMaterialHandle();
	
	/** Initialize an instance of HLSLMaterialHandle.
	A HLSLMaterialHandle should be initialized before it's used.
	\param[in] fileName HLSL shader file name.
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool InitializeWithFile(const MSTR& fileName);

	/** Initialize an instance of HLSLMaterialHandle with shader string.
	A HLSLMaterialHandle should be initialized before it's used.
	\param[in] shaderContent HLSL shader content. 
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool InitializeWithString(const CStr& shaderContent);

	/** Initialize an instance of HLSLMaterialHandle from resource.
	A HLSLMaterialHandle should be initialized before it's used.
	\param[in] iShaderID resource ID of the shader
	\param[in] hInst module instance handle
	\param[in] strTypeName resource block name
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool InitializeWithResource(int iShaderID, HINSTANCE hInst, const MSTR& strTypeName);

	/** Returns the material required streams to tell how to setup mesh data.
	\return the material required streams.
	*/
	GraphicsDriverAPI virtual const MaterialRequiredStreams* GetRequiredStreams() const;

	/** Returns the parameter count of material.
	\return the parameter count.
	*/
	GraphicsDriverAPI int GetParameterCount() const;

	/** Returns the description of parameter.
	\param[in] index The parameter index.
	\param[out] parameterDesc The parameter description
	\return true if successful, otherwise false.
	*/
	GraphicsDriverAPI bool GetParameterDescription(int index, ParameterDesc& parameterDesc) const;

	/** Returns the index of parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\return valid index if successful, otherwise -1
	*/
	GraphicsDriverAPI int GetParameterIndex(const MSTR& parameterName) const;

	/** Sets int parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The int parameter value.
	*/
	GraphicsDriverAPI void SetIntParameter(const MSTR& parameterName, int value);
	/** Sets int parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The int parameter value.
	*/
	GraphicsDriverAPI void SetIntParameter(int index, int value);

	/** Sets int array parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The pointer of int array parameter value.
	\param[in] count The count of int array.
	*/
	GraphicsDriverAPI void SetIntArrayParameter(const MSTR& parameterName, const int* value, unsigned int count);

	/** Sets int array parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The pointer of int array parameter value.
	\param[in] count The count of int array.
	*/
	GraphicsDriverAPI void SetIntArrayParameter(int index, const int* value, unsigned int count);

	/** Sets float parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The parameter value.
	*/
	GraphicsDriverAPI void SetFloatParameter(const MSTR& parameterName, float value);

	/** Sets float parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The parameter value.
	*/
	GraphicsDriverAPI void SetFloatParameter(int index, float value);

	/** Sets float array parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The pointer of float array parameter value.
	\param[in] count The count of float array.
	*/
	GraphicsDriverAPI void SetFloatArrayParameter(const MSTR& parameterName, const float* value, unsigned int count);

	/** Sets float array parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The pointer of float array parameter value.
	\param[in] count The count of float array.
	*/
	GraphicsDriverAPI void SetFloatArrayParameter(int index, const float* value, unsigned int count);

	/** Sets bool parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The parameter value.
	*/
	GraphicsDriverAPI void SetBoolParameter(const MSTR& parameterName, BOOL value);

	/** Sets bool parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The parameter value.
	*/
	GraphicsDriverAPI void SetBoolParameter(int index, BOOL value);

	/** Sets bool array parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The pointer of bool array parameter value.
	\param[in] count The count of bool array.
	*/
	GraphicsDriverAPI void SetBoolArrayParameter(const MSTR& parameterName, const BOOL* value, unsigned int count);
	/** Sets bool array parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The pointer of bool array parameter value.
	\param[in] count The count of bool array.
	*/
	GraphicsDriverAPI void SetBoolArrayParameter(int index, const BOOL* value, unsigned int count);

	/** Sets float4 parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The parameter value.
	*/
	GraphicsDriverAPI void SetFloat4Parameter(const MSTR& parameterName, const Point4& value);

	/** Sets float4 parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The parameter value.
	*/
	GraphicsDriverAPI void SetFloat4Parameter(int index, const Point4& value);

	/** Sets float4 array parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The pointer of float4 array parameter value.
	\param[in] count The count of float4 array.
	*/
	GraphicsDriverAPI void SetFloat4ArrayParameter(const MSTR& parameterName, const Point4* value, unsigned int count);

	/** Sets float4 array parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The pointer of float4 array parameter value.
	\param[in] count The count of float4 array.
	*/
	GraphicsDriverAPI void SetFloat4ArrayParameter(int index, const Point4* value, unsigned int count);
	
	/** Sets float3 parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The parameter value.
	*/
	GraphicsDriverAPI void SetFloat3Parameter(const MSTR& parameterName, const Point3& value);

	/** Sets float3 parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The parameter value.
	*/
	GraphicsDriverAPI void SetFloat3Parameter(int index, const Point3& value);

	/** Sets float3 array parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The pointer of float3 array parameter value.
	\param[in] count The count of float3 array.
	*/
	GraphicsDriverAPI void SetFloat3ArrayParameter(const MSTR& parameterName, const Point3* value, unsigned int count);

	/** Sets float3 array parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The pointer of float3 array parameter value.
	\param[in] count The count of float3 array.
	*/
	GraphicsDriverAPI void SetFloat3ArrayParameter(int index, const Point3* value, unsigned int count);

	/** Sets matrix parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The parameter value.
	*/
	GraphicsDriverAPI void SetMatrixParameter(const MSTR& parameterName, const Matrix44& value);

	/** Sets matrix parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The parameter value.
	*/
	GraphicsDriverAPI void SetMatrixParameter(int index, const Matrix44& value);
		

	/** Sets matrix array parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] value The pointer of matrix array parameter value.
	\param[in] count The count of matrix array.
	*/
	GraphicsDriverAPI void SetMatrixArrayParameter(const MSTR& parameterName, const Matrix44* value, unsigned int count);

	/** Sets matrix array parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] value The pointer of matrix array parameter value.
	\param[in] count The count of matrix array.
	*/
	GraphicsDriverAPI void SetMatrixArrayParameter(int index, const Matrix44* value, unsigned int count);

	/** Sets texture parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] textureHandle The parameter texture handle value.
	*/
	GraphicsDriverAPI void SetTextureParameter(const MSTR& parameterName, const TextureHandle& textureHandle);

	/** Sets texture parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] textureHandle The parameter texture handle value.
	*/
	GraphicsDriverAPI void SetTextureParameter(const MSTR& parameterName, const TargetHandle& textureHandle);

	/** Sets texture parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] textureHandle The parameter texture handle value.
	*/
	GraphicsDriverAPI void SetTextureParameter(int index, const TextureHandle& textureHandle);

	/** Sets texture parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] textureHandle The parameter texture handle value.
	*/
	GraphicsDriverAPI void SetTextureParameter(int index, const TargetHandle& textureHandle);
	
	/** Sets buffer parameter.
	\param[in] parameterName The unique parameter name of the parameter.
	\param[in] bufferHandle The parameter VertexBufferHandle or IndexBufferHandle value.
			   Note this buffer should be created with BufferUsageStatic | BufferUsageTarget usage flag
	*/
	GraphicsDriverAPI void SetBufferParameter(const MSTR& parameterName, const SmartHandle& bufferHandle);
	
	/** Sets buffer parameter.
	\param[in] index The parameter index of the parameter.
	\param[in] bufferHandle The parameter VertexBufferHandle or IndexBufferHandle value.
			   Note this buffer should be created with BufferUsageStatic | BufferUsageTarget usage flag
	*/
	GraphicsDriverAPI void SetBufferParameter(int index, const SmartHandle& bufferHandle);

	/** Sets map channel parameter.
	\param[in] textureCoordinateIndex The texture coordinate index of shader.
	\param[in] maxMapChannel Map channel value of 3ds max.
	*/
	GraphicsDriverAPI void SetMapChannelParameter(int textureCoordinateIndex, int maxMapChannel);
	
	/** Sets technique name parameter.
	\param[in] techniqueName The active technique name.
	*/
	GraphicsDriverAPI void SetActiveTechniqueName(const MSTR& techniqueName);

};


} } // end namespace