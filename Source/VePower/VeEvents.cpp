////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeEvents.cpp
//  Created:     2015/08/04 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
VeEventQueue::VeEventQueue() noexcept
{

}
//--------------------------------------------------------------------------
VeEventQueue::~VeEventQueue() noexcept
{

}
//--------------------------------------------------------------------------
void VeEventQueue::Init() noexcept
{
	static_assert(sizeof(EventEntry) <= 64, "struct VeEvent is too large");
	m_spMemPool = VE_NEW VePoolAllocator(64, 64);
}
//--------------------------------------------------------------------------
void VeEventQueue::Term() noexcept
{
	m_kDisabledEvents.clear();
	m_kFreeList.clear();
	m_kEventQueue.clear();
	m_spMemPool = nullptr;
}
//--------------------------------------------------------------------------
void VeEventQueue::EnableEventType(VeUInt32 u32Type) noexcept
{
	auto it = m_kDisabledEvents.find(u32Type);
	if (it != m_kDisabledEvents.end())
	{
		m_kDisabledEvents.erase(it);
	}
}
//--------------------------------------------------------------------------
void VeEventQueue::DisableEventType(VeUInt32 u32Type) noexcept
{
	m_kDisabledEvents.insert(u32Type);
}
//--------------------------------------------------------------------------
bool VeEventQueue::IsEventTypeEnable(VeUInt32 u32Type) noexcept
{
	auto it = m_kDisabledEvents.find(u32Type);
	return it == m_kDisabledEvents.end() ? true : false;
}
//--------------------------------------------------------------------------
void VeEventQueue::PushEvent(VeEvent& kEvent) noexcept
{
	VeEvent* pkNew = AddEvent();
	VE_ASSERT(pkNew);
	kEvent = *pkNew;
}
//--------------------------------------------------------------------------
bool VeEventQueue::GetEvent(VeEvent& kEvent) noexcept
{
	if (m_kEventQueue.size())
	{
		EventEntry* pkEntity = m_kEventQueue.get_head_node()->m_Content;
		kEvent = pkEntity->m_kEvent;
		m_kFreeList.attach_back(pkEntity->m_kNode);
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------
VeUInt32 VeEventQueue::GetEventNumber() noexcept
{
	return (VeUInt32)m_kEventQueue.size();
}
//--------------------------------------------------------------------------
VeEvent* VeEventQueue::AddEvent() noexcept
{
	VE_ASSERT(m_spMemPool);
	EventEntry* pkEntry(nullptr);
	if (m_kFreeList.size())
	{
		pkEntry = m_kFreeList.get_tail_node()->m_Content;
	}
	else
	{
		pkEntry = new(m_spMemPool->Allocate()) EventEntry();
	}
	VE_ASSERT(pkEntry);
	m_kEventQueue.attach_back(pkEntry->m_kNode);
	return &(pkEntry->m_kEvent);
}
//--------------------------------------------------------------------------
void VeEventQueue::PeekEvent(VeVector<VeEvent*>& kOutput) noexcept
{
	kOutput.clear();
	for (auto obj : m_kEventQueue)
	{
		kOutput.push_back(&(obj->m_kEvent));
	}
}
//--------------------------------------------------------------------------
void VeEventQueue::FlushEvent(VeUInt32 u32Event) noexcept
{
	for (auto obj : m_kEventQueue)
	{
		if (obj->m_kEvent.m_u32Type == u32Event)
		{
			m_kFreeList.attach_back(obj->m_kNode);
		}
	}
}
//--------------------------------------------------------------------------
void VeEventQueue::FlushEvents(VeUInt32 u32Min, VeUInt32 u32Max) noexcept
{
	for (auto obj : m_kEventQueue)
	{
		if (obj->m_kEvent.m_u32Type >= u32Min
			&& obj->m_kEvent.m_u32Type <= u32Max)
		{
			m_kFreeList.attach_back(obj->m_kNode);
		}
	}
}
//--------------------------------------------------------------------------
void VeEventQueue::FlushEvents() noexcept
{
	for (auto obj : m_kEventQueue)
	{
		m_kFreeList.attach_back(obj->m_kNode);
	}
	VE_ASSERT(m_kEventQueue.empty());
}
//--------------------------------------------------------------------------
