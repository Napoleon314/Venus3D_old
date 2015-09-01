////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeShader.h
//  Created:     2015/08/25 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_MAIN_API VeShader : public VeResource
{
	VeNoCopy(VeShader);
	VeRTTIDecl(VeShader, VeResource);
public:
	enum Type
	{
		TYPE_VS,
		TYPE_PS,
		TYPE_GS,
		TYPE_HS,
		TYPE_DS,
		TYPE_CS,
		TYPE_NUM
	};

	VeShader(Type eType, const VeChar8* pcName, const VeChar8* pcFullName) noexcept;

	virtual ~VeShader() noexcept;

	virtual const VeChar8* GetTypeName() const noexcept override;

	static inline Type GetType(const VeChar8* pcName) noexcept;

	static inline const VeChar8* GetTypeName(Type eType) noexcept;

	static void Regist() noexcept;

	static void Unregist() noexcept;

protected:
	void LoadDirect(const VeBlobPtr& spBlob) noexcept;

	virtual void ParseStream(const VeMemoryIStreamPtr& spStream) noexcept override;

	virtual void Clear() noexcept override;

	static void Parse(const VeResourceManager::FileCachePtr& spCache,
		VeJSONValue& lkValue) noexcept;

	class JSONParser : public VeRefObject
	{
	public:
		JSONParser(const VeResourceManager::JSONCachePtr& spCache,
			VeJSONValue& lkValue) noexcept;

		virtual ~JSONParser() noexcept;

		void UpdateShader() noexcept;

		VeRefNode<VeRefObject*> m_kNode;
		VeResourceManager::JSONCachePtr m_spCache;
		VeJSONValue* m_pkValue = nullptr;
		const VeChar8* m_pcName = nullptr;
		const VeChar8* m_pcFile = nullptr;
		const VeChar8* m_pcTarget = nullptr;
		VeResourceManager::FileCachePtr m_spFileCache;
		VeShader::Type m_eType;
		VeBlobPtr m_spBlob;
		VeResourceGroupPtr m_spGroup;

	};

	const Type m_eType;
	VeBlobPtr m_spBlob;

};

VeSmartPointer(VeShader);

#include "VeShader.inl"
