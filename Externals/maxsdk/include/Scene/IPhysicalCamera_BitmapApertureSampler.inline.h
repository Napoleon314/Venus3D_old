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

// Use M_PI from math.h
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <algorithm>
#include <cassert>
#include <math.h>

// Allow using std::min and std::max
#undef min
#undef max

namespace MaxSDK 
{;

inline IPhysicalCamera_BitmapApertureSampler::RGBValue operator*(const float a, const IPhysicalCamera_BitmapApertureSampler::RGBValue& b)
{
    return IPhysicalCamera_BitmapApertureSampler::RGBValue(a * b.r, a * b.g, a * b.b);
}

inline IPhysicalCamera_BitmapApertureSampler::RGBValue operator*(const IPhysicalCamera_BitmapApertureSampler::RGBValue& a, const float b)
{
    return b * a;
}

inline IPhysicalCamera_BitmapApertureSampler::RGBValue operator*(const IPhysicalCamera_BitmapApertureSampler::RGBValue& a, const IPhysicalCamera_BitmapApertureSampler::RGBValue& b)
{
    return IPhysicalCamera_BitmapApertureSampler::RGBValue(a.r * b.r, a.g * b.g, a.b * b.b);
}

inline IPhysicalCamera_BitmapApertureSampler::RGBValue operator+(const IPhysicalCamera_BitmapApertureSampler::RGBValue& a, const IPhysicalCamera_BitmapApertureSampler::RGBValue& b)
{
    return IPhysicalCamera_BitmapApertureSampler::RGBValue(a.r + b.r, a.g + b.g, a.b + b.b);
}

inline IPhysicalCamera_BitmapApertureSampler::RGBValue operator-(const IPhysicalCamera_BitmapApertureSampler::RGBValue& a, const IPhysicalCamera_BitmapApertureSampler::RGBValue& b)
{
    return IPhysicalCamera_BitmapApertureSampler::RGBValue(a.r - b.r, a.g - b.g, a.b - b.b);
}

//==================================================================================================
// class IPhysicalCamera_BitmapApertureSampler::RGBValue
//==================================================================================================

inline IPhysicalCamera_BitmapApertureSampler::RGBValue::RGBValue()
{
}

inline IPhysicalCamera_BitmapApertureSampler::RGBValue::RGBValue(const float p_r, const float p_g, const float p_b)
    : r(p_r), g(p_g), b(p_b)
{
}

inline void IPhysicalCamera_BitmapApertureSampler::RGBValue::operator*=(const float v)
{
    r *= v;
    g *= v;
    b *= v;
}

//==================================================================================================
// class IPhysicalCamera_BitmapApertureSampler
//==================================================================================================

inline IPhysicalCamera_BitmapApertureSampler::IPhysicalCamera_BitmapApertureSampler(
    IApertureBitmapAccess& aperture_bitmap_access,
    const float center_bias,
    const bool bitmap_affects_exposure)
    : m_sampler_valid(false),
    m_x_resolution(0),
    m_y_resolution(0)
{
    m_x_resolution = aperture_bitmap_access.get_x_resolution();
    m_y_resolution = aperture_bitmap_access.get_y_resolution();

    if((m_x_resolution > 0) && (m_y_resolution > 0))
    {
        const size_t num_pixels = size_t(m_x_resolution) * size_t(m_y_resolution);
        m_pixel_weight.resize(num_pixels);
        m_y_selection_cdf.resize(m_y_resolution);
        m_x_selection_cdf.resize(num_pixels);

        double original_bitmap_weight = 0.0f;   // use double as additions are terrible on floating-point precision

        // Accumulate the CDF for the entire bitmap
        double cdf_entire_bitmap = 0.0;     // use double as additions are terrible on floating-point precision
        for(size_t y = 0, pixel_index = 0; y < m_y_resolution; ++y)
        {
            // Accumulate the CDF for the current row
            double cdf_current_row = 0.0;       // use double as additions are terrible on floating-point precision
            for(unsigned int x = 0; x < m_x_resolution; ++x, ++pixel_index)
            {
                // Evaluate the pixel value
                const RGBValue original_pixel_value = aperture_bitmap_access.get_pixel_value(x, unsigned int(y));
                // Apply the center bias
                const RGBValue pixel_value_with_bias = apply_center_bias(original_pixel_value, center_bias, x, unsigned int(y), m_x_resolution, m_y_resolution);

                // Accumulate CDF using pixel value that has the center bias applied (sampling takes this bias into account)
                const float pixel_sampling_weight = get_sampling_weight(pixel_value_with_bias);
                cdf_current_row += pixel_sampling_weight;
                m_x_selection_cdf[pixel_index] = cdf_current_row;
                m_pixel_weight[pixel_index] = pixel_value_with_bias;

                // The original pixel value, without center bias, is accumulated such that we may undo its effect if "affect exposure" is disabled
                original_bitmap_weight += get_sampling_weight(original_pixel_value);
            }

            // Accumulate the CDF for the entire row
            cdf_entire_bitmap += cdf_current_row;
            m_y_selection_cdf[y] = cdf_entire_bitmap;
        }

        // Check whether the bitmap wasn't entirely black
        if(cdf_entire_bitmap > 0.0f)
        {
            // Normalize the CDFs
            for(size_t y = 0; y < m_y_resolution; ++y)
            {
                if(m_y_selection_cdf[y] > 0.0f)
                {
                    const float row_total_cdf = m_x_selection_cdf[(y * m_x_resolution) + m_x_resolution - 1];
                    if(row_total_cdf > 0.0f)
                    {
                        for(size_t x = 0, pixel_index = y * m_x_resolution; x < m_x_resolution; ++x, ++pixel_index)
                        {
                            m_x_selection_cdf[pixel_index] /= row_total_cdf;
                            assert(m_x_selection_cdf[pixel_index] <= 1.0f);
                        }
                    }

                    m_y_selection_cdf[y] /= m_y_selection_cdf[m_y_resolution - 1];
                    assert(m_y_selection_cdf[y] <= 1.0f);
                }
            }

            // Calculate the global weight to be applied to every pixel of the bitmap
            double global_bitmap_weight = 1.0;
            if(bitmap_affects_exposure)
            {
                // The bitmap's contents implicitly affect the the exposure since we multiply the ray result by the bitmap's pixel value.
                // Here we scale the bitmap's contents such that a bitmap containing a white circle would render identically 
                // to a circular aperture.
                global_bitmap_weight *= 1.0 / (M_PI * (0.5 * 0.5));        // one over the area of a disc with radius 0.5

                // If center bias is present, it has modified the texture but we don't want to let this affect exposure. We therefore
                // counteract the effect.
                global_bitmap_weight *= original_bitmap_weight / cdf_entire_bitmap;
            }
            else
            {
                // Because the bitmap's contents implicitly affect the exposure, here we counteract this such that the average
                // pixel value gets normalized to 1.0.
                const double bitmap_average_pixel_value = cdf_entire_bitmap / num_pixels;
                global_bitmap_weight /= bitmap_average_pixel_value;
            }

            // Calculate the final weight of every pixel in the bitmap
            for(size_t y = 0, pixel_index = 0; y < m_y_resolution; ++y)
            {
                // Probability of sampling this y coordinate
                const float previous_y_cdf = (y > 0) ? m_y_selection_cdf[y - 1] : 0.0f;
                const float y_sampling_prob = m_y_selection_cdf[y] - previous_y_cdf;

                for(unsigned int x = 0; x < m_x_resolution; ++x, ++pixel_index)
                {
                    // Apply the global weight (results from how we want exposure to be affected)
                    m_pixel_weight[pixel_index] *= global_bitmap_weight;
                    
                    // Probability of sampling this x coordinate within the current row
                    const float previous_x_cdf = (x > 0) ? m_x_selection_cdf[pixel_index - 1] : 0.0f;
                    const float x_sampling_prob = m_x_selection_cdf[pixel_index] - previous_x_cdf;

                    // Probability of sampling this pixel overall
                    const double pixel_sampling_prob = double(x_sampling_prob) * double(y_sampling_prob);

                    // Compensate for the sampling probability of the pixel. Pixels should all have equal weight on the final result (photons
                    // have an equal likelihood of passing through any location on the aperture), but since we sample the aperture non-uniformly
                    // we need to compensate.
                    const double uniform_sampling_probability = (1.0 / num_pixels);
                    const double sample_compensation = (pixel_sampling_prob > 0.0f) ? (uniform_sampling_probability / pixel_sampling_prob) : 0.0;
                    m_pixel_weight[pixel_index] *= sample_compensation;
                }
            }

            m_sampler_valid = true;
        }
    }
}

inline float IPhysicalCamera_BitmapApertureSampler::sample_cdf_pixel(
    const float sample_value, 
    const std::vector<float>::const_iterator begin_iterator, 
    const std::vector<float>::const_iterator end_iterator) const
{
    assert(((end_iterator - begin_iterator) > 0) &&  (*(end_iterator - 1) == 1.0f));        // last value of CDF table should always be 1.0

    // Look for first value that is greater than the sample value. Clamp result to last element in list, in case precision issues and all.
    const std::vector<float>::const_iterator search_result = std::min(std::upper_bound(begin_iterator, end_iterator, sample_value), end_iterator - 1);
    const int found_index = search_result - begin_iterator;

    const float previous_cdf = (search_result > begin_iterator) ? *(search_result - 1) : 0.0f;
    const float current_cdf = *search_result;

    // Rescale the sample to point to a position within the pixel, to be used as the fractional part of the texture coordinate
    const float rescaled_sample = std::min((sample_value - previous_cdf) / (current_cdf - previous_cdf), 1.0f);     // clamp to 1.0 to avoid precision issues that could result in sampling last row that has entirely zeros
    assert(rescaled_sample < 1.0f);

    return float(found_index) + rescaled_sample;
}

inline IPhysicalCamera_BitmapApertureSampler::RGBValue IPhysicalCamera_BitmapApertureSampler::get_aperture_bitmap_weight(const float x, const float y) const
{
    // Retrieve the pixel value, without any filtering or interpolation. I experimented with interpolation, but it results in artifacts as it introduces
    // a mismatch between the sampling weight and evaluation result. To properly do interpolation, we'd have to take the average value of the interpolated
    // pixel when calculating its sampling weight. I don't think it's worth it, though: aliasing shouldn't be visible on the aperture bitmap.
    const unsigned int clamped_x = std::min(unsigned int(x), m_x_resolution - 1);
    const unsigned int clamped_y = std::min(unsigned int(y), m_y_resolution - 1);
    const size_t pixel_index = size_t(clamped_x) + (size_t(clamped_y) * m_x_resolution);
    return m_pixel_weight[pixel_index];
}

inline void IPhysicalCamera_BitmapApertureSampler::sample_aperture(
    // The random sample values, in [0,1)
    const float sample_u, const float sample_v,
    // Returns the the sampled position in the aperture bitmap, in [0,1)
    float& sampled_position_u, float& sampled_position_v,
    // Returns the weight of the sample, to be applied to the outgoing camera ray
    RGBValue& sample_weight) const
{
    if(m_sampler_valid)
    {
        // Sample a row
        const float y_coord = sample_cdf_pixel(sample_v, m_y_selection_cdf.begin(), m_y_selection_cdf.end());
        assert(y_coord < m_y_resolution);

        // Sample a x coordinate within the selected row
        const std::vector<float>::const_iterator cdf_row_begin = m_x_selection_cdf.begin() + (size_t(y_coord) * m_x_resolution);
        const float x_coord = sample_cdf_pixel(sample_u, cdf_row_begin, cdf_row_begin + m_x_resolution);
        assert(x_coord < m_x_resolution);

        // Rescale sampled coordinate in [-1,1]
        sampled_position_u = ((x_coord / m_x_resolution) * 2.0f) - 1.0f;
        sampled_position_v = ((y_coord / m_y_resolution) * 2.0f) - 1.0f;
        assert((sampled_position_u >= -1.0f) && (sampled_position_u <= 1.0f) && (sampled_position_v >= -1.0f) && (sampled_position_v <= 1.0f));

        // Evaluate the aperture bitmap weight
        sample_weight = get_aperture_bitmap_weight(x_coord, y_coord);
        assert((sample_weight.r > 0.0f) || (sample_weight.g > 0.0f) || (sample_weight.b > 0.0f));
    }
    else
    {
        // Sampler not valid: sample center point only
        sampled_position_u = 0.0f;
        sampled_position_v = 0.0f;
        sample_weight = RGBValue(1.0f, 1.0f, 1.0f);
    }
}

inline float IPhysicalCamera_BitmapApertureSampler::square(const float val)
{
    return val * val;
}

inline IPhysicalCamera_BitmapApertureSampler::RGBValue IPhysicalCamera_BitmapApertureSampler::apply_center_bias(
    const RGBValue& input, 
    const float center_bias,
    const unsigned int x, const unsigned int y,
    const unsigned int x_resolution, const unsigned int y_resolution)
{
    // Calculate the effect of center bias
    if(center_bias != 0.0f)
    {
        // Calculate distance from center, clamping the distance at 1.0 to avoid growing outside the circle
        const float distance_from_center = std::min(sqrtf(square((float(x) / float(x_resolution) * 2.0f) - 1.0f) + square((float(y) / float(y_resolution) * 2.0f) - 1.0f)), 1.0f);

        // Calculate the position in the bias curve, which gets reversed if doing ring bias
        const float bias_curve_position = 
            (center_bias > 0.0f) 
            // Positive exponent biases towards ring
            ? (1.0f - distance_from_center)
            // Negative exponent biases towards center
            : distance_from_center;

        // Calculate the bias curve. This curve doesn't yield exactly the same result as the way the center bias is applied when sampling the 
        // circular or bladed aperture, but it's close enough and efficient.
        const float center_bias_mult = powf((cosf(bias_curve_position * float(M_PI)) + 1.0f) * 0.5f, fabsf(center_bias));
        return input * center_bias_mult;
    }
    else
    {
        return input;
    }
}

inline float IPhysicalCamera_BitmapApertureSampler::get_sampling_weight(const RGBValue& rgb)
{
    // Returns the brightness as perceived by the human eye
    return rgb.r * 0.263f + rgb.g * 0.655f + rgb.b * 0.082f;
}

inline bool IPhysicalCamera_BitmapApertureSampler::is_sampler_valid() const
{
    return m_sampler_valid;
}

inline const std::vector<IPhysicalCamera_BitmapApertureSampler::RGBValue>& IPhysicalCamera_BitmapApertureSampler::get_pixel_weights() const
{
    return m_pixel_weight;
}

inline const std::vector<float>& IPhysicalCamera_BitmapApertureSampler::get_y_selection_cdf() const
{
    return m_y_selection_cdf;
}

inline const std::vector<float>& IPhysicalCamera_BitmapApertureSampler::get_x_selection_cdf() const
{
    return m_x_selection_cdf;
}

inline const int IPhysicalCamera_BitmapApertureSampler::get_x_resolution() const
{
    return m_x_resolution;
}

inline const int IPhysicalCamera_BitmapApertureSampler::get_y_resolution() const
{
    return m_y_resolution;
}

}   // namespace