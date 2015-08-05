////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   _VeRendererD3D12.h
//  Created:     2015/08/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef VE_ENABLE_D3D12



class VeRendererD3D12 : public VeRenderer
{
public:
	VeRendererD3D12() noexcept;

	virtual ~VeRendererD3D12() noexcept;

	virtual void Init() noexcept;

	virtual void Term() noexcept;

protected:
	VeSharedLibPtr m_spLib;

};

#endif
