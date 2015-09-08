//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2014 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

//==================================================================================================
// class ToneOperator
//==================================================================================================

inline void ToneOperator::SetActive(
    bool		active,
    TimeValue	/*t*/
    ) {
        if (active ^ (TestAFlag(A_TONEOP_DISABLED) == 0)) {
            if (active) {
                ClearAFlag(A_TONEOP_DISABLED);
            }
            else {
                SetAFlag(A_TONEOP_DISABLED);
            }
            NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
        }
}

inline void ToneOperator::SetProcessBackground(bool active) {
    if (active ^ (TestAFlag(A_TONEOP_PROCESS_BG) != 0)) {
        if (active) {
            SetAFlag(A_TONEOP_PROCESS_BG);
        }
        else {
            ClearAFlag(A_TONEOP_PROCESS_BG);
        }
        NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
    }
}

inline void ToneOperator::SetIndirectOnly(bool active) {
    if (active ^ (TestAFlag(A_TONEOP_INDIRECT_ONLY) != 0)) {
        if (active) {
            SetAFlag(A_TONEOP_INDIRECT_ONLY);
        }
        else {
            ClearAFlag(A_TONEOP_INDIRECT_ONLY);
        }
        NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
    }
}

inline bool ToneOperator::CanInvert()
{
    return GetInterface(INVERTABLE_TONE_OPERATOR_INTERFACE) != NULL;
}

// Calculate the physical value from the display value
inline void ToneOperator::RGBToScaled(float energy[3])
{
    ToneOperatorInvertable* p = static_cast<ToneOperatorInvertable*>(GetInterface(INVERTABLE_TONE_OPERATOR_INTERFACE));

    if (p != NULL)
        p->InverseMap(energy);
}

inline float ToneOperator::RGBToScaled(float energy)
{
    ToneOperatorInvertable* p = static_cast<ToneOperatorInvertable*>(GetInterface(INVERTABLE_TONE_OPERATOR_INTERFACE));

    return p == NULL ? energy : p->InverseMap(energy);
}

inline void ToneOperator::Update(const TimeValue t, INode* const cameraNode, const IPoint2 renderResolution, const float pixelAspectRatio, Interval& validity)
{
    // Forward the call to ToneOperator2, if possible
    ToneOperator2* const tone_op2 = dynamic_cast<ToneOperator2*>(this);
    if(tone_op2 != nullptr)
    {
        tone_op2->Update(t, cameraNode, renderResolution, pixelAspectRatio, validity);
    }
    else
    {
        // Not a ToneOperator2: forward to legacy method
        Update(t, validity);
    }
}

inline void ToneOperator::ScaledToRGB(float energy[3], const Point2& xyCoord) 
{
    // Forward the call to ToneOperator2, if possible
    const ToneOperator2* const tone_op2 = dynamic_cast<const ToneOperator2*>(this);
    if(tone_op2 != nullptr)
    {
        return tone_op2->ScaledToRGB(energy, xyCoord);
    }
    else
    {
        return ScaledToRGB(energy);
    }
}

inline float ToneOperator::ScaledToRGB(float energy, const Point2& xyCoord) 
{
    // Forward the call to ToneOperator2, if possible
    const ToneOperator2* const tone_op2 = dynamic_cast<const ToneOperator2*>(this);
    if(tone_op2 != nullptr)
    {
        return tone_op2->ScaledToRGB(energy, xyCoord);
    }
    else
    {
        return ScaledToRGB(energy);
    }
}

//==================================================================================================
// class ShadeContext
//==================================================================================================

// Does this tone operator really map physical values to RGB. This method
// is provided so shaders can determine whether the shading calculations
// are in physical or RGB space.
inline bool ShadeContext::IsPhysicalSpace() const
	{ return globContext != NULL && globContext->pToneOp != NULL
		&& globContext->pToneOp->IsPhysicalSpace( ); }

// Map an scaled energy value into RGB. The first version of the
// method converts a color value and the second converts a monochrome
// value. The converted color value is stored in <i>energy</i>.
// The converted monochrome value is returned.
inline float ShadeContext::ScaledToRGB( float energy ) const
	{ return (globContext == NULL || globContext->pToneOp == NULL)
		? energy : globContext->pToneOp->ScaledToRGB( energy ); }

// Map an energy value int out.c into RGB. The converted value is stored in
// out.c.
inline void ShadeContext::ScaledToRGB( )
	{ ScaledToRGB( out.c ); }

// Scale physical values so they can be used in the renderer. The
// first version of the method converts a color value and the second
// converts a monochrome value. The converted color value is stored
// in <i>energy</i>. The converted monochrome value is returned.
inline float ShadeContext::ScalePhysical(float energy) const
	{ return (globContext == NULL || globContext->pToneOp == NULL)
		? energy : globContext->pToneOp->ScalePhysical( energy ); }

// Scale RGB values, just supplied to invert ScalePhysical. The first
// version of the method converts a color value and the second
// converts a monochrome value. The converted color value is stored
// in <i>energy</i>. The converted monochrome value is returned.
inline float ShadeContext::ScaleRGB(float energy) const
	{ return (globContext == NULL || globContext->pToneOp == NULL)
		? energy : globContext->pToneOp->ScaleRGB( energy ); }



