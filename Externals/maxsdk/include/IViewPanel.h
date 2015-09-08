//
// Copyright [2011] Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
//
#pragma once

#include "WindowsDefines.h"
#include "baseinterface.h"
#include "strclass.h"

class IViewInternalExp;
class ViewEx;
class ViewExp;

#define VIEW_PANEL_INTERFACE Interface_ID(0xe3505bb9, 0x1d734df6)

/**
* Many viewpanel related methods of CORE interface only work on the active
* tabbed view panel. For example, Interface::getNumViewports() only return
* the number of viewports of current view panel. This interface provides 
* methods to get/modify settings of a tabbed view panel no matter it is currently
* active or not.
* Here is an example on how to get the number of active and non-extended 
* viewports of all view panels.
* \code
* int viewportCount = 0;
* for (int i = 0; i < GetViewPanelManager()->GetViewPanelCount(); ++i)
* {
*		IViewPanel* pViewPanel = GetViewPanelManager()->GetViewPanel(i);
*		if (pViewPanel)
*		{
*			viewportCount += pViewPanel->GetNumberOfViewports();
*		}
* }
* \endcode
* \see IViewPanelManager
*/
class IViewPanel : public BaseInterface
{
public:
	virtual ~IViewPanel() {}

	virtual Interface_ID GetID() { return VIEW_PANEL_INTERFACE; }

	/**
	 * Gets number of the enabled and non-extended viewports.
	 * \return the number of the enabled and non-extended viewports.
	 */
	virtual size_t GetNumberOfViewports() const = 0;

	/**
	 * Gets the layout of this view panel. 
	 * \return the layout id
	 * \see ViewPanelLayoutConfigIds 
	 */
	virtual int GetLayout() const = 0;

	/**
	 * Set layout of this view panel.
	 * \param[in] layout the layout configuration to use
	 * \see ViewPanelLayoutConfigIds 
	 */
	virtual void SetLayout(int layout) = 0;

	/**
	 * Set layout of this view panel.
	 * \param[in] layout the layout configuration to use
	 * \param[in] forceSettings force the config setting to be re-applied
	 * \see ViewPanelLayoutConfigIds 
	 */
	virtual void SetLayout(int layout, bool forceSettings) = 0;

	/**
	 * Gets index of the viewport with the specified window handle.
	 * User can use the viewport index to get the corresponding ViewExp
	 * interface by calling IViewPanel::GetViewExpByIndex(int index).
	 * \param[in] hwnd the window handle of a viewport in this viewpanel.
	 * \return the index of the viewport with the given window handle.
	 * If none of the viewport matches the given handle, -1 is returned.
	 */
	virtual int	GetViewportIndex(HWND hwnd) = 0;

	/**
	 * Set active viewport according to the input param.
	 * \param[in] index the index of the viewport to be activated
	 * \return true if this operation succeeds, false otherwise.
	*/
	virtual bool SetActiveViewport(int index) = 0;

	/**
	 * Get the index of the active viewport.
	 * \return the index of the active viewport.
	 * If there is no active viewport in this viewpanel, -1 is returned.
	*/
	virtual int	GetActiveViewportIndex(void) = 0;

	
	/**
	 * Get the window handle of the tabbed view panel
	 * \return Return the window handle of the view panel.
	*/
	virtual HWND GetHWnd() = 0;

	/**
	* Set the view panel name.
	* \param[in] newName the new name for the view panel.
	*/
	virtual void SetViewPanelName(const MSTR& newName) = 0;

	/**
	* Get the view panel name.
	* \return Return the view panel name.
	*/
	virtual const MSTR& GetViewPanelName() = 0;

	/**
	* Get the ViewExp interface given the viewport index.
	* \param[in] index a valid viewport index
	* \return if input is valid, the specified ViewExp interface will be returned.
	* Otherwise, the ViewExp interface of the first viewport(index = 0) will be returned.
	*/
	virtual ViewExp& GetViewExpByIndex(int index) = 0;
};