////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Parser
//  File name:   VeXML.h
//  Created:     2016/08/12 by Albert
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

#include <rapidxml.hpp>

namespace venus
{
	class xml_document;

	class xml_node : public rapidxml::xml_node<char>
	{
	public:
		typedef rapidxml::xml_node<char> super_class;

		xml_node() noexcept = delete;

		xml_document *document() const noexcept
		{
			return (xml_document*)super_class::document();
		}

		venus::xml_node *first_node(const char *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const noexcept
		{
			return (venus::xml_node*)super_class::first_node(name, name_size, case_sensitive);
		}

		venus::xml_node *last_node(const char *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const noexcept
		{
			return (venus::xml_node*)super_class::last_node(name, name_size, case_sensitive);
		}

		venus::xml_node *previous_sibling(const char *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const noexcept
		{
			return (venus::xml_node*)super_class::previous_sibling(name, name_size, case_sensitive);
		}

		venus::xml_node *next_sibling(const char *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const noexcept
		{
			return (venus::xml_node*)super_class::next_sibling(name, name_size, case_sensitive);
		}

		template <class _Ty, bool _Case = true>
		bool transfer_attribute(const char *name, _Ty& _Out) noexcept
		{
			auto attr = first_attribute(name, 0, _Case);
			if (attr)
			{
				const char *value = attr->value();
				if (value)
				{
					return transfer_str(value, _Out);
				}
			}
			return false;
		}

		template <class _Ty, bool _Case = true>
		void transfer_attributes(const char *name, vtd::vector<_Ty>& _Out) noexcept
		{
			_Ty _Temp;
			_Out.clear();
			auto attr = first_attribute(name, 0, _Case);
			while (attr)
			{
				const char *value = attr->value();
				if (value && transfer_str(value, _Temp))
				{
					_Out.push_back(_Temp);
				}
				attr = attr->next_attribute(name, 0, _Case);
			}
		}

		template <class _Ty, bool _Case = true>
		_Ty convert_attribute(const char *name, _Ty _Default) noexcept
		{
			_Ty _Val;
			if (transfer_attribute<_Ty, _Case>(name, _Val))
			{
				return _Val;
			}
			else
			{
				return _Default;
			}
		}
	};

	class xml_document : public rapidxml::xml_document<char>
	{
	public:
		typedef rapidxml::xml_document<char> super_class;

		xml_document() noexcept {}

		void parse(char* text) noexcept
		{
			super_class::parse<0>(text);
		}

		venus::xml_node* root_node() noexcept
		{
			return (venus::xml_node*)super_class::first_node();
		}

		venus::xml_node* first_node(const char *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const noexcept
		{
			return (venus::xml_node*)super_class::first_node(name, name_size, case_sensitive);
		}
	};

}
