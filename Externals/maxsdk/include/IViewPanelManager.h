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
#include "ifnpub.h"
#include "maxtypes.h"
#include "GetCOREInterface.h"

class IViewPanel;

#define VIEWPANEL_MANAGER_INTERFACE Interface_ID(0x0cbc11a6, 0xab341400)

/**
* In 3ds Max 2013, 3ds Max introduces the tabbed view panels. Previously 3ds Max
* can have only one view panel(One view panel contains at most 4 viewports). 
* Now 3ds Max users can create additional view panels other than the default
* view panel by using the tabbed view panel toolbar or MaxScript. Users can
* switch between those view panels. At one time, there can only be one view panel
* visible which we usually call it current view panel or active view panel. Each
* view panel can have one active viewport. Users can also destroy those additional
* view panels (NOTE: The default view panel can't be destroyed). Tabbed view panels
* will be saved & loaded during FileIO. When saving as previous, only the current
* active view panel will be saved.
* This interface is used to manage the multiple tabbed view panels. 
* To acquire this interface, simply use the codes below.
* \code
* #include "IViewPanelManager.h"
* ...
* IViewPanelManager* pViewPanelMgr = GetViewPanelManager();
* ...
* \endcode
*/
class IViewPanelManager : public FPStaticInterface
{
public:
	/**
	* Check if we can create any more tabbed view panels.
	* \return true if we can create more view panels, false otherwise.
	*/
	virtual bool CanCreateMoreViewPanels() const =0;

	/**
	* Create a new tabbed view panel.
	* \param[in] tabName the name of the new tabbed view panel. If NULL, a default name will be assigned
	* to the new tabbed view panel automatically.
	* \param[in] layout the initial layout of the new tabbed view panel. Use VP_LAYOUT_XXX macro in maxapi.h.
	* \param[in] bActive if true, the new tabbed view panel will be set as active view panel after creation.
	* \return Return the index of the new tabbed view panel.If creation fails, -1 will be returned.
	*/
	virtual int	CreateViewPanel(const MCHAR* tabName, int layout, bool bActive) = 0;

	/**
	* Create a new tabbed view panel.  The panel is given a default name based on its layout.
	* \param[in] layout the initial layout of the new tabbed view panel. Use VP_LAYOUT_XXX macro in maxapi.h.
	* \param[in] bActive if true, the new tabbed view panel will be set as active view panel after creation.
	* \return Return the index of the new tabbed view panel.  If creation fails, -1 will be returned.
	*/
	virtual int			CreateViewPanel(int layout, bool bActive) = 0;

	/**
	* Destroy the specified tabbed view panel.
	* \param[in] index the index of the tabbed view panel to be destroyed.
	* An valid index should be from 1(0 is the default view panel) to GetViewPanelCount() - 1.
	* Also, the panel specified cannot be 'locked'.  Locked panels cannot be destroyed.
	* \return Return true if destruction succeeds, false otherwise.
	* \see IsViewPanelLocked(int)
	*/
	virtual bool DestroyViewPanel(int index) = 0;

	/**
	* Get the IViewPanel interface of the active tabbed view panel.
	* \return Return the IViewPanel interface of the current active view panel.
	* Return NULL if there is no active view panel.
	*/
	virtual IViewPanel* GetActiveViewPanel() const = 0;

	/**
	* Set a tabbed view panel as the current active view panel
	* \param[in] index the index of the view panel to be activated.
	* \return Return true if operation succeeds, false otherwise.
	*/
	virtual bool SetActiveViewPanel(int index) = 0;

	/**
	* Get the index of the active view panel.
	* \return Return the index of the current active view panel.
	* Return -1 is there is no active view panel.
	*/
	virtual int GetActiveViewPanelIndex() const = 0;

	/**
	* Get the count of current existing view panels.
	* \return Return the count of current existing view panels.
	*/
	virtual int GetViewPanelCount() const = 0;

	/**
	* Get the IViewPanel interface of the specified tabbed view panel.
	* \param[in] index the index of the tabbed view panel 
	* \return Return the IViewPanel interface of the tabbed view panel specified by
	* the input index. Return NULL
	*/
	virtual IViewPanel*	GetViewPanel(int index) const = 0;

	/**
	* Get the name of the specified view panel.
	* \param[in] index the index of the view panel whose name to be returned
	* \return Return the name of the specified view panel. If the input index is 
	* invalid, NULL will be returned.
	*/
	virtual const MCHAR* GetViewPanelName(int index) const = 0;
	/**
	* Set the name of the specified view panel.
	* \param[in] index the index of the view panel whose name to be set
	* \param[in] newName the new name of the specified view panel
	* \return Return true if the operation succeeds, false otherwise.
	*/
	virtual bool SetViewPanelName(int index, const MCHAR* newName) = 0;

	/**
	 * A locked view panel is one that cannot be deleted by the user.
	 * \param[in] index of the panel to check
	 * \return true if the index is valid and the panel is considered 'locked' by the manager
	 */
	virtual bool IsViewPanelLocked(int index) const = 0;

	/**
	 * Returns the index of the currently highlighted ViewPanel.
	 * This is the index of the tabs as iterated through via Ctrl-Tab
	 * \return Index of the currently highlighted ViewPanel, or -1 if none are highlighted.
	 */
	virtual int GetHighlightViewPanelIndex() const = 0;

	/**
	 * Cycle the highlight through available view panels.
	 * This is the equivalent of the ctrl-tab functionality
	 */
	virtual void CycleHighlightedViewPanels() = 0;
	
	/**
	 * Cycle back through the highlight through available view panels.
	 * This is the equivalent of the shift-ctrl-tab functionality
	 */
	virtual void CycleHighlightedViewPanelsBack() = 0;

	/**
	 * Select the currently highlighted view panel
	 * \return True if the active panel is changed.
	 */
	virtual bool SelectHighlightedViewPanel() = 0;
};

/**
* Return the IViewPanelManager interface.
*/
inline IViewPanelManager* GetViewPanelManager()
{
	return static_cast<IViewPanelManager*>(GetCOREInterface(VIEWPANEL_MANAGER_INTERFACE));
};
