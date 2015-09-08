//
// Copyright 2013 Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
//
//

#pragma once

#include "./Variable.h"
#include "../../maxtypes.h"

namespace MaxSDK { namespace Graphics {
	
	/** Built-in char input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_Char;
	/** Built-in unsigned char input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_UnsignedChar;
	/** Built-in wchar_t input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_WideChar;
	/** Built-in short input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_Short;
	/** Built-in unsigned short input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_UnsignedShort;
	/** Built-in int input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_Int;
	/** Built-in unsigned int input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_UnsignedInt;
	/** Built-in long input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_Long;
	/** Built-in unsigned long input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_UnsignedLong;
	/** Built-in __int64 input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_Int64;
	/** Built-in unsigned __int64 input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_UnsignedInt64;
	/** Built-in float input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_Float;
	/** Built-in double input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_Double;
	/** Built-in multi-byte string input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_StringA;
	/** Built-in Unicode string input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_StringW;
	/** Built-in dictionary input/output type. This can be used in the constructor of 
	class Input/Output. 
	*/
	FragmentGraphAPI extern Class_ID TYPE_ID_Dictionary;

	class Input;
	class Output;
	class Fragment;
	
	/** ConnectionPoint is the base class that manages the connections between 
	view fragments. It stores the connection pointer, a pointer that points back 
	to its owner fragment, and the input/output data type. Only inputs/outputs 
	that have compatible data types can be connected together.
	*/
	class ConnectionPoint : public MaxHeapOperators
	{
	public:
		/** Destructor.
		*/
		FragmentGraphAPI virtual ~ConnectionPoint();
		
		/** Gets the pointer to owner view fragment.
		\return the pointer to the owner fragment. 
		*/
		Fragment* GetOwnerFragment() const
		{
			return mpOwnerFragment;
		}
		
		/** Gets the data type of this connection point. Once the connection point 
		is created, its data type is fixed and cannot be changed.
		\return the data type of this connection point. 
		*/
		const Class_ID& GetTypeID() const
		{
			return mVarTypeID;
		}

	protected:
		// The following function are used for initializing the connection point.
		friend class Fragment;

		ConnectionPoint(Fragment* pOwnerFragment, const Class_ID& typeID);

	protected:
		Fragment* mpOwnerFragment;
		Class_ID mVarTypeID;

	private:
		ConnectionPoint();
		ConnectionPoint(const ConnectionPoint&);
		ConnectionPoint& operator = (const ConnectionPoint&);
	};

	/** Class that stores input information for a fragment. An input can only be 
	connected to an output.
	*/
	class Input : public ConnectionPoint
	{
	protected:
		friend class Output;
		friend class Fragment;

		FragmentGraphAPI Input(Fragment* pOwnerFragment, const Class_ID& typeID);
		FragmentGraphAPI void OnVarSourceDeleted();

		Output* mpVarSource;

	public:
		/** Destructor.
		*/
		FragmentGraphAPI virtual ~Input();

		/** Gets the data source that connects to this input. 
		\return the data source that connects to this input.
		*/
		Output* GetVarSource() const
		{
			return mpVarSource;
		}

		/** Sets the source of this input. This source could be an output from a fragment node or 
			a fragment graph.
		\param[in] pVarSource the source of this input
		\return true if the type of source is compatible with the type of this input,
						false otherwise.
		*/
		FragmentGraphAPI bool SetVarSource(Output* pVarSource);

	private:
		Input();
		Input(const Input&);
		Input& operator = (const Input&);
	};
	
	/** Class that stores output information for a fragment. An output can be 
	connected to multiple inputs.
	*/
	class Output : public ConnectionPoint
	{
	protected:
		friend class Input;
		friend class Fragment;

		FragmentGraphAPI Output(Fragment* pOwnerFragment, const Class_ID& typeID);
		FragmentGraphAPI void AddVarTarget(Input* pVarTarget);
		FragmentGraphAPI void RemoveVarTarget(Input* pVarTarget);

	public:
		/** Destructor.
		*/
		FragmentGraphAPI virtual ~Output();
		
		/** Gets the number of connected inputs.
		\return the number of connected inputs.
		*/
		FragmentGraphAPI size_t GetNumberOfVarTargets() const;
		/** Gets the i-th connected input.
		\param[in] i the index of the input connected to this output class.
		\return pointer to the i-th connected input.
		*/
		FragmentGraphAPI Input* GetVarTarget(size_t i) const;
		
		/** Gets the variable that stores in the output.
		\return the variable that stores in the output.
		*/
		const Variable& GetVar() const
		{
			return mVar;
		}

		/** Update the variable that stores in the output.
		\return true if the type of the variable is compatible with the type of 
				this output, false otherwise.
		*/
		FragmentGraphAPI bool SetVar(const Variable& v);

	public:
		/** Evaluate the current output. If this output has an owner fragment,
			the owner fragment will also be evaluated.
			\param[in] evaluationContext context info for evaluation.
			\return true if succeed, false otherwise.
		*/
		FragmentGraphAPI virtual bool Evaluate(void* evaluationContext);

	private:
		Variable mVar;
		size_t mNumVarTargets;
		size_t mNumVarTargetsCapacity;
		Input** mppVarTargets;

	private:
		Output();
		Output(const Output&);
		Output& operator = (const Output&);
	};

} } // namespace MaxSDK::Graphics
