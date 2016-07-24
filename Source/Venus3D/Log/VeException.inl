////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Log
//  File name:   VeException.inl
//  Created:     2016/07/24 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline const vtd::string& VeException::GetName() noexcept
{
	return m_kName;
}
//--------------------------------------------------------------------------
inline const vtd::string& VeException::GetDescription() noexcept
{
	return m_kDescription;
}
//--------------------------------------------------------------------------
inline const char* VeException::GetTriggerFile() noexcept
{
	return m_pcFile;
}
//--------------------------------------------------------------------------
inline int32_t VeException::GetTriggerLine() noexcept
{
	return m_i32Line;
}
//--------------------------------------------------------------------------
inline const char* VeException::GetTriggerFunction() noexcept
{
	return m_pcFunction;
}
//--------------------------------------------------------------------------
inline void VeException::SetContent(const char* pcName,
	const char* pcDesc) noexcept
{
	m_kName = pcName;
	m_kDescription = pcDesc;
}
//--------------------------------------------------------------------------
inline void VeException::SetTriggerPoint(const char* pcFile,
	int32_t i32Line, const char* pcFunc) noexcept
{
	m_pcFile = pcFile;
	m_i32Line = i32Line;
	m_pcFunction = pcFunc;
}
//--------------------------------------------------------------------------
