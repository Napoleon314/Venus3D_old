/*! \file IPFActionListMaterialHolder.h
    \brief ActionList interface for material handling.
*/
/**********************************************************************
 *<
	CREATED BY: Oleg Bayborodin

	HISTORY: created 2008-06-04

 *>	Copyright Orbaz Technologies(c) 2008, All Rights Reserved.
 **********************************************************************/

#pragma once

#include "PFExport.h"
#include "../ifnpub.h"
#include "../object.h"

PFExport Object* GetPFObject(Object* obj);
// interface ID
#define PFACTIONLISTMATERIALHOLDER_INTERFACE Interface_ID(0x97009dcf, 0x1eb34500) 
#define GetPFActionMaterialHolderInterface(obj) ((IPFActionListMaterialHolder*)((GetPFObject(obj))->GetInterface(PFACTIONLISTMATERIALHOLDER_INTERFACE))) 

class IPFActionListMaterialHolder : public FPMixinInterface
{
public:

	// function IDs
	enum {	kIsMaterialHolder,
			kInheritsMaterial,
			kGetMaterial,
			kSetMaterial,
			kGetMaterialPriority,
			kGetStreamDepth,
			kSetStreamDepth,
			kUpdateMaterial,
			kUpdateMaterials,
			kUpdateMaterialsDownstream
	}; 

	// Function Map for Function Publish System 
	//***********************************
	BEGIN_FUNCTION_MAP

	FN_0(kIsMaterialHolder, TYPE_bool, IsMaterialHolder);
	FN_0(kInheritsMaterial, TYPE_bool, InheritsMaterial);
	FN_0(kGetMaterial, TYPE_MTL, GetMaterial);
	FN_1(kSetMaterial, TYPE_bool, SetMaterial, TYPE_MTL);
	FN_0(kGetMaterialPriority, TYPE_INT, GetMaterialPriority);
	FN_0(kGetStreamDepth, TYPE_INT, GetStreamDepth);
	VFN_1(kSetStreamDepth, SetStreamDepth, TYPE_INT);
	FN_1(kUpdateMaterial, TYPE_bool, UpdateMaterial, TYPE_bool);
	FN_1(kUpdateMaterials, TYPE_INT, UpdateMaterials, TYPE_bool);
	FN_1(kUpdateMaterialsDownstream, TYPE_INT, UpdateMaterialsDownstream, TYPE_bool);

	END_FUNCTION_MAP

    	/** @defgroup IPFActionListMaterialHolder IPFActionListMaterialHolder.h
    	*  @{
    	*/

    	/*! \fn virtual bool	IsMaterialHolder() const	{ return false; }
    	*  \brief If the ActionList has an associated material (either via material
	inheritance or from any actions at the action list, then return true
        */
	virtual bool	IsMaterialHolder() const	{ return false; }

    	/*! \fn virtual bool	InheritsMaterial() const	{ return false; }
    	*  \brief Defines whether this action list depends on the upstream
		action lists to generate its material. If the action list has an operator
		with a material, and it does not inherit material then it returns false
        */
	virtual bool	InheritsMaterial() const	{ return false; }

    	/*! \fn virtual bool GenerateInheritedMaterial(void) {}
    	*  \brief Generates material as defined by inheritance
        */
	virtual bool GenerateInheritedMaterial(void) { return false; }


    	/*! \fn virtual Mtl*	GetMaterial()				{ return NULL; }
    	*  \brief Returns a material for particles
        */
	virtual Mtl*	GetMaterial()				{ return NULL; }

    	/*! \fn virtual bool	SetMaterial(Mtl* mtl)		{ return false; }
    	*  \brief Return true if material was set successfully
        */
	virtual bool	SetMaterial(Mtl* mtl)		{  UNUSED_PARAM(mtl); return false; }

	/*! \fn virtual	int		GetMaterialPriority(void) const { return 0; }
	*  \brief Returns material priority number. The priority is used
	when defining material inheritance, for example material inheritance can
	be defined by inheriting material with highest (largest) priority.
	*/
	virtual	int		GetMaterialPriority(void) const { return 0; }

	/*! \fn virtual	int		GetStreamDepth(void) const { return 0; }
	*  \brief Returns stream depth index - shortest distance to pf source up stream
	*/
	virtual	int		GetStreamDepth(void) const { return 0; }

	/*! \fn virtual void	SetStreamDepth(int streamDepth)	{}
	*  \brief Sets stream depth index - shortest distance to pf source up stream
    */
	virtual void	SetStreamDepth(int streamDepth)	{  UNUSED_PARAM(streamDepth); }

	/*! \fn virtual INodeTab& UpStreamActionLists(void) = 0;
	*  \brief Returns Tab of up-stream action list nodes (for read and write)
	*/
	virtual INodeTab& UpStreamActionLists(void) = 0;

	/*! \fn virtual INodeTab& UpStreamMaterialActionLists(void) = 0;
	*  \brief Returns Tab of up-stream action list nodes that are
	relevant for material inheritance re-generation (for read and write)
	*/
	virtual INodeTab& UpStreamMaterialActionLists(void) = 0;

	/*! \fn virtual void SetVisited(bool visited) {}
	*  \brief The method is used to implemented Visitor pattern
	in the actionList graph
	*/
	virtual void SetVisited(bool visited) {  UNUSED_PARAM(visited); }

	/*! \fn virtual bool IsVisited(void) const {}
	*  \brief The method is used to implemented Visitor pattern
	in the actionList graph
	*/
	virtual bool IsVisited(void) const { return false; }

	/*! \fn virtual void SetSortOrder(int index) {}
	*  \brief The method is used to sort action lists to resolve material dependency
	for inheritance
	*/
	virtual void SetSortOrder(int index) {  UNUSED_PARAM(index); }

	/*! \fn virtual int GetSortOrder(void) const { return 0; }
	*  \brief The method is used to sort action lists to resolve material dependency
	for inheritance
	*/
	virtual int GetSortOrder(void) const { return 0; }

	/*! \fn virtual	bool	UpdateMaterial(void) { return false; }
	*  \brief Returns whether the event material was updated.
	*/
	virtual	bool	UpdateMaterial(bool doReport) {  UNUSED_PARAM(doReport); return 0; }

	/*! \fn virtual	int		UpdateMaterials(void) { return 0; }
	*  \brief Returns number of actionLists/events with materials updated.
	*/
	virtual	int		UpdateMaterials(bool doReport) {  UNUSED_PARAM(doReport); return 0; }

	/*! \fn virtual	int		UpdateMaterialsDownstream(void) { return 0; }
	*  \brief Returns number of actionLists/events with materials updated.
	The functions updates materials in all events in the downstream of the current event;
	The function does not update material in the current event; use UpdateMaterial
	function instead for this purpose
	*/
	virtual	int		UpdateMaterialsDownstream(bool doReport) {  UNUSED_PARAM(doReport); return 0; }

    	/*! \fn FPInterfaceDesc* GetDesc() { return GetDescByID(PFACTIONLISTMATERIALHOLDER_INTERFACE); }
    	*  \brief 
      */
	FPInterfaceDesc* GetDesc() { return GetDescByID(PFACTIONLISTMATERIALHOLDER_INTERFACE); }

};

/*! \fn inline IPFActionListMaterialHolder* PFActionListMaterialHolderInterface(Object* obj) 
*  \brief 
*/
inline IPFActionListMaterialHolder* PFActionListMaterialHolderInterface(Object* obj) {
	return ((obj == NULL) ? NULL : GetPFActionMaterialHolderInterface(obj));
};

/*! \fn inline IPFActionListMaterialHolder* PFActionListMaterialHolderInterface(INode* node)
*  \brief 
*/
inline IPFActionListMaterialHolder* PFActionListMaterialHolderInterface(INode* node) {
	return ((node == NULL) ? NULL : PFActionListMaterialHolderInterface(node->GetObjectRef()));
};

/*@}*/


