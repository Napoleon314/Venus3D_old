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

#ifndef __MAXSDK_MAXBINARYSTREAM_H__
#define __MAXSDK_MAXBINARYSTREAM_H__

#include <stdlib.h>
#include <stdio.h>
#include "maxstring.h"

namespace MaxSDK { namespace Util {

class BinaryStreamMemory;

/** Enum describing file open mode options */
enum BinaryStreamMode
{
	/** Allow reading from a stream.  */
	BINARYSTREAM_MODE_READ = 1 << 0,

	/** Allow writing to a stream.  */
	BINARYSTREAM_MODE_WRITE = 1 << 1,
	
	/** When opened, a stream is positioned at the end. */
	BINARYSTREAM_MODE_APPEND = 1 << 2,

	/** When opened, the stream is truncated. */
	BINARYSTREAM_MODE_TRUNCATE = 1 << 3,

	/** If the underlying file doesn't exist, it will create it. */
	BINARYSTREAM_MODE_CREATE = 1 << 4,

	/**
	 * Specify that the underlying data should be interpreted as
	 * text. Technically, BinaryStream doesn't do anything with
	 * that flag. It's was made to use in conjunction with
	 * TextFile::Reader and TextFile::Writer.
	 */
	BINARYSTREAM_MODE_TEXT = 1 << 5,

	/** Specify that the underlying data should be considered binary. */
	BINARYSTREAM_MODE_BINARY = 1 << 6,

	/**
	 * By default, when a file is opened, another process always have
	 * the possibility to open the same file concurrently for reading.
	 * When this flag is present, it will prevent another process to
	 * to open it.
	 */
	BINARYSTREAM_MODE_EXCLUSIVE = 1 << 7,

	/**Write operations will not go through any intermediate cache, they will go directly to disk.*/
	BINARYSTREAM_MODE_COMMIT = 1 << 8,

	/**The file is being used for temporary storage.*/
	BINARYSTREAM_MODE_TEMPORARY = 1 << 9,

	/**The file is to be deleted immediately after all of its handles are closed*/
	BINARYSTREAM_MODE_DELETE = 1 << 10,

	/**Access is intended to be sequential from beginning to end. */
	BINARYSTREAM_MODE_SEQUENTIAL_SCAN = 1 << 11,

	/**Access is intended to be random.*/
	BINARYSTREAM_MODE_RANDOM_ACCESS = 1 << 12
};

/**
 * Represent an abstract binary stream of data.
 */
class UtilExport BinaryStream : public MaxHeapOperators 
{
public:
	typedef __int64 OffsetType;

private:
	/** Copy Constructor. Made private to prevent copying */
	BinaryStream(const BinaryStream&);

	/** Assignment Operator. Made private to prevent copying */
	BinaryStream& operator = (const BinaryStream&);

public:
	/** Default Constructor */
	inline BinaryStream() {}
	/** Destructor */
	virtual ~BinaryStream() {}

	/**
	 * Returns the last error encountered using the stream.
	 *
	 * If no error occurred during the uses of this class, this
	 * function will returns null. If for any reason, any call
	 * to this interface, this member will returns a user-readable
	 * error code that explain why the operation failed.
	 */
	virtual MaxString LastError() const = 0;

	/**
	 * Returns true when at end of file.
	 */
	virtual bool IsEndOfFile() const = 0;

	/**
	 * Returns true if a call to Write(void*, size_t) would succeed.
	 */
	virtual bool CanWrite() const = 0;

	/**
	 * Read a specified number bytes from this stream and stores it in the data parameter.
	 *
	 * \param[out]  data - Buffer to store the bytes taken out of this stream.
	 * \param  length - Number of bytes available in "data" to store the content of this stream.
	 *
	 * \return Returns the number of bytes read. Or 0 if the end of file was reached.
	 */
	virtual size_t Read(void* data, size_t length) = 0;

	/**
	 * Write a number of bytes from the data parameter to this stream
	 * \param[in] data - The byte source to copy from
	 * \param length - The number of bytes to copy from the data parameter. 
	 * \return Returns the number of bytes written.
	 */
	virtual size_t Write(const void* data, size_t length) = 0;

	/**
	 * Move the pointer inside this stream to a specified position.
	 * \param offset - The amount to offset the file pointer by.
	 * \param direction  - Can be either SEEK_SET, SEEK_CUR, SEEK_END.
	 */
	virtual OffsetType Seek(OffsetType offset, int direction) = 0;

	/**
	 * Make sure that all the buffer are synced with the OS native objects.
	 */
	virtual void Flush() = 0;

	/**
	 * Erase all the data after the current position.
	 */
	virtual void Truncate() = 0;

	/**
	 * Transform a C fopen-style file mode into a file mode that you can use in the
	 * BinaryStream::FromFilename parameter.
	 *
	 * Here is the table of interpretation. It was made to match Microsoft's VC Runtime
	 * interpretation:
	 *
	 *   r:        BINARYSTREAM_MODE_READ
	 *   r+ or rw: BINARYSTREAM_MODE_READ  | BINARYSTREAM_MODE_WRITE
	 *   w:        BINARYSTREAM_MODE_WRITE | BINARYSTREAM_MODE_CREATE | BINARYSTREAM_MODE_TRUNCATE
	 *   w+:       BINARYSTREAM_MODE_READ  | BINARYSTREAM_MODE_WRITE  | BINARYSTREAM_MODE_CREATE | BINARYSTREAM_MODE_TRUNCATE
	 *   a:        BINARYSTREAM_MODE_WRITE | BINARYSTREAM_MODE_APPEND | BINARYSTREAM_MODE_CREATE
	 *   a+:       BINARYSTREAM_MODE_READ  | BINARYSTREAM_MODE_WRITE  | BINARYSTREAM_MODE_APPEND | BINARYSTREAM_MODE_CREATE
	 *
	 * And you can add the following suffix:
	 *   b: BINARYSTREAM_MODE_BINARY
	 *   t: BINARYSTREAM_MODE_TEXT
	 *   e: BINARYSTREAM_MODE_EXCLUSIVE
	 *
	 * If the stream mode contains any invalid sequence or unknown character, it will
	 * returns 0.
	 */
	static unsigned int ParseStreamMode(const MCHAR* streamMode = _M("rt") );

	/**
	 * Create an abstract BinaryStream that will point to the specified file.
	 *
	 * \param fileName  The file's name you wish to open.
	 * \param mode      Specify how to open the file.
	 * \param error     If this parameter is not null, it will contains an error string
	 *                  if this function fails.
	 */
	static BinaryStream* FromFilename(const MaxString& fileName, unsigned int mode, MaxString* error = NULL);

	/**
	 * Create an abstract BinaryStream that will point to the specified file.
	 *
	 * \param fileName  The file's name you wish to open.
	 * \param write     Open for write ?
	 * \param truncate  Will erase the file prior to open it.
	 * \param exclusive Prevent any other object or process to open it simultaneously.
	 * \param error     If this parameter is not null, it will contains an error string
	 *                  if this function fails.
	 */
	static BinaryStream* FromFilename(const MaxString& fileName, bool write = false, bool truncate = false, bool exclusive = false, MaxString* error = NULL);

	/**
	 * Create an abstract BinaryStream out of a Win32 file handle.
	 * \param handle - The file handle.
	 * \param closeOnDelete - Will call CloseHandle(HANDLE) when the new object will be destroyed.
	 */
	static BinaryStream* FromFileHandle(HANDLE handle, bool closeOnDelete = true);

	/**
	 * Create an abstract BinaryStream out of a Win32 file handle.
	 * \param file - The file pointer.
	 * \param closeOnDelete - Will call fclose(FILE*) when the new object will be destroyed.
	 */
	static BinaryStream* FromFile(FILE* file, bool closeOnDelete = true);

	/**
	 * Create an abstract BinaryStream pointing to a memory region.
	 *
	 * \param data - Initial data.
	 * \param length - Initial data's length. (Can be 0.)
	 * \param freeFunction - Function to call when this object is destroyed. (Can be NULL.)
	 */
	static BinaryStreamMemory* FromMemory(void* data, size_t length, void (*freeFunction)(void*));

	/**
	 * Create an abstract BinaryStream pointing to a memory region.
	 *
	 * \param data - Initial data.
	 * \param length - Initial data's length. (Can be 0.)
	 * \param reallocFunction  - Function to call when data needs to be resized. (Can be NULL.)
	 * \param freeFunction - Function to call when this object is destroyed. (Can be NULL.)
	 */
	static BinaryStreamMemory* FromMemory(void* data, size_t length, void* (*reallocFunction)(void*, size_t), void (*freeFunction)(void*));
};

/**
	Holds binary data in memory only. The Reader class uses this as a buffer for reading data.
*/
class UtilExport BinaryStreamMemory : public BinaryStream
{
protected:
	void*       _data;
	size_t      _allocated;
	size_t      _len;
	size_t      _pos;

	void* (*_reallocFunction)(void*, size_t);
	void  (*_freeFunction)(void*);

public:
	BinaryStreamMemory(void* data, size_t length, void* (*reallocFunction)(void*, size_t), void (*freeFunction)(void*));
	virtual ~BinaryStreamMemory();

public:
	inline size_t      CurrentPosition() const { return _pos; }
	inline size_t      Length() const { return _len; }
	inline size_t      Remaining() const { return _len - _pos; }
	inline size_t      Allocated() const { return _allocated; }
	inline void*       Data() const { return _data; }
	inline void*       Data(size_t pos) const { return ((unsigned char*)_data) + pos; }
	inline void*       CurrentData() const { return ((unsigned char*)_data) + _pos; }

	/** Reduce the memory used by this object. */
	void Compact();

	/** Reset this object, _len  = 0 */
	void Reset();

	/** Loose the information the beginning of this buffer up to the current position.
	 *
	 * \param keep  Keep the X last bytes of the memory buffer
	 */
	void Precate(size_t keep = 0);

	/** Loose the information from the current position up to the end of this buffer. */
	void Truncate();

	/** Loose the information from the specified position up to the end of this buffer. */
	void Truncate(size_t);

	/** Quick-read. This function will returns you a direct-pointer to the memory of
	 *  this buffer and move the stream position. */
	void* Read(size_t& length);

	/** Quick-write. This function will returns you a direct-pointer to the memory of
	 *  this buffer where you should write and move the stream position. */
	void* Write(size_t& length);

	void EnsureAllocated(size_t length);

	void* Insert(size_t position, size_t length);

public:
	virtual MaxString   LastError() const;
	virtual bool        IsEndOfFile()   const;
	virtual bool        CanWrite()  const;
	virtual size_t      Read (void* data, size_t length);
	virtual size_t      Write(const void* data, size_t length);
	virtual OffsetType  Seek (OffsetType distance, int direction);
	virtual void        Flush();
};

}}

#endif
