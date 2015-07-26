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

class WindowsVideoDevice : public VeVideoDevice
{
public:
	WindowsVideoDevice() noexcept;

	virtual ~WindowsVideoDevice() noexcept;

	virtual void Init() noexcept;

	virtual void Term() noexcept;

protected:
	void InitModes() noexcept;

};

