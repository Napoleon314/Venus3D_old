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
#include "../toneOp.h"

namespace MaxSDK 
{;

//==================================================================================================
// class IPhysicalCamera
//
//! \brief Access to the properties of the Physical Camera object
/*! This interface provides SDK access to the properties of the Physical Camera object. To retrieve a pointer to this interface, perform a dynamic_cast
    on a GenCamera, CameraObject, or Object as returned by INode::EvalWorldState():
    \par
    \code 
        IPhysicalCamera* physicalCamera = dynamic_cast<IPhysicalCamera*>(node->EvalWorldState(t).obj);
    \endcode 
*/
class IPhysicalCamera :
    public GenCamera
{
public:

    class RenderTransformEvaluationGuard;

    //! Shape of the aperture used to render bokeh with the depth of field effect.
    enum class BokehShape
    {
        //! Circular aperture.
        Circular,
        //! Bladed aperture (as a regular polygon)
        Bladed,
        //! Custom shape supplied through a texture
        Texture
    };

    //! Type of lens distortion
    enum class LensDistortionType
    {
        //! No lens distortion
        None,
        //! Cubic lens distortion
        Cubic,
        //! Lens distortion extracted from a texture
        Texture
    };

    //! \name Sensor/Film & Lens Properties
    //! \brief The Physical Camera defines sensor/film and lens properties using real-world characteristics.
    //@{
    /*! Returns the width of the film plane, in system units. The height is derived from the aspect ratio of the image to be rendered.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetFilmWidth(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the effective focal length of the lens, in system units. 
        \remark This value may be different from the focal length specified by the user; when focusing, the lens may move thus affecting its 
            effective focal length and the resulting field of view. This returns the result of such movement - the distance between the film plane
            and the lens.
        \remark The crop zoom factor is <b>not</b> taken into account by this method (see GetCropZoomFactor()).   
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetEffectiveLensFocalLength(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the crop zoom factor of the lens.
        This feature behaves as an image crop. As such, it effectively increases the lens focal length for purposes of calculating the field-of-view.
        However, exposure and depth of field are not affected. To achieve this, exposure is performed using the f-number (see GetLensApertureFNumber()), 
        while depth of field is performed using the absolute aperture radius relative to the non-zoomed lens (see GetLensApertureRadius()).
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetCropZoomFactor(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the absolute aperture radius, in system units.
        The aperture size affects depth of field (larger apertures result in amplified depth of field effect), as well as exposure (if specifying exposure
        manually through the ISO).
       \remark 
            When rendering a <b>circular aperture</b>, this is the radius of the disc. 
            When rendering a <b>bladed aperture</b>, this is NOT the radius of the polygon: instead, the polygon should have an area which is equal to that of a disc
            with the given radius. This ensures that exposure is unaffected by the presence of blades (blades therefore have only an aesthetic property).
            When rendering a <b>texture aperture</b>, the texture will be remapped to a square with sides equal to twice the radius.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. 
        \param[in] adjustForBlades When true, and the aperture shape is set to <b>bladed</b>, the correct radius of the polygon will be returned instead
            of the radius of the disc. When false, the radius of the disc aperture is returned; as noted above, this radius cannot be used directly to generate
            a bladed polygon aperture. */
    virtual float GetLensApertureRadius(const TimeValue t, Interval& validity, const bool adjustForBlades) const = 0;
    /*! Returns the aperture size as an f-number. 
        The aperture size affects depth of field (larger apertures result in amplified depth of field effect), as well as exposure (if specifying exposure
        manually through the ISO).
        \remark When rendering a <b>bladed aperture</b>, the radius of the polygon should be adjusted such that the area of the polygon matches that of
            a disc at this aperture size; this ensures consistent exposure, see \see GetLensApertureRadius().
        \note The f-number returned here is relative to the focal length returned by GetEffectiveLensFocalLength(). It may therefore differ
            from the value supplied by the user, as it takes lens breathing into account. Lens breathing will cause the f-number to grow smaller
            as the lens focuses closer (though the absolute aperture radius doesn't change), thus affecting exposure.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter.  */
    virtual float GetLensApertureFNumber(const TimeValue t, Interval& validity) const = 0;
    //@}

    //! \name Shutter & Motion Blur
    //! \brief The shutter parameters affect motion blur, as well as exposure (if specifying exposure manually through the ISO).
    //@{
    /*! Returns whether motion blur is to be enabled. 
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual bool GetMotionBlurEnabled(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the shutter open duration, the units being frames at the current frame rate, as returned by GetFrameRate().
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetShutterDurationInFrames(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the shutter open offset, the units being frames at the current frame rate, as returned by GetFrameRate().
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetShutterOffsetInFrames(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the distance at which the lens is focused, in system units.
        \remark The focus distance is relative to the lens, which is positioned on the camera origin.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetFocusDistance(const TimeValue t, Interval& validity) const = 0;
    //@}

    //! \name Exposure control
    //! \brief Exposure control can be specified using the camera properties, using the global exposure control plugin (in the environment panel),
    //!  or disabled entirely.
    //@{
    /*! Returns the ISO to be used when rendering this camera. This is not necessarily the ISO currently stored in the parameter block; it may
        be an ISO value derived from the target exposure value.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetEffectiveISO(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the EV to be used when rendering this camera. This is not necessarily the EV currently stored in the parameter block; it may
        be an EV value derived from the target exposure value.
        \remark The EV being returned is technically the "equivalent EV at ISO 100", such that increasing the ISO is correctly represented as decreasing the EV
                (thus making the image brighter). This differs from the standard definition of EV (which would have little use here), and is somtimes known
                as EV100.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetEffectiveEV(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the white point, to be used with exposure control.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual Color GetWhitePoint(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the amount of vignetting in the exposure (darkening on the edges of the rendered image), as a non-negative value. Returns 0
        if vignetting is disabled.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetExposureVignettingAmount(const TimeValue t, Interval& validity) const = 0;
    //@}

    //! \name Depth of field, bokeh
    //! \brief These properties control the aspect of the depth of field effect. The <b>amount</b> of depth of field is controlled by the aperture size.
    //@{
    /*! Returns whether depth of field effects are enabled. If this returns false, then a pinhole camera should be rendered (everything will be in focus).
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual bool GetDOFEnabled(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the shape of the bokeh to be rendered.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual BokehShape GetBokehShape(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the number of blades to be rendered, if GetBokehShape() returns BokehShape::Bladed.
        This is the number of sides to the regular polygon.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual int GetBokehNumberOfBlades(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the rotation of the aperture, if GetBokehShape() returns BokehShape::Bladed.
        The regular polygon rendered for a bladed aperture should be rotated by this angle.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetBokehBladesRotationDegrees(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the texture to be used as the aperture shape, if GetBokehShape() returns BokehShape::Texture.
        The aperture texture is specified as a transparency map: white means fully transparent, black means opaque. Colors also work and will tint the result.
        A texture that contains a white circle, over a black background, that runs to the edges of the texture, should render identically to a circular aperture.
        The texture will be fit into a square which's side has length equal to the diameter of the circular aperture (twice the radius returned by GetLensApertureRadius()).
        \remark If no texture is specified, a circular aperture should be used as a fallback. If the texture cannot be loaded or is invalid (all black), then DOF should be disabled.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual Texmap* GetBokehTexture(const TimeValue t, Interval& validity) const = 0;
    /*! Returns whether the texture aperture shape should affect the exposure.
        If true, then the aperture shape provided in the texture will affect exposure: a texture with a small white circle will result in a darker image
        than a texture with a larger white circle.
        If false, the aperture texture should not impact the overall exposure (though it will certainly impact local exposure): the overall exposure
        should be compensated such that it is equivalent to a circular aperture.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual bool GetBokehTextureAffectExposure(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the amount of optical vignetting to be rendered.
        Optical vignetting is also know as the cat's eye effect; it is caused by the presence of multiple element in the lens, with multiple circular
        elements combining to form a shape resembling the pupil of a cat's eye. Values typically range from -1.0 to 1.0, though values outside this
        range are valid but will produce heavy vignetting.
        The algorithm to generate the effect is as follows:
        \code
        Point2 film_point = raster space coordinate in [-1,1], with y being scaled according to the image aspect ratio
        Point2 aperture_point = sampled point on aperture in [-1,1]
        float distance = distance_between_points(aperture_point, film_point * optical_vignetting_amount)
        if(distance > 1.0)
            reject sample
        \endcode
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetBokehOpticalVignetting(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the amount of center bias to be rendered.
        Center bias is used to generate a non-uniform aperture, with positive values biasing towards a ring shape and negative values biasing 
        towards the center.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetBokehCenterBias(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the amount of anisotropy present in the bokeh.
        Anisotropy is used to simulate (apparent) non-circular apertures such as is produced by anamorphic lenses. Positive values stretch the bokeh
        along X, while negative values stretch along Y. The algorithm for generating this effect is as follows:
        \code
        Point2 aperture_sample_point = point being sampled on the aperture
        if(bokeh_anisotropy > 0.0f) 
        {
            const float aniso = 1.0f / (1.0f - bokeh_anisotropy);
            aperture_sample_point.x *= aniso;
            aperture_sample_point.y /= aniso;
        } 
        else 
        {
            const float aniso = 1.0f / (1.0f + bokeh_anisotropy);
            aperture_sample_point.x /= aniso;
            aperture_sample_point.y *= aniso;
        }
        \endcode
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetBokehAnisotropy(const TimeValue t, Interval& validity) const = 0;
    //@}
    
    //! \name Lens Distortion
    //! \brief This effect simulates the distortion that results from using thick lenses.
    //@{
    /*! Returns the type of lens distortion to be used.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual LensDistortionType GetLensDistortionType(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the amount of distortion to be rendered, if GetLensDistortionType() returns LensDistortionType::Cubic.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual float GetLensDistortionCubicAmount(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the texture to be used, if GetLensDistortionType() returns LensDistortionType::Texture.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual Texmap* GetLensDistortionTexture(const TimeValue t, Interval& validity) const = 0;
    //@}

    //! \name Perspective correction
    //! \brief This effect simulates tilt/shift lenses.
    //! \remark See class RenderTransformEvaluationGuard to simulate these effects correctly in a renderer.
    //@{
    /*! Returns the offset to be applied to the image on the film plane. 
        The value is expressed as a ratio of the film plane width: a value of 1.0 with a film width of 36mm results in an offset of 36mm - or, if
        the X resolution were 640, an offset of 640 pixels. On the Y axis, the scale is also relative to the film plane <b>width</b>, such that the
        offset in real-world space is equally scaled in X or Y. The offset in pixels is therefore calculated as follows: 
            - for X, it is: film_plane_offset.x * resolution.x
            - for Y, it is: film_plane_offset.y * resolution.y * image_aspect_ratio, <b>or:</b> film_planet_offset.y * resolution.x * pixel_aspect_ratio
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual Point2 GetFilmPlaneOffset(const TimeValue t, Interval& validity) const = 0;
    /*! Returns the offset to be applied to the image on the film plane, in pixels.
        Whereas GetFilmPlaneOffset() returns the offset as a resolution-indepent value, this method resolves the actual raster-space offset,
        but requires the resolution to be given to it.
        \param resolution The resolution of the image to be rendered.
        \param pixelAspectRatio The pixel aspect ratio of the image to be rendered.
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    Point2 GetFilmPlaneOffsetInPixels(const IPoint2& resolution, const float pixelAspectRatio, const TimeValue t, Interval& validity) const;
    /*! Returns the tilt to be applied to the lens.
        The value is to be applied as follows, where ray_target is the destination of point of the camera ray onto the focus plane (in camera space):
        \code
            Point2 tilt = GetTiltCorrection(t, validity);
            ray_target.x /= sqrt(1.0 + square(tilt.x);
            ray_target.y /= sqrt(1.0 + square(tilt.y);
            ray_target.z -= (ray_target.x * tilt.x) + (ray_target.y * tilt.y);
        \endcode
        \param t The time at which to retrieve the property value.
        \param [in, out] validity The validity of the returned value is intersected into the input value of this parameter. */
    virtual Point2 GetTiltCorrection(const TimeValue t, Interval& validity) const = 0;
    //@}

private:

    // This private method is used to put the camera into a mount where Object::Eval() will evaluate the render-time transform matrix.
    // That is, the transform matrix which doesn't take into account the lens shift and tilt.
    // The method is private because the user should pass through class RenderTransformEvaluationGuard to perform this operation safely.
    virtual bool SetEvaluatingRenderTransform(const bool evaluatingRenderTransform) = 0;
};

//==================================================================================================
// class IPhysicalCameraToneOperator
//
//! \brief Access to the properties of the Physical Camera Exposure Control
/*! This interface provides SDK access to the properties of the Physical Camera Exposure control. To retrieve a pointer to this interface, perform a dynamic_cast
    on a ToneOperator:
    \par
    \code 
        ToneOperator* tone_op = ...;
        IPhysicalCameraToneOperator* phys_cam_tone_op = dynamic_cast<IPhysicalCameraToneOperator*>(tone_op);
    \endcode 
*/
class IPhysicalCameraToneOperator : public ToneOperator2
{
public:

    //! The set of parameters used by the Physical Camera Exposure Control
    struct Parameters
    {
        //! The white point of the rendered image.
        Color whitePoint;
        //! The aperture size, as an f-number.
        float apertureFNumber;
        //! The shutter duration, in seconds.
        float shutterSeconds;
        //! The ISO sensitivity.
        float iso;
        //! The exposure value derived from: f-number, shutter, and iso.
        float ev;
        //! The amount of vignetting.
        float vignetting;
        //! Strength of highlights.
        float highlights;
        //! Gamma curve for the rendered image (incorporates the mid tones parameters).
        float gamma;
        //! Strength of dark values.
        float shadows;
        //! Color saturation.
        float saturation;
    };

    /*! Returns the set of parameters currently used by the tone operator.
        \note <b>Make sure to call ToneOperator2::Update() before calling this method. The parameters returned here are those which were set up on the last call to
            ToneOperator2::Update().</b> */
    virtual Parameters GetParameters() const = 0;
};

//==================================================================================================
// class IPhysicalCamera::RenderTransformEvaluationGuard
//
//! \brief A guard class which enables a physical camera INode to evaluate itself for ray-traced rendering (as opposed to viewport rendering).
/*! In order to render viewports (more or less) correctly, the physical camera overrides Object::Eval() to return a object-space transform matrix
    which approximates the lens tilt/shift effect. Because this approximation fails in some cases, a ray-traced renderer should implement its own
    version of the tilt/shift effect. To do this, the renderer must disable the tilt/shift approximation from the evaluation of the transform matrix;
    it does so by instantiating this guard class wherever a camera INode is evaluated (e.g. through INode::EvalWorldState() or INode::GetObjTMAfterWSM()).
    \remark This class follows the guard implementation pattern, which performs all required work in the constructor and destructor.  
*/
class IPhysicalCamera::RenderTransformEvaluationGuard : public MaxHeapOperators
{
public:

    /*! The constructor enables an option, on the physical camera, which further calls to Object::Eval() ignore the tilt/shift feature. 
    \remark This constructor also invalidates the node's object cache to ensure that further evaluations will behave correctly.
    \param node The node which potentially evaluates to an IPhysicalCamera object. It is OK to pass a null pointer, or a node which refers
        to an object that is not an IPhysicalCamera.
    \param t The time at which the object is evaluated. While this call is not dependent on any animated parameters, it is preferable to pass
        the time value at which the renderer will be evaluating the camera, for efficiency purposes. */
    RenderTransformEvaluationGuard(INode* const node, const TimeValue t);
    /*! The destructor restores the previous state of the flag which disables the tilt/shift feature. 
        \remark This destructor also invalidates the node's object cache to ensure that further evaluations will behave correctly. */
    ~RenderTransformEvaluationGuard();

private:

    // Private copy constructor and operator declaration to solve C4512
    RenderTransformEvaluationGuard(const RenderTransformEvaluationGuard&);
    void operator=(const RenderTransformEvaluationGuard&) const;


    INode* const m_node;
    const TimeValue m_timeValue;
    bool m_previous_value;
};

}   // namespace

#include "IPhysicalCamera.inline.h"