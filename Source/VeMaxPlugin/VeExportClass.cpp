////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   VeExportClass.cpp
//  Created:     2015/09/08 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//--------------------------------------------------------------------------
VeExport::VeExport()
{

}
//--------------------------------------------------------------------------
VeExport::~VeExport()
{

}
//--------------------------------------------------------------------------
int VeExport::ExtCount()
{
	return 1;
}
//--------------------------------------------------------------------------
const TCHAR * VeExport::Ext(int n)
{
	return _T("JSON");
}
//--------------------------------------------------------------------------
const TCHAR * VeExport::LongDesc()
{
	return _T("Export for Venus3D");
}
//--------------------------------------------------------------------------
const TCHAR * VeExport::ShortDesc()
{
	return _T("VeExport");
}
//--------------------------------------------------------------------------
const TCHAR * VeExport::AuthorName()
{
	return _T("Napoleon314");
}
//--------------------------------------------------------------------------
const TCHAR * VeExport::CopyrightMessage()
{
	return _T("Copyright (C), Venus Interactive Entertainment.2012");
}
//--------------------------------------------------------------------------
const TCHAR * VeExport::OtherMessage1()
{
	return _T("");
}
//--------------------------------------------------------------------------
const TCHAR * VeExport::OtherMessage2()
{
	return _T("");
}
//--------------------------------------------------------------------------
unsigned int VeExport::Version()
{
	return 100;
}
//--------------------------------------------------------------------------
void VeExport::ShowAbout(HWND hWnd)
{

}
//--------------------------------------------------------------------------
BOOL VeExport::SupportsOptions(int ext, DWORD options)
{
	return TRUE;
}
//--------------------------------------------------------------------------
int VeExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i,
	BOOL suppressPrompts, DWORD options)
{
	return FALSE;
}
//--------------------------------------------------------------------------
VeExportClassDesc VeExportClassDesc::ms_kInstance;
//--------------------------------------------------------------------------
