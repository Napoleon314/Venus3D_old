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
// DESCRIPTION: Max Script File streams
// AUTHOR:      Richard Houle
// DATE:        April 29 2011
//***************************************************************************/

#ifndef __MAXSCRIPT_STREAMFILE_H__
#define __MAXSCRIPT_STREAMFILE_H__

#include "../../maxbinarystream.h"
#include "../../maxtextfile.h"
#include "CharStream.h"

class CharBinaryStream : public CharStream
{
protected:
	MaxSDK::Util::BinaryStream*   stream;
	MaxSDK::Util::TextFile::BaseTextReader* textReader;
	MaxSDK::Util::TextFile::BaseTextWriter* textWriter;

public:
	CharBinaryStream();
	virtual ~CharBinaryStream();

public:
	/**
	* Open a text file
	* \param[in]  stream The binary stream that holds the file.
	* \param[in]  mode The file open mode. See the MAXScript Help file, topic FileStream Values, for a description of the mode string values.
	* \param[in]  encoding The encoding to use if if can not be determined from the file contents, If -1, a default
	*			encoding based on the file language settings in Preference /Files (exposed through Interface14) will
	*			be used.
	* \return false if stream is null or this instance already has an open stream, true otherwise.
	*/
	bool Open(MaxSDK::Util::BinaryStream* stream, const MSTR& mode, unsigned int encoding = -1);

public:
 	virtual MaxSDK::Util::Char get_char  ();
 	virtual void               unget_char(const MaxSDK::Util::Char& c);
 	virtual MaxSDK::Util::Char peek_char ();

public:
 	virtual int		at_eos();
 	virtual void	rewind();
	virtual void	seek(int pos);

	virtual unsigned int pos();
	virtual unsigned int line();

public:
 	virtual void flush();
	virtual void flush_to_eol();

public:
 	virtual MaxSDK::Util::Char putch (const MaxSDK::Util::Char& c);
	virtual const MCHAR*       puts  (const MCHAR* str);
 	virtual int                printf(const MCHAR* format, ...);

public:
 	virtual void close();

public:
	bool IsEncrypted() const;

public:
	void check_file_is_open() const;
	void check_readable() const;
	void check_writeable() const;
	void check_is_not_eof() const;

protected:
	virtual void ThrowNotOpenedError() const;
	virtual void ThrowNotReadableError() const;
	virtual void ThrowNotWritableError() const;
	virtual void ThrowReachEOFError() const;
};

#endif
