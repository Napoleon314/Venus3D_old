////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   WindowsMouse.h
//  Created:     2015/08/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class WindowsMouse : public VeMouse
{
	VeNoCopy(WindowsMouse);
public:
	WindowsMouse() noexcept;

	virtual ~WindowsMouse() noexcept;

	virtual void _Init() noexcept override;

	virtual void _Term() noexcept override;

	virtual bool _CreateCursor(VeCursor::Data* pkCur,
		const VeSurfacePtr& spSurface,
		int32_t i32HotX, int32_t i32HotY) noexcept override;

	virtual bool _CreateSystemCursor(VeCursor::Data* pkCur,
		VeSystemCursor eCursor) noexcept override;

	virtual void _ShowCursor(VeCursor::Data* pkCur) noexcept override;

	virtual void _FreeCursor(VeCursor::Data* pkCur) noexcept override;

	virtual void _Warp(VeWindow::Data* pkWindow, int32_t x, int32_t y) noexcept override;

	virtual bool _SetRelativeMode(bool bEnabled) noexcept override;

	friend class WindowsVideoDevice;
	HCURSOR m_hCursor = nullptr;
	

};
