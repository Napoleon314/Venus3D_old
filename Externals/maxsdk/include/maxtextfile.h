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
// DESCRIPTION: Text File I/O
// AUTHOR:      Richard Houle
// DATE:        Fev 7 2011
//***************************************************************************/

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "maxstring.h"
#include "maxchar.h"
#include "utilexp.h"
// forward declarations
class CharBinaryStream;

namespace MaxSDK 
{ 
	namespace Util 
	{
		class BinaryStream;
		class BinaryStreamMemory;
	}
}

namespace MaxSDK { namespace Util { namespace TextFile {

/**
 * This class is the base for all text file readers.
 * It declare a common interface to all implementation
 */
class UtilExport BaseTextReader : public MaxHeapOperators 
{
public:
	virtual ~BaseTextReader();

	/** 
	 * Reads a single char and returns it's UTF32 representation.
	 * UTF-32 (or UCS-4) is a protocol to encode Unicode characters 
	 * that uses exactly 32 bits per Unicode code point. All other Unicode 
	 * transformation formats use variable-length encodings. 
	 * The UTF-32 form of a character is a direct representation of its codepoint. 
	 * \param peek - Read a char but does not move the internal pointer to next char.
	 * \return UTF32 character read
	 **/
	virtual unsigned int ReadCharUTF32(bool peek = false) const = 0;

	/**
	 * Reads characters from the file
	 *
	 * \param nchars         Stop reading after 'nchars' characters or when end of file reached.
	 * \return				 Characters read.
	 */
	virtual MaxString ReadChars(size_t nchars) const = 0;

	/**
	 * Reads a line from the file (or nchars, depending on which one comes first.)
	 *
	 * \param nchars         Stop reading after 'nchars' characters even if the EOL was not found.
	 * \param dontReturnEOL  By default, this function will returns the line including it's end-of-line
	 *                       character(s) unless you set "dontReturnEOL" to true.
	 * \return				 Line read.
	 */
	virtual MaxString ReadLine(size_t nchars = (size_t)-1, bool dontReturnEOL = false) const = 0;

	/** Get the number of characters read so far. */
	virtual size_t Position() const = 0;


	/** Get the current line number. */
	virtual size_t LineNumber() const = 0;

	/**
	 * Read a character from the file
	 *
	 * \param peek			Read a char but does not move the internal pointer to next char.
	 * \return				Character read.
	 */
	virtual Char ReadChar(bool peek = false) const = 0;

	/**
	 * Unread a character
	 *
	 * Put back a character inside the buffer. The data
	 * will be re-read next time you call read.
	 * This is used internally when parsing max scripts.
	 *
	 * \param c Char to put back in the buffer
	 * \return Number of character written.
	 */
	virtual size_t UnreadChar(const Char& c) = 0;

	/**
	 * Seek inside the stream.
	 *
	 * \param offset    The seeks operations are done in number of characters (not bytes).
	 * \param origin	The direction to move.  Origin can be one of the following
	 *  * SEEK_CUR 	 Current position of file pointer.
	 *  * SEEK_END	 End of file.
	 *  * SEEK_SET	 Beginning of file.
	 * \return Returns the absolute position of the text file. (in chars)
	 */
	virtual size_t Seek(long offset, int origin) = 0;

	/**
	 * Returns true if file is open
	 */
	virtual bool IsFileOpen() const = 0;

	/**
	 * Returns true if file at the end
	 */
	virtual bool IsEndOfFile() const = 0;
};

/**
* This class is the base for all text file writers.
* It declare a common interface to all implementations and expose
* a number of services to output formatted strings to a file.
 */
class UtilExport BaseTextWriter : public MaxHeapOperators 
{
public:
	virtual ~BaseTextWriter();

	/** 
	 * Write a string
	 * \param string	String to write
	 * \param nchars	Number of character to write. Default is size of string in characters
	 * \return			Number of characters written
	 */
	virtual size_t Write(const char* string, size_t nchars = (size_t)-1) = 0;

	/** 
	 * Write an UTF-16 string 
	 * \param string	String to write
	 * \param nchars	Number of character to write. Default is size of string in characters
	 * \return			Number of characters written
	 */
	virtual size_t Write(const wchar_t*string, size_t nchars = (size_t)-1) = 0;

	/** 
	 * Write an MaxString 
	 * \param String	String to write
	 * \return			Number of characters written
	 */
	virtual size_t Write(const MaxString&String) = 0;

	/**
	 * Returns true if we can write to file.
	 */
	virtual bool CanWrite() const = 0;

	/**
	 * Returns true if file is open
	 */
	virtual bool IsFileOpen() const = 0;

	/**
	 * Make sure that all the buffer are synced with the OS native objects.
	 */
	virtual void Flush() = 0;

	/**
	 * Returns true if file at the end
	 */
	virtual bool IsEndOfFile() const = 0;

	/** 
	 * Get the current position in the file. 
	 */
	virtual size_t Position() const = 0;

	/**
	 * Seek inside the stream.
	 *
	 * \param offset    The seeks operations are done in number of characters (not bytes).
	 * \param origin	The direction to move.  Origin can be one of the following
	 *  * SEEK_CUR 	 Current position of file pointer.
	 *  * SEEK_END	 End of file.
	 *  * SEEK_SET	 Beginning of file.
	 * \return Returns the absolute position of the text file. (in chars)
	 */
	virtual size_t Seek(long offset, int origin) = 0;

#ifdef UNICODE
	size_t Printf (const wchar_t*, ...);
	size_t Vprintf(const wchar_t*, va_list);
#else
	size_t Printf (const char*, ...);
	size_t Vprintf(const char*, va_list);
#endif
private:

	/** Write formatted strings to the file */
	size_t VprintfA(const char*,    va_list);
	size_t VprintfW(const wchar_t*, va_list);

	/** Write formatted strings to the file */
	size_t PrintfA(const char*,    ...);
	size_t PrintfW(const wchar_t*, ...);

};

/**
 * LineEndMode is used to control how text file end of line characters are handled.
 */
enum LineEndMode {
	/** 
	 * When reading or writing a file, don't alter the end of line character sequence.
	 */
	Unchanged,			

	/** 
	 * When reading or writing a file, ensure that all lines are terminated by the CRLF characters sequence
	 */
	Enforce_CRLF, 

	/** 
	 * When reading or writing a file, ensure that all lines are terminated by the LF character.
	 */
	Enforce_LF,

	/* On Windows, the standard text is CRLF. */
	Text = Enforce_CRLF
};

/**
 * Reads and interprets text files.
 *
 * This class was designed to perform file and stream I/O in a code page neutral way.
 *
 * It was designed to perform resolve the following problems:
 *   * Reads and interprets correctly the BOM (an invisible character at the beginning of unicode files.)
 *   * Detect correctly UTF-8 and UTF-16 files. (even if it's not signed.)
 *   * Detect encoding cookies. XML files usually begins with "<?xml encoding='????'>". The detection algorithm
 *     will interpret this directive correctly.
 *   * Prevent from splitting a character. In UTF-16, UTF-8 and some ANSI codepage, characters can be
 *     stored on 1 to 6 bytes. All the operations of this object are designed to avoid to returns a partial
 *     character.
 *	
 * Plugin developer should consider using this class to perform File I/O to ensure that the files they generate
 * remain compatible to previous version of Max.
 * \see ReaderWriter class when file IO requirements are for reading and writing.
 */
class UtilExport Reader : public BaseTextReader
{
public:

	/**
	 * Text file reading encoding
	 */
	enum TextFileReaderEncoding {
		/* If the file's encoding cannot be detected, favor UTF-8.
		   By default, we favor ACP encoding. */
		FAVOR_UTF8 = 0x10000000,

		/* Found a BOM at the beginning of the file. */
		FOUND_BOM = 0x20000000,

		/* Found a cookie at the beginning of the file. */
		FOUND_COOKIE = 0x40000000,

		/* Found flipped UTF-16 data. */
		FLIPPED = 0x80000000
	};

	/**
	 * EOF character handling
	 */
	enum EOFCharacterHandling {
		DEFAULT_EOF_HANDLING,		/// Used by ReaderWriter to override default value.
		STOP_READING_AT_EOF,			/// Reading of file terminate at EOF character - this is Reader in text mode.
		FILTEROUT_EOF_CHARACTER,	/// EOF character are filter out - this is the ReaderWriter preferred mode
		IGNORE_EOF_CHARACTER		/// EOF character are read as regular character - this is Reader in binary mode.
	};

protected:

#pragma region Protected members and methods
	/**
	 * Internal processing error code.  Used by LastError to determine the proper
	 * message to generate.
	 */
	enum TextFileReaderError {
		ALL_OK,						/// No errors
		STREAM_INVALID_ARGUMENT,	/// Invalid argument error
		STREAM_ALREADY_OPEN,		/// Stream already open
		STREAM_NOT_OPEN,			/// Stream not open
		ERR_INVALID_FORMAT,			/// Invalid 
		STREAM_ERROR				/// Stream error
	};

	BinaryStream*				_stream;
	bool						_streamDelete;
	mutable bool				_readCR;
	mutable bool				_readLF;
	LineEndMode					_endOfLineMode;
	size_t						_detectSize;
	size_t						_readSize;
	mutable EOFCharacterHandling _eofCharacterHandling;
	mutable TextFileReaderError _error;

	mutable unsigned int        _encoding;
	mutable bool                _encodingDetected;
	mutable BinaryStreamMemory* _backbuffer;
	mutable size_t              _ignoreBytes;
	mutable size_t              _positionBytes;
	mutable size_t              _positionChars;
	mutable size_t              _line;


	/**
	 * Detect the opened file encoding by analyzing the first "len" bytes of the file.
	 *
	 * \param len			Size of the buffer to use to detect the encoding
	 * \return				.
	 */
	void Detect(size_t len) const;

	/**
	 * Read and cache len bytes from stream 
	 *
	 * \param len
	 * \param force
	 * \param binary
	 * \param detecting_encoding
	 */
	size_t FillBuffer(size_t len, bool force = false, bool binary = false, bool detecting_encoding = false) const;

	/**
	 * Apply the selected open mode on the internal cache buffer up to position
	 * \param	pos	Position to stop filtering
	 * \return	Size of the buffer filtered
	**/
	size_t Filter(size_t pos) const;

	/**
	 * Apply the selected open mode on the internal cache buffer up to position
	 * \param	pos	Position to stop filtering
	 * \param processBufferBoundaryCRLF if true, take care of the CR or LF which is read in the last FillBuffer call 
	 * \return	Size of the buffer filtered
	 **/
	size_t Filter(size_t pos, bool processBufferBoundaryCRLF) const; 

	/**
	 * Ensure buffer size can contains the passed length
	 * \param	len	The minimum size of the cache buffer
	 * \return	The remaining buffer size
	**/
	size_t EnsureBufferContains(size_t len) const;

	/**
	 * Return the number of characters in the passed buffer depending on the current encoding
	 *
	 * \param data	Buffer to evaluate
	 * \param size	The size of the buffer in bytes
	 * \return		The total number of chars.
	**/
	size_t NumberOfChars(const void* data, size_t size) const;

	/**
	 * Return the number of bytes corresponding to the num of UTF8 chars in passed buffer 
	 *
	 * \param data - Buffer to evaluate
	 * \param num - num of chars in the passed buffer
	 * \return		The total number of bytes.
	**/
	size_t ConvertNumUTF8CharsToNumBytes( const char* data, size_t num);

	/**
	 * Return the number of bytes corresponding to the num of UTF16 chars in passed buffer
	 *
	 * \param data Buffer to evaluate
	 * \param num num of chars in the passed buffer
	 * \return		The total number of bytes.
	**/
	size_t ConvertNumUTF16CharsToNumBytes(const MCHAR* data, size_t num);

	/**
	 * Return the number of bytes corresponding to the num of chars in passed buffer depending on the encoding
	 *
	 * \param data Buffer to evaluate
	 * \param num number of chars in the passed buffer
	 * \param encoding
	 * \return		The total number of bytes.
	**/
	size_t ConvertNumCharsToNumBytes(const char* data, size_t num, unsigned int encoding);

	/**
	 * Calculate the total number of line feeds in the passed buffer
	 *
	 * \param data The buffer to evaluate the number of line
	 * \param size The size of the buffer in bytes
	 * \return		The total number of lines.
	**/
	size_t NumberOfLines(const void* data, size_t size) const;

	/** INTERNAL FUNCTION. Used in the implementation of ReadChar() and ReadCharUTF32(). */
	template <typename ChType, typename CharLengthFunctor, int maxCharLength>
	ChType* ReadChar(size_t& charLengthT, bool peek, const CharLengthFunctor& CharLengthFunction) const;

	template <typename ChType>
	MaxString MakeString(const ChType* data, size_t length, bool dontReturnEndingCRLF) const;

	/**
	 * Unread String
	 *
	 * Put back a sequence of character inside the buffer. The data
	 * will be re-read next time you call read.
	 * This is used internally when parsing max scripts.
	 *
	 * \param string String to put back in the buffer
	 * \return Unread size in number of characters.
	 */
	size_t Unread(const MaxString& string);

	/** INTERNAL FUNCTION. Used by Seek */
	/**
	 * Seek inside this text stream with the end as the reference point.
	 *
	 * \param offset	Offset characters from end of file to seek to
	 * \return Returns the absolute position of the text file. (in chars)
	 */
	size_t SeekToEnd(long offset = 0);

	/**
	 * Seek to an absolute point inside the text stream.
	 *
	 * \param offset	Offset characters from end of file to seek to
	 * \return Returns the absolute position of the text file. (in chars)
	 */
	size_t SeekToAbsolute(long offset);

	/**
	 * Advance "offset" characters.
	 *
	 * \param offset	Offset characters from end of file to seek to
	 * \return Returns the absolute position of the text file. (in chars)
	 */
	size_t SeekFromCurrent(long offset);

	/**
	* Open an abstract BinaryStream.
	*
	* \param stream   Opened stream the Reader uses
	* \param encoding This parameter can contains hint to the detection algorithm.
	*                 Acceptable values are all codepages numbers that are recognized by
	*                 Windows.
	*
	*                 In addition to that, you can also specify FAVOR_UTF8. It can be used
	*                 to cascade the detection of the codepage. For example, if you specify
	*                 "CP_ACP | FAVOR_UTF8", the detection algorithm will treat any non-UTF8
	*                 data as ACP.
	*				   \see TextFileReaderEncoding
	* \param mode	   \see LineEndMode - default Text
	* \param closeOnDelete Delete the "stream" at the same time of this object.
	*/
	bool Open(BinaryStream* stream, unsigned int encoding = 0, LineEndMode mode = Text, bool closeOnDelete = false);

#pragma endregion

public:

	friend class ReaderWriter;
	friend class CharBinaryStream;
	friend class BinaryStreamMemory;

	/** Default Constructor */
	Reader();
	/** Destructor */
	virtual ~Reader();

	/**
	 * Wrap ANSI C FILE pointer
	 * This service is used to allow a developer to access a file open with fopen but 
	 * with the advantage to use the SDK API.
	 * Using this service allows the developer to not worry about character encoding
	 * The developer is responsible to close the file once he is done.
	 *
	 * \param file	   ANSI C FILE pointer
	 * \param encoding This parameter can contains hint to the detection algorithm.
	 *                 Acceptable values are all codepages numbers that are recognized by
	 *                 Windows.
	 *
	 *                 In addition to that, you can also specify FAVOR_UTF8. It can be used
	 *                 to cascade the detection of the codepage. For example, if you specify
	 *                 "CP_ACP | FAVOR_UTF8", the detection algorithm will treat any non-UTF8
	 *                 data as ACP.
	 *				   \see TextFileReaderEncoding
	 *				   The flag is only used when no BOM is present or if the file is a new file.  
	 *				   If the file has been open with css=\<encoding\>, a BOM is present and this 
	 *				   parameter is ignored.
	 * \param mode	   \see LineEndMode - default Text
	 * \return		   true if successful, false otherwise
	 */
	bool Open(FILE* file, unsigned int encoding = 0, LineEndMode mode = Text);

	/**
	 * Wrap a Win32 file handle
	 *
	 * \param fileHandle File Handle 
	 * \param encoding This parameter can contains hint to the detection algorithm.
	 *                 Acceptable values are all codepages numbers that are recognized by
	 *                 Windows.
	 *
	 *                 In addition to that, you can also specify FAVOR_UTF8. It can be used
	 *                 to cascade the detection of the codepage. For example, if you specify
	 *                 "CP_ACP | FAVOR_UTF8", the detection algorithm will treat any non-UTF8
	 *                 data as ACP.
	 *				   \see TextFileReaderEncoding
	 * \param mode	   \see LineEndMode - default Text
	 * \return		   true if successful, false otherwise
	 */
	bool Open(HANDLE fileHandle, unsigned int encoding = 0, LineEndMode mode = Text);

	/**
	 * Open a file using a file name
	 *
	 * \param fileName	File name to open.  If file does not exist, it will be created.
	 * \param encoding This parameter can contains hint to the detection algorithm.
	 *                 Acceptable values are all codepages numbers that are recognized by
	 *                 Windows.
	 *
	 *                 In addition to that, you can also specify FAVOR_UTF8. It can be used
	 *                 to cascade the detection of the codepage. For example, if you specify
	 *                 "CP_ACP | FAVOR_UTF8", the detection algorithm will treat any non-UTF8
	 *                 data as ACP.
	 *				   \see TextFileReaderEncoding
	 * \param mode	   \see LineEndMode - default Text
	 * \return		   true if successful, false otherwise
	 */
	bool Open(const MCHAR* fileName, unsigned int encoding = 0, LineEndMode mode = Text);

	/**
	 * Open a file using a file name
	 *
	 * \param fileName	File name to open.  If file does not exist, it will be created.
	 * \param encoding This parameter can contains hint to the detection algorithm.
	 *                 Acceptable values are all codepages numbers that are recognized by
	 *                 Windows.
	 *
	 *                 In addition to that, you can also specify FAVOR_UTF8. It can be used
	 *                 to cascade the detection of the codepage. For example, if you specify
	 *                 "CP_ACP | FAVOR_UTF8", the detection algorithm will treat any non-UTF8
	 *                 data as ACP.
	 *				   \see TextFileReaderEncoding
	 * \param mode	   \see LineEndMode - default Text
	 * \return		   true if successful, false otherwise
	 */
	bool Open(const MaxSDK::Util::MaxString& fileName, unsigned int encoding = 0, LineEndMode mode = Text);


	/** Close the underlying stream and free any intermediate data. */
	void Close();

	/**
	 * Returns the last error return by the BinaryStream
	 *
	 * \return		Error string
	**/
	MaxString LastError() const;


	/**
	 * Returns the current encoding of this file. 
	 *
	 * \see TextFileReaderEncoding
	 *	The actual code page can be retrieved this way : "Encoding() & MSDE_CP_MASK"
	 *
	 * \return	Returns the actual encoding found
	**/
	unsigned int Encoding() const;

	/**
	 * Determine how this reader handles line ending.
	 *
	 * \return	LineEndMode
	**/
	LineEndMode Mode() const;

	/**
	 * Set the read buffer size.
	 * The larger the buffer is, the better read performance is. 
	 * \param readSize		Size of the buffer to read.  
	 *                      Default 4096
	 */
	void SetReadBufferSize(size_t readSize);


	/**
	 * Returns the read buffer size
	 */
	size_t GetReadBufferSize() const;

	/**
	 * Set the default size of the buffer used to validate encoding.  
	 * This parameter is used internally when calling Detect.
	 * \param detectSize	Size of the buffer used when detecting the current character type.
	 *                      Default 65536
	 */
	void SetDetectSize(size_t detectSize);

	/**
	 * Returns the detect buffer size
	 */
	size_t GetDetectSize() const;

	/**
	 * Return the number of characters in the file
	 *
	 * \return		The total number of chars.
	**/
	size_t NumberOfChars() const;

	/**
	 * Calculate the total number of line feeds in the file
	 *
	 * \return		The total number of lines.
	**/
	size_t NumberOfLines() const;

	/**
	 * Set the EOF character handling method and refilter the buffer. \see EOFCharacterHandling
	 */
	void SetEOFCharacterHandling( EOFCharacterHandling );

	/**
	 * Get the EOF character handling method. \see EOFCharacterHandling
	 */
	EOFCharacterHandling GetEOFCharacterHandling() const;

	/**
	 * Reads a single char
	 *
	 * \param	peek	Read a char but does not move the internal pointer to next char.  
	 *					Default is false so we move to next character
	 * \return			The character read.
	**/
	virtual Char ReadChar(bool peek = false) const;

	/**
	 * Reads a single char and returns it's UTF32 representation.
	 *
	 * \param peek		Read a char but does not move the internal pointer to next char.
	 *					Default is false so we move to next character
	 * \return			The UTF32 char representation
	**/

	virtual unsigned int ReadCharUTF32(bool peek = false) const;

	/**
	 * Reads characters from the file
	 *
	 * \param nchars         Stop reading after 'nchars' characters.
	 * \return				 Line read.
	 */
	virtual MaxString ReadChars(size_t nchars) const;

	/**
	 * Reads a line from the file (or nchars, depending on which one comes first.)
	 *
	 * \param nchars         Stop reading after 'nchars' characters even if the EOL was not found.
	 * \param dontReturnEOL  By default, this function will returns the line including it's end-of-line
	 *                       character(s) unless you set "dontReturnEOL" to true.
	 */
	virtual MaxString ReadLine(size_t nchars = (size_t)-1, bool dontReturnEOL = false) const;

	/**
	 * Reads up to "len" bytes from the file and convert it to a unicode-compliant string.
	 *
	 * \param len                Number of bytes to take out of the underlying stream.
	 * \param dontReturnLastEOL  Determine if this function will trim the last EOL sequence.
	 */
	virtual MaxString ReadChunk(size_t len, bool dontReturnLastEOL = false) const;

	/**
	 * Reads the file in a single operation and returns it in a single string object.
	 *
	 * \return	The full stream content
	**/
	virtual MaxString ReadFull() const;

	/** Get the number of bytes read so far. */
	virtual size_t PositionBytes() const;

	/** Get the number of characters read so far. */
	size_t Position() const;

	/** Get the current line number. */
	size_t LineNumber() const;

	/**
	 * Seek inside the stream.
	 *
	 * \param offset    The seeks operations are done in number of characters (not bytes).
	 * \param origin	The direction to move.  Origin can be one of the following
	 *  * SEEK_CUR 	 Current position of file pointer.
	 *  * SEEK_END	 End of file.
	 *  * SEEK_SET	 Beginning of file.
	 * \return Returns the absolute position of the text file. (in chars)
	 */
	virtual size_t Seek(long offset, int origin);

	/**
	 * Returns true if file at the end
	 */
	virtual bool IsEndOfFile() const;

	/**
	 * Returns true if file is open
	 */
	virtual bool IsFileOpen() const;

	/**
	 * Unread a character
	 *
	 * Put back a character inside the buffer. The data
	 * will be re-read next time you call read.
	 * \param c					Char to put back in the buffer
	 * \return					Number of character written.
	 */
	virtual size_t UnreadChar(const Char& c);

	/**
	 * Detect the encoding of the passed buffer.
	 *
	 * \param   data			  Buffer to detect the encoding
	 * \param   len				  Size of the passed buffer
	 * \param   encoding (in/out) In input, tell the detector what to expect. On output
	 *                            it contains what the detector found.
	 * \param   ignoreBytes (out) On output, tell the caller how much bytes it must ignore
	 *                            at the beginning of the file because of the BOM.
	 *
	 * \return Returns true if the encoding was formally detected. Or false, if it was guessed.
	 */
	static bool Detect(void* data, size_t len, unsigned int& encoding, size_t* ignoreBytes);

	/**
	 * \brief Determine the number of chars inside a buffer.
	 *
	 * It's more complex than just strlen or wcslen. Those two functions will returns
	 * the number of char or WCHAR entries. This function will returns the number of
	 * of characters (or symbol).
	 *
	 * \param data				 Buffer containing a string to count the number of symbols
	 * \param len				 The size of the buffer in bytes
	 * \param encoding			 Encoding to use to count the number of symbols
	 * \return					 Number of symbols
	 */
	static size_t NumberOfChars(const void* data, size_t len, unsigned int encoding = CP_ACP);

	/**
	 * Determine the number of line feeds inside a buffer.
	 *
	 * \param data      Buffer in which '\n' are counted.
	 * \param length    The size of the buffer in bytes
	 * \param encoding  Encoding of "data". Can be any valid encoding. ie. MSDE_CP_UTF16, 
	 *                  CP_UTF8, CP_ACP, etc.
	 * \return			Number of line feed detected
	 */
	static size_t NumberOfLines(const void* data, size_t length, unsigned int encoding = CP_ACP);

	/**
	 * Remove ch character from the passed buffer
	 *
	 * \param data  Buffer in which ch are to be removed.
	 * \param len   Length of data (in MCHAR)
	 */
	template <typename ChType, ChType ch>
	static size_t RemoveCharacter(ChType* data, size_t len);

	/**
	 * Truncate buffer at ch character.
	 *
	 * \param data  Buffer to validate.
	 * \param len   Length of data (in MCHAR)
	 */
	template <typename ChType, ChType ch>
	static size_t TruncateAtCharacter(ChType* data, size_t len);
};

/**
 * Write Text files.
 * 
 * This class was intended to be used to write text file. It differ from the ANSI
 * fopen, fprintf, et al, because it's smarter when it's to modify existing files.
 * It detect the underlying file format and match it intelligently.
 *
 * It was designed to resolve the following problems:
 *   * Avoid mixing different text format in the same file.
 *   * Convert intelligently strings into binary data.
 */
class UtilExport Writer : public BaseTextWriter
{
protected:
	/**
	 * Internal processing error code.  Used by LastError to determine the proper
	 * message to generate.
	 */
	enum TextFileWriterError {
		ALL_OK,                     /// No error occurred
		STREAM_INVALID_ARGUMENT,	/// Invalid argument error 
		STREAM_ALREADY_OPEN,		/// Instance of class is already in used
		STREAM_NOT_OPEN,			/// Stream not open
		ERROR_PRINTING_BUFFER,		/// Error printing buffer
		ERROR_ALLOCATING_MEMORY,	/// Error allocating memory
		STREAM_ERROR				/// Windows API error.
	};

	BinaryStream*				_stream;
	bool						_streamDelete;
	bool						_append;
	unsigned int				_encoding;
	LineEndMode					_endOfLineMode;
	mutable TextFileWriterError	_error;

	/**
	 * Open an abstract BinaryStream
	 *
	 * \param stream   Stream that the TextFile::Writer must use
	 * \param append   Append to file or truncate file
	 * \param encoding If the stream actually contains any data and append is true, it will detect it's format and match
	 *                 it. You can hint the detection algorithm by using FAVOR_UTF8.
	 *                 If the stream is empty, it will use the code page specified here. You can mix
	 *                 it with "WRITE_BOM" and/or "FLIPPED" bits.
	 *                 \see TextFileWriterEncoding
	 * \param mode     \see LineEndMode - default Text mode
	 * \param closeOnDelete Delete the "stream" at the same time of this object. Default false
	 * \return			true if successful, false otherwize
	 */
	bool Open(BinaryStream* stream, bool append = false, unsigned int encoding = 0, LineEndMode mode = Text, bool closeOnDelete = false);

public:
	/**
	* Bind on a FILE pointer
	*
	* \param file     File pointer to use
	*				   User is responsible for closing the file.
	* \param append   Append to file or truncate file
	* \param encoding If the stream actually contains any data and append is true, it will detect it's format and match
	*                 it. You can hint the detection algorithm by using FAVOR_UTF8.
	*                 If the stream is empty, it will use the code page specified here. You can mix
	*                 it with "WRITE_BOM" and/or "FLIPPED" bits.
	*                 \see TextFileWriterEncoding
	* \param mode     \see LineEndMode - default Text 
	* \return			true if successful, false otherwize
	*/
	bool Open(FILE* file, bool append = false, unsigned int encoding = 0, LineEndMode mode = Text);

	/**
	* Bind on a file File handle
	* This service is used to allow a developer to access a file open with fopen but 
	* with the advantage to use the SDK API.
	* Using this service allows the developer to now worry about character encoding
	* The developer is responsible to close the file once he is done.
	*
	* \param fileHandle     File handle to use
	*                 User is responsible for closing the file.
	* \param append   Append to file or truncate file
	* \param encoding If the stream actually contains any data and append is true, it will detect it's format and match
	*                 it. You can hint the detection algorithm by using FAVOR_UTF8.
	*                 If the stream is empty, it will use the code page specified here. You can mix
	*                 it with "WRITE_BOM" and/or "FLIPPED" bits.
	*                 \see TextFileWriterEncoding
	* \param mode     \see LineEndMode - default Text 
	* \return			true if successful, false otherwize
	*/
	bool Open(HANDLE fileHandle, bool append = false, unsigned int encoding = 0, LineEndMode mode = Text);

	enum TextFileWriterEncoding {
		/* If the file's encoding cannot be detected, favor UTF-8.
		By default, we favor ACP encoding. */
		FAVOR_UTF8 = 0x10000000,

		/* Write a BOM at the beginning of the file. */
		WRITE_BOM = 0x20000000,

		/* Write flipped UTF-16 data. */
		FLIPPED = 0x80000000
	};

	/** Default Constructor */
	Writer();
	/** Destructor */
	virtual ~Writer();

	friend class ReaderWriter;
	friend class CharBinaryStream;
	friend class TextFileWriterTest;

	/**
	* Open a file
	*
	* \param fileName File name to open
	* \param append   Append to file or truncate file
	* \param encoding If the stream actually contains any data and append is true, it will detect it's format and match
	*                 it. You can hint the detection algorithm by using FAVOR_UTF8.
	*                 If the stream is empty, it will use the code page specified here. You can mix
	*                 it with "WRITE_BOM" and/or "FLIPPED" bits.
	*                 \see TextFileWriterEncoding
	* \param mode     \see LineEndMode - default Text 
	* \return			true if successful, false otherwize
	*/
	bool Open(const MCHAR* fileName, bool append = false, unsigned int encoding = 0, LineEndMode mode = Text );

	/**
	* Open a file
	*
	* \param fileName File name to open
	* \param append   Append to file or truncate file
	* \param encoding If the stream actually contains any data and append is true, it will detect it's format and match
	*                 it. You can hint the detection algorithm by using FAVOR_UTF8.
	*                 If the stream is empty, it will use the code page specified here. You can mix
	*                 it with "WRITE_BOM" and/or "FLIPPED" bits.
	*                 \see TextFileWriterEncoding
	* \param mode     \see LineEndMode - default Text  
	* \return			true if successful, false otherwize
	*/
	bool Open(const MaxString& fileName, bool append = false, unsigned int encoding = 0, LineEndMode mode = Text);


	/**
	* Close the file
	**/
	void Close();

	/** 
	* Returns the last error return by the BinaryStream. 
	*/
	MaxString LastError() const;

	/** Returns the actual encoding used.
	*
	* \return			The encoding \see MaxSDK::Util::MaxStringDataEncoding.
	*					The actual code page can be retrieved this way : "Encoding() & MSDE_CP_MASK"
	*					\see TextFileReaderEncoding
	*/
	unsigned int Encoding() const;

	/**
	* Write string to file 
	*
	* \param	string		The string to write
	* \param	nbchars		The size of the string in characters.  -1 will write all chars up to '\0'
	* \return				Number of characters written.  
	*						(size_t)-1 if error.
	**/
	size_t Write(const char* string, size_t nbchars = (size_t)-1);

	/**
	* Write string to file 
	*
	* \param	string		The string to write.
	* \param	nbchars		The size of the string in characters.  -1 will write all chars up to '\0'
	* \return				Number of characters written.  
	*						(size_t)-1 if error.
	**/
	size_t Write(const wchar_t* string, size_t nbchars = (size_t)-1);

	/**
	* Write string to file 
	*
	* \param string		The string to write
	* \return				Number of characters written.  
	*						(size_t)-1 if error.
	**/
	size_t Write(const MaxString& string);

	/**
	* Write an ASCII char
	*
	* \param	character	The character to write
	* \return				Number of characters written.  
	*						-1 if error
	**/
	size_t WriteChar(char character);

	/**
	* Write an ASCII char
	*
	* \param	character	The character to write
	* \return				Number of bytes written.  
	*						-1 if error
	**/
	size_t WriteChar(wchar_t character);

	/**
	* Returns true if we can write to file
	*/
	bool CanWrite() const;

	/**
	* Returns true if file is open
	*/
	virtual bool IsFileOpen() const;

	/**
	* Returns true if file pointer is at end if file
	*/
	virtual bool IsEndOfFile() const;

	/** 
	* Get the current position in the file. 
	*/
	virtual size_t Position() const;

	/**
	* Seek inside the stream.
	*
	* \param offset    The seeks operations are done in number of characters (not bytes).
	* \param origin	The direction to move.  Origin can be one of the following
	*  * SEEK_CUR 	 Current position of file pointer.
	*  * SEEK_END	 End of file.
	*  * SEEK_SET	 Beginning of file.
	* \return Returns the absolute position of the text file. (in chars)
	*/
	virtual size_t Seek(long offset, int origin);

	/**
	* Make sure that all the buffer are synced with the OS native objects.
	*/
	void Flush();
};

/**
 * This class is used to access text file with both reader and writer functionality.
 * \see Reader class when file IO requirements are only for reading.
 * \see Writer class when file IO requirements are only for writing.
 */
class UtilExport ReaderWriter 
	: public BaseTextReader
	, public BaseTextWriter
{
protected:

#pragma region Protected members and data

	BinaryStream*	_stream;
	bool			_closeOnDelete;
	unsigned int	_streamMode;
	LineEndMode		_endOfLineMode;
	unsigned int	_encoding;
	unsigned int*  _chars;
	size_t         _allocated;
	mutable size_t _len;
	mutable size_t _pos;
	bool           _dirty;

	/**
	 * Open a file or stream for both read and write actions
	 *
	 * \param [in,out]	stream	If non-null, the stream.
	 * \param	encoding		The file encoding or preferred encoding
	 * \param	streamMode		Streaming mode \see BinaryStreamMode
	 * \param	mode			Text file end of line termination mode. Default Text
	 * \param	closeOnDelete	true to close on delete. Default true
	 *
	 * \return					true if it succeeds, false if it fails.
	**/

	bool Open(
			BinaryStream* stream,
			unsigned int  streamMode,
			unsigned int  encoding,
			LineEndMode	  mode = Text,
			bool          closeOnDelete = true);

	/**
	* Used to seek to a position in the file
	*
	* \param	offset			Position we which we want to position the cursor.
	*
	* \return					The current position in the file
	**/

	size_t SeekToAbsolute(long offset);

	/**
	* Used to seek to end. 
	*
	* \param	offset			The nchars.
	*
	* \return					The current position in the file
	**/

	size_t SeekToEnd(long offset = 0);

	/**
	* Advance "offset" characters.
	*
	* \returns Returns the absolute position of the text file. (in chars)
	*/
	size_t SeekFromCurrent(long offset);

	/**
	* Unread the passed string
	*
	* \param	string			The string that will be put back into the buffer
	*
	* \return					size in nb of characters that has been written
	**/
	virtual size_t Unread(const MaxString& string);

#pragma endregion

public:
	/** Default Constructor */
	ReaderWriter();
	/** Destructor */
	virtual ~ReaderWriter();

	friend class CharBinaryStream;

	/**
	 * Open a file using a file name
	 *
	 * \param fileName	File name to open.  If file does not exist, it will be created.
	 * \param streamMode	Streaming mode \see BinaryStreamMode
	 * \param encoding This parameter can contains hint to the detection algorithm.
	 *                 Acceptable values are all codepages numbers that are recognized by
	 *                 Windows.
	 *
	 *                 In addition to that, you can also specify FAVOR_UTF8. It can be used
	 *                 to cascade the detection of the codepage. For example, if you specify
	 *                 "CP_ACP | FAVOR_UTF8", the detection algorithm will treat any non-UTF8
	 *                 data as ACP.
	 *				   \see TextFileReaderEncoding
	 * \param mode	   \see LineEndMode - default Text
	 * \return		   true if successful, false otherwise
	 */
	bool Open(const MCHAR* fileName, unsigned int  streamMode, unsigned int encoding = 0, LineEndMode mode = Text);

	/**
	 * Open a file using a file name
	 *
	 * \param fileName	File name to open.  If file does not exist, it will be created.
	 * \param streamMode	Streaming mode \see BinaryStreamMode
	 * \param encoding This parameter can contains hint to the detection algorithm.
	 *                 Acceptable values are all codepages numbers that are recognized by
	 *                 Windows.
	 *
	 *                 In addition to that, you can also specify FAVOR_UTF8. It can be used
	 *                 to cascade the detection of the codepage. For example, if you specify
	 *                 "CP_ACP | FAVOR_UTF8", the detection algorithm will treat any non-UTF8
	 *                 data as ACP.
	 *				   \see TextFileReaderEncoding
	 * \param mode	   \see LineEndMode - default Text
	 * \return		   true if successful, false otherwise
	 */
	bool Open(const MaxSDK::Util::MaxString& fileName, unsigned int  streamMode, unsigned int encoding = 0, LineEndMode mode = Text);

	/**
	 * Close the opened file
	**/
	void Close();

	/**
	 * Flush to file the cached data
	**/
	void Flush();

	/**
	 * Seek inside the file.
	 *
	 * \param offset    The seeks operations are done in number of characters (not bytes).
	 * \param origin	The direction to move.  Origin can be one of the following
	 *  * SEEK_CUR 		Current position of file pointer.
	 *  * SEEK_END		End of file.
	 *  * SEEK_SET		Beginning of file.
	 * \return Returns the absolute position of the text file. (in chars)
	 */
	virtual size_t Seek(long offset, int origin);

	/**
	 * Read a character from the file
	 *
	 * \param peek			Read a char but does not move the internal pointer to next char.
	 * \return				Character read.
	 */
	virtual Char ReadChar(bool peek = false) const;

	/**
	 * Read a char in UTF32 encoding
	 *
	 * \param peek				Read a char but does not move the internal pointer to next char.
	 *
	 * \return					The UTF32 character.
	**/
	virtual unsigned int ReadCharUTF32(bool peek = false) const;

	/**
	 * Reads characters from the file
	 *
	 * \param nchars         Stop reading after 'nchars' characters.
	 * \return				 Line read.
	 */
	virtual MaxString ReadChars(size_t nchars) const;

	/**
	 * Read a line from the file
	 *
	 * \param	nchars			Max number of characters to read
	 *							Default -1 - read until end of line character
	 * \param	dontReturnEOL	Control if end of line character is in the returned string
	 *
	 * \return					The line read
	**/
	virtual MaxString ReadLine(size_t nchars = (size_t)-1, bool dontReturnEOL = false) const;

	/**
	 * Used to determine if we read the end of file
	 *
	 * \return					true if end of file, false if not.
	**/
	virtual bool IsEndOfFile() const;

	/**
	 * used to determine the current position in the file
	 *
	 * \return					The position in the file
	**/
	virtual size_t Position() const;

	/**
	 * Used to obtain the current line number
	 *
	 * \return					The line number
	**/
	virtual size_t LineNumber() const;

	/**
	* Unread the passed character
	 *
	 * \param	c				The character that will be put back into the buffer
	 * \return					Number of character written
	**/
	virtual size_t UnreadChar(const Char& c);

	/**
	* Write string to file
	 *
	 * \param	string			The string to write
	 * \param	nchars			The number of characters to write.  Alternatively, this parameter can be set to -1 
	 *							if the string is null-terminated. Note that, if string is NULL, the function fails.
	 *							If this parameter is -1, the function processes the entire input string, up to the terminating NULL.
	 *							If this parameter is set to a positive integer, the function processes exactly the 
	 *							specified number of characters up to the terminating NULL.
	 * \return					The number of characters written
	**/
	virtual size_t Write(const char* string, size_t nchars = (size_t)-1);

	/**
	* Write string to file
	 *
	 * \param	string			The string to write
	 * \param	nchars			The number of characters to write.  Alternatively, this parameter can be set to -1 
	 *							if the string is null-terminated. Note that, if string is NULL, the function fails.
	 *							If this parameter is -1, the function processes the entire input string, up to the terminating NULL.
	 *							If this parameter is set to a positive integer, the function processes exactly the 
	 *							specified number of characters up to the terminating NULL.
	 * \return					The number of characters written
	**/
	virtual size_t Write(const wchar_t*string, size_t nchars = (size_t)-1);

	/**
	* Write string to file
	*
	* \param	string			The string to write
	* \return					The number of characters written
	**/
	virtual size_t Write(const MaxString& string);

	/**
	 * Determine if we can write to the file
	 *
	 * \return					true if we can write, false otherwise
	**/
	virtual bool CanWrite() const;

	/**
	 * Returns true if file is open
	 */
	virtual bool IsFileOpen() const;

	void EnsureAllocated(size_t s);
};

/**
 * Convert the codepage name string to the codepage number in codPageTable
 * \param  codePageName The name of the code page
 * \param  codePage number corresponding to the input name
 * \return True if the input codepage name is found
 *
 * The codepage numbers corresponding to the codepage names:
 * {_M("utf-8"),          CP_UTF8},
 * {_M("windows-1252"),   1252},
 * {_M("iso-8859-13"),    28603},
 * {_M("iso-8859-15"),    28605},
 * {_M("iso-8859-1"),     28591},
 * {_M("iso-8859-2"),     28592},
 * {_M("iso-8859-3"),     28593},
 * {_M("iso-8859-4"),     28594},
 * {_M("iso-8859-5"),     28595},
 * {_M("iso-8859-6"),     28596},
 * {_M("iso-8859-7"),     28597},
 * {_M("iso-8859-8"),     28598},
 * {_M("iso-8859-9"),     28599},
 * // Asiatic code pages.
 * {_M("koi8-r"),         20866}, // Russian (KOI8-R); Cyrillic (KOI8-R)
 * {_M("iso-2022-kr"),    50225}, // ISO 2022 Korean
 * {_M("iso-2022-jp"),    50222}, // ISO 2022 Japanese JIS X 0201-1989; Japanese (JIS-Allow 1 byte Kana - SO/SI)
 * {_M("shift_jis"),      932},   // ANSI/OEM Japanese; Japanese (Shift-JIS)
 * {_M("gb2312"),         936},   // ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)
 * {_M("ks_c_5601"),      949},   // ANSI/OEM Korean (Unified Hangul Code)
 * {_M("big5"),           950},   // ANSI/OEM Traditional Chinese (Taiwan; Hong Kong SAR, PRC); Chinese Traditional (Big5)
 * {_M("utf-16"),         65012},
 * {NULL,             0},
 */
UtilExport bool GetCodePageNumberFromString(const MCHAR* codePageName, unsigned int& codePage);


}}}

