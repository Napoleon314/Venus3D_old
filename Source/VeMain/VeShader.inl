////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeShader.inl
//  Created:     2015/08/25 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline VeShader::Type VeShader::GetType(const VeChar8* pcName) noexcept
{
	Type eRes;
	switch (VeToLower(*pcName))
	{
	case 'v':
		eRes = TYPE_VS;
		break;
	case 'p':
		eRes = TYPE_PS;
		break;
	case 'g':
		eRes = TYPE_GS;
		break;
	case 'h':
		eRes = TYPE_HS;
		break;
	case 'd':
		eRes = TYPE_DS;
		break;
	case 'c':
		eRes = TYPE_CS;
		break;
	default:
		return TYPE_NUM;
	}
	if (VeToLower(pcName[1]) == 's' && pcName[2] == 0)
	{
		return eRes;
	}
	return TYPE_NUM;
}
//--------------------------------------------------------------------------
inline const VeChar8* VeShader::GetTypeName(Type eType) noexcept
{
	switch (eType)
	{
	case VeShader::TYPE_VS:
		return "vs";
	case VeShader::TYPE_PS:
		return "ps";
	case VeShader::TYPE_GS:
		return "gs";
	case VeShader::TYPE_HS:
		return "hs";
	case VeShader::TYPE_DS:
		return "ds";
	case VeShader::TYPE_CS:
		return "cs";
	default:
		return "shader";
	}
}
//--------------------------------------------------------------------------
