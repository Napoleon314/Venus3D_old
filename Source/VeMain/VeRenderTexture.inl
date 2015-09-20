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
inline VeRenderTexture::Useage VeRenderTexture::GetUseage() noexcept
{
	return m_eUseage;
}
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
inline std::pair<VeUInt16, VeUInt16>
VeRenderTexture::GetSampleDesc() noexcept
{
	return std::make_pair(m_u16Count, m_u16Quality);
}
//--------------------------------------------------------------------------
inline bool VeRenderTexture::CanUse(Useage eUse) noexcept
{
	return VE_MASK_HAS_ALL(GetUseage(), eUse);
}
//--------------------------------------------------------------------------
