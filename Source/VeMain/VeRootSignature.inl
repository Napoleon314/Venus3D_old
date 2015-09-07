////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRootSignature.inl
//  Created:     2015/08/31 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline const VeRenderer::RootSignaturePtr& VeRootSignature::GetObject() noexcept
{
	return m_spObject;
}
//--------------------------------------------------------------------------
inline const VeChar8* VeRootSignature::GetName() noexcept
{
	return "root_signature";
}
//--------------------------------------------------------------------------