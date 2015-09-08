//**************************************************************************/
// Copyright (c) 1998-2011 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Character Class
// AUTHOR:      Richard Houle
// DATE:        May 9 2011
//***************************************************************************/

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "maxheap.h"
#include "strbasic.h"

namespace MaxSDK { namespace Util {

class MaxString;

/**
 * Represents a single unicode character.
 *
 * The main advantage of this class is the ability of representing
 * them without allocating external memory.
 *
 * This class was designed to be able to cast "unsigned int*" array
 * to a "Char*" array. This class doesn't have any destructor or virtual
 * functions.
 */
class UtilExport Char : public MaxHeapOperators 
{
protected:
	/* UCS4 character */
	unsigned int _ch;

public:
	/* Create a NULL character. */
	Char();

	/**
	 * Create a character with a single-byte Active Code Page character.
	 *
	 * This constructor is best used with a compile-time constant
	 * because if you specify a only the first half of a DBCS character
	 * this function will throw an exception.
	 */
	Char(char);

	/**
	 * Create a character with a single-WCHAR UTF16 character.
	 *
	 * This constructor is best used with a compile-time constant
	 * because if you specify a only the first half of a UTF16 character
	 * this function will throw an exception.
	 */
	Char(wchar_t);

	/* Create a character with the first character of a MaxString. */
	Char(const MaxString&);

	/* Create a character with the first character of a UTF16 String. */
	Char(const wchar_t*);

	/* Create a character with the first character of an Active Code Page String. */
	Char(const char*);

	/* Create a character with the first character of a Code Page String. */
	Char(unsigned int code_page, const char*);

	/**
	 * Create a character using a UCS4 character.
	 *
	 * We specified "explicit" because "unsigned int" are so generic that
	 * we didn't want to construct a "Char" implicitly by mistake.
	 */
	explicit Char(unsigned int ch);

public:
	/**
	 * Convert this character to a MaxString.
	 */
	MaxString ToMaxString() const;

	/**
	 * Convert this character to UTF16.
	 * \param string         string buffer to put the converted characters
	 * \param length         the buffer size of param string in number characters
	 * \return				 number of characters converted.
	 */
	size_t ToUTF16(wchar_t* string, size_t length) const;

	/**
	 * Convert this character to the active code page.
	 * \param string - string buffer to put the converted characters
	 * \param length - the buffer size of param string in number characters
	 * \return - number of characters converted.
	 */
	size_t ToACP(char* string, size_t length) const;

	/**
	 * Convert this character to the specified code page.
	 * \param string         string buffer to put the converted characters
	 * \param length         the buffer size of param string in number characters
	 * \return				 number of characters converted.
	 */
	size_t ToCP(unsigned int cp, char* string, size_t length) const;

	/**
	 * Convert this character to an MCHAR (Active Code Page or UTF16).
	 * \param s              string buffer to put the converted characters
	 * \param length         the buffer size of param s in number characters
	 * \return				 number of characters converted.
	 */
#if defined(MCHAR_IS_WCHAR)
	inline size_t ToMCHAR(MCHAR* s, size_t length) const { return ToUTF16(s, length); };
#else
	inline size_t ToMCHAR(MCHAR* s, size_t length) const { return ToACP(s, length); };
#endif

public:
	/**
	 * Compare this character to an active code page character.
	 */
	bool operator == (char) const;

	/**
	 * Compare this character to a UTF16 character.
	 */
	bool operator == (wchar_t) const;

	/**
	 * Compare this character to a UTF16 character.
	 */
	bool operator == (const Char&) const;
	
	/** Not equal operator for an active code page character. */
	inline bool operator != (char c)        const { return !(*this == c); }
	/** Not equal operator for a UTF16 character. */
	inline bool operator != (wchar_t c)     const { return !(*this == c); }
	/** Not equal operator for a UTF16 character. */
	inline bool operator != (const Char& c) const { return !(*this == c); }

	/** Assignment operator for a UTF16 character. */
	Char& operator = (wchar_t);

	/** Assignment operator for an active code page character. */
	Char& operator = (char);

	/** Assignment operator for a MaxString. */
	Char& operator = (const MaxString&);

	/** Assignment operator for a constant UTF16 character. */
	Char& operator = (const wchar_t*);

	/** Assignment operator for a constant active code page character. */
	Char& operator = (const char*);

	/**
	 * Change this character to represents the first character of the string specified, 
	 * based on the code page specified.
	 * \param cp - The specified code page
	 * \param string - The string to use as a copy source
	 */
	Char& Set(unsigned int cp, const char* string);

	/** Returns true if this character represents a NULL character. */
	inline bool IsNull() const { return _ch == 0; }

	/** Returns true if this character represents a non-NULL character. */
	inline bool IsValid() const { return _ch != 0; }

	/** Returns the character as UTF32 */
	inline unsigned int CharCode() const { return _ch; }

	/* Returns true if this character is a space. Wrapper over iswspace */
	bool IsSpace() const;

	/* Returns true if this character is a numerical digit. Wrapper over iswdigit */
	bool IsDigit() const;

	/* Returns true if this character is a letter. Wrapper over iswalpha */
	bool IsAlpha() const;

	/* Returns true if this character is a letter or a number. Wrapper over iswalnum */
	bool IsAlphaNumeric() const;

	/* Returns true if this character is punctuation. Wrapper over iswpunc */
	bool IsPunctuation() const;

	/* Returns true if the character is a unicode character
	 *
	 * By design, in MBCS mode, all character that are under
	 * 0x7e are exactly the same in all code page.
	 */
	inline bool IsUnicode() const { return _ch > 0x7e; }

	/* Converts the character to upper case. */
	Char ToUpper() const;

	/* Converts the character to lower case. */
	Char ToLower() const;

	/**
	 * Determine if the UTF-8 string is valid.
	 *
	 * \param data				 Buffer pointer to validate encoding of
	 * \param len				 Size of the buffer to validate
	 * \param pFoundExtendedChar If not null, returns if the presence of multi-bytes character was found.
	 * \param pPartialCharAtEnd  If not null, returns if the number of bytes missing for the  last character.
	 *                           The beginning was correct, but it missed the ending.
	 * \return					 True if the string is a valid UTF8, False otherwise
	 */
	static bool IsValidUTF8(const void* data, size_t len, bool* pFoundExtendedChar = 0, size_t* pPartialCharAtEnd = 0);

	/**
	 * Determine the length of an UTF-8 character.
	 * 
	 * \param character			 UTF-8 character to get the length of
	 * \return					 length of the character in bytes. 
	 */
	static unsigned int UTF8CharLength(unsigned char character);

	/**
	 * Determine if the UTF-16 char is a lead character. In some situation, UTF-16 can be stored
	 * on four bytes instead of two.
	 *
	 * \param character			 Character to test
	 * \return					 True if the character is a lead character, False otherwise
	 */
	static bool IsUTF16LeadChar(wchar_t character);

	/**
	 * Determine if this character is found inside a particular
	 * character set.
	 *
	 * This function slightly differs from strrchr or wcsrchr, 
	 * because it can search DBCS or UTF16 character accurately
	 * inside a string without partially matching it.
	 */
	MCHAR* IsInCharacterSet(const MCHAR*) const;
};

/**
 * Iterate intelligently inside a string.
 *
 * This class should be used to iterate through
 * characters of a string. It guarantees that the string pointer
 * never points to the end of a character.
 * The template parameter should be either char, wchar_t, const wchar_t etc...
 */
template <typename ChType>
class CharIterator : public MaxHeapOperators  
{
protected:
	ChType* _ptr;

public:
	/** Construct this iterator with a NULL pointer. */
	inline CharIterator() 
		: _ptr(NULL)
	{ }

	/** Construct this iterator with a string pointer. */
	inline CharIterator(ChType* ptr) 
		: _ptr(ptr)
	{ }

	/** Copy an existing character iterator. */
	inline CharIterator(const CharIterator<ChType>& p) 
		: _ptr(p._ptr)
	{ }

public:
	/**
	 * Gets the number of elements of the current template type, for the current
	 * character pointed to by the iterator. 
	 */
	int CharLength() const;

public:
	/** Assignment operator. Copies an existing character iterator. */
	CharIterator<ChType>& operator = (const CharIterator<ChType>& p)
	{
		_ptr = p._ptr;
		return *this;
	}

	/** Assignment operator. Copies another character as the templated type as this iterator. */
	CharIterator<ChType>& operator = (ChType* ptr)
	{
		_ptr = ptr;
		return *this;
	}

public:
	/** Move to the next character */
	CharIterator<ChType>& operator ++ ();
	CharIterator<ChType>  operator ++ (int);

	/** Move the iterator character forward by a specified amount. (It will stop at the first null character.) */
	CharIterator<ChType>& operator += (size_t s);

	/** Move the iterator character forward by a specified amount. (It will stop at the first null character.) */
	CharIterator<ChType> operator + (size_t s) const;

	/** Return the current character. */
	const Char operator * () const;

	/** Return the nth character from the iterator. 
	 * \param index - The index of the character to retrieve from the string
	*/
	inline const Char operator [] (size_t index) const
	{
		CharIterator<ChType> it(*this);

		it += index;
		return *it;
	}

	/** Determine if the iterator has reached the end of the string. */
	inline bool AtEnd() const
	{
		return *_ptr == 0;
	}

	/** Obtain the current string pointer. */
	inline ChType* Get() const
	{
		return _ptr;
	}

	/** Return the nth character from the iterator. 
	 * \param index - The index of the character to retrieve from the string
	*/
	inline const Char Get(size_t index) const
	{
		CharIterator<ChType> it(*this);

		it += index;
		return *it;
	}

	/** Obtain the current string pointer. */
	inline operator ChType* () const
	{
		return _ptr;
	}
};

// Explicitly declare which template function to export publicly.
template <> UtilExport int                 CharIterator<char>::CharLength() const;
template <> UtilExport CharIterator<char>& CharIterator<char>::operator ++ ();
template <> UtilExport CharIterator<char>  CharIterator<char>::operator ++ (int);
template <> UtilExport CharIterator<char>& CharIterator<char>::operator += (size_t s);
template <> UtilExport CharIterator<char>  CharIterator<char>::operator +  (size_t s) const;
template <> UtilExport const Char          CharIterator<char>::operator *  () const;

template <> UtilExport int                       CharIterator<const char>::CharLength() const;
template <> UtilExport CharIterator<const char>& CharIterator<const char>::operator ++ ();
template <> UtilExport CharIterator<const char>  CharIterator<const char>::operator ++ (int);
template <> UtilExport CharIterator<const char>& CharIterator<const char>::operator += (size_t s);
template <> UtilExport CharIterator<const char>  CharIterator<const char>::operator +  (size_t s) const;
template <> UtilExport const Char                CharIterator<const char>::operator *  () const;

template <> UtilExport int                    CharIterator<wchar_t>::CharLength() const;
template <> UtilExport CharIterator<wchar_t>& CharIterator<wchar_t>::operator ++ ();
template <> UtilExport CharIterator<wchar_t>  CharIterator<wchar_t>::operator ++ (int);
template <> UtilExport CharIterator<wchar_t>& CharIterator<wchar_t>::operator += (size_t s);
template <> UtilExport CharIterator<wchar_t>  CharIterator<wchar_t>::operator +  (size_t s) const;
template <> UtilExport const Char             CharIterator<wchar_t>::operator *  () const;

template <> UtilExport int                          CharIterator<const wchar_t>::CharLength() const;
template <> UtilExport CharIterator<const wchar_t>& CharIterator<const wchar_t>::operator ++ ();
template <> UtilExport CharIterator<const wchar_t>  CharIterator<const wchar_t>::operator ++ (int);
template <> UtilExport CharIterator<const wchar_t>& CharIterator<const wchar_t>::operator += (size_t s);
template <> UtilExport CharIterator<const wchar_t>  CharIterator<const wchar_t>::operator +  (size_t s) const;
template <> UtilExport const Char                   CharIterator<const wchar_t>::operator *  () const;

/**
 * Character accumulator
 *
 * Concatenate "Char" object to form a character string.
 */
class UtilExport CharAccumulator : public MaxHeapOperators 
{
protected:
	MCHAR* _buf;
	size_t _len;
	size_t _allocated;

public:
	/** Construct a new empty character accumulator. */
	CharAccumulator();

	/**
	 * Destructor
	 *
	 * During cleanup the underlying string is freed.
	 */
	~CharAccumulator();

public:
	/** Get the string pointer. */
	operator MCHAR* () const;

	/** Get the string pointer. */
	MCHAR* Get() const;

public:
	/** Add a character at the end of this string. */
	CharAccumulator& operator += (const Char&);

	/** Reset this accumulator */
	void Reset();

	/** Obtain the last character in this accumulator */
	Char LastCharacter() const;

	/** Remove the last character of this accumulator */
	Char RemoveLastCharacter();

	/** Return the number of MCHAR already in the accumulator */
	inline size_t Length() const { return _len; }
};

}}

extern "C"
{
	/**
	 * Convert a UTF32 character string to UTF16 characters.
	 *
	 * \param lpUTF32CharStr   Pointer to the character string to convert.
	 *
	 * \param ccUTF32Char      Size, in WWCHAR, of the string indicated by the lpUTF32CharStr parameter.
	 *                        Alternatively, this parameter can be set to -1 if the string is null-terminated.
	 *                        Note that, if ccUTF32Char is 0, the function fails.
	 *
	 *                        If this parameter is -1, the function processes the entire input string, 
	 *                        including the terminating null character. Therefore, the resulting Unicode string
	 *                        has a terminating null character, and the length returned by the function includes
	 *                        this character.
	 *
	 *                        If this parameter is set to a positive integer, the function processes exactly the
	 *                        specified number of bytes. If the provided size does not include a terminating null
	 *                        character, the resulting Unicode string is not null-terminated, and the returned length
	 *                        does not include this character.
	 *
	 * \param lpWideCharStr   Optional. Pointer to a buffer that receives the converted string.
	 *
	 * \param cchWideChar     Size, in WCHAR, of the buffer indicated by lpWideCharStr. If this value is 0, the
	 *                        function returns the required buffer size, in characters, including any terminating null
	 *                        character, and makes no use of the lpWideCharStr buffer.
	 *
	 * \returns Returns the number of characters written to the buffer indicated by lpWideCharStr if successful. If
	 *          the function succeeds and cchWideChar is 0, the return value is the required size, in WCHAR, for
	 *          the buffer indicated by lpWideCharStr.
	 *
     *          The function returns 0 if it does not succeed. To get extended error information, the application can
	 *          call GetLastError, which can return one of the following error codes:
     *          <ul>
	 *          <li>ERROR_INSUFFICIENT_BUFFER. A supplied buffer size was not large enough, or it was incorrectly set to NULL.</li>
	 *          <li>ERROR_INVALID_PARAMETER. Any of the parameter values was invalid.</li>
	 *          </ul>
	 */
	UtilExport size_t UTF32ToWideChar(const unsigned int* lpUTF32CharStr, size_t ccUTF32Char, wchar_t* lpWideCharStr, size_t cchWideChar);

	/**
	 * Convert UTF16 character string to UTF32 characters.
	 *
	 * \param lpWideCharStr   Pointer to the character string to convert.
	 *
	 * \param cchWideChar     Size, in WCHAR, of the string indicated by the lpWideCharStr parameter.
	 *                        Alternatively, this parameter can be set to -1 if the string is null-terminated.
	 *                        Note that, if cchWideChar is 0, the function fails.
	 *
	 *                        If this parameter is -1, the function processes the entire input string, 
	 *                        including the terminating null character. Therefore, the resulting Unicode string
	 *                        has a terminating null character, and the length returned by the function includes
	 *                        this character.
	 *
	 *                        If this parameter is set to a positive integer, the function processes exactly the
	 *                        specified number of bytes. If the provided size does not include a terminating null
	 *                        character, the resulting Unicode string is not null-terminated, and the returned length
	 *                        does not include this character.
	 *
	 * \param lpUTF32CharStr   Optional. Pointer to a buffer that receives the converted string.
	 *
	 * \param ccUTF32Char      Size, in WWCHAR, of the buffer indicated by lpUTF32CharStr. If this value is 0, the
	 *                        function returns the required buffer size, in characters, including any terminating null
	 *                        character, and makes no use of the lpUTF32CharStr buffer.
	 *
	 * \returns Returns the number of character written to the buffer indicated by lpUTF32CharStr if successful. If
	 *          the function succeeds and ccUTF32Char is 0, the return value is the required size, in WWCHAR, for
	 *          the buffer indicated by lpUTF32CharStr.
	 *
     *          The function returns 0 if it does not succeed. To get extended error information, the application can
	 *          call GetLastError, which can return one of the following error codes:
     *          <ul>
	 *          <li>ERROR_INSUFFICIENT_BUFFER. A supplied buffer size was not large enough, or it was incorrectly set to NULL.</li>
	 *          <li>ERROR_INVALID_PARAMETER. Any of the parameter values was invalid.</li>
	 *          </ul>
	 */
	UtilExport size_t WideCharToUTF32(const wchar_t* lpWideCharStr, size_t cchWideChar, unsigned int* lpUTF32CharStr, size_t ccUTF32Char);
}

