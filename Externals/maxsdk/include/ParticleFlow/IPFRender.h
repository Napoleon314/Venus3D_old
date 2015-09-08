/*! \file IPFRender.h
    \brief Render related interface for an Operator.
				 If an Operator has a Render interface
				 then the Operator is called to supply geometry for render
				 in the current ActionList.
*/
/**********************************************************************
 *<
	CREATED BY: Oleg Bayborodin

	HISTORY: created 01-29-02

 *>	Copyright (c) 2001, All Rights Reserved.
 **********************************************************************/

#pragma once

#include "PFExport.h"
#include "../ifnpub.h"
#include "../triobj.h"

PFExport Object* GetPFObject(Object* obj);
// interface ID
#define PFRENDER_INTERFACE Interface_ID(0x75aa7a7f, 0x19c66a0a)
#define GetPFRenderInterface(obj) ((IPFRender*)((GetPFObject(obj))->GetInterface(PFRENDER_INTERFACE))) 

class IPFRender : public FPMixinInterface
{

public:
      /** @defgroup IPFRender IPFRender.h
      *  @{
      */

      /*! \brief The following methods are redirected from the particle object level to 
	   a particular ActionList. The first parameter is a particle container of the ActionList.
	   Returns 0 if the action doesn't render in all frames for any reason, for example a non-animated parameter prevents render.
      */
	virtual int HasRenderableGeometry() { return(1); }
#pragma warning(push)
#pragma warning(disable:4100)
      /*! \brief If an object creates different  meshes depending on the particular instance (view-dependent) it should return 1.
      \param pCont: particle container to render particles from      
      \param pSystem: master particle system shell
      */
	virtual int IsInstanceDependent(IObject* pCont, Object* pSystem) { return 0; }

      /*! \brief GetRenderMesh should be implemented by all renderable GeomObjects.
	Set needDelete to TRUE if the render should delete the mesh, FALSE otherwise.
	Primitives that already have a mesh cached can just return a pointer to it (and set needDelete = FALSE).
      \param pCont: particle container to render particles from
      \param t: current frame to render particles at. It's supposed that the container has been already updated to satisfy this moment
      \param pSystem: master particle system shell
      \param inode: INode of the particle group
      \param view: View as of the GeomObject::GetRenderMesh() method
      \param needDelete: boolean as of the GeomObject::GetRenderMesh() method
      */
	virtual Mesh* GetRenderMesh(IObject* pCont, TimeValue t, Object* pSystem, INode *inode, View& view, BOOL& needDelete) = 0;

      /*! \brief Objects may now supply multiple render meshes. If this function
	returns a positive number, then GetMultipleRenderMesh and GetMultipleRenderMeshTM will be 
	called for each mesh, instead of calling GetRenderMesh. 
      \param pCont: particle container to render particles from
      \param t: current frame to render particles at. It's supposed that the container has been already updated to satisfy this moment
      \param pSystem: master particle system shell
      */
	virtual int NumberOfRenderMeshes(IObject* pCont, TimeValue t, Object* pSystem) { return 0; } // 0 indicates multiple meshes not supported.

      /*! \brief For multiple render meshes, this method must be implemented. 
	Set needDelete to TRUE if the render should delete the mesh, FALSE otherwise.
	meshNumber specifies which of the multiplie meshes is being asked for.
      \param pCont: particle container to render particles from
      \param t: current frame to render particles at. It's supposed that the container has been already updated to satisfy this moment
      \param pSystem: master particle system shell
      \param inode: INode of the particle group
      \param view: View as of the GeomObject::GetRenderMesh() method
      \param needDelete: boolean as of the GeomObject::GetRenderMesh() method
      \param meshNumber: specifies which of the multiplie meshes is being asked for
      */
      virtual	Mesh* GetMultipleRenderMesh(IObject* pCont, TimeValue t, Object* pSystem, INode *inode, View& view, BOOL& needDelete, int meshNumber) { return NULL; }

      /*! \brief For multiple render meshes, this method must be implemented. 
	meshTM should be returned with the transform defining the offset of the particular mesh in object space.
	meshTMValid should contain the validity interval of meshTM 
      \param pCont: particle container to render particles from
      \param t: current frame to render particles at. It's supposed that the container has been already updated to satisfy this moment
      \param pSystem: master particle system shell
      \param inode: INode of the particle group
      \param view: View as of the GeomObject::GetRenderMesh() method
      \param meshNumber: specifies which of the multiplie meshes is being asked for
      \param meshTM
      \param meshTMValid
      */
	virtual void GetMultipleRenderMeshTM(IObject* pCont, TimeValue t, Object* pSystem, INode *inode, View& view, int meshNumber, 
					Matrix3& meshTM, Interval& meshTMValid) { return;  }

      /*! \brief If this returns NULL, then GetRenderMesh will be called
      \param pCont: particle container to render particles from
      \param t: current frame to render particles at. It's supposed that the container has been already updated to satisfy this moment
      \param pSystem: master particle system shell
      \param inode: INode of the particle group
      \param view: View as of the GeomObject::GetRenderMesh() method
      \param needDelete: boolean as of the GeomObject::GetRenderMesh() method
      */
	virtual PatchMesh* GetRenderPatchMesh(IObject* pCont, TimeValue t, Object* pSystem, INode *inode, View& view, BOOL& needDelete) { return NULL; }

      /*! \brief This method allows objects to specify the class that is the best class to convert to when the user collapses the stack. 
      \param pCont: particle container 
      \param pSystem: master particle system shell
      \returns The Class_ID of the preferred object type. 
      */
	virtual Class_ID PreferredCollapseType(IObject* pCont, Object* pSystem) { return triObjectClassID; }

      /*! \brief Returns TRUE if this object can do displacement mapping; otherwise FALSE. 
      \param pCont: particle container 
      \param pSystem: master particle system shell
      */
	virtual BOOL CanDoDisplacementMapping(IObject* pCont, Object* pSystem) { return 0; }
#pragma warning(pop)

      /*! \brief 
      */
	FPInterfaceDesc* GetDesc() { return GetDescByID(PFRENDER_INTERFACE); }
};

inline IPFRender* PFRenderInterface(Object* obj) {
	return ((obj == NULL) ? NULL : GetPFRenderInterface(obj));
};

inline IPFRender* PFRenderInterface(INode* node) {
	return ((node == NULL) ? NULL : PFRenderInterface(node->GetObjectRef()));
};



