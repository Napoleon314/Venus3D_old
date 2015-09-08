//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "BaseMaterialHandle.h"
#include "TextureHandle.h"
#include "../acolor.h"


namespace MaxSDK { namespace Graphics {

/** StandardMaterialHandle is a material class to support phong style material .
This class is used for viewport display and Quicksilver.
It's used by RenderItemHandle::SetCustomMaterial() to describe the phong style surface.
How to use:
\code
StandardMaterialHandle hMaterial;
hMaterial.Initialize();
pItem->SetCustomMaterial(hMaterial); // pItem is an instance of RenderItemHandle
\endcode
*/
class StandardMaterialHandle : public BaseMaterialHandle
{
public:
	GraphicsDriverAPI StandardMaterialHandle();
	GraphicsDriverAPI StandardMaterialHandle(const StandardMaterialHandle& from);
	GraphicsDriverAPI StandardMaterialHandle& operator = (const StandardMaterialHandle& from);
	GraphicsDriverAPI virtual ~StandardMaterialHandle();
	
	/** Initialize the standard material instance.
	A StandardMaterialHandle must be initialized before it's used.
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool Initialize();

	/** Returns the material required streams to tell how to setup mesh data.
	When a StandardMaterialHandle is used by RenderItemHandle::SetCustomMaterial(), 
	it also needs to know about the material requirement streams to setup corresponding 
	mesh data. It could be used by GenerateRenderItems::GenerateRenderItems() for RenderItemHandle.
	\return the material required streams.
	*/
	GraphicsDriverAPI virtual const MaterialRequiredStreams* GetRequiredStreams() const;

	/** Sets ambient color.
	\param[in] color The ambient color.
	*/
	GraphicsDriverAPI void SetAmbient(const AColor& color);

	/** Returns the ambient color.
	\return the ambient color.
	*/
	GraphicsDriverAPI const AColor& GetAmbient() const;

	/** Sets diffuse color.
	If diffuse texture exists, this value will be ignored.
	\param[in] color The diffuse color.
	*/
	GraphicsDriverAPI void SetDiffuse(const AColor& color);
	
	/** Returns the diffuse color.
	\return the diffuse color.
	*/
	GraphicsDriverAPI const AColor& GetDiffuse() const;

	/** Sets specular color.
	\param[in] color The specular color.
	*/
	GraphicsDriverAPI void SetSpecular(const AColor& color);

	/** Returns the specular color.
	\return the specular color.
	*/
	GraphicsDriverAPI const AColor& GetSpecular() const;

	/** Sets specular level.
	\param[in] level The specular level.
	*/
	GraphicsDriverAPI void SetSpecularLevel(float level);

	/** Returns the specular level.
	\return the specular level.
	*/
	GraphicsDriverAPI float GetSpecularLevel() const;

	/** Sets specular power.
	\param[in] power The specular power.
	*/
	GraphicsDriverAPI void SetSpecularPower(float power);
	
	/** Returns the specular power.
	\return the specular power.
	*/
	GraphicsDriverAPI float GetSpecularPower() const;

	/** Sets emission color.
	\param[in] color The emission color.
	*/
	GraphicsDriverAPI void SetEmission(const AColor& color);

	/** Returns the emission color.
	\return the emission color.
	*/
	GraphicsDriverAPI const AColor& GetEmission() const;

	/** Sets opacity value.
	If opacity texture exists, this value will be ignored.
	\param[in] opacity The opacity value.
	*/
	GraphicsDriverAPI void SetOpacity(float opacity);

	/** Returns the opacity value.
	\return the opacity value.
	*/
	GraphicsDriverAPI float GetOpacity() const;

	/** Sets two side flag.
	\param[in] twoSided The two side flag.
	*/
	GraphicsDriverAPI void SetTwoSided(bool twoSided);
	
	/** Returns the two side flag.
	\return the two side flag.
	*/
	GraphicsDriverAPI bool GetTwoSided() const;

	/** Sets diffuse texture.
	\param[in] hTexture The diffuse texture handle.
	*/
	GraphicsDriverAPI void SetDiffuseTexture(const BaseRasterHandle& hTexture);

	/** Returns the diffuse texture.
	\return the diffuse texture.
	*/
	GraphicsDriverAPI BaseRasterHandle GetDiffuseTexture();

	/** Clear the diffuse texture.
	*/
	GraphicsDriverAPI void ClearDiffuseTexture();

	/** Sets opacity texture.
	\param[in] hTexture The opacity texture.
	*/
	GraphicsDriverAPI void SetOpacityTexture(const BaseRasterHandle& hTexture);

	/** Returns the opacity texture.
	\return the opacity texture.
	*/
	GraphicsDriverAPI BaseRasterHandle GetOpacityTexture();

	/** Clear the opacity texture.
	*/
	GraphicsDriverAPI void ClearOpacityTexture();
};

}}