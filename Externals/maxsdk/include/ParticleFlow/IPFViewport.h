/*! \file IPFViewport.h
    \brief Viewport related interface for an Operator
				 If an Operator has a Viewport interface
				 then the Operator is called to draw particles
				 in the current ActionList
*/
/**********************************************************************
 *<
	CREATED BY: Oleg Bayborodin

	HISTORY: created 11-07-01

 *>	Copyright (c) 2001, All Rights Reserved.
 **********************************************************************/

#pragma once

#include "PFExport.h"
#include "../object.h"
#include "../BuildWarnings.h"


/** The Interface ID used for IPFViewportExtension2.
*/
#define IPFVIEWPORT_EXTENSION2_INTERFACE_ID Interface_ID (0x44775b10, 0x61a56439)

/**
	This interface is used for extending IPFViewport with an additional method UpdateDisplay(). 
	This provides a default implementation of the virtual function MaxSDK::Graphics::IObjectDisplay::UpdateDisplay().
*/
class IPFViewportExtension2 : public BaseInterface
{
public:
	IPFViewportExtension2() {}
	virtual ~IPFViewportExtension2() {}

public:
      /*! virtual int PrepareDisplay(IObject* pCont, TimeValue time, Object* pSystem, INode* psNode, INode* pgNode, ViewExp *vpt, int flags);
      *   This provides a default implementation of the virtual function MaxSDK::Graphics::IObjectDisplay::UpdateDisplay(). 
	\param generateRenderItemsContext context used for prepare render items.
      */
	virtual bool PrepareDisplay(
		const MaxSDK::Graphics::GenerateMeshRenderItemsContext& generateRenderItemsContext) 
	{
		UNUSED_PARAM(generateRenderItemsContext);
		return false;
	}

	/*! Generate render items for current particle node.
	\param generateRenderItemsContext context for generate render items
	\param updateDisplayContext The display context, which contains all the requirements for 
								plugin to generate render items.
	\param nodeContext  which contains all infos required for adding per node
						relevant render items.
	\param pCont particle container to draw particles from
	\param pSystem particle system that makes the call
	\param psNode INode of the particle system.
	*/
	virtual bool UpdatePerNodeItems(
		const MaxSDK::Graphics::GenerateMeshRenderItemsContext& generateRenderItemsContext,
		const MaxSDK::Graphics::UpdateDisplayContext& updateDisplayContext,
		MaxSDK::Graphics::UpdateNodeContext& nodeContext,
		MaxSDK::Graphics::IRenderItemContainer& targetRenderItemContainer, 
		IObject*		pCont, 
		Object*			pSystem,
		INode*			psNode)
	{
		UNUSED_PARAM(generateRenderItemsContext);
		UNUSED_PARAM(updateDisplayContext);
		UNUSED_PARAM(nodeContext);
		UNUSED_PARAM(targetRenderItemContainer);
		UNUSED_PARAM(pCont);
		UNUSED_PARAM(pSystem);
		UNUSED_PARAM(psNode);

		return false;
	}
};

PFExport Object* GetPFObject(Object* obj);
// interface ID
#define PFVIEWPORT_INTERFACE Interface_ID(0x1a340cb9, 0x26d12e01) 
#define GetPFViewportInterface(obj) ((IPFViewport*)((GetPFObject(obj))->GetInterface(PFVIEWPORT_INTERFACE))) 

class IPFViewport : public FPMixinInterface
{

public:
      /** @defgroup IPFViewport IPFViewport.h
      *  @{
      */

      /*! \fn virtual int Display(IObject* pCont, TimeValue time, Object* pSystem, INode* psNode, INode* pgNode, ViewExp *vpt, int flags) = 0;
      *  \brief The following methods are redirected from the particle object level to 
	a particular ActionList. The first parameter is a particle container of the ActionList.
	\param pCont: particle container to draw particles from
	\param time: current frame to draw particles at. It's supposed that the container has been already updated to satisfy this moment
	\param pSystem: particle system that makes the call
	\param psNode: INode of the particle system.
	\param pgNode: INode of particle group. Particle system may have several particle groups 	according to number of ActionLists
	\param vpt: viewport as of the BaseObject::Display(...) method
	\param flags: flags as of the BaseObject::Display(...) method
      */
	virtual int Display(IObject* pCont, TimeValue time, Object* pSystem, INode* psNode, INode* pgNode, ViewExp *vpt, int flags) = 0;

      /*! \fn virtual int HitTest(IObject* pCont, TimeValue time, Object* pSystem, INode* psNode, INode* pgNode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt) = 0;
      *  \brief See virtual int Display() 
      */
	virtual int HitTest(IObject* pCont, TimeValue time, Object* pSystem, INode* psNode, INode* pgNode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt) = 0;

      /*! \fn virtual void GetWorldBoundBox(IObject* pCont, TimeValue time, Object* pSystem, INode* pgNode, ViewExp* vp, Box3& box ) = 0;
      *  \brief See virtual int Display() 
      */
	virtual void GetWorldBoundBox(IObject* pCont, TimeValue time, Object* pSystem, INode* pgNode, ViewExp* vp, Box3& box ) = 0;

      /*! \fn virtual void GetLocalBoundBox(IObject* pCont, TimeValue time, Object* pSystem, INode* pgNode, ViewExp* vp, Box3& box ) = 0;
      *  \brief See virtual int Display() 
      */
	virtual void GetLocalBoundBox(IObject* pCont, TimeValue time, Object* pSystem, INode* pgNode, ViewExp* vp, Box3& box ) = 0;

      /*! \fn virtual DWORD GetWireColor() const = 0;
      *  \brief See virtual int Display() 
      */
	virtual DWORD GetWireColor() const = 0;

      /*! \fn virtual void SetWireColor(DWORD color) = 0;
      *  \brief See virtual int Display() 
      */
	virtual void SetWireColor(DWORD color) = 0;
#pragma warning(push)
#pragma warning(disable:4100)
      /*! \fn virtual void MaybeEnlargeViewportRect(IObject* pCont, TimeValue time, GraphicsWindow *gw, Rect &rect) { ; }
      *  \brief See virtual int Display() 
      */
	virtual void MaybeEnlargeViewportRect(IObject* pCont, TimeValue time, GraphicsWindow *gw, Rect &rect) { ; }
#pragma warning(pop)
      /*! \fn FPInterfaceDesc* GetDesc() { return GetDescByID(PFVIEWPORT_INTERFACE); }
      *  \brief 
      */
	FPInterfaceDesc* GetDesc() { return GetDescByID(PFVIEWPORT_INTERFACE); }
};

inline IPFViewport* PFViewportInterface(Object* obj) {
	return ((obj == NULL) ? NULL : GetPFViewportInterface(obj));
};

inline IPFViewport* PFViewportInterface(INode* node) {
	return ((node == NULL) ? NULL : PFViewportInterface(node->GetObjectRef()));
};

