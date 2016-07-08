////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeRefObject.h
//  Created:     2016/07/08 by Albert
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

class VENUS_API VeRefObject : public VeMemObject
{
	VeNoCopy(VeRefObject);
public:
	VeRefObject() noexcept;

	virtual ~VeRefObject() noexcept;

	void IncRefCount() noexcept;

	void DecRefCount() noexcept;

	inline uint32_t GetRefCount() const noexcept;

	inline static uint32_t GetTotalObjectCount() noexcept;

protected:
	void DeleteThis() noexcept;

private:
	uint32_t m_u32RefCount = 0;
	static uint32_t ms_u32Objects;

};

namespace vtd
{
	template <class _Ty>
	struct intrusive_ref_obj
	{
		static_assert(std::is_base_of<VeRefObject, typename std::remove_cv<_Ty>::type>::value, "wrong type");

		static void inc(_Ty* ptr) noexcept
		{
			if (ptr) ptr->IncRefCount();
		}

		static void dec(_Ty* ptr) noexcept
		{
			if (ptr) ptr->DecRefCount();
		}
	};

	template <class _Ty>
	struct intrusive_custom_obj
	{
		static_assert(std::is_class<_Ty>::value, "wrong type");

		static void inc(_Ty*) noexcept
		{

		}

		static void dec(_Ty* ptr) noexcept
		{
			if (ptr) delete ptr;
		}
	};

	template <class _Ty>
	struct intrusive_obj : std::conditional<std::is_base_of<VeRefObject, typename std::remove_cv<_Ty>::type>::value,
		intrusive_ref_obj<_Ty>, intrusive_custom_obj<_Ty> >::type
	{


	};
}

#define VeSmartPointer(classname)										\
	class classname;													\
	typedef vtd::intrusive_ptr<classname> classname##Ptr

VeSmartPointer(VeRefObject);

#include "VeRefObject.inl"
