//**************************************************************************/
// Copyright (c) 1998-2013 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/

/*! \file SimpleRandom.h
    \brief	A random number generator to be used by PFlow operators when
			processing particle channel data in post-cache stage.
			The generator is history-independent; instead it uses
			particle IDs to generate preudo-random numbers
*/

#pragma once
#include <WTypes.h>
#include "../MaxHeap.h"
#include "PFExport.h"

class SimpleRandom: public MaxHeapOperators 
{
	public:
		/*! \remarks Constructor. */
		PFExport SimpleRandom(const int seed); // constructor

		// for a given integer value, the following functions
		// produce the same "random" number on a consistent basis

		/*! \remarks This method returns the random number sign, either -1 or 1. */
		PFExport int	RandSign( const int id ) const;
		/*! \remarks This method return a random number between 0.0f and 1.0f.	*/
		PFExport float	Rand01( const int id ) const;
		/*! \remarks This method return a random number between -1.0f and 1.0f. */
		PFExport float	Rand11( const int id ) const;
		/*! \remarks This method return a random number between -0.5f and 0.5f. */
		PFExport float	Rand55( const int id ) const;
		/*! \remarks This method return a random number between 0 and maxnum. */
		PFExport unsigned int	Rand0X(const unsigned int maxnum, const int id ) const;

	private:
		// intentionally private and undefined
		SimpleRandom(void);
		// intentionally private and undefined
		SimpleRandom& operator=(const SimpleRandom&);

		unsigned int rand( const int id ) const;

		// Warning - instances of this class are saved as a binary blob to scene file
		// Adding/removing members will break file i/o
		static const DWORD32 HALF_RAND_MAX;
		static const double kIntMax, kIntMax1, kHalfIntMax;

		const int m_seed;
};


