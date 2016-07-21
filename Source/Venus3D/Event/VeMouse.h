////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Event
//  File name:   VeMouse.h
//  Created:     2016/07/20 by Albert
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

#define VE_BUTTON(X)       (1 << ((X)-1))
#define VE_BUTTON_LEFT     1
#define VE_BUTTON_MIDDLE   2
#define VE_BUTTON_RIGHT    3
#define VE_BUTTON_X1       4
#define VE_BUTTON_X2       5
#define VE_BUTTON_LMASK    VE_BUTTON(VE_BUTTON_LEFT)
#define VE_BUTTON_MMASK    VE_BUTTON(VE_BUTTON_MIDDLE)
#define VE_BUTTON_RMASK    VE_BUTTON(VE_BUTTON_RIGHT)
#define VE_BUTTON_X1MASK   VE_BUTTON(VE_BUTTON_X1)
#define VE_BUTTON_X2MASK   VE_BUTTON(VE_BUTTON_X2)

class VENUS_API VeCursor : public VeRefObject
{
public:
	struct Data
	{
		vtd::intrusive_node<Data*> m_kNode;
		void* m_pvDriverdata = nullptr;
	};

	VeCursor() noexcept;

	virtual ~VeCursor() noexcept;

	static VeCursor* Cast(VeCursor::Data* pkData) noexcept;

protected:
	friend class VeMouse;

	Data m_kData;

};

VeSmartPointer(VeCursor);

struct VeMouseClickState
{
	int32_t m_i32LastX = 0;
	int32_t m_i32LastY = 0;
	uint32_t m_u32LastTimeStamp = 0;
	uint8_t m_u8ClickCount = 0;
};

typedef uint32_t VeMouseID;

enum VeSystemCursor
{
	VE_SYSTEM_CURSOR_ARROW,
	VE_SYSTEM_CURSOR_IBEAM,
	VE_SYSTEM_CURSOR_WAIT,
	VE_SYSTEM_CURSOR_CROSSHAIR,
	VE_SYSTEM_CURSOR_WAITARROW,
	VE_SYSTEM_CURSOR_SIZENWSE,
	VE_SYSTEM_CURSOR_SIZENESW,
	VE_SYSTEM_CURSOR_SIZEWE,
	VE_SYSTEM_CURSOR_SIZENS,
	VE_SYSTEM_CURSOR_SIZEALL,
	VE_SYSTEM_CURSOR_NO,
	VE_SYSTEM_CURSOR_HAND,
	VE_NUM_SYSTEM_CURSORS
};

class VENUS_API VeMouse : public VeRefObject
{
	VeNoCopy(VeMouse);
public:
	VeMouse() noexcept;

	virtual ~VeMouse() noexcept;

	inline VeWindowPtr GetFocus() noexcept;

	inline uint32_t GetButtonState() noexcept;

	inline uint32_t GetState(int32_t& x, int32_t& y) noexcept;

	inline uint32_t GetRelativeState(int32_t& x, int32_t& y) noexcept;

	inline bool IsRelativeModeEnable() noexcept;

	inline VeCursorPtr GetCursor() noexcept;

	inline VeCursorPtr GetDefaultCursor() noexcept;

	void SetCursor(VeCursor::Data* pkCursor) noexcept;

	void WarpInWindow(VeWindow::Data* pkWindow, int32_t x, int32_t y) noexcept;

	void SetRelativeMode(bool bEnabled) noexcept;

	/*VeCursorPtr CreateCursor(const VeByte* pbyData,
		const Uint8 * mask,
		int w, int h, int hot_x,
		int hot_y);*/

	void ShowCursor(bool bToggle) noexcept;

public:
	void Init() noexcept;

	void Term() noexcept;

	void SetDoubleClickTime(uint32_t u32Interval) noexcept;

	void SetDefaultCursor(VeCursor::Data* pkCursor) noexcept;

	void SetFocus(VeWindow::Data* pkWindow) noexcept;

	void SendMotion(VeWindow::Data* pkWindow, VeMouseID u32MouseID,
		int32_t i32Relative, int32_t x, int32_t y) noexcept;

	void SendButton(VeWindow::Data* pkWindow, VeMouseID u32MouseID,
		uint8_t u8State, uint8_t u8Button) noexcept;

	void SendWheel(VeWindow::Data* pkWindow, VeMouseID u32MouseID,
		int32_t x, int32_t y) noexcept;

protected:
	bool UpdateFocus(VeWindow::Data* pkWindow, int32_t x, int32_t y,
		uint32_t u32ButtonState) noexcept;

	void PrivateSendMotion(VeWindow::Data* pkWindow, VeMouseID u32mouseID,
		int32_t i32Relative, int32_t x, int32_t y) noexcept;

	VeMouseClickState* GetClickState(uint8_t u8Button) noexcept;

	VeCursor::Data* GetCursorData(VeCursor* pkCursor) noexcept;

	virtual void _Init() noexcept {}

	virtual void _Term() noexcept {}

	virtual bool _CreateCursor(VeCursor::Data*, const VeSurfacePtr&,
		int32_t, int32_t) noexcept { return false; }

	virtual bool _CreateSystemCursor(VeCursor::Data*,
		VeSystemCursor) noexcept { return false; }

	virtual void _ShowCursor(VeCursor::Data*) noexcept {}

	virtual void _MoveCursor(VeCursor::Data*) noexcept {}

	virtual void _FreeCursor(VeCursor::Data*) noexcept {}

	virtual void _Warp(VeWindow::Data* pkWindow, int32_t x, int32_t y) noexcept;

	virtual bool _SetRelativeMode(bool) noexcept { return false; }

	friend class VeKeyboard;
	friend class VeVideoDevice;
	VeMouseID m_u32MouseID = 0;
	VeWindow::Data* m_pkFocus = nullptr;
	int32_t m_i32PosX = 0;
	int32_t m_i32PosY = 0;
	int32_t m_i32DeltaX = 0;
	int32_t m_i32DeltaY = 0;
	int32_t m_i32LastX = 0;
	int32_t m_i32LastY = 0;
	uint32_t m_u32ButtonState = 0;
	uint32_t m_u32DoubleClickTime = 500;
	int32_t m_i32DoubleClickRadius = 1;
	VE_BOOL m_bRelativeMode = VE_FALSE;
	VE_BOOL m_bRelativeModeWarp = VE_FALSE;
	VeVector<VeMouseClickState> m_kClickStateList;
	vtd::intrusive_list<VeCursor::Data*> m_kCursorList;
	VeCursorPtr m_spDefCursor;
	VeCursorPtr m_spCurCursor;
	VE_BOOL m_bCursorShown = VE_FALSE;

};

VeSmartPointer(VeMouse);

#include "VeMouse.inl"
