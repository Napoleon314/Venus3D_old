////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      MaterialCompiler
//  File name:   Material.h
//  Created:     2016/08/11 by Albert
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

#include <Venus3D/Venus3D.h>
#include <d3dcompiler.h>
#include <d3d12.h>

struct CD3DX12_ROOT_SIGNATURE_DESC : public D3D12_ROOT_SIGNATURE_DESC
{
	CD3DX12_ROOT_SIGNATURE_DESC() noexcept
	{
		memset(this, 0, sizeof(CD3DX12_ROOT_SIGNATURE_DESC));
	}

	~CD3DX12_ROOT_SIGNATURE_DESC() noexcept
	{
		if (pParameters)
		{
			VeFree((void*)pParameters);
		}

		if (pStaticSamplers)
		{
			VeFree((void*)pStaticSamplers);
		}
	}
};

class Material : public VeStackObject
{
public:
	enum ShaderType
	{
		VERTEX_SHADER,
		PIXEL_SHADER,
		GEOMETRY_SHADER,
		HULL_SHADER,
		DOMAIN_SHADER,
		COMPUTE_SHADER,
		SHADER_MAX
	};

	Material(const char* pcPath) noexcept;

	~Material() noexcept;

	bool Init(const char* pcPath) noexcept;

	bool Compile() noexcept;

private:
	void AddConfig(const VeDirectoryPtr& spDir, VeDirectory::ChildData& data) noexcept;

	bool CompileShaders() noexcept;

	bool CompileShader(ShaderType eType, venus::xml_node* pkNode) noexcept;

	bool CompileRootSignatures() noexcept;

	bool CompileRootSignature(venus::xml_node* pkNode) noexcept;

	vtd::string m_kName;
	vtd::string m_kPath;
	VeVector<std::pair<VeBlobPtr, venus::xml_document>> m_kSourceArray;
	VeStringMap<VeBlobPtr> m_akShaderCodes[SHADER_MAX];
	VeStringMap<VeBlobPtr> m_kRootSignature;

};
