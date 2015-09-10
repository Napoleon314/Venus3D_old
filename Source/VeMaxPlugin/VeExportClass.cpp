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
	return _T("VMESH");
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

//--------------------------------------------------------------------------
int VeExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i,
	BOOL suppressPrompts, DWORD options)
{
	VeFixedString kPath = ToStr(name);
	VE_BOOL bRes = VE_FALSE;
	if (scence_loader.Load(VE_MASK_HAS_ANY(options, SCENE_EXPORT_SELECTED)))
	{
		bRes = VE_TRUE;
	}
	return bRes;
}
//--------------------------------------------------------------------------
void VeExport::Parse(IGameMesh* pkMesh) noexcept
{
	//m_kMesh.Reset();
	/*bool bHasNormal = false;
	{
		Mesh* pkMaxMesh = pkMesh->GetMaxMesh();
		if (pkMaxMesh && pkMaxMesh->normalCount > 0)
			bHasNormal = true;
	}

	VeInt32 i32TangentMapChannel = -1;
	{
		for (VeInt32 i32MapChannel(-2); i32MapChannel <= 99; ++i32MapChannel)
		{
			if (pkMesh->GetNumberOfTangents(i32MapChannel) > 0)
			{
				i32TangentMapChannel = i32MapChannel;
				break;
			}
		}
	}

	int faceTotalCount = 0;
	auto matIDs = pkMesh->GetActiveMatIDs();
	int matIDCount = matIDs.Count();
	for (int matIdx = 0; matIdx < matIDCount; ++matIdx)
	{
		int matID = matIDs[matIdx];
		auto faces = pkMesh->GetFacesFromMatID(matID);
		int numFaces = faces.Count();
		faceTotalCount += numFaces;
		assert(numFaces > 0);

		FaceEx* face = faces[0];


	}*/
}

//--------------------------------------------------------------------------
VeExportClassDesc VeExportClassDesc::ms_kInstance;
//--------------------------------------------------------------------------
