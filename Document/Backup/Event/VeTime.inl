////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Event
//  File name:   VeTime.inl
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

//--------------------------------------------------------------------------
inline float VeTime::GetDeltaTime() noexcept
{
	return (float)m_f64DeltaTime;
}
//--------------------------------------------------------------------------
inline double VeTime::GetDeltaTime64() noexcept
{
	return m_f64DeltaTime;
}
//--------------------------------------------------------------------------
inline double VeTime::GetTime() noexcept
{
	return m_f64Time;
}
//--------------------------------------------------------------------------
inline uint32_t VeTime::GetTimeUInt() noexcept
{
	return (uint32_t)(m_f64Time * 100);
}
//--------------------------------------------------------------------------
inline uint64_t VeTime::GetCurrentCount() noexcept
{
	return m_u64CurrentCount;
}
//--------------------------------------------------------------------------
inline void VeTime::Pause() noexcept
{
	m_bPaused = true;
}
//--------------------------------------------------------------------------
inline void VeTime::Resume() noexcept
{
	m_bPaused = false;
}
//--------------------------------------------------------------------------
inline bool VeTime::GetPaused() noexcept
{
	return m_bPaused;
}
//--------------------------------------------------------------------------
inline void VeTime::SetInvert(bool bInv) noexcept
{
	m_bInvert = bInv;
}
//--------------------------------------------------------------------------
inline bool VeTime::GetInvert() noexcept
{
	return m_bInvert;
}
//--------------------------------------------------------------------------
