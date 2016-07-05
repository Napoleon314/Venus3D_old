////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      vtd
//  File name:   rtti.h
//  Created:     2016/04/01 by Albert D Yang
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

#include "utility.h"

namespace vtd
{
	namespace rtti
	{
		class base
		{
		public:
			typedef size_t size_type;
			typedef const char* str_type;
			typedef std::pair<const base*, size_t> _Base;

			base(str_type _Name) noexcept
				: name(_Name)
			{

			}

			~base() noexcept
			{
				base_vec = nullptr;
				base_num = 0;
			}

			const _Base* begin() const noexcept
			{
				return base_vec;
			}

			const _Base* end() const noexcept
			{
				return base_vec + base_num;
			}

			str_type get_name() const noexcept
			{
				return name;
			}

			size_type get_base_num() const noexcept
			{
				return base_num;
			}

			size_type get_path_from(const base* _From, size_type idx = 0) const noexcept
			{
				if (this == _From)
				{
					return idx;
				}
				else
				{
					for (auto i : *_From)
					{
						size_type _Temp = get_path_from(i.first, idx + i.second);
						if (_Temp != ((size_type)-1))
						{
							return _Temp;
						}
					}
				}
				return ((size_type)-1);
			}

			template <class _Base, class _Der>
			static inline size_type offset() noexcept
			{				
				return (size_type)(void*)static_cast<_Base*>(((_Der*)1)) - 1;
			}

			template<class _Ty1, class _Ty2>
			static inline bool is_exact_kind_of(_Ty2* _Object) noexcept
			{
				return _Object ? _Object->is_exact_kind_of(&_Ty1::_rtti) : false;
			}

			template<class _Ty1, class _Ty2>
			static inline bool is_kind_of(_Ty2* _Object) noexcept
			{
				return _Object ? _Object->is_kind_of(&_Ty1::_rtti) : false;
			}

			template<class _Ty1, class _Ty2>
			static inline _Ty1* _dynamic_cast(_Ty2* _Object) noexcept
			{
				return _Object ? (_Ty1*)_Object->_Ty2::_dynamic_cast(&_Ty1::_rtti) : nullptr;
			}

		protected:
			str_type name;
			_Base* base_vec = nullptr;
			size_type base_num = 0;

		};

		template<class... _Types>
		struct unpacker;

		template<>
		struct unpacker<>
		{
			template<class _Ty>
			static void set(identity<_Ty>, base::_Base* _Desc) noexcept
			{

			}
		};

		template<class _This, class... _Rest>
		struct unpacker<_This, _Rest...>
		{
			template<class _Ty>
			static void set(identity<_Ty> _Id, base::_Base* _Desc) noexcept
			{
				static_assert(std::is_base_of<_This, _Ty>::value,
					"init rtti with wrong base");
				_Desc->first = &_This::_rtti;
				_Desc->second = base::offset<_This, _Ty>();
				unpacker<_Rest...>::set(_Id, _Desc + 1);
			}
		};

		template<class _Der, class... _Bases>
		class rtti : public base
		{
		public:
			rtti(str_type _Name) noexcept
				: base(_Name)
			{
				static_assert(sizeof...(_Bases) > 0, "init rtti with unknown error");
				unpacker<_Bases...>::set(identity<_Der>(), base_array);
				base_vec = base_array;
				base_num = sizeof...(_Bases);
			}

		private:
			_Base base_array[sizeof...(_Bases)];

		};

		template<class _Der, class... _Bases>
		struct select : std::conditional<sizeof...(_Bases) == 0, base, rtti<_Der, _Bases...>>
		{

		};

	}
}

#define vtd_rtti_decl(classname,...) \
	public: \
		static const vtd::rtti::select<classname,##__VA_ARGS__>::type _rtti; \
		virtual const vtd::rtti::base* get_rtti() const noexcept { return &_rtti; } \
		bool is_exact_kind_of(const vtd::rtti::base* _Rtti) const noexcept \
		{ \
			return (get_rtti() == _Rtti); \
		} \
		void* _dynamic_cast(const vtd::rtti::base* _Rtti) const noexcept \
		{ \
			auto _Temp = _Rtti->get_path_from(get_rtti()); \
			if(_Temp == (decltype(_Temp)(-1))) \
			{ \
				return nullptr; \
			} \
			else \
			{ \
				_Temp = (decltype(_Temp))(void*)this + _Temp; \
				_Temp -= classname::_rtti.get_path_from(get_rtti()); \
				return (void*)_Temp; \
			} \
		} \
		bool is_kind_of(const vtd::rtti::base* _Rtti) const noexcept \
		{ \
			return _dynamic_cast(_Rtti) ? true : false; \
		}

#define vtd_rtti_impl(classname,...) \
	const vtd::rtti::select<classname,##__VA_ARGS__>::type classname::_rtti(#classname)

#define vtd_is_exact_kind_of vtd::rtti::base::is_exact_kind_of

#define vtd_is_kind_of vtd::rtti::base::is_exact_kind_of

#define vtd_dynamic_cast vtd::rtti::base::_dynamic_cast
