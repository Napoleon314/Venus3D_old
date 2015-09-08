//
// Copyright 2013 Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
//
//

#pragma once

#include "../IRefObject.h"
#include "FragmentGraphBase.h"
#include "IVariableDictionary.h"

namespace MaxSDK { namespace Graphics {
	
	/** built-in variable types. For more details please see class Variable.
	*/
	enum VarType
	{
		/** A single char type. 
		*/
		VT_Char, 
		/** unsigned char type.
		*/
		VT_UnsignedChar, 
		/** wchar_t type.
		*/
		VT_WideChar, 
		/** short type.
		*/
		VT_Short, 
		/** unsigned short type.
		*/
		VT_UnsignedShort, 
		/** int type.
		*/
		VT_Int, 
		/** unsigned int type.
		*/
		VT_UnsignedInt, 
		/** long type.
		*/
		VT_Long, 
		/** unsigned long type.
		*/
		VT_UnsignedLong, 
		/** __int64 type.
		*/
		VT_Int64, 
		/** unsigned __int64 type.
		*/
		VT_UnsignedInt64, 
		/** float type.
		*/
		VT_Float, 
		/** double type.
		*/
		VT_Double, 
		/** Ascii string type.
		*/
		VT_StringA, 
		/** Unicode string type.
		*/
		VT_StringW, 
		/** dictionary type. If a variable is this type, it is actually a map. 
		The key is a multi-byte string, while the value is another variable.
		*/
		VT_Dictionary, 
		/** A IRefObject type. The variable will hold a reference of that object.
		*/
		VT_IRefObject, 
	};
	
	/** Checks if the given type is number type.
	\param[in] type type value.
	\return true if the type is a number type, (for example, char, wide char, int, double),
	false otherwise. 
	*/
	FragmentGraphAPI bool IsNumberType(VarType type);

	/** Checks if two types can be converted to each other. 
	\param[in] lhs The first type.
	\param[in] rhs The second type.
	\return true if two types are equal or both are numbers, false otherwise.
	*/
	FragmentGraphAPI bool IsCompatibleType(VarType lhs, VarType rhs);

	/** Variable is a simple class that stores certain data that a developer can 
	decide at runtime. We use this class to transfer parameters from fragment to 
	fragment.
	*/
	class Variable : public MaxHeapOperators
	{
	public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifndef DISABLE_UNIT_TESTS
		// for unit test only
		static size_t GetNetStringCount();
#endif
#endif
		/** Default constructor. The default type of the variable is int and 
		the default value is zero.
		*/
		FragmentGraphAPI Variable();
		/** Constructs a char-type variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(char iValue);
		/** Constructs an unsigned char variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(unsigned char iValue);
		/** Constructs a wchar_t variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(wchar_t iValue);
		/** Constructs a short variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(short iValue);
		/** Constructs an unsigned short variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(unsigned short iValue);
		/** Constructs an int variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(int iValue);
		/** Constructs an unsigned int variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(unsigned int iValue);
		/** Constructs a long variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(long iValue);
		/** Constructs an unsigned long variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(unsigned long iValue);
		/** Constructs a __int64 variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(__int64 iValue);
		/** Constructs an unsigned __int64 variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(unsigned __int64 iValue);
		/** Constructs a float variable with the given value.
		\param[in] fValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(float fValue);
		/** Constructs a double variable with the given value.
		\param[in] fValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(double fValue);
		/** Constructs a multi-byte string variable with the given value.
		\param[in] str The initial value of this variable. Cannot be NULL.
		*/
		FragmentGraphAPI Variable(const char* str);
		/** Constructs a Unicode string variable with the given value.
		\param[in] str The initial value of this variable. Cannot be NULL.
		*/
		FragmentGraphAPI Variable(const wchar_t* str);
		/** Constructs an IRefObject auto-ptr variable with the given value.
		\param[in] pRefObject The initial value of this variable. Can be NULL.
		*/
		FragmentGraphAPI Variable(MaxSDK::Graphics::IRefObject* pRefObject);
		/** Copy constructor.
		\param[in] from The default value of this variable. 
		*/
		FragmentGraphAPI Variable(const Variable& from);
		/** Destructor.
		*/
		FragmentGraphAPI ~Variable();

		/** Changes this variable to a certain type, and resets the value. 
		For numbers, resets to zero.
		For string, resets to empty string.
		For RefObject, resets to NULL.
		\param[in] type The type that this variable will be changed to.
		*/
		FragmentGraphAPI void ChangeType(VarType type);

		/** Gets the type of this variable.
		\return the type of this variable.
		*/
		VarType GetType() const
		{
			return mType;
		}

	public:
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return A char value.
		*/
		FragmentGraphAPI char AsChar() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return An unsigned char value.
		*/
		FragmentGraphAPI unsigned char AsUnsignedChar() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return A wchar_t value.
		*/
		FragmentGraphAPI wchar_t AsWideChar() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return A short value.
		*/
		FragmentGraphAPI short AsShort() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return An unsigned short value.
		*/
		FragmentGraphAPI unsigned short AsUnsignedShort() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return An int value.
		*/
		FragmentGraphAPI int AsInt() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return An unsigned int value.
		*/
		FragmentGraphAPI unsigned int AsUnsignedInt() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return A long value.
		*/
		FragmentGraphAPI long AsLong() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return An unsigned long value.
		*/
		FragmentGraphAPI unsigned long AsUnsignedLong() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return An __int64 value.
		*/
		FragmentGraphAPI __int64 AsInt64() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return An unsigned __int64 value.
		*/
		FragmentGraphAPI unsigned __int64 AsUnsignedInt64() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return A float value.
		*/
		FragmentGraphAPI float AsFloat() const;
		/** Tries to convert this variable to char and return the value.
		If this variable is not a number, an exception is thrown.
		\return A double value.
		*/
		FragmentGraphAPI double AsDouble() const;
		/** Tries to get the multi-byte string value of this variable. If this 
		variable is not a multi-byte string, an exception is thrown.
		\return multi-byte string pointer. It's life time is controlled by the 
		variable, you don't need to delete it.
		*/
		FragmentGraphAPI const char* AsStringA() const;
		/** Tries to get the Unicode string value of this variable. If this 
		variable is not a Unicode string, an exception is thrown.
		\return Unicode string pointer. It's life time is controlled by the 
		variable, you don't need to delete it.
		*/
		FragmentGraphAPI const wchar_t* AsStringW() const;
		/** Tries to get the IRefObject pointer that is referenced by this variable. 
		If this variable is not an IRefObject variable, an exception is 
		thrown.
		\return This function directly returns the pointer to the IRefObject. It 
		doesn't increase the reference count of that IRefObject.
		*/
		FragmentGraphAPI MaxSDK::Graphics::IRefObject* AsRefObject() const;
		/** Tries to get the dictionary pointer that is referenced by this variable. 
		If this variable is not a dictionary variable, an exception is 
		thrown.
		\return This function directly returns the pointer to the dictionary. It's 
		life time is controlled by the variable, developer don't need to delete it.
		*/
		FragmentGraphAPI IVariableDictionary* AsDictionary() const;

		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(char iValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(unsigned char iValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(wchar_t iValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(short iValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(unsigned short iValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(int iValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(unsigned int iValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(long iValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(unsigned long iValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(__int64 iValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(unsigned __int64 iValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] fValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(float fValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] fValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(double fValue);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] str input value.
		\return true if the variable is a multi-byte string and is successfully 
		updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(const char* str);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] str input value.
		\return true if the variable is an unicode string and is successfully 
		updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(const wchar_t* str);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] pRefObject input value.
		\return true if the variable is an IRefObject pointer and is successfully 
		updated, false otherwise. If it succeeds, this function tries to release 
		the old IRefObject that is referenced by the variable and increase the reference 
		count of the input value.
		*/
		FragmentGraphAPI bool TryUpdateValue(MaxSDK::Graphics::IRefObject* pRefObject);
		/** Tries to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, returns false.
		\param[in] var input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(const Variable& var);

		/** Converts this variable to the input value type, then assigns
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (char rhs);
		/** convert this variable to the input value type, then assigns
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (unsigned char rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (wchar_t rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (short rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (unsigned short rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (int rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (unsigned int rhs);
		/** convert this variable to the input value type, then assigns
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (long rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (unsigned long rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (__int64 rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (unsigned __int64 rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (float rhs);
		/** convert this variable to the input value type, then assigns
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (double rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (const char* rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (const wchar_t* rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] rhs input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (MaxSDK::Graphics::IRefObject* rhs);
		/** convert this variable to the input value type, then assigns 
		the input value to this variable.
		\param[in] var input value.
		\return a reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (const Variable& var);

	private:

		void Release();

		void CleanMembers(VarType type)
		{
			mType = type;
			mValue.mUnsignedInt64 = 0;
		}

		VarType mType;
		union ValueStorage
		{
			char mChar;
			unsigned char mUnsignedChar;
			wchar_t mWideChar;
			short mShort;
			unsigned mUnsignedShort;
			int mInt;
			unsigned int mUnsignedInt;
			long mLong;
			unsigned long mUnsignedLong;
			__int64 mInt64;
			unsigned __int64 mUnsignedInt64;
			float mFloat;
			double mDouble;
			char* mpStringA;
			wchar_t* mpStringW;
			MaxSDK::Graphics::IRefObject* mpRefObject;
			IVariableDictionary* mpDictionary;
		};
		ValueStorage mValue;
	};

} } // namespace MaxSDK::Graphics
