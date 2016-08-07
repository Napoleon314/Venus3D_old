////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      MeshConvertor
//  File name:   Mesh.h
//  Created:     2016/08/07 by Albert
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
#include "Utility/DirectXMesh.h"

struct Mesh
{
	vtd::string m_kName;
	size_t m_stFaces;
	size_t m_stVerts;
	std::vector<uint32_t> m_kIndices;
	std::vector<uint32_t> m_kAttributes;
	std::vector<uint32_t> m_kRepeat;
	std::vector<uint32_t> m_kAdjacency;
	std::vector<DirectX::XMFLOAT3> m_kPosition;
	std::vector<std::vector<DirectX::XMFLOAT3>> m_kNormals;
	std::vector<std::vector<DirectX::XMFLOAT4>> m_kTangents;
	std::vector<std::vector<DirectX::XMFLOAT3>> m_kBiTangents;
	std::vector<std::vector<DirectX::XMFLOAT2>> m_kTexcoords;
	std::vector<std::vector<DirectX::XMFLOAT4>> m_kColors;

	void Clear() noexcept;

	void Process(float fEpsilon = 0) noexcept;

	void GenerateAdj(float fEpsilon = 0) noexcept;

	void ValidateRepeats(float fEpsilon = 0) noexcept;

	void RemoveDuplicates(float fEpsilon) noexcept;

	void CleanMesh() noexcept;

	void Optimize(uint32_t vertexCache = DirectX::OPTFACES_V_DEFAULT, uint32_t restart = DirectX::OPTFACES_R_DEFAULT) noexcept;

	bool EqualVertexInSamePosition(uint32_t u32First, uint32_t u32Second, float fEpsilon = 0) noexcept;

};
