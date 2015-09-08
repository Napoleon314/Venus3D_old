//**************************************************************************/
// Copyright (c) 1998-2011 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/

#pragma once

#include "maxtypes.h"
#include "point3.h"

namespace MaxSDK 
{
/**
	The class used to compute vertex normals considering smoothing.
*/
class GEOMEXPORT VertexNormal : public MaxHeapOperators
{
public:
	Point3 norm;             ///< The normal.
	DWORD smooth;            ///< The smoothing groups.
	VertexNormal* next;      ///< A pointer to the next normal -- this class is a linked list.
	BOOL init;               ///< The init variable is used as a flag to indicate if the first normal in the list has been initialized.

	/** Default Constructor */
	VertexNormal();
	
	/** Constructor.
	\param n - The vertex normal vector
	\param s - The ID of the smoothing group
	*/
	VertexNormal(Point3& n,DWORD s);
	
	/** Copy Constructor */
	VertexNormal(const VertexNormal& copyVNormal) ;
	
	/** Assignment operator */
	VertexNormal& operator=(const VertexNormal& from);
	
	/** Destructor */
	virtual ~VertexNormal();

	/** Add a normal to the list if the smoothing group bits overlap, otherwise create a new vertex normal in the list.
	It is used when a face is going to add its normal to a vertex. 
	This method is passed the normal and the smoothing information for that face. 
	It checks if the normal passed shares smoothing information with the existing normal. 
	If it does, the normal is added in, and the smoothing bits are bitwise OR-ed in. 
	If it does not, a new vertex normal is created. 
	\param n The new normal to be added.
	\param s The smoothing group information.
	*/
	void AddNormal(Point3 &n,DWORD s);

	/** Retrieves a normal if the smoothing groups overlap or there is
	    only one in the list
	\param s The smoothing group information.
	\return The Normal
	*/
	Point3& GetNormal(DWORD s);

	/** Normalize each normal in the list
	*/
	void Normalize();
private:

	void internalCopy( const VertexNormal& from );
};

}