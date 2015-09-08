//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../utilexp.h"
#include "../maxapi.h"

namespace MaxSDK { namespace Util{

/*! An interface of bailing out the busy processing functions by pressing 
	the 'Esc' key.
*/

class IBailOutBusyProcessManager : public MaxHeapOperators
{
public:
	/*!	Plug-ins may have functions of long busy process which can't be accepted by users.
		In this condition, users can use this interface to escape the long busy process by pressing ESC key.
		\code
		void LongBusyProcess()
		{
			using MaxSDK::Util;
			...
			if(GetBailOutManager())
			{
				GetBailOutManager()->EmbedBailOut();
			}
			
			for(int i=0; i<100000; i++)
			{
			   if(GetBailOutManager() && GetBailOutManager()->ShouldBail())
			   {
				   MemoryClear(); // here users need to clear the memory manually.
			       break;
			   }
			   ...
			   Process()
			   ...
			}
			if(GetBailOutManager())
			{
				GetBailOutManager()->BailOutEnd();
			}
		}
		\endcode
	*/
	//! Destructor
	virtual ~IBailOutBusyProcessManager(){}

	/*! At the beginning of the busy process, a bail-out can be embed, so that
		you can trigger the bail-out in the busy process to jump out the busy process.
		For example, you can embed bail-out at the front of long time loop.
	*/
	virtual void EmbedBailOut() = 0;

	/*!	After bail-out operation is done, you need to end the bail-out.
	*/
	virtual void BailOutEnd() = 0;

	/*!	To detect whether the bail-out should jump out the busy process or not.
		\return If true bail-out should jump out the busy process.
		If false bail-out should not jump out the busy process.
	*/
	virtual bool ShouldBail() = 0;

	/*! To detect whether the bail-out is done or not.
		\return If true bail-out is done. If false bail-out is missed.
	*/
	virtual bool IsBailOut() const = 0;

	/*! To detect whether the bail-out is embed or not.
		\return If true bail-out is embed. If false bail-out is not embed.
	*/
	virtual bool IsBailOutEmbeded() const = 0;

};

UtilExport IBailOutBusyProcessManager* GetBailOutManager();

}}