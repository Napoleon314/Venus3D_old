//**************************************************************************/
// Copyright (c) 1998-2010 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
#pragma once

#include "CoreExport.h"
#include "MaxHeap.h"
#include "strclass.h"

namespace MaxSDK
{
	/*	This class provides an exception safe way to record undo operations using
		Hold::Accept and Hold::Cancel. It uses the RAII idiom to ensure Hold::Begin
		is called in the constructor, and Hold::Accept in the destructor. 
		In the presence of an exception it will call Hold::Cancel in the 
		destructor.
		\code
		HoldBegin undo_action;
		...
		undo_action.Accept("My undo action");
		\endcode
		Nothing happens in the destructor if Accept is called before destruction of the instance.

		This class should be used instead of using theHold.Begin and theHold.Accept. 
	*/
	class CoreExport HoldBegin : public MaxHeapOperators
	{
	public:
		//! Constructor
		//! \param message - The string to pass to Hold::Accept.
		HoldBegin(const MCHAR* message);

		//! Constructor
		HoldBegin();

		//! Destructor. By default will accept the undo operation. If the stack is unwinding due to an exception
		//! it will cancel the undo operation
		~HoldBegin();

		//! Cancels the hold by calling Hold::Cancel.
		//! Cannot be called after HoldBegin::Accept is called.
		//! Calling this multiple times will have no effect.
		//! After calling this, the instance of this class has fulfilled its
		//! task and nothing more can be done with it.
		void Cancel();

		//! Accepts the hold, and calls Hold::Accept.
		//! Cannot be called after HoldBegin::Cancel is called. 
		//! Calling this multiple times will have no effect. 
		//! After calling this, the instance of this class has fulfilled its
		//! task and nothing more can be done with it.
		void Accept();

		//! Accepts the hold. 
		//! When this is called, mMessage (specified via HoldBegin::HoldBegin(MCHAR*) is ignored.
		//! Cannot be called after HoldBegin::Cancel is called. 
		//! Calling this multiple times will have no effect.
		//! After calling this, the instance of this class has fulfilled its
		//! task and nothing more can be done with it.
		//! \param message - A message to pass to Hold::Accept.
		void Accept(const MCHAR* message);

	private:
		//! Unimplemented copy constructor to prevent copying
		HoldBegin(const HoldBegin&);
		//! Unimplemented assignment operator to prevent copying
		HoldBegin& operator=(const HoldBegin&);
		//! Performs the actual accept
		void DoAccept(const MCHAR* message);
		//! Text to use when accepting the undo transaction. This is only used when specified via the constructor.
		//! when HoldBegin::Accept(MCHAR*) is called, mMessage is ignored.
		MSTR mMessage;
		//! State variable used to ensure that Hold::Accept or Hold::Cancel is only called once.
		bool   mFinished;
	};
}