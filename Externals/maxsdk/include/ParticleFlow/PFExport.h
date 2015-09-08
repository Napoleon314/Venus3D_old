/*! \file PFExport.h
    \brief defines preprocessor switch for PF.dll
				 and client application
*/
/**********************************************************************
 *<
	CREATED BY: Oleg Bayborodin

	HISTORY: created 9-28-01

 *>	Copyright (c) 2001, All Rights Reserved.
 **********************************************************************/

#pragma once



#ifdef PF_EXPORTS
#define PFExport	__declspec( dllexport )
#else
#define PFExport	__declspec( dllimport )
#endif




