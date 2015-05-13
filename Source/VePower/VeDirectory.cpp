////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeDirectory.cpp
//  Created:     2015/03/10 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
bool VeWildNameMatch(const VeChar8* pcName,
	const VeChar8* pcPattern) noexcept
{
	bool bRes;
	while (*pcName)
	{
		if (*pcPattern == '*')
		{
			while ((*pcPattern == '*') || (*pcPattern == '?'))
				++pcPattern;
			if (!*pcPattern)
				return true;
			while (*pcName && (*pcName != *pcPattern))
				++pcName;
			if (!*pcName)
				return false;
			bRes = VeWildNameMatch(pcName, pcPattern);
			while ((!bRes) && (*(pcName + 1)) && (*pcName == *pcPattern))
				bRes = VeWildNameMatch(++pcName, pcPattern);
			return bRes;
		}
		else
		{
			if ((*pcName == *pcPattern) || ('?' == *pcPattern))
			{
				return VeWildNameMatch(++pcName, ++pcPattern);
			}
			else
			{
				return false;
			}
		}
	}
	return (*pcPattern) ? (*pcPattern == '*') &&
		(*(pcPattern + 1) == 0) : true;
}
//--------------------------------------------------------------------------
VeRTTIImpl(VeDirectory);
//--------------------------------------------------------------------------
VeDirectory::VeDirectory() noexcept
{

}
//--------------------------------------------------------------------------
VeDirectory::~VeDirectory() noexcept
{

}
//--------------------------------------------------------------------------
