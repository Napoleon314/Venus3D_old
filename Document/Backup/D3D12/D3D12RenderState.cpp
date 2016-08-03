////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      D3D12
//  File name:   VeRendererD3D12.cpp
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
#include "D3D12Renderer.h"
#include "D3D12RenderWindow.h"
#include "D3D12RenderState.h"

//--------------------------------------------------------------------------
#ifdef VE_ENABLE_D3D12
//--------------------------------------------------------------------------
VeRTTIImpl(D3D12InputLayout, VeInputLayout);
//--------------------------------------------------------------------------
static constexpr const char* s_apcSematicNameMap[VE_SN_NUM] =
{
	"BINORMAL",
	"BLENDINDICES",
	"BLENDWEIGHT",
	"COLOR",
	"NORMAL",
	"POSITION",
	"POSITIONT",
	"PSIZE",
	"TANGENT",
	"TEXCOORD"
};
//--------------------------------------------------------------------------
D3D12InputLayout::D3D12InputLayout(
	const VeInputLayout::ElementDesc* pkDescs, size_t stNum) noexcept
{
	VE_ASSERT(stNum);
	D3D12_INPUT_ELEMENT_DESC* pkAllocated = VeAlloc(D3D12_INPUT_ELEMENT_DESC, stNum);
	for (size_t i(0); i < stNum; ++i)
	{
		pkAllocated[i] =
		{
			s_apcSematicNameMap[pkDescs[i].m_eName],
			pkDescs[i].m_u32Index,
			(DXGI_FORMAT)pkDescs[i].m_eFormat,
			pkDescs[i].m_u32Slot,
			pkDescs[i].m_u32Offset,
			(D3D12_INPUT_CLASSIFICATION)pkDescs[i].m_eClass,
			pkDescs[i].m_u32Rate
		};
	}
	m_kDesc = { pkAllocated, (UINT)stNum };
}
//--------------------------------------------------------------------------
D3D12InputLayout::~D3D12InputLayout() noexcept
{
	VeFree((void*)m_kDesc.pInputElementDescs);
	m_kDesc.pInputElementDescs = nullptr;
	m_kDesc.NumElements = 0;
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
