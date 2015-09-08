/*! \file IPViewItem.h
    \brief Interface for PViewItem. An Action should implement
				 the interface to be modifiable in Particle View.
*/
/**********************************************************************
 *<
	CREATED BY: Oleg Bayborodin

	HISTORY: created 02-21-02

 *>	Copyright (c) 2001, All Rights Reserved.
 **********************************************************************/

#pragma once

#include "PFExport.h"
#include "../object.h"

PFExport Object* GetPFObject(Object* obj);
// interface ID
#define PVIEWITEM_INTERFACE Interface_ID(0x74f93d08, 0x1eb34500) 
#define GetPViewItemInterface(obj) ((IPViewItem*)((GetPFObject(obj))->GetInterface(PVIEWITEM_INTERFACE))) 

// operator/test icon size in particle view
enum {  kActionImageWidth=22, kActionImageHeight=22 }; // in pixels

class IPViewItem : public FPMixinInterface
{
public:
	// the AppData sub-chunk id where the comment string is stored
	static const DWORD PViewItemCommentsChunk = 0x354b7085;

	// function IDs
	enum {	kNumPViewParamBlocks,
			kGetPViewParamBlock,
			kHasComments,
			kGetComments,
			kSetComments,
			kGetWireExtension,
			kSetWireExtension
	}; 

	BEGIN_FUNCTION_MAP
	FN_0(kNumPViewParamBlocks, TYPE_INT, NumPViewParamBlocks );
	FN_1(kGetPViewParamBlock, TYPE_REFTARG, GetPViewParamBlock, TYPE_INDEX );
	FN_1(kHasComments, TYPE_bool, HasComments, TYPE_INODE );
	FN_1(kGetComments, TYPE_STRING, GetComments, TYPE_INODE );
	VFN_2(kSetComments, SetComments, TYPE_INODE, TYPE_STRING );
	FN_2(kGetWireExtension, TYPE_INT, GetWireExtension, TYPE_INODE, TYPE_INT );
	VFN_3(kSetWireExtension, SetWireExtension, TYPE_INODE, TYPE_INT, TYPE_INT );
	END_FUNCTION_MAP

    /** @defgroup IPViewItem IPViewItem.h
    *  @{
    */

	// constructor: inherited as a base class constructor
	IPViewItem() { m_numInstances = 1; }

    /*! \fn virtual int NumPViewParamBlocks() const = 0;
    *  \brief 
    */
	virtual int NumPViewParamBlocks() const = 0;

    /*! \fn virtual IParamBlock2* GetPViewParamBlock(int i) const = 0;
    *  \brief     
    */
	virtual IParamBlock2* GetPViewParamBlock(int i) const = 0;

    /*! \fn PFExport int NumPViewParamMaps() const;
    *  \brief 
    */
	PFExport int NumPViewParamMaps() const;

    /*! \fn PFExport IParamMap2* GetPViewParamMap(int i) const;
    *  \brief 
    */
	PFExport IParamMap2* GetPViewParamMap(int i) const;

    /*! \fn PFExport void AddPViewParamMap(IParamMap2* map);
    *  \brief 
    */
	PFExport void AddPViewParamMap(IParamMap2* map);

    /*! \fn PFExport void RemovePViewParamMap(IParamMap2* map);
    *  \brief 
    */
	PFExport void RemovePViewParamMap(IParamMap2* map);

    /*! \fn bool HasCustomPViewIcons() { return false; }
    *  \brief Implement if you want your operator/test to be shown with a custom icon in ParticleView. 
    */
	virtual bool HasCustomPViewIcons() { return false; }

    /*! \fn bool HasTransparentPViewIcons() const { return false; }
    *  \brief Defines whether the custom icons in Particle View have alpha channels. It is not possible
	    for any specific PFlow action/operator/test to have some icons with alpha, and some without.
		If the method returns true, then the PFlow action/operator/test has to implement all PViewIcon
		related methods below and return a 32-bit HBITMAP with alpha channel.
    */
	virtual bool HasTransparentPViewIcons() const { return false; }

    /*! \fn HBITMAP GetActivePViewIcon() { return NULL; }  
    *  \brief Implement if you want your operator/test to be shown with a custom icon in ParticleView. For operator/test when in active state. 
    */
	virtual HBITMAP GetActivePViewIcon() { return NULL; }  

    /*! \fn HBITMAP GetInactivePViewIcon() { return NULL; }
    *  \brief Implement if you want your operator/test to be shown with a custom icon in ParticleView. For operator when in disabled state.
    */
	virtual HBITMAP GetInactivePViewIcon() { return NULL; }

    /*! \fn HBITMAP GetTruePViewIcon() { return NULL; } 
    *  \brief Implement if you want your operator/test to be shown with a custom icon in ParticleView. For test when in "always true" state.
    */
	virtual HBITMAP GetTruePViewIcon() { return NULL; } 

    /*! \fn HBITMAP GetFalsePViewIcon() { return NULL; } 
    *  \brief Implement if you want your operator/test to be shown with a custom icon in ParticleView. For test when in "always false" state. 
    */
	virtual HBITMAP GetFalsePViewIcon() { return NULL; } 

    /*! \fn bool ShowColorSwatch() const { return false; } 
    *  \brief An operator may have a color swatch UI (color circle on the right side) if it can be used to display particles in viewport.
	    The method is used for the case when the color swatch is variable: visibility of the swatch depends on the parameters of
		the operator. The typical case is 'mP Shape' operator where the particle collision shape is used for debugging purposes only. 
	    The operators Display, Display Script, Display Data don't implement this method since they always show their color swatch.
    */
	virtual bool ShowColorSwatch() const { return false; }

    /*! \fn PFExport int GetNumInstances() const;
    *  \brief Used to track instanced actions. An action may have several instances, which PView shows in italic font.
    */
	PFExport int GetNumInstances() const;

    /*! \fn PFExport void SetNumInstances(int num);
    *  \brief Used to track instanced actions. An action may have several instances, which PView shows in italic font.
    */
	PFExport void SetNumInstances(int num);

    /*! \fn PFExport void IncNumInstances();
    *  \brief Used to track instanced actions. An action may have several instances, which PView shows in italic font.
    */
	PFExport void IncNumInstances();

    /*! \fn PFExport bool HasComments(INode* itemNode) const;
    *  \brief A PView item can have comments.
	 Comments are also shown and edited via right-click above the PView item.
    */
	PFExport bool HasComments(INode* itemNode) const;

    /*! \fn PFExport MSTR GetComments(INode* itemNode) const;
    *  \brief See HasComments().
    */
	PFExport const MCHAR* GetComments(INode* itemNode) const;

    /*! \fn PFExport void SetComments(INode* itemNode, const MCHAR* comments);
    *  \brief See HasComments().
    */
	PFExport void SetComments(INode* itemNode, const MCHAR* comments);

    /*! \fn PFExport void EditComments(INode* itemNode, HWND parentWnd, long x, long y);
    *  \brief See HasComments().
    */
	PFExport void EditComments(INode* itemNode, HWND parentWnd, long x, long y);

    /*! \fn PFExport LRESULT CALLBACK commentsProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
    *  \brief For internal use
    */
	PFExport LRESULT CALLBACK commentsProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

    /*! \fn virtual bool HasDynamicName(MSTR& nameSuffix) { return false; }
    *  \brief For dynamic names support; if the action has a dynamic name then it supplies the name suffix
	*/
	virtual bool HasDynamicName(MSTR& nameSuffix) { UNUSED_PARAM(nameSuffix); return false; }
	
    /*! \fn PFExport static int GetWireExtension(INode* node, int& wireHeight);
    *  \brief To read/modify the length/height of the wire nub extended from an emitter, or an action list, or a test.
	 You must supply the node of the item. This method returns wire length.
    */
	PFExport static int GetWireExtension(INode* node, int& wireHeight);

    /*! \fn PFExport static void SetWireExtension(INode* node, int wireLen, int wireHight);
    *  \brief Sets wire length. See GetWireExtension().
    */
	PFExport static void SetWireExtension(INode* node, int wireLen, int wireHight);

    /*! \fn FPInterfaceDesc* GetDesc() { return GetDescByID(PVIEWITEM_INTERFACE); }
    *  \brief 
    */
	FPInterfaceDesc* GetDesc() { return GetDescByID(PVIEWITEM_INTERFACE); }

/*@}*/

private:
	void add(INode* itemNode, HWND hWnd);
	INode* node(HWND hWnd);
	HWND hWnd(INode* itemNode);
	void erase(HWND hWnd);

private:
	static void UpdateAppDataLoadProc(Animatable *anim, const Class_ID& cid, SClass_ID sid, ILoad* iload, Tab<DWORD> &subIDs);
	static bool bAppDataLoadCallbackRegistered;

protected:
	Tab<IParamMap2*> m_paramMaps;
	int m_numInstances;
	Tab<INode*> m_nodes;
	Tab<HWND> m_hWnds;
};

inline IPViewItem* PViewItemInterface(Object* obj) {
	return ((obj == NULL) ? NULL : GetPViewItemInterface(obj));
};

inline IPViewItem* PViewItemInterface(INode* node) {
	return ((node == NULL) ? NULL : PViewItemInterface(node->GetObjectRef()));
};


