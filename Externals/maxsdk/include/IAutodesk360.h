#pragma once

#include "iFnPub.h"

#define AUTODESK360_INTERFACE Interface_ID(0x52050f19, 0x654018d5)

/** \brief Interface class for exposing methods associated with Autodesk 360.
 * This class can be used to get some information on the login state of the 
 * current user into Autodesk 360. 
 * This informations may be helpful for dealing with rendering in the A360 
 * cloud as well as for using the OxygenID of the current user that may be used
 * for Autodesk AppStore related validations. 
 */
class IAutodesk360 : public FPStaticInterface
{
	public:
		
		/** \brief Logs into Autodesk 360.
		 * This function will return immediately, the login procedure gets 
		 * performed in an asynchronous way.
		 *
		 * \see IsLoggedIn(), IsLoginValid(), Logout()
		 */
		virtual bool Login() const = 0;


		/** \brief Logs out of Autodesk 360. 
		 * \see Login(), IsLoggedIn(), IsLoginValid()
		 */
		virtual bool Logout() const = 0;


		/** \brief Checks if the user is logged in into Autodesk 360. 
		 * This function just checks if the user is logged in on this machine,
		 * but the login information might be cached and the session might not 
		 * be valid any more. 
		 * To Check if the session is valid, you have to call IsLoginValid().
		 *
		 * \see IsLoginValid()
		 */
		virtual bool IsLoggedIn() const = 0;


		/** \brief Checks if the user is logged into Autodesk 360 and the 
		 * login session is still valid. 
		 * Checks if the user is logged in into and the login session has not 
		 * yet expired. To Check if a user is logged in only, you may also call 
		 * IsLoggedIn(). 
		 *
		 * \return True if the user is logged in and the login session has not 
		 *         expired, false in all the other cases.
		 * \see IsLoggedIn()
		 */
		virtual bool IsLoginValid() const = 0;


		/** \brief Gets the username if the user is logged in.
		 * \return The Autodesk360 username of the logged in user, or an empty 
		 *         string, if the user is not logged in. 
		 * \see GetOxygenID(), IsLoggedIn()
		 */
		virtual const MSTR GetLoginUserName() const = 0;


		/** \brief Gets the OxygenID of the logged in Autodesk360 user. 
		 * The OxygenID can be used for Autodesk AppStore related validations. 
		 * \return The OxygenID of the logged in A360 user or an empty string, 
		 *         if the user is currently not logged in. 
		 * \see IsLoggedIn(), IsLoginValid()
		 */ 
		virtual MSTR GetOxygenID() const = 0; 


		/** \brief Initializes web-services if not previously initialized.
		 * The internal web-services get implicit initialized, so this function 
		 * normally has not to be called at all, but since loading the services
		 * may be a costly operation, this call may be used to preload the 
		 * needed web-services. 
		 */
		virtual bool InitializeWebServices() const = 0;

		// functions ----------------------------------------------------------
		enum { em_login,
			   em_logout,
			   em_isloggedin,
			   em_isloginvalid,
			   em_getloginusername,
			   em_getoxygenid,
			   em_initialize,
			   };
};

//! \brief Gets the IAutodesk360 interface pointer.
CoreExport IAutodesk360* GetAutodesk360Interface();