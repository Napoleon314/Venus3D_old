// Copyright 2014 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../FragmentGraph/Fragment.h"

namespace MaxSDK { namespace Graphics {

/** This class provide some facility to add some extended behavior to the fixed 
	render pipeline. For e.g., 3rd party developer may specify their own background 
	fragment, shading fragment for the viewport. 
	Also developer could add post-shading fragments and overlay fragments. Post-shading
	fragments(usually some screen space effects, for e.g., bloom effect or glow effect)
	are directly evaluated right after the evaluation of shaded fragments.
	Overlay fragments which are evaluated at last could be some viewport UI overlay or 
	any other elements to be rendered topmost.
 */
class IFixedViewportPipeline : public MaxHeapOperators
{
public:
	/** Get the count of post shading fragments added.
	*/
	virtual size_t GetNumPostShadingFragments() const = 0;
	/** Get the post shading fragments with index 'i'
	\param[in] i the index of the post shading fragment
	\return the post shading fragment required.
	*/
	virtual FragmentPtr GetPostShadingFragment(size_t i) const = 0;
	/** Insert a new post shading fragments before the post-shading fragment with 
		index 'i'
	\param[in] i the index of the new post shading fragment
	\param[in] pFragment the new post shading fragment
	\return true if succeed or false otherwise.
	*/
	virtual bool InsertPostShadingFragment(size_t i, FragmentPtr pFragment) = 0;
	/** Delete the post-shading fragment with index 'i'
	\param[in] i the index of the post shading fragment to be deleted
	\return true if succeed or false otherwise.
	*/
	virtual bool DeletePostShadingFragment(size_t i) = 0;
	/** Delete a specified post-shading fragment
	\param[in] pFragment the post shading fragment to be deleted
	\return true if succeed or false otherwise.
	*/
	virtual bool DeletePostShadingFragment(FragmentPtr pFragment) = 0;

	/** Get the count of overlay fragments added.
	*/
	virtual size_t GetNumOverlayFragments() const = 0;
	/** Get the overlay fragments with index 'i'
	\param[in] i the index of the overlay fragment
	\return the overlay fragment required.
	*/
	virtual FragmentPtr GetOverlayFragment(size_t i) const = 0;
	/** Insert a new overlay fragments before the post-shading fragment with 
		index 'i'
	\param[in] i the index of the new overlay fragment
	\param[in] pFragment the new overlay fragment
	\return true if succeed or false otherwise.
	*/
	virtual bool InsertOverlayFragment(size_t i, FragmentPtr pFragment) = 0;
	/** Delete the post-shading fragment with index 'i'
	\param[in] i the index of the overlay fragment to be deleted
	\return true if succeed or false otherwise.
	*/
	virtual bool DeleteOverlayFragment(size_t i) = 0;
	/** Delete a specified post-shading fragment
	\param[in] pFragment the overlay fragment to be deleted
	\return true if succeed or false otherwise.
	*/
	virtual bool DeleteOverlayFragment(FragmentPtr pFragment) = 0;

	/** Get the customized background fragment
	*/
	virtual FragmentPtr GetBackgroundFragment() const = 0;
	/** Set a customized background fragment that will replace the default background
		fragment
	\param[in] pFragment the customized background fragment
	\return true if succeed or false otherwise.
	*/
	virtual bool SetBackgroundFragment(FragmentPtr pFragment) = 0;
	/** Restore the default background fragment
	*/
	virtual void RestoreDefaultBackgroundFragment() = 0;

	/** Get the customized shading fragment
	*/
	virtual FragmentPtr GetShadingFragment() const = 0;
	/** Set a customized shading fragment that will replace the default illuminance/shading
		fragment
	\param[in] pFragment the customized shading fragment
	\return true if succeed or false otherwise.
	*/
	virtual bool SetShadingFragment(FragmentPtr pFragment) = 0;
	/** Restore the default shading fragment
	*/
	virtual void RestoreDefaultShadingFragment() = 0;
};

} }// namespaces
