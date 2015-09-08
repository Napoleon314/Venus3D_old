// Copyright 2013 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../IRefObject.h"
#include "../../strclass.h"

namespace MaxSDK { namespace Graphics {

/** This class is a representation of a basic view.
 */
class IView : public ARefObject
{
public:
	/** Try to get a view attachment from the view
	\param[in] name the name of this attachment
	\return the attachment of the name if it exists or NULL otherwise .
	*/
	virtual MaxSDK::Graphics::RefObjectPtr GetViewAttachment(const MSTR& name) = 0;

	/** Assign a view attachment with a given name to the view
	\param[in] name the name of this attachment
	\param[in] pAttachment the attachment to be assigned
	*/
	virtual void SetViewAttachment(const MSTR& name, 
		const MaxSDK::Graphics::RefObjectPtr& pAttachment) = 0;
};

} }// namespaces
