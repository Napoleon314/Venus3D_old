//
// Copyright 2010 Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.

#pragma once
#include "maxheap.h"
#include "baseinterface.h"
#include "maxapi.h"
#include "GetCOREInterface.h"
#include "matrix3.h"
#include "maxscript/ScripterExport.h"

// forward declarations
class ClassDesc;
class INode;
class ParamBlock;
class IParamBlock2;
class ReferenceTarget;
class Mtl;
class Interface;
class ParamBlock2;


/** Provides various methods to emit pieces of script to the Macro
 * Recorder. There are also methods to specify the nature of the scripts generated
 * by the user operating 3ds Max. Additionally there are methods to enable or
 * disable the recording of scripts.
 * This class may be used by plug-ins but is also used internally as part of the
 * 3ds Max Macro Recorder. Inside key areas of 3ds Max macro scripts are emitted
 * corresponding to the operation being performed. For example when a call goes to
 * a parameter block to set a value 3ds Max internally call this classes method
 * ParamBlock2SetValue(...) to emit script to record the change. Thus, many
 * operations a plug-in performs are recorded automatically. There are however
 * operations a plug-in can perform which won't be recorded automatically. In
 * these cases methods of this class may be used to emit script to record these
 * operations.
 * Several of the functions in this class use an ellipsis argument (...).
 * This ellipsis is the var-args style of passing information to a method. This
 * allows a developer to pass a variable number of values to the method. These are
 * typically a set of tag values followed by some C++ types. The sample code shown
 * with many of the methods shows how this is done. For a starting point for more
 * general information on variable argument lists see va_arg in the Window
 * help file. An instance of the MacroRecorder can be retrieved using the function 
 * Interface::GetMacroRecorder().
 * \see ClassDesc, INode, IParamBlock, IParamBlock2, Matrix3, ReferenceTarget.
 */
class MacroRecorder : public BaseInterfaceServer
{
public:
	class MacroRecorderDisable;

	/** Used for implementing a custom creation manager. This method 
	 * starts a special accumulation 'mode' in which certain
	 * other Macro Recorder calls are treated specially until the next
	 * <b>EmitScript()</b>. <b>BeginCreate()</b> effectively signals the start of
	 * a scene node creation and enters a mode in which calls to
	 * <b>SetNodeTM()</b>, <b>SetProperty()</b>, <b>ParamBlockXSetValue()</b> and
	 * <b>SetSelProperty()</b> all generate keyword parameters to the current
	 * constructor, rather than emitting stand-alone property assignment scripts.
	 * Outside the 'create' mode, a call to <b>SetNodeTM()</b> would generate
	 * something like:
	 * <b>move $foo [10,0,0]</b>\n\n
	 * but when in the mode would add a pos: argument to the constructor:\n\n
	 * <b>sphere radius:20 pos:[10,0,0]</b>
	 * \par Parameters:
	 * <b>ClassDesc* cd</b>
	 * Points to the class descriptor for the plug-in.
	 * \return  This tells you whether MAXScript successfully entered the
	 * BeginCreate mode. It will fail if MAXScript can't create the object
	 * described by the ClassDesc, so you might use it in some generic situations
	 * to decided whether to call the closing EmitScript(). */
	virtual BOOL BeginCreate(ClassDesc* cd, int flags = 0) = 0;

	/** \internal For internal use only. */
	virtual void SetNodeTM(INode* n, Matrix3 m) = 0;

	/** \internal For internal use only. */
	virtual void ParamBlockSetValue(ParamBlock* pb, int i, BYTE type, ...) = 0;

	/** \internal For internal use only. */
	virtual void ParamBlock2SetValue(IParamBlock2* pb, int i, int tabIndex, BYTE type, ...) = 0;

	/** \internal For internal use only. */
	virtual void ParamBlock2SetCount(IParamBlock2* pb, int i, int n) = 0;

	/** Provides a simple way to emit a property assignment
	 * script. An example of this type of script is s: $sphere01.radius = 50
	 * \param targ Pointer to the object whose property is changing.
	 * \param prop_name The string which is the name of the property. This is the fixed
	 * machine-parsable name.
	 * \param type One of the type tags from List of Macro Recorder Value Types
	 * \param ... This ellipsis is the var-args style of passing information to a method.
	 * This allows a developer to pass a variable number of values to a method.
	 * These are typically a set of 'tags' followed by some C++ data types. */
	virtual void SetProperty(ReferenceTarget* targ, const MCHAR* prop_name, BYTE type, ...) = 0;
	/** Used to signal that an mr_reftarg argument in
	 * the currently accumulating script should be emitted as a copy. For example,
	 * when maps or materials are dragged onto sub-map/mtl buttons in the material
	 * editor, an instance/copy requester dialog is presented and depending on the
	 * choice, either a direct assignment or assignment of a copy is
	 * appropriate:
	 * \code
	 * meditMaterials[4].materialList[2] = meditMaterials[11] //instance
	 * meditMaterials[4].materialList[2] = copy meditMaterials[11] //copy
	 * \endcode
	 * The actual assignment script is set up using a
	 * macroRecorder-\>SetProperty() call with the dropped map/material
	 * supplied as an mr_reftarg argument. In situations where the
	 * copy/instance status is known, you can emit directly an <b>mr_funcall</b>
	 * argument for the copy, but there may be situations in which this choice is
	 * decided in some piece of remote code or control flow and so you can use
	 * this method to condition the emitted script to apply a 'copy ' call.
	 * \param to_copy The pointer to the object to be copied. */
	virtual void SetCopy(ReferenceTarget* to_copy) = 0;
	
	/** Provides a simple way to emit a property assignment
	 * script for the current selection set. Sample Code:
	 * \code 
	 * macroRecorder->SetSelProperty(_M("material"), mr_reftarg, (Mtl*)dropThis);
	 * \endcode
	 * \param prop_name The name of the property to set.
	 * \param type One of the type tags from List of Macro Recorder Value Types.
	 * \param ... This ellipsis is the var-args style of passing information to a method.
	 * This allows a developer to pass a variable number of values to a method.
	 * These are typically a set of 'tags' followed by some C++ types.
	 */
	virtual void SetSelProperty(const MCHAR* prop_name, BYTE type, ...) = 0;
	
	/** Used to build a MAXScript function call. In
	 * the general case, such a call may have positional arguments followed by
	 * keyword arguments. Sample Code:
	 * \code
	 * macroRecorder->FunctionCall(_M("addModifier"), 2, 1, mr_reftarg, this,
	 * mr_create, mod->ClassID(), mod->SuperClassID(), 0, _M("before"), mr_int,
	 * before);
	 * \endcode
	 * This generates an addModifier() function call, such as * <b>addModifier $foo (bend()) before:3</b>
	 * The call has 2 positional arguments and 1 keyword argument (hence the <b>2,
	 * 1</b>). The first positional is given as <b>mr_reftarg, this</b> which
	 * refers to the current node, the second positional is given as <b>mr_create,
	 * mod-\>ClassID(), mod-\>SuperClassID(), 0</b> which causes a 0-argument
	 * constructor to be emitted for the modifier, and finally the single keyword
	 * argument is given as <b>_M("before"), mr_int, before</b> which is the
	 * keyword name followed by the arg value. 
	 * \param op_name Pointer to the function to call.
	 * \param arg_count The number of positional arguments in the var-args section.
	 * \param keyarg_count The number of keyword arguments in the var-args section.
	 * \param ... This ellipsis is the var-args style of passing information to a method.
	 * This allows a developer to pass a variable number of values to a method.
	 * These are typically a set of 'tags' followed by some C++ types. List of Macro
	 * Recorder Value Types for a list of potential tags and arguments.
	 */ 
	virtual void FunctionCall(const MCHAR* op_name, int arg_count, int keyarg_count, ...) = 0;
	
	/** Emits a piece of macro script as a literal string. To
	 * understand when this is used consider the following example. Say you have a
	 * button in your user interface which does a certain thing but there is no
	 * way using the other macro recorder calls of constructing the piece of
	 * script that you need emitted. For instance the button may invoke a for
	 * loop. In such a case you can use this method to emit a macro script string
	 * which does a for loop. This is a string, just as you would type it into the
	 * MAXScript Listener.
	 * \param s Pointer to the string to emit. */
	virtual void ScriptString(const MCHAR* s) = 0;
	
	/** Called to emit script to record the assignment
	 * to a MAXScript variable.
	 * \param var_name The variable name.
	 * \param type One of the type tags from List of Macro Recorder Value Types
	 * \param ... This ellipsis is the var-args style of passing information to a method.
	 * This allows a developer to pass a variable number of values to a method.
	 * These are typically a set of 'tags' followed by some C++ types. */
	virtual void Assign(const MCHAR* var_name, BYTE type, ...) = 0;
	
	virtual void Assign(BYTE type, ...) = 0;
	
	virtual void OpAssign(const MCHAR* op, BYTE type, ...) = 0;
	
	virtual void OperandSequence(int count, BYTE type, ...) = 0;
	
	/** \internal For internal use only. */
	virtual BOOL BeginSelectNode() = 0;
	
	/** \internal For internal use only. */
	virtual void Select(INode*) = 0;
	
	/** \internal For internal use only. */
	virtual void DeSelect(INode*) = 0;
	
	/** \internal For internal use only. */
	virtual void MAXCommand(int com) = 0;
	
	virtual void AddComment(const MCHAR* str) = 0;
	
	/** Cancels and clears the currently accumulating script.
	 * This would be used for example, if the operation that is being accumulated
	 * can be canceled by the user, such as right-clicking out of a transform or a
	 * geometry node create. There are calls to <b>macroRecorder()-\>Cancel()</b>
	 * in the <b>MOUSE_ABORT</b> processing in the default creation manager. */
	virtual void Cancel() = 0;
	
	/** Signals the completion of an accumulating script,
	 * causing it to be frozen in the recorder pane and any new calls that might
	 * have been folded into the current script will cause a new one to be
	 * started. For example, when you drag the time slider, the sliderTime
	 * assignment script accumulates the changes, but when you let go of the mouse
	 * button, an EmitScript() is called, so that subsequent drags will start a
	 * separate script fragment. Same with interactive transforms and node
	 * creation. */
	virtual void EmitScript() = 0;
	
	/** Returns the property name of the 'i-th' sub-material of the
	 * specified material.
	 * \param m The material whose 'i-th' sub-material property name is returned.
	 * \param i The zero based index of the sub-material. */
	virtual MSTR GetSubMtlPropName(Mtl* m, int i) = 0;
	
	/**Returns the property name of the 'i-th' sub-texmap of the
	 * specified material.
	 * \param m The material or texmap whose 'i-th' sub-texmap property name is returned.
	 * \param i The zero based index of the sub-texmap.
	 */
	virtual MSTR GetSubTexmapPropName(ReferenceTarget* m, int i) = 0;
	
	/** Enables the Macro Recorder. This call is 'nestable', i.e. it
	 * uses a use counter internally so recursive or nested code can manage local
	 * enables and disable states. */
	virtual void Enable() = 0;
	
	/** Disables the Macro Recorder. This allows the developer to
	 * disable the automatic macro recording. */
	virtual void Disable() = 0;
	
	/** Returns true if the Macro Recorder is enabled (via
	 * Enable() ), false otherwise. */
	virtual BOOL Enabled() = 0;
	
	/** master enable and option controls. In the MAXScript Listener Window Macro 
	 * Recorder pull-down menu choice is an option to enable or disable the Macro 
	 * Recorder. This method corresponds to that state. 
	 * \return true if enabled, false if disabled.
	 */
	virtual BOOL MasterEnable() = 0;
	
	/** In the MAXScript Listener Window Macro Recorder pull-down menu
	 * choice is an option to enable or disable the Macro Recorder. This method
	 * sets this state.
	 * \param onOff true for enabled; false for disabled. */
	virtual void MasterEnable(BOOL onOff) = 0;
	
	/** Returns TRUE if code is emitted when command panels are
	 * changed; FALSE if code is not emitted. */
	virtual BOOL ShowCommandPanelSwitch() = 0;
	
	/** Determines if the macro recorder will emit script for command
	 * panel mode changes.
	 * \param onOff true to record command panel changes, false to ignore them. */
	virtual void ShowCommandPanelSwitch(BOOL onOff) = 0;
	
	/** Returns true if the macro recorder will emit script for 3ds
	 * Max toolbar tool selections, false otherwise. */
	virtual BOOL ShowToolSelections() = 0;
	
	/** Determines if the macro recorder will emit script for 3ds Max
	 * toolbar selections.
	 * \param onOff true to record toolbar selections, false to ignore them. */
	virtual void ShowToolSelections(BOOL onOff) = 0;
	
	/** Returns true if the macro recorder will emit script for 3ds
	 * Max menu selections, false otherwise. */
	virtual BOOL ShowMenuSelections() = 0;
	
	/** Determines if the macro recorder will emit script for 3ds Max
	 * menu selections.
	 * \param onOff true to record menu selections, false to ignore them. */
	virtual void ShowMenuSelections(BOOL onOff) = 0;
	
	/** Returns true if specific node names are used in the generated
	 * code false, if the current selection is used. */
	virtual BOOL EmitAbsoluteSceneNames() = 0;
	
	/** Controls whether the code generated refers to the exact
	 * node names being operated or or simply the current selection.
	 * \param onOff true to record absolute scene names, false to use the selection set. */
	virtual void EmitAbsoluteSceneNames(BOOL onOff) = 0;
	
	/** Returns true if recording absolute sub-object numbers, false
	 * if using the selection set. */
	virtual BOOL EmitAbsoluteSubObjects() = 0;
	
	/** Determines if code generated is relative to the current
	 * sub-object selection state or if explicit sub-object numbers are generated.
	 * \param onOff true to record explicit, absolute sub-object numbers; false to use the
	 * selection set. */
	virtual void EmitAbsoluteSubObjects(BOOL onOff) = 0;
	
	/** Tells if the code is generated using absolute transform
	 * assignments.
	 * \return true if code is generated using absolute transform
	 * assignments, false if relative transforms operations are generated. */
	virtual BOOL EmitAbsoluteTransforms() = 0;
	
	/** Sets if code is generated using absolute transform
	 * assignments.
	 * \param onOff true for on, false for off. */
	virtual void EmitAbsoluteTransforms(BOOL onOff) = 0;
};
   
#if defined(BLD_CORE) || defined(BLD_PARAMBLK2)
	extern MacroRecorder *macroRecorder;
#else
	extern ScripterExport MacroRecorder *macroRecorder;
#endif

/** \internal This global function is used internally to initialize the macro
 * recorder and should not be called by plug-in developers. */
extern ScripterExport void InitMacroRecorder();

//! \defgroup macro_recorder_value_types Macro Recorder Value Types
//! Value types 
//!@{
enum {	mr_int,				//!< An integer. Follow the tag with the int (Basic C type). Example: <pre>91462</pre>
		mr_float,			//!< A floating point value. Follow the tag with a double (Basic C type). Example: <pre>123.45</pre>
		mr_string,			//!< A string. Follow the tab with the string (MCHAR*) (Basic C type). Example: <pre>_T("Benson")</pre>
		mr_bool,			//!< A boolean. Follow the tag with a int (Basic C type). Example: <pre>TRUE</pre>
		mr_point3,			//!< A Point3. Follow the tag with a Point3* (MAX SDK type). Example: <pre>Point3 point (10,0,0);</pre>, 
							//!< referenced by pointer:  <pre>mr_point3, &point,...</pre>
		mr_color,			//!< A Color. Follow the tag with a Color* (MAX SDK type). Example: <pre>Color color (.047f,1.5f,0.0f);</pre>
							//!< referenced by pointer:  <pre>mr_color, &color,...</pre>
		mr_angaxis,			//!< An AngAxis. Follow the tag with an AngAxis* (MAX SDK type). Example: <pre>AngAxis angaxis(1.0f, 0.0f, 0.0f, 0.785f);</pre>
							//!< referenced by pointer:  <pre>mr_angaxis, &angaxis ...</pre>
		mr_quat,			//!< A Quaternion. Follow the tag with a Quat* (MAX SDK type). Example: <pre>Quat quat(1.0f, 0.0f, 0.0f, 1.0f);</pre>
							//!< referenced by pointer:  <pre>mr_quat, &quat ...</pre>
		mr_time,			//!< A TimeValue, follow the tag with a TimeValue (MAX SDK type). Example: <pre>GetTicksPerFrame() * 22;</pre>
		mr_reftarg,			//!< A Reference Target pointer. Follow the tag with a ReferenceTarget* (MAX SDK type). Example: <pre>ReferenceTarget *targ;</pre>,
							//!< referenced by pointer:  <pre>mr_reftarg, targ ...</pre>
		mr_bitarray,		//!< A bit array. Follow the tag with a pointer to a BitArray* (MAX SDK type). Example: <pre>BitArray bitArray;</pre>
							//!< referenced by pointer:  <pre>mr_bitarray, &bitArray ...</pre>
		mr_pbbitmap,		//!< A parameter block2 bitmap object. Follow the tag with a PBBitmap* (MAX SDK type). Example: <pre>PBBitmap *map;</pre>
							//!< referenced by pointer:  <pre>mr_pbbitmap, map ...</pre>
		mr_matrix3,			//!< A Matrix3. Follow the tag with a Matrix3* (MAX SDK type). Example: <pre>Matrix3 theMatrix;</pre>
							//!< referenced by pointer:  <pre>mr_matrix3, &theMatrix ...</pre>
		mr_nurbssel,		//!< A NURBS selection level. Follow the tag with: int, ReferenceTarget*. The int parameter is the selection level desired 
							//!< of the specified NURBS base object. (MAXScript type).  Example: <pre>ReferenceTarget *targ;</pre>.
							//!< referenced by pointer:  <pre>mr_nurbssel, 1, targ ...</pre>
		mr_meshselbits,		 
		mr_meshsel, 
		mr_subanim, 		//!< Example: <pre>mr_subanim, Animatable* animatable, int subAnimNum</pre>
		mr_animatable,		//!< Example: <pre>mr_animatable, Animatable* animatable</pre>
		mr_classid, 		//!< A Class ID. follow the tag with a Class_ID and a SClass_ID. (MAX SDK type). Example: <pre>mr_classid, Class_ID(123,456), SClass_ID(12345) ...</pre>
		mr_nurbsselbits,    //!< A NURBS selection level (identical to mr_nurbssel). Follow the tag with: int, ReferenceTarget*. The int parameter is the selection level desired 
							//!< of the specified NURBS base object. (MAXScript type).  Example: <pre>ReferenceTarget *targ;</pre>.
							//!< referenced by pointer:  <pre>mr_nurbsselbits, 1, targ ...</pre>
		mr_point4, 			//!< A Point4. Follow the tag with a Point4* (MAX SDK type). Example: <pre>Point4 point (10,0,0,10);</pre>, 
							//!< referenced by pointer:  <pre>mr_point4, &point,...</pre>
		mr_acolor,			//!< An AColor. Follow the tag with an AColor* (MAX SDK type). Example: <pre>AColor color (.047f,1.5f,0.0f, 0.5f);</pre>
							//!< referenced by pointer:  <pre>mr_acolor, &color,...</pre>
		mr_sel,				//!< A selection set. This tag works alone with no additional data. This tag denotes the current scene node selection and will
							//!< emit as either a '$' or an array of explicit objects, depending on the state of the EmitAbsoluteSceneNames() flag. (MAXScript type).
							//!< Example: <pre>mr_sel</pre> 
		mr_funcall,			//!< A function call. Follow the tag with the function name, then int, int, \<args\>. The first int parameter is the number of positional arguments, 
							//!< given first in the \<args\> items, as \<arg\>, the second int parameter is the number of keyword args following the positional args in \<args\>, 
							//!< given as TCHAR*, \<arg\> pairs. (MAXScript type). Example: 
							//!< <pre>macroRecorder->Assign(mr_index, mr_varname, _T("meditMaterials"), mr_int, activeSlot + 1,     
							//!< mr_funcall, _T("copy"), 1, 0, mr_reftarg, oldm);</pre>
							//!< Will emit : <pre>meditmaterials[2] = (copy $.material)</pre> 
		mr_varname,			//!< A variable name. Follow the tag with a string (TCHAR*). (MAXScript type). Example: <pre>_T("baz")</pre>
		mr_create,			//!< A constructor call for the class. Follow the tag with: Class_ID, SClass_ID, int, \<args\>. The int parameter is the number of keyword args 
							//!< following, these are specified in pairs as TCHAR*, \<arg\>. (MAX SDK type). Example: <pre>mr_create, Class_ID(123,456), SClass_ID(12345), 0 ...</pre>
		mr_angle,			//!< A floating point value with a ParamDimension of a stdAngleDim. Value is specified as radians, and emitted as degrees.
							//!< Follow the tag with a double (MAX SDK type). Example: <pre>DegToRad(90)</pre> 
		mr_percent, 		//!< A floating point value with a ParamDimension of stdPercentDim. Value is specified as a fraction (0-1), emitted in percent (0-100).
							//!< Follow the tag with a double. (MAX SDK type). Example: <pre>0.1</pre> 
		mr_index, 			//!< An index. Follow the tag with: \<op_arg\>, \<index_arg\>. \<op_arg\> is an \<arg\> specifying the operand to be indexed and \<index_arg\> 
							//!< is an \<arg\> specifying the index value. The value passed is the 0-based index, but the value emitted is 1-based (specified value +1).
							//!< (MAXScript type). 
							//!< <pre>macroRecorder->Assign(mr_index, mr_varname, _T("meditMaterials"), mr_index, activeSlot,     
							//!< mr_funcall, _T("copy"), 1, 0, mr_reftarg, oldm);</pre>
		mr_prop,			//!< A property name. Follow the tag with TCHAR*, \<arg\>. The TCHAR* is the property name, the \<arg\> specifies the item being accessed. 
							//!< (MAXScript type). Example: <pre>mr_prop, _T("PropertyName"), \<object being accessed\> ...</pre>
							//!< See maxsdk\\samples\\materials\\checker.cpp for an example.
							/** A name. Follow the tag with a string (TCHAR *).   The string is emitted preceeded by a "#" to form a
								MAXScript name value.  (MAXScript type).
								Example: <pre>_T("Benson")</pre>  Emits: <pre>"#Benson"</pre>
								Note: A name in MAXScript is a simple symbolic value, often used instead of numbers to denote options in some function call. For example, 
								when setting the motion blur of a scene node, one can say: 
								\code $foo.motionBlur = #image (or #none, or #object) \endcode
								generated, perhaps by: 
								\code macroRecorder->SetSelProperty(_T("motionBlur"), mr_name, blurname); \endcode
								or when choosing g-buffer channels in the render() function, one would say: 
								\code bm = render camera:$c1 channels:#(#zdepth, #coverage, #objectID) \endcode
							*/
		mr_name, 			
		mr_dimfloat,		//!< A floating point value with the specified ParamDimension. Follow the tag with: ParamDimension*, double.  
							//!< The emitted value is the specified value after passing through the ParamDimension* (MAX SDK type).
		mr_dimpoint3,		//!< A Point3 value with the specified ParamDimension. Follow the tag with: ParamDimension*, Point3* (MAX SDK type)
							//!< Example: <pre>ParamDimension dim;Point3 point;</pre>
							//!< referenced by pointer:  <pre>mr_dimpoint3, &dim, &point ...</pre>
		mr_point2, 			//!< A Point2. Follow the tag with a Point2* (MAX SDK type). Example: <pre>Point2 point (10,0);</pre>, 
							//!< referenced by pointer:  <pre>mr_point2, &point,...</pre>
	};
//!@}
/** A utility macro for accessing the global instance of the macro recorder. */
#define macroRec (GetCOREInterface() ? GetCOREInterface()->GetMacroRecorder() : nullptr)

/** Class for easily & safely disabling the macro recorder.
 * This class disables the macro recorder in its constructor and re-enables it
 * in its destructor. It is therefore a fool-proof (and exception safe) way of
 * temporarily disabling the macro recorder
 */
class MacroRecorder::MacroRecorderDisable : public MaxHeapOperators {
public:
	/** Constructor. Enables the macro recorder automatically. */
	MacroRecorderDisable() {
		MacroRecorder* recorder = macroRec;
		if(recorder != NULL)
			recorder->Disable();
	}

	/** Destructor. Disables the macro recorder automatically. */
	~MacroRecorderDisable() {
		MacroRecorder* recorder = macroRec;
		if(recorder != NULL)
			recorder->Enable();
	}
};


/** \internal This global function is used internally to convert from a command id
to the corresponding maxscript command name. */
ScripterExport const MCHAR* command_name_from_code(int com);

