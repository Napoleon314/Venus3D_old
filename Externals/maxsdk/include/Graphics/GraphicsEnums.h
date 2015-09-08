//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

namespace MaxSDK { namespace Graphics {

	/** Enum type for all visual styles of nitrous viewport
	*/
	enum VisualStyle
	{
		/** The realistic visual style
		*/
		VisualStyleRealistic = 0,
		/** The shaded visual style
		*/
		VisualStyleShaded,
		/** The facet visual style
		*/
		VisualStyleFacets,
		/** The consistent color visual style
		*/
		VisualStyleConsistentColor,
		/** The hidden line visual style
		*/
		VisualStyleHiddenLine,
		/** The wireframe visual style
		*/
		VisualStyleWireframe,
		/** The bounding box visual style
		*/
		VisualStyleBoundingBox,
		/** The ink visual style for NPR
		*/
		VisualStyleInk,
		/** The color ink visual style of NPR
		*/
		VisualStyleColorInk,
		/** The acrylic stylized visual style
		*/
		VisualStyleAcrylic,
		/** The tech stylized visual style
		*/
		VisualStyleTech,
		/** The graphite stylized visual style
		*/
		VisualStyleGraphite,
		/** The color pencil stylized visual style
		*/
		VisualStyleColorPencil,
		/** The pastel stylized visual style
		*/
		VisualStylePastel,
		/** The clay visual style
		*/
		VisualStyleClay,
		/** The count of all visual styles
		*/
		VisualStyleCount,
	};

} } // end namespace

