////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeSmartPointer.h
//  Created:     2015/02/23 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

template <class T>
class VePointer : public VeMemObject
{
public:
	typedef T TypeClass;

	VePointer(T* pObject = nullptr) noexcept
	{
		m_pObject = pObject;
		if (m_pObject)
			m_pObject->IncRefCount();
	}

	VePointer(const VePointer& lptr) noexcept
	{
		m_pObject = lptr.m_pObject;
		if (m_pObject)
			m_pObject->IncRefCount();
	}

	VePointer(VePointer&& rptr) noexcept
	{
		m_pObject = rptr.m_pObject;
		rptr.m_pObject = nullptr;
	}

	~VePointer() noexcept
	{
		if (m_pObject)
			m_pObject->DecRefCount();
	}

	template <typename U>
	operator VePointer<U>() const noexcept
	{
		return static_cast<U*>(m_pObject);
	}

	operator T* () const noexcept
	{
		return m_pObject;
	}

	template <typename _Other>
	operator _Other* () const noexcept
	{
		return static_cast<_Other*>(m_pObject);
	}

	T& operator * () const noexcept
	{
		return *m_pObject;
	}

	T* operator -> () const noexcept
	{
		return m_pObject;
	}

	VePointer& operator = (const VePointer& lptr) noexcept
	{
		if (m_pObject != lptr.m_pObject)
		{
			if (m_pObject)
				m_pObject->DecRefCount();
			m_pObject = lptr.m_pObject;
			if (m_pObject)
				m_pObject->IncRefCount();
		}
		return *this;
	}

	VePointer& operator = (VePointer&& rptr) noexcept
	{
		m_pObject = rptr.m_pObject;
		rptr.m_pObject = nullptr;
		return *this;
	}

	VePointer& operator = (T* pObject) noexcept
	{
		if (m_pObject != pObject)
		{
			if (m_pObject)
				m_pObject->DecRefCount();
			m_pObject = pObject;
			if (m_pObject)
				m_pObject->IncRefCount();
		}
		return *this;
	}

	bool operator == (T* pObject) const noexcept
	{
		return (m_pObject == pObject);
	}

	bool operator != (T* pObject) const noexcept
	{
		return (m_pObject != pObject);
	}

	bool operator == (const VePointer& ptr) const noexcept
	{
		return (m_pObject == ptr.m_pObject);
	}

	bool operator != (const VePointer& ptr) const noexcept
	{
		return (m_pObject != ptr.m_pObject);
	}

	T* p() noexcept
	{
		return m_pObject;
	}

	const T* p() const noexcept
	{
		return m_pObject;
	}

	T& l() noexcept
	{
		VE_ASSERT(m_pObject);
		return *m_pObject;
	}

	const T& l() const noexcept
	{
		VE_ASSERT(m_pObject);
		return *m_pObject;
	}

	T&& r() noexcept
	{
		VE_ASSERT(m_pObject);
		return std::move(*m_pObject);
	}

	const T&& r() const noexcept
	{
		VE_ASSERT(m_pObject);
		return std::move(*m_pObject);
	}

	static const VePointer ms_NULL;

protected:
	T* m_pObject;
};

template<class T> const VePointer<T> VePointer<T>::ms_NULL = nullptr;

#define VeSmartPointer(classname)										\
	class classname;													\
	typedef VePointer<classname> classname##Ptr

#define VeSmartPointerCast(type, smartptr) ((type*)(void*) (smartptr))

VeSmartPointer(VeRefObject);
