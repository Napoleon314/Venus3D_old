////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeTime.inl
//  Created:     2015/08/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeFloat32 VeTime::GetDeltaTime() noexcept
{
	return (VeFloat32)m_f64DeltaTime;
}
//--------------------------------------------------------------------------
inline VeFloat64 VeTime::GetDeltaTime64() noexcept
{
	return m_f64DeltaTime;
}
//--------------------------------------------------------------------------
inline VeFloat64 VeTime::GetTime() noexcept
{
	return m_f64Time;
}
//--------------------------------------------------------------------------
inline VeUInt32 VeTime::GetTimeUInt() noexcept
{
	return (VeUInt32)(m_f64Time * 100);
}
//--------------------------------------------------------------------------
inline VeUInt64 VeTime::GetCurrentCount() noexcept
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
