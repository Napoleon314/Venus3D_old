//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once
#include "GraphicsExport.h"
#include "../MaxHeap.h"

namespace MaxSDK { namespace Graphics {

	/** this class contains an integer which is initialized to zero. One can 
		increase/decrease the value of the integer thread-safely.
	*/
	class AtomicRefCounter : public MaxHeapOperators
	{
	public:
		/** constructor. 
		   integer is initialized to zero.
		*/
		GraphicsUtilityAPI  AtomicRefCounter();
		GraphicsUtilityAPI ~AtomicRefCounter();

		/** increase the integer value by one. 
		\return the increased value.
		*/
		GraphicsUtilityAPI long Increment();
		/** decrease the integer value by one. 
		\return the decreased value.
		*/
		GraphicsUtilityAPI long Decrement();

	private:
		long volatile mReferenceCount;
	};

}} // namespaces

