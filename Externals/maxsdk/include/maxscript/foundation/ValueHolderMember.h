/* a simple C++ class that controls the lifetime of a MAXScript ValueHolder class instance.

These classes are useful when you want to automatically protect a value from garbage collection without making that
value heap permanent. For example, if you want to hold a mxs function value as a callback in some C++ code, you don't
want to mark the mxs function value as heap permanent, because when you release it you would want to make it collectable
again, but the function could be registered somewhere else that also wants it heap permanent. By marking it as collectable,
the function value could be garbage collected while still in use.
*/

#pragma once
#include "../../maxheap.h"
#include "../ScripterExport.h"

class Value;
class ValueHolder;

/* A C++ class that creates a ValueHolder as necessary when a Value* is assigned to it (either in its constructor or via
set_value) and protects the ValueHolder from garbage collection. The ValueHolder holds a pointer to the specified Value*. 
Since the ValueHolder is protected from garbage collection, the specified value is also protected from garbage collection.
The destructor makes the ValueHolder collectable. 

Example:

class LoopExit : public MAXScriptException
{
public:
  ValueHolderMember	loop_result;  // protects loop exit value from garbage collection
  LoopExit (Value* v) 
  { 
      loop_result.set_value(v);
  }
  ~LoopExit () 
  { 
      // destructor for loop_result will set the ValueHolder it holds a pointer to to collectable, allowing
      // it and potentially the Value* it pointed to to be collected at the next garbage collection.
  }
  void sprin1(CharStream* s)
  {
      s->puts(_T("-- "));
      s->printf(MaxSDK::GetResourceStringAsMSTR(IDS_EXIT_NOT_INSIDE_A_LOOP));
  }
};


*/
class ScripterExport ValueHolderMember : public MaxHeapOperators
{
	friend class ValueHolder;
	ValueHolder* mValueHolder;
public:
	ValueHolderMember(Value* val = NULL);
	ValueHolderMember(const ValueHolderMember&);
	~ValueHolderMember();
	ValueHolderMember& operator=(const ValueHolderMember&);

	operator const Value*() const { return get_value(); }
	operator Value*() const { return get_value(); }

	Value*	get_value() const;
	void	set_value(Value* val);
};
