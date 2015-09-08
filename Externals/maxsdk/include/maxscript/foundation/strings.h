/*	
 *		Strings.h - string family for MAXScript
 *
 *			Copyright (c) John Wainwright 1996
 *
 */

#pragma once

#include "../foundation/streams.h"
#include "../../assetmanagement/AssetUser.h"
#include "../../strclass.h"

#ifndef DOXYGEN
visible_class_debug_ok (String)
#endif

// Collectable::flags3 - bit 0 set if string is a literal
class String : public Value
{
	MSTR string;
	const MaxSDK::AssetManagement::AssetId* pAssetId;

protected:
	void init(const MSTR& init_string, const MaxSDK::AssetManagement::AssetId* assetId);
	void release_asset();

public:
 ScripterExport String(const MCHAR *init_string);
 ScripterExport String(const MSTR& init_string);
 ScripterExport String(const MCHAR *init_string, const MaxSDK::AssetManagement::AssetId& assetId);
 ScripterExport String(const MSTR& init_string, const MaxSDK::AssetManagement::AssetId& assetId);
			   ~String();

				classof_methods (String, Value);
#	define		is_string(v) ((DbgVerify(!is_sourcepositionwrapper(v)), (v))->tag == class_tag(String))
	void		collect();
	ScripterExport void		sprin1(CharStream* s);

	Value*		append(const MSTR& str);
	Value*		append(const MCHAR* str);
	Value*		append(Value* str_val) { return append(str_val->to_string()); }

	const MaxSDK::AssetManagement::AssetId* GetAssetId();


#include "../macros/define_implementations.h"
#	include "../protocols/strings.inl"

	def_property( count );

	const MCHAR*		to_string();
	MSTR		to_mstr();
	MSTR		to_filename();
	void		to_fpvalue(FPValue& v) { v.s = to_string(); v.type = TYPE_STRING; }
	Value*		eval(); // returns new string if string is a literal

	// scene I/O 
	IOResult Save(ISave* isave);
	static Value* Load(ILoad* iload, USHORT chunkID, ValueLoader* vload);

	friend class StringStream;
};

applyable_class_debug_ok (StringStream)

class StringStream : public CharStream
{
public:
	MCHAR*	content_string;		/* the content string				*/
	MaxSDK::Util::CharIterator<MCHAR> cursor;				/* current read/write cursor		*/
	size_t	buf_len;			/* allocated buffer length			*/
 	int		ungetchar_count;
 	Parser* reader;

	ScripterExport StringStream();
	ScripterExport ~StringStream();
	ScripterExport StringStream(const MCHAR* init_string);
	ScripterExport StringStream(int ilen);
	ScripterExport StringStream(Value* init_string_value);
	ScripterExport void init(const MCHAR* init_string);

			classof_methods (StringStream, CharStream);
#	define	is_stringstream(v) ((DbgVerify(!is_sourcepositionwrapper(v)), (v))->tag == class_tag(StringStream))
	void	collect();
	void	gc_trace();
	ScripterExport void	sprin1(CharStream* s);

 #include "../macros/define_implementations.h"
 #	include "../protocols/streams.inl"
 	use_generic( coerce,  "coerce");
	use_generic( copy,    "copy");
	use_generic( append,  "append");

	Value* get_property(Value** arg_list, int count);
	Value* set_property(Value** arg_list, int count);

	// only want CharStream::file_name to be used in context of resolving resource file name
	Value*	get_file_name() { return NULL; } 

	ScripterExport MaxSDK::Util::Char	get_char();
	ScripterExport void               unget_char(const MaxSDK::Util::Char& c);
	ScripterExport MaxSDK::Util::Char	peek_char();

 	ScripterExport unsigned int		pos();
				   void		seek(int pos);
	ScripterExport int		at_eos();
	ScripterExport void		rewind();
	ScripterExport void		set_size(int size);
	ScripterExport void		flush_to_eol();
	ScripterExport void		flush_to_eobuf();
	ScripterExport void		undo_lookahead();

	ScripterExport	const MCHAR*	puts(const MCHAR* str);
	ScripterExport	MaxSDK::Util::Char putch(const MaxSDK::Util::Char& c);
	ScripterExport	int		printf(const MCHAR *format, ...);

	const MCHAR*	to_string();
	MSTR	to_mstr();
	void	to_fpvalue(FPValue& v) { v.s = to_string(); v.type = TYPE_STRING; }
};

// Note that the following functions will throw a runtime error if an invalid symbolic
// pathname is specified (for example $maxroot\....)
ScripterExport MSTR expand_file_name(const MSTR& file_name);
ScripterExport MSTR expand_file_dir(const MSTR& dir_name);

// Safe versions of the above which do not throw runtime errors. If an invalid symbolic
// pathname is specified (for example $maxroot\....), the returned MSTR is the same as
// the incoming MSTR.
ScripterExport MSTR expand_symbolic_pathname_in_file_name(const MSTR& file_name);
ScripterExport MSTR expand_symbolic_pathname_in_dir_name(const MSTR& dir_name);
