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

#include "Fragment.h"
#include "../../strclass.h"

	/** Class ID of FragmentGraph 
	*/
#define FRAGMENT_GRAPH_CLASS_ID Class_ID(0x32ca2b60, 0x2f2c4f4c)

namespace MaxSDK { namespace Graphics {	

	class FragmentGraphImpl;
	class InputSlot;
	class OutputSlot;

	/** FragmentGraph is a graph consisting of fragment nodes. A FragmentGraph 
	that has inputs and outputs can also be treated as a fragment node. Various
	kinds of fragment nodes can be combined together to yield arbitrary multi-passes
	render pipeline. The class provides several utility functions to create and connect
	fragment nodes.
	*/
	class FragmentGraph : public Fragment
	{
	public:
		/** Constructor.
		*/
		FragmentGraphAPI FragmentGraph();
		/** Destructor.
		*/
		FragmentGraphAPI virtual ~FragmentGraph();

	protected:
		/** Does evaluation. A concrete fragment class must override this function, 
		perform necessary computation in this function, and set value to its 
		output. 
		Note that this function cannot be called directly, it is called 
		indirectly by calling Fragment::Evaluate(), but is only called if the
		Fragment is dirty. 
		\param[in] evaluationContext context info for evaluation.
		\return true on success, false otherwise. If this function returns false, 
		the fragment will still be dirty. 
		*/
		FragmentGraphAPI virtual bool DoEvaluate(void* evaluationContext);

		/** This function is invoked when any fragment flag gets changed.
		\param[in] flag the flag that gets changed
		\param[in] oldValue the old value of the flag
		*/
		FragmentGraphAPI virtual void HandleFlagChanged(FragmentFlags flag, 
			bool oldValue);

	public:
		/** Gets the number of nodes
		\return the count of nodes.
		*/
		FragmentGraphAPI size_t GetNodeCount() const;
		/** Adds a fragment node to the current graph
		\param[in] pFragmentNode the node to be added. This node could be either
					a fragment node or a fragment graph.
		\param[in] nodeName the name of the node to be added. We use this name
					to identify this node in the graph.
		*/
		FragmentGraphAPI void AddNode(const FragmentPtr& pFragmentNode, const MSTR& nodeName);
		/** Gets a fragment node given its index
		\param[in] index the index of the node. 
		\return the node corresponding to index.
		*/
		FragmentGraphAPI FragmentPtr GetNode(size_t index) const;
		/** Gets a fragment node by name
		\param[in] nodeName the name of the node. 
		\return the node corresponding to nodeName.
		*/
		FragmentGraphAPI FragmentPtr GetNode(const MSTR& nodeName) const;
		/** Gets the name of the node given its index
		\param[in] index the index of the node. 
		\return the name of the node corresponding to index.
		*/
		FragmentGraphAPI const MSTR& GetNodeName(size_t index) const;
		/** Removes a fragment node given its name
		\param[in] nodeName the name of the node to be removed. 
		\return true if this node is found and deleted, false otherwise.
		*/
		FragmentGraphAPI void RemoveNode(const MSTR& nodeName);
		/** Removes a fragment node from this graph
		\param[in] pFragmentNode the node to remove
		*/
		FragmentGraphAPI void RemoveNode(const FragmentPtr& pFragmentNode);

		/** Wires a connection between a specified input of the graph and a specified
			input of a fragment node.
		\param[in] graphInputIndex the index of a specified input
		\param[in] nodeName the name of the fragment node
		\param[in] nodeInputIndex the index of a specified input of the fragment node
		\return true if this connection could be established, false otherwise. 
			Successful connection depends on:
			-# the node can be found
			-# both inputs can be found (the index given is in a valid range)
			-# the types of both inputs match  
		*/
		FragmentGraphAPI bool WireInput(size_t graphInputIndex, const MSTR& nodeName, size_t nodeInputIndex);
		/** Wires a connection between a specified output of the graph and a specified
			output of a fragment node.
		\param[in] graphOutputIndex the index of a specified output
		\param[in] nodeName the name of the fragment node
		\param[in] nodeOutputIndex the index of a specified output of the fragment node
		\return true if this connection could be established, false otherwise. 
			Successful connection depends on:
			-# the node can be found
			-# both outputs can be found (the index given is in a valid range)
			-# the types of both outputs match  
		*/
		FragmentGraphAPI bool WireOutput(size_t graphOutputIndex, const MSTR& nodeName, size_t nodeOutputIndex);
		/** Wires a connection between a specified output of a source node and a specified
			input of a target node.
		\param[in] sourceNodeName the name of the source node
		\param[in] sourceNodeOutputIndex the index of a specified output of the source node
		\param[in] targetNodeName the name of the target node
		\param[in] targetNodeInputIndex the index of a specified output of the source node
		\return true if this connection could be established, false otherwise. 
			Successful connection depends on:
			-# both nodes can be found
			-# the input and the output can be found (the index given is in a valid range)
			-# the type of the source output matches the type of the target input
		*/
		FragmentGraphAPI bool WireNode(const MSTR& sourceNodeName, size_t sourceNodeOutputIndex,
			const MSTR& targetNodeName, size_t targetNodeInputIndex);

		/** Loads input, output and connection information for a fragment graph from an XML file.
		After loading the XML file, the FragmentGraph completes wiring input, wiring output 
		and wiring nodes.
		\param[in] filePathName file path name.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool LoadXML(const MSTR& filePathName);

		/** Saves the inputs, outputs and connection information of one fragment graph into one XML file.
		\param[in] filePathName file path name.
		\return true on success, false otherwise.
		*/
		FragmentGraphAPI bool SaveXML(const MSTR& filePathName);

		/** Gets the class ID of the fragment graph.
		\return the class ID of the fragment graph.
		*/
		FragmentGraphAPI virtual Class_ID GetClassID() const;

		using Fragment::InitializeInputs;
		using Fragment::InitializeOutputs;

		FragmentGraphAPI virtual Input* CreateInput(const Class_ID& typeID);
		FragmentGraphAPI virtual Output* CreateOutput(const Class_ID& typeID);

		/** Check if this fragment graph is valid or not
		\return 0 if valid or non-zero otherwise
		*/
		FragmentGraphAPI int CheckValidity();

	private:
		FragmentGraph(const Fragment&);
		FragmentGraph& operator = (const Fragment&);

		InputSlot& GetInputSlot(size_t slotIndex);
		OutputSlot& GetOutputSlot(size_t slotIndex);
		void Invalidate();

		bool TryConnectCompatibleTypes(
			const MSTR& sourceOutputName, 
			Output& sourceOutput, 
			const MSTR& targetInputName, 
			Input& targeInput);

		bool WireFragment(Fragment* pFragment,
			Output& sourceOutput,
			const MSTR& sourceName,
			size_t fragmentInputIndex,
			const MSTR& targetName);

		// The FragmentGraph class will manage the life time of mpImpl which is
		// just used to hide the implementation detail of FragmentGraph.
		FragmentGraphImpl* mpImpl;
	};

typedef RefPtr<FragmentGraph> FragmentGraphPtr;

} } // namespace MaxSDK::Graphics
