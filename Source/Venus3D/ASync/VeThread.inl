////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   VeThread.inl
//  Created:     2016/07/11 by Albert
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

//--------------------------------------------------------------------------
inline bool VeThread::IsRunning() noexcept
{
	return m_u32State.load(std::memory_order_relaxed) == 1;
}
//--------------------------------------------------------------------------
inline void VeThread::SetEntry(const std::function<void()>& kEntry) noexcept
{
	if (m_u32State.load(std::memory_order_relaxed) == 0)
	{
		m_kEntry = kEntry;
	}
}
//--------------------------------------------------------------------------
inline void VeThread::Start() noexcept
{
	uint32_t u32Stop(0);
	if (m_u32State.compare_exchange_weak(u32Stop, 1u, std::memory_order_relaxed))
	{
		m_pkParams->m_kEventLoop.set();
		m_pkParams->m_kEvent.reset();
	}
}
//--------------------------------------------------------------------------
void VeThread::StartEntry(const std::function<void()>& kEntry) noexcept
{
	uint32_t u32Stop(0);
	if (m_u32State.compare_exchange_weak(u32Stop, 1u, std::memory_order_relaxed))
	{
		m_kEntry = kEntry;
		m_pkParams->m_kEventLoop.set();
		m_pkParams->m_kEvent.reset();
	}
}
//--------------------------------------------------------------------------
inline void VeThread::Join() noexcept
{
	m_pkParams->m_kEvent.wait();
}
//--------------------------------------------------------------------------
inline void VeThread::Suspend() noexcept
{
	VeSuspendThread(m_hThread);
}
//--------------------------------------------------------------------------
inline void VeThread::Resume() noexcept
{
	VeResumeThread(m_hThread);
}
//--------------------------------------------------------------------------
inline void VeThread::Init()
{
	VeThreadInit();
}
//--------------------------------------------------------------------------
inline void VeThread::Term()
{
	VeThreadTerm();
}
//--------------------------------------------------------------------------