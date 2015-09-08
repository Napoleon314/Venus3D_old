/* a simple MAXScript Value Holder class that holds another value

Also see class ValueHolderMember which is a C++ class that controls the lifetime of a 
ValueHolder class instance.

These classes are useful when you want to automatically protect a value from garbage collection without making that
value heap permanent. For example, if you want to hold a mxs function value as a callback in some C++ code, you don't
want to mark the mxs function value as heap permanent, because when you release it you would want to make it collectable
again, but the function could be registered somewhere else that also wants it heap permanent. By marking it as collectable,
the function value could be garbage collected while still in use.
*/

#pragma once

#include "../kernel/value.h"

// forward declaration
class ValueHolderMember;

/* a simple Value Holder class that holds another value. By creating this class as GC_PERMANENT, it protects
the value being held from garbage collection. Make sure you call make_collectable on the Value Holder value
when you are done using it.

Example:

ValueHolder* mValueHolder = NULL;
mValueHolder = new(GC_PERMANENT)ValueHolder(); // mValueHolder protected from garbage collection
mValueHolder->set_value(new HashTable); // mValueHolder protects the HashTable from garbage collection
mValueHolder->make_collectable(); // mValueHolder no longer protected from garbage collection, allowing it and HashTable to be collected
*/

invisible_class (ValueHolder)

class ValueHolder : public Value
{
	friend ValueHolderMember;
	Value*	mVal;
	ValueHolderMember*	mOwner;
public:
	ValueHolder(Value* val = NULL, ValueHolderMember* owner = NULL);
	~ValueHolder();
	classof_methods (ValueHolder, Value);
	void	collect();
	void	gc_trace();
	void	sprin1(CharStream* s);

	Value*	get_value() const { return mVal; }
	void	set_value(Value* val) { mVal = heap_ptr(val); }

#	define	is_value_holder(v) ((DbgVerify(!is_sourcepositionwrapper(v)), (v))->tag == &ValueHolder_class)
};
