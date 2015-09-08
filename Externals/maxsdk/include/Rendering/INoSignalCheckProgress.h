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
// class INoSignalCheckProgress
//
//! \brief An alternative progress reporting interface to class RendProgressCallback, which does not process the window message queue.
/*! Production rendering typically reports progress through RendProgressCallback::Progress(), a method which processes the window message queue in
    order to update the progress bar and determine wheter the user has pressed the cancel button. This behaviour is sometimes undesirable. When
    updating a scene for ActiveShade, processing the window message queue while reporting progress may cause user interactions to be processed as
    well - and this may interfere with the scene update. In such cases, this interface should be used.

    To retrieve a pointer to this interface, perform a dynamic_cast on a RendProgressCallback:
    \par 
    \code
        RendProgressCallback* progress_callback = ...;
        INoSignalCheckProgress* no_signals_progress_callback = dynamic_cast<INoSignalCheckProgress*>(progress_callback);
    \endcode 
*/
class INoSignalCheckProgress
{
public:

    /*! Reports a progress update.
    \param done The number of steps which are currently done.
    \param total The total number of steps to be perform. */
	virtual void UpdateProgress(int done, int total) = 0;
};

}