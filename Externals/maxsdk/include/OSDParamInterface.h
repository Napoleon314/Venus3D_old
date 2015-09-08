//**************************************************************************/
// Copyright (c) 2014 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/

#pragma once

/** \file OSDParamInterface.h

		This header file provides an interface into objects supporting Pixar's OpenSubdiv subdivsion technology,
		in order to access parameters controlling OSD subdivision.
*/

#define OSD_PARAMETER_INTERFACE Interface_ID(0x146d13c3, 0x1b310489)

/** \class OpenSubdivParameters

	If a renderer or other plugin wants to determine if an object is using Pixar's OpenSubdiv to perform
	subdivision, it can use the GetInterface method to query the INode for the OSD_PARAMETER_INTERFACE
	interface.  If the node contains an object that uses OpenSubdiv, it will return a pointer to this
	interface class, otherwise it will return NULL.

	Once you have a valid pointer, you can use this class to retrieve the parameters for OpenSubdiv's
	subdivision operations for that node.  

*/

class OpenSubdivParameters : public BaseInterface {
public:
	/** \enum VertexBoundary
		These enums describe how the geometery will be subdivided by OpenSubdiv.
		They match the parameter values OpenSubdiv uses internally.
	*/
	enum VertexBoundary {
		VBOUND_INTERP_EDGES=0,				
		VBOUND_INTERP_EDGES_CORNERS
	};
	/** \enum VaryingBoundary
		These enums describe how the varying data ie UV maps etc. will be subdivided by OpenSubdiv.
		They match the parameter values OpenSubdiv uses internally.
	*/
	enum VaryingBoundary {
		FBOUND_BILINEAR=0,
		FBOUND_SMOOTH_EDGE,
		FBOUND_SMOOTH_EDGE_CORNER,
		FBOUND_SMOOTH_SHARP,
	};
	/** \enum CreaseMode
		These enums describe how creases are handled by OpenSubdiv.
		They match the parameter values OpenSubdiv uses internally.
	*/
	enum CreaseMode {
		CREASE_NORMAL=0,
		CREASE_CHAIKIN
	};

	OpenSubdivParameters() {}
	virtual ~OpenSubdivParameters() {}

	/** Get the tessellation level of this object.
		\param t - The time at which to sample
		\param valid - The validity interval to update based on the validity of the tessellation level
		\return For non-adaptive subdivision, this is the number of iterations.  For adaptive
		subdivision, this is the secondary tessellation level.
	*/
	virtual unsigned int TessellationLevel(TimeValue t, Interval &valid) = 0;

	/** Get the OpenSubdiv Vertex Boundary parameter
		\return The OpenSubdiv VertexBoundary type to be used; see above enums
	*/
	virtual OpenSubdivParameters::VertexBoundary InterpolateBoundaryVert() = 0;

	/** Get the OpenSubdiv Varying Boundary parameter
		\return The OpenSubdiv VaryingBoundary type to be used; see above enums
	*/
	virtual OpenSubdivParameters::VaryingBoundary InterpolateBoundaryFVar() = 0;

	/** Get the OpenSubdiv Crease Mode parameter
		\return The OpenSubdiv CreaseMode type to be used; see above enums
	*/
	virtual OpenSubdivParameters::CreaseMode CreaseMethod() = 0;

	/** Get the OpenSubdiv Smooth Triangle parameter
		\return true if OpenSubdiv is to smooth triangles in its processing
	*/
	virtual bool SmoothTriangle() = 0;

	/** Get the OpenSubdiv Propagate Corners parameter
		\return true if OpenSubdiv is to propagate corners in its processing
	*/
	virtual bool PropagateCorners() = 0;

	/** \fn Adaptive
		\brief Get the OpenSubdiv Adaptive parameter
		\return true if OpenSubdiv is to use adaptive subdivision in its processing
	*/
	virtual bool Adaptive() = 0;

	/** Get the OpenSubdiv Adaptive Tessellation parameter
		\param t - The time at which to sample
		\param valid - The validity interval to update based on the validity of the adaptive tessellation level
		\return The maximum number of iterations for adaptive tessellation
	*/
	virtual unsigned int AdaptiveTessellationLevel(TimeValue t, Interval &valid) = 0;
};

