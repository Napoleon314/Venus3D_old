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

VeSmartPointer(VeRenderBuffer);

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

	enum ShaderType
	{
		TYPE_VS,
		TYPE_PS,
		TYPE_GS,
		TYPE_HS,
		TYPE_DS,
		TYPE_CS,
		SHADER_TYPE_NUM
	};

	enum BlendType
	{
		REPLACE,
		ADD,
		BLEND
	};

	enum RasterType
	{
		CULL_BACK,
		CULL_FRONT,
		CULL_NONE
	};

	enum DepthStencilType
	{
		DS_NONE,
		DS_STANDARD
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

	void PreCache(const VeDirectoryPtr& spCache, const VeDirectoryPtr& spShader,
		const VeDirectoryPtr& spScript) noexcept;

	VeRenderWindowPtr CreateRenderWindow(const VeChar8* pcTitle, VeInt32 x, VeInt32 y,
		VeInt32 w, VeInt32 h, VeUInt32 u32Flags) noexcept;

	virtual bool Init() noexcept = 0;

	virtual void Term() noexcept = 0;

	virtual void BeginSyncCopy() noexcept = 0;

	virtual void EndSyncCopy() noexcept = 0;

	virtual VeRenderWindowPtr CreateRenderWindow(const VeWindowPtr& spWindow) noexcept = 0;

	virtual std::pair<VeBlobPtr, ShaderType> CompileShader(const VeChar8* pcFile,
		const VeChar8* pcTarget, const VeChar8* pcPath, VeJSONValue& kConfig,
		const VeStringMap<VeUInt32>& kShaderNameMap,
		const VeVector<VeBlobPtr>& kShaderList) noexcept = 0;

	virtual VeBlobPtr SerializeRootSignature(VeJSONValue& kConfig) noexcept = 0;

	virtual RootSignaturePtr CreateRootSignature(const VeBlobPtr& spBlob) noexcept = 0;

	virtual PipelineStatePtr CreatePipelineState(VeJSONValue& kConfig, VeBlobPtr& spOut) noexcept = 0;

	virtual VeRenderBufferPtr CreateBuffer(VeRenderBuffer::Type eType,
		VeRenderBuffer::Useage eUse, VeUInt32 u32Size) noexcept = 0;

protected:
	VeRenderer(API eType) noexcept;

	virtual ~VeRenderer() noexcept;

	const API m_eType;
	VeStringMap<VeBlobPtr> m_akShaderMap[SHADER_TYPE_NUM];
	VeStringMap<RootSignaturePtr> m_kRootSignatureMap;
	VeStringMap<PipelineStatePtr> m_kPipelineStateMap;

};

VeSmartPointer(VeRenderer);

#include "VeRenderer.inl"
