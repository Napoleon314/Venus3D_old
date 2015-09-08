//**************************************************************************/
// Copyright (c) 2014 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/

#pragma once

#include "..\ifnpub.h"
#include "..\ipoint2.h"
#include "..\export.h"
#include "..\Graphics\Matrix44.h"
#include "..\containers\Array.h"
#include "IPointCloudVisibleNode.h"
#include <vector>

class ViewExp;
class INode;

namespace MaxSDK { namespace PointCloud{

//! \brief Defines the interface ID for the <b>IPointCloud</b> interface.
/** The ID can be used to return the interface pointer through the <b>GetInterface()</b> method for classes 
	which implement <b>IPointCloud</b>.
*/
#define IID_POINT_CLOUD Interface_ID(0x181a2a1c, 0x34ea4e72)

	//! \defgroup Point Cloud Object ParamBlock IDs
	//@{
	enum PointCloudPBIds
	{
		pb_filename, //!< TYPE_FILENAME: the point cloud file
		pb_voxelsize, //!< used internally
		pb_as_pixel_pointsize, //!< TYPE_FLOAT: screen space size 
		pb_real_world_scale_pointsize, //!< TYPE_FLOAT: world space size
		pb_pointsize_types, //!< TYPE_INT: whether to use screen space size or world space size
		pb_solidColor, //!< color used when the display technique is solid color mode
		pb_display_technique, //!< TYPE_INT: display mode : intensity, normal, evaluation, etc...
		pb_gradient_texmap, //!< TYPE_TEXMAP: gradient map used in evaluation mode.
		pb_performance_quality, //!< TYPE_INT: control the performance <=> quality slider bar
		pb_fixed_LOD_enable, //!< TYPE_BOOL: whether use fixed LOD during off-line rendering
		pb_fixed_LOD_level, //!< TYPE_INT: the fixed LOD level to be used in off-line rendering
		pb_enable_limitplanes, //!< TYPE_BOOL: whether limit box is enabled or not
		pb_fence_globalenable, //!< TYPE_BOOL: whether display volumes are enabled or not
		pb_fence_objects, //!< TYPE_INODE_TAB the nodes which are used as display volumes (only box, sphere and plane are supported)
		pb_fence_globalinvert, //!< TYPE_BOOL: whether to invert the clip effect 
		pb_geomshader, //!< used internally
		pb_scan_files, //!< used internally
	};
	//@}
	
	//! \brief The interface for interacting with point cloud objects.
	/*! This interface provides methods for interacting with point cloud objects. For example, it provides a method to 
		perform	hit tests on point cloud objects and a method to get information about all visible point cloud
		vertices from a specified view. 
		Users can get this interface by calling the GetInterface(IID_POINT_CLOUD) method for classes that implement 
		this interface.
		\code
			const ObjectState& os = node->EvalWorldState( t );
			IPointCloud* pPointCloud = dynamic_cast<IPointCloud*>(os.obj->GetInterface(IID_POINT_CLOUD));
			if (pPointCloud)
			{
			    // your code...
			}
		\endcode
	*/
	class IPointCloud : public FPMixinInterface
	{
	public:		
		//! \brief Creates an IPointCloudVisibleVoxelNode array based on the current point cloud settings and input parameters.
		//! IPointCloudVisibleVoxelNode contains methods to get information about points (position, normal, color, etc).
		//! After creating this object, you are responsible for destroying the IPointCloudVisibleVoxelNode in the array.
		//! \param[in] t - time 
		//! \param[in] pNode - point cloud object node
		//! \param[in] viewMatrix - world to view transformation matrix
		//! \param[in] projectMatrix - view to screen transformation matrix
		//! \param[in] width - display output target width
		//! \param[in] height - display output target height
		//! \param[out] visibleNodesList - the result based on all the input parameters and point cloud settings
		//! \return true if the operation succeeded, false otherwise.
		virtual bool CreateIPointCloudVisibleVoxelNodes(
			TimeValue t,
			INode* pNode,
			const MaxSDK::Graphics::Matrix44& viewMatrix, 
			const MaxSDK::Graphics::Matrix44& projectMatrix, 
			int width, 
			int height,
			MaxSDK::Array<IPointCloudVisibleVoxelNode*>& visibleNodesList) = 0;

		//! \brief Return the total point count of the current point cloud file.
		virtual unsigned long GetTotalPointCount() = 0;

		//! \brief Return the visible point count in a specified viewport.
		virtual unsigned long GetVisiblePointCount(ViewExp& vpt) = 0;

		/*!	 Return the hit test result for a point cloud. If you just want to know whether the hit test ray hits
			the point cloud object, and do not need to get the hit points, the last two parameters of this function
			can be assigned as NULL. 
			\param[in] pPoint - The screen point hit by the ray. 
			\param[in] width - The extent in width for the screen point hit by the test ray. 
			\param[in] height - The extent in height for the screen point hit by the test ray. 
			\param[in] pViewExp - The viewport for the screen point hit by the test ray. 
			\param[out] pHitListOut - The unsorted vertices of the point cloud hit by the test ray. 
			\param[out] pNearestHitPoint - The nearest vertex of the point cloud hit by the test ray.  
		*/
		virtual bool HitTestPointCloud(const IPoint2* pPoint, const int width, const int height, ViewExp *pViewExp, MaxSDK::Array<PointCloudVertex>* pHitListOut, PointCloudVertex* pNearestHitPoint) = 0;

		//! \brief Returns a pointer to the interface metadata for this class.
		//! \see FPMixinInterface::GetDesc
		virtual FPInterfaceDesc* GetDesc() { return this->GetDescByID(IID_POINT_CLOUD); }

	};
}}

