////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Venus3D
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   VeCPUInfo.h
//  Created:     2016/07/01 by Albert
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

enum VeCPUFeature
{
	VE_CPU_HAS_RDTSC		= 0x00000001,
	VE_CPU_HAS_ALTIVEC		= 0x00000002,
	VE_CPU_HAS_MMX			= 0x00000004,
	VE_CPU_HAS_3DNOW		= 0x00000008,
	VE_CPU_HAS_SSE			= 0x00000010,
	VE_CPU_HAS_SSE2			= 0x00000020,
	VE_CPU_HAS_SSE3			= 0x00000040,
	VE_CPU_HAS_SSE41		= 0x00000100,
	VE_CPU_HAS_SSE42		= 0x00000200,
	VE_CPU_HAS_AVX			= 0x00000400,
	VE_CPU_HAS_AVX2			= 0x00000800
};

VENUS_API unsigned int VeGetCPUCount() noexcept;

VENUS_API unsigned int VeGetCPUCacheLineSize() noexcept;

VENUS_API unsigned int VeGetCPUFeatures() noexcept;

VENUS_API unsigned int VeGetSystemRAM() noexcept;

inline bool VeHasRDTSC() noexcept
{
	return VE_MASK_HAS_ANY(VeGetCPUFeatures(), VE_CPU_HAS_RDTSC);
}

inline bool VeHasAltiVec() noexcept
{
	return VE_MASK_HAS_ANY(VeGetCPUFeatures(), VE_CPU_HAS_ALTIVEC);
}

inline bool VeHasMMX() noexcept
{
	return VE_MASK_HAS_ANY(VeGetCPUFeatures(), VE_CPU_HAS_MMX);
}

inline bool VeHas3DNow() noexcept
{
	return VE_MASK_HAS_ANY(VeGetCPUFeatures(), VE_CPU_HAS_3DNOW);
}

inline bool VeHasSSE() noexcept
{
	return VE_MASK_HAS_ANY(VeGetCPUFeatures(), VE_CPU_HAS_SSE);
}

inline bool VeHasSSE2() noexcept
{
	return VE_MASK_HAS_ANY(VeGetCPUFeatures(), VE_CPU_HAS_SSE2);
}

inline bool VeHasSSE3() noexcept
{
	return VE_MASK_HAS_ANY(VeGetCPUFeatures(), VE_CPU_HAS_SSE3);
}

inline bool VeHasSSE41() noexcept
{
	return VE_MASK_HAS_ANY(VeGetCPUFeatures(), VE_CPU_HAS_SSE41);
}

inline bool VeHasSSE42() noexcept
{
	return VE_MASK_HAS_ANY(VeGetCPUFeatures(), VE_CPU_HAS_SSE42);
}

inline bool VeHasAVX() noexcept
{
	return VE_MASK_HAS_ANY(VeGetCPUFeatures(), VE_CPU_HAS_AVX);
}

inline bool VeHasAVX2() noexcept
{
	return VE_MASK_HAS_ANY(VeGetCPUFeatures(), VE_CPU_HAS_AVX2);
}
