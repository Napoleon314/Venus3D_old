/*	
 *		Pipe.h - NT MCHAR Pipe wrapper for MAXScript
 *
 *			Copyright (c) John Wainwright 1996
 *
 */

#pragma once

#include "Strings.h"
class FileStream;

#define PIPE_BUF_SIZE	512

// The undelivered data in the pipe is held in a linked list of
// buffers, pointed into by read and write cursors.  
// A side list is kept if writers supply info about sourcing files.
// This is provided to readers like the compiler to add source
// tags to generated code.  

class src_info						
{
public:
	src_info*	 next;			// next marker
	MaxSDK::Util::Char* start;	// source start character in buffer chain
	Value*		 file;			// source file name if any
	unsigned int offset;		// starting offset into source
	unsigned int linenumber;    // line number in the source

	//Constructor / Destructor
	src_info();
	~src_info();
};

// Collectable::flags3 - bit 0 set if have attempted to load resource file
class Pipe : public CharStream	
{
private:
	unsigned int read_source_offset; // running reader offset in source
	CRITICAL_SECTION pipe_update;	// for syncing pipe updates
public:
	MaxSDK::Util::Char* write_buffer;		// pipe buffers & cursors
	MaxSDK::Util::Char* write_cursor;
	MaxSDK::Util::Char* read_buffer;
	MaxSDK::Util::Char* read_cursor;
	int			ungetch_count;

	HANDLE		pipe_event;			// for signalling data ready
	HANDLE		restart_event;		// used to restart a stopped pipe
	bool		waiting;			// reader is waiting for data
	bool		stopped;			// pipe reading is blocked

	FileStream* log;				// log stream if non-NULL

	src_info*	markers;			// marker list...
	src_info*	marker_tail;		
	MaxSDK::Util::Char* next_source_start;	// upcoming marker starting character
	Value*		write_source_file;	// current write source file, used to determine source change
	unsigned int write_source_offset;// running writer offset

	MaxSDK::Util::Char lastCharacterOfLastBuffer;	// used when looking back one character from current read cursor,
											// and read cursor is at the beginning of the read buffer

				Pipe();
				~Pipe();
				
#	define		is_pipe(v) ((DbgVerify(!is_sourcepositionwrapper(v)), (v))->tag == INTERNAL_PIPE_TAG)
	void		collect();
 	void		gc_trace();

	MaxSDK::Util::Char get_char  ();
	void               unget_char(const MaxSDK::Util::Char& c);
	MaxSDK::Util::Char peek_char ();

	int			at_eos();
	unsigned int pos(); // Gets the current position in the stream
	unsigned int line(); // Gets the current line number
	void		rewind();
	void		flush_to_eol();
	void		flush_to_eobuf();

	void		put_char(const MaxSDK::Util::Char& c, Value* source_file = NULL, unsigned int offset = 0, unsigned int line = 1);
	void		put_str(const MCHAR* str, Value* source_file = NULL, unsigned int  offset = 0, unsigned int line = 1);
	void		put_buf(const MaxSDK::Util::Char* str, UINT count, Value* source_file = NULL, unsigned int offset = 0, unsigned int line = 1);
	void		new_write_buffer();
	void		check_write_source_change(Value* file, unsigned int offset, unsigned int line, int new_len);
	void		read_source_change();
	void		clear_source();
	void		stop();
	void		go();

	const MCHAR*		puts(const MCHAR* str);
	int			printf(const MCHAR *format, ...);
	void		flag_eos(); // sets the end of script marker in the pipe's stream

	void		log_to(FileStream* log);
	void		close_log();
	CharStream* get_log() { return log; }
};

