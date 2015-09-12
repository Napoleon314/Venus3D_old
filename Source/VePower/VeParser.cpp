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
	m_kOpMap["("] = -1;
	m_kOpMap["<<"] = 1;
	m_kOpMap[">>"] = 1;
	m_kOpMap["+"] = 2;
	m_kOpMap["-"] = 2;
	m_kOpMap["*"] = 3;
	m_kOpMap["/"] = 3;
	m_kOpMap["%"] = 3;
	m_kOpMap["^"] = 4;
}
//--------------------------------------------------------------------------
VeParser::~VeParser() noexcept
{
	for (auto func : m_kDestructList)
	{
		func();
	}
	m_kDestructList.clear();
	m_kOpMap.clear();
	m_kValueMap.clear();
}
//--------------------------------------------------------------------------
VeFloat32 VeParser::CalculateExpression(const VeChar8* pcExpr) noexcept
{
	VeList<VeParser::Token> kRpnQueue;
	kRpnQueue.clear();
	VeStack<TokenStr> kOpStack;
	bool bLastTokenWasOp = true;
	while (*pcExpr && VeIsSpace(*pcExpr)) ++pcExpr;
	while (*pcExpr)
	{
		if (VeIsDigit(*pcExpr))
		{
			VeChar8* pcNextChar = 0;
			VeFloat32 f32Digit = VeStrtof(pcExpr, &pcNextChar);
			Token kTok;
			kTok.m_eType = TYPE_NUMBER;
			kTok.m_f32Number = f32Digit;
			kRpnQueue.push_back(kTok);
			pcExpr = pcNextChar;
			bLastTokenWasOp = false;
		}
		else if (VeIsAlpha(*pcExpr) || (*pcExpr) == '_')
		{
			const VeChar8* pcStr = pcExpr;
			do
			{
				++pcExpr;
			}
			while (VeIsAlnum(*pcExpr) || (*pcExpr) == '_');
			VeFixedString kStr(pcStr, (VeUInt32)(pcExpr - pcStr));
			auto it = m_kValueMap.find(kStr);
			if (it == m_kValueMap.end())
			{
				kRpnQueue.clear();
				return 0;
			}
			Token kTok;
			kTok.m_eType = TYPE_NUMBER;
			kTok.m_f32Number = it->second;
			kRpnQueue.push_back(kTok);
			bLastTokenWasOp = false;
		}
		else
		{
			switch (*pcExpr)
			{
			case '(':
			{
				kOpStack.push({ "(", 1 });
				++pcExpr;
			}				
			break;
			case ')':
			{
				while ((*kOpStack.top().m_pcStr) != '(')
				{
					Token kTok;
					kTok.m_eType = TYPE_STRING;
					kTok.m_kString = kOpStack.pop();
					kRpnQueue.push_back(kTok);
				}
				kOpStack.pop();
				++pcExpr;				
			}
			break;
			default:
			{
				const VeChar8* pcStr = pcExpr;
				do
				{
					++pcExpr;
				} while ((*pcExpr) && (!VeIsSpace(*pcExpr)) && (!VeIsAlnum(*pcExpr))
					&& (*pcExpr) != '_' && (*pcExpr) != '(' && (*pcExpr) != ')');
				TokenStr kStr = { pcStr, VeSizeT(pcExpr - pcStr) };
				if (bLastTokenWasOp)
				{
					if (*pcStr == '-' || *pcStr == '+')
					{
						Token kTok;
						kTok.m_eType = TYPE_NUMBER;
						kTok.m_f32Number = 0;
						kRpnQueue.push_back(kTok);
					}
					else
					{
						kRpnQueue.clear();
						return 0;
					}
				}
				
				while (!kOpStack.empty())
				{
					auto opCur = m_kOpMap.find(VeFixedString(kStr.m_pcStr, VeUInt32(kStr.m_stLen)));
					if (opCur == m_kOpMap.end())
					{
						kRpnQueue.clear();
						return 0;
					}
					auto opTop = m_kOpMap.find(VeFixedString(kOpStack.top().m_pcStr, VeUInt32(kOpStack.top().m_stLen)));
					if (opTop == m_kOpMap.end())
					{
						kRpnQueue.clear();
						return 0;
					}
					if (opCur->second > opTop->second)
					{
						break;
					}
					Token kTok;
					kTok.m_eType = TYPE_STRING;
					kTok.m_kString = kOpStack.pop();
					kRpnQueue.push_back(kTok);
				}
				kOpStack.push(kStr);
				bLastTokenWasOp = true;				
			}
			break;
			}
		}
		while (*pcExpr && VeIsSpace(*pcExpr)) ++pcExpr;
	}
	while (!kOpStack.empty())
	{
		Token kTok;
		kTok.m_eType = TYPE_STRING;
		kTok.m_kString = kOpStack.pop();
		kRpnQueue.push_back(kTok);		
	}
	VeStack<VeFloat32> kEvaluation;
	while (!kRpnQueue.empty())
	{
		auto& tok = kRpnQueue.front();
		kRpnQueue.pop_front();

		if (tok.m_eType == TYPE_STRING)
		{
			if (kEvaluation.size() < 2)
			{
				return 0;
			}
			VeFloat32 f32Right = kEvaluation.pop();
			VeFloat32 f32Left = kEvaluation.pop();
			if (!VeStrncmp(tok.m_kString.m_pcStr, "+", tok.m_kString.m_stLen))
			{
				kEvaluation.push(f32Left + f32Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "*", tok.m_kString.m_stLen))
			{
				kEvaluation.push(f32Left * f32Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "-", tok.m_kString.m_stLen))
			{
				kEvaluation.push(f32Left - f32Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "/", tok.m_kString.m_stLen))
			{
				kEvaluation.push(f32Left / f32Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "<<", tok.m_kString.m_stLen))
			{
				kEvaluation.push((VeUInt32)f32Left << (VeUInt32)f32Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "^", tok.m_kString.m_stLen))
			{
				kEvaluation.push(powf(f32Left, f32Right));
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, ">>", tok.m_kString.m_stLen))
			{
				kEvaluation.push((VeUInt32)f32Left >> (VeUInt32)f32Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "%", tok.m_kString.m_stLen))
			{
				kEvaluation.push((VeUInt32)f32Left % (VeUInt32)f32Right);
			}
			else
			{
				return 0;
			}
		}
		else if (tok.m_eType == TYPE_NUMBER)
		{
			kEvaluation.push(tok.m_f32Number);
		}
		else
		{
			return 0;
		}		
	}
	return kEvaluation.size() ? kEvaluation.top() : 0;
}
//--------------------------------------------------------------------------
void VeParser::AddDestructor(std::function<void()> funcDest) noexcept
{
	m_kDestructList.push_back(funcDest);
}
//--------------------------------------------------------------------------
