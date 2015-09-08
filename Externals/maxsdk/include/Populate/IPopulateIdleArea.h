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

#include "../SingleWeakRefMaker.h"
#include "PopulateExport.h"
// forward declaration
class INode;

//! \brief The class ID for the IPopulateIdleArea class	
#define POPULATE_IDLEAREA_CLASS_ID	Class_ID(685327,452283)

namespace MaxSDK 
{
	namespace Populate 
	{
/** IPopulateIdleArea is the interface class to Populate idle area objects.  
The idle area object is a scripted plugin that can be found in: stdplugs\stdscripts\IdleAreaObject.ms.
Because it is a scripted plugin, the interface to it is different than a standard class interface.  
Also, Idle Areas inherit from \ref EPoly class.  To create or edit their geometry, you have to access the base edit poly object.

\par Example
The following code creates a rectangular Populate idle area, sets some if its parameters, and adds it to Populate.
\snippet howto/Populate/PopulateExample.cpp Create Populate Idle Area

		\sa IPopulate, IPopulateFlow, IPopulateSeat
		*/
		class IPopulateIdleArea : public MaxSDK::Util::Noncopyable
		{
		  private:
			SingleRefMaker mIdleAreaOb_swrm;
			float GetFloat(int ID) const;
			bool  SetFloat(int ID, float value);
			int   GetInt(int ID) const;
			bool  SetInt(int ID, int value);

		  public:
			IPopulateIdleArea() {}
			~IPopulateIdleArea() {}
	
			//! \brief Sets the internal reference to the idle area object
			/*! This function must be called, and must return true, in order to use any of the other functions. */
			//! \param[in] instance - an object of class \ref POPULATE_IDLEAREA_CLASS_ID
			POPULATE_EXPORT bool SetIdleAreaObjInstance(ReferenceTarget *instance);
			//! \brief Returns the area density, a value between 0.0 and 1.0
			POPULATE_EXPORT float GetDensity() const;
			//! \brief Sets the area density
			//! \param[in] density - a value between 0.0 and 1.0
			POPULATE_EXPORT bool  SetDensity(float density);
			//! \brief Returns the percent of groups in the area, a value between 0.0 and 1.0
			POPULATE_EXPORT float GetGroups() const;
			//! \brief Sets the percent of groups in the area
			//! \param[in] groupPercent - a value between 0.0 and 1.0
			POPULATE_EXPORT bool  SetGroups(float groupPercent);
			//! \brief Returns the percent of pairs in the area, a value between 0.0 and 1.0
			POPULATE_EXPORT float GetPair() const;
			//! \brief Sets the percent of pairs in the area
			//! \param[in] pairPercent - a value between 0.0 and 1.0
			POPULATE_EXPORT bool  SetPair(float pairPercent);
			//! \brief Returns the percent of females in the area, a value between 0.0 and 1.0
			POPULATE_EXPORT float GetFemPercent() const;
			//! \brief Sets the percent of females in the area
			//! \param[in] femPercent - a value between 0.0 and 1.0
			POPULATE_EXPORT bool  SetFemPercent(float femPercent);
			//! \brief Returns the base orientation of single characters, an angle in degrees
			POPULATE_EXPORT float GetSinglesOrientation() const;
			//! \brief Sets the base orientation of single characters
			//! \param[in] orientation - an angle in degrees
			POPULATE_EXPORT bool  SetSinglesOrientation(float orientation);
			//! \brief Returns the maximum random variation from the base orientation of single characters, an angle in degrees
			POPULATE_EXPORT float GetOrientSpread() const;
			//! \brief Sets the maximum random variation from the base orientation of single characters
			//! \param[in] spread - an angle in degrees
			POPULATE_EXPORT bool  SetOrientSpread(float spread);
			//! \brief Returns whether or not the single characters on the area are facing the look at object
			POPULATE_EXPORT bool GetFacing() const;
			//! \brief Sets whether or not the single characters on the area are facing the look at object
			//! \param[in] facing
			POPULATE_EXPORT bool SetFacing(bool facing);	
			//! \brief Returns the area look at object
			POPULATE_EXPORT INode *GetLookAtObject() const;
			//! \brief Sets the area look at object
			//! \param[in] node
			POPULATE_EXPORT bool SetLookAtObject(INode *node);
			//! \brief Returns the position randomization seed
			POPULATE_EXPORT int  GetPositionRandomSeed() const;
			//! \brief Sets the position randomization seed
			//! \param[in] seed
			POPULATE_EXPORT bool SetPositionRandomSeed(int seed);	
			//! \brief Returns the singles randomization seed
			/*! This seeds the random placement of singles characters. */
			POPULATE_EXPORT int  GetSinglesRandomSeed() const;
			//! \brief Sets the singles randomization seed
			/*! This seeds the random placement of singles characters. */
			//! \param[in] seed
			POPULATE_EXPORT bool SetSinglesRandomSeed(int seed);
			//! \brief Returns the orientation spread randomization seed
			POPULATE_EXPORT int  GetOrientationRandomSeed() const;
			//! \brief Sets the orientation spread randomization seed
			//! \param[in] seed
			POPULATE_EXPORT bool SetOrientationRandomSeed(int seed);	
			//! \brief Returns the gender randomization seed
			POPULATE_EXPORT int  GetGenderRandomSeed() const;
			//! \brief Sets the gender randomization seed
			//! \param[in] seed
			POPULATE_EXPORT bool SetGenderRandomSeed(int seed);	
			//! \brief Returns the motion randomization seed
			POPULATE_EXPORT int  GetMotionRandomSeed() const;
			//! \brief Sets the motion randomization seed
			//! \param[in] seed
			POPULATE_EXPORT bool SetMotionRandomSeed(int seed);	
		};
	} // namespace Populate
} // namespace MaxSDK
