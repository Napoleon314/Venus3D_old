/**********************************************************************
 *<
	FILE: hitdata.h
				  
	DESCRIPTION:  Defines the basic Hit Data class

	CREATED BY: Dan Silva

	HISTORY: created 9 September 1994

 *>	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/

#pragma once
#include "maxheap.h"

/*! \brief This class is used during sub-object hit testing to identify sub-object
components. 
If the 4 bytes available in the <b>info</b> data member of
<b>HitRecord</b> are insufficient to identify the sub-object component an
instance of this class can be created to contain the necessary data.
Note, a plugin should derive from this class. Then an instance of the 
derived class is passed to the system, which then will be responsible for 
owning and freeing the memory. Therefore do not free the memory for this instance
after passing it to the system. 
\sa  Class HitRecord. */
class HitData: public MaxHeapOperators {
	public:
		/*! \remarks Implemented by the Plug-In.\n\n
		The virtual destructor allows the instance of <b>HitData</b> to be
		deleted when the <b>HitRecord</b> that points to it is deleted. */
		virtual ~HitData() {}
	}; 

