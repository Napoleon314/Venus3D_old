////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Log
//  File name:   VeException.h
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

#pragma once

class VENUS_API VeException : public VeRefObject
{
	VeNoCopy(VeException);
	VeRTTIDecl(VeException);
public:
	VeException() noexcept = default;

	VeException(const char* pcName, const char* pcDesc,
		const char* pcFile, int32_t i32Line, const char* pcFunc) noexcept;

	inline const vtd::string& GetName() noexcept;

	inline const vtd::string& GetDescription() noexcept;

	inline const char* GetTriggerFile() noexcept;

	inline int32_t GetTriggerLine() noexcept;

	inline const char* GetTriggerFunction() noexcept;

	inline void SetContent(const char* pcName, const char* pcDesc) noexcept;

	inline void SetTriggerPoint(const char* pcFile, int32_t i32Line,
		const char* pcFunc) noexcept;

protected:
	vtd::string m_kName;
	vtd::string m_kDescription;
	const char* m_pcFile = nullptr;
	int32_t m_i32Line = 0;
	const char* m_pcFunction = nullptr;

};

VeSmartPointer(VeException);

template <class _Ex>
void VeThrow(const char* pcFile, int32_t i32Line, const char* pcFunc,
	const char* pcName, const char* pcFormat, ...)
{
	static_assert(std::is_base_of<VeException, _Ex>::value,
		"_Ex has to be a subclass of VeException");
	VeExceptionPtr spException = VE_NEW _Ex();
	assert(spException);
	{
		va_list kArgs;
		va_start(kArgs, pcFormat);
		char acBuffer[VE_MAX_LOG_MESSAGE];
		VeVsprintf(acBuffer, VE_MAX_LOG_MESSAGE, pcFormat, kArgs);
		spException->SetContent(pcName, acBuffer);
		va_end(kArgs);
	}
	spException->SetTriggerPoint(pcFile, i32Line, pcFunc);
	throw spException;
}

#define VE_THROW(...) VeThrow<VeException>(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)
#define VE_THROW_TYPE(t,...) VeThrow<t>(__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)

#define VE_TRY_CALL(f)			\
	try {(f);}					\
	catch(VeExceptionPtr){}

#define VE_TRY(f,e)				\
	try {(f);}					\
	catch(VeExceptionPtr e)

#include "VeException.inl"
