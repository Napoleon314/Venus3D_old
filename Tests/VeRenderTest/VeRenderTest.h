////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeRenderTest
//  File name:   VeRenderTest.h
//  Created:     2015/09/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <VeFrameworkPch.h>

class VeRenderTest : public VeApplication
{
public:
	VeRenderTest() noexcept;

	virtual ~VeRenderTest() noexcept override;

	virtual void OnInit() noexcept override;

	virtual void OnTerm() noexcept override;

	virtual void OnUpdate() noexcept override;

protected:
	VeResourcePtr m_spTexture;

};
