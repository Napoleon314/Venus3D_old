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
VeFloat64 VeParser::CalculateExpression(const VeChar8* pcExpr,
	VeFloat64 f64Default) noexcept
{
	return CalculateExpression(m_kValueMap, pcExpr, f64Default);
}
//--------------------------------------------------------------------------
VeFloat64 VeParser::CalculateExpression(const VeStringMap<VeFloat64>& kMap,
	const VeChar8* pcExpr, VeFloat64 f64Default) noexcept
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
			VeFloat64 f64Digit = VeStrtod(pcExpr, &pcNextChar);
			Token kTok;
			kTok.m_eType = TYPE_NUMBER;
			kTok.m_f64Number = f64Digit;
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
			} while (VeIsAlnum(*pcExpr) || (*pcExpr) == '_');
			VeFixedString kStr(pcStr, (VeUInt32)(pcExpr - pcStr));
			auto it = kMap.find(kStr);
			if (it == kMap.end())
			{
				kRpnQueue.clear();
				return f64Default;
			}
			Token kTok;
			kTok.m_eType = TYPE_NUMBER;
			kTok.m_f64Number = it->second;
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
						kTok.m_f64Number = 0;
						kRpnQueue.push_back(kTok);
					}
					else
					{
						kRpnQueue.clear();
						return f64Default;
					}
				}

				while (!kOpStack.empty())
				{
					auto opCur = m_kOpMap.find(VeFixedString(kStr.m_pcStr, VeUInt32(kStr.m_stLen)));
					if (opCur == m_kOpMap.end())
					{
						kRpnQueue.clear();
						return f64Default;
					}
					auto opTop = m_kOpMap.find(VeFixedString(kOpStack.top().m_pcStr, VeUInt32(kOpStack.top().m_stLen)));
					if (opTop == m_kOpMap.end())
					{
						kRpnQueue.clear();
						return f64Default;
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
	VeStack<VeFloat64> kEvaluation;
	while (!kRpnQueue.empty())
	{
		auto& tok = kRpnQueue.front();
		kRpnQueue.pop_front();

		if (tok.m_eType == TYPE_STRING)
		{
			if (kEvaluation.size() < 2)
			{
				return f64Default;
			}
			VeFloat64 f64Right = kEvaluation.pop();
			VeFloat64 f64Left = kEvaluation.pop();
			if (!VeStrncmp(tok.m_kString.m_pcStr, "+", tok.m_kString.m_stLen))
			{
				kEvaluation.push(f64Left + f64Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "*", tok.m_kString.m_stLen))
			{
				kEvaluation.push(f64Left * f64Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "-", tok.m_kString.m_stLen))
			{
				kEvaluation.push(f64Left - f64Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "/", tok.m_kString.m_stLen))
			{
				kEvaluation.push(f64Left / f64Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "<<", tok.m_kString.m_stLen))
			{
				kEvaluation.push((VeUInt32)f64Left << (VeUInt32)f64Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "^", tok.m_kString.m_stLen))
			{
				kEvaluation.push(powf(f64Left, f64Right));
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, ">>", tok.m_kString.m_stLen))
			{
				kEvaluation.push((VeUInt32)f64Left >> (VeUInt32)f64Right);
			}
			else if (!VeStrncmp(tok.m_kString.m_pcStr, "%", tok.m_kString.m_stLen))
			{
				kEvaluation.push((VeUInt32)f64Left % (VeUInt32)f64Right);
			}
			else
			{
				return f64Default;
			}
		}
		else if (tok.m_eType == TYPE_NUMBER)
		{
			kEvaluation.push(tok.m_f64Number);
		}
		else
		{
			return f64Default;
		}
	}
	return kEvaluation.size() ? kEvaluation.top() : f64Default;
}
//--------------------------------------------------------------------------
void VeParser::AddDestructor(std::function<void()> funcDest) noexcept
{
	m_kDestructList.push_back(funcDest);
}
//--------------------------------------------------------------------------
