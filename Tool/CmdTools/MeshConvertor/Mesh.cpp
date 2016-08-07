////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      MeshConvertor
//  File name:   Mesh.cpp
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

#include "Mesh.h"

//--------------------------------------------------------------------------
using namespace std;
using namespace DirectX;
//--------------------------------------------------------------------------
void Mesh::Clear() noexcept
{
	m_kName = "";
	m_stFaces = 0;
	m_stVerts = 0;
	m_kIndices.clear();
	m_kAttributes.clear();
	m_kRepeat.clear();
	m_kAdjacency.clear();
	m_kPosition.clear();
	m_kNormals.clear();
	m_kTangents.clear();
	m_kBiTangents.clear();
	m_kTexcoords.clear();
	m_kColors.clear();
}
//--------------------------------------------------------------------------
void Mesh::Process(float fEpsilon) noexcept
{
	GenerateAdj(fEpsilon);
	ValidateRepeats(fEpsilon);
	RemoveDuplicates(fEpsilon);
	CleanMesh();
	Optimize();
}
//--------------------------------------------------------------------------
void Mesh::GenerateAdj(float fEpsilon) noexcept
{
	m_kRepeat.resize(m_stVerts);
	m_kAdjacency.resize(m_stVerts);
	VE_ASSERT_ALWAYS(SUCCEEDED(GenerateAdjacencyAndPointReps(
		m_kIndices.data(), m_stFaces, m_kPosition.data(), m_stVerts,
		fEpsilon, m_kRepeat.data(), m_kAdjacency.data())));
}
//--------------------------------------------------------------------------
void Mesh::ValidateRepeats(float fEpsilon) noexcept
{
	for (uint32_t i(0); i < m_kRepeat.size(); ++i)
	{
		if (m_kRepeat[i] == i) continue;

		if (!EqualVertexInSamePosition(i, m_kRepeat[i]))
		{
			m_kRepeat[i] = i;
		}
	}
}
//--------------------------------------------------------------------------
void Mesh::RemoveDuplicates(float fEpsilon) noexcept
{
	std::vector<uint32_t> kIndexRemap(m_kRepeat.size());
	uint32_t idx(0);
	for (uint32_t i(0); i < m_kRepeat.size(); ++i)
	{
		if (m_kRepeat[i] == i)
		{
			kIndexRemap[i] = idx;
			m_kPosition[idx] = m_kPosition[i];
			for (auto& v : m_kNormals)
			{
				v[idx] = v[i];
			}
			for (auto& v : m_kTexcoords)
			{
				v[idx] = v[i];
			}
			for (auto& v : m_kColors)
			{
				v[idx] = v[i];
			}
			++idx;
		}
		else
		{
			VE_ASSERT_ALWAYS(m_kRepeat[i] < i);
			kIndexRemap[i] = kIndexRemap[m_kRepeat[i]];
		}
	}
	m_stVerts = idx;
	m_kPosition.resize(m_stVerts);
	for (auto& v : m_kNormals)
	{
		v.resize(m_stVerts);
	}
	for (auto& v : m_kTexcoords)
	{
		v.resize(m_stVerts);
	}
	for (auto& v : m_kColors)
	{
		v.resize(m_stVerts);
	}
	for (uint32_t i(0); i < m_kIndices.size(); ++i)
	{
		m_kIndices[i] = kIndexRemap[m_kIndices[i]];
	}
	GenerateAdj(fEpsilon);
}
//--------------------------------------------------------------------------
void Mesh::CleanMesh() noexcept
{
	std::vector<uint32_t> dups;
	VE_ASSERT_ALWAYS(SUCCEEDED(Clean(m_kIndices.data(),
		m_stFaces, m_stVerts, m_kAdjacency.data(),
		m_kAttributes.size() ? m_kAttributes.data() : nullptr, dups)));

	if (dups.empty()) return;

	size_t nNewVerts = m_stVerts + dups.size();

	m_kPosition.resize(nNewVerts);
	for (auto& v : m_kNormals)
	{
		v.resize(nNewVerts);
	}
	for (auto& v : m_kTexcoords)
	{
		v.resize(nNewVerts);
	}
	for (auto& v : m_kColors)
	{
		v.resize(nNewVerts);
	}
	
	size_t j = m_stVerts;
	for (auto it = dups.begin(); it != dups.end() && (j < nNewVerts); ++it, ++j)
	{
		VE_ASSERT_ALWAYS(*it < m_stVerts);

		m_kPosition[j] = m_kPosition[*it];
		for (auto& v : m_kNormals)
		{
			v[j] = v[*it];
		}
		for (auto& v : m_kTexcoords)
		{
			v[j] = v[*it];
		}
		for (auto& v : m_kColors)
		{
			v[j] = v[*it];
		}
	}

	m_stVerts = nNewVerts;
}
//--------------------------------------------------------------------------
void Mesh::Optimize(uint32_t vertexCache, uint32_t restart) noexcept
{
	{
		std::vector<uint32_t> remap(m_stFaces);
		if (m_kAttributes.size() == m_stFaces)
		{
			// Note that Clean handles vertex splits due to reuse between attributes
			VE_ASSERT_ALWAYS(SUCCEEDED(AttributeSort(m_stFaces,
				m_kAttributes.data(), remap.data())));

			VE_ASSERT_ALWAYS(SUCCEEDED(ReorderIBAndAdjacency(m_kIndices.data(),
				m_stFaces, m_kAdjacency.data(), remap.data())));

			// Optimize faces for pre-transform vertex cache
			VE_ASSERT_ALWAYS(SUCCEEDED(OptimizeFacesEx(m_kIndices.data(),
				m_stFaces, m_kAdjacency.data(), m_kAttributes.data(),
				remap.data(), vertexCache, restart)));
		}
		else
		{
			// Optimize faces for pre-transform vertex cache
			VE_ASSERT_ALWAYS(SUCCEEDED(OptimizeFaces(m_kIndices.data(),
				m_stFaces, m_kAdjacency.data(), remap.data(),
				vertexCache, restart)));
		}

		VE_ASSERT_ALWAYS(SUCCEEDED(ReorderIBAndAdjacency(m_kIndices.data(),
			m_stFaces, m_kAdjacency.data(), remap.data())));
	}

	// Optimize vertices for post-transform vertex cache
	std::vector<uint32_t> remap(m_stVerts);
	VE_ASSERT_ALWAYS(SUCCEEDED(OptimizeVertices(m_kIndices.data(),
		m_stFaces, m_stVerts, remap.data())));
	
	VE_ASSERT_ALWAYS(SUCCEEDED(FinalizeIB(m_kIndices.data(), m_stFaces,
		remap.data(), m_stVerts)));

	VE_ASSERT_ALWAYS(SUCCEEDED(FinalizeVB(m_kPosition.data(),
		sizeof(XMFLOAT3), m_stVerts, remap.data())));

	for (auto& v : m_kNormals)
	{
		VE_ASSERT_ALWAYS(SUCCEEDED(FinalizeVB(v.data(),
			sizeof(XMFLOAT3), m_stVerts, remap.data())));
	}
	for (auto& v : m_kTexcoords)
	{
		VE_ASSERT_ALWAYS(SUCCEEDED(FinalizeVB(v.data(),
			sizeof(XMFLOAT2), m_stVerts, remap.data())));
	}
	for (auto& v : m_kColors)
	{
		VE_ASSERT_ALWAYS(SUCCEEDED(FinalizeVB(v.data(),
			sizeof(XMFLOAT4), m_stVerts, remap.data())));
	}
}
//--------------------------------------------------------------------------
bool Mesh::EqualVertexInSamePosition(uint32_t u32First,
	uint32_t u32Second, float fEpsilon) noexcept
{
	XMFLOAT4 epsi(fEpsilon, fEpsilon, fEpsilon, fEpsilon);
	auto e = XMLoadFloat4(&epsi);
	for (auto& v : m_kNormals)
	{
		auto v1 = XMLoadFloat3(&v[u32First]);
		auto v2 = XMLoadFloat3(&v[u32Second]);
		if (!XMVector3NearEqual(v1, v2, e)) return false;
	}
	for (auto& v : m_kTexcoords)
	{
		auto v1 = XMLoadFloat2(&v[u32First]);
		auto v2 = XMLoadFloat2(&v[u32Second]);
		if (!XMVector2NearEqual(v1, v2, e)) return false;
	}
	for (auto& v : m_kColors)
	{
		auto v1 = XMLoadFloat4(&v[u32First]);
		auto v2 = XMLoadFloat4(&v[u32Second]);
		if (!XMVector4NearEqual(v1, v2, e)) return false;
	}
	return true;
}
//--------------------------------------------------------------------------
