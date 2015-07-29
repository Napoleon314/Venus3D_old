////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   WindowsVideo.h
//  Created:     2015/07/27 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

struct VeDisplayData : public VeRefObject
{
	VeDisplayData() noexcept
	{
		VeZero(DeviceName);
	}

	CHAR DeviceName[32];
} ;

struct VeDisplayModeData : public VeRefObject
{
	DEVMODEA DeviceMode;
	VeFloat32 ScaleX;
	VeFloat32 ScaleY;
};

class WindowsVideoDevice : public VeVideoDevice
{
public:
	WindowsVideoDevice() noexcept;

	virtual ~WindowsVideoDevice() noexcept;

	virtual void Init() noexcept;

	virtual void Term() noexcept;

	virtual void GetDisplayBounds(VeRect* pkRect, VeVideoDisplay* pkDisplay) noexcept;

	virtual void GetDisplayModes(VeVideoDisplay* pkDisplay) noexcept;

	virtual bool SetDisplayMode(VeVideoDisplay* pkDisplay, VeDisplayMode* pkMode) noexcept;

protected:
	bool AddDisplay(const VeChar8* pcDeviceName) noexcept;

	void InitModes() noexcept;

	void TermModes() noexcept;

};
