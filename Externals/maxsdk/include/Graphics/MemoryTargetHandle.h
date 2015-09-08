//
// Copyright 2014 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "RenderEnums.h"
#include "BaseRasterHandle.h"

namespace MaxSDK { namespace Graphics {

/** Application can use this class to create a render target in memory which can
	be a target input/output for a fragment node. 
*/
class MemoryTargetHandle : public BaseRasterHandle
{
public:
	GraphicsDriverAPI MemoryTargetHandle();
	GraphicsDriverAPI MemoryTargetHandle(const MemoryTargetHandle& from);
	GraphicsDriverAPI MemoryTargetHandle& operator = (const MemoryTargetHandle& from);
	GraphicsDriverAPI virtual ~MemoryTargetHandle();

	GraphicsDriverAPI static Class_ID ClassID();

private:
	virtual bool InitializeTarget();
};

}}