////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeRenderTest
//  File name:   VeRenderTest.cpp
//  Created:     2015/09/26 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VeRenderTest.h"

//--------------------------------------------------------------------------
VeRenderTest::VeRenderTest() noexcept
	: VeApplication("file#config.json")
{

}
//--------------------------------------------------------------------------
VeRenderTest::~VeRenderTest() noexcept
{

}
//--------------------------------------------------------------------------
void VeRenderTest::OnInit() noexcept
{
	m_spTexture = ve_res_mgr.GetResource("stone_d.dds");
	m_spTexture->Load();

	m_spMesh = ve_res_mgr.GetResource("plane.vmesh");
	m_spMesh->Load();

	const VeChar8* apcList[] =
	{
		"final_output"
	};

	m_spMainWindow->SetupCompositorList(apcList, sizeof(apcList) / sizeof(*apcList));
}
//--------------------------------------------------------------------------
void VeRenderTest::OnTerm() noexcept
{
	m_spTexture = nullptr;
}
//--------------------------------------------------------------------------
void VeRenderTest::OnUpdate() noexcept
{
	/*VE_VECTOR eye = { 500,0,500,0 };
	VE_VECTOR focus = { 0,0,0,0 };
	VE_VECTOR up = g_MathIdentityR2;
	VE_MATRIX4X3 view = VeMatrixLookAtLH(eye, focus, up);

	VE_MATRIX view4;
	view4.r[0] = view.r[0];
	view4.r[1] = view.r[1];
	view4.r[2] = view.r[2];
	view4.r[3] = g_MathIdentityR3;
	VE_MATRIX proj = VeMatrixPerspectiveFovLH(VE_MATH_PI_2_F, 720.f / 1280.f, 0.25f, 8000.f);

	view4 = VeMatrixMultiply(view4, proj);*/

	//static VeFloat32 s_Test = 0.0f;
	//s_Test -= ve_time.GetDeltaTime() * 0.3f;

	//VE_FLOAT4A* pvData = (VE_FLOAT4A*)spCB->Map();
	//VE_VECTOR v = { 0, s_Test, 0.5f, 0.5f };
	//VeStoreFloat4A(pvData, v);
	////VeStoreFloat4x4A(pvData, view4);
	//spCB->Unmap();

	//VE_FLOAT4X4A* pvData = (VE_FLOAT4X4A*)spCB->Map();
	//VeStoreFloat4x4A(pvData, view4);
	//spCB->Unmap();

	static VeFloat32 s_f32TimeCount(0);
	static VeUInt32 s_u32FrameCount(0);
	static VeUInt32 s_u32FPS;

	s_f32TimeCount += ve_time.GetDeltaTime();
	++s_u32FrameCount;

	if (s_f32TimeCount > 1.0f)
	{
		s_u32FPS = s_u32FrameCount;
		VeChar8 s_acFPS[64];
		VeSprintf(s_acFPS, "%s[FPS:%d]", (const VeChar8*)m_kString, s_u32FrameCount);
		m_spMainWindow->SetTitle(s_acFPS);
		s_f32TimeCount -= VeFloorf(s_f32TimeCount);
		s_u32FrameCount = 0;
	}
}
//--------------------------------------------------------------------------
void VeRenderTest::OnRender() noexcept
{
	for (VeUInt32 i(0); i < ((VeUInt32)VeCPUInfo::GetCount()); ++i)
	{
		m_spMainWindow->RecordScene(i);
	}

	for (VeUInt32 i(0); i < m_spMainWindow->GetRecorderNum(); ++i)
	{
		m_spMainWindow->Record(i);
	}
}
//--------------------------------------------------------------------------
