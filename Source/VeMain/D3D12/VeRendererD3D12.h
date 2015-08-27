////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRendererD3D12.h
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef VE_ENABLE_D3D12

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

using namespace Microsoft::WRL;

#define D3D_COMPLIER "d3dcompiler_47"

class VeRendererD3D12 : public VeRenderer
{
	VeNoCopy(VeRendererD3D12);
	VeRTTIDecl(VeRendererD3D12, VeRenderer);
public:
	VeRendererD3D12() noexcept;

	virtual ~VeRendererD3D12() noexcept;

	virtual bool Init() noexcept override;

	virtual void Term() noexcept override;

	virtual VeRenderWindowPtr CreateRenderWindow(const VeWindowPtr& spWindow) noexcept override;

	virtual bool IsShaderTargetSupported(const VeChar8* pcTarget) noexcept override;

	virtual VeShader::Type GetShaderType(const VeChar8* pcTarget) noexcept override;

	virtual VeBlobPtr CompileShader(const VeChar8* pcName, const VeChar8* pcTarget,
		const VeChar8* pcConfigPath, VeJSONValue& kConfig,
		const VeResourceManager::FileCachePtr& spCache) noexcept override;

protected:
	VeSharedLibPtr m_spD3D12;
	VeSharedLibPtr m_spDXGI;
	VeSharedLibPtr m_spD3DCompiler;
	ComPtr<ID3D12Device> m_cpDevice;

	HRESULT (WINAPI* D3D12GetDebugInterface)(
		_In_ REFIID riid, _COM_Outptr_opt_ void** ppvDebug) = nullptr;
	HRESULT (WINAPI* D3D12CreateDevice)(
		_In_opt_ IUnknown* pAdapter,
		D3D_FEATURE_LEVEL MinimumFeatureLevel,
		_In_ REFIID riid,
		_COM_Outptr_opt_ void** ppDevice) = nullptr;

	HRESULT(WINAPI* CreateDXGIFactory1)(
		REFIID riid, _COM_Outptr_ void **ppFactory) = nullptr;

	HRESULT(WINAPI *D3DCompile)(
		LPCVOID pSrcData, SIZE_T SrcDataSize, LPCSTR pFileName,
		CONST D3D_SHADER_MACRO* pDefines, ID3DInclude* pInclude,
		LPCSTR pEntrypoint, LPCSTR pTarget, UINT Flags1, UINT Flags2,
		ID3DBlob** ppCode, ID3DBlob** ppErrorMsgs) = nullptr;

};

VeRendererPtr CreateRendererD3D12() noexcept;

#endif
