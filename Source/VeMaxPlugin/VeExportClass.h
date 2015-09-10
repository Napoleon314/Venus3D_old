////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   VeExportClass.h
//  Created:     2015/09/08 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VeExport : public SceneExport
{
public:
	VeExport();

	virtual ~VeExport();

	virtual int ExtCount();
	virtual const TCHAR * Ext(int n);
	virtual const TCHAR * LongDesc();
	virtual const TCHAR * ShortDesc();
	virtual const TCHAR * AuthorName();
	virtual const TCHAR * CopyrightMessage();
	virtual const TCHAR * OtherMessage1();
	virtual const TCHAR * OtherMessage2();
	virtual unsigned int Version();
	virtual void ShowAbout(HWND hWnd);

	virtual BOOL SupportsOptions(int ext, DWORD options);
	virtual int	DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0);

	void Parse(IGameMesh* pkMesh) noexcept;

	//VeMaxMesh m_kMesh;

};

#define VE_EXPORT_CLASS_ID Class_ID(0x4a984915, 0x6d317c5a)

class VeExportClassDesc : public ClassDesc2
{
public:
	virtual int IsPublic() { return TRUE; }
	virtual void* Create(BOOL) { return new VeExport(); }
	virtual const TCHAR *	ClassName() { return _T("VeExport"); }
	virtual SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() { return VE_EXPORT_CLASS_ID; }
	virtual const TCHAR* Category() { return _T("Export Category"); }
	virtual const TCHAR* InternalName() { return _T("VeExport"); }
	virtual HINSTANCE HInstance() { return g_hInstance; }
	static ClassDesc2* Get() { return &ms_kInstance; }

protected:
	static VeExportClassDesc ms_kInstance;

};
