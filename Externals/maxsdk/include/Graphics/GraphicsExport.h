//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#ifdef GRAPHICS_DRIVER_EXPORTS
#	define GraphicsDriverAPI __declspec( dllexport )
#else
#	define GraphicsDriverAPI __declspec( dllimport )
#endif

#if defined(GRAPHICS_UTILITY_MODULE)
#	define GraphicsUtilityAPI __declspec(dllexport)
#else
#	define GraphicsUtilityAPI __declspec(dllimport)
#endif

#ifdef MAX_GRAPHICS_OBJECTS_EXPORTS
#	define MaxGraphicsObjectAPI __declspec(dllexport)
#else
#	define MaxGraphicsObjectAPI __declspec(dllimport)
#endif

#ifdef VIEW_SYSTEM_EXPORTS
#	define ViewSystemAPI __declspec(dllexport)
#else
#	define ViewSystemAPI __declspec(dllimport)
#endif

#ifdef DEFAULT_RENDERITEM_EXPORTS
#	define DefaultRenderItemAPI __declspec( dllexport )
#else
#	define DefaultRenderItemAPI __declspec( dllimport )
#endif