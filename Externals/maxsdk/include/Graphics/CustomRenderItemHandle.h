//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once
#include "../Graphics/RenderItemHandle.h"
#include "ICustomRenderItem.h"

namespace MaxSDK { namespace Graphics {

/** CustomRenderItemHandle is a wrapper of ICustomRenderItem. It can be used to add a custom implementation
into the Max graphics system without directly deriving from RenderItemHandle.
*/
class CustomRenderItemHandle : public RenderItemHandle
{
public:
	GraphicsDriverAPI CustomRenderItemHandle();
	GraphicsDriverAPI CustomRenderItemHandle(const CustomRenderItemHandle& from);
	GraphicsDriverAPI CustomRenderItemHandle& operator = (const CustomRenderItemHandle& from);
	GraphicsDriverAPI virtual ~CustomRenderItemHandle();

	/**Initialize this handle. The handle is empty after initialization. It doesn't have
	a default implementation.
	*/
	GraphicsDriverAPI void Initialize();

	/**Sets an ICustomRenderItem object to this handle.
	\param[in] impl A user-created class that derives from the ICustomRenderItem interface.
	*/
	GraphicsDriverAPI void SetCustomImplementation(ICustomRenderItem* impl);

	/**Gets the ICustomRenderItem object in this handle.
	\return The ICustomRenderItem object, or nullptr if there is no implementation.
	*/
	GraphicsDriverAPI ICustomRenderItem* GetCustomeImplementation() const;
};

} } // end namespace
