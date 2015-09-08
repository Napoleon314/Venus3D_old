//**************************************************************************/
// Copyright (c) 1998-2006 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
/*! \file ilayer.h
	\brief Declaration of the ILayer Interface.
*/

#pragma once
#include "maxheap.h"
#include "maxtypes.h"
#include "ref.h"
// forward declarations
class View;
class Point3;
class INode;

//! The index into an INode's references that point to an ILayer ReferenceTarget.
#define NODE_LAYER_REF		6

/*! \brief Interface to the layers functionality.
	\par Description: 
	Layers govern (override) some properties of the nodes which are on the layer. 
	For example, a Layer could be used to freeze all the nodes on it without having to set this 
	property of each node individually.\n\n
	Some of the methods below are not functional in 3ds Max. Such cases are noted
	in the remarks for the method. 
	\sa  Class ReferenceTarget, Class ILayerManager, Class INode. */
class ILayer : public ReferenceTarget
{
public:
	//! The super class ID of the layer interface class.
	static const SClass_ID kLayerSuperClassID;

	// from Animatable
	virtual SClass_ID SuperClassID() { return kLayerSuperClassID; }


	//! \name Layer Information
	//@{
	/*! \remarks Sets the name of this layer.
		\param name - The name for this layer. */
	virtual void SetName(const MSTR & name) = 0;
	/*! \remarks Returns the name of this layer.  */
	virtual MSTR GetName() const = 0;
	/*! \remarks Sets the wire frame color.
		\param newcol - See <a href="idx__r_colorref_html.html">COLORREF</a>.
		\param propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void SetWireColor(DWORD newcol, bool propagate = false) = 0;
	/*! \remarks Returns the wire frame color. See
	<a href="idx__r_colorref_html.html">COLORREF</a>. */
	virtual DWORD GetWireColor() const = 0;
	//@}


	//! \name Interactivity Properties
	//@{
	/*! \remarks Sets the hidden state.
		\param onOff - Use true for hidden; false for not hidden. 
		\param  propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void Hide(bool onOff, bool propagate = false) = 0;
	/*! \remarks Returns true if hidden; false if not hidden. */
	virtual bool IsHidden() const = 0;
	/*! \remarks Sets the frozen state.
		\param  onOff - Use true for on; false for off. 
		\param  propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void Freeze(bool onOff, bool propagate = false) = 0;
	/*! \remarks Returns true if frozen; false if not. */
	virtual bool IsFrozen() const = 0;
	//@}


	//! \name Display Properties
	//@{
	/*! \remarks Sets the See-through material property.
		\param  onOff - Use true for on; false for off.
		\param  propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void XRayMtl(bool onOff, bool propagate = false) = 0;
	/*! \remarks Returns true if See-through material is set; false if not. */
	virtual bool HasXRayMtl() const = 0;
	/*! \remarks Sets the box mode state.
		\param  onOff - Use true for on; false for off.
		\param  propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void BoxMode(bool onOff, bool propagate = false) = 0;
	/*! \remarks Returns true if box mode is on; false if off. */
	virtual bool GetBoxMode() const = 0;
	/*! \remarks Sets the backface culling state.
		\param  onOff - Use true for on; false for off.
		\param  propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void BackCull(bool onOff, bool propagate = false) = 0;
	/*! \remarks Returns true if backface culling is on; false if not. */
	virtual bool GetBackCull() const = 0;
	/*! \remarks Sets the all edges setting.
		\param  onOff - Use true for on; false for off.
		\param  propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void AllEdges(bool onOff, bool propagate = false) = 0;
	/*! \remarks Returns true if all edges is on; false if off. */
	virtual bool GetAllEdges() const = 0;
	/*! \remarks Sets the vertex ticks state.
		\param  onOff - Use true for on; false for off.
		\param  propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void VertTicks(bool onOff, bool propagate = false) = 0;
	/*! \remarks Returns true if vertex ticks is on; false if off. */
	virtual bool GetVertTicks() const = 0;
	/*! \remarks Sets the Trajectory flag state
		\param  onOff - Use true for on; false for off.
		\param  propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void Trajectory(bool onOff, bool propagate = false) = 0;
	/*! \remarks Returns true if the display trajectory flag for the layer is set. */
	virtual bool GetTrajectory() const = 0;
	/*! \remarks Sets the ignore extents property.
		\param onOff - Use true for on; false for off.
		\param propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void IgnoreExtents(bool onOff, bool propagate = false) = 0;
	/*! \remarks Returns true if ignore extents is on; false if off. */
	virtual bool GetIgnoreExtents() const = 0;
	/*! \remarks Sets the Show Frozen with Material property.
		\param onOff - Use true for on; false for off.
		\param propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void SetShowFrozenWithMtl( bool onOff, bool propagate = false) = 0;
	/*! \remarks Gets whether the objects on the layer are frozen in gray. */
	virtual bool ShowFrozenWithMtl() const = 0;
	/*! \remarks Sets the color per vertex display mode.
		\param  onOff - Use true for on; false for off.
		\param  propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void SetCVertMode(bool onOff, bool propagate = false) = 0;
	/*! \remarks Returns true if the color vertex display mode is on;
		otherwise false. */
	virtual bool GetCVertMode() const = 0;
	/*! \remarks Sets the shaded color vertex display mode.
		\param  onOff - Use true for on; false for off. */
	virtual void SetShadeCVerts(bool onOff) = 0;
	/*! \remarks Returns true if the shaded color vertex mode is on; false if off. */
	virtual bool GetShadeCVerts() const = 0;
	//@}


	//! \name Rendering Properties
	//@{
	/*! \remarks Sets the visibility at the specified time
		\param t - The time at which to set the value
		\param vis - The new value for visibility */
	virtual void SetVisibility(TimeValue t, float vis) = 0;
	/*! \remarks Gets the visibility at the specified time
		\param t - Unused. 
		\param valid - Unused. */
	virtual float GetVisibility(TimeValue t, Interval* valid=NULL) const = 0;
	/*! \remarks Gets the visibility at the specified time and viewport.
		Identical to GetVisibility. 
		\param t - Unused.
		\param view - Unused. 
		\param valid - Unused. */
	virtual float GetVisibility(TimeValue t, View& view, Interval* valid=NULL) const = 0;
	/*! \remarks Identical to GetVisibility. All params unused. */
	virtual float GetLocalVisibility(TimeValue t,Interval *valid) const = 0;
	/*! \remarks Always returns false */
	virtual bool IsHiddenByVisControl() = 0;
	/*! \remarks Sets the renderable state.
		\param  onOff - Use true for renderable; false for not renderable. 
		\param  propagate - Use true to pass the value to child layers, false to set just this one layer */
	virtual void SetRenderable(bool onOff, bool propagate = false) = 0;
	//! \remarks Returns true if renderable; false if not.
	virtual bool Renderable() const = 0;
	/*! \remarks This method allows you to set the inherit visibility flag for the layer.
		\param  onOff - TRUE to set; FALSE to unset. */
	virtual void SetInheritVisibility(bool onOff) = 0;
	/*! \remarks This method returns TRUE if the inherit visibility flag for the layer is set. */
	virtual bool GetInheritVisibility() const = 0;
	/*! \remarks This method allows you to set or unset the primary visibility flag for the layer.
		Otherwise known as Visible to Camera.
		\param onOff - TRUE to set; FALSE to unset. */
	virtual void SetPrimaryVisibility(bool onOff) = 0;
	/*! \remarks This method returns TRUE if the primary visibility flag for the layer is set. 
		Otherwise known as Visible to Camera. */
	virtual bool GetPrimaryVisibility() const = 0;
	/*! \remarks This method allows you to set the secondary visibility flag for the layer.
		Otherwise known as Visible to Reflection and Refraction.
		\param  onOff - TRUE to set; FALSE to unset. */
	virtual void SetSecondaryVisibility(bool onOff) = 0;
	/*! \remarks This method returns TRUE if the secondary visibility flag for the layer is set. 
		Otherwise known as Visible to Reflection and Refraction. */
	virtual bool GetSecondaryVisibility() const = 0;
	/*! \remarks Sets the cast shadow state.
		\param  onOff - Use true for on; false for off. */
	virtual void SetCastShadows(bool onOff) = 0;
	/*! \remarks Returns true if cast shadow is on; false if off. */
	virtual bool CastShadows() const = 0;
	/*! \remarks Sets the receives shadow state.
		\param  onOff - Use true for on; false for off. */
	virtual void SetRcvShadows(bool onOff) = 0;
	/*! \remarks Returns true if receives shadow is on; false if off. */
	virtual bool RcvShadows() const = 0;
	/*! \remarks When on, atmospheric effects are applied to the layer. 
		When off atmospheric effects do not change the rendered appearance of this layer.
		\param  onOff - Use true for on; false for off. */
	virtual void SetApplyAtmospherics(bool onOff) = 0;
	/*! \remarks Returns true if Atmospherics apply to this layer, false if not. */
	virtual bool ApplyAtmospherics() const = 0;
	/*! \remarks Allows special effects to affect objects in the scene that are occluded 
		by this layer.
		\param  onOff - Use true for on; false for off. */
	virtual void SetRenderOccluded(bool onOff) = 0;
	/*! \remarks Returns true if Render Occluded is on; false if off. */
	virtual int	GetRenderOccluded() const = 0;
	//@}


	//! \name Motion Blur Properties
	//@{
	/*! \remarks This method returns the state of the motion blur flag at the specified time.
		\param t - The time at which to get the flag. */
	virtual bool GetMotBlurOnOff(TimeValue t) const = 0;
	/*! \remarks This method allows you to set the state of the motion blur flag at the specified time.
		\param t - The time at which to set the flag.
		\param onOff - TRUE to turn on; FALSE to turn off. */
	virtual void  SetMotBlurOnOff(TimeValue t, bool onOff) = 0;
	/*! \remarks This method allows you to set the motion blur multiplier for the layer.
		\param t - The timevalue to get the motion blur multiplier for. */
	virtual float GetImageBlurMultiplier(TimeValue t) const = 0;
	/*! \remarks This method allows you to set the image blur multiplier.
		\param t - The timevalue to set the image blur multiplier for.
		\param m - The multiplier to set. */
	virtual void  SetImageBlurMultiplier(TimeValue t, float m) = 0;
	/*! \remarks Sets the type of motion blur used by the layer.
		\par Parameters:
		<b>int kind</b>\n\n
		The kind of motion blur. One of the following values:\n\n
		<b>0</b>: None.\n\n
		<b>1</b>: Object Motion Blur.\n\n
		<b>2</b>: Image Motion Blur. */
	virtual void SetMotBlur(int kind) = 0;
	/*! \remarks Returns the type of motion blur used by the layer. One of the
		following values:\n\n
		<b>0</b>: None.\n\n
		<b>1</b>: Object Motion Blur.\n\n
		<b>2</b>: Image Motion Blur. */
	virtual int MotBlur() const = 0;
	//@}


	//! \name By Layer Methods
	//@{
	/*! \remarks This method allows you to set the display by layer flag for the specified node.
		\param  onOff - TRUE to set; FALSE to unset.
		\param node - The pointer to the node. */
	virtual void SetDisplayByLayer(BOOL onOff, INode* node ) = 0;
	/*! \remarks This method allows you to set the render by layer flag for the specified node.
		\param  onOff - TRUE to set; FALSE to unset.
		\param node - The pointer to the node. */
	virtual void SetRenderByLayer(BOOL onOff, INode* node) = 0;
	/*! \remarks This method allows you to set the motion by layer flag for the specified node.
		\param  onOff - TRUE to set; FALSE to unset.
		\param node - The pointer to the node. */
	virtual void SetMotionByLayer(BOOL onOff, INode* node) = 0;
	/*! \remarks This method returns the state of the display by layer flag for the specified node.
		\param node - The pointer to the node.
		\return - TRUE if set; FALSE if not set. */
	virtual BOOL GetDisplayByLayer(INode* node) const = 0;
	/*! \remarks This method returns the state of the render by layer flag for the specified node.
		\param node - The pointer to the node.
		\return - TRUE if set; FALSE if not set. */
	virtual BOOL GetRenderByLayer(INode* node) const = 0;
	/*! \remarks This method returns the state of the motion by layer flag for the specified node.
		\param node - The pointer to the node.
		\return  TRUE if set; FALSE if not set. */
	virtual BOOL GetMotionByLayer(INode* node) const = 0;
	//@}

	
	//! \name INode methods
	//@{
	/*! \remarks Adds the specified node to this layer.
		\param node - The node to add. */
	virtual bool AddToLayer(INode* node) = 0;
	/*! \remarks Deletes the specified node from this layer. 
		Note: This method does nothing in 3ds Max.
		\param rtarg - The node to delete from this layer. */
	virtual bool DeleteFromLayer(INode* rtarg) = 0;
	/*! \remarks Returns true if the layer is used (nodes have been added); otherwise false. */
	virtual bool Used(void) const = 0;
	/*! \remarks Returns true if the layer is used (nodes have been added); otherwise false. */
	virtual bool HasObjects() const = 0;
		/*! \remarks This method will select the objects of the layer. */
	virtual void SelectObjects(void) = 0;
	//@}


	//! \name Hierarchy methods
	//@{
	/*! \remarks Returns the parent Layer. This could be NULL meaning the layer is at the top level.*/
	virtual ILayer* GetParentLayer() const = 0; 
	/*! \remarks Sets the parent layer for this layer.
		\param parentLayer - The layer that will become the parent layer.
		\return  Returns TRUE if set; FALSE if not set.
		There are some cases where you cannot set, for example, you cannot set yourself as your parent. 
		Another example is you cannot set one of your childs as your parent (no cycles).*/
	virtual bool SetParentLayer(ILayer* parentLayer) = 0;
	/*! \remarks Returns a pointer to the Nth child Layer for this layer. 
		\param n - The index of the child layer.
		\return  Returns a pointer to the Nth child layer. */
	virtual ILayer* GetChildLayer(int n) const = 0;
	/*! \remarks Returns the number of child layers in this layer.
		\return  The number of child layers in this layer. */
	virtual int GetNumOfChildLayers() const = 0;
	//@}


	//! \name Internal Flags and Unused Properties
	//! \internal
	//@{
	//! \internal
	virtual int GetRenderFlags() const = 0;
	//! \internal
	virtual void SetRenderFlags(int flags) = 0;
	//! \internal
	virtual int GetDisplayFlags() const = 0;
	//! \internal
	virtual bool GetFlag(int mask) const = 0;
	//! \internal
	virtual bool GetFlag2(int mask) const = 0;
	/*! \remarks This method is for internal use in VIZ. Note: This method does nothing in 3ds Max. 
		\internal */
	virtual void UpdateSelectionSet(void) = 0;
	/*! \remarks This method returns the render flags associated with the layer.
		\internal
		\param oldlimits - The old limits flag. */
	virtual int GetRenderFlags(int oldlimits) const = 0;
	//! \endinternal
	//@}
};

//! \brief Extended interface to the layers functionality.  Added for 3dsMax 2016
class ILayerEx : public ILayer
{
public:
	/*! \remarks Returns true if the layer or one if its child layers contains a scene selected node. */
	virtual bool HasSelectedNodesInHierarchy() const = 0;

	/*! \remarks Returns the maximum value node handle of the nodes held by the layer. 
		For example, if the layer has nodes with handles 12, 32, and 101, this method will return 101.
		\param recurse - if true, includes nodes in children layers.
		\return - the maximum value node handle of nodes held by the layer, 0 if no nodes held. */
	virtual ULONG GetMaxNodeHandle(bool recurse) = 0;

	/*! \remarks Returns true if the layer or one if its child layers contains an xref scene node. */
	virtual bool HasSceneXRefNodesInHierarchy() const = 0;
};

/*! This is a struct for NOTIFY_LAYER_PARENT_CHANGED BroadcastNotification
*  layerChanged is the pointer on the layer that has changed parent. 
*  oldParent is the pointer on the old parent of the layer.
*/
struct LayerParentChange: public MaxHeapOperators {
	ILayer *layerChanged;
	ILayer *oldParent;
}; 
