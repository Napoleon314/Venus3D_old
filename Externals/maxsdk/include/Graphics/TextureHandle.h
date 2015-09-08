//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "GraphicsExport.h"
#include "BaseRasterHandle.h"
#include "TargetHandle.h"
#include "RenderEnums.h"
#include "../MaxHeap.h"
#include "../strclass.h"

namespace MaxSDK { namespace Graphics {

/** Defines a rectangular region that has been locked by a call to TextureHandle::LockRect(). 
If a RECT was provided to the LockRect call, pBits will be appropriately offset from the 
start of the texture resource.
*/
struct LockedRect : public MaxHeapOperators
{
	/** Number of bytes in one row of the texture resource. 
	*/
	long pitch;
	/** Pointer to the locked bits. 
	*/
	unsigned char* pBits;
} ;

/** This is texture class to support texture for material, such as TextureMaterialHandle.
How to use:
\code
TextureHandle hTexture;
hTexture.Initialize(100, 100, TargetFormatA8R8G8B8);
LockedRect lockedRect;
hTexture.WriteOnlyLockRectangle(0, lockedRect, NULL);
if (NULL != lockedRect.pBits )
{
	memset(lockedRect.pBits, 0xff, lockedRect.pitch * 100); // fill data to locked address
}
hTexture.WriteOnlyUnlockRectangle();

TextureMaterialHandle hMaterial;
hMaterial.Initialize();
hMaterial.SetTexture(hTexture);
\endcode
*/
class TextureHandle : public BaseRasterHandle
{
public:
	GraphicsDriverAPI TextureHandle();
	GraphicsDriverAPI TextureHandle(const TextureHandle& from);
	GraphicsDriverAPI TextureHandle& operator = (const TextureHandle& from);
	GraphicsDriverAPI virtual ~TextureHandle();

	/** Initialize the instance of a 2D texture.
	A TextureHandle should be initialized before it's used
	\param[in] width the width of texture.
	\param[in] height the height of texture.
	\param[in] format member of the TargetFormat enumerated type, describing the pixel format of the texture.
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool Initialize(size_t width, size_t height, TargetFormat format);
	
	/** Initialize the instance of specified type texture.
	A TextureHandle should be initialized before it's used
	\param[in] type type of the texture
	\param[in] width the width of texture.
	\param[in] height the height of texture.
	\param[in] depth the depth of texture. Used for volume texture.
	\param[in] slice number of sub textures. Used for texture array in DX11. For single texture, specify "1". DX9 ignores this parameter.
	\param[in] mipmaplevels level number of texture. If this value is zero, a complete mipmap chain is created.
	\param[in] format member of the TargetFormat enumerated type, describing the pixel format of the texture.
	\param[in] usage resource usage of the texture
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool Initialize(TextureType type, size_t width, size_t height, size_t depth, size_t slice, size_t mipmaplevels,TargetFormat format, ResourceUsage usage);

	/** Initialize the instance of texture from file.
	A TextureHandle should be initialized before it's used.
	\param[in] filename filename of the texture. Supported format:  .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga.
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool Initialize(const MSTR& filename);

	/** locking the Texture Target makes the target writable. 
	When a developer has finished writing to the target they should always call WriteOnlyUnlockRectangle.
	\param[in] subTargetIndex it specifies the index of sub-target in the cube texture to lock. For non-cube texture, use 0.
	\param[out] lockedRect store the returned locked rectangle information.
	\param[in] pRectangle pointer to a rectangle to lock. Specified by a pointer to a RECT structure. 
	Specifying NULL for this parameter expands the dirty region to cover the entire texture. 
	\return true if successful, otherwise false.
	*/
	GraphicsDriverAPI bool WriteOnlyLockRectangle(
		UINT subTargetIndex,
		LockedRect& lockedRect,
		CONST RECT* pRectangle);

	/** Unlocks a rectangle on a texture resource.
	\return true if successful, otherwise false.
	*/
	GraphicsDriverAPI bool WriteOnlyUnlockRectangle();

	/** Get number of sub-targets
	\return for CubeMap, returns 6. For volume map, returns depth in creation parameter. Otherwise, return 1
	*/
	GraphicsDriverAPI size_t GetSubTargetCount();

	/** Get sub target of the texture. 
	For standard 1D, 2D, 3D texture, only "0" is valid.
	For cube texture, valid value is 0-5.
	For Volume texture, valid value depends on creation parameter.
	\param[in] subIndex sub target index
	\return sub-target handle.
	*/
	GraphicsDriverAPI TargetHandle GetSubTarget(size_t subIndex);

	/** Set the size of the texture.
	\param[in] width the width of the texture
	\param[in] height the height of the texture
	\param[in] depth the height of the texture
	*/
	GraphicsDriverAPI void SetSize(size_t width, size_t height, size_t depth);

	/** Set whether require auto-generate mipmaps. Default value is true.
	\param[in] value true to enable auto-generate mipmap. false to disable
	*/
	GraphicsDriverAPI void SetAutoGenerateMipMaps(bool value);

	/** Get whether auto-generate mipmaps.
	\return true means system will handle mipmap generation.
	*/
	GraphicsDriverAPI bool GetAutoGenerateMipMaps() const;
private:
	GraphicsDriverAPI virtual bool InitializeTarget();
};

} } // namespaces
