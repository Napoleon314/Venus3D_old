////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Video
//  File name:   VeVideo.h
//  Created:     2016/07/24 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#pragma once

enum VeMessageBoxFlag
{
	//Button
	VE_MB_OK						= 0x00000000,
	VE_MB_OKCANCEL					= 0x00000001,
	VE_MB_ABORTRETRYIGNORE			= 0x00000002,
	VE_MB_YESNOCANCEL				= 0x00000003,
	VE_MB_YESNO						= 0x00000004,
	VE_MB_RETRYCANCEL				= 0x00000005,

	//Icon
	VE_MB_ERROR						= 0x00000010,
	VE_MB_QUESTION					= 0x00000020,
	VE_MB_WARNING					= 0x00000030,
	VE_MB_INFORMATION				= 0x00000040,

	//
	VE_MB_APPLMODAL					= 0x00000000,
	VE_MB_SYSTEMMODAL				= 0x00001000,
	VE_MB_TASKMODAL					= 0x00002000
};

enum VeMessageBoxReturn
{
	VE_MB_IDOK						= 0x00000001,
	VE_MB_IDCANCEL					= 0x00000002,
	VE_MB_IDABORT					= 0x00000003,
	VE_MB_IDRETRY					= 0x00000004,
	VE_MB_IDIGNORE					= 0x00000005,
	VE_MB_IDYES						= 0x00000006,
	VE_MB_IDNO						= 0x00000007
};

class VENUS_API VeVideo : public VeRefObject
{
	VeNoCopy(VeVideo);
	VeRTTIDecl(VeVideo);
public:



	VeVideo(const char* pcName) noexcept;

	virtual ~VeVideo() noexcept;

	inline const vtd::string& GetDriverName() const noexcept;

	virtual void Init() = 0;

	virtual void Term() = 0;

	virtual int32_t MessageBoxSync(const char* pcCaption,
		const char* pcText, uint32_t u32Flags) noexcept = 0;

protected:
	const vtd::string m_kName;

};

VeSmartPointer(VeVideo);

class VENUS_API VeDesktopVideo : public VeVideo
{
	VeNoCopy(VeDesktopVideo);
	VeRTTIDecl(VeDesktopVideo, VeVideo);
public:
	VeDesktopVideo(const char* pcName) noexcept;

	virtual ~VeDesktopVideo() noexcept;

	virtual void Init();

	virtual void Term();

protected:


};

VeSmartPointer(VeDesktopVideo);

#include "VeVideo.inl"
