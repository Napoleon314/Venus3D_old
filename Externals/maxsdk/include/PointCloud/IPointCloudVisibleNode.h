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
#include "..\export.h"
#include "..\box3.h"
#include "..\containers\Array.h"
#include <vector>

namespace MaxSDK { namespace PointCloud {
	struct DllExport DoublePoint3 : public MaxHeapOperators
	{
		DoublePoint3() : x(0), y(0), z(0) {}
		double x;
		double y;
		double z;
	};

	struct DllExport UBytePoint4 : public MaxHeapOperators
	{
		UBytePoint4() : x(0), y(0), z(0), w(0) {}
// warning C4201: nonstandard extension used : nameless struct/union
#pragma warning(disable:4201)
		union {
			struct {
				unsigned char x;
				unsigned char y;
				unsigned char z;
				unsigned char w;
			};
			struct {
				unsigned char r;
				unsigned char g;
				unsigned char b;
				unsigned char a;
			};
		};
#pragma warning(default:4201)		
	};

	//! \brief A structure that contains the basic information about a point cloud vertex.
	struct DllExport PointCloudVertex : public MaxHeapOperators
	{
		//! The vertex position in the point cloud local space.
		DoublePoint3 mPosition;
		//! The vertex color in RGBA.
		UBytePoint4 mColor;
		//! The vertex normal in point cloud local space.
		DoublePoint3 mNormal;
	};

	//! \brief The interface for interacting with point cloud voxel nodes.
	//! The local space of a point cloud scan data is divided into many parts based on the 
	//! distribution of point data, and each part is represented as a voxel node. This interface 
	//! provides methods to get information about the point cloud vertices contained by the voxel node.
	//! This interface can be retrieved by calling the IPointCloud::CreateIPointCloudVisibleVoxelNodes method.
	class IPointCloudVisibleVoxelNode : public MaxHeapOperators
	{
	public:
		virtual ~IPointCloudVisibleVoxelNode(){}

		//! Get the visible points list in this voxel node.
		virtual const MaxSDK::Array<PointCloudVertex>& GetVisiblePointsList() = 0;

		//! Get the node's bounding box in the point cloud object's local space.
		virtual Box3 GetLocalBoundingBox() = 0;
	};

}}
