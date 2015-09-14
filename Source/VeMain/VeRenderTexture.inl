////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderResource.inl
//  Created:     2015/09/14 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeRenderResource::Format VeRenderTexture::GetFormat() noexcept
{
	return m_eFormat;
}
//--------------------------------------------------------------------------
inline VeUInt32 VeRenderTexture::GetWidth() noexcept
{
	return m_u32Width;
}
//--------------------------------------------------------------------------
inline VeUInt32 VeRenderTexture::GetHeight() noexcept
{
	return m_u32Height;
}
//--------------------------------------------------------------------------
inline VeUInt16 VeRenderTexture::GetDepth() noexcept
{
	return m_u16Depth;
}
//--------------------------------------------------------------------------
inline VeUInt16 VeRenderTexture::GetMipLevels() noexcept
{
	return m_u16MipLevels;
}
//--------------------------------------------------------------------------
inline VeRenderTexture::SampleDesc VeRenderTexture::GetSampleDesc() noexcept
{
	return m_kSampleDesc;
}
//--------------------------------------------------------------------------
