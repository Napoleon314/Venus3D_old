 /*	
  *		CharStream.h - MAXScript
  *
  *			Copyright © John Wainwright 1996
  *
  */
 
#pragma once
 
 class Listener;
 class FileStream;
 class StringStream;
 class Parser;
 
#include "../ScripterExport.h"
#include "../kernel/value.h"
#include "../compiler/codetree.h"
#include "../../maxchar.h"

 visible_class_debug_ok (CharStream)
 
// Collectable::flags3 - bit 1 set if have attempted to load resource file
// Collectable::flags3 - bit 2 set if Parser::get_char() doesn't echo character to error line buffer. This
//		is used when converting a resource id to resource value, where the string corresponding to the resource
//		value is placed in a stringstream, and that stringstream is set as the parser source. We don't want the
//		resource value string in the error buffer since the resource id is already there.
class CharStream : public Value
 {
 protected:
	 unsigned int	mlineNumber;
//	 bool			mTrippedLineFeed;

	 Value*			file_name;			// source file
	 HashTable*		resourceMap;		// resource hashtable associated with source file
	 StringStream*	resourceValueStream; // resource stream used to process the resource value string by the parser

 public:
	ScripterExport CharStream();
	ScripterExport virtual ~CharStream();

 					classof_methods (CharStream, Value);
 			BOOL	_is_charstream() { DbgAssert(!is_sourcepositionwrapper(this)); return 1; }
 #	define			is_charstream(v) ((v)->_is_charstream())

	virtual void	gc_trace();
 
 	virtual MaxSDK::Util::Char	get_char() = 0;
 	virtual void	unget_char(const MaxSDK::Util::Char& c) = 0;
 	virtual MaxSDK::Util::Char	peek_char() = 0;
 	virtual int		at_eos() = 0;
 	virtual void	rewind() = 0;

	virtual unsigned int pos();
	virtual unsigned int line(); // Get the current line

//	bool			getTrippedLineFeed(bool reset = true); // get whether passed a line feed since last setTrippedLineFeed(false)/reset
//	void			setTrippedLineFeed(bool val); // set whether passed a line feed

	virtual void	seek(int pos);
	virtual void	flush_to_eol() = 0;
 	virtual void	flush_to_eobuf();
 	virtual void	flush_whitespace();
 
 	virtual MaxSDK::Util::Char        putch(const MaxSDK::Util::Char& c)   { return c;   }
	virtual const MCHAR*              puts (const MCHAR* str)              { return str; }

	#pragma warning(push)
	#pragma warning(disable:4100)
 	virtual int		printf(const MCHAR* format, ...) { return 0; }
	#pragma warning(pop)
 	virtual void	close();
 	virtual void	flush();
 
 	virtual void	log_to(CharStream* log);
 	virtual void	close_log();
 	virtual Listener* get_listener() { return NULL; }
 	virtual CharStream* get_log() { return NULL; }

	virtual Value*	get_file_name();
	virtual void	set_file_name(const MCHAR* fileName); // just sets the file_name member variable
	virtual CharStream*	get_resource_value_stream(Value* resourceID);
 };
 