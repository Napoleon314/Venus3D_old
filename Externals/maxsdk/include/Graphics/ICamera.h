//
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../../../maxsdk/include/point3.h"
#include "../../../maxsdk/include/Graphics/Matrix44.h"
#include "../../../maxsdk/include/Graphics/IRefObject.h"

class ViewExp;

namespace MaxSDK { namespace Graphics {
/** The interface of a render camera for nitrous viewport. ICamera would be required
	by CustomRenderStageHandle as the render camera for a custom render stage.
	\sa CustomRenderStageHandle
*/
class ICamera : public ARefObject
{
public:
	/** Create a new ICamera object
		\return a new ICamera object created
	*/
	GraphicsDriverAPI static RefPtr<ICamera> Create();

	/** Set the view matrix of this camera
		\param[in] viewMatrix the view matrix
	*/
	virtual void SetViewMatrix(const Matrix44& viewMatrix) = 0;
	/** Get the view matrix of the camera
		\return the view matrix of the camera
	*/
	virtual const Matrix44& GetViewMatrix() const = 0;

	/** Set the projection matrix of this camera
		\param[in] viewMatrix the projection matrix
	*/
	virtual void SetProjectionMatrix(const Matrix44& projectionMatrix) = 0;
	/** Get the projection matrix of this camera
		\return the projection matrix of this camera
	*/
	virtual const Matrix44& GetProjectionMatrix() const = 0;

	/** Set the target distance for this camera
		\param[in] targetDistance the target distance
	*/
	virtual void SetTargetDistance(float targetDistance) = 0;
	/** Get the target distance of this camera
		\return the target distance of this camera
	*/
	virtual float GetTargetDistance() const = 0;

	/** Get the position of this camera
		\return the position of this camera
	*/
	virtual const Point3& GetPosition() const = 0;
	/** Get the up vector of this camera
		\return the up vector of this camera
	*/
	virtual const Point3& GetUp() const = 0;
	/** Get the direction of this camera
		\return the direction of this camera
	*/
	virtual const Point3& GetDirection() const = 0;

	/** Is the projection matrix used by a perspective viewport or not
		\return true if the projection matrix is used by a perspective viewport or 
		false otherwise
	*/
	virtual bool IsPerspective() const = 0;
};

typedef RefPtr<ICamera> CameraPtr;

} } // namespaces

