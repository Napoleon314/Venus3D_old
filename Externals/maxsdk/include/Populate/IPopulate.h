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
#include "../GetCOREInterface.h"
// forward declarations
class INode;

//! \brief The interface ID for the IPopulate class
#define POPULATE_INTERFACE Interface_ID(0x50b442e5, 0x31c8552f)

namespace MaxSDK 
{
	namespace Populate 
	{
		/** This is the interface class to Populate. 
		It is a singleton class which is created upon 3dsMax start up. It manages all of the Populate operations.\n\n
		The following code sets some Populate parameters and simulates.  It then selects all the simulated 
		characters and sets them to high resolution.  This code will only do something if Populate objects exist in the scene.
		Code to create Populate objects can be found in the IPopulateFlow, IPopulateSeat and IPopulateIdleArea documentation. 
		
        \par Example
		\snippet howto/Populate/PopulateExample.cpp Create Populate
		
		\sa IPopulateFlow, IPopulateIdleArea, IPopulateSeat
		*/
		class IPopulate : public  FPStaticInterface
		{
		  public:

			//! \brief Figure Display Types  
			enum FigureDisplayType{ 
				//! \brief Stick Figure Characters
				eDisplayStickFigs = 1, 
				//! \brief Segmented Characters
				/** \deprecated Deprecated in 3ds Max 2015.  
				Segmented characters are not supported.  Setting the display type to segmented will do nothing. */
				eDisplaySegmented , 
#pragma deprecated (eDisplaySegmented)
				//! \brief Custom Skin
				eDisplayCustomSkin,
				//! \brief Textured Skin
				eDisplayTexturedSkin,
			};
	
			//! \brief Turn Optimization Types
			/*! Each of these options specifies an algorithm used to decide which way a character will turn when it reaches an intersection. */
			enum TurnOptimizationType{ 
				//! \brief Chooses turns based on a weighted combination of the eMaxspeed, eMaxdistribution, and eMindistance algorithms.
				eHybrid = 1, 
				//! \brief Chooses turns that move pedestrians at faster speeds through intersections when avoiding collisions.
				eMaxspeed, 
				//! \brief Chooses turns to equally distribute pedestrians into outgoing flows.
				eMaxdistribution,
				//! \brief Chooses turns to minimize distance traveled through intersections.
				eMindistance,
				//! \brief Chooses turns randomly.
				eNone
			};

			//! \name Environment Objects: Flows, Idle Areas and Seats
			//@{ 
			//! \brief Adds a flow to Populate
			/*! A flow must be added to Populate in order for simulation to take place on it.
			The node won't be added if it is not a flow or it is already in Populate. */
			/*! \param[in] flow - The flow node to be added */
			/*! \return true if the flow was added, false if not */
			virtual bool AddFlow(INode *flow)=0;

			//! \brief Adds an idle area to Populate
			/*! An idle area must be added to Populate in order for simulation to take place on it.
			The node won't be added if it is not an idle area or it is already in Populate. */
			/*! \param[in] iArea - The idle area node to be added */
			/*! \return true if the idle area was added, false if not */
			virtual bool AddIdleArea(INode *iArea)=0;
	
			//! \brief Adds a seat to Populate
			/*! A seat must be added to Populate in order for simulation to take place on it.
			The node won't be added if it is not a seat or it is already in Populate. */
			/*! \param[in] seat - The seat node to be added */
			/*! \return true if the seat was added, false if not */
			virtual bool AddSeat(INode *seat)=0;

			//! \brief Updates the marks drawn on an idle area
			/*! This will force the marks on an idle area to be recomputed.
			The node won't be updated if it is not an idle area or it is not in Populate. */
			/*! \param[in] iArea - The idle area node to be updated */
			/*! \return true if the idle area was updated, false if not */
			virtual bool UpdateIdleArea(INode *iArea)=0;

			//! \brief Shows or hides idle areas, flows, and seats
			/*! \param[in] show - true to show idle areas, flows and seats, false to hide them */
			virtual void DisplayEnvironment(bool show)=0;

			//! \brief Returns whether or not the idle areas, flows, and seats are displayed
			virtual bool IsEnvironmentDisplayed()const=0;

			//! \brief Shows or hides marks on idle areas, flows, and seats
			/*! \param[in] show - true to show marks, false to hide them */
			virtual void DisplayMarks(bool show)=0;

			//! \brief Returns whether or not marks on idle areas, flows, and seats are displayed
			virtual bool AreMarksDisplayed()const=0;
			//@}
	
			//! \name Seats
			//@{ 
			//! \brief Returns the probability the next seat created will be for a female
			//! \returns a value between 0.0 and 1.0 indicating the probability the next seat created will be for a female
			virtual float GetSeatFemalePercent()const=0;
	
			//! \brief Sets the probability the next seat created will be for a female
			//! \param[in] percent - a value between 0.0 and 1.0 specifying the probability the next seat created will be for a female
			virtual void SetSeatFemalePercent(float percent)=0;

			//! \brief Sets the gender of the selected seats
			/*! \brief Uses the value of Seat Female Percent to set the gender */
			//! \see SetSeatFemalePercent()
			virtual bool SetSelectedSeatsGender()=0;
			//@}
	
			//! \name Seat Talking Specifications
			//! In order for a person in seat A and B to talk to one another the following conditions must be met:
			//! \li The angle between the vector from seat A to seat B and the orientation vector of seat A must be less than the specified maximum angle.
			//! \li The angle between the vector from seat B to seat A and the orientation vector of seat B must be less than the specified maximum angle. 
			//! \li The distance between seat A and seat B must be less than the maximum talk distance.
			//@{ 
			//! \brief Returns the maximum distance at which seated people will talk to each other
			//! \see GetSeatMaxTalkAngle()
			virtual float GetSeatMaxTalkDist()const=0;

			//! \brief Sets the maximum distance at which seated people will talk to each other
			//! \param[in] distance - the maximum distance at which seated people will talk to each other
			//! \see SetSeatMaxTalkAngle()
			virtual void SetSeatMaxTalkDist(float distance)=0;

			//! \brief Returns the maximum angle at which seated people will talk to each other
			/*! The angle is in degrees. */
			//! \see GetSeatMaxTalkDist()
			virtual float GetSeatMaxTalkAngle()const=0;
	
			//! \brief Sets the maximum angle at which seated people will talk to each other
			//! \param[in] angle - the maximum angle, in degrees, at which seated people will talk to each other
			//! \see SetSeatMaxTalkDist()
			virtual void SetSeatMaxTalkAngle(float angle)=0;
			//@}

			//! \name Character Retrieval
			//! These functions return the number of each type of character, and a node from each character.
			//! These functions must be performed after a simulation, after characters have been created.
			//! If you delete characters or re-simulate, these functions may return different values than they did after a previous simulation.
			//! If you get the node of a character and then you delete the character or change its \ref FigureDisplayType, the node will no longer exist.
			//! Characters of display type \ref eDisplayStickFigs do not have nodes.  They are lines drawn by Populate.
			//! However if you switched from any other display type to the stick figure type, the nodes will still exist and these functions will still return them.
			//@{ 
			//! Returns the number of standing idlers
			virtual int  NumStandingIdlers()=0;
			//! Returns the number of seated idlers
			virtual int  NumSeatedIdlers()=0;
			//! Returns the number of pedestrians
			virtual int  NumPedestrians()=0;
			//! Returns a node from a standing idler, if there is one, NULL if not
			virtual INode *GetStandingIdler(int i)=0;
			//! Returns a node from a seated idler, if there is one, NULL if not
			virtual INode *GetSeatedIdler(int i)=0;
			//! Returns a node from a pedestrian, if there is one, NULL if not
			virtual INode *GetPedestrian(int i)=0;
			//@}

			//! \name Characters
			//@{ 
			//! Returns the display type of the characters
			//! \see FigureDisplayType
			virtual FigureDisplayType  GetDisplayType()const=0;
	
			//! \brief Sets the display type of the characters
			//! \param[in] type 
			//! \see FigureDisplayType
			virtual bool SetDisplayType(FigureDisplayType type)=0;
	
			//! \brief Shows or hides characters
			/*! \param[in] show - true to show characters, false to hide them */
			virtual void DisplayCharacters(bool show)=0;
	
			//! \brief Returns whether or not the characters are displayed
			virtual bool AreCharactersDisplayed()const=0;

			//! \brief Generates new meshes and texture maps for the selected characters
			//! \brief If a pedestrian appears more than once, then only its appearance during time t will be changed.
			//! \param[in] t - time
			virtual bool RegenerateSelected(TimeValue t)=0;

			//! \brief Swaps the appearance of the selected characters
			/*! Only two characters of the same gender can be swapped.
			If a pedestrian appears more than once, all of its appearances will be changed. */
			//! \return True if successful, false if not.
			virtual bool SwapSelectedCharacterAppearance()=0;
	
			//! \brief Switches the mesh resolution for the selected characters
			//! \brief High resolution characters will switch to low, and low resolution characters will switch to high.
			virtual bool SwitchSelectedCharactersResolution()=0;

			//! \brief Deletes all the selected characters
			//! \brief If a pedestrian appears more than once, then only its appearance during time t will be deleted.
			//! \param[in] t - time */
			virtual void DeleteSelectedCharacters(TimeValue t)=0;

			//! \brief Deletes all the characters
			virtual void DeleteAllCharacters()=0;
			//@}
	
			//! \name Simulation
			//@{ 
			//! Returns the number of frames to simulate
			virtual int  GetNumFrames()const=0;
	
			//! Sets the number of frames to simulate
			//! \param[in] nFrames - the number of frames to simulate
			virtual void SetNumFrames(int nFrames)=0;
	
			//! Returns the turn optimization type
			//! \see TurnOptimizationType
			virtual TurnOptimizationType GetTurnOptimization()const=0;

			//! Sets the turn optimization type
			//! \param[in] type - turn optimization type
			//! \see TurnOptimizationType
			virtual void SetTurnOptimization(TurnOptimizationType type)=0;

			//! Computes a simulation
			/*! Characters of the specified display type will appear on all the marks of the idle areas and flows, 
			and on all the chairs.  They will be animated for the specified number of frames.  New characters 
			will be spawned at the ends of the flows over time.  Pedestrian spawning, speeds and genders
			will conform to flow parameters. */
			//! \see SetDisplayType(), SetNumFrames(), IPopulateFlow
			virtual void Simulate()=0;
	
			//! Resimulates all the selected standing or seated idle characters
			virtual void ResimulateSelected()=0;
			//@}
	
			//! \name Scale
			//@{ 
			//! \brief Returns the real world scale
			virtual float GetRealWorldScale()const=0;

			//! \brief Sets the real world scale
			/*! The environment objects will scale appropriately when the real world scale is set.
			But you must simulate in order to scale the characters appropriately. */
			//! \param[in] scale - the real world scale
			virtual void SetRealWorldScale(float scale)=0;
			//@}
		
			//! \name Custom Material, Texture Maps, and Appearance
			//@{ 
			//! Returns the custom material for custom skinned characters
			virtual Mtl *GetCustomMaterial()const=0;

			//! \brief Sets the custom material for custom skinned characters
			/*! This material will only be used when new custom characters are created. */
			//! \param[in] mat - the custom material
			virtual void SetCustomMaterial(Mtl *mat)=0;

			//! \brief Sets the Save Texture Maps flag
			/*! \param[in] save - true to save the texture maps, false to regenerate the texture maps on load */
			virtual void SaveTextureMaps(bool save)=0;

			//! \brief Returns the value of the Save Texture Maps flag
			virtual bool GetSaveTextureMapsState()const=0;

			//! \brief Shows the Appearance Dialog
			virtual void ShowAppearanceDialog()=0;
			//@}

			//! Returns the interface to Populate
			static IPopulate* GetInstance()
			{
				return static_cast<IPopulate*>(GetCOREInterface(POPULATE_INTERFACE));
			}
		};
	} // namespace Populate
} // namespace MaxSDK




