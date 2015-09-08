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

namespace MaxSDK { namespace Graphics {

/** This is material class to support material with texture.
How to use:
\code
TextureMaterialHandle hMaterial
hMaterial.Initialize();
hMaterial.SetTexture(hTexture); // hTexture is an instance of TextureHandle
pItem->SetCustomMaterial(hMaterial); // pItem is an instance of RenderItemHandle
\endcode
*/
class TextureMaterialHandle : public BaseMaterialHandle
{
public:
	GraphicsDriverAPI TextureMaterialHandle();
	GraphicsDriverAPI TextureMaterialHandle(const TextureMaterialHandle& from);
	GraphicsDriverAPI TextureMaterialHandle& operator = (const TextureMaterialHandle& from);
	GraphicsDriverAPI virtual ~TextureMaterialHandle();
	
	/** Initialize an instance of texture material.
	A TextureMaterialHandle should be initialized before it's used.
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool Initialize();

	/** Specifies a texture handle.
	\param[in] hTexture the texture handle.
	*/
	GraphicsDriverAPI void SetTexture(const BaseRasterHandle& hTexture);

	/** Returns the texture handle.
	\return the texture handle.
	*/
	GraphicsDriverAPI BaseRasterHandle GetTexture() const;

	/** Specifies texture map channel ID.
	Change of texture map channel ID will change the MaterialRequiredStreams.
	\param[in] channelID the texture map channel ID.
	*/
	GraphicsDriverAPI void SetTextureMapChannel(int channelID);

	/** Returns the texture map channel ID.
	\return the texture map channel ID.
	*/
	GraphicsDriverAPI int GetTextureMapChannel();

	/** Returns the material required streams to tell how to setup mesh data.
	\return the material required streams.
	*/
	GraphicsDriverAPI virtual const MaterialRequiredStreams* GetRequiredStreams() const;
};


} } // end namespace
