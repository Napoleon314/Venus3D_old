////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      VeRenderer
//  File name:   VeRenderer.inl
//  Created:     2016/07/22 by Albert
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
inline VeInputLayoutPtr VeRenderer::CreateInputLayout(
	const VeDyanmicStack<VeInputLayout::ElementDesc>& kDesc) noexcept
{
	return CreateInputLayout(kDesc.data(), kDesc.size());
}
//--------------------------------------------------------------------------
inline void VeRenderer::PrepareShaders() noexcept
{
	//PrepareShaders(venus3d.GetConfig("vsc_src"), venus3d.GetConfig("vsc_cache"));
}
//--------------------------------------------------------------------------
inline VeVertexShaderPtr VeRenderer::FindVertexShader(
	const char* pcName) noexcept
{
	auto it = m_kVertexShaderMap.find(pcName);
	return it == m_kVertexShaderMap.end() ? nullptr : it->second;
}
//--------------------------------------------------------------------------
inline VePixelShaderPtr VeRenderer::FindPixelShader(
	const char* pcName) noexcept
{
	auto it = m_kPixelShaderMap.find(pcName);
	return it == m_kPixelShaderMap.end() ? nullptr : it->second;
}
//--------------------------------------------------------------------------
inline VeGeometryShaderPtr VeRenderer::FindGeometryShader(
	const char* pcName) noexcept
{
	auto it = m_kGeometryShaderMap.find(pcName);
	return it == m_kGeometryShaderMap.end() ? nullptr : it->second;
}
//--------------------------------------------------------------------------
inline VeHullShaderPtr VeRenderer::FindHullShader(
	const char* pcName) noexcept
{
	auto it = m_kHullShaderMap.find(pcName);
	return it == m_kHullShaderMap.end() ? nullptr : it->second;
}
//--------------------------------------------------------------------------
inline VeDomainShaderPtr VeRenderer::FindDomainShader(
	const char* pcName) noexcept
{
	auto it = m_kDomainShaderMap.find(pcName);
	return it == m_kDomainShaderMap.end() ? nullptr : it->second;
}
//--------------------------------------------------------------------------
inline VeComputeShaderPtr VeRenderer::FindComputeShader(
	const char* pcName) noexcept
{
	auto it = m_kComputeShaderMap.find(pcName);
	return it == m_kComputeShaderMap.end() ? nullptr : it->second;
}
//--------------------------------------------------------------------------
