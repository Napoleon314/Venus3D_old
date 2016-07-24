////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Video
//  File name:   VeWindow.h
//  Created:     2016/07/25 by Albert
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

enum VeWindowFlag
{
	VE_WINDOW_MINIMIZED					= 0x00000001,
	VE_WINDOW_MAXIMIZED					= 0x00000002,
	VE_WINDOW_SHOWN						= 0x00000010,
	VE_WINDOW_BORDERLESS				= 0x00000020,
	VE_WINDOW_RESIZABLE					= 0x00000040,
	VE_WINDOW_ALLOW_HIGHDPI				= 0x00000080,
	VE_WINDOW_FOREIGN					= 0x00000100,
	VE_WINDOW_VALID						= 0x80000000
};

#define VE_WINDOWPOS_UNDEFINED_MASK			0x1FFF0000
#define VE_WINDOWPOS_UNDEFINED_DISPLAY(X)	(VE_WINDOWPOS_UNDEFINED_MASK|(X))
#define VE_WINDOWPOS_UNDEFINED				VE_WINDOWPOS_UNDEFINED_DISPLAY(0)
#define VE_WINDOWPOS_ISUNDEFINED(X)			(((X)&0xFFFF0000) == VE_WINDOWPOS_UNDEFINED_MASK)

#define VE_WINDOWPOS_CENTERED_MASK			0x2FFF0000
#define VE_WINDOWPOS_CENTERED_DISPLAY(X)	(VE_WINDOWPOS_CENTERED_MASK|(X))
#define VE_WINDOWPOS_CENTERED				VE_WINDOWPOS_CENTERED_DISPLAY(0)
#define VE_WINDOWPOS_ISCENTERED(X)			(((X)&0xFFFF0000) == VE_WINDOWPOS_CENTERED_MASK)

VeSmartPointer(VeWindow);
VeSmartPointer(VeDesktopWindow);

class VENUS_API VeWindow : public VeRefObject
{
	VeNoCopy(VeWindow);
	VeRTTIDecl(VeWindow);
public:
	VeWindow() noexcept;

	virtual ~VeWindow() noexcept = default;

	virtual void* GetNativeHandle() noexcept = 0;
	
protected:
	static uint32_t ms_u32Accumulator;

	const uint32_t m_u32Index = 0;
	uint32_t m_u32Flags = 0;

};

class VENUS_API VeDesktopWindow : public VeWindow
{
	VeNoCopy(VeDesktopWindow);
	VeRTTIDecl(VeDesktopWindow, VeWindow);
public:
	VeDesktopWindow() noexcept;

	virtual ~VeDesktopWindow() noexcept;

protected:
	VeDesktopWindowPtr m_spParent;
	vtd::intrusive_node<VeDesktopWindow*> m_kNode;
	vtd::intrusive_list<VeDesktopWindow*> m_kChildList;

};
