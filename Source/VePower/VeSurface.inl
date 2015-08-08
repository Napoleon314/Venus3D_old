////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeSurface.inl
//  Created:     2015/08/08 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline bool VeSurface::SetPalette(const VePalettePtr& spPalette) noexcept
{
	if (m_spFormat)
	{
		return m_spFormat->SetPalette(spPalette);
	}
	return false;
}
//--------------------------------------------------------------------------
inline VeInt32 VeSurface::GetWidth() noexcept
{
	return m_i32Width;
}
//--------------------------------------------------------------------------
inline VeInt32 VeSurface::GetHeight() noexcept
{
	return m_i32Height;
}
//--------------------------------------------------------------------------
inline VeInt32 VeSurface::GetPitch() noexcept
{
	return m_i32Pitch;
}
//--------------------------------------------------------------------------
inline VePixelFormatPtr VeSurface::GetFormat() noexcept
{
	return m_spFormat;
}
//--------------------------------------------------------------------------
