////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeResource.h
//  Created:     2015/08/23 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeResource : public VeRefObject
{
	VeNoCopy(VeResource);
	VeRTTIDecl(VeResource);
public:
	enum State
	{
		UNLOADED,
		LOADING,
		LOADED,
		INVALID
	};

	VeResource() noexcept;

	virtual ~VeResource() noexcept;

	inline const VeChar8* GetName() const noexcept;

	inline const VeChar8* GetFullName() const noexcept;

	inline const VeChar8* GetGroupName() const noexcept;

	inline State GetState() const noexcept;

	void Enter() noexcept;

	void Leave() noexcept;

	virtual const VeChar8* GetTypeName() const noexcept = 0;

protected:
	VeRefNode<VeResource*> m_kNode;
	VeFixedString m_kName;
	VeFixedString m_kFullName;
	VeResourceGroupPtr m_spGroup;
	State m_eState = UNLOADED;
	VeUInt32 m_u32WaitNumber = 0;
	VeList<VeResource*> m_kWaitList;
	VeUInt32 m_u32FreeTime = 0;
	VeUInt32 m_u32EnterCount = 0;

};

VeSmartPointer(VeResource);

#include "VeResource.inl"
