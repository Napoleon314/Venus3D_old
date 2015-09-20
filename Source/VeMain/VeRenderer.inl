////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderer.inl
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeRenderer::API VeRenderer::GetAPI() const noexcept
{
	return m_eType;
}
//--------------------------------------------------------------------------
inline void VeRenderer::RegistResource(const VeChar8* pcName,
	const VeRenderResourcePtr& spRes) noexcept
{
	m_kResourceMap[pcName] = spRes;
}
//--------------------------------------------------------------------------
inline void VeRenderer::UnregistResource(const VeChar8* pcName) noexcept
{
	auto it = m_kResourceMap.find(pcName);
	if (it != m_kResourceMap.end())
	{
		m_kResourceMap.erase(it);
	}
}
//--------------------------------------------------------------------------
