/*	
 *		Name.h - Name class for MAXScript
 *
 *			Copyright (c) John Wainwright 1996
 *
 */

#pragma once

#include "../ScripterExport.h"
#include "../kernel/value.h"
#include "../../strclass.h"
// forward declarations
class HashTable;

#ifndef DOXYGEN
visible_class_debug_ok (Name)
#endif

class Name : public Value
{
public:
	MSTR		string;
	static		HashTable* intern_table;
				Name(const MSTR& init_string);
	ScripterExport ~Name();

#	define		is_name(v) ((DbgVerify(!is_sourcepositionwrapper(v)), (v))->tag == class_tag(Name))
	static void	setup();
	static ScripterExport Value* intern(const MCHAR* str);
	static ScripterExport Value* intern(const MSTR& str);
	static ScripterExport Value* find_intern(const MCHAR* str);
				classof_methods (Name, Value);
	
	ScripterExport void sprin1(CharStream* s);
	void		collect();
	const MCHAR*		to_string();
	MSTR		to_mstr();
	MSTR		to_filename();
	void		to_fpvalue(FPValue& v) { v.s = to_string(); v.type = TYPE_NAME; }

#include "../macros/define_implementations.h"
	use_generic( coerce,	"coerce");
	use_generic( gt,		">");
	use_generic( lt,		"<");
	use_generic( ge,		">=");
	use_generic( le,		"<=");
	use_generic( copy,		"copy");

	// scene I/O 
	IOResult Save(ISave* isave);
	static Value* Load(ILoad* iload, USHORT chunkID, ValueLoader* vload);

};

/* core interned names */

#include "../macros/define_external_functions.h"
#	include "../protocols/corenames.inl"


