/*==============================================================================

  file:     imrLPE.h

  author:   t_amanp

  created:  31oct2013

  description:

    Interface for accessing the iray LPE.

  modified:	


(c) 2013 Autodesk
==============================================================================*/
#pragma once
#include "../GetCOREInterface.h"
#include "../ifnpub.h"

#define IMRLPE_INTERFACEID Interface_ID(0x77b542ab, 0x744527b4)

//==============================================================================
// class imrLPE
//
// This interface provides access to the iray LPE.
//==============================================================================
class imrLPE : public FPStaticInterface {

public:
    // log file name
    virtual const MCHAR* GetLPE() const = 0;
};

// Retrieve a pointer to the preferences interface
inline imrLPE* GetLPE() {

    return static_cast<imrLPE*>(GetCOREInterface(IMRLPE_INTERFACEID));
}
