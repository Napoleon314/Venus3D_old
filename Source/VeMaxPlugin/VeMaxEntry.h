////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   VeMaxEntry.h
//  Created:     2015/09/08 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#define VE_MAX_EXPORT __declspec(dllexport)
#define VE_MAX_LIB_DESC _T("VenusMaxPlugin")

extern HINSTANCE g_hInstance;

class VeMaxEntry : public VeSingleton<VeMaxEntry>
{
public:
	VeMaxEntry() noexcept;

	~VeMaxEntry() noexcept;

	void Init() noexcept;

	void Term() noexcept;

	VeInt32 GetClassNum() noexcept;

	ClassDesc* GetClass(VeInt32 i32Index) noexcept;

protected:
	VeVector<ClassDesc*> m_kClassList;

};

#define ve_max_ent VeMaxEntry::GetSingleton()
