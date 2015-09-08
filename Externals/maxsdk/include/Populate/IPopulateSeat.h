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
#include "../object.h"

//! \brief The class ID for the IPopulateSeat class
#define POPULATE_SEAT_CLASS_ID	Class_ID(0x3a981738, 0x5250208c)
//! \brief The interface ID for the IPopulateSeat class
#define POPULATE_SEAT_INTERFACE Interface_ID(0x5c90219a, 0x53886809)

namespace MaxSDK 
{
	namespace Populate 
	{
/** IPopulateSeat is the interface class to Populate seat objects. 
\par Example
The following code creates a Populate seat, sets a seat parameter, and adds it to Populate.
\snippet howto/Populate/PopulateExample.cpp Create Populate Seat
\sa IPopulate, IPopulateFlow, IPopulateIdleArea
*/
		class IPopulateSeat : public BaseInterface
		{
		  public:
	  
			//! \brief Seat Gender Types
			enum SeatGenderType
			{
				//! \brief Male Seat
				eMaleSeat = 0,
				//! \brief Female Seat
				eFemaleSeat, 
			};
	
			//! \brief Returns the seat gender
			//! \see SeatGenderType
			virtual SeatGenderType GetGender()=0;
			//! \brief Sets the seat gender
			//! \param[in] gender
			//! \see SeatGenderType
			virtual void SetGender(SeatGenderType gender)=0;
			//! \brief Returns whether or not the person on the seat will be single
			/*! If single, the person on the seat will be simulated not interacting with anyone else, 
			not talking or listening to another seated character. 
			Otherwise, the simulation will decide if the person should interact or not, and with whom. */
			virtual bool GetSingle() const = 0;
			//! \brief Sets whether or not the person on the seat will be single
			/*! If single, the person on the seat will be simulated not interacting with anyone else, 
			not talking or listening to another seated character. 
			Otherwise, the simulation will decide if the person should interact or not, and with whom. */
			//! \param[in] single
			virtual void SetSingle(bool single)=0;
			//! \brief Returns the seat height
			virtual float GetHeight() const = 0;
			//! \brief Sets the seat height
			/*! The height will be clipped to be in between 16 and 18 inches. */
			//! \param[in] height
			virtual void SetHeight(float height)=0;

			// Function publishing
			//FPInterfaceDesc* GetDesc();
			virtual Interface_ID GetID() { return POPULATE_SEAT_INTERFACE; }
	
			//! \brief Gets the interface to a Populate Seat object
			//! \param[in] obj - Populate seat object
			//! \return - a pointer to an IPopulateSeat interface, or NULL.
			static inline IPopulateSeat* GetPopulateSeatInterface(Object* obj) { 
				return obj? static_cast<IPopulateSeat*>(obj->GetInterface(POPULATE_SEAT_INTERFACE)): NULL;}
		};
	} // namespace Populate
} // namespace MaxSDK

