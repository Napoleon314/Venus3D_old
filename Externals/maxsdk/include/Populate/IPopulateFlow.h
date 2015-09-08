/**********************************************************************
//
// Copyright 2014 Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
**********************************************************************/

#pragma once

#include "../iFnPub.h"
#include "../Point3.h"
#include "../object.h"

//! \brief The class ID for the IPopulateFlow class
#define POPULATE_FLOW_CLASS_ID	Class_ID(0x9c973e7, 0x244a5e1c)
//! \brief The interface ID for the IPopulateFlow class
#define POPFLOW_INTERFACE Interface_ID(0x12577496, 0x28c67c63)

namespace MaxSDK 
{
	namespace Populate 
	{
/** IPopulateFlow is the interface class to Populate flow objects. 
\par Example
The following code creates a Populate flow, sets some if its parameters, and adds it to Populate.
\snippet howto/Populate/PopulateExample.cpp Create Populate Flow
\sa IPopulate, IPopulateIdleArea, IPopulateSeat
*/
		class IPopulateFlow : public FPMixinInterface
		{
		  public:
	  
			//! \brief Lane Weave Types
			enum LaneWeaveType
			{
				//! \brief Single direction forward
				eForward = 1, 
				//! \brief Lanes on left forward, lanes on right backward
				eHugLeft, 
				//! \brief Lanes alternate between forward and backward
				eWeaveLeft, 
				//! \brief Lanes alternate between backward and forward
				eWeaveRight, 
				//! \brief Lanes on right forward, lanes on left backward
				eHugRight, 
				//! \brief Single direction backward
				eBackward
			};

			//! \name Flow Parameters
			//@{ 
			//! \brief Returns the path width
			virtual float GetPathWidth() const = 0;
			//! \brief Sets the path width
			//! \param[in] width - the path width
			virtual bool  SetPathWidth(float width) = 0;
			//! \brief Returns the lane width
			virtual float GetLaneWidth() const = 0;
			//! \brief Sets the lane width
			/*! If the width is too large or small, it will be clipped to the allowable limits. */
			//! \param[in] width - the lane width
			//! \returns true if the width was successfully set, false if not
			virtual bool SetLaneWidth(float width) = 0;
			//! \brief Returns whether or not the portals are unified (or linked)
			/*! If the portals are unified, portal 1 parameters apply to both portals, and portal 2 parameters are ignored.
			If the portals are not unified, portal 1 parameters apply to portal 1, and portal 2 parameters apply to portal 2.*/
			virtual bool GetPortalUnified() const = 0;
			//! \brief Sets whether or not the portals are unified (or linked)
			/*! If the portals are unified, portal 1 parameters apply to both portals, and portal 2 parameters are ignored.
			If the portals are not unified, portal 1 parameters apply to portal 1, and portal 2 parameters apply to portal 2.*/
			//! \param[in] unified - whether or not the portals are unified
			virtual void SetPortalUnified(bool unified) = 0;
			//! \brief Returns the lane weave type
			//! \see IPopulateFlow::LaneWeaveType
			virtual LaneWeaveType GetLaneWeave() const = 0;
			//! \brief Sets the lane weave type
			//! \param[in] weave - the lane weave type
			//! \see IPopulateFlow::LaneWeaveType
			//! \returns true if the weave was successfully set, false if not
			virtual bool SetLaneWeave(LaneWeaveType weave) = 0;
			//! \brief Returns the position randomization seed
			virtual int GetPositionSeed() const = 0;
			//! \brief Sets the position randomization seed
			//! \param[in] seed - the position seed
			//! \returns true if the seed was successfully set, false if not
			virtual bool SetPositionSeed(int seed) = 0;
			//! \brief Returns the gender randomization seed
			virtual int GetGenderSeed() const = 0;
			//! \brief Sets the gender randomization seed
			//! \param[in] seed - the gender seed
			//! \returns true if the seed was successfully set, false if not
			virtual bool SetGenderSeed(int seed) = 0;
			//@}

			//! \name Portal 1 Parameters
			//! If the portals are unified, portal 1 parameters apply to both portals, and if not, portal 1 parameters apply only to portal 1.
			//@{ 
			//! \brief Returns the flow density at the specified time
			/*! Density is a value between 0.0 and 1.0, 0 meaning the portal releases no characters, 
			and 1 meaning the portal releases the maximum allowable number of characters */
			//! \param[in] t - time at which the density is retrieved
			virtual float GetDensity(TimeValue t) const = 0;
			//! \brief Sets the flow density at the specified time
			/*! Density is a value between 0.0 and 1.0, 0 meaning the portal releases no characters, 
			and 1 meaning the portal releases the maximum allowable number of characters */
			//! \param[in] t - time at which the density is set
			//! \param[in] density - density of the flow at time t
			//! \returns true if the density was successfully set, false if not
			virtual bool SetDensity(TimeValue t, float density) = 0;

			//! \brief Returns the flow speed at the specified time
			/*! Speed is a value between 0.0 and 1.0, 0 meaning the portal releases slow pedestrians,
			and 1 meaning the portal releases fast pedestrians. */
			//! \param[in] t - time at which the speed is retrieved
			virtual float GetSpeed(TimeValue t) const = 0;
			//! \brief Sets the flow speed at the specified time
			/*! Speed is a value between 0.0 and 1.0, 0 meaning the portal releases slow pedestrians,
			and 1 meaning the portal releases fast pedestrians. */
			//! \param[in] t - time at which the speed is set
			//! \param[in] speed - speed of the pedestrians at time t
			//! \returns true if the speed was successfully set, false if not
			virtual bool SetSpeed(TimeValue t, float speed) = 0;
	
			//! \brief Returns the percent of runners at the specified time
			/*! Running percent is a value between 0.0 and 1.0, 0 meaning the portal releases no running pedestrians,
			and 1 meaning the portal releases all running pedestrians. */
			//! \param[in] t - time at which the running percent is retrieved
			virtual float GetRunningPercent(TimeValue t) const = 0;
			//! \brief Sets the percent of runners at the specified time
			/*! Running percent is a value between 0.0 and 1.0, 0 meaning the portal releases no running pedestrians,
			and 1 meaning the portal releases all running pedestrians. */
			//! \param[in] t - time at which the running percent is set
			//! \param[in] runPercent - percent of running pedestrians at time t
			//! \returns true if the run percent was successfully set, false if not
			virtual bool SetRunningPercent(TimeValue t, float runPercent) = 0;
	
			//! \brief Returns the percent of females at the specified time
			/*! Gender percent is a value between 0.0 and 1.0, 0 meaning the portal releases all males,
			and 1 meaning the portal releases all females. */
			//! \param[in] t - time at which the gender percent is retrieved
			virtual float GetGenderPercent(TimeValue t) const = 0;
			//! \brief Sets the percent of females at the specified time
			/*! Gender percent is a value between 0.0 and 1.0, 0 meaning the portal releases all males,
			and 1 meaning the portal releases all females. */
			//! \param[in] t - time at which the gender percent is set
			//! \param[in] genderPercent - percent of females at time t
			//! \returns true if the gender percent was successfully set, false if not
			virtual bool SetGenderPercent(TimeValue t, float genderPercent) = 0;
			//@}

			//! \name Portal 2 Parameters
			//! If the portals are unified, portal 2 parameters are ignored, and if not, portal 2 parameters apply only to portal 2.
			//@{ 
			//! \brief Returns the flow density of portal 2 at the specified time
			/*! Density is a value between 0.0 and 1.0, 0 meaning the portal releases no characters, 
			and 1 meaning the portal releases the maximum allowable number of characters */
			//! \param[in] t - time at which the density is retrieved
			virtual float GetDensity2(TimeValue t) const = 0;
			//! \brief Sets the flow density of portal 2 at the specified time
			/*! Density is a value between 0.0 and 1.0, 0 meaning the portal releases no characters, 
			and 1 meaning the portal releases the maximum allowable number of characters */
			//! \param[in] t - time at which the density is set
			//! \param[in] density - density of the flow at time t
			//! \returns true if the density was successfully set, false if not
			virtual bool SetDensity2(TimeValue t, float density) = 0;
	
			//! \brief Returns the flow speed of portal 2 at the specified time
			/*! Speed is a value between 0.0 and 1.0, 0 meaning the portal releases slow pedestrians,
			and 1 meaning the portal releases fast pedestrians. */
			//! \param[in] t - time at which the speed is retrieved
			virtual float GetSpeed2(TimeValue t) const = 0;
			//! \brief Sets the flow speed of portal 2 at the specified time
			/*! Speed is a value between 0.0 and 1.0, 0 meaning the portal releases slow pedestrians,
			and 1 meaning the portal releases fast pedestrians. */
			//! \param[in] t - time at which the speed is set
			//! \param[in] speed - speed of the pedestrians at time t
			//! \returns true if the speed was successfully set, false if not
			virtual bool SetSpeed2(TimeValue t, float speed) = 0;
	
			//! \brief Returns the percent of runners released from portal 2 at the specified time
			/*! Running percent is a value between 0.0 and 1.0, 0 meaning the portal releases slow pedestrians,
			and 1 meaning the portal releases fast pedestrians. */
			//! \param[in] t - time at which the running percent is retrieved
			virtual float GetRunningPercent2(TimeValue t) const = 0;
			//! \brief Sets the percent of runners released from portal 2 at the specified time
			/*! Running percent is a value between 0.0 and 1.0, 0 meaning the portal releases slow pedestrians,
			and 1 meaning the portal releases fast pedestrians. */
			//! \param[in] t - time at which the running percent is set
			//! \param[in] runPercent - percent of running pedestrians at time t
			//! \returns true if the run percent was successfully set, false if not
			virtual bool SetRunningPercent2(TimeValue t, float runPercent) = 0;
	
			//! \brief Returns the percent of females released from portal 2 at the specified time
			/*! Gender percent is a value between 0.0 and 1.0, 0 meaning the portal releases all males,
			and 1 meaning the portal releases all females. */
			//! \param[in] t - time at which the gender percent is retrieved
			virtual float GetGenderPercent2(TimeValue t) const = 0;
			//! \brief Sets the percent of females released from portal 2 at the specified time
			/*! Gender percent is a value between 0.0 and 1.0, 0 meaning the portal releases all males,
			and 1 meaning the portal releases all females. */
			//! \param[in] t - time at which the gender percent is set
			//! \param[in] genderPercent - percent of females at time t
			//! \returns true if the gender percent was successfully set, false if not
			virtual bool SetGenderPercent2(TimeValue t, float genderPercent) = 0;
			//@}
	
			//! \name Geometry Querying and Editing
			//! To edit the geometry of a flow, simply edit the points which make up the center line of the flow.
			//! The sides are created based on the center line of points and the width of the flow.   
			//! If the points cannot generate a valid flow, then the flow is marked unusable.
			//! An unusable flow is displayed without lanes or marks, and with orange edges in the viewport, and no simulation will take place on it.
			//! If executed inside a hold, all functions which change the geometry can be undone.
			//! \see ComputeSides(), IsPathUsable()
			//@{ 
			//! \brief Returns the number of points in the center line of the flow
			virtual int GetNumPoints() const = 0;
			//! \brief Returns the specified point
			//! \param[in] i - the index of the retrieved point
			virtual Point3 GetPoint(int i) const = 0;
			//! \brief Returns true if a valid flow will result from the point being appended to the flow and false if not.
			//! \param[in] pt - the point to be potentially added
			virtual bool CanAddPoint(Point3 pt) = 0;
			//! \brief Appends a point to the flow
			//! \param[in] pt - the point to be added
			//! \returns true if the point was successfully added, false if not
			virtual void AddPoint(Point3 pt) = 0;
			//! \brief Returns true if a valid flow will result from the point being inserted at the specified index, and false if not.
			/*! index == 0 will insert at the start of the flow, and index == numpts will append past the end of the path. */
			//! \param[in] index - the index at which the point will be inserted
			//! \param[in] pt - the point to be potentially inserted
			virtual bool CanInsertPoint(int index, Point3 pt) = 0;
			//! \brief Inserts a point in the flow
			/*! index == 0 will insert at the start of the flow, and index == numpts will append past the end of the path. */
			//! \param[in] index - the index at which the point will be inserted
			//! \param[in] pt - the point to be inserted
			//! \returns true if the point was successfully inserted, false if not
			virtual bool InsertPoint(int index, Point3 pt) = 0;
			//! \brief Sets a point in the flow
			/*! A point at the start or end of a ramp cannot be set with this method. */
			//! \see SetCreasePercent()
			//! \param[in] index - the index of the point to be set
			//! \param[in] pt - the point to be set
			//! \returns true if the point was successfully set, false if not
			virtual bool SetPoint(int index, Point3 pt) = 0;
			//! \brief Deletes a point in the flow
			/*! Points at ramp creases cannot be deleted with this function. */ 
			//! \see DeleteRamp()
			//! \param[in] index - the index of the point to be deleted
			//! \returns true if the point was successfully deleted, false if not
			virtual bool DeletePoint(int index) = 0;
			//! \brief Selects or deselects a point in the flow
			//! \param[in] index - the index of the point to be selected
			//! \param[in] selected - the value to set the point's selected property to.  Set to true to select,
			//! false to de-select.
			//! \returns true if the operation was successful
			virtual bool SetPointSelected(int index, bool selected) = 0;
			//@}

			/*! \name Ramps
			A ramp consists of a single inclined or flat segment in a flow surrounded by two landing segments.
			The points at the start and end of a ramp are called creases. 
			The points of the ramp creases can not be set directly.  Instead, they must be set with the crease percent.
			The crease percent indicates how far a ramp crease is between the ends of the surrounding landings.  
			It is a value between 0.0 and 1.0.  A value of 0.0 indicates that the crease is just slightly past the start of the landing before the ramp.
			A value of 1.0 indicates that the crease is just slightly before the end of the last landing segment.
			The crease percent at the start of the ramp must be less than the crease percent at the end of the ramp. */
			//@{ 
			//! \brief Creates a ramp
			/*! The ramp will be created flat inside the segment, adding two new points inside the specified segment,
			with the ramp in between the two new points and the landing segments on either side. 
			If it's impossible to create a ramp in the specified segment, it will not be created. */
			//! \param[in] index - the index of the segment in which the ramp will be created
			//! \returns true if the ramp was successfully created, false if it is not possible to create a ramp in this segment
			virtual bool CreateRamp(int index) = 0;
			//! \brief Deletes a ramp
			/*! The ramp whose starting crease is at the specified index will be deleted.  Both ramp crease points will be deleted.  
			If the resulting flow is on two levels, it will be broken up into two flows. */
			//! \param[in] index - the index of the starting crease of the ramp to be deleted
			//! \returns true if the ramp was successfully deleted, false if it is not possible to delete the ramp
			virtual bool DeleteRamp(int index) = 0;
			//! \brief Returns whether or not the specified index is at the starting crease of a ramp
			//! \param[in] index - the index to check
			virtual bool IsRamp(int index) = 0;
			//! \brief Returns whether or not the specified index is at the start of a ramp landing segment
			/*! A ramp landing is the segment before or after a ramp. */
			//! \param[in] index - the index to check
			virtual bool IsLanding(int index) = 0;
			//! \brief Returns the crease percent
			/*! The crease percent is a value between 0.0 and 1.0, indicating how far the crease is between the start of the first landing and the end of the second landing. */
			//! \param[in] index - the index of the crease
			virtual float GetCreasePercent(int index) const = 0;
			//! \brief Sets the crease percent
			/*! The crease percent is a value between 0.0 and 1.0, indicating how far the crease is between the start of the first landing and the end of the second landing.  
			The first crease percent must have a lower value than the second crease percent. */
			//! \param[in] index - the index of the crease
			//! \param[in] percent - a value between 0.0 and 1.0
			//! \returns true if the crease percent was successfully set, false if it was not possible to set the percent to the specified value
			virtual bool SetCreasePercent(int index, float percent) = 0;
			//! \brief Sets points to be creases
			/*! For internal use only.  This is very dangerous because it doesn't check ramp validity when it creates creases. */
			virtual void SetRamp(int index) = 0;
			//@}
	
			//! \name Miscellaneous Functions
			//@{ 
			//! \brief Computes the sides of the flow
			/*! When you add, insert, set, or delete points, or add, delete or change a ramp, an internal flag is set 
			indicating that the flow has changed and the sides need to be recomputed.  You can call this function to recompute them, 
			or they will be recomputed when the flow is displayed.  Computation causes the internal flag to be reset. */
			//! \param[in] forceComputation - if true, the sides will be computed, if false, the sides will only be computed if the flow has changed since the last side computation
			//! \returns true if the flow is usable, false if not
			virtual bool ComputeSides(bool forceComputation = true) = 0;
			//! \brief Returns whether or not the flow is usable
			//! \brief An unusable flow displays without lanes or marks, and with orange edges in the viewport, and no simulation will take place on it.
			virtual bool IsPathUsable() = 0;
			//! \brief Returns the default path width
			virtual float GetDefaultPathWidth() const = 0;
			//! \brief Returns the color of the edges of an unusable flow
			virtual Color GetInvalidPathColor() const = 0;
			//! \brief Temporarily stores the flow points
			/*! You can store the points before changing them, check the flow validity, and if they are invalid, restore them.
			However, since the functions which edit the flow are undoable, this function is not terribly useful and is only used internally in special cases. */
			//! \see RestoreLastKnownGoodPoints(), IsPathUsable()
			virtual void StoreLastKnownGoodPoints() = 0;
			//! \brief Restores the temporarily stored points
			/*! You can store the points before changing them, check the flow validity, and if they are invalid, restore them.
			However, since the functions which edit the flow are undoable, this function is not terribly useful and is only used internally in special cases. */
			//! \see StoreLastKnownGoodPoints(), IsPathUsable()
			virtual void RestoreLastKnownGoodPoints() = 0;
			//@}

			// Function publishing
			FPInterfaceDesc* GetDesc();
			virtual Interface_ID GetID() { return POPFLOW_INTERFACE; }
	
			//! \brief Gets the interface to a Populate Flow object
			//! \param[in] obj - Populate flow object
			//! \return A pointer to an IPopulateFlow interface, or NULL.
			static inline IPopulateFlow* GetPopulateFlowInterface(Object* obj) { 
				return obj? static_cast<IPopulateFlow*>(obj->GetInterface(POPFLOW_INTERFACE)): NULL;}
		};
	} // namespace Populate
} // namespace MaxSDK

