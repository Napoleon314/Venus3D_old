////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeRootSignature.h
//  Created:     2015/08/31 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeRootSignature : public VeResource
{
	VeNoCopy(VeRootSignature);
	VeRTTIDecl(VeRootSignature, VeResource);
public:
	VeRootSignature(const VeChar8* pcName, const VeChar8* pcFullName) noexcept;

	virtual ~VeRootSignature() noexcept;

	virtual const VeChar8* GetTypeName() const noexcept override;

	static inline const VeChar8* GetName() noexcept;

	static void Regist() noexcept;

	static void Unregist() noexcept;

protected:
	void LoadDirect(const VeRenderer::RootSignaturePtr& spObj) noexcept;

	virtual void Clear() noexcept override;

	static void Parse(const VeResourceManager::FileCachePtr& spCache,
		VeJSONValue& lkValue) noexcept;

	class JSONParser : public VeRefObject
	{
	public:
		JSONParser(const VeResourceManager::JSONCachePtr& spCache,
			VeJSONValue& lkValue) noexcept;

		virtual ~JSONParser() noexcept;

		void UpdateRootSignature() noexcept;

		VeRefNode<VeRefObject*> m_kNode;
		VeResourceManager::JSONCachePtr m_spCache;
		VeJSONValue* m_pkValue = nullptr;
		const VeChar8* m_pcName = nullptr;
		VeResourceManager::FileCachePtr m_spFileCache;
		VeRenderer::RootSignaturePtr m_spObject;

	};

	VeRenderer::RootSignaturePtr m_spObject;

};

VeSmartPointer(VeRootSignature);

#include "VeRootSignature.inl"
