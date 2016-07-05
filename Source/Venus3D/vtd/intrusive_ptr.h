////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      vtd
//  File name:   intrusive_ptr.h
//  Created:     2016/04/02 by Albert D Yang
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

#include <type_traits>
#include "utility.h"

namespace vtd
{
	class ref_obj
	{
	public:
		ref_obj() noexcept = default;

		virtual ~ref_obj() noexcept = default;

		void inc() noexcept
		{
			++ref_count;
		}

		void dec() noexcept
		{
			--ref_count;
			if (!ref_count)
			{
				delete_this();
			}
		}

		inline size_t get_ref_count() const noexcept
		{
			return ref_count;
		}

	protected:
		virtual void delete_this() noexcept
		{
			delete this;
		}

	private:
		size_t ref_count = 0;
	};

	template <class _Ty>
	struct intrusive_ref_obj
	{
        static_assert(std::is_base_of<ref_obj, typename std::remove_cv<_Ty>::type>::value, "wrong type");

		static void inc(_Ty* ptr) noexcept
		{
			if(ptr) ptr->inc();
		}

		static void dec(_Ty* ptr) noexcept
		{
			if (ptr) ptr->dec();
		}
	};

	template <class _Ty>
	struct intrusive_custom_obj
	{
		static_assert(std::is_class<_Ty>::value, "wrong type");

		static void inc(_Ty* ptr) noexcept
		{
			
		}

		static void dec(_Ty* ptr) noexcept
		{
			if(ptr) delete ptr;
		}
	};

	template <class _Ty>
	struct intrusive_obj : std::conditional<std::is_base_of<ref_obj, typename std::remove_cv<_Ty>::type>::value,
		intrusive_ref_obj<_Ty>, intrusive_custom_obj<_Ty> >::type
	{


	};

	template <class _Ty>
	class intrusive_ptr
	{
	public:
		intrusive_ptr(_Ty* ptr = nullptr) noexcept
			: naked_ptr(ptr)
		{
			intrusive_obj<_Ty>::inc(naked_ptr);
		}

		intrusive_ptr(const intrusive_ptr& lptr) noexcept
		{
			naked_ptr = lptr.naked_ptr;
			intrusive_obj<_Ty>::inc(naked_ptr);
		}

		intrusive_ptr(intrusive_ptr&& rptr) noexcept
		{
			naked_ptr = rptr.naked_ptr;
			rptr.naked_ptr = nullptr;
		}

		~intrusive_ptr() noexcept
		{
			intrusive_obj<_Ty>::dec(naked_ptr);
		}

		template <typename _Other>
		operator intrusive_ptr<_Other>() const noexcept
		{
			return static_cast<_Other*>(naked_ptr);
		}

		operator _Ty* () const noexcept
		{
			return naked_ptr;
		}

		template <typename _Other>
		operator _Other* () const noexcept
		{
			return static_cast<_Other*>(naked_ptr);
		}

		_Ty& operator * () const noexcept
		{
			return *naked_ptr;
		}

		_Ty* operator -> () const noexcept
		{
			return naked_ptr;
		}

		intrusive_ptr& operator = (const intrusive_ptr& lptr) noexcept
		{
			if (naked_ptr != lptr.naked_ptr)
			{
				intrusive_obj<_Ty>::dec(naked_ptr);
				naked_ptr = lptr.naked_ptr;
				intrusive_obj<_Ty>::inc(naked_ptr);
			}
			return *this;
		}

		intrusive_ptr& operator = (intrusive_ptr&& rptr) noexcept
		{
			naked_ptr = rptr.naked_ptr;
			rptr.naked_ptr = nullptr;
			return *this;
		}

		intrusive_ptr& operator = (_Ty* ptr) noexcept
		{
			if (naked_ptr != ptr)
			{
				intrusive_obj<_Ty>::dec(naked_ptr);
				naked_ptr = ptr;
				intrusive_obj<_Ty>::inc(naked_ptr);
			}
			return *this;
		}

		bool operator == (_Ty* ptr) const noexcept
		{
			return (naked_ptr == ptr);
		}

		bool operator != (_Ty* ptr) const noexcept
		{
			return (naked_ptr != ptr);
		}

		bool operator == (const intrusive_ptr& ptr) const noexcept
		{
			return (naked_ptr == ptr.naked_ptr);
		}

		bool operator != (const intrusive_ptr& ptr) const noexcept
		{
			return (naked_ptr != ptr.naked_ptr);
		}

		_Ty* p() noexcept
		{
			return naked_ptr;
		}

		const _Ty* p() const noexcept
		{
			return naked_ptr;
		}

		_Ty& l() noexcept
		{
			return *naked_ptr;
		}

		const _Ty& l() const noexcept
		{
			return *naked_ptr;
		}

		_Ty&& r() noexcept
		{
			return std::move(*naked_ptr);
		}

		const _Ty&& r() const noexcept
		{
			return std::move(*naked_ptr);
		}

		static const intrusive_ptr _null;

	protected:
		_Ty* naked_ptr = nullptr;
	};

	template<class _Ty> const intrusive_ptr<_Ty> intrusive_ptr<_Ty>::_null = nullptr;
}

#define vtd_intrusive_ptr(classname)									\
	class classname;													\
	typedef vtd::intrusive_ptr<classname> classname##_ptr
