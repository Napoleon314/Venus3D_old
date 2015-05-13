////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeRefList.h
//  Created:     2015/03/05 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

template<class> class VeRefList;

template <class _Ty>
class VeRefNode : public VeMemObject
{
public:
	typedef VeRefNode<_Ty> _Node;
	typedef VeRefList<_Ty> _List;
	typedef _Ty value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	friend _List;

	VeRefNode() noexcept
	{

	}

	VeRefNode(_Node&& move) noexcept
	{
		_Parent = move._Parent;
		_Pre = move._Pre;
		_Next = move._Next;
		move._Parent = nullptr;
		move._Pre = nullptr;
		move._Next = nullptr;
	}

	~VeRefNode() noexcept
	{
		detach();
	}

	_Node& operator=(_Node&& move) noexcept
	{
		detach();
		_Parent = move._Parent;
		_Pre = move._Pre;
		_Next = move._Next;
		move._Parent = nullptr;
		move._Pre = nullptr;
		move._Next = nullptr;
		return *this;
	}

	bool is_attach() const noexcept
	{
		return _Parent != nullptr;
	}

	bool is_attach(const _List& list) const noexcept
	{
		return _Parent == &list;
	}

	_Node* pre() noexcept
	{
		return _Pre;
	}

	const _Node* pre() const noexcept
	{
		return _Pre;
	}

	_Node* next() noexcept
	{
		return _Next;
	}

	const _Node* next() const noexcept
	{
		return _Next;
	}

	_List* get_list() noexcept
	{
		return _Parent;
	}

	const _List* get_list() const noexcept
	{
		return _Parent;
	}

	void detach() noexcept
	{
		if (_Parent)
		{
			if (_Parent->_InnerIter == this)
			{
				_Parent->_InnerIter = next();
			}

			if (_Pre)
			{
				_Pre->_Next = _Next;
			}
			else
			{
				_Parent->_Head = _Next;
			}
			if (_Next)
			{
				_Next->_Pre = _Pre;
			}
			else
			{
				_Parent->_Tail = _Pre;
			}

			--(_Parent->_Size);
			_Parent = nullptr;
			_Pre = nullptr;
			_Next = nullptr;
		}
	}

	reference content() noexcept
	{
		return m_Content;
	}

	const_reference content() const noexcept
	{
		return m_Content;
	}

	pointer content_ptr() noexcept
	{
		return &m_Content;
	}

	const_pointer content_ptr() const noexcept
	{
		return &m_Content;
	}

	reference pre_content(reference value) noexcept
	{
		return _Pre ? _Pre->m_tContent : value;
	}

	const_reference pre_content(const_reference value) const noexcept
	{
		return _Pre ? _Pre->m_tContent : value;
	}

	reference next_content(reference value) noexcept
	{
		return _Next ? _Next->m_tContent : value;
	}

	const_reference next_content(const_reference value) const noexcept
	{
		return _Next ? _Next->m_tContent : value;
	}

	value_type m_Content;

private:
	VeRefNode(const _Node&) noexcept
	{
		VE_ASSERT(!"Copy constructor is no available");
	}

	_Node& operator=(const _Node&) noexcept
	{
		VE_ASSERT(!"Copy equal is no available");
		return *this;
	}

	_Node* _Pre = nullptr;
	_Node* _Next = nullptr;
	_List* _Parent = nullptr;

};

template <class _Ty>
class VeRefList : public VeMemObject
{
public:
	typedef VeRefNode<_Ty> _Node;
	typedef VeRefList<_Ty> _List;
	typedef typename _Node::value_type value_type;
	typedef typename _Node::pointer pointer;
	typedef typename _Node::const_pointer const_pointer;
	typedef typename _Node::reference reference;
	typedef typename _Node::const_reference const_reference;
	typedef VeSizeT size_type;
	typedef VePtrDiff difference_type;
	friend _Node;

	class const_iterator : public VeMemObject
	{
	public:
		const_iterator(_List* list) noexcept
			: _Parent(list)
		{

		}

		bool operator == (const const_iterator& iter) const noexcept
		{
			return (_Parent == nullptr || _Parent->_InnerIter == nullptr) ==
				(iter._Parent == nullptr || iter._Parent->_InnerIter == nullptr);
		}

		bool operator != (const const_iterator& iter) const noexcept
		{
			return !((*this) == iter);
		}

		const_reference operator*() const noexcept
		{
			VE_ASSERT(_Parent && _Parent->_InnerIter);
			const_reference res = _Parent->_InnerIter->content();
			_Parent->_InnerIter = _Parent->_InnerIter->_Next;
			return res;
		}

		const_iterator& operator++() noexcept
		{
			return *this;
		}

	protected:
		_List* _Parent;

	};

	class iterator : public const_iterator
	{
	public:
		iterator(_List* list) noexcept : const_iterator(list)
		{

		}

		reference operator*() noexcept
		{
			VE_ASSERT(const_iterator::_Parent && const_iterator::_Parent->_InnerIter);
			reference res = const_iterator::_Parent->_InnerIter->content();
			const_iterator::_Parent->_InnerIter = const_iterator::_Parent->_InnerIter->_Next;
			return res;
		}

		iterator& operator++() noexcept
		{
			return *this;
		}

	};

	class ReverseIter : public VeMemObject
	{
	public:
		class const_iterator : public VeMemObject
		{
		public:
			const_iterator(_List* list) noexcept
				: _Parent(list)
			{

			}

			bool operator == (const const_iterator& iter) const noexcept
			{
				return (_Parent == nullptr || _Parent->_InnerIter == nullptr) ==
					(iter._Parent == nullptr || iter._Parent->_InnerIter == nullptr);
			}

			bool operator != (const const_iterator& iter) const noexcept
			{
				return !((*this) == iter);
			}

			const_reference operator*() const noexcept
			{
				VE_ASSERT(_Parent && _Parent->_InnerIter);
				const_reference res = _Parent->_InnerIter->content();
				_Parent->_InnerIter = _Parent->_InnerIter->_Pre;
				return res;
			}

			const_iterator& operator++() noexcept
			{
				return *this;
			}

		protected:
			_List* _Parent;

		};

		class iterator : public const_iterator
		{
		public:
			iterator(_List* list) noexcept : const_iterator(list)
			{

			}

			reference operator*() noexcept
			{
				VE_ASSERT(const_iterator::_Parent && const_iterator::_Parent->_InnerIter);
				reference res = const_iterator::_Parent->_InnerIter->content();
				const_iterator::_Parent->_InnerIter = const_iterator::_Parent->_InnerIter->_Pre;
				return res;
			}

			iterator& operator++() noexcept
			{
				return *this;
			}

		};

		iterator begin() noexcept
		{
			_Parent->_InnerIter = _Parent->_Tail;
			return iterator(_Parent);
		}

		const_iterator begin() const noexcept
		{
			_Parent->_InnerIter = _Parent->_Tail;
			return const_iterator(_Parent);
		}

		iterator end() noexcept
		{
			return iterator(nullptr);
		}

		const_iterator end() const noexcept
		{
			return const_iterator(nullptr);
		}

		ReverseIter(_List* list) noexcept
			: _Parent(list)
		{

		}

	private:
		_List* _Parent;
	};

	VeRefList() noexcept
	{
		
	}

	VeRefList(_List&& move) noexcept
	{
		_InnerIter = nullptr;
		_Head = move._Head;
		_Tail = move._Tail;
		_Size = move._Size;
		move._InnerIter = nullptr;
		move._Head = nullptr;
		move._Tail = nullptr;
		move._Size = 0;
	}

	~VeRefList() noexcept
	{
		clear();
	}

	_List& operator=(_List&& move) noexcept
	{
		_InnerIter = nullptr;
		_Head = move._Head;
		_Tail = move._Tail;
		_Size = move._Size;
		move._InnerIter = nullptr;
		move._Head = nullptr;
		move._Tail = nullptr;
		move._Size = 0;
		return *this;
	}

	void attach_back(_Node& node) noexcept
	{
		node.detach();
		node._Parent = this;
		if (_Tail)
		{
			_Tail->_Next = &node;
			node._Pre = _Tail;
		}
		else
		{
			_Head = &node;
		}
		_Tail = &node;
		++_Size;
	}

	void attach_front(_Node& node) noexcept
	{
		node.detach();
		node._Parent = this;
		if (_Head)
		{
			_Head->_Pre = &node;
			node._Next = _Head;
		}
		else
		{
			_Tail = &node;
		}
		_Head = &node;
		++_Size;
	}

	void attach_after(_Node& front, _Node& node) noexcept
	{
		VE_ASSERT(front.is_attach(*this));
		node.detach();
		node._Parent = this;
		node._Pre = &front;
		node._Next = front._Next;
		front._Next = &node;
		if (node._Next)
		{
			node._Next->_Pre = &node;
		}
		else
		{
			_Tail = &node;
		}
		++_Size;
	}

	void attach_before(_Node& after, _Node& node) noexcept
	{
		VE_ASSERT(after.is_attach(*this));
		node.detach();
		node._Parent = this;
		node._Pre = after._Pre;
		node._Next = &after;
		after._Pre = &node;
		if (node._Pre)
		{
			node._Pre->_Next = &node;
		}
		else
		{
			_Head = &node;
		}
		++_Size;
	}

	_Node* get_head_node() noexcept
	{
		return _Head;
	}

	const _Node* get_head_node() const noexcept
	{
		return _Head;
	}

	_Node* get_tail_node() noexcept
	{
		return _Tail;
	}

	const _Node* get_tail_node() const noexcept
	{
		return _Tail;
	}

	void clear() noexcept
	{
		_Node* node = _Head;
		while (node)
		{
			_Node* next = node->_Next;
			node->_Parent = nullptr;
			node->_Pre = nullptr;
			node->_Next = nullptr;
			node = next;
		}
		_Head = nullptr;
		_Tail = nullptr;
		_Size = 0;
	}

	size_type size() const noexcept
	{
		return _Size;
	}

	bool empty() const noexcept
	{
		return _Size == 0;
	}

	iterator begin() noexcept
	{
		_InnerIter = _Head;
		return iterator(this);
	}

	const_iterator begin() const noexcept
	{
		_InnerIter = _Head;
		return const_iterator(const_cast<_List*>(this));
	}

	iterator end() noexcept
	{
		return iterator(nullptr);
	}

	const_iterator end() const noexcept
	{
		return const_iterator(nullptr);
	}

	ReverseIter r() noexcept
	{
		return ReverseIter(this);
	}

	const ReverseIter r() const noexcept
	{
		return ReverseIter(const_cast<_List*>(this));
	}

private:
	VeRefList(const _List&) noexcept
	{
		VE_ASSERT(!"Copy constructor is no available");
	}

	VeRefList& operator=(const _List&) noexcept
	{
		VE_ASSERT(!"Copy equal is no available");
		return *this;
	}

	mutable _Node* _InnerIter = nullptr;
	_Node* _Head = nullptr;
	_Node* _Tail = nullptr;
	size_type _Size = 0;

};
