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

	enum PrimitiveTopologyType
	{
		PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED = 0,
		PRIMITIVE_TOPOLOGY_TYPE_POINT = 1,
		PRIMITIVE_TOPOLOGY_TYPE_LINE = 2,
		PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE = 3,
		PRIMITIVE_TOPOLOGY_TYPE_PATCH = 4
	};

	enum FramePassType
	{
		PASS_CLEAR,
		PASS_RENDER_QUAD,
		PASS_MAX
	};
	
	enum FrameTargetType
	{
		TARGET_NONE,
		TARGET_INPUT,
		TARGET_OUTPUT
	};

	enum ClearFlag
	{
		CLEAR_COLOR = 0x1,
		CLEAR_DEPTH = 0x2,
		CLEAR_STENCIL = 0x4,
		CLEAR_ALL = 0xf
	};

	struct FramePass : public VeRefObject
	{
		FramePassType m_eType;		
	};

	typedef VePointer<FramePass> FramePassPtr;

	struct FrameClear : public FramePass
	{
		VeUInt32 m_u32Flags = 0;
		VeVector<VeColor> m_kColorArray;
		VeFloat32 m_f32Depth = 0;
		VeUInt8 m_u8Stencil = 0;
	};

	struct FrameClick
	{
		VeFixedString m_kTarget;
		VeVector<FramePassPtr> m_kPassList;
	};

	struct FrameResource
	{
		VeFixedString m_kName;
		VeRenderResource::Dimension m_eDimension;
		VeRenderResource::Format m_eFormat;
		VeUInt32 m_u32Width;
		VeUInt32 m_u32Height;
		VeUInt16 m_u16Depth;
		VeUInt16 m_u16MipLevels;
		VeUInt16 m_u16Count;
		VeUInt16 m_u16Quality;
	};	

	struct FrameRTV
	{
		VeFixedString m_kResName;
		VeUInt32 m_u32Param0;
		VeUInt32 m_u32Param1;
		VeUInt32 m_u32Param2;
		VeUInt32 m_u32Param3;
	};

	struct FrameDSV
	{
		VeFixedString m_kResName;
		VeUInt32 m_u32Param0;
		VeUInt32 m_u32Param1;
		VeUInt32 m_u32Param2;
		VeUInt32 m_u32Param3;
	};	

	struct FrameTarget
	{
		VeFixedString m_kName;
		FrameTargetType m_eType;
		VeVector<FrameRTV> m_kRTVList;
		FrameDSV m_kDSV;
	};

	struct FrameTechnique
	{
		VeFixedString m_kName;
		VeVector<FrameResource> m_kResourceList;
		VeVector<FrameTarget> m_kTargetList;
		VeVector<FrameClick> m_kClickList;
	};

	struct FrameCompositor : public VeRefObject
	{
		VeVector<FrameTechnique> m_kTechniqueList;
	};

	typedef VePointer<FrameCompositor> FrameCompositorPtr;

	class VE_MAIN_API RootSignature : public VeRefObject
	{
		VeNoCopy(RootSignature);
		VeRTTIDecl(RootSignature);
	protected:
		RootSignature() noexcept = default;
		virtual ~RootSignature() noexcept = default;

	};

	typedef VePointer<RootSignature> RootSignaturePtr;

	class VE_MAIN_API Binding : public VeRefObject
	{
		VeNoCopy(Binding);
		VeRTTIDecl(Binding);
	public:
		virtual void ClearTableList() noexcept = 0;

		virtual void AddContextIndex(VeUInt32 u32Index,
			VeSizeT stContextIndex) noexcept = 0;

		virtual void AddCustomer(VeUInt32 u32Index,
			VeSizeT stCustomerIndex) noexcept = 0;

		virtual void AddResource(VeUInt32 u32Index,
			const VeRenderResourcePtr& spResource) noexcept = 0;

	protected:
		Binding() noexcept = default;
		virtual ~Binding() noexcept = default;

	};

	typedef VePointer<Binding> BindingPtr;

	class VE_MAIN_API PipelineState : public VeRefObject
	{
		VeNoCopy(PipelineState);
		VeRTTIDecl(PipelineState);
	protected:
		PipelineState() noexcept = default;
		virtual ~PipelineState() noexcept = default;

	};

	typedef VePointer<PipelineState> PipelineStatePtr;

	class VE_MAIN_API Geometry : public VeRefObject
	{
		VeNoCopy(Geometry);
		VeRTTIDecl(Geometry);
	public:
		virtual bool IsValid() noexcept = 0;

		virtual void SetPrimitiveTopologyType(PrimitiveTopologyType eType) noexcept = 0;

		virtual void SetVertexBufferNum(VeUInt32 u32Num) noexcept = 0;

		virtual void SetVertexBuffer(VeUInt32 u32Index, const VeRenderBufferPtr& spBuffer,
			VeUInt32 u32Stride) noexcept = 0;

		virtual void SetVertexBuffer(VeUInt32 u32Index, const VeRenderBufferPtr& spBuffer,
			VeUInt32 u32Offset, VeUInt32 u32Size, VeUInt32 u32Stride) noexcept = 0;

		virtual void ClearIndexBuffer() noexcept = 0;

		virtual void SetIndexBuffer(const VeRenderBufferPtr& spBuffer,
			bool bUse32Bit = false) noexcept = 0;

		virtual void SetIndexBuffer(const VeRenderBufferPtr& spBuffer, VeUInt32 u32Offset,
			VeUInt32 u32Size, bool bUse32Bit = false) noexcept = 0;

	protected:
		Geometry() noexcept = default;
		virtual ~Geometry() noexcept = default;

	};

	typedef VePointer<Geometry> GeometryPtr;	

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

	virtual GeometryPtr CreateGeometry() noexcept = 0;

	virtual BindingPtr CreateBinding() noexcept = 0;

	virtual VeRenderBufferPtr CreateBuffer(VeRenderBuffer::Type eType,
		VeRenderBuffer::Useage eUse, VeUInt32 u32Size) noexcept = 0;

protected:
	VeRenderer(API eType) noexcept;

	virtual ~VeRenderer() noexcept;

	FrameCompositorPtr CreateCompositor(VeJSONValue& kValue) noexcept;

	void SetTechnique(FrameTechnique& kTechnique, VeJSONValue& kValue) noexcept;

	void SetResource(FrameResource& kResource, VeJSONValue& kValue) noexcept;

	void SetTarget(FrameTarget& kTarget, VeJSONValue& kValue) noexcept;

	void SetRTV(FrameRTV& kRTV, VeJSONValue& kValue) noexcept;

	void SetDSV(FrameDSV& kDSV, VeJSONValue& kValue) noexcept;

	void SetClick(FrameClick& kClick, VeJSONValue& kValue) noexcept;

	FramePassPtr CreatePass(VeJSONValue& kValue) noexcept;

	FramePassPtr CreateClear(VeJSONValue& kValue) noexcept;

	const API m_eType;
	VeStringMap<VeBlobPtr> m_akShaderMap[SHADER_TYPE_NUM];
	VeStringMap<RootSignaturePtr> m_kRootSignatureMap;
	VeStringMap<PipelineStatePtr> m_kPipelineStateMap;
	VeStringMap<FrameCompositorPtr> m_kCompositorMap;

};

VeSmartPointer(VeRenderer);

struct VeRenderDrawCall
{
	VeRenderer::RootSignature* m_pkRoot;
	VeRenderer::PipelineState* m_pkState;
	VeRenderer::Binding* m_pkBinding;
	VeRenderer::Geometry* m_pkGeometry;
	VeRenderResource* m_apkCustomerResList[3];
	VeUInt32 m_u32InstanceCount;
	VeUInt32 m_u32Group;
};

#include "VeRenderer.inl"
