////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRendererD3D12.cpp
//  Created:     2015/08/22 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "../VeMainPch.h"
#include "VeRendererD3D12.h"

//--------------------------------------------------------------------------
#ifdef VE_ENABLE_D3D12
//--------------------------------------------------------------------------
VeRTTIImpl(VeRendererD3D12, VeRenderer);
//--------------------------------------------------------------------------
VeRendererD3D12::VeRendererD3D12() noexcept
	: VeRenderer(API_D3D12)
{

}
//--------------------------------------------------------------------------
VeRendererD3D12::~VeRendererD3D12() noexcept
{
	
}
//--------------------------------------------------------------------------
bool VeRendererD3D12::Init() noexcept
{
	m_spD3D12 = VE_NEW VeSharedLib(VE_SHARED_LIB_NAME("d3d12"));
	if (!m_spD3D12->Load()) return false;
	m_spDXGI = VE_NEW VeSharedLib(VE_SHARED_LIB_NAME("dxgi"));
	if (!m_spDXGI->Load()) return false;
	m_spD3DCompiler = VE_NEW VeSharedLib(VE_SHARED_LIB_NAME(D3D_COMPLIER));
	if (!m_spD3DCompiler->Load()) return false;

	D3D12GetDebugInterface = (decltype(D3D12GetDebugInterface))
		m_spD3D12->GetProc("D3D12GetDebugInterface");
	if (!D3D12GetDebugInterface) return false;

	D3D12CreateDevice = (decltype(D3D12CreateDevice))
		m_spD3D12->GetProc("D3D12CreateDevice");
	if (!D3D12CreateDevice) return false;

	CreateDXGIFactory1 = (decltype(CreateDXGIFactory1))
		m_spDXGI->GetProc("CreateDXGIFactory1");
	if (!CreateDXGIFactory1) return false;

	D3DCompile = (decltype(D3DCompile))
		m_spD3DCompiler->GetProc("D3DCompile");
	if (!D3DCompile) return false;

#	ifdef VE_DEBUG
	{
		ComPtr<ID3D12Debug> cpDebugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&cpDebugController))))
		{
			cpDebugController->EnableDebugLayer();
		}
	}
#	endif

	if (VE_FAILED(D3D12CreateDevice(nullptr,
		D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_cpDevice))))
		return false;

	VeCoreLogI("VeRendererD3D12 renderer is created.");

	return true;
}
//--------------------------------------------------------------------------
void VeRendererD3D12::Term() noexcept
{
	m_cpDevice = nullptr;
	D3D12GetDebugInterface = nullptr;
	D3D12CreateDevice = nullptr;
	CreateDXGIFactory1 = nullptr;
	m_spD3D12 = nullptr;
	m_spDXGI = nullptr;
}
//--------------------------------------------------------------------------
VeRenderWindowPtr VeRendererD3D12::CreateRenderWindow(
	const VeWindowPtr& spWindow) noexcept
{
	return nullptr;
}
//--------------------------------------------------------------------------
bool VeRendererD3D12::IsShaderTargetSupported(
	const VeChar8* pcTarget) noexcept
{
	VeChar8 acBuffer[VE_MAX_PATH_LEN];
	VeStrcpy(acBuffer, pcTarget);
	VeChar8* pcContext;
	VeChar8* pcTemp = VeStrtok(acBuffer, "|", &pcContext);
	while (pcTemp)
	{
		if (VeStricmp(pcTemp, "d12") == 0)
		{
			return true;
		}
		pcTemp = VeStrtok<VeChar8>(nullptr, "|", &pcContext);
	}
	return false;
}
//--------------------------------------------------------------------------
VeShader::Type VeRendererD3D12::GetShaderType(
	const VeChar8* pcTarget) noexcept
{
	switch (VeToLower(*pcTarget))
	{
	case 'v':
		return VeShader::TYPE_VS;
	case 'p':
		return VeShader::TYPE_PS;
	case 'g':
		return VeShader::TYPE_GS;
	case 'h':
		return VeShader::TYPE_HS;
	case 'd':
		return VeShader::TYPE_DS;
	case 'c':
		return VeShader::TYPE_CS;
	default:
		return VeShader::TYPE_NUM;
	}
}
//--------------------------------------------------------------------------
VeBlobPtr VeRendererD3D12::CompileShader(const VeChar8* pcName,
	const VeChar8* pcTarget, VeJSONValue& kConfig,
	const VeResourceManager::FileCachePtr& spCache) noexcept
{
	const VeBlobPtr& spHLSL = spCache->GetData()->GetBlob();
	const VeChar8* pcEntry = kConfig["e"].GetString();
#	ifdef VE_DEBUG
	VeUInt32 u32CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#	else
	VeUInt32 u32CompileFlags = 0;
#	endif
	VeBlobPtr spBlob;
	ID3DBlob* pkCode = nullptr;
	ID3DBlob* pkError = nullptr;
	if (SUCCEEDED(D3DCompile(spHLSL->GetBuffer(), spHLSL->GetSize(), spCache->GetFullPath(),
		nullptr, nullptr, pcEntry, pcTarget, u32CompileFlags, 0, &pkCode, &pkError)))
	{
		spBlob = VE_NEW VeBlob(pkCode->GetBufferSize());
		VeMemoryCopy(spBlob->GetBuffer(), pkCode->GetBufferPointer(), pkCode->GetBufferSize());
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeSprintf(acBuffer, "%s.%s", pcName, VeShader::GetTypeName(GetShaderType(pcTarget)));
		spCache->GetGroup()->GetWriteDir()->WriteAsync(acBuffer, VE_NEW VeMemoryOStream(spBlob));
	}
	else
	{
		spBlob = nullptr;
	}
	VE_SAFE_RELEASE(pkCode);
	VE_SAFE_RELEASE(pkError);
	return spBlob;
}
//--------------------------------------------------------------------------
VeRendererPtr CreateRendererD3D12() noexcept
{
	return VE_NEW VeRendererD3D12();
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
