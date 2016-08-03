////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      D3D12
//  File name:   D3D12RenderState.h
//  Created:     2016/07/29 by Albert
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

#ifdef VE_ENABLE_D3D12

class D3D12VertexShader : public VeVertexShader
{
	VeNoCopy(D3D12VertexShader);
	VeRTTIDecl(D3D12VertexShader, VeVertexShader);
public:
	D3D12VertexShader(size_t stSize) noexcept;

	virtual ~D3D12VertexShader() noexcept;

	void* m_pvData = nullptr;
	size_t m_stSize = 0;
};

VeSmartPointer(D3D12VertexShader);

class D3D12PixelShader : public VePixelShader
{
	VeNoCopy(D3D12PixelShader);
	VeRTTIDecl(D3D12PixelShader, VePixelShader);
public:
	D3D12PixelShader(size_t stSize) noexcept;

	virtual ~D3D12PixelShader() noexcept;

	void* m_pvData = nullptr;
	size_t m_stSize = 0;
};

VeSmartPointer(D3D12PixelShader);

class D3DGeometryShader : public VeGeometryShader
{
	VeNoCopy(D3DGeometryShader);
	VeRTTIDecl(D3DGeometryShader, VeGeometryShader);
public:
	D3DGeometryShader(size_t stSize) noexcept;

	virtual ~D3DGeometryShader() noexcept;

	void* m_pvData = nullptr;
	size_t m_stSize = 0;
};

VeSmartPointer(D3DGeometryShader);

class D3DHullShader : public VeHullShader
{
	VeNoCopy(D3DHullShader);
	VeRTTIDecl(D3DHullShader, VeHullShader);
public:
	D3DHullShader(size_t stSize) noexcept;

	virtual ~D3DHullShader() noexcept;

	void* m_pvData = nullptr;
	size_t m_stSize = 0;
};

VeSmartPointer(D3DHullShader);

class D3DDomainShader : public VeDomainShader
{
	VeNoCopy(D3DDomainShader);
	VeRTTIDecl(D3DDomainShader, VeDomainShader);
public:
	D3DDomainShader(size_t stSize) noexcept;

	virtual ~D3DDomainShader() noexcept;

	void* m_pvData = nullptr;
	size_t m_stSize = 0;
};

VeSmartPointer(D3DDomainShader);

class D3DComputeShader : public VeComputeShader
{
	VeNoCopy(D3DComputeShader);
	VeRTTIDecl(D3DComputeShader, VeComputeShader);
public:
	D3DComputeShader(size_t stSize) noexcept;

	virtual ~D3DComputeShader() noexcept;

	void* m_pvData = nullptr;
	size_t m_stSize = 0;
};

VeSmartPointer(D3DComputeShader);

class D3D12InputLayout : public VeInputLayout
{
	VeNoCopy(D3D12InputLayout);
	VeRTTIDecl(D3D12InputLayout, VeInputLayout);
public:
	D3D12InputLayout(const VeInputLayout::ElementDesc* pkDescs, size_t stNum) noexcept;

	virtual ~D3D12InputLayout() noexcept;

	operator const D3D12_INPUT_LAYOUT_DESC&() noexcept
	{
		return m_kDesc;
	}

	const D3D12_INPUT_LAYOUT_DESC& Get() noexcept
	{
		return m_kDesc;
	}

private:
	D3D12_INPUT_LAYOUT_DESC m_kDesc;

};


#endif
