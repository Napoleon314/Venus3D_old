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

#include "../gencam.h"

namespace MaxSDK 
{;

//==================================================================================================
// class IPhysicalCamera
//

inline Point2 IPhysicalCamera::GetFilmPlaneOffsetInPixels(const IPoint2& resolution, const float pixelAspectRatio, const TimeValue t, Interval& validity) const 
{
    const Point2 relative_offset = GetFilmPlaneOffset(t, validity);

    const float image_aspect_ratio = pixelAspectRatio * float(resolution.x) / float(resolution.y);
    return Point2(
        relative_offset.x * resolution.x,
        relative_offset.y * resolution.y * image_aspect_ratio);
}

//==================================================================================================
// class IPhysicalCamera::RenderTransformEvaluationGuard
//

inline IPhysicalCamera::RenderTransformEvaluationGuard::RenderTransformEvaluationGuard(INode* const node, const TimeValue t)
    :m_node(node),
    m_timeValue(t),
    m_previous_value(false)
{
    if(m_node != nullptr)
    {
        // Evaluate the node
        const ObjectState &os = m_node->EvalWorldState(m_timeValue);
    
        // See if it's a physical camera
        IPhysicalCamera* const physical_camera = dynamic_cast<IPhysicalCamera*>(os.obj);
        if(physical_camera != nullptr)
        {
            // Enable future evaluation of render transform
            m_previous_value = physical_camera->SetEvaluatingRenderTransform(true);

            if(!m_previous_value)
            {
                // Invalidate object cache to force object transform to be re-evaluated
                GetCOREInterface7()->InvalidateObCache(m_node);
                // Re-evaluate world state in case caller holds a reference to the node's ObjectState - make sure that ObjectState is re-initialized
                m_node->EvalWorldState(m_timeValue);
            }
        }
    }
}

inline IPhysicalCamera::RenderTransformEvaluationGuard::~RenderTransformEvaluationGuard()
{
    if(m_node != nullptr)
    {
        // Evaluate the node
        const ObjectState &os = m_node->EvalWorldState(m_timeValue);

        // See if it's a physical camera
        IPhysicalCamera* const physical_camera = dynamic_cast<IPhysicalCamera*>(os.obj);
        if(physical_camera != nullptr)
        {
            // Restore the previous flag value
            physical_camera->SetEvaluatingRenderTransform(m_previous_value);

            // Invalidate world state cache to force object transform to be re-evaluated
            if(!m_previous_value)
            {
                // Invalidate object cache to force object transform to be re-evaluated
                GetCOREInterface7()->InvalidateObCache(m_node);
                // Re-evaluate world state in case caller holds a reference to the node's ObjectState - make sure that ObjectState is re-initialized
                m_node->EvalWorldState(m_timeValue);
            }
        }
    }
}



}   // namespace
