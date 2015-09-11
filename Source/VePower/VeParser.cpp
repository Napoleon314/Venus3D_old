////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeParser.cpp
//  Created:     2015/09/11 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
void VeAddParserDestructor(std::function<void()> funcDest) noexcept
{
	ve_parser.AddDestructor(funcDest);
}
//--------------------------------------------------------------------------
VeParser::VeParser() noexcept
{

}
//--------------------------------------------------------------------------
VeParser::~VeParser() noexcept
{
	for (auto func : m_kDestructList)
	{
		func();
	}
	m_kDestructList.clear();
}
//--------------------------------------------------------------------------
void VeParser::AddDestructor(std::function<void()> funcDest) noexcept
{
	m_kDestructList.push_back(funcDest);
}
//--------------------------------------------------------------------------
