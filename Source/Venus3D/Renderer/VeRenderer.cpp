////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   VeRenderer.cpp
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

#include "stdafx.h"

//--------------------------------------------------------------------------
VeRTTIImpl(VeRenderer);
//--------------------------------------------------------------------------
VeRenderer::VeRenderer() noexcept
{
	
}
//--------------------------------------------------------------------------
VeRenderer::~VeRenderer() noexcept
{

}
//--------------------------------------------------------------------------
#ifdef VE_ENABLE_D3D12
//--------------------------------------------------------------------------
extern VeRendererPtr CreateRendererD3D12() noexcept;
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
VeRenderWindowPtr VeRenderer::CreateRenderWindow(const char* pcTitle,
	int32_t w, int32_t h, int32_t x, int32_t y, uint32_t u32Flags) noexcept
{
	VeDesktopVideoPtr spVideo = VeDynamicCast(VeDesktopVideo, Venus3D::Ref().GetVideo());
	if (!spVideo) return nullptr;
	VeDesktopWindowPtr spWindow = spVideo->Create(pcTitle, w, h, x, y, u32Flags);
	VE_ASSERT(spWindow);
	VeRenderWindowPtr spRes = CreateRenderWindow(spWindow);
	spWindow = nullptr;
	return spRes;
}
//--------------------------------------------------------------------------
VeRendererPtr VeRenderer::Create(VeRenderAPI eAPI) noexcept
{
	switch (eAPI)
	{
#	ifdef VE_ENABLE_D3D12
	case VE_RENDER_D3D12:
		return CreateRendererD3D12();
#	endif
	default:
		return nullptr;
	}
}
//--------------------------------------------------------------------------
