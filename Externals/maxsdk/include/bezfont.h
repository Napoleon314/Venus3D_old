//**************************************************************************/
// Copyright (c) 1998-2006 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// FILE:        bezfont.h
// DESCRIPTION: Bezier Font Support methods
// AUTHOR:      Tom Hudson
// HISTORY:     Created 1 November 1995
//**************************************************************************/

#pragma once

#include "maxheap.h"
#include <WTypes.h>
#include "coreexp.h"
#include "strclass.h"

// Forward references
class BezFontManager;
class ClassDesc;
class DllDir;
class BezierShape;

class BezFontMetrics: public MaxHeapOperators {
    public:
	   	LONG   Height;
	    LONG   Ascent;
	    LONG   Descent;
	    LONG   InternalLeading;
	    LONG   ExternalLeading;
	    LONG   AveCharWidth;
	    LONG   MaxCharWidth;
	    LONG   Weight;
	    LONG   Overhang;
	    LONG   DigitizedAspectX;
	    LONG   DigitizedAspectY;
	    BCHAR  FirstChar;
	    BCHAR  LastChar;
	    BCHAR  DefaultChar;
	    BCHAR  BreakChar;
	    BYTE   Italic;
	    BYTE   Underlined;
	    BYTE   StruckOut;
	    BYTE   PitchAndFamily;
	    BYTE   CharSet;
	    DWORD  Flags;
	    UINT   SizeEM;
	    UINT   CellHeight;
	    UINT   AvgWidth;
		CoreExport BezFontMetrics() {}	// To Do; Fill in fields with reasonable values
		CoreExport BezFontMetrics(NEWTEXTMETRIC *from);
	};

// BezFontInfo type
#define BEZFONT_TRUETYPE	0
#define BEZFONT_OTHER		1

// BezFontInfo flags
// None currently defined

class BezFontInfo: public MaxHeapOperators {
	public:
		MSTR name;
		MSTR style;
		int type;		// See above
		DWORD flags;	// See above
		BezFontMetrics metrics;
		BezFontInfo() {}
		BezFontInfo(MSTR n, MSTR s, int t, DWORD f, BezFontMetrics &m) { name=n; style=s; type=t; flags=f; metrics=m; }
		CoreExport BezFontInfo &operator=(BezFontInfo &from);
	};

/**
 * A class for Dlls to use for info that will be sent back to them at load time.
 */
class DllData: public MaxHeapOperators {
	public:
		DllData() {}
	};

/**
 * This is a callback class which is used to process font enumerations
 */
class BezFontEnumProc: public MaxHeapOperators {
	public:
		virtual ~BezFontEnumProc() {}
		virtual BOOL CALLBACK Entry(BezFontInfo &info, LPARAM userInfo)=0;
	};

/**
 * A special enumerator for the font manager
 */
class BezFontMgrEnumProc: public MaxHeapOperators {
	public:
		BezFontManager *mgr;
		CoreExport BOOL CALLBACK Entry(BezFontInfo &info, LPARAM userInfo, DllData *dllData);
		void SetMgr(BezFontManager *m) { mgr = m; }
	};

/**
 * A class for listing font input dlls
 */
class BezFontDll: public MaxHeapOperators {
	public:
		ClassDesc* dll;
		BezFontDll() { dll = NULL; }
		BezFontDll(ClassDesc* d) { dll = d; }
	};

typedef Tab<BezFontDll *> BezFontDllTab;


/**
 * A class for providing access to required 3ds Max internals
 */
class FontMgrInterface: public MaxHeapOperators {
     public:
		virtual ~FontMgrInterface() {}
		virtual HINSTANCE	AppInst() = 0;
		virtual HWND		AppWnd() = 0;
		virtual DllDir*		AppDllDir() = 0;
    	virtual int			GetFontDirCount() = 0;
		virtual const MCHAR* GetFontDir (int i) = 0;
	};

typedef int BEZFONTHANDLE;

/**
 * A class used for listing the fonts we currently have available
 */
class AvailableFont: public MaxHeapOperators {
	public:
		BezFontInfo info;
		DllData *dllData;
		INT_PTR dllIndex;	// The index of the DLL which provides this font (in BezFontDllTab)
		AvailableFont() { dllData = NULL; }
		AvailableFont(BezFontInfo &i, INT_PTR di, DllData *dd=NULL) { info=i; dllIndex=di; dllData=dd; }
		CoreExport ~AvailableFont();
	};

typedef Tab<AvailableFont *> AvailableFontTab;


/**
 * This class is the basic bezier font class.
 * It declares a common interface to all implementation
 */
class BezFont: public MaxHeapOperators {
	public:
		CoreExport BezFont() { }
		CoreExport virtual ~BezFont();
		CoreExport virtual void EnumerateFonts(BezFontMgrEnumProc& proc, LPARAM userInfo)=0;
		CoreExport virtual int OpenFont(MSTR name, DWORD flags, DllData *dllData)=0;
		CoreExport virtual void CloseFont()=0;
		CoreExport virtual BOOL BuildCharacter(UINT index, float height, BezierShape& shape, float& width, int fontShapeVersion=1)=0;
	};

/**
 * This class extends BezFont class
 * It adds GetFont() which returns the font handle currently opened.
 */
class BezFontEX: public BezFont{
	public:
		/** 
		 * Get the handle to a logical font currently opened.
		 * \return HFONT, handle to a logical font
		 */
		CoreExport virtual HFONT GetFont() = 0;
};

/**
 * This class extends the BezFontEX class which extends the function BuildCharacter() 
 * to obtain glyph outline data from a character based on its TrueType glyph index.
 */
class BezFontEX2: public BezFontEX
{
	public:
		/** 
		 * Build the character shape.
		 * \param index TrueType Glyph Index of a character if ggo_native is true, or a character code if ggo_native is false
		 * \param height the request height
		 * \param width the width to return
		 * \param shape the built BezierShape result of the chatracter
		 * \param ggo_native if true, build character shape of glyph outline data on TrueType Glyph Index, otherwise on a character code
		 * \return BOOL, TRUE on success, FALSE on failure
		 */
		CoreExport virtual BOOL BuildCharacterEx(UINT index, float height, BezierShape& shape, float& width, int fontShapeVersion=1, bool ggo_native= false) = 0;
};

/**
 * A class used to list the fonts currently open
 */
class OpenBezFont: public MaxHeapOperators {
	public:
		//! The index in AvailableFont
		int index;
		//! The style flags
		DWORD flags;
		//! The handle we know the font by
		BEZFONTHANDLE handle;
		//! The number of users
		int count;
		//! The loader for the font
		BezFont* loader;
		OpenBezFont() {}
		OpenBezFont(int i, DWORD f, BEZFONTHANDLE h, BezFont *l) { index=i; flags=f; handle=h; count=1; loader=l; }
		CoreExport ~OpenBezFont();
	};

typedef Tab<OpenBezFont *> OpenBezFontTab;

/** \brief Manages Bezier Fonts in 3ds Max.
 *  This is the interface into Bezier fonts within the 3ds Max.
 *  This includes TrueType fonts and any other fonts supported via
 *  plugins.  */
class BezFontManager: public MaxHeapOperators
{
	friend class BezFontMgrEnumProc;

private:
	FontMgrInterface* iface;
	BOOL initialized;
	BezFontDllTab dllTab;
	AvailableFontTab available;
	OpenBezFontTab open;
	//! Enumerator to get available fonts
	BezFontMgrEnumProc enumProc;
public:
	CoreExport BezFontManager();
	CoreExport ~BezFontManager();
	CoreExport void SetInterface(FontMgrInterface* i) { iface = i; }
	CoreExport void Init();
	CoreExport void Uninit();
	CoreExport void Reinit();
	CoreExport void EnumerateFonts(BezFontEnumProc& proc, LPARAM userInfo);
	//! Returns TRUE if the font is available, FALSE otherwise
	CoreExport BOOL FontExists(MSTR name);
	CoreExport BEZFONTHANDLE OpenFont(MSTR name, DWORD flags);
	//! Returns TRUE if the font is still in use, FALSE otherwise
	CoreExport BOOL CloseFont(BEZFONTHANDLE handle);
	//! \deprecated in 3ds Max 2014. Use BuildCharacterEx instead.
	CoreExport BOOL BuildCharacter(BEZFONTHANDLE handle, UINT index, float height, BezierShape& shape, float &width, int fontShapeVersion=1);
	/*!	Build the given bezier from of the character in the given shape, and send back
		its relative width. 
		\sa class BezFontEX2
		\param handle - The handle
		\param index - The index
		\param height - The height
		\param shape - The bezier shape
		\param width - The out parameter containing the width of the character
		\param fontShapeVersion - The version. Default value of 1
		\param ggo_native -if true, build character shape of glyph outline data on TrueType Glyph Index, otherwise on a character code
		\return Returns FALSE and zero width if character not available. */
	CoreExport BOOL BuildCharacterEx(BEZFONTHANDLE handle, UINT index, float height, BezierShape& shape, float& width, int fontShapeVersion=1, bool ggo_native = false);
	CoreExport FontMgrInterface* GetInterface() { return iface; }
	CoreExport BOOL GetFontInfo(MSTR name, BezFontInfo &info);
	CoreExport HFONT GetFont(BEZFONTHANDLE handle);
};

//! The single instance of the BezFontManager class
extern CoreExport BezFontManager theBezFontManager;

