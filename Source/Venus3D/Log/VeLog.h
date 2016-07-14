////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Log
//  File name:   VeLog.h
//  Created:     2016/07/14 by Albert
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

#define VE_MAX_LOG_MESSAGE (4096)

class VENUS_API VeLog : public VeRefObject
{
public:
	enum Type
	{
		TYPE_DEBUG,
		TYPE_INFO,
		TYPE_WARN,
		TYPE_ERROR,
		TYPE_MAX
	};

	enum Level
	{
		LEVEL_DEVELOP,
		LEVEL_STANDARD,
		LEVEL_SHIPPING,
		LEVEL_MAX
	};

	typedef std::function<void(Type, const char*, const char*)> OutputFunc;

	class Shell : public VeMemObject
	{
	public:
		Shell(Type eType, const char* pcTag, VeLog& kLog) noexcept
			: m_eType(eType), m_pcTag(pcTag), m_kLog(kLog)
		{

		}

		void LogFormat(const char* pcFormat, ...) noexcept
		{
			va_list kArgs;
			va_start(kArgs, pcFormat);
			m_kLog.LogFormat(m_eType, m_pcTag, pcFormat, kArgs);
			va_end(kArgs);
		}

		template <class... _Params>
		void Log(_Params... _Pak) noexcept
		{
			m_kLog.Log<_Params...>(m_eType, m_pcTag, _Pak...);
		}


	protected:
		Type m_eType;
		const char* m_pcTag;
		VeLog& m_kLog;

	};

	struct Pack : public VeMemObject
	{
		Pack(const char* pcTag, VeLog& kLog)
			: D(TYPE_DEBUG, pcTag, kLog)
			, I(TYPE_INFO, pcTag, kLog)
			, W(TYPE_WARN, pcTag, kLog)
			, E(TYPE_ERROR, pcTag, kLog)
		{

		}

		Shell D;
		Shell I;
		Shell W;
		Shell E;
	};

	VeLog();

	virtual ~VeLog();

	inline void SetTarget(OutputFunc funcTarget) noexcept;

	inline void SetLevel(Level eLevel) noexcept;

	inline Level GetLevel() const noexcept;

	void LogFormat(Type eType, const char* pcTag, const char* pcFormat, ...) noexcept;

	void LogFormat(Type eType, const char* pcTag, const char* pcFormat, va_list kArgs) noexcept;

	template <class... _Params>
	void Log(Type eType, const char* pcTag, _Params&... _Pak) noexcept
	{
		char acBuffer[VE_MAX_LOG_MESSAGE];
		VeStrLinker<_Params&...>::Append(acBuffer, VE_MAX_LOG_MESSAGE, _Pak...);
		Output(eType, pcTag, acBuffer);
	}

	static void ConsoleOutput(VeLog::Type eType, const char* pcTag,
		const char* pcText) noexcept;

private:
	void Output(Type eType, const char* pcTag, const char* pcContent) noexcept;

	Level m_eLevel = LEVEL_STANDARD;
	OutputFunc m_funcTarget = nullptr;
	size_t m_stPointer = 0;

};

#include "VeLog.inl"
