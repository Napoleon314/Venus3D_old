//*****************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//*****************************************************************************/
/*===========================================================================*\
 | 
 |  FILE:  imrPhysicalSunLight.h
 |         
 | 
 |  AUTH:   Benjamin Cecchetto
 |          Nov 7th, 2007
 |			
\*===========================================================================*/

#pragma once

#include "../baseinterface.h"
#include "../maxtypes.h"
#include "../point3.h"
#include "../acolor.h"

// forward declarations
class Interval;

//! \brief Class id for the Mental Ray Physical Sky Environment Map
#define MRSKYENVSHADER_CLASS_ID	Class_ID(0x628e269e, 0x72386abf) 
//! \brief Interface id for the Mental Ray Physical Sky Environment Map object
#define IID_MR_PHYSICAL_SKY_ENV_MAP Interface_ID(0x72941467, 0x1a8f3ae2)
//! \brief Interface id for the additional functionality for the Mental Ray Physical Sky object
#define IID_MR_PHYSICAL_SKY_ENV_MAP2 Interface_ID(0x2b8a14f0, 0x6dfa7b9b)

class Texmap;

//! \brief Access interface for the Mental Ray Physical Sky Environment Map.
/*! This class represents the Mental Ray Physical Sky Environment Map, implemented by 3ds Max.
SDK client code can get this interface by querying 3ds Max for the current
environment map, then asking that object for this interface:
\code 
Texmap* envMap = GetCOREInterface()->GetEnvironmentMap()
BaseInterface* envMapInterface = envMap->GetInterface(IID_MR_PHYSICAL_SKY_ENV_MAP);
IMRPhysicalSkyEnvironmentMap* mrEnvMap = dynamic_cast<IMRPhysicalSkyEnvironmentMap*>(envMapInterface);
\endcode
*/
class IMRPhysicalSkyEnvironmentMap : public BaseInterface {
public:
	//! \brief Returns the Interface ID of this interface
	virtual Interface_ID GetID() { return IID_MR_PHYSICAL_SKY_ENV_MAP; };
	
	//! \brief A virtual destructor for the interface
	virtual ~IMRPhysicalSkyEnvironmentMap() {};

	//! \name Inherit from mrSky
	//! \brief Uses the equivalent settings from the mrSky Parameters rollout
	//! for the remaining mr Physical Sky Parameters rollout settings, except
	//! for Aerial Perspective. Default=on.
	//@{
	/*! \brief Gets whether or not the environment map inherits from mrSky.
	\param[in] t - The time at which we're getting the value
	\param[in] valid - The interval at which we're getting the value
	\return	A boolean if we're inheriting from mrSky or not
	*/
	virtual bool   getInheritFromSky(TimeValue &t, Interval &valid) const = 0;
	/*! \brief Sets whether or not the environment map inherits from mrSky.
	\param[in] t - The time at which to set the value
	\param[in] inherits - A boolean value of what whether or not we're inheriting from mrSky
	\return true if the setting was successful	
	*/
	virtual bool   setInheritFromSky(TimeValue t, bool inherits) = 0;
	//@}

	//! \name Background Enabled
	//! \brief When on but no background map is specified, the background of
	//! the rendering is transparent black, suitable for external compositing.
	//! If you supply a background shader by clicking the button and then
	//! specifying a map or shader, the background of the rendering will come
	//! from that shader (for example, a texture map that uses a background
	//! photograph). In either case the mr Physical Sky will still be visible
	//! in reflections and refraction.
	//@{
	/*! \brief Gets the boolean value of the background being enabled
	\param[in] t - The time at which we're getting the value
	\param[in] valid - The interval at which we're getting the value
	\return	The value of if the background is enabled or not
	*/
	virtual bool   getBackgroundEnabled(TimeValue &t, Interval &valid) const = 0;
	/*! \brief Sets the boolean value of the background being enabled
	\param[in] t - The time at which to set the value
	\param[in] enableBackground - The value of what to set the background being enabled to
	\return true if the setting was successful	
	*/
	virtual bool   setBackgroundEnabled(TimeValue t, bool enableBackground) = 0;
	//@}

	//! \name Sun Disk Intensity
	//! \brief The brightness of the sun.
	//@{
	/*! \brief Gets the intensity of the sun's disk.
	\param[in] t - The time at which we're getting the intensity
	\param[in] valid - The interval at which we're getting intensity
	\return	The value of the intensity
	*/
	virtual float  getSunDiskIntensity(TimeValue &t, Interval &valid) const = 0;
	/*! \brief Sets the intensity of the sun's disk.
	\param[in] t - The time at which to set the intensity
	\param[in] sunDiskIntensity - The value of what to set the intensity to
	\return true if the setting was successful	
	*/
	virtual bool   setSunDiskIntensity(TimeValue t, float sunDiskIntensity) = 0;
	//@}

	//! \name Sun Disk Scale
	//! \brief The size of the sun in the sky.
	//@{
	/*! \brief Gets the scale of the sun.
	\param[in] t - The time at which we're getting the scale
	\param[in] valid - The interval at which we're getting the scale
	\return	The value of the scale
	*/
	virtual float  getSunDiskScale(TimeValue &t, Interval &valid) const = 0;
	/*! \brief Sets the scale of the sun.
	\param[in] t - The time at which to set the scale
	\param[in] sunDiskScale - The value of what to set the scale to
	\return true if the setting was successful	
	*/
	virtual bool   setSunDiskScale(TimeValue t, float sunDiskScale) = 0;
	//@}

	//! \name Sun Glow Intensity
	//! \brief The brightness of the glow surrounding the sun.
	//@{
	/*! \brief Gets the intensity of the sun's glow.
	\param[in] t - The time at which we're getting the intensity
	\param[in] valid - The interval at which we're getting the intensity
	\return	The value of the intensity
	*/
	virtual float  getSunGlowIntensity(TimeValue &t, Interval &valid) const = 0;
	/*! \brief Sets the intensity of the sun's glow.
	\param[in] t - The time at which to set the intensity
	\param[in] sunGlowIntensity - The value of what to set the intensity to
	\return true if the setting was successful	
	*/
	virtual bool   setSunGlowIntensity(TimeValue t, float sunGlowIntensity) = 0;
	//@}

	//! \name Background
	//! \brief The custom user specified background to be used as a background
	//! map in the scene.
	//@{
	/*! \brief Gets the current background being used.
	\param[in] t - The time at which we're getting background
	\param[in] valid - The interval at which we're getting the background
	\return	A pointer to a texture map of the background
	*/
	virtual Texmap*  getBackground(TimeValue &t, Interval &valid) const = 0;
	/*! \brief Sets the background of the scene.
	\param[in] t - The time at which to set the background
	\param[in] background - A pointer to a texture map of what to set the background to
	\return true if the setting was successful	
	*/
	virtual bool     setBackground(TimeValue t, Texmap* background) = 0;
	//@}
};

//! \brief Access interface for the Mental Ray Physical Sky Environment Map.
/*! This class represents the Mental Ray Physical Sky Environment Map, implemented by 3ds Max.
SDK client code can get this interface by querying 3ds Max for the current
environment map, then asking that object for this interface:
\code 
IMRPhysicalSkyEnvironmentMap* mrEnvMap = IMRPhysicalSkyEnvironmentMap2::GetThisInterface(GetCOREInterface()->GetEnvironmentMap());
\endcode
*/
class IMRPhysicalSkyEnvironmentMap2 : public IMRPhysicalSkyEnvironmentMap {
public:

	/* \brief Retrieves the unique identifier of this interface
	*/
	virtual Interface_ID GetID() { return IID_MR_PHYSICAL_SKY_ENV_MAP2; }

	/* \brief Fetches this interface on the given interface server.
    \return null if the interface doesn't exist.
	*/
    static IMRPhysicalSkyEnvironmentMap2* GetThisInterface(InterfaceServer* server)
    {
        return 
            (server != nullptr) 
            ? dynamic_cast<IMRPhysicalSkyEnvironmentMap2*>(server->GetInterface(IID_MR_PHYSICAL_SKY_ENV_MAP2)) 
            : nullptr;
    }

    struct ShadingParameters
    {
        //! The Default constructor initializes all values to 0
        ShadingParameters();

        //! The sun direction in world space, null vector if no sun is present.
        Point3 sunDirection;
        //! Global multiplier for the environment; both the sun and sky are affected by this.
        float multiplier;
        //! Multiplier used to bring physical values (candelas) into RGB space. This takes into account the scene's phyiscal scale, global light level, and global light tint.
        AColor rgbUnitConversion;
        //! The haze value, expressed in the [0.0, 15.0] range.
        float haze;
        //! The texture for the haze property; null if not present or disabled.
        Texmap* hazeTexture;
        //! Red-blue shift property, in the range[-1.0, 1.0]. Positive values increase red, reduce blue, Negative values increase blue, reduce red.
        float redBlueShift;
        //! The texture for the red-blue shift property; null if not present or disabled.
        Texmap* redBlueShiftTexture;
        float saturation;
        //! The texture for the saturation property; null if not present or disabled.
        Texmap* saturationTexture;
        //! Ground color, used to generate an infinite fake ground plane.
        AColor groundColor;
        //! The texture for the ground color property; null if not present or disabled.
        Texmap* groundColorTexture;
        //! The night color, used as minimum illuminance level for the sky.
        AColor nightColor;
        //! The texture for the night color property; null if not present or disabled.
        Texmap* nightColorTexture;
        //! The horizon height offset.
        float horizonHeight;
        //! The amount of blurring for the horizon line.
        float horizonBlur;
        //! The background color; null if not present or disabled.
        Texmap* backgroundTexture;
        //! The scale for the sun disc (at 1.0, the disc should be scaled to physical size).
        float sunDiskScale;
        //! The intensity for the sun disc (at 1.0, the sun should emit as much light as the real sun).
        float sunDiskIntensity;
        //! The intensity of the sun glow which surrounds the disc.
        float sunGlowIntensity;
        //! The visibility distance used to compute aerial perspective.
        float visibilityDistance;

        //! True if the sky luminance is to be calculated using the following Perez distribution coefficients, rather than being calculated using the Preetham coefficients.
        bool useCustomPerezLuminanceCoefficients;
        //! Custom Perez function coefficient A.
        float customPerez_A;
        //! Custom Perez function coefficient B.
        float customPerez_B;
        //! Custom Perez function coefficient C.
        float customPerez_C;
        //! Custom Perez function coefficient D.
        float customPerez_D;
        //! Custom Perez function coefficient E.
        float customPerez_E;
        //! Total diffuse illuminance of the sky on a horizontal surface. The zenith luminance, used in the Perez function, must be derived from this.
        float customPerez_DiffuseHorizontalIlluminance;
    };

	/*! \brief Fetches the various shading parameters of the sky environment.
    This method is used to fetch all the relevant shading parameters exposed by the sky environment map. It implements full support for the
    "inherit from scene Sun & Sky" parameter: if the inherit flag is set, this method will search the scene for Sun & Sky objects, will read its
    properties, and will return them instead of the sky environment's.
	\param [in] t - The time at which to evaluate the properties.
	\param [in, out] valid - The validity interval, its input value being intersected with that of all fetched properties.
    \return The structure containing all fetched parameter values.
	*/
    virtual ShadingParameters GetShadingParameters(const TimeValue t, Interval& valid) = 0;

};

#include "imrPhysicalSkyEnvironmentMap.inline.h"