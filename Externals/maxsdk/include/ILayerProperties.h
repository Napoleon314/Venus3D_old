/**********************************************************************
 *<
	FILE:			ILayerProperties.h

	DESCRIPTION:	Declare the function published interface to layers
					note: This interface is a facade over disparate property sets on layers:
					
					1) Node-based properties:
					it currently spans the traditional INode properties and the GI properties.
					This is a convenience interface and provides only control of "coarse" properties:
					those access iconically in the layer UI.
					
					2) Additional properties unique to layers: Current.

					3) membership
											 
	CREATED BY:		John Hutchinson

	HISTORY:		created Aug 6, 2001
					add		Aug 9, 2001		add interface for layermanager
					add		Sept 3, 2001	adding properties for color and GIExclusion
					add		July 8, 2013	adding nested layer support

 *>	Copyright (c) 2001, All Rights Reserved.
 **********************************************************************/
#pragma once

#include "ifnpub.h"

// The interface ID for class ILayerProperties
#define LAYERPROPERTIES_INTERFACE Interface_ID(0x202e0798, 0x3589129b)

// Provides access to a layer's properties
class ILayerProperties : public FPMixinInterface
{
public:
	//membership
	virtual	void	addNode	(INode *)	=	0;
	virtual void	select	(BOOL)		=	0;

	//properties
	virtual	BOOL	getCurrent	(void)		=	0;
	//The semantics of setCurrent are special, 
	//You can set it to be current, but can't set it _not_ current
	virtual	void	setCurrent (BOOL)		=	0;

	virtual	BOOL	getOn	(void)		=	0;
	virtual	void	setOn	(BOOL)		=	0;
	virtual	BOOL	getLock	(void)		=	0;
	virtual	void	setLock	(BOOL)		=	0;

	virtual Color	getWireColor(void)		=	0;
	virtual void	setWireColor(Color)		=	0;
	virtual BOOL	getGIIsExcluded()	const	=	0;
	virtual void	setGIIsExcluded(BOOL)		=	0;

	virtual const MCHAR*	getName	(void)		=	0;
	// not a property, per se, since it can fail due to uniqueness of layer names
	virtual BOOL	setName	(const MCHAR *)		=	0;

	/*! Extracts all the nodes for a particular layer. 
		\param [out] layerNodes - a reference to a Tab to be filled with the layers nodes. */
	virtual BOOL	Nodes(Tab<INode*>& layerNodes)	= 0;

	/*! \remarks Returns the parent Layer. This could be NULL meaning the layer is at the top level.*/
	virtual ILayerProperties* getParentLayerProperties() const = 0;

	/*! \remarks Sets the parent layer properties for this layer
	\par Parameters:
	<b>FPInterface* parentLayerProp</b>\n\n
	The layer properties that will become the parent layer.
	\return  Returns TRUE if set; FALSE if not set.
	There are some cases where you cannot set, for example, you cannot set yourself as your parent. 
	Another example is you cannot set one of your childs as your parent (no cycles).*/
	virtual BOOL setParentLayerProperties(FPInterface* parentLayerProp) = 0;

	/*! \remarks Returns a pointer to the Nth child Layer properties for this layer. 
	\par Parameters:
	<b>int n</b>\n\n
	The index of the child layer properties.
	\return  Returns a pointer to the Nth child layer properties. */
	virtual ILayerProperties* getChildLayerProperties(int n) const = 0;

	/*! \remarks Returns the number of child layer properties in this layer.
	\return  The number of child layer properties in this layer. */
	virtual int getNumOfChildLayerProperties() const = 0;
};


#define LAYERMANAGER_INTERFACE Interface_ID(0x77ff1709, 0x19186e96)
// Provides access to the layer manager
class IFPLayerManager : public FPStaticInterface
{
public:
	//access to the layers
	virtual	ILayerProperties*	getLayer	(int which)	=	0;
	virtual	ILayerProperties*	newLayer	(void)		=	0;
	virtual	BOOL	            deleteLayer (const MCHAR*)	=	0;

	//overloads for access by name
	virtual	ILayerProperties*	getLayer	(const MCHAR *)		=	0;
	virtual	ILayerProperties*	newLayer	(const MCHAR *)		=	0;

	//read only properties
	virtual	int		getCount	(void)					=	0;
	virtual ILayerProperties*	getCurrent	(void)		=	0;
};


// The interface ID for class ILayerProperties
#define LAYERDISPLAYPROPERTIES_INTERFACE Interface_ID(0x202e0f11, 0x35892211)

// Provides access to a layer's Max 10 display properties
class ILayerDisplayProperties : public FPMixinInterface
{
public:
	//membership
	/*! \remarks Returns TRUE if the layer is set to never degrade; FALSE if not . */
	virtual	BOOL	getNeverDegrade	(void)		=	0;
	/*! \remarks Sets the never degrade state.
	\param onOff - Use TRUE if the later is to never degrade; FALSE for not. */	
	virtual	void	setNeverDegrade	(BOOL onOff)		=	0;
};


