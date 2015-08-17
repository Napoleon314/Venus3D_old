////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeLog.inl
//  Created:     2015/03/02 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline void VeLog::SetLevel(Level eLevel) noexcept
{
	m_eLevel = eLevel;
}
//--------------------------------------------------------------------------
inline VeLog::Level VeLog::GetLevel() const noexcept
{
	return m_eLevel;
}
//--------------------------------------------------------------------------
inline void VeLog::SetTarget(OutputFunc funcTarget) noexcept
{
	m_funcTarget = funcTarget;
}
//--------------------------------------------------------------------------
