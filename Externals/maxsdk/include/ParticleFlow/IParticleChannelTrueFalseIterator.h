/**********************************************************************
 *<
	FILE: IParticleChannelTrueFalseIterator.h

	DESCRIPTION: Channel-generic interface for boolean type particle channels.
				The interface is used for fast iteration through the subset of
				true (or false) values. 

	CREATED BY:	Oleg Bayborodin

	HISTORY:	created 2007-09-05

 *>	Copyright (c) 2007 Orbaz Technologies, Inc. All Rights Reserved.
 **********************************************************************/

#pragma once

#include "PFExport.h"
#include "IParticleContainer.h"
#include "../ifnpub.h"
#include "../bitarray.h"
// forward declarations
class OrbazTrueBlock;

// generic particle channel "group selection"
// interface ID
#define PARTICLECHANNELTRUEFALSEITERATOR_INTERFACE Interface_ID(0x91cd191c, 0x1eb34500)
#define GetParticleChannelTrueFalseIteratorInterface(obj) ((IParticleChannelTrueFalseIterator*)obj->GetInterface(PARTICLECHANNELTRUEFALSEITERATOR_INTERFACE))


class IParticleChannelTrueFalseIterator : public FPMixinInterface
{
public:

	// resets iterator to first true value of the channel; returns particle index
	// if unable to find a true value then returns number of particles in the channel (end index)
	virtual int	GetFirstTrue(void) = 0;
	// for multi-threaded usage
	virtual int GetFirstTrue(int& localIndex) = 0;

	// finds the next true value; returns particle index
	// if unable to find the next true value then returns number of particles in the channel (end index)
	virtual int	GetNextTrue(void) = 0;
	// for multi-threaded usage
	virtual int GetNextTrue(int& localIndex) const = 0;

	// updates the table of true values and returns number of true indices
	virtual int GetTrueCount(void) = 0;
	// returns index of the i-th true value
	virtual int GetTrueIndex(int i) const = 0;

	// update the table of blocks of true values and returns number of trueBlocks
	virtual int GetTrueBlockCount(void) = 0;
	// returns TrueBlock with given index
	virtual const OrbazTrueBlock& GetTrueBlock(int i) const = 0;

	FPInterfaceDesc* GetDesc() { return GetDescByID(PARTICLECHANNELTRUEFALSEITERATOR_INTERFACE); }
};

class OrbazTrueEnumerator : public BitArrayCallback
{
public:
	OrbazTrueEnumerator(int*& iterIndices)
		: m_iterIndices(iterIndices)
		, m_index(0)
	{
	}

	void proc(int n)
	{
		m_iterIndices[m_index++] = n;
	}
	OrbazTrueEnumerator& operator=(const OrbazTrueEnumerator& rhs)
	{
		if (this != & rhs)
		{
			m_iterIndices = rhs.m_iterIndices;
			m_index       = rhs.m_index;
		}
		return *this;
	}
private:
	int*&		m_iterIndices;
	int			m_index;
};

class PFExport OrbazTrueBlock : public MaxHeapOperators
{
public:	
	OrbazTrueBlock();
	OrbazTrueBlock(const OrbazTrueBlock& block);
	OrbazTrueBlock(int majorIndex, DWORD32 blockIndices);

	inline int GetMajorIndex(void) const { return m_majorIndex; }
	inline void	SetMajorIndex(int index) { m_majorIndex = index; }
	void AddBlockIndex(int index);

	//! returns number of bits set in the TrueBlock
	int NumberSet(void) const;

	//! returns number of trueBlocks allocated and set
	static int SetAllTrueBlocks(int particleCount, OrbazTrueBlock*& trueBlocks);

	static int GetMaxNumTrueBlocks(int particleCount);

	static int OptimizeMemoryUsage(int numBlocksSet, int numBlocksAllocated, OrbazTrueBlock*& trueBlocks);

protected:
	int		m_majorIndex;
	DWORD32 m_blockIndices;

private:
	static const int kBitsPerDWORDMask = 31;
	static const int kBitsPerDWORD = 32;

	class NumberBitsSetInUChar;
};

class PFExport OrbazTrueBlockIterator : public OrbazTrueBlock
{
public:
	OrbazTrueBlockIterator(const OrbazTrueBlock& block);

	// finds the next true value; if used for the first time after constructor finds first true value of the block;
	// return particle index; if unable to find the next true value then returns -1
	int GetNextTrue(void);

	static const int kBitsPerDWORD = 32;

private:
	OrbazTrueBlockIterator(void);

	int m_iter;
	int m_mask;
};

class OrbazTrueBlockEnumerator : public BitArrayCallback
{
public:
	OrbazTrueBlockEnumerator(OrbazTrueBlock*& trueBlocks)
		: m_trueBlocks(trueBlocks)
		, m_blockIndex(-1)
	{
	}

	virtual void proc(int n)
	{
		int curMajorIndex = n & kMajorIndexMask;
		if (m_blockIndex  >= 0)
		{
			if (m_trueBlocks[m_blockIndex].GetMajorIndex() != curMajorIndex)
			{
				m_trueBlocks[++m_blockIndex].SetMajorIndex(curMajorIndex);
			}
		}
		else
		{
			m_blockIndex = 0;
			m_trueBlocks[0].SetMajorIndex(curMajorIndex);
		}
		m_trueBlocks[m_blockIndex].AddBlockIndex(n);
	}

	inline int NumBlocksSet(void)  { return m_blockIndex+1; }
	OrbazTrueBlockEnumerator& operator=(const OrbazTrueBlockEnumerator& rhs)
	{
		if (this != &rhs)
		{
			m_trueBlocks = rhs.m_trueBlocks;
			m_blockIndex = rhs.m_blockIndex;
		}
		return *this;
	}
private:
	OrbazTrueBlock*&	m_trueBlocks;
	int					m_blockIndex;
	static const DWORD	kMajorIndexMask = 0xFFFFFFE0;
};


