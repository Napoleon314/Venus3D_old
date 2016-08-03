////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      VeRenderer
//  File name:   VeRenderer.h
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

#pragma once

#define VE_RENDER_D3D			(0x0100)
#define VE_RENDER_KHR			(0x0200)
#define VE_RENDER_APPLE			(0x0400)
#define VE_RENDER_MOBILE		(0x0800)
#define VE_RENDER_NGAPI			(0x1000)
#define VE_RENDER_API_VER_MASK	(0x00FF)

enum VeRenderAPI
{
	VE_RENDER_D3D11		= (0xB0 | VE_RENDER_D3D),
	VE_RENDER_D3D12		= (0xC0 | VE_RENDER_D3D | VE_RENDER_NGAPI),
	VE_RENDER_OPENGL	= (VE_RENDER_KHR | VE_RENDER_APPLE),
	VE_RENDER_GLES3		= (0x30 | VE_RENDER_KHR | VE_RENDER_APPLE | VE_RENDER_MOBILE),
	VE_RENDER_VULKAN	= (VE_RENDER_KHR | VE_RENDER_MOBILE | VE_RENDER_NGAPI),
	VE_RENDER_METAL		= (VE_RENDER_APPLE | VE_RENDER_MOBILE | VE_RENDER_NGAPI)
};

VeSmartPointer(VeRenderer);

class VENUS_API VeRenderer : public VeRefObject
{
	VeNoCopy(VeRenderer);
	VeRTTIDecl(VeRenderer);
public:
	enum API
	{
		API_NONE = 0x0,
		API_D3D11 = 0x1,
		API_D3D12 = 0x2,
		API_OGL = 0x4,
		API_OGLES2 = 0x8,
		API_OGLES3 = 0x10,
		API_MASK = 0xFF
	};

	VeRenderer() noexcept;

	virtual ~VeRenderer() noexcept;

	VeRenderWindowPtr CreateRenderWindow(const char* pcTitle, int32_t w, int32_t h,
		int32_t x = VE_WINDOWPOS_CENTERED, int32_t y = VE_WINDOWPOS_CENTERED,
		uint32_t u32Flags = VE_WINDOW_DEFAUT_FLAGS) noexcept;

	inline VeInputLayoutPtr CreateInputLayout(const VeDyanmicStack<VeInputLayout::ElementDesc>& kDesc) noexcept;

	inline void PrepareShaders() noexcept;

	inline VeVertexShaderPtr FindVertexShader(const char* pcName) noexcept;

	inline VePixelShaderPtr FindPixelShader(const char* pcName) noexcept;

	inline VeGeometryShaderPtr FindGeometryShader(const char* pcName) noexcept;

	inline VeHullShaderPtr FindHullShader(const char* pcName) noexcept;

	inline VeDomainShaderPtr FindDomainShader(const char* pcName) noexcept;

	inline VeComputeShaderPtr FindComputeShader(const char* pcName) noexcept;

	template <size_t n>
	VeInputLayoutPtr CreateInputLayout(const VeInputLayout::ElementDesc(&desc)[n]) noexcept
	{
		return CreateInputLayout(desc, n);
	}

	void PrepareShaders(const char* pcSrcPath, const char* pcCachePath) noexcept;

	virtual void Init() = 0;

	virtual void Term() = 0;

	virtual void BeginSyncCopy() noexcept = 0;

	virtual void EndSyncCopy() noexcept = 0;

	virtual VeRenderWindowPtr CreateRenderWindow(const VeWindowPtr& spWindow) noexcept = 0;

	virtual VeInputLayoutPtr CreateInputLayout(const VeInputLayout::ElementDesc* pkDescs, size_t stNum) noexcept = 0;

	virtual void PrepareShaders(const VeDirectoryPtr& spSrc, const VeDirectoryPtr& spCache) noexcept = 0;
	
	static VeRendererPtr Create(VeRenderAPI eAPI) noexcept;

protected:
	VeStringMap<VeVertexShaderPtr> m_kVertexShaderMap;
	VeStringMap<VePixelShaderPtr> m_kPixelShaderMap;
	VeStringMap<VeGeometryShaderPtr> m_kGeometryShaderMap;
	VeStringMap<VeHullShaderPtr> m_kHullShaderMap;
	VeStringMap<VeDomainShaderPtr> m_kDomainShaderMap;
	VeStringMap<VeComputeShaderPtr> m_kComputeShaderMap;	

};

#include "VeRenderer.inl"
