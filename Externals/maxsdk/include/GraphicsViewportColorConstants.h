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
#pragma once

//*! \defgroup viewportDrawingColors Viewport Drawing Color Indices
//@{
#define COLOR_SELECTION				0 //!< Current selection
#define COLOR_SUBSELECTION			1 //!< Sub-object selection
#define COLOR_FREEZE				2 //!< Frozen scene object
#define COLOR_GRID					3 //!< Viewport Grid
#define COLOR_GRID_INTENS			4 //!< Viewport Grid Intensity
#define COLOR_SF_LIVE				5 //!< 
#define COLOR_SF_ACTION				6 //!< 
#define COLOR_SF_TITLE				7 //!< 
#define COLOR_VP_LABELS				8 //!< Viewport menu labels
#define COLOR_VP_INACTIVE			9 //!< Viewport menu labels when inactive
#define COLOR_ARCBALL				10 //!< 
#define COLOR_ARCBALL_HILITE		11 //!< 
#define COLOR_ANIM_BUTTON			12 //!< Animate Button
#define COLOR_SEL_BOXES				13 //!< 
#define COLOR_LINK_LINES			14 //!< Link Lines
#define COLOR_TRAJECTORY			15 //!< 
#define COLOR_ACTIVE_AXIS			16 //!< 
#define COLOR_INACTIVE_AXIS			17 //!< 
#define COLOR_SPACE_WARPS			18 //!< 
#define COLOR_DUMMY_OBJ				19 //!< Dummy  objects
#define COLOR_POINT_OBJ				20 //!< 
#define COLOR_POINT_AXES			21 //!< 
#define COLOR_TAPE_OBJ				22 //!< Tape objects
#define COLOR_BONES					23 //!< Bone objects
#define COLOR_GIZMOS				24 //!< Gizmo objects
#define COLOR_SEL_GIZMOS			25 //!< Gizmos when selected.
#define COLOR_SPLINE_VECS			26 //!< Spline vertex vector handles
#define COLOR_SPLINE_HANDLES		27 //!< Spline vertext handles
#define COLOR_PATCH_LATTICE			28 //!< No longer used
#define COLOR_PARTICLE_EM			29 //!< 
#define COLOR_CAMERA_OBJ			30 //!< Camera objects
#define COLOR_CAMERA_CONE			31 //!< Camera view cone
#define COLOR_CAMERA_HORIZ			32 //!< Camera horizon line
#define COLOR_NEAR_RANGE			33 //!< Camera near range
#define COLOR_FAR_RANGE				34 //!< Camera far range
#define COLOR_LIGHT_OBJ				35 //!< Light object
#define COLOR_TARGET_LINE			36 //!< Light object target line
#define COLOR_HOTSPOT				37 //!< Light object hotspot cone
#define COLOR_FALLOFF				38 //!< Light object falloff cone
#define COLOR_START_RANGE			39 //!< Light object start range
#define COLOR_END_RANGE				40 //!< Light object end range
#define COLOR_VIEWPORT_BKG			41 //!< Viewport Background
#define COLOR_TRAJ_TICS_1			42 //!< Animation Trajectory Ticks one
#define COLOR_TRAJ_TICS_2			43 //!< Animation Trajectory Ticks two
#define COLOR_TRAJ_TICS_3			44 //!< Animation Trajectory Ticks three
#define COLOR_GHOST_BEFORE			45 //!< 
#define COLOR_GHOST_AFTER			46 //!< 
#define COLOR_12FIELD_GRID			47 //!< 
#define COLOR_START_RANGE1			48 //!< 
#define COLOR_END_RANGE1			49 //!< 
#define COLOR_CAMERA_CLIP  			50 //!< 
#define COLOR_NURBS_CV				51 //!< 
#define COLOR_NURBS_LATTICE			52 //!< 
#define COLOR_NURBS_CP				53 //!< 
#define COLOR_NURBS_FP				54 //!< 
#define COLOR_NURBS_DEP				55 //!< 
#define COLOR_NURBS_ERROR			56 //!< 
#define COLOR_NURBS_HILITE			57 //!< 
#define COLOR_NURBS_FUSE			58 //!< 
#define COLOR_END_EFFECTOR			59 //!< 
#define COLOR_END_EFFECTOR_STRING	60 //!< 
#define COLOR_JOINT_LIMIT_SEL		61 //!< 
#define COLOR_JOINT_LIMIT_UNSEL		62 //!< 
#define COLOR_IK_TERMINATOR			63 //!< 
#define COLOR_SF_USER				64 //!< 
#define COLOR_VERT_TICKS			65 //!< 
#define COLOR_XRAY					66 //!< 
#define COLOR_GROUP_OBJ				67 //!< 
#define COLOR_MANIPULATOR_X			68 //!< 
#define COLOR_MANIPULATOR_Y			69 //!< 
#define COLOR_MANIPULATOR_Z			70 //!< 
#define COLOR_MANIPULATOR_ACTIVE	71 //!< 
#define COLOR_VPT_CLIPPING			72 //!< 
#define COLOR_DECAY_RADIUS			73 //!< 
#define COLOR_VERT_NUMBERS			74 //!< 
#define COLOR_CROSSHAIR_CURSOR		75 //!< 

#define COLOR_SV_WINBK              76 //!< Schematic View Window Background
#define COLOR_SV_NODEBK             77 //!< Schematic View Default Node Background
#define COLOR_SV_SELNODEBK          78 //!< Schematic View Selected Node Background
#define COLOR_SV_NODE_HIGHLIGHT     79 //!< Schematic View Viewport Selected Node Highlight
#define COLOR_SV_MATERIAL_HIGHLIGHT 80 //!< Schematic View MEDIT Selected Node Highlight
#define COLOR_SV_MODIFIER_HIGHLIGHT 81 //!< Schematic View Selected Modifier Highlight
#define COLOR_SV_PLUGIN_HIGHLIGHT   82 //!< Schematic View Plug-in Highlight
#define COLOR_SV_SUBANIM_LINE       83 //!< Schematic View Subanim line color
#define COLOR_SV_CHILD_LINE         84 //!< Schematic View Child node line color
#define COLOR_SV_FRAME              85 //!< Schematic View Frame color
#define COLOR_SV_SELTEXT            86 //!< Schematic View Selected Label Color
#define COLOR_SV_TEXT               87 //!< Schematic View Label Color

#define COLOR_UNSEL_TAB				88
#define COLOR_ATMOS_APPARATUS		89	//!< 
#define COLOR_SUBSELECTION_HARD		90  //!< 
#define COLOR_SUBSELECTION_MEDIUM	91  //!< 
#define COLOR_SUBSELECTION_SOFT		92  //!< 

#define COLOR_SV_UNFOLD_BUTTON      93 //!< SV Unfold Button
#define COLOR_SV_GEOMOBJECT_BK      94 //!< Geometry Object Node Background
#define COLOR_SV_LIGHT_BK           95 //!< Light Node Background
#define COLOR_SV_CAMERA_BK          96 //!< Camera Node Background
#define COLOR_SV_SHAPE_BK           97 //!< Shape Node Background
#define COLOR_SV_HELPER_BK          98 //!< Helper Node Background
#define COLOR_SV_SYSTEM_BK          99 //!< System Node Background
#define COLOR_SV_CONTROLLER_BK     100 //!< Controller Node Background
#define COLOR_SV_MODIFIER_BK       101 //!< Modifier Node Background
#define COLOR_SV_MATERIAL_BK       102 //!< Material Node Background
#define COLOR_SV_MAP_BK            103 //!< Map Node Background
#define COLOR_SETKEY_BUTTON        104

#define COLOR_BACK_LINES           105 //!< Backface lines on selected objects
#define COLOR_BACK_VERTS           106 //!< Backface vertices on selected objects

#define COLOR_MANIPULATOR_CONTOUR  107 //!< Background color for the rotation gizmo
#define COLOR_MANIPULATOR_SCREEN   108 //!< screen space manipulator handle color for the rotation gizmo
#define COLOR_MANIPULATOR_TRAIL    109 //!< move gizmo displacement trail color
//@}

const int kColorNormalsUnspecified = 110;
const int kColorNormalsSpecified = 111;
const int kColorNormalsExplicit = 112;

/*! \addtogroup viewportDrawingColors */
//@{
#define COLOR_SV_GRID              113 //!< SV Grid
#define COLOR_SV_REL_INSTANCE      114 //!< SV Relationship Instances
#define COLOR_SV_REL_CONSTRAINT    115 //!< SV Relationship Contraints
#define COLOR_SV_REL_PARAMWIRE     116 //!< SV Relationship Param Wires
#define COLOR_SV_REL_LIGHT         117 //!< SV Relationship Lights
#define COLOR_SV_REL_MODIFIER      118 //!< SV Relationship Modifiers
#define COLOR_SV_REL_CONTROLLER    119 //!< SV Relationship Controllers
#define COLOR_SV_REL_OTHER         120 //!< SV Relationship Others
#define COLOR_SV_SPACEWARP_BK      121 //!< SV SpaceWarp
#define COLOR_SV_BASEOBJECT_BK     122 //!< SV BaseObject

#define COLOR_VP_STATISTICS			123	//!< Colour for viewport statistics display

#define COLOR_SPLINE_KNOT_UNSELECTED		124 //!< color of bezier knots unselected
#define COLOR_SPLINE_KNOT_FIRST					125 //!< color of first knot of the bezier spline
#define COLOR_HIDDENLINE_UNSELECTED			126 //!< color of first knot of the bezier spline
#define COLOR_VP_LABEL_HIGHLIGHT				127 //!< Color of selected/mouse over viewport labels
#define COLOR_SNAP_AXIS_CENTER_HANDLE		128 //!< Color of the inactive axis center handle, its active color is the same as COLOR_MANIPULATOR_ACTIVE
#define COLOR_SNAP_POINT_ACTIVE					129 //!< Color of snap points that are in the snap preview radius
#define COLOR_SNAP_POINT_SNAPPED				130 //!< Color of snapped snap points

#define COLOR_TOTAL                131	//!< always the max number of colors
//@}
