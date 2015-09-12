////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeParser.inl
//  Created:     2015/09/12 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeFloat32 VeParser::GetValue(const VeChar8* pcName) noexcept
{
	auto it = m_kValueMap.find(pcName);
	if (it != m_kValueMap.end())
	{
		return it->second;
	}
	return 0;
}
//--------------------------------------------------------------------------
inline void VeParser::SetValue(const VeChar8* pcName,
	VeFloat32 f32Value) noexcept
{
	m_kValueMap[pcName] = f32Value;
}
//--------------------------------------------------------------------------
