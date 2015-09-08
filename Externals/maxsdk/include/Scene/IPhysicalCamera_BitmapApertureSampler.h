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

#include <vector>

namespace MaxSDK 
{;

//==================================================================================================
// class IPhysicalCamera_BitmapApertureSampler
//
//! \brief Utility class that provides efficient sampling for a bitmap aperture on the physical camera.
/*! When a bitmap aperture is used, uniformly sampling the aperture will result in massively reduced performance or increased noise.
    This class implements a smarter, weight-based sampling method which will introduce minimal performance degradation from using a bitmap
    aperture. This class is intended to be used in one of three ways:
    * For directly sampling the bitmap aperture
    * For building the sampling acceleration structures, and then using them in the shading code (e.g. if the shader cannot reference C++ code)
    * As a reference for implementing your own sampler
 \remark <b>WARNING: This class makes no reference whatsoever to the rest of the Max SDK. This enables it to be used in a renderer's shading
    code without pulling in the Max SDK.</b>
*/
class IPhysicalCamera_BitmapApertureSampler
{
public:

    //! A simple RGB color value. the Max SDK's Color class couldn't be used since we want to remain independent from the rest of the Max SDK.
    struct RGBValue
    {
        RGBValue();
        RGBValue(const float r, const float g, const float b);
        void operator*=(const float v);

        float r, g, b;
    };

    //! Interface used to provide generic access to the bitmap aperture.
    /*! This class abstracts the access mechanism to the bitmap aperture, such that we have no direct dependency to class Texmap.
        If the client is implementing this interface from a Max SDK plugin, then it may provide direct access to the contents of the Texmap, baked
        using Texmap::RenderBitmap. But if the client is calling the sampler from a shader that doesn't have access to the Max SDK, then it 
        should access the texmap using whatever facilities are exposed by the shading API. */
    class IApertureBitmapAccess
    {
    public:
        //! Returns the resolution of the bitmap aperture in X.
        virtual unsigned int get_x_resolution() const = 0;
        //! Returns the resolution of the bitmap aperture in Y.
        virtual unsigned int get_y_resolution() const = 0;
        //! Returns the contents of the pixel in the bitmap aperture, at the given X and Y coordinates.
        virtual RGBValue get_pixel_value(const unsigned int x, const unsigned int y) const = 0;
    };

    /*! Constructs the sampler using the parameter values extracted from IPhysicalCamera and a callback interface used for fetching values from
        the aperture bitmap.
        \param aperture_bitmap_access The interface which provides access to contents of the aperture bitmap.
        \param center_bias The physical camera's aperture center bias, as returned by IPhysicalCamera::GetBokehCenterBias().
        \param bitmap_affects_exposure Whether the aperture bitmap affects exposure, as returned by IPhysicalCamera::GetBokehTextureAffectExposure(). */
    IPhysicalCamera_BitmapApertureSampler(IApertureBitmapAccess& aperture_bitmap_access, const float center_bias, const bool bitmap_affects_exposure);

    //! Samples a location in the aperture bitmap
    void sample_aperture(
        //! The first random sample value, in [0,1)
        const float sample_u, 
        //! The second random sample value, in [0,1)
        const float sample_v,
        //! [out] Returns the the sampled position in U on the aperture in [-1,1]
        float& sampled_position_u, 
        //! [out] Returns the the sampled position in V on the aperture in [-1,1]
        float& sampled_position_v,
        //! [out] Returns the weight of the sample that needs to be applied to the outgoing camera ray
        RGBValue& sample_weight) const;

    // These return the sampler data, in case the client needs to implement its own sampling code
    //! \name Access to sampling acceleration structure
    //! \brief These provide access to the internal acceleration structures, useful if the client uses this class for building the structures
    //! but wishes to implement its own sampling code
    //@{
    //! Returns whether the sampling acceleration structure was built successfully. May be false, for example, if the aperture bitmap is all black.
    bool is_sampler_valid() const;
    //! Returns the weights to be applied to the outgoing camera ray.
    const std::vector<RGBValue>& get_pixel_weights() const;
    //! Returns the cumulative distribution function (CDF) used to select a row in the aperture bitmap.
    const std::vector<float>& get_y_selection_cdf() const;
    //! Returns the cumulative distribution function (CDF) used to select a column, within a previously selected row, in the aperture bitmap.
    const std::vector<float>& get_x_selection_cdf() const;
    //! Returns the resolution of the acceleration structure in X.
    const int get_x_resolution() const;
    //! Returns the resolution of the acceleration structure in Y.
    const int get_y_resolution() const;
    //@}

private:

    static float square(const float val);

    // Applies the center bias to the given pixel value
    static RGBValue apply_center_bias(
        const RGBValue& input, 
        const float center_bias,
        const unsigned int x, const unsigned int y,
        const unsigned int x_resolution, const unsigned int y_resolution);

    // Returns the sampling weight of the given RGB value
    static float get_sampling_weight(const RGBValue& rgb);

    // Performs a binary search in a table of CDF values. The returned value is the raster-space position of the sampled pixel.
    float sample_cdf_pixel(
        // Random sample in [0,1)
        const float sample_value, 
        // Begin and end iterators for the range of values to search         
        const std::vector<float>::const_iterator begin_iterator, 
        const std::vector<float>::const_iterator end_iterator) const;

    // Returns the weight of the given pixel in the aperture bitmap
    RGBValue get_aperture_bitmap_weight(const float x, const float y) const;

private:

    // Saves whether the sampler is valid. Will be false if the bitmap aperture was empty or all black, and thus can't be sampled.
    bool m_sampler_valid;

    // This is the individual weight of every pixel - the weight of the ray that results from sampling this location on the aperture.
    // This takes into account the original bitmap values and parameters, as well as the probability of each pixel to be sampled.
    std::vector<RGBValue> m_pixel_weight;

    // Normalized CDF used for selecting a row when sampling the aperture bitmap. Each entry stores the sum of that row and all the previous reviews.
    // The last entry will store 1.0.
    std::vector<float> m_y_selection_cdf;

    // CDF used for selecting a pixel within a row. The values are normalized on a per-row basis (every row has values ranging from 0.0 to 1.0).
    // A row first needs to be selected using m_row_selection_cdf. Then a pixel is selected within that row using this table.
    std::vector<float> m_x_selection_cdf;

    // Resolution of the aperture bitmap.
    unsigned int m_x_resolution;
    unsigned int m_y_resolution;
};

}      // namespace

#include "IPhysicalCamera_BitmapApertureSampler.inline.h"