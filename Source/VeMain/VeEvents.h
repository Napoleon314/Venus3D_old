////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeEvents.h
//  Created:     2015/08/04 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
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
	VeUInt32 m_u32Type;
	VeUInt32 m_u32TimeStamp;
};

struct VeWindowEvent
{
	VeUInt32 m_u32Type;
	VeUInt32 m_u32TimeStamp;
	VeUInt32 m_u32WindowID;
	VeUInt8 m_u8Event;
	VeUInt8 m_u8Padding1;
	VeUInt8 m_u8Padding2;
	VeUInt8 m_u8Padding3;
	VeInt32 m_i32Data1;
	VeInt32 m_i32Data2;
};

struct VeKeyboardEvent
{
	VeUInt32 m_u32Type;
	VeUInt32 m_u32TimeStamp;
	VeUInt32 m_u32WindowID;
	VeUInt8 m_u8State;
	VeUInt8 m_u8Repeat;
	VeUInt8 m_u8Padding2;
	VeUInt8 m_u8Padding3;
	VeKeysym m_kKeysym;
};

struct VeMouseMotionEvent
{
	VeUInt32 m_u32Type;
	VeUInt32 m_u32TimeStamp;
	VeUInt32 m_u32WindowID;
	VeUInt32 m_u32Which;
	VeUInt32 m_u32State;
	VeInt32 x;
	VeInt32 y;
	VeInt32 xrel;
	VeInt32 yrel;
};

struct VeMouseButtonEvent
{
	VeUInt32 m_u32Type;
	VeUInt32 m_u32TimeStamp;
	VeUInt32 m_u32WindowID;
	VeUInt32 m_u32Which;
	VeUInt8 m_u8Button;
	VeUInt8 m_u8State;
	VeUInt8 m_u8Clicks;
	VeUInt8 m_u8Padding1;
	VeInt32 x;
	VeInt32 y;
};

struct VeMouseWheelEvent
{
	VeUInt32 m_u32Type;
	VeUInt32 m_u32TimeStamp;
	VeUInt32 m_u32WindowID;
	VeUInt32 m_u32Which;
	VeInt32 x;
	VeInt32 y;
};

struct VeQuitEvent
{
	VeUInt32 m_u32Type;
	VeUInt32 m_u32TimeStamp;
};

union VeEvent
{
	VeUInt32 m_u32Type;
	VeCommonEvent m_kCommon;
	VeWindowEvent m_kWindow;
	VeKeyboardEvent m_kKey;
	VeMouseMotionEvent m_kMotion;
	VeMouseButtonEvent m_kButton;
	VeMouseWheelEvent m_kWheel;
	VeQuitEvent m_kQuit;
};

#define VE_TICKS_PASSED(A, B)  ((VeInt32)((B) - (A)) <= 0)

class VE_MAIN_API VeEventQueue : public VeRefObject
{
public:
	struct EventEntry
	{
		EventEntry() noexcept
		{
			m_kNode.m_Content = this;
		}

		VeRefNode<EventEntry*> m_kNode;
		VeEvent m_kEvent;
	};

	typedef std::function<bool(VeEvent&)> EventFilter;

	VeEventQueue() noexcept;

	virtual ~VeEventQueue() noexcept;

	void Init() noexcept;

	void Term() noexcept;

	void EnableEventType(VeUInt32 u32Type) noexcept;

	void DisableEventType(VeUInt32 u32Type) noexcept;

	bool IsEventTypeEnable(VeUInt32 u32Type) noexcept;

	void PushEvent(VeEvent& kEvent) noexcept;

	bool GetEvent(VeEvent& kEvent) noexcept;

	VeUInt32 GetEventNumber() noexcept;

	VeEvent* AddEvent() noexcept;

	void PeekEvents(VeVector<VeEvent*>& kOutput) noexcept;

	void FlushEvent(VeUInt32 u32Event) noexcept;

	void FlushEvents(VeUInt32 u32Min, VeUInt32 u32Max) noexcept;

	void FlushEvents() noexcept;

	void FilterEvents(EventFilter funcFilter) noexcept;

	void SendAppEvent(VeEventType eType) noexcept;

	static VeUInt32 GetTicks() noexcept;

protected:
	VeUnorderedSet<VeUInt32> m_kDisabledEvents;
	VePoolAllocatorPtr m_spMemPool;
	VeRefList<EventEntry*> m_kFreeList;
	VeRefList<EventEntry*> m_kEventQueue;

};

VeSmartPointer(VeEventQueue);
