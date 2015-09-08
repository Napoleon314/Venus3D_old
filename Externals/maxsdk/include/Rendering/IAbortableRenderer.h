//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2014 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

namespace MaxSDK 
{;

//==================================================================================================
// class IAbortableRenderer
//
//! \brief An interface that may be implemented by ActiveShade renderers to enable asynchronous aborts.
/*! This interface should be implemented by a class which derives from class IInteractiveRender. */
class IAbortableRenderer 
{
public:

    /*! This method is called to abort the render operation. The implementation should stop the rendering process as quickly as possible
        and return only once the render has actually stopped rendering. */
	virtual void AbortRender() = 0;
};

}
