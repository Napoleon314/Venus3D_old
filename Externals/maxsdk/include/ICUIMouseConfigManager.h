//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "ifnpub.h"
#include "GetCOREInterface.h"

/** Interface ID of class IMouseConfigManager*/
#define IID_CUI_MOUSE_CONFIGURE_MANAGER Interface_ID(0x790c76c3, 0x591c3988)

namespace MaxSDK { namespace CUI {

	/*! \defgroup Customized Operation Modes
		Possible supported customized operation modes. The mode can be set in UI or by MAXScript.
	*/
	//@{
	enum OperationMode
	{
		//!< Indicates no operation mode.
		EOperationMode_None = 0,
		//!< Indicates the operation mode is pan.
		EOperationMode_Pan,
		//!< Indicates the operation mode is zoom.
		EOperationMode_Zoom,
		//!< Indicates the operation mode is rotate.
		EOperationMode_Rotate,
		//!< Indicates the count of operation modes.
		EOperationMode_Count,
	};
	//@}

	/*! \defgroup Key as one part of one customized mode's shortcut.
		Possible supported key as one part of one customized mode's shortcut.The key can be set in UI or by MAXScript.
	*/
	//@{
	enum KeyOption
	{
		//!< Indicates no key is selected as one part of one customized mode's shortcut.
		EKey_Null			= 0,
		//!< Indicates the Shift key is selected as one part of one customized mode's shortcut.
		EKey_Shift			= (1<<0),
		//!< Indicates the Ctrl key is selected as one part of one customized mode's shortcut.
		EKey_Ctrl			= (1<<1),
		//!< Indicates the Alt key is selected as one part of one customized mode's shortcut.
		EKey_Alt			= (1<<2),
		//!< Indicates the Shift key and Ctrl key are selected as one part of one customized mode's shortcut.
		EKey_ShiftCtrl		= EKey_Shift|EKey_Ctrl,
		//!< Indicates the Ctrl key and Alt key are selected as one part of one customized mode's shortcut.
		EKey_CtrlAlt		= EKey_Ctrl|EKey_Alt,
		//!< Indicates the Alt key and Shift key are selected as one part of one customized mode's shortcut.
		EKey_AltShift		= EKey_Alt|EKey_Shift,
		//!< Indicates the Shift key and Ctrl key and Alt key and are selected as one part of one customized mode's shortcut.
		EKey_ShiftCtrlAlt	= EKey_Shift|EKey_Ctrl|EKey_Alt,
	};
	//@}

	/*! \defgroup Mouse button as one part of one customized mode's shortcut.
		Possible supported mouse button as one part of one customized mode's shortcut.The mouse button can be set in UI or by MAXScript.
	*/
	//@{
	enum MouseButtonOption
	{
		//!< Indicates no mouse button is selected as one part of one customized mode's shortcut.
		EMouseButton_Null   = 0,
		//!< Indicates the left mouse button is selected as one part of one customized mode's shortcut.
		EMouseButton_Left	= (1<<3),
		//!< Indicates the middle mouse button is selected as one part of one customized mode's shortcut.
		EMouseButton_Middle = (1<<4),
		//!< Indicates the right mouse button is selected as one part of one customized mode's shortcut.
		EMouseButton_Right	= (1<<5),

	};
	//@}

	/*! \defgroup Parameter used by some customized operation mode.
		Possible supported parameter used by some customized operation mode.
	*/
	//@{
	enum OperationParameter
	{
		//!< Indicates no operation parameter.
		EOperationParameter_Null	= 0,
		//!< Indicates the operation parameter is zoom in direction.
		EOperationParameter_ZoomInDirection,
		//!< Indicates the count of operation parameters.
		EOperationParameter_Count,
	};
	//@}

	/*! \defgroup Types of zoom in direction used in customized zoom mode.
		Possible supported zoom in dierections.The zoom in direction can be set in UI or by MAXScript.
	*/
	//@{
	enum ZoomInDirectionOption
	{
		//!< Indicates the zoom in direction is north.
		EZoomInDirection_North		= 1,
		//!< Indicates the zoom in direction is east.
		EZoomInDirection_East		= 2,
		//!< Indicates the zoom in direction is south.	
		EZoomInDirection_South		= 4,
		//!< Indicates the zoom in direction is west.
		EZoomInDirection_West		= 8,
		//!< Indicates the zoom in direction is northeast.
		EZoomInDirection_NorthEast	= EZoomInDirection_North|EZoomInDirection_East,
		//!< Indicates the zoom in direction is southeast.
		EZoomInDirection_SouthEast	= EZoomInDirection_South|EZoomInDirection_East,
		//!< Indicates the zoom in direction is southwest.
		EZoomInDirection_SouthWest	= EZoomInDirection_South|EZoomInDirection_West,
		//!< Indicates the zoom in direction is northwest.
		EZoomInDirection_NorthWest	= EZoomInDirection_North|EZoomInDirection_West,
	};
	//@}

/*! A core interface for accessing properties of the customized mode introduced in 3ds Max 2013.
The mouse config manager allows plug-ins to get the shortcut of customized pan mode or zoom mode
and value of some modes' parameters.
*/
class IMouseConfigManager: public FPStaticInterface
{
public: 
	/*!Plug-ins that have  functions of setting customized pan,zoom,rotate or other customized mode 
	will need to get concrete shortcut of different customized mode when users are switching among modes.
	If the operation mode as input parameter is valid, the output parameters of function GetOperationShortcut()
	will be the shortcut that has been set in custom user interface or by MAXScript.
	\param[in] eOperation The customized mode indicated by UI or MAXScript.
	\param[out] eKey The key of customized mode's shortcut indicated by UI or MAXScript.
	\param[out] eButton The mouse button of customized mode's shortcut indicated by UI or MAXScript.
	\code
	bool MyPlugin::IsCustomizedPanMode() 
	{
		using namespace MaxSDK::CUI;
		KeyOption ePanKey				= EKey_Null;
		MouseButtonOption ePanButton	= EMouseButton_Null;
		if(GetOperationShortcut(EOperationMode_Pan,ePanKey,ePanButton) == false)
			return false;

		int iKeyFlag = 0;
		if (ePanKey & EKey_Shift)
		{
			iKeyFlag |= MOUSE_SHIFT;
		}
		if (ePanKey & EKey_Ctrl)
		{
			iKeyFlag |= MOUSE_CTRL;
		}
		if (ePanKey & EKey_Alt)
		{
			iKeyFlag |= MOUSE_ALT;
		}

		int iMouseFlag = 0;
		if (ePanButton & EMouseButton_Left)
		{
			iMouseFlag |= MOUSE_LBUTTON;
		}
		if (ePanButton & EMouseButton_Right)
		{
			iMouseFlag |= MOUSE_RBUTTON;
		}
		if (ePanButton & EMouseButton_Middle)
		{
			iMouseFlag |= MOUSE_MBUTTON;
		}
		int iPanFlag		= iKeyFlag|iMouseFlag;


		int iButtonStates = 0;
		if ( GetAsyncKeyState(VK_MENU) & 0x8000 ) 
		{
			iButtonStates |= MOUSE_ALT;
		}
		if ( GetAsyncKeyState(VK_CONTROL) & 0x8000 ) 
		{
			iButtonStates |= MOUSE_CTRL;
		}
		if ( GetAsyncKeyState(VK_SHIFT) & 0x8000 ) 
		{
			iButtonStates |= MOUSE_SHIFT;
		}

		if ( GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			iButtonStates |= MOUSE_LBUTTON;
		}
		if ( GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			iButtonStates |= MOUSE_RBUTTON;
		}
		if ( GetAsyncKeyState(VK_MBUTTON) & 0x8000)
		{
			iButtonStates |= MOUSE_MBUTTON;
		}

		//Try the exact match firstly
		if(iButtonStates == iPanFlag)
		{
			return true;
		}
		//if no exact match, use & instead of ==
		if((iButtonStates & iPanFlag) == iPanFlag)
		{
			return true;
		}

		return false;
	}
	\endcode
	return true if the customized mode is pan mode.
	*/
	virtual bool GetOperationShortcut(OperationMode eOperation,KeyOption &eKey,MouseButtonOption &eButton) const = 0;

	/*!Plug-ins that have  functions such as zoom or the other operation that need to get values of some operation parameters 
	will use this function.For example,plug-ins will need to get the zoom in direction when user is in zoom mode and want to 
	try different zoom in directions.If there is the corresponding operation parameter as input, the return value is true and 
	the input/output parameter will be the value of	operation parameter.Otherwise,if there is no corresponding operation paramter
	as input,the return value is false and the output parameter	keep unchanged.
	\param[in] parmeter The parameter of some customized modes indicated by UI or MAXScript.
	\param[out] ivalue The value of parameter of some customized modes.
	\code
	#define ZOOM_FACT	0.01f
	void MyPlugin::Zoom() 
	{
		IPoint2 delta = OrigialPoint-CurrentPoint;
		int iDirection = 0;
		MaxSDK::CUI::GetValueOfOperationParameter(MaxSDK::CUI::EOperationParameter_ZoomInDirection,iDirection);
		float fVerticalCoefficience		= 1.0;
		float fHorizontalCoefficience	= 1.0;

		if(iDirection & MaxSDK::CUI::EZoomInDirection_North)
		{
			fVerticalCoefficience = delta.y < 0 ? (1.0f/(1.0f-ZOOM_FACT*delta.y)) : (1.0f+ZOOM_FACT*delta.y);
		}
		else if(iDirection & MaxSDK::CUI::EZoomInDirection_South)
		{
			fVerticalCoefficience = delta.y < 0 ? (1.0f-ZOOM_FACT*delta.y) : (1.0f/(1.0f+ZOOM_FACT*delta.y));
		}

		if(iDirection & MaxSDK::CUI::EZoomInDirection_East)
		{
			fHorizontalCoefficience = delta.x < 0 ? (1.0f-ZOOM_FACT*delta.x) : (1.0/(1.0f+ZOOM_FACT*delta.x));
		}
		else if(iDirection & MaxSDK::CUI::EZoomInDirection_West)
		{
			fHorizontalCoefficience = delta.x > 0 ? (1.0f+ZOOM_FACT*delta.x) : (1.0/(1.0f-ZOOM_FACT*delta.x));;
		}
		float fZoomCoefficience = fVerticalCoefficience * fHorizontalCoefficience;
		..................

	}
	\endcode
	*/
	virtual bool GetValueOfOperationParameter(OperationParameter parmeter, int& ivalue) const = 0;

	/*! In selection mode, using modifier keys add additional behaviors to pure selection. For instance, 
	 * Ctrl + click is used for add selection, Alt for substract selection. While in Maya mode, 
	 * Shift is used for add/substract selection, Ctrl for substract selection.
	 * This function set the Maya selection mode.
	\param[in] isMayaSelection true if using Maya selection mode.
	*/
	CoreExport void SetMayaSelection(bool isMayaSelection);

	/*! In selection mode, using modifier keys add additional behaviors to pure selection. For instance, 
	 * Ctrl + click is used for add selection, Alt for substract selection. While in Maya mode, 
	 * Shift is used for add/substract selection, Ctrl for substract selection.
	 * This function get whether Max is in Maya selection mode.
	\return true if Max is in Maya selection mode, otherwise false.
	*/
	CoreExport bool GetMayaSelection() const;

};

/*! Helper function to access the IMouseConfigManager core interface
\return A pointer to IMouseConfigManager
*/
inline IMouseConfigManager* GetIMouseConfigManager()
{
	return dynamic_cast<IMouseConfigManager*>(GetCOREInterface(IID_CUI_MOUSE_CONFIGURE_MANAGER));
};

/*! Helper function to get shortcuts of customized mode.
 * The output parameters are the shortcuts. Returns true if the shortcuts can be obtained, otherwise false.
*/
inline bool GetOperationShortcut(OperationMode eOperation,KeyOption &eKey,MouseButtonOption &eButton)
{
	IMouseConfigManager* pMouseConfigManager = GetIMouseConfigManager();
	if(pMouseConfigManager != NULL)
	{
		return pMouseConfigManager->GetOperationShortcut(eOperation,eKey,eButton);
	}
	return false;
};

/*! Helper function to get the value of an operation parameter such as zoom in direction of customized zoom mode.
\returns true if value can be obtained, otherwise false.
*/
inline bool GetValueOfOperationParameter(OperationParameter eParmeter, int& iValue)
{
	IMouseConfigManager* pMouseConfigManager = GetIMouseConfigManager();
	if(pMouseConfigManager != NULL)
	{
		return pMouseConfigManager->GetValueOfOperationParameter(eParmeter, iValue);
	}

	return false;
};

} }// end namespace 
