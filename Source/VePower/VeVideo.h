////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeVideo.h
//  Created:     2015/07/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeVideoDevice : public VeRefObject
{
public:

	inline const VeChar8* GetName() const noexcept;

	inline const VeChar8* GetDesc() const noexcept;

	virtual void Init() noexcept = 0;

	virtual void Term() noexcept = 0;

	virtual void GetDisplayBounds(VeRect* pkRect, VeVideoDisplay* pkDisplay) noexcept = 0;

	virtual void GetDisplayModes(VeVideoDisplay* pkDisplay) noexcept = 0;

	virtual bool SetDisplayMode(VeVideoDisplay* pkDisplay, VeDisplayMode* pkMode) noexcept = 0;


protected:
	VeVideoDevice() noexcept {}

	virtual ~VeVideoDevice() noexcept {}

	VeFixedString m_kName;
	VeFixedString m_kDesc;
	VeVector<VeVideoDisplay> m_kDisplayList;


};

VeSmartPointer(VeVideoDevice);

#include "VeVideo.inl"
