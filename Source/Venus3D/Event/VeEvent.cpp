////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Event
//  File name:   VeEvent.cpp
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

#include "stdafx.h"

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
	m_spMemPool = VE_NEW VePoolAllocator(sizeof(EventEntry), 64);
}
//--------------------------------------------------------------------------
void VeEventQueue::Term() noexcept
{
	m_kDisabledEvents.clear();
	for (auto obj : m_kEventQueue)
	{
		obj->m_kNode.detach();
		m_spMemPool->Deallocate(obj);
	}
	for (auto obj : m_kFreeList)
	{
		obj->m_kNode.detach();
		m_spMemPool->Deallocate(obj);
	}
	m_spMemPool = nullptr;
}
//--------------------------------------------------------------------------
void VeEventQueue::EnableEventType(uint32_t u32Type) noexcept
{
	auto it = m_kDisabledEvents.find(u32Type);
	if (it != m_kDisabledEvents.end())
	{
		m_kDisabledEvents.erase(it);
	}
}
//--------------------------------------------------------------------------
void VeEventQueue::DisableEventType(uint32_t u32Type) noexcept
{
	m_kDisabledEvents.insert(u32Type);
}
//--------------------------------------------------------------------------
bool VeEventQueue::IsEventTypeEnable(uint32_t u32Type) noexcept
{
	auto it = m_kDisabledEvents.find(u32Type);
	return it == m_kDisabledEvents.end() ? true : false;
}
//--------------------------------------------------------------------------
void VeEventQueue::PushEvent(VeEvent& kEvent) noexcept
{
	VeEvent* pkNew = AddEvent();
	assert(pkNew);
	kEvent = *pkNew;
}
//--------------------------------------------------------------------------
bool VeEventQueue::GetEvent(VeEvent& kEvent) noexcept
{
	if (m_kEventQueue.size())
	{
		EventEntry* pkEntity = m_kEventQueue.get_head_node()->_Content;
		kEvent = pkEntity->m_kEvent;
		m_kFreeList.attach_back(pkEntity->m_kNode);
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------
uint32_t VeEventQueue::GetEventNumber() noexcept
{
	return (uint32_t)m_kEventQueue.size();
}
//--------------------------------------------------------------------------
VeEvent* VeEventQueue::AddEvent() noexcept
{
	assert(m_spMemPool);
	EventEntry* pkEntry(nullptr);
	if (m_kFreeList.size())
	{
		pkEntry = m_kFreeList.get_tail_node()->_Content;
	}
	else
	{
		pkEntry = new(m_spMemPool->Allocate()) EventEntry();
	}
	assert(pkEntry);
	m_kEventQueue.attach_back(pkEntry->m_kNode);
	return &(pkEntry->m_kEvent);
}
//--------------------------------------------------------------------------
void VeEventQueue::PeekEvents(VeVector<VeEvent*>& kOutput) noexcept
{
	kOutput.clear();
	for (auto obj : m_kEventQueue)
	{
		kOutput.push_back(&(obj->m_kEvent));
	}
}
//--------------------------------------------------------------------------
void VeEventQueue::FlushEvent(uint32_t u32Event) noexcept
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
void VeEventQueue::FlushEvents(uint32_t u32Min, uint32_t u32Max) noexcept
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
	assert(m_kEventQueue.empty());
}
//--------------------------------------------------------------------------
void VeEventQueue::FilterEvents(EventFilter funcFilter) noexcept
{
	for (auto obj : m_kEventQueue)
	{
		if (!funcFilter(obj->m_kEvent))
		{
			m_kFreeList.attach_back(obj->m_kNode);
		}		
	}
}
//--------------------------------------------------------------------------
void VeEventQueue::SendAppEvent(VeEventType eType) noexcept
{
	if (IsEventTypeEnable(eType))
	{
		VeEvent* pkNew = AddEvent();
		pkNew->m_kCommon.m_u32Type = eType;
		pkNew->m_kCommon.m_u32TimeStamp = GetTicks();
	}
}
//--------------------------------------------------------------------------
uint32_t VeEventQueue::GetTicks() noexcept
{
	return Venus3D::Ref().GetTime().GetTimeUInt();
}
//--------------------------------------------------------------------------
