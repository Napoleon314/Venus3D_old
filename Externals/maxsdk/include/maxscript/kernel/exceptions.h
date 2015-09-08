/*		Exception.h - exception class for MAXScript
 *
 *		Copyright (c) John Wainwright, 1996
 *		
 *
 */

#pragma once

#include "../ScripterExport.h"
#include "../../strbasic.h"           // MCHAR
#include "../../WindowsDefines.h"
#include "../foundation/ValueHolderMember.h"

class Value;
class Thunk;
class ValueMetaClass;
class CharStream;

extern MCHAR* null_string;

class ScripterExport MAXScriptException
{
public:
	MAXScriptException() {}
	virtual ~MAXScriptException() {}
	virtual void sprin1(CharStream* s);
};

class ScripterExport UnknownSystemException : public MAXScriptException
{
public:
			UnknownSystemException() {}
	void	sprin1(CharStream* s);
};

class ScripterExport SignalException : public MAXScriptException
{
public:
	void	sprin1(CharStream* s);
};

class ScripterExport CompileError : public MAXScriptException
{
public:
	MCHAR*	description;
	MCHAR*	info;
	MCHAR*  line;
	MCHAR*	file;
			CompileError (const MCHAR* d, const MCHAR* i, const MCHAR* l, const MCHAR* f = null_string);
			CompileError () { description = NULL; info = null_string; line = null_string; file = null_string; }
		   ~CompileError ();

	void	sprin1(CharStream* s);
	void	set_file(const MCHAR* f);
};

class ScripterExport SyntaxError : public CompileError
{
	MCHAR*	wanted;
	MCHAR*	got;
public:
			SyntaxError (const MCHAR* w, const MCHAR* g, const MCHAR* l = null_string, const MCHAR* f = null_string);
		   ~SyntaxError ();

	void	sprin1(CharStream* s);
};

class ScripterExport TypeError : public MAXScriptException
{
	ValueHolderMember	target;
	ValueMetaClass* wanted_class;
	MCHAR*	description;
public:
			TypeError (const MCHAR* d, Value* t, ValueMetaClass* c = NULL);
		   ~TypeError ();

	void	sprin1(CharStream* s);
};

class ScripterExport NoMethodError : public MAXScriptException
{
	ValueHolderMember	target;
	MCHAR*	fn_name;
public:
			NoMethodError (const MCHAR* fn, Value* t);
		   ~NoMethodError ();

	void	sprin1(CharStream* s);
};

#define unimplemented(m, t) throw NoMethodError (m, t)

class ScripterExport AccessorError : public MAXScriptException
{
	ValueHolderMember	target;
	ValueHolderMember	prop;
public:
			AccessorError (Value* t, Value* p);

	void	sprin1(CharStream* s);
};

class ScripterExport AssignToConstError : public MAXScriptException
{
	ValueHolderMember	thunk;
public:
			AssignToConstError (Thunk* t);

	void	sprin1(CharStream* s);
};

class ScripterExport ArgCountError : public MAXScriptException
{
	int		wanted;
	int		got;
	MCHAR*	fn_name;
public:
			ArgCountError (const MCHAR* fn, int w, int g);
		   ~ArgCountError ();

	void	sprin1(CharStream* s);
};

class ScripterExport RuntimeError : public MAXScriptException
{
public:
			MCHAR*	desc1;
			MCHAR*  desc2;
			ValueHolderMember  info;
			RuntimeError (const MCHAR* d1);
			RuntimeError (const MCHAR* d1, const MCHAR* d2);
			RuntimeError (const MCHAR* d1, Value* ii);
			RuntimeError (const MCHAR* d1, const MCHAR* d2, Value* ii);
			RuntimeError (Value* ii);
		   ~RuntimeError ();

	void	init(const MCHAR* d1, const MCHAR* d2, Value* ii);
	void	sprin1(CharStream* s);
};

class ScripterExport UserThrownError : public MAXScriptException
{
public:
			MCHAR*	desc;
			ValueHolderMember  info;
			BOOL	debugBreak;
			UserThrownError (const MCHAR* d1, Value* ii, BOOL dbgBreak);
			UserThrownError (const MCHAR* di, BOOL dbgBreak);
		   ~UserThrownError ();

	void	init(const MCHAR* d1, Value* ii, BOOL dbgBreak);
	void	sprin1(CharStream* s);
};

class ScripterExport DebuggerRuntimeError : public MAXScriptException
{
public:
	MCHAR*	desc1;
	MCHAR*  desc2;
	ValueHolderMember  info;
	DebuggerRuntimeError (const MCHAR* d1);
	DebuggerRuntimeError (const MCHAR* d1, const MCHAR* d2);
	DebuggerRuntimeError (const MCHAR* d1, Value* ii);
	DebuggerRuntimeError (const MCHAR* d1, const MCHAR* d2, Value* ii);
	DebuggerRuntimeError (Value* ii);
	~DebuggerRuntimeError ();

	void	init(const MCHAR* d1, const MCHAR* d2, Value* ii);
	void	sprin1(CharStream* s);
};

class ScripterExport IncompatibleTypes : public MAXScriptException
{
	ValueHolderMember  val1;
	ValueHolderMember  val2;
public:
			IncompatibleTypes (Value* v1, Value* v2);

	void	sprin1(CharStream* s);
};

class ScripterExport ConversionError : public MAXScriptException
{
	ValueHolderMember	val;
	MCHAR*   type;
public:
			ConversionError (Value* v, const MCHAR* t);
		   ~ConversionError ();

	void	sprin1(CharStream* s);
};

class FunctionReturn : public MAXScriptException
{
public:
	ValueHolderMember	return_result;
			FunctionReturn (Value* v);

	void	sprin1(CharStream* s);
};

class LoopExit : public MAXScriptException
{
public:
	ValueHolderMember	loop_result;
			LoopExit (Value* v);

	void	sprin1(CharStream* s);
};

class LoopContinue : public MAXScriptException
{
public:
			LoopContinue () { }

	void	sprin1(CharStream* s);
};

