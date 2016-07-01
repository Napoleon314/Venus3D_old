////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Venus3D
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   VeSIMDMath.h
//  Created:     2016/07/02 by Albert
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

constexpr float VE_PI = 3.141592654f;
constexpr float VE_2PI = 6.283185307f;
constexpr float VE_1DIVPI = 0.318309886f;
constexpr float VE_1DIV2PI = 0.159154943f;
constexpr float VE_PIDIV2 = 1.570796327f;
constexpr float VE_PIDIV4 = 0.785398163f;

constexpr uint32_t VE_SELECT_0 = 0x00000000;
constexpr uint32_t VE_SELECT_1 = 0xFFFFFFFF;

constexpr uint32_t VE_PERMUTE_0X = 0;
constexpr uint32_t VE_PERMUTE_0Y = 1;
constexpr uint32_t VE_PERMUTE_0Z = 2;
constexpr uint32_t VE_PERMUTE_0W = 3;
constexpr uint32_t VE_PERMUTE_1X = 4;
constexpr uint32_t VE_PERMUTE_1Y = 5;
constexpr uint32_t VE_PERMUTE_1Z = 6;
constexpr uint32_t VE_PERMUTE_1W = 7;

constexpr uint32_t VE_SWIZZLE_X = 0;
constexpr uint32_t VE_SWIZZLE_Y = 1;
constexpr uint32_t VE_SWIZZLE_Z = 2;
constexpr uint32_t VE_SWIZZLE_W = 3;

constexpr uint32_t VE_CRMASK_CR6 = 0x000000F0;
constexpr uint32_t VE_CRMASK_CR6TRUE = 0x00000080;
constexpr uint32_t VE_CRMASK_CR6FALSE = 0x00000020;
constexpr uint32_t VE_CRMASK_CR6BOUNDS = VE_CRMASK_CR6FALSE;

inline constexpr float VeConvertToRadians(float fDegrees) { return fDegrees * (VE_PI / 180.0f); }
inline constexpr float VeConvertToDegrees(float fRadians) { return fRadians * (180.0f / VE_PI); }


