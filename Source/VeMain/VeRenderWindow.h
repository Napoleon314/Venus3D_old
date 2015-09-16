////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderWindow.h
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeRenderWindow : public VeRefObject
{
	VeNoCopy(VeRenderWindow);
	VeRTTIDecl(VeRenderWindow);
public:
	inline const VeWindowPtr& GetTargetWindow() noexcept;

	inline VeUInt32 GetID() const noexcept;

	inline VeUInt32 GetFlags() const noexcept;

	inline const VeChar8* GetTitle() const noexcept;

	inline VeInt32 GetPosX() const noexcept;

	inline VeInt32 GetPosY() const noexcept;

	inline VeInt32 GetWidth() const noexcept;

	inline VeInt32 GetHeight() const noexcept;

	inline VeInt32 GetMinimumWidth() const noexcept;

	inline VeInt32 GetMinimumHeight() const noexcept;

	inline VeInt32 GetMaximumWidth() const noexcept;

	inline VeInt32 GetMaximumHeight() const noexcept;

	inline VeFloat32 GetBrightness() const noexcept;

	inline void SetTitle(const VeChar8* pcTitle) noexcept;

	inline void SetPosition(VeInt32 x, VeInt32 y) noexcept;

	inline void SetSize(VeInt32 w, VeInt32 h) noexcept;

	inline void SetMinimumSize(VeInt32 min_w, VeInt32 min_h) noexcept;

	inline void SetMaximumSize(VeInt32 max_w, VeInt32 max_h) noexcept;

	inline void Show() noexcept;

	inline void Hide() noexcept;

	inline void Raise() noexcept;

	inline void Maximize() noexcept;

	inline void Minimize() noexcept;

	inline void Restore() noexcept;

	inline void SetBordered(bool bBordered) noexcept;

	inline void SetFullscreen(VeUInt32 u32Flags) noexcept;

	inline bool SetBrightness(VeFloat32 f32Brightness) noexcept;

	inline void SetGrab(bool bGrabbed) noexcept;

	virtual bool IsValid() noexcept;

	virtual void SetupCompositorList(const VeChar8* pcHint,
		const VeChar8** ppcList, VeSizeT stNum) noexcept = 0;

	virtual void Update(void* pvCBuffer, void* pvVBuffer) noexcept = 0;

protected:
	friend class VeRenderer;

	VeRenderWindow(const VeWindowPtr& spWindow) noexcept;

	virtual ~VeRenderWindow() noexcept;

	VeWindowPtr m_spTargetWindow;
	bool m_bNeedDestory = false;

};

VeSmartPointer(VeRenderWindow);

#include "VeRenderWindow.inl"
