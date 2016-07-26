////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      D3D12
//  File name:   VeRendererD3D12.h
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

#ifdef VE_ENABLE_D3D12

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

#define LIB_D3D12 "d3d12.dll"
#define LIB_DXGI "dxgi.dll"
#define LIB_D3DCOMPLIER "d3dcompiler_47.dll"

inline D3D12_RESOURCE_BARRIER BarrierTransition(
	ID3D12Resource* pkResource,
	D3D12_RESOURCE_STATES eStateBefore,
	D3D12_RESOURCE_STATES eStateAfter,
	UINT u32Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
	D3D12_RESOURCE_BARRIER_FLAGS eFlags = D3D12_RESOURCE_BARRIER_FLAG_NONE) noexcept
{
	D3D12_RESOURCE_BARRIER result;
	result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	result.Flags = eFlags;
	result.Transition.pResource = pkResource;
	result.Transition.StateBefore = eStateBefore;
	result.Transition.StateAfter = eStateAfter;
	result.Transition.Subresource = u32Subresource;
	return result;
}

template <D3D12_DESCRIPTOR_HEAP_TYPE TYPE, uint32_t NUM, D3D12_DESCRIPTOR_HEAP_FLAGS FLAGS>
class D3D12HeapShell
{
public:
	D3D12HeapShell() noexcept
	{
		m_u32FreeStart.store(0, std::memory_order_relaxed);
	}

	void Init(ID3D12Device* pkDevice) noexcept
	{
		D3D12_DESCRIPTOR_HEAP_DESC kHeapDesc =
		{
			TYPE, NUM, FLAGS, 0
		};
		VE_ASSERT_GE(pkDevice->CreateDescriptorHeap(
			&kHeapDesc, IID_PPV_ARGS(&m_pkHeap)), S_OK);
		VE_ASSERT(m_pkHeap);
		m_u32DescIncSize = pkDevice->GetDescriptorHandleIncrementSize(TYPE);
	}

	void Term() noexcept
	{
		VE_ASSERT(m_u32FreeStart == m_kFreeIndexList.size());
		VE_SAFE_RELEASE(m_pkHeap);
	}

	uint32_t Alloc() noexcept
	{
		uint32_t u32Res = m_kFreeIndexList.pop();
		if (u32Res < NUM)
		{
			return u32Res;
		}
		else
		{
			do 
			{
				u32Res = m_u32FreeStart.load(std::memory_order_relaxed);
				if (u32Res >= NUM)
				{
					VE_ASSERT(!"Can not alloc descriptor heap.");
					return UINT_MAX;
				}
			} while (! m_u32FreeStart.compare_exchange_weak(u32Res, u32Res + 1, std::memory_order_relaxed));

			return u32Res * m_u32DescIncSize;
		}
	}

	void Free(uint32_t u32Pointer) noexcept
	{
		VE_ASSERT(u32Pointer % m_u32DescIncSize == 0
			&& (u32Pointer / m_u32DescIncSize) < NUM);
		m_kFreeIndexList.push(u32Pointer);
	}

	ID3D12DescriptorHeap* Get() noexcept
	{
		return m_pkHeap;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUStart() noexcept
	{
		return m_pkHeap->GetCPUDescriptorHandleForHeapStart();
	}

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUStart() noexcept
	{
		return m_pkHeap->GetGPUDescriptorHandleForHeapStart();
	}

private:
	ID3D12DescriptorHeap* m_pkHeap = nullptr;
	uint32_t m_u32DescIncSize = 0;
	std::atomic<uint32_t> m_u32FreeStart;
	vtd::ring_buffer<uint32_t, UINT_MAX, NUM> m_kFreeIndexList;

};

class VeRendererD3D12 : public VeRenderer
{
	VeNoCopy(VeRendererD3D12);
	VeRTTIDecl(VeRendererD3D12);
public:
	static constexpr uint32_t FRAME_COUNT = 3;
	static constexpr uint32_t MAX_MRT_COUNT = 8;
	static constexpr uint32_t RTV_COUNT = 64;
	static constexpr uint32_t DSV_COUNT = 32;
	static constexpr uint32_t SRV_COUNT = 4096;

	typedef D3D12HeapShell<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_COUNT, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> RTVHeap;
	typedef D3D12HeapShell<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, DSV_COUNT, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> DSVHeap;
	typedef D3D12HeapShell<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, SRV_COUNT, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> SRVHeap;

	VeRendererD3D12() noexcept;

	virtual ~VeRendererD3D12() noexcept;

	virtual void Init() override;

	virtual void Term() override;

	virtual void BeginSyncCopy() noexcept override;

	virtual void EndSyncCopy() noexcept override;

	virtual VeRenderWindowPtr CreateRenderWindow(const VeWindowPtr& spWindow) noexcept override;

protected:
	friend class D3D12RenderWindow;

	VeSharedLibPtr m_spD3D12;
	VeSharedLibPtr m_spDXGI;
	VeSharedLibPtr m_spD3DCompiler;

	IDXGIFactory1* m_pkDXGIFactory = nullptr;
	IDXGIAdapter1* m_pkDefaultAdapter = nullptr;
	ID3D12Device* m_pkDevice = nullptr;

	RTVHeap m_kRTVHeap;
	DSVHeap m_kDSVHeap;
	SRVHeap m_kSRVHeap;

	vtd::intrusive_list<D3D12RenderWindow*> m_kRenderWindowList;

	HRESULT(WINAPI* D3D12GetDebugInterface)(
		_In_ REFIID riid, _COM_Outptr_opt_ void** ppvDebug) = nullptr;

	HRESULT(WINAPI* D3D12CreateDevice)(
		_In_opt_ IUnknown* pAdapter,
		D3D_FEATURE_LEVEL MinimumFeatureLevel,
		_In_ REFIID riid,
		_COM_Outptr_opt_ void** ppDevice) = nullptr;

	HRESULT(WINAPI* D3D12SerializeRootSignature)(
		_In_ const D3D12_ROOT_SIGNATURE_DESC* pRootSignature,
		_In_ D3D_ROOT_SIGNATURE_VERSION Version,
		_Out_ ID3DBlob** ppBlob,
		_Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob) = nullptr;

	HRESULT(WINAPI* CreateDXGIFactory1)(
		REFIID riid, _COM_Outptr_ void **ppFactory) = nullptr;

	HRESULT(WINAPI* D3DCompile)(
		LPCVOID pSrcData, SIZE_T SrcDataSize, LPCSTR pFileName,
		CONST D3D_SHADER_MACRO* pDefines, ID3DInclude* pInclude,
		LPCSTR pEntrypoint, LPCSTR pTarget, UINT Flags1, UINT Flags2,
		ID3DBlob** ppCode, ID3DBlob** ppErrorMsgs) = nullptr;

};

VeRendererPtr CreateRendererD3D12() noexcept;

#endif
