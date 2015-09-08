//*****************************************************************************/
// Copyright (c) 1998-2014 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//*****************************************************************************/

#pragma once

inline IMRPhysicalSkyEnvironmentMap2::ShadingParameters::ShadingParameters()
    : sunDirection(0.0f, 0.0f, 0.0f),
    multiplier(0.0f),
    rgbUnitConversion(0.0f, 0.0f, 0.0f, 0.0f),
    haze(0.0f),
    hazeTexture(nullptr),
    redBlueShift(0.0f),
    redBlueShiftTexture(nullptr),
    saturationTexture(nullptr),
    groundColor(0.0f, 0.0f, 0.0f, 0.0f),
    groundColorTexture(nullptr),
    nightColor(0.0f, 0.0f, 0.0f, 0.0f),
    nightColorTexture(nullptr),
    horizonHeight(0.0f),
    horizonBlur(0.0f),
    backgroundTexture(nullptr),
    sunDiskScale(0.0f),
    sunDiskIntensity(0.0f),
    sunGlowIntensity(0.0f),
    visibilityDistance(0.0f),
    useCustomPerezLuminanceCoefficients(false),
    customPerez_A(0.0f),
    customPerez_B(0.0f),
    customPerez_C(0.0f),
    customPerez_D(0.0f),
    customPerez_E(0.0f),
    customPerez_DiffuseHorizontalIlluminance(0.0f)
{
}
