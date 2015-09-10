////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRenderer.h
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeRenderer : public VeRefObject
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

	class VE_MAIN_API RootSignature : public VeRefObject
	{
		VeNoCopy(RootSignature);
		VeRTTIDecl(RootSignature);
	protected:
		RootSignature() noexcept = default;
		virtual ~RootSignature() noexcept = default;

	};

	typedef VePointer<RootSignature> RootSignaturePtr;

	class VE_MAIN_API PipelineState : public VeRefObject
	{
		VeNoCopy(PipelineState);
		VeRTTIDecl(PipelineState);
	protected:
		PipelineState() noexcept = default;
		virtual ~PipelineState() noexcept = default;

	};

	typedef VePointer<PipelineState> PipelineStatePtr;

	inline API GetAPI() const noexcept;

	void RegistResTypes() noexcept;

	void UnregistResTypes() noexcept;

	VeRenderWindowPtr CreateRenderWindow(const VeChar8* pcTitle, VeInt32 x, VeInt32 y,
		VeInt32 w, VeInt32 h, VeUInt32 u32Flags) noexcept;

	virtual bool Init() noexcept = 0;

	virtual void Term() noexcept = 0;

	virtual void BeginSyncCopy() noexcept = 0;

	virtual void EndSyncCopy() noexcept = 0;

	virtual VeRenderWindowPtr CreateRenderWindow(const VeWindowPtr& spWindow) noexcept = 0;

	virtual bool IsSupported(const VeChar8* pcPlatform) noexcept = 0;

	virtual VeShader::Type GetShaderType(const VeChar8* pcTarget) noexcept = 0;

	virtual VeBlobPtr CompileShader(const VeChar8* pcName, const VeChar8* pcTarget,
		const VeChar8* pcConfigPath, VeJSONValue& kConfig,
		const VeResourceManager::FileCachePtr& spCache) noexcept = 0;

	virtual VeBlobPtr SerializeRootSignature(const VeChar8* pcName, VeJSONValue& kConfig,
		const VeResourceGroupPtr& spGroup) noexcept = 0;

	virtual RootSignaturePtr CreateRootSignature(const VeBlobPtr& spBlob) noexcept = 0;

	virtual PipelineStatePtr CreatePipelineState(VeJSONValue& kConfig) noexcept = 0;

	virtual VeRenderBufferPtr CreateBuffer(VeRenderBuffer::Type eType,
		VeRenderBuffer::Useage eUse, VeUInt32 u32Size) noexcept = 0;

protected:
	VeRenderer(API eType) noexcept;

	virtual ~VeRenderer() noexcept;

	const API m_eType;

};

VeSmartPointer(VeRenderer);

#include "VeRenderer.inl"
