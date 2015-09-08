//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//
#pragma once

#include "AtomicRefCounter.h"
#include "../MaxHeap.h"
#include "../maxtypes.h"

namespace MaxSDK { namespace Graphics
{

/** This class is similar to IUnknown in COM.

 A class that implements IRefObject interface means that an instance 
 of that classes can be shared by multiple owners. Each owner 
 holds a reference to that instance. Once an owner doesn't need that 
 instance, Release() of that instance must be called by the owner and 
 then the reference count of that instance will be decreased by one.

 An instance of such class will be deleted automatically if its 
 reference count is zero.
*/
class IRefObject : public MaxHeapOperators
{
public:
	GraphicsUtilityAPI IRefObject();
	GraphicsUtilityAPI virtual ~IRefObject();

	/** increase reference count of this object by one
	\return increased count
	*/
	virtual long AddRef() = 0;
	/** decrease reference count of this object by one, if the decreased 
	reference count equals zero, the object will be deleted.
	\return decreased count
	*/
	virtual long Release() = 0;
	/** Try query an interface from the ref object using a given interface id.
	\param[in] interfaceID the id of the interface type to query
	\return a pointer to the desired interface if this interface exists or NULL
	otherwise.
	*/
	virtual IRefObject* QueryInterface(Interface_ID interfaceID) = 0;
};

#define AREFOBJECT_INTERFACE_ID Interface_ID(0x4fe71654, 0x25c00a56)

/** This class implemented IRefObject interfaces. 
 concrete class can derive from this class to avoid implementing 
 IRefObject interfaces.
 */
class ARefObject : public IRefObject
{
public:
	GraphicsUtilityAPI ARefObject();

	/** from IRefObject, see comments in IRefObject*/
	GraphicsUtilityAPI virtual long AddRef();
	GraphicsUtilityAPI virtual long Release();
	GraphicsUtilityAPI IRefObject* QueryInterface(Interface_ID classID);

protected:
	/** This function will be called when the reference count of an ARefObject 
	 equals zero. Concrete class may need to override this function because 
	 it may be created on different heaps and need different ways to free its 
	 memory back to system.

	 Default behavior: delete this.
	 */
	GraphicsUtilityAPI virtual void DeleteThis();

	GraphicsUtilityAPI virtual ~ARefObject();

private:
	AtomicRefCounter mRefCount;
};

/** This class is internally used for preventing the AddRef and Release of a 
 pointer returned by RefPtr::operator -> from being called. 

 This trick is used by microsoft CComPtr. see atlcomcli.h:170
 */
template <class RefObjectType> class SafeReferenceObjectProxy : public RefObjectType
{
private:
	SafeReferenceObjectProxy() {}
	virtual ~SafeReferenceObjectProxy() {}

	/** make the following function private. Since the RefPtr::operator -> 
	 returns a SafeReferenceObjectProxy<RefObjectType>* but not a 
	 RefObjectType* therefore user cannot call those functions.
	 */
	virtual long AddRef() = 0;
	virtual long Release() = 0;
};

template <typename RefObjectType> class DefaultLifeTimePolicy : public MaxHeapOperators
{
public:
	static inline void AddRef(RefObjectType* pObject) { pObject->AddRef(); }
	static inline void Release(RefObjectType* pObject) { pObject->Release(); }
};

/**This template class act similar as CComPtr. Using RefPtr<RefObjectType> 
	will be much safer than RefObjectType* since this template class 
	handles the AddRef and Release in its constructor and destructor. 

	Usage Example: 
	class DerivedRefObject : public ARefObject {
	public:
		void foo();
	};

	Use DerivedRefObject without RefPtr: 
	{
		DerivedRefObject* pObject = new DerivedRefObject;
		pObject->AddRef(); // Add reference
		pObject->foo();
		pObject->Release(); // release the object
		pObject = NULL;
	}

	Use DerivedRefObject with RefPtr: 
	{
		RefPtr<DerivedRefObject> pObject = new DerivedRefObject;
		pObject->foo();
		// don't need to worry about deleting this object
	}

	More RefPtr examples: 
	RefPtr<DerivedRefObject> pObject2 = new DerivedRefObject;
	RefPtr<DerivedRefObject> pObject3 = pObject2;
	pObject2 = NULL; // pObject3 is still valid.
	pObject3->foo();
	pObject3 = NULL; // Release the last reference to the object and the 
					 // object is destroyed automatically.
*/
template <
	class RefObjectType, 
	class SafeRefObjectType = SafeReferenceObjectProxy<RefObjectType>, 
	class LifeTimePolicy    = DefaultLifeTimePolicy<RefObjectType>  > 
class RefPtr : public MaxHeapOperators
{
public:

	typedef RefObjectType ObjectType;
	typedef RefPtr<RefObjectType, SafeRefObjectType, LifeTimePolicy> SelfType;

	RefPtr()
	{
		mpObject = NULL;
	}

	RefPtr(RefObjectType* pPtr)
	{
		mpObject = pPtr;
		if (mpObject != NULL)
		{
			LifeTimePolicy::AddRef(mpObject);
		}
	}

	RefPtr(const SelfType& other)
	{
		mpObject = other.mpObject;
		if (mpObject != NULL)
		{
			LifeTimePolicy::AddRef(mpObject);
		}
	}

	template <
		class OtherInterface, 
		class OtherSafeRefObjectType, 
		class OtherLifeTimePolicy> 
	explicit RefPtr(const RefPtr<OtherInterface, OtherSafeRefObjectType, OtherLifeTimePolicy>& other)
	{
		if (other == NULL)
		{
			mpObject = NULL;
			return;
		}
		mpObject = dynamic_cast<RefObjectType*>(other.GetPointer());
		if (NULL != mpObject)
		{
			LifeTimePolicy::AddRef(mpObject);
		}
	}

	template <class OtherInterface> 
	explicit RefPtr(OtherInterface* pPtr)
	{
		if (pPtr == NULL)
		{
			mpObject = NULL;
		}
		else
		{
			mpObject = dynamic_cast<RefObjectType*>(pPtr);
		}
		if (NULL != mpObject)
		{
			LifeTimePolicy::AddRef(mpObject);
		}
	}

	~RefPtr()
	{
		Release();
	}

	void Swap(SelfType& other)
	{
		RefObjectType* pTemp = mpObject;
		mpObject = other.mpObject;
		other.mpObject = pTemp;
	}

	void Release()
	{
		RefObjectType* pTemp = mpObject;
		if (mpObject)
		{
			mpObject = NULL;
			LifeTimePolicy::Release(pTemp);
		}
	}

	operator RefObjectType*() const
	{
		return mpObject;
	}

	RefObjectType& operator*() const
	{
		return *mpObject;
	}

	RefObjectType* GetPointer() const
	{
		return mpObject;
	}

	SafeRefObjectType* operator->() const
	{
		return (SafeRefObjectType*)mpObject;
	}

	RefObjectType* operator=(RefObjectType* pPtr)
	{
		if (pPtr != NULL)
		{
			LifeTimePolicy::AddRef(pPtr);
		}
		if (mpObject)
		{
			LifeTimePolicy::Release(mpObject);
		}
		mpObject = pPtr;
		return pPtr;
	}

	RefObjectType* operator=(const SelfType& other)
	{
		RefObjectType* pPtr = other.mpObject;
		if (pPtr != NULL)
		{
			LifeTimePolicy::AddRef(pPtr);
		}
		if (mpObject)
		{
			LifeTimePolicy::Release(mpObject);
		}
		mpObject = pPtr;
		return pPtr;
	}

	template <class OtherInterface> RefObjectType* operator=(
		OtherInterface* pPtr)
	{
		RefObjectType* pBackup = mpObject;
		if (pPtr != NULL)
		{
			mpObject = dynamic_cast<RefObjectType*>(pPtr);
		}
		else
		{
			mpObject = NULL;
		}
		if (NULL != mpObject)
		{
			LifeTimePolicy::AddRef(mpObject);
		}

		if (pBackup != NULL)
		{
			LifeTimePolicy::Release(pBackup);
		}
		return mpObject;
	}

	template <
		class OtherInterface, 
		class OtherSafeRefObjectType, 
		class OtherLifeTimePolicy> RefObjectType* operator=(
		const RefPtr<OtherInterface, OtherSafeRefObjectType, OtherLifeTimePolicy>& other)
	{
		RefObjectType* pBackup = mpObject;
		if (other != NULL)
		{
			mpObject = dynamic_cast<RefObjectType*>(other.GetPointer());
		}
		else
		{
			mpObject = NULL;
		}
		if (NULL != mpObject)
		{
			LifeTimePolicy::AddRef(mpObject);
		}

		if (pBackup != NULL)
		{
			LifeTimePolicy::Release(pBackup);
		}
		return mpObject;
	}

	bool operator!() const
	{
		return mpObject == NULL;
	}

	bool operator<(RefObjectType* pPtr) const
	{
		return mpObject < pPtr;
	}

	bool operator<(const SelfType& pPtr) const
	{
		return mpObject < pPtr.mpObject;
	}

	bool operator==(RefObjectType* pPtr) const
	{
		return mpObject == pPtr;
	}

	bool operator==(const SelfType& pPtr) const
	{
		return mpObject == pPtr.mpObject;
	}

	bool operator!=(RefObjectType* pPtr) const
	{
		return mpObject != pPtr;
	}

	bool operator!=(const SelfType& pPtr) const
	{
		return mpObject != pPtr.mpObject;
	}

private:
	RefObjectType* mpObject;
};

template <
	class RefObjectType, 
	class SafeRefObjectType, 
	class LifeTimePolicy > 
inline bool operator == (
	void* pPtr, 
	const RefPtr<RefObjectType, SafeRefObjectType, LifeTimePolicy>& pInterface)
{
	return pPtr == pInterface.GetPointer();
}

template <
	class RefObjectType, 
	class SafeRefObjectType, 
	class LifeTimePolicy > 
inline bool operator != (
	void* pPtr, 
	const RefPtr<RefObjectType, SafeRefObjectType, LifeTimePolicy>& pInterface)
{
	return pPtr != pInterface.GetPointer();
}

typedef RefPtr<IRefObject> RefObjectPtr;

}} // namespaces

