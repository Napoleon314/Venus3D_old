////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      VeRenderer
//  File name:   VeRenderWindow.h
//  Created:     2016/07/22 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#pragma once

class VENUS_API VeRenderWindow : public VeRefObject
{
	VeNoCopy(VeRenderWindow);
	VeRTTIDecl(VeRenderWindow);
public:
	VeRenderWindow(const VeWindowPtr& spWindow) noexcept;

	virtual ~VeRenderWindow() noexcept;

	inline const VeWindowPtr& GetTargetWindow() noexcept;

	inline uint32_t GetID() const noexcept;

	inline uint32_t GetFlags() const noexcept;

	inline const char* GetTitle() const noexcept;

	inline int32_t GetPosX() const noexcept;

	inline int32_t GetPosY() const noexcept;

	inline int32_t GetWidth() const noexcept;

	inline int32_t GetHeight() const noexcept;

	inline int32_t GetMinimumWidth() const noexcept;

	inline int32_t GetMinimumHeight() const noexcept;

	inline int32_t GetMaximumWidth() const noexcept;

	inline int32_t GetMaximumHeight() const noexcept;

	inline float GetBrightness() const noexcept;

	inline void SetTitle(const char* pcTitle) noexcept;

	inline void SetPosition(int32_t x, int32_t y) noexcept;

	inline void SetSize(int32_t w, int32_t h) noexcept;

	inline void SetMinimumSize(int32_t min_w, int32_t min_h) noexcept;

	inline void SetMaximumSize(int32_t max_w, int32_t max_h) noexcept;

	inline void Show() noexcept;

	inline void Hide() noexcept;

	inline void Raise() noexcept;

	inline void Maximize() noexcept;

	inline void Minimize() noexcept;

	inline void Restore() noexcept;

	inline void SetBordered(bool bBordered) noexcept;

	inline void SetFullscreen(uint32_t u32Flags) noexcept;

	inline bool SetBrightness(float f32Brightness) noexcept;

	inline void SetGrab(bool bGrabbed) noexcept;

	inline bool IsSync() noexcept;

	inline void SetSync(bool bEnable) noexcept;

	inline uint64_t GetFrameIndex() noexcept;

	virtual bool IsValid() noexcept;

	virtual void Begin() noexcept = 0;

	virtual void End() noexcept = 0;

protected:
	friend class VeRenderer;
	VeWindowPtr m_spTargetWindow;
	VE_BOOL m_bNeedDestory = VE_FALSE;
	VE_BOOL m_bSync = VE_FALSE;
	uint64_t m_u64FrameIndex = 0;

};

VeSmartPointer(VeRenderWindow);

#include "VeRenderWindow.inl"
