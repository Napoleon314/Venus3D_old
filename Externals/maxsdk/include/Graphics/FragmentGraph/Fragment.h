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

#include "ConnectionPoint.h"
#include "../IRefObject.h"
#include "IVariableDictionary.h"
#include "../../tab.h"
#include "../../maxheap.h"

namespace MaxSDK { namespace Graphics {

	/** Use this value to unregister a timer with a fragment.
	\sa Fragment::SetTimerTicks(DWORD timerTicks)
	*/
	const size_t TIMER_NONE = (size_t)-1;

	/** This enum contains flags that specify certain states of a fragment or 
		fragment graph node.
	*/
	enum FragmentFlags
	{
		/** Indicates whether this fragment has already been evaluated during a 
			recursive call to FragmentGraph::Evaluate(). If this flag is true,
			we simply ignore evaluating this fragment an additional time.
		*/
		FragmentFlagsEvaluated = 0x00000001,
		/** Indicates whether this fragment is dirty and needs to be evaluated
			once more. Note this flag is mainly used to indicate if a viewport 
			redraw is required.
		*/
		FragmentFlagsDirty = 0x00000002,
	};

	/** This enum describes types of messages a fragment could register and receive 
		from the fragment system. Fragment::OnMessage is invoked whenever 
		a fragment receives the registered messages from the fragment system.
	*/
	enum FragmentMessageID
	{
		/** The timer message. A fragment may call Fragment::SetTimerTicks to register
			a timer. When the timer ticks, Fragment::OnMessage is invoked.
		*/
		FragmentMessageIDTimer,
		/** The end frame message. A fragment will receive this message at the end of 
			each frame.
		*/
		FragmentMessageIDEndFrame,
		/** Preserved count of built-in message.
		*/
		BuiltInFragmentMessageIDCount = 0xFF
	};

	/** Message parameters. 
	*/
	struct FragmentMessageParameter : public MaxHeapOperators
	{
		FragmentMessageParameter() : messageID(0),parameter1(0),
			parameter2(0),parameter3(0) {}

		size_t messageID;
		LONG_PTR parameter1;
		LONG_PTR parameter2;
		LONG_PTR parameter3;
	};

	/** Fragment is the base computation unit in the view system. A fragment has 
	multiple inputs and outputs. Fragments can be connected together as a 
	compute graph via those inputs and outputs. In this way, very complicated 
	view features can be achieved, for example, combining the mental-ray GI pass 
	with the Nitrous gizmo pass.
	*/
	class Fragment : public ARefObject
	{
	public:
		/** Constructor.
		*/
		FragmentGraphAPI Fragment();
		/** Destructor.
		*/
		FragmentGraphAPI virtual ~Fragment();

	protected:
		/** Initializes the inputs for the fragment. The inputs can only be 
		initialized once for each fragment. Once the inputs are initialized, 
		developers cannot change any of the inputs at runtime. 
		This function will initialize the fragment to one input with the given type.
		\param[in] type1 type of the first input.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool InitializeInputs(
			const Class_ID& type1);
		/** Initializes the inputs for the fragment. The inputs can only be 
		initialized once for each fragment. Once the inputs are initialized, 
		developers cannot change any of the inputs at runtime. 
		This function will initialize the fragment to two inputs with the given types.
		\param[in] type1 type of the first input.
		\param[in] type2 type of the second input.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool InitializeInputs(
			const Class_ID& type1, 
			const Class_ID& type2);
		/** Initializes the inputs for the fragment. The inputs can only be 
		initialized once for each fragment. Once the inputs are initialized, 
		developers cannot change any of the inputs at runtime. 
		This function will initialize the fragment to three inputs with the given types.
		\param[in] type1 type of the first input.
		\param[in] type2 type of the second input.
		\param[in] type3 type of the third input.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool InitializeInputs(
			const Class_ID& type1, 
			const Class_ID& type2, 
			const Class_ID& type3);
		/** Initializes the inputs for the fragment. The inputs can only be 
		initialized once for each fragment. Once the inputs are initialized, 
		developers cannot change any of the inputs at runtime. 
		This function will initialize the fragment to four inputs with the given types.
		\param[in] type1 type of the first input.
		\param[in] type2 type of the second input.
		\param[in] type3 type of the third input.
		\param[in] type4 type of the forth input.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool InitializeInputs(
			const Class_ID& type1, 
			const Class_ID& type2, 
			const Class_ID& type3, 
			const Class_ID& type4);
		/** Initializes the inputs for the fragment. The inputs can only be 
		initialized once for each fragment. Once the inputs are initialized, 
		developers cannot change any of the inputs at runtime. 
		This function will initialize the fragment to n input with the given types.
		\param[in] numberOfInputs the number of inputs for this fragment.
		\param[in] pInputTypes pointer to an array of the input types. This array 
		must be the same size as numberOfInputs.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool InitializeInputs(
			size_t numberOfInputs, 
			const Class_ID* pInputTypes);

		/** Initializes the outputs for the fragment. The outputs can only be 
		initialized once for each fragment. Once the outputs are initialized, 
		developers cannot change any of the outputs at runtime. 
		This function will initialize the fragment to one output with the given type.
		\param[in] type1 type of the first output.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool InitializeOutputs(
			const Class_ID& type1);
		/** Initializes the outputs for the fragment. The outputs can only be 
		initialized once for each fragment. Once the outputs are initialized, 
		developers cannot change any of the outputs at runtime. 
		This function will initialize the fragment to two outputs with the given types.
		\param[in] type1 type of the first output.
		\param[in] type2 type of the second output.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool InitializeOutputs(
			const Class_ID& type1, 
			const Class_ID& type2);
		/** Initializes the outputs for the fragment. The outputs can only be 
		initialized once for each fragment. Once the outputs are initialized, 
		developers cannot change any of the outputs at runtime. 
		This function will initialize the fragment to three outputs with the given types.
		\param[in] type1 type of the first output.
		\param[in] type2 type of the second output.
		\param[in] type3 type of the third output.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool InitializeOutputs(
			const Class_ID& type1, 
			const Class_ID& type2, 
			const Class_ID& type3);
		/** Initializes the outputs for the fragment. The outputs can only be 
		initialized once for each fragment. Once the outputs are initialized, 
		developers cannot change any of the outputs at runtime. 
		This function will initialize the fragment to four outputs with the given types.
		\param[in] type1 type of the first output.
		\param[in] type2 type of the second output.
		\param[in] type3 type of the third output.
		\param[in] type4 type of the forth output.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool InitializeOutputs(
			const Class_ID& type1, 
			const Class_ID& type2, 
			const Class_ID& type3, 
			const Class_ID& type4);
		/** Initializes the outputs for the fragment. The outputs can only be 
		initialized once for each fragment. Once the outputs are initialized, 
		developers cannot change any of the outputs at runtime. 
		This function will initialize the fragment to n outputs with the given types.
		\param[in] numberOfOutputs the number of outputs for this fragment.
		\param[in] pOutputTypes pointer to an array of the output types. This array 
		must be the same size as numberOfOutputs.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool InitializeOutputs(
			size_t numberOfOutputs, 
			const Class_ID* pOutputTypes);

		/** Create an input instance of this fragment. This function is called by
			InitializeInputs() when initializing inputs. This function could be 
			overridden if a fragment would like to customize its own implementation
			of fragment inputs.
			\param[in] typeID type of the input
			\return the input object corresponding to typeID.
		*/
		FragmentGraphAPI virtual Input* CreateInput(const Class_ID& typeID);

		/** Create an output instance of this fragment. This function is called by
			InitializeOutputs() when initializing outputs. This function could be 
			overridden if a fragment would like to customize its own implementation
			of fragment inputs.
			\param[in] typeID type of the output
			\return the output object corresponding to typeID.
		*/
		FragmentGraphAPI virtual Output* CreateOutput(const Class_ID& typeID);

		/** This function will be invoked when any fragment flag gets changed.
		\param[in] flag the flag that gets changed
		\param[in] oldValue the old value of the flag
		*/
		FragmentGraphAPI virtual void HandleFlagChanged(FragmentFlags flag, 
			bool oldValue);

		/** This function will be invoked when any fragment flag of a sub fragment
			gets changed.
		\param[in] pSubFragment the sub fragment whose flag gets changed
		\param[in] flag the flag that gets changed
		\param[in] oldValue the old value of the flag
		*/
		FragmentGraphAPI virtual void HandleSubFragmentFlagChanged(
			Fragment* pSubFragment,	FragmentFlags flag, bool oldValue);

	protected:
		/** Do evaluation. A concrete fragment class must override this function, 
		perform necessary computation in this function, and set value to its 
		output. 
		Please note this function cannot be directly called, it can only be called 
		indirectly by calling Fragment::Evaluate(). 
		\param[in] evaluationContext context info for evaluation.
		\return true on success, false otherwise. 
		*/
		virtual bool DoEvaluate(void* evaluationContext) = 0;

	public:
		/** Evaluates this fragment. Updates the output values. 
		\param[in] evaluationContext context info for evaluation.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool Evaluate(void* evaluationContext);
		
		/** Re-evaluates this fragment. We will first clear the FragmentFlagsEvaluated
			flag and then Call Evaluate(). 
		\param[in] evaluationContext context info for evaluation.	
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool ReEvaluate(void* evaluationContext);

		/** Gets the number of inputs for this fragment.
		\return the number of inputs of this fragment.
		*/
		FragmentGraphAPI size_t GetNumberOfInputs() const;
		/** Gets the n-th input for this fragment.
		\param[in] inputIndex the index of the input.
		\return the n-th input for this fragment.
		*/
		FragmentGraphAPI Input& GetInput(size_t inputIndex);
		/** Gets the n-th input for this fragment.
		\param[in] inputIndex the index of the input.
		\return the n-th input for this fragment.
		*/
		FragmentGraphAPI const Input& GetInput(size_t inputIndex) const;
		/** Gets the n-th input variable for this fragment. 
		\param[in] inputIndex the index of the input.
		\return if the given input is connected to an output, this function returns 
		the variable from that output. Otherwise, this function returns NULL.
		*/
		FragmentGraphAPI const Variable* GetInputVariable(size_t inputIndex) const;
		/** Gets the output of another fragment that is connected to n-th input for 
		this fragment. 
		\param[in] inputIndex the index of the input.
		\return if the given input is connected to an output, this function returns 
		that output. Otherwise, this function returns NULL.
		*/
		FragmentGraphAPI Output* GetInputSource(size_t inputIndex) const;
		/** Connects the input to an output. 
		\param[in] inputIndex the index of the input.
		\param[in] pOutput the pointer to an output of another fragment. Please 
		note the fragment system doesn't accept circular connections. (i.e. A 
		is connected to B, B is connected to C, while C is connected to A).
		\return true if connected, false otherwise.
		*/
		FragmentGraphAPI bool SetInputSource(
			size_t inputIndex, 
			Output* pOutput);
		
		/** Gets the number of outputs for this fragment.
		\return the number of outputs of this fragment.
		*/
		FragmentGraphAPI size_t GetNumberOfOutputs() const;
		/** Gets the n-th output for this fragment.
		\param[in] outputIndex the index of the output.
		\return the n-th output for this fragment.
		*/
		FragmentGraphAPI Output& GetOutput(size_t outputIndex);
		/** Gets the n-th output for this fragment.
		\param[in] outputIndex the index of the output.
		\return the n-th output for this fragment.
		*/
		FragmentGraphAPI const Output& GetOutput(size_t outputIndex) const;
		/** Updates the value of the n-th output. 
		\param[in] outputIndex the index of the output.
		\param[in] var the value to update.
		\return true if updated successfully, false otherwise.
		*/
		FragmentGraphAPI bool SetOutputVariable(
			size_t outputIndex, 
			const Variable& var);

		/** Gets a flag for this fragment
		\param[in] flag the flag bit to query
		\return true if this flag is on, false otherwise.
		*/
		FragmentGraphAPI bool GetFlag(FragmentFlags flag) const;

		/** Sets a flag for this fragment
		\param[in] flag the flag bit to set
		\param[in] onOff the value for the flag bit.
		*/
		FragmentGraphAPI void SetFlag(FragmentFlags flag, bool onOff);

		/** Assigns the parent fragment. This function will be invoked when a fragment
			node is added to a fragment graph.
		\param[in] pParent the parent of this fragment 
		*/
		FragmentGraphAPI void SetParent(Fragment* pParent);

		/** Gets the parent fragment. 
		\return the parent fragment.
		*/
		FragmentGraphAPI Fragment* GetParent() const;

		/** Gets the container of all built-in parameters
		\return the container of all built-in parameters.
		*/
		FragmentGraphAPI IVariableDictionary* GetParameterTable();

		/** Gets the container of all built-in parameters
		\return the container of all built-in parameters.
		*/
		FragmentGraphAPI const IVariableDictionary* GetParameterTable() const;
		/** The fragment message handling callback function that is called
			if any of its registered messages (including the timer message) is invoked.
		\param[in] message the id of the message that has been invoked.
		\param[in] param the parameters of this message
		*/
		FragmentGraphAPI virtual bool OnMessage(const FragmentMessageParameter& param);
		/** Registers or deregisters the timer message and set the timer ticks value 
			in milliseconds. To deregister this timer,  assign TIMER_NONE 
			to timerTicks.
		\param[in] timerTicks the timer ticks value
		*/
		FragmentGraphAPI void SetTimerTicks(size_t timerTicks);
		/** Gets the timer ticks value in milliseconds.
		\return the timer ticks value if a timer is registered or TIMER_NONE otherwise.
		*/
		FragmentGraphAPI size_t GetTimerTicks() const;

		/** Register a message with this fragment.
		\param[in] messageID the id of the message to register
		*/
		FragmentGraphAPI void RegisterMessage(size_t messageID);
		/** Deregister a message with this fragment.
		\param[in] messageID the id of the message to deregister
		*/
		FragmentGraphAPI bool UnRegisterMessage(size_t messageID);
		/** Gets all the messages that have been registered.
		\return all messages that have been registered.
		*/
		FragmentGraphAPI const Tab<size_t>& GetRegisteredMessageIDs() const;

		/** Gets the class ID of this fragment.
		\return the class ID of this fragment.
		*/
		FragmentGraphAPI virtual Class_ID GetClassID() const;

	private:
		// private functions
		void ReleaseInputs();
		void ReleaseOutputs();
		void SetFlagCore(FragmentFlags flag, bool onOff);

	private:
		unsigned long mFragmentFlags;
		size_t mNumInputs;
		Input** mppInput;
		size_t mNumOutputs;
		Output** mppOutput;
		Fragment* mpParentFragment;
		IVariableDictionary* mpDictionaryVar;
		size_t mTimerTicks;
		Tab<size_t> mMessageIDs;
	private:
		Fragment(const Fragment&);
		Fragment& operator = (const Fragment&);
	};

	typedef RefPtr<Fragment> FragmentPtr;

} } // namespace MaxSDK::Graphics
