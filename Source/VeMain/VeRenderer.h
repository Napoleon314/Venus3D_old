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

	inline API GetAPI() const noexcept;

	void RegistResTypes() noexcept;

	void UnregistResTypes() noexcept;

	virtual bool Init() noexcept = 0;

	virtual void Term() noexcept = 0;

	virtual VeRenderWindowPtr CreateRenderWindow(const VeWindowPtr& spWindow) noexcept = 0;

	virtual bool IsShaderTargetSupported(const VeChar8* pcTarget) noexcept = 0;

	virtual VeShader::Type GetShaderType(const VeChar8* pcTarget) noexcept = 0;

	virtual VeBlobPtr CompileShader(const VeChar8* pcName, const VeChar8* pcTarget,		
		VeJSONValue& kConfig, const VeResourceManager::FileCachePtr& spCache) noexcept = 0;

protected:
	VeRenderer(API eType) noexcept;

	virtual ~VeRenderer() noexcept;

	const API m_eType;

};

VeSmartPointer(VeRenderer);

#include "VeRenderer.inl"
