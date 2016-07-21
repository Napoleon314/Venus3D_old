////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Event
//  File name:   VeEvent.h
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

enum VeEventType
{
	VE_FIRSTEVENT = 0,
	VE_QUIT = 0x100,
	VE_APP_TERMINATING,
	VE_APP_LOWMEMORY,
	VE_APP_WILLENTERBACKGROUND,
	VE_APP_DIDENTERBACKGROUND,
	VE_APP_WILLENTERFOREGROUND,
	VE_APP_DIDENTERFOREGROUND,

	VE_WINDOWEVENT = 0x200,
	VE_SYSWMEVENT,

	VE_KEYDOWN = 0x300,
	VE_KEYUP,
	VE_TEXTEDITING,
	VE_TEXTINPUT,

	VE_MOUSEMOTION = 0x400,
	VE_MOUSEBUTTONDOWN,
	VE_MOUSEBUTTONUP,
	VE_MOUSEWHEEL,

	VE_JOYAXISMOTION = 0x600,
	VE_JOYBALLMOTION,
	VE_JOYHATMOTION,
	VE_JOYBUTTONDOWN,
	VE_JOYBUTTONUP,
	VE_JOYDEVICEADDED,
	VE_JOYDEVICEREMOVED,

	VE_CONTROLLERAXISMOTION = 0x650,
	VE_CONTROLLERBUTTONDOWN,
	VE_CONTROLLERBUTTONUP,
	VE_CONTROLLERDEVICEADDED,
	VE_CONTROLLERDEVICEREMOVED,
	VE_CONTROLLERDEVICEREMAPPED,

	VE_FINGERDOWN = 0x700,
	VE_FINGERUP,
	VE_FINGERMOTION,

	VE_DOLLARGESTURE = 0x800,
	VE_DOLLARRECORD,
	VE_MULTIGESTURE,

	VE_CLIPBOARDUPDATE = 0x900,

	VE_DROPFILE = 0x1000,

	VE_RENDER_TARGETS_RESET = 0x2000,

	VE_USEREVENT = 0x8000,

	VE_LASTEVENT = 0xFFFF
};

struct VeCommonEvent
{
	uint32_t m_u32Type;
	uint32_t m_u32TimeStamp;
};

struct VeWindowEvent
{
	uint32_t m_u32Type;
	uint32_t m_u32TimeStamp;
	uint32_t m_u32WindowID;
	uint8_t m_u8Event;
	uint8_t m_u8Padding1;
	uint8_t m_u8Padding2;
	uint8_t m_u8Padding3;
	int32_t m_i32Data1;
	int32_t m_i32Data2;
};

struct VeKeyboardEvent
{
	uint32_t m_u32Type;
	uint32_t m_u32TimeStamp;
	uint32_t m_u32WindowID;
	uint8_t m_u8State;
	uint8_t m_u8Repeat;
	uint8_t m_u8Padding2;
	uint8_t m_u8Padding3;
	VeKeysym m_kKeysym;
};

struct VeMouseMotionEvent
{
	uint32_t m_u32Type;
	uint32_t m_u32TimeStamp;
	uint32_t m_u32WindowID;
	uint32_t m_u32Which;
	uint32_t m_u32State;
	int32_t x;
	int32_t y;
	int32_t xrel;
	int32_t yrel;
};

struct VeMouseButtonEvent
{
	uint32_t m_u32Type;
	uint32_t m_u32TimeStamp;
	uint32_t m_u32WindowID;
	uint32_t m_u32Which;
	uint8_t m_u8Button;
	uint8_t m_u8State;
	uint8_t m_u8Clicks;
	uint8_t m_u8Padding1;
	int32_t x;
	int32_t y;
};

struct VeMouseWheelEvent
{
	uint32_t m_u32Type;
	uint32_t m_u32TimeStamp;
	uint32_t m_u32WindowID;
	uint32_t m_u32Which;
	int32_t x;
	int32_t y;
};

struct VeQuitEvent
{
	uint32_t m_u32Type;
	uint32_t m_u32TimeStamp;
};

union VeEvent
{
	uint32_t m_u32Type;
	VeCommonEvent m_kCommon;
	VeWindowEvent m_kWindow;
	VeKeyboardEvent m_kKey;
	VeMouseMotionEvent m_kMotion;
	VeMouseButtonEvent m_kButton;
	VeMouseWheelEvent m_kWheel;
	VeQuitEvent m_kQuit;
};

#define VE_TICKS_PASSED(A, B)  ((int32_t)((B) - (A)) <= 0)

class VENUS_API VeEventQueue : public VeRefObject
{
	VeNoCopy(VeEventQueue);
public:
	struct EventEntry
	{
		EventEntry() noexcept
		{
			m_kNode._Content = this;
		}

		vtd::intrusive_node<EventEntry*> m_kNode;
		VeEvent m_kEvent;
	};

	typedef std::function<bool(VeEvent&)> EventFilter;

	VeEventQueue() noexcept;

	virtual ~VeEventQueue() noexcept;

	void Init() noexcept;

	void Term() noexcept;

	void EnableEventType(uint32_t u32Type) noexcept;

	void DisableEventType(uint32_t u32Type) noexcept;

	bool IsEventTypeEnable(uint32_t u32Type) noexcept;

	void PushEvent(VeEvent& kEvent) noexcept;

	bool GetEvent(VeEvent& kEvent) noexcept;

	uint32_t GetEventNumber() noexcept;

	VeEvent* AddEvent() noexcept;

	void PeekEvents(VeVector<VeEvent*>& kOutput) noexcept;

	void FlushEvent(uint32_t u32Event) noexcept;

	void FlushEvents(uint32_t u32Min, uint32_t u32Max) noexcept;

	void FlushEvents() noexcept;

	void FilterEvents(EventFilter funcFilter) noexcept;

	void SendAppEvent(VeEventType eType) noexcept;

	static uint32_t GetTicks() noexcept;

protected:
	VeUnorderedSet<uint32_t> m_kDisabledEvents;
	VePoolAllocatorPtr m_spMemPool;
	vtd::intrusive_list<EventEntry*> m_kFreeList;
	vtd::intrusive_list<EventEntry*> m_kEventQueue;

};

VeSmartPointer(VeEventQueue);
