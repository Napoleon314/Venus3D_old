//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "Types.h"
#include "../MaxHeap.h"

namespace MaxSDK { namespace Graphics {

class IDisposable : public MaxHeapOperators
{
public:
	IDisposable() {
		mIsDisposed = false;
	}
	virtual ~IDisposable() {}
	/* Dispose internal resource
	*/
	void Dispose() {
		mIsDisposed = true;
	}
	/* Check if the resource is disposed
	\return true if disposed, otherwise false.
	*/
	bool IsDisposed() const {
		return mIsDisposed;
	}

private:
	bool mIsDisposed;
};

} } // namespaces
