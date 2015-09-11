////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeLog.h
//  Created:     2015/03/02 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#define VE_LOG_BUFFER_SIZE (2048)

class VE_POWER_API VeLog : public VeRefObject
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

	typedef std::function<void(Type, const VeChar8*, const VeChar8*)> OutputFunc;

	class Shell : public VeMemObject
	{
	public:
		Shell(Type eType, const VeChar8* pcTag, VeLog& kLog) noexcept
			: m_eType(eType), m_pcTag(pcTag), m_kLog(kLog)
		{

		}

		void LogFormat(const VeChar8* pcFormat, ...) noexcept
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
		const VeChar8* m_pcTag;
		VeLog& m_kLog;

	};

	struct Pack : public VeMemObject
	{
		Pack(const VeChar8* pcTag, VeLog& kLog)
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

	void LogFormat(Type eType, const VeChar8* pcTag, const VeChar8* pcFormat, ...) noexcept;

	void LogFormat(Type eType, const VeChar8* pcTag, const VeChar8* pcFormat, va_list kArgs) noexcept;

	template <class... _Params>
	void Log(Type eType, const VeChar8* pcTag, _Params&... _Pak) noexcept
	{
		VeStrLinker<_Params&...>::Append(m_acBuffer, VE_LOG_BUFFER_SIZE, _Pak...);
		Output(eType, pcTag);
	}

	void Begin() noexcept
	{
		m_stPointer = 0;
	}

	template <class _Ty>
	void Append(const _Ty& _Val) noexcept
	{
		m_stPointer += VeTSprintf(m_acBuffer + m_stPointer,
			VE_LOG_BUFFER_SIZE - m_stPointer, _Val);
	}

	void End(Type eType, const VeChar8* pcTag) noexcept
	{
		Output(eType, pcTag);
	}


private:
	void Output(Type eType, const VeChar8* pcTag) noexcept;

	Level m_eLevel = LEVEL_STANDARD;
	OutputFunc m_funcTarget = nullptr;
	VeChar8 m_acBuffer[VE_LOG_BUFFER_SIZE];
	VeSizeT m_stPointer = 0;

};

VeSmartPointer(VeLog);

#include "VeLog.inl"
