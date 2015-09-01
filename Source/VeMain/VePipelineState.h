////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VePipelineState.h
//  Created:     2015/09/01 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VePipelineState : public VeResource
{
	VeNoCopy(VePipelineState);
	VeRTTIDecl(VePipelineState, VeResource);
public:
	VePipelineState(const VeChar8* pcName, const VeChar8* pcFullName) noexcept;

	virtual ~VePipelineState() noexcept;

	virtual const VeChar8* GetTypeName() const noexcept override;

	static inline const VeChar8* GetName() noexcept;

	static void Regist() noexcept;

	static void Unregist() noexcept;

protected:
	void LoadDirect(const VeRenderer::PipelineStatePtr& spObj) noexcept;

	virtual void Clear() noexcept override;

	static void Parse(const VeResourceManager::FileCachePtr& spCache,
		VeJSONValue& lkValue) noexcept;

	VeRenderer::PipelineStatePtr m_spObject;

};

VeSmartPointer(VePipelineState);

#include "VePipelineState.inl"
