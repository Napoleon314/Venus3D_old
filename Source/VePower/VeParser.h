////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeParser.h
//  Created:     2015/09/11 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

VE_POWER_API void VeAddParserDestructor(std::function<void()> funcDest) noexcept;

template<class _Ty>
class VeEnumParser : public VeSingleton<VeEnumParser<_Ty>>
{
public:
	static_assert(std::is_enum<_Ty>::value, "_Ty has to be a enum");

	VeEnumParser(std::initializer_list < std::pair <
		_Ty, const VeChar8* >> kList) noexcept
	{
		for (auto pair : kList)
		{
			m_kValueMap[pair.first] = pair.second;
			m_kNameMap[pair.second] = pair.first;
		}
		VeAddParserDestructor([]() noexcept
		{
			VeEnumParser<_Ty>::Destory();
		});
	}

	~VeEnumParser() noexcept
	{
		m_kNameMap.clear();
		m_kValueMap.clear();
	}
	
	_Ty ValueToEnum(VeUInt32 u32Value, _Ty eDefault) noexcept
	{
		auto it = m_kValueMap.find(_Ty(u32Value));
		if (it != m_kValueMap.end())
		{
			return it->second;
		}
		return eDefault;
	}

	_Ty ParseEnum(const VeChar8* pcStr, _Ty eDefault) noexcept
	{
		auto it = m_kNameMap.find(pcStr);
		if (it != m_kNameMap.end())
		{
			return it->second;
		}
		return eDefault;
	}

	_Ty ParseFlags(const VeChar8* pcStr, _Ty eStart) noexcept
	{
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeStrcpy(acBuffer, pcStr);
		VeChar8* pcContext;
		VeChar8* pcTemp = VeStrtok(acBuffer, "|", &pcContext);
		while (pcTemp)
		{
			pcTemp = VeTrim(pcTemp);
			auto it = m_kNameMap.find(pcTemp);
			if (it != m_kNameMap.end())
			{
				eStart |= it->second;
			}
			pcTemp = VeStrtok<VeChar8>(nullptr, "|", &pcContext);
		}
		return eStart;
	}

	VeFixedString EnumToStr(_Ty eFlags) noexcept
	{
		auto it = m_kValueMap.find(eFlags);
		if (it != m_kValueMap.end())
		{
			return it->second;
		}
		return "";
	}

	VeFixedString FlagsToStr(_Ty eFlags) noexcept
	{
		VeChar8 acBuffer[VE_MAX_PATH_LEN];
		VeChar8* pcTemp = nullptr;
		for (auto val : m_kValueList)
		{
			if (VE_MASK_HAS_ALL(eFlags, val.first))
			{
				if (pcTemp)
				{
					pcTemp += VeSprintf(pcTemp, VE_MAX_PATH_LEN - (pcTemp - acBuffer),
						"|%s", (const VeChar8*)val.second);
				}
				else
				{
					pcTemp = VeSprintf(acBuffer, (const VeChar8*)val.second);
				}				
			}
		}
		return acBuffer;
	}

protected:
	VeMap<_Ty, VeFixedString> m_kValueMap;
	VeStringMap<_Ty> m_kNameMap;

};

#define VE_ENUM(ty,...) VE_NEW VeEnumParser<ty>(__VA_ARGS__)

class VE_POWER_API VeParser : public VeSingleton<VeParser>
{
	VeNoCopy(VeParser);
public:
	VeParser() noexcept;

	virtual ~VeParser() noexcept;

	template<class _Ty>
	_Ty Parse(const VeChar8* pcStr, _Ty eDefault) noexcept
	{
		return eDefault;
	}

	/*template<class _Ty>
	_Ty ParseEnum(const VeChar8* pcStr, _Ty eDefault) noexcept
	{
		return VeEnumParser::GetSingleton().ParseEnum(pcStr, eDefault);
	}

	template<class _Ty>
	_Ty ParseFlags(const VeChar8* pcStr, _Ty eStart) noexcept
	{
		return VeEnumParser::GetSingleton().ParseFlags(pcStr, eStart);
	}

	template<class _Ty>
	VeFixedString EnumToStr(_Ty eFlags) noexcept
	{
		return VeEnumParser::GetSingleton().EnumToStr(_Ty eFlags);
	}

	template<class _Ty>
	VeFixedString FlagsToStr(_Ty eFlags) noexcept
	{
		return VeEnumParser::GetSingleton().FlagsToStr(_Ty eFlags);
	}*/

	void AddDestructor(std::function<void()> funcDest) noexcept;

protected:
	VeVector<std::function<void()>> m_kDestructList;

};

#define ve_parser VeParser::GetSingleton()
