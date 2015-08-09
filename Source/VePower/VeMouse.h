////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMouse.h
//  Created:     2015/08/09 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeCursor : public VeRefObject
{
public:
	struct Data
	{
		VeRefNode<Data*> m_kNode;
		VeRefObjectPtr m_spDriverdata;
	};

	VeCursor() noexcept;

	virtual ~VeCursor() noexcept;

protected:
	Data m_kData;

};

struct VeMouseClickState
{
	VeInt32 m_i32LastX;
	VeInt32 m_i32LastY;
	VeUInt32 m_u32LastTimeStamp;
	VeUInt8 m_u8ClickCount;
};


