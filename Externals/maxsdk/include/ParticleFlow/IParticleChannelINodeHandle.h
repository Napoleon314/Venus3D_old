/*! \file IParticleChannelINodeHandle.h
    \brief Channel-generic interface for particle channels
				 that store INode handle data (ULONGs)
				 The channel can be global (all particles have the same 
				 INode handle) and the most common case - shared 
				 (particles are sharing a limited set of INode handles)
*/
/**********************************************************************
 *<
	CREATED BY: Oleg Bayborodin

	HISTORY: created 2010-03-27

 *>	Copyright (c) 2010, All Rights Reserved.
 **********************************************************************/

#pragma once

#include "..\ifnpub.h"

// standard particle channel "INode Handle"
// interface ID
#define PARTICLECHANNELINODEHANDLER_INTERFACE Interface_ID(0x5122ced7, 0x1eb34500)
#define PARTICLECHANNELINODEHANDLEW_INTERFACE Interface_ID(0x5122ced7, 0x1eb34501)

// since it's a "type" channel there is no "GetChannel" defines
//#define GetParticleChannelINodeHandleRInterface(obj) ((IParticleChannelINodeHandleR*)obj->GetInterface(PARTICLECHANNELINODEHANDLER_INTERFACE))
//#define GetParticleChannelINodeHandleWInterface(obj) ((IParticleChannelINodeHandleW*)obj->GetInterface(PARTICLECHANNELINODEHANDLEW_INTERFACE))


class IParticleChannelINodeHandleR : public FPMixinInterface
{
public:

	// function IDs Read
	enum {	kIsShared,
			kGetValueCount,
			kGetValueIndex,
			kGetValueByIndex,
			kGetValue,
			kGetValueFirst,
	};

	BEGIN_FUNCTION_MAP

	FN_0(kGetValueCount, TYPE_INT, GetValueCount);
	FN_1(kGetValueIndex, TYPE_INT, GetValueIndex, TYPE_INT);
	FN_1(kGetValueByIndex, TYPE_DWORD, GetValueByIndex, TYPE_INT);
	FN_1(kGetValue, TYPE_DWORD, GetValue, TYPE_INT);
	FN_0(kGetValueFirst, TYPE_DWORD, GetValue);

	END_FUNCTION_MAP

	// get total number of actual handles (values) in the channel
	virtual int			GetValueCount() const = 0;
	// get the value index of a particle
	virtual int			GetValueIndex(int particleIndex) const = 0;
	// get handle of the valueIndex-th value
	virtual ULONG GetValueByIndex(int valueIndex) const = 0;
	// get handle for particle with index
	virtual ULONG	GetValue(int particleIndex) const = 0;
	// get global handle for all particles
	// the method returns the handle of the first particle if it is local or shared
	virtual ULONG	GetValue() const = 0;

	FPInterfaceDesc* GetDesc() { return GetDescByID(PARTICLECHANNELINODEHANDLER_INTERFACE); }
};

class IParticleChannelINodeHandleW : public FPMixinInterface
{
public:

	// function IDs Write
	enum {	kSetValue,
			kSetValueMany,
			kSetValueAll,
			kCopyValue,
			kCopyValueMany,
			kCopyValueAll
	};

	BEGIN_FUNCTION_MAP

	FN_2(kSetValue, TYPE_bool, SetValue, TYPE_INT, TYPE_DWORD);
	FN_2(kSetValueMany, TYPE_bool, SetValue, TYPE_INT_TAB_BR, TYPE_DWORD);
	FN_1(kSetValueAll, TYPE_bool, SetValue, TYPE_DWORD);
	FN_2(kCopyValue, TYPE_bool, CopyValue, TYPE_INT, TYPE_INT);
	FN_2(kCopyValueMany, TYPE_bool, CopyValue, TYPE_INT, TYPE_INT_TAB_BR);
	FN_1(kCopyValueAll, TYPE_bool, CopyValue, TYPE_INT);

	END_FUNCTION_MAP

	// copies handle to be a local value for particle with index "particleIndex"
	// returns true if successful
	virtual bool	SetValue(int particleIndex, ULONG handle) = 0;
	// copies handle to be a shared value with indices in "particleIndices"
	// returns true if successful
	virtual bool	SetValue(Tab<int>& particleIndices, ULONG handle) = 0;
	// copies handle to be a global values for all particles
	// returns true if successful
	virtual bool	SetValue(ULONG handle) = 0;
	// copy handle value from fromParticle to toParticle
	virtual bool	CopyValue(int fromParticle, int toParticle) = 0;
	// copy handle value from fromParticle to toParticles
	virtual bool	CopyValue(int fromParticle, Tab<int>& toParticles) = 0;
	// copy handle value from fromParticle to all particles
	virtual bool	CopyValue(int fromParticle) = 0;

	FPInterfaceDesc* GetDesc() { return GetDescByID(PARTICLECHANNELINODEHANDLEW_INTERFACE); }
};
