/**********************************************************************
 *<
	FILE: toneop.h

	DESCRIPTION: Definitions for tone operator. Tone operators are used
	             to map high dynamic range images into RGB. Usually they
	             are used with physical energy values.

	CREATED BY: Cleve Ard

	HISTORY:

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/
#pragma once

#include "imtl.h"
#include "render.h"

// The tone operator uses the standard Special Effects
// parameter dialog class for its UI.
typedef SFXParamDlg ToneOpParamDlg;

/*=====================================================================
 * Tone Operator Interface class
 *===================================================================*/

/*! \sa  Class SpecialFX, Class ToneOperatorInterface, Class IRendParams, Class RendParams, Class RenderGlobalContext, Class RenderMapsContext, Class ShadeContext, Class Interval.\n\n
\par Description:
This is the base class for the creation of Tone Operator plug-ins. A Tone
Operator performs two functions:\n\n
1. It converts physically based values to RGB for filtering and display.
The renderer calls the tone operator immediately after <b>Mtl::Shade</b> is
called.\n\n
2. It balances physical and non-physical lighting.\n\n
The tone operator balances the physical and non-physical lighting by providing
a scale relationship between them. The scale converts physical candelas to the
non-physical value 1.0. Physically based objects in the 3ds Max scene use this
scale to convert their values for use by the renderer and materials. The tone
operator then converts the scaled value to RGB for display.\n\n
An example of this is the tone operator for a radiosity plug-in. 3ds Max works
in a lighting space where values run from 0 to 1 and don't have any meaning.
Pre-rendered Reflection maps, Refraction maps and self-illumination maps also
use a 0 to 1 scale without any meaning. A radiosity plug-in introduces physical
values to 3ds Max that range from 0 to 90000 for the sun.\n\n
So the question is "How do we mix these values with physical values?" One
solution is to use a scale. Physical values are scaled to "3ds Max" values.
Then they are processed by the shaders and materials, and then the scaled
values are converted to RGB by ScaledToRGB.\n\n
So the PhysicalUnits, ScalePhysical and ScaleRGB are used to convert from 3ds
Max lighting values to physical lighting values. We can use this to balance 3ds
Max lights with physical lights, and to assign physical values to 3ds Max
lights when we want to use them in a radiosity solution.\n\n
The tone operator may include a UI that allows the user to set the scale, or it
can set the scale apriori. The scale is also used for reflection maps, which
are usually implemented in 32 bit bitmaps. If the scale is set too high,
reflection maps can show banding because of rounding errors. If the scale is
set too low, reflection maps can wash out because of clipping values to
0...255.\n\n
The tone operator uses the standard Special Effects parameter dialog class for
its user interface.\n\n
Note: <b>typedef SFXParamDlg ToneOpParamDlg;</b>   */
#pragma warning(push)
#pragma warning(disable:4239 4100)

class ToneOperator : public SpecialFX {
public:

	ToneOperator() { ClearAFlag(A_TONEOP_PROCESS_BG); }

	// Standard methods from ReferenceMaker and Animatable
	RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget,  PartID& partID, RefMessage message, BOOL propagate) {return REF_SUCCEED;}
	SClass_ID SuperClassID() { return TONE_OPERATOR_CLASS_ID; }
	
	// Saves and loads name. These should be called at the start of
	// a plug-in's save and load methods.
	IOResult Save(ISave *isave) { return SpecialFX::Save(isave); }
	IOResult Load(ILoad *iload) { return SpecialFX::Load(iload); }

	virtual BOOL Active(TimeValue t) { return !TestAFlag(A_TONEOP_DISABLED); }
	/*! \remarks This method indicates whether the tone operator is active. The default
	implementation does not use the <b>TimeValue t</b>. The result of the
	default implementation can be retrieved using <b>SpecialFX::GetActive</b>.
	If you override this method and change the mechanism for storing this
	state, you should also override <b>SpecialFX::GetActive</b> so the correct
	state is returned.
	\par Parameters:
	<b>bool active</b>\n\n
	A boolean indicating if the tone operator is active.\n\n
	<b>TimeValue t</b>\n\n
	The time at which the active check is made.
	\par Default Implementation:
	<b>if (active ^ (TestAFlag(A_ATMOS_DISABLED) == 0)) {</b>\n\n
	<b> if (active) {</b>\n\n
	<b>ClearAFlag(A_ATMOS_DISABLED);</b>\n\n
	<b> }</b>\n\n
	<b> else {</b>\n\n
	<b>SetAFlag(A_ATMOS_DISABLED);</b>\n\n
	<b> }</b>\n\n
	<b> NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);</b>\n\n
	<b>}</b> */
	virtual void SetActive(bool active, TimeValue t);

	/*! \remarks Returns the state of <b>A_TONEOP_PROCESS_BG</b>, indicating
	whether the ToneOperator will be processing the background. */
	bool GetProcessBackground() { return TestAFlag(A_TONEOP_PROCESS_BG) != 0; }
	/*! \remarks This method allows you to set <b>A_TONEOP_PROCESS_BG</b>.
	\par Parameters:
	<b>bool active</b>\n\n
	TRUE to activate, FALSE to deactivate. */
	void SetProcessBackground(bool active);

	bool GetIndirectOnly() { return TestAFlag(A_TONEOP_INDIRECT_ONLY) != 0; }
	void SetIndirectOnly(bool active);

	// UI Access

	// Put up a modal dialog that lets the user edit the plug-ins parameters.
	/*! \remarks This method creates the rollup pages in the render panel that
	lets the user edit the tone operator's parameters. You can use
	<b>IRendParams::AddRollupPage</b> and <b>IRendParams::DeleteRollupPage</b>
	to manage your rollup pages directly. Or, if your parameters are stored in
	a ParamBlock2 object, you can use <b>CreateRParamMap2</b> and
	<b>DestroyRParamMap2</b> to manage the rollups. You may return NULL, if no
	UI is required.
	\par Parameters:
	<b>IRendParams *ip</b>\n\n
	Points to the render parameter dialog interface. You may call the methods
	of that class using this pointer.
	\return  Pointer to the tone operator dialog.
	\par Default Implementation:
	<b>{ return NULL; }</b> */
	virtual ToneOpParamDlg *CreateParamDialog(IRendParams *ip) { return NULL; }

	// Implement this if you are using the ParamMap2 AUTO_UI system and the 
	// atmosphere has secondary dialogs that don't have the effect as their 'thing'.
	// Called once for each secondary dialog for you to install the correct thing.
	// Return TRUE if you process the dialog, false otherwise.
	/*! \remarks Implement this if you are using the ParamMap2 AUTO_UI system and the
	atmosphere has secondary dialogs that don't have the effect as their
	'thing'. Called once for each secondary dialog for you to install the
	correct thing.
	\par Parameters:
	<b>ToneOpParamDlg* dlg</b>\n\n
	Points tot he tone operator dialog.
	\return  Return TRUE if you process the dialog, FALSE otherwise. */
	virtual BOOL SetDlgThing(ToneOpParamDlg* dlg) { return FALSE; }

	// Render access

	// Does this tone operator really map physical values to RGB. This method
	// is provided so shaders can determine whether the shading calculations
	// are in physical or RGB space.
	/*! \remarks Returns a boolean which indicates if this tone operator really maps
	physical values to RGB. This method is provided so shaders can determine
	whether the shading calculations are in physical or RGB space.
	\par Default Implementation:
	<b>{ return true; }</b> */
	virtual bool IsPhysicalSpace() const { return true; }

	// This method is called once per frame when the renderer begins. This
	// gives the atmospheric or rendering effect the chance to cache any
	// values it uses internally so they don't have to be computed on
	// every pixel. But, this method should not be used to perform any very
	// long tasks, such as sampling the environment to calculate a
	// mapping histogram. This would be the likely method that caches the
	// frames physical scaling value.
	/*! \remarks This method is called once per frame when the renderer
	begins. This gives the tone operator the chance to cache any values it uses
	internally so they don't have to be computed on every pixel. But, this
	method should <b>not</b> be used to perform any very long tasks. This would
	be the likely method that caches the frames physical scaling value.
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which the rendering is beginning.\n\n
	<b>Interval\& valid</b>\n\n
	The validity interval for the update.
	\par Default Implementation:
	<b>{ }</b> */
	virtual void Update(TimeValue t, Interval& valid) { }

    /*! \brief Shortcut method for easily supporting new functionality from ToneOperator2.
        This method automatically checks if this class implements the new ToneOperator2 interface. If it does, it forwards the call to it.
        Otherwise, it calls the legacy method from ToneOperator.
        \see virtual void ToneOperator2::Update(const TimeValue, INode*, const IPoint2&, float, Interval&) 
        \remark This method suffers from the overhead of a dynamic cast, but that is expected to go away in the near future as all functionality
            from ToneOperator2 will be folded into ToneOperator. */
    void Update(const TimeValue t, INode* const cameraNode, const IPoint2 renderResolution, const float pixelAspectRatio, Interval& validity);

	// This method is called for the operator to do any work it needs
	// to do prior to rendering. Rendering using the RenderMapsContext
	// uses the identity tone operator.
	/*! \remarks This method is called for the operator to do any work it
	needs to do prior to rendering. You may use this method to perform a
	subrender to sample the rendered output for histogramming or automatic
	exposure.
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which the rendering is taking place.\n\n
	<b>RenderMapsContext\& rmc</b>\n\n
	The context of the map rendering.
	\return  True means this method succeeded. False means it didn't. This
	method should return false if it the sub-render fails or if it can't
	allocate memory or some other error occurs. If BuildMaps returns false, the
	render is aborted.
	\par Default Implementation:
	<b>{ return true; }</b> */
	virtual bool BuildMaps(TimeValue t, RenderMapsContext& rmc)
		{ return true; }

	// This method is called during subrenders to give the tone operator
	// a chance to sample the image with full dynamic range. If your operator
	// needs to sample the image, you can set a flag so you know when you are
	// sampling.
	virtual void SubRenderSample(float energy[3]) { }

    // Map an scaled energy value into RGB. The first version of the
	// method converts a color value and the second converts a monochrome
	// value. The converted color value is stored in <i>energy</i>.
	// The converted monochrome value is returned.
	// This method assumes that Update has been called to cache the
	// various values needed by the tone operator.
	// By using a float array to pass in color values, we can use the same
	// routine to handle the various classes used to store color information,
	// for example, Color, AColor and Point3. The red, green and blue
	// components are stored in that order in the array.
	/*! \remarks This method maps a scaled energy value into RGB. This version
	converts a color value. The converted color value is stored in
	<b>energy</b>.\n\n
	This method assumes that <b>Update()</b> has been called to cache the
	various values needed by the tone operator.\n\n
	Note: By using a float array to pass in color values, we can use the same
	routine to handle the various classes used to store color information, for
	example, Color, AColor and Point3.
	\par Parameters:
	<b>float energy[3]</b>\n\n
	The input energy value to convert. The converted color value is stored here
	as well. The red, green and blue components are stored in that order in the
	array. The valid ranges are -infinity to +infinity, but the returned value
	is clipped by the renderer very quickly to [0,1]. The tone operator can do
	it's own clipping, but it isn't required. */
	virtual void ScaledToRGB(float energy[3]) = 0;

    /*! \brief Shortcut method for easily supporting new functionality from ToneOperator2.
        This method automatically checks if this class implements the new ToneOperator2 interface. If it does, it forwards the call to it.
        Otherwise, it calls the legacy method from ToneOperator.
        \see virtual void ToneOperator2::ScaledToRGB(float energy[3], const Point2& xyCoord) const
        \remark This method suffers from the overhead of a dynamic cast, but that is expected to go away in the near future as all functionality
            from ToneOperator2 will be folded into ToneOperator. */
    void ScaledToRGB(float energy[3], const Point2& xyCoord);
    
    /*! \remarks This method maps a scaled energy value to monochrome. The
	converted monochrome value is returned. This method assumes that
	<b>Update()</b> has been called to cache the various values needed by the
	tone operator.
	\par Parameters:
	<b>float energy</b>\n\n
	The input energy value to convert. */
	virtual float ScaledToRGB(float energy) = 0;

    /*! \brief Shortcut method for easily supporting new functionality from ToneOperator2.
        This method automatically checks if this class implements the new ToneOperator2 interface. If it does, it forwards the call to it.
        Otherwise, it calls the legacy method from ToneOperator.
        \see virtual void ToneOperator2::ScaledToRGB(float energy, const Point2& xyCoord) const
        \remark This method suffers from the overhead of a dynamic cast, but that is expected to go away in the near future as all functionality
            from ToneOperator2 will be folded into ToneOperator. */
    float ScaledToRGB(float energy, const Point2& xyCoord);

	// Get and Set the Physical value that is scaled to 1.
	/*! \remarks This method returns the physical value that is scaled to 1.
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which to return the value.\n\n
	<b>Interval\& valid = Interval(0,0)</b>\n\n
	The validity interval for the value. */
	virtual float GetPhysicalUnit(TimeValue	t, Interval& valid = Interval(0,0)) const = 0;

	/*! \remarks This method sets the physical value that is scale to 1. This is simply a
	programmatic method to change the physical scaling of the tone operator.
	Valid values are (0,+infinity).
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which to set the value.\n\n
	<b>Interval\& valid = Interval(0,0)</b>\n\n
	The validity interval for the value. */
	virtual void SetPhysicalUnit(float value, TimeValue t) = 0;

	// Scale physical values so they can be used in the renderer. The
	// first version of the method converts a color value and the second
	// converts a monochrome value. The converted color value is stored
	// in <i>energy</i>. The converted monochrome value is returned.
	// This method assumes that Update has been called to cache the
	// various values needed by the tone operator.
	// By using a float array to pass in color values, we can use the same
	// routine to handle the various classes used to store color information,
	// for example, Color, AColor and Point3. The red, green and blue
	// components are stored in that order in the array.
	/*! \remarks This method is used to scale a physical color value so it may be used in
	the renderer.\n\n
	This method assumes that Update has been called to cache the various values
	needed by the tone operator.\n\n
	Note: By using a float array to pass in color values, we can use the same
	routine to handle the various classes used to store color information, for
	example, Color, AColor and Point3.
	\par Parameters:
	<b>float energy[3]</b>\n\n
	The input and output (converted) color value. The colors are stored as
	red=energy[0], green=energy[1], and blue=energy[2]. */
	virtual void ScalePhysical(float energy[3]) const = 0;
	/*! \remarks This method is used to scale a physical monochrome value so it may be used
	in the renderer.\n\n
	This method assumes that Update has been called to cache the various values
	needed by the tone operator.
	\par Parameters:
	<b>float energy</b>\n\n
	The input value to scale.
	\return  The scaled output value is returned. */
	virtual float ScalePhysical(float energy) const = 0;

	// Scale RGB values, just supplied to invert ScalePhysical. The first
	// version of the method converts a color value and the second
	// converts a monochrome value. The converted color value is stored
	// in <i>energy</i>. The converted monochrome value is returned.
	// This method assumes that Update has been called to cache the
	// various values needed by the tone operator.
	// By using a float array to pass in color values, we can use the same
	// routine to handle the various classes used to store color information,
	// for example, Color, AColor and Point3. The red, green and blue
	// components are stored in that order in the array.
	/*! \remarks This method is called to scale RGB values (the inverse of
	ScalePhysical()).\n\n
	This method assumes that Update has been called to cache the various values
	needed by the tone operator.
	\par Parameters:
	<b>float color[3]</b>\n\n
	The input values to scale and storage for the output scaled values as well.
	The colors are stored as red=energy[0], green=energy[1], and
	blue=energy[2]. The output values are in the range 0-1. */
	virtual void ScaleRGB(float color[3]) const = 0;
	/*! \remarks This method is called to scale a monochrome value (the inverse of
	ScalePhysical()).
	\par Parameters:
	<b>float color</b>\n\n
	The input value to scale.
	\return  The scaled output value is returned. */
	virtual float ScaleRGB(float color) const = 0;
	
	// Is this tone operator invertible
	bool CanInvert();
	
	// Calculate the physical value from the display value
	void RGBToScaled(float energy[3]);
	float RGBToScaled(float energy);
};
#pragma warning(pop)

/*=====================================================================
 * class ToneOperator2
 *===================================================================*/

//! \brief Extended version of the ToneOperator interface.
/*! This interface enables tone operator plugins to implement more advanced functionality, such as exposure that is dependent on the camera
    being rendered. To retrieve an pointer to this interface from a ToneOperator*, use a dynamic cast:
    \code 
        ToneOperator* tone_op = <something>;
        ToneOperator2* tone_op2 = dynamic_cast<ToneOperator2*>(tone_op);
    \endcode 
    \remark This interface supersedes class ToneOperator. All plugins should update to using ToneOperator2 in order to be fully compatible with
        new and future features. */
class ToneOperator2 : public ToneOperator
{
public:

    //! \brief Enables processing a tone operator independently of class ToneOperator, for example in a multi-threaded manner.
    /*! As class ToneOperator derives from ReferenceTarget, it may only safely be used from the main thread. But tone operators can be expensive
        to process, making it desirable to process them from non-UI threads (or even from multiple threads). This interface encapsulates everything
        a tone operator needs to process itself, but doesn't derive from ReferenceTarget so may safely be used from arbitrary threads.
        The implementation of this interface <b>must allow for concurrent calls from arbitrary threads</b>.
        \see ToneOperator2::AllocateThreadedProcessor() */
    class ThreadedProcessor : public MaxHeapOperators
    {
    public:
        virtual ~ThreadedProcessor() {}

        //! See virtual void ScaledToRGB(float energy[3])
        //! \remark This method is thread-safe, it may be called from multiple threads concurrently.
        virtual void ScaledToRGB(float energy[3], const Point2& xyCoord) const = 0;
        //! See virtual float ScaledToRGB(float energy)
        //! \remark This method is thread-safe, it may be called from multiple threads concurrently.
        virtual float ScaledToRGB(float energy, const Point2& xyCoord) const = 0;
    };

    /*! \brief Initializes the tone operator at the given time, and with the given camera & view options.
        This method needs to be called before any ToneOperator functionality is used - for example, before ScaledToRGB() is called. It enables
        the ToneOperator to initialize itself with the given view parameters, as well as cache data which would be costly to compute for every pixel.
        Using the tone operator without previously calling this method will result in incorrect tone mapping: the parameters being used may not be
        updated for the correct time or camera.
        \param t The time at which the tone operator is to be evaluated.
        \param cameraNode The camera node being used for rendering. The tone operator is free to affect the tone mapping process based on the camera parameters.
        \param renderResolution The resolution being rendered, used to determine the position in raster space when calling ScaleToRGB().
        \param pixelAspectRatio The pixel aspect ratio, which can be used to derive an image aspect ratio.
        \param [in, out] validity The validity of the parameters which were evaluated by the call. */
    virtual void Update(const TimeValue t, INode* const cameraNode, const IPoint2& renderResolution, const float pixelAspectRatio, Interval& validity) = 0;
    /*! \brief Scales a value from physical space to RGB space.
        Rendered images, as directly output by renderers, will store values in physical units (candelas per meter-squared) which must be transformed
        into RGB space before they can be viewed on a monitor. This method performs this transformation.
        \param [in, out] energy The physical RGB value, which gets converted in-place into an RGB value.
        \param xyCoord The x/y coordinate of the pixel being scaled, the valid range being [0, renderResolution] (renderResolution having been passed to Update()). */
    virtual void ScaledToRGB(float energy[3], const Point2& xyCoord) const = 0;
    /*! \brief Scales a <b>grayscale</b> value from physical space to RGB space. 
        \see void ScaledToRGB(energy[3], xyCoord) const
        \param [in, out] energy The physical RGB <b>grayscale</b> value, which gets converted in-place into an RGB <b>grayscale</b> value.
        \param xyCoord The x/y coordinate of the pixel being scaled, the valid range being [0, renderResolution] (renderResolution having been passed to Update()). */
	virtual float ScaledToRGB(float energy, const Point2& xyCoord) const = 0;

    /*! \brief Allocates an interface which enables processing of the tone operator from arbitrary (or multiple) threads.
        \return A pointer to a newly allocated interface which is then owned exclusively by the caller. The caller is responsible for deleting
            the pointer once done. <b>May return null</b> if the tone operator doesn't implement support for the ThreadedProcessor interface,
            in which case the tone operator must be processed <b>from the main thread</b>, using ToneOperator::ScaledToRGB(). 
        \remark This method may only be called from the main thread - as every other method in in this class. 
        \remark The tone operator must be updated using ToneOperator2::Update() or ToneOperator::Update() <b>before calling this method</b>. The 
            processor to be returned will reflect the parameter values setup by the last call to Update(). */
    virtual const ThreadedProcessor* AllocateThreadedProcessor() const = 0;
};


/*=====================================================================
 * Invertible Tone Operator Interface class
 *===================================================================*/
// Not all tone operators can map display RGB values to physical values.
// This interface is used by tone operators that are invertible to
// do this mapping.

#define INVERTABLE_TONE_OPERATOR_INTERFACE	Interface_ID(0xbe9171b, 0x71183b19)

class ToneOperatorInvertable : public BaseInterface {
public:
	// Calculate the physical value from the display value
	virtual void InverseMap(float rgb[3]) = 0;
	virtual float InverseMap(float rgb) = 0;
};

/*=====================================================================
 * Tone Operator Core Interface class
 *===================================================================*/

// This class is used to get access to the tone operator and
// its UI.
#define TONE_OPERATOR_INTERFACE	Interface_ID(0x1563269c, 0x7ec41d84)

/*! \sa  Class FPStaticInteface,  Class ToneOperator\n\n
\par Description:
This class allows plug-ins and the scripter to get access to the tone operator
assigned to a scene. You can get a pointer to the interface using the global
interface pointer in this manner:\n\n
<b>ToneOperatorInterace* toneOpInt =
static_cast\<ToneOperatorInterface*\>( GetCOREInterface(TONE_OPERATOR_INTERFACE));</b>\n\n
If the return value is NULL, the running version of 3ds Max doesn't support
tone operators. If the return value is not NULL, you can use these methods to
perform some scene management.  */
class ToneOperatorInterface : public FPStaticInterface {
public:
	typedef void (*ToneChangeCallback)(
		ToneOperator*  newOp,
		ToneOperator*  oldOp,
		void*          param
	);

	// Get and Set the tone operator in the scene
	/*! \remarks This method returns the current tone operator assigned to a
	scene. If no tone operator is assigned NULL is returned. */
	virtual ToneOperator* GetToneOperator() const = 0;
	/*! \remarks This method assigns a tone operator to the scene. To remove a
	tone operator, assign NULL. When a new tone operator is assigned the
	current operator is removed automatically. */
	virtual void SetToneOperator(ToneOperator* op) = 0;

	/*! \remarks This method registers a callback that is called when the tone
	operator is changed. Note the definition of ToneChangeCallback:\n\n
	<b>typedef void (*ToneChangeCallback)(ToneOperator* newOp, ToneOperator*
	oldOp, void* param);</b>
	\par Parameters:
	<b>ToneChangeCallback callback</b>\n\n
	The callback to register.\n\n
	<b>void* param</b>\n\n
	This parameter is passed to the callback function as the parameter argument
	when it is called. */
	virtual void RegisterToneOperatorChangeNotification(
		ToneChangeCallback   callback,
		void*                param
	) = 0;
	/*! \remarks This method un-registers a callback that was registered by
	RegisterToneOperatorChangeNotification. Note the definition of
	ToneChangeCallback:\n\n
	<b>typedef void (*ToneChangeCallback)(ToneOperator* newOp, ToneOperator*
	oldOp, void* param);</b>
	\par Parameters:
	<b>ToneChangeCallback callback</b>\n\n
	The callback to un-register.\n\n
	<b>void* param</b>\n\n
	This parameter is passed to the callback function as the parameter argument
	when it is called. */
	virtual void UnRegisterToneOperatorChangeNotification(
		ToneChangeCallback   callback,
		void*                param
	) = 0;
};


/*=====================================================================
 * class IToneOperatorExtension
 *
 * This class is an extension to allow tone operators to work more
 * closely with radiosity engines. The interface supports tone operator
 * that can choose to display illuminance (irradiance) or luminance
 * (radiance). This is especially useful for performing lighting analysis
 * through special-purpose tone operators.
 *===================================================================*/
#define ITONEOPERATOR_EXTENSION_INTERFACE Interface_ID(0x512b3541, 0x1c413aad)

class IToneOperatorExtension : public BaseInterface {

public:

    enum Quantity {
        kQuantity_Illuminance = 0,
        kQuantity_Luminance = 1
    };

    virtual Quantity GetUsedQuantity() const = 0;
    virtual void SetUsedQuantity(Quantity q) = 0;

    // -- from BaseInterface
    virtual Interface_ID GetID() { return ITONEOPERATOR_EXTENSION_INTERFACE; }
    
};


#include "toneOp.inline.h"
