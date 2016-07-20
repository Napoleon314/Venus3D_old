////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Memory
//  File name:   VeAllocator.h
//  Created:     2016/07/20 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#define VE_STACK_ALLOCATOR_DEFAULT_SIZE (128 * 1024)

class VENUS_API VeStackAllocator : public VeMemObject
{
	VeNoCopy(VeStackAllocator);
public:
	VeStackAllocator(size_t stSize = VE_STACK_ALLOCATOR_DEFAULT_SIZE) noexcept;

	~VeStackAllocator() noexcept;

	inline size_t Space() noexcept;

	inline size_t RemainSpace() noexcept;

	void* Allocate(size_t stSizeInBytes) noexcept;

	void Deallocate() noexcept;

protected:
	const size_t m_stSize;
	uint8_t* m_pu8Buffer = nullptr;
	uint8_t* m_pu8Current = nullptr;
	vtd::stack<size_t> m_kStack;

};

VeSmartPointer(VePoolAllocator);

class VENUS_API VePoolAllocator : public VeRefObject
{
	VeNoCopy(VePoolAllocator);
public:
	VePoolAllocator(size_t stUnitSize,
		size_t stUnitPerChunk = 128, size_t stAlign = 16) noexcept;

	~VePoolAllocator() noexcept;

	inline size_t GetUnitSize() const noexcept;

	inline size_t GetUnitPerChunk() const noexcept;

	inline size_t GetAlign() const noexcept;

	inline void* Allocate() noexcept;

	inline void Deallocate(void* pvMemory) noexcept;

	static const VePoolAllocatorPtr& GetAllocator(size_t stUnitSize) noexcept;

protected:
	void _AddChunk() noexcept;

	void* _Allocate() noexcept;

	void _Deallocate(void* pvMemory) noexcept;

	struct Chunk
	{
		void* m_pvFreeStart;
		void* m_pvEnd;
		Chunk* m_pvPre;
	};

	const size_t m_stUnitSize;
	const size_t m_stUnitPerChunk;
	const size_t m_stAlign;
	vtd::spin_lock m_kLock;
	Chunk* m_pkChunkHead = nullptr;
	void* m_pvFreeHead = nullptr;
#	ifdef VE_MEM_DEBUG
	uint32_t m_u32UnitCount = 0;
	uint32_t m_u32ChunkCount = 0;
#	endif

};

namespace venus
{
	template<class _Ty>
	class allocator
	{
	public:
		typedef _Ty value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		template<class _Other>
		struct rebind
		{
			typedef allocator<_Other> other;
		};

		allocator() noexcept
		{

		}

		allocator(const allocator<_Ty>&) noexcept
		{

		}

		template<class _Other>
		allocator(const allocator<_Other>&) noexcept
		{

		}

		template<class _Other>
		allocator<_Ty>& operator=(const allocator<_Other>&)
		{
			return *this;
		}

		pointer address(reference _Val) const noexcept
		{
			return &_Val;
		}

		const_pointer address(const_reference _Val) const noexcept
		{
			return &_Val;
		}

		void deallocate(pointer _Ptr, size_type) noexcept
		{
			VeAlignedFree(_Ptr);
		}

		pointer allocate(size_type _Count) noexcept
		{
			return (pointer)VeAlignedMalloc(_Count * sizeof(_Ty), VE_MAX(alignof(_Ty), sizeof(size_t) * 2));
		}

		pointer allocate(size_type _Count, const void *) noexcept
		{
			return allocate(_Count);
		}

		template<class _Objty, class... _Types>
		void construct(_Objty *_Ptr, _Types&&... _Args)
		{
			::new ((void *)_Ptr) _Objty(std::forward<_Types>(_Args)...);
		}

		template<class _Uty>
		void destroy(_Uty *_Ptr)
		{
			_Ptr->~_Uty();
			_Ptr = nullptr;
		}

		size_t max_size() const noexcept
		{
			return (size_t(-1) / sizeof(_Ty));
		}
	};

	template<>
	class allocator<void>
	{
	public:
		typedef void value_type;

		typedef void *pointer;
		typedef const void *const_pointer;

		template<class _Other>
		struct rebind
		{
			typedef allocator<_Other> other;
		};

		allocator() noexcept
		{

		}

		allocator(const allocator<void>&) noexcept
		{

		}

		template<class _Other>
		allocator(const allocator<_Other>&) noexcept
		{

		}

		template<class _Other>
		allocator<void>& operator=(const allocator<_Other>&)
		{
			return *this;
		}
	};

	template<class _Ty, class _Other>
	inline bool operator==(const allocator<_Ty>&,
		const allocator<_Other>&) noexcept
	{
		return true;
	}

	template<class _Ty, class _Other>
	inline bool operator!=(const allocator<_Ty>&,
		const allocator<_Other>&) noexcept
	{
		return false;
	}

	template<class _Ty>
	class pool_allocator
	{
	public:
		typedef _Ty value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		template<class _Other>
		struct rebind
		{
			typedef pool_allocator<_Other> other;
		};

		pool_allocator() noexcept
		{
			m_spPool = VePoolAllocator::GetAllocator(sizeof(_Ty));
			assert(m_spPool && sizeof(_Ty) <= m_spPool->GetUnitSize());
		}

		pool_allocator(const pool_allocator<_Ty>& kCopy) noexcept
		{
			m_spPool = kCopy.m_spPool;
			assert(m_spPool && sizeof(_Ty) <= m_spPool->GetUnitSize());
		}

		template<class _Other>
		pool_allocator(const pool_allocator<_Other>&) noexcept
		{
			m_spPool = VePoolAllocator::GetAllocator(sizeof(_Ty));
			assert(m_spPool && sizeof(_Ty) <= m_spPool->GetUnitSize());
		}

		template<class _Other>
		pool_allocator<_Ty>& operator=(const pool_allocator<_Other>&)
		{
			m_spPool = VePoolAllocator::GetAllocator(sizeof(_Ty));
			assert(m_spPool && sizeof(_Ty) <= m_spPool->GetUnitSize());
			return *this;
		}

		pointer address(reference _Val) const noexcept
		{
			return &_Val;
		}

		const_pointer address(const_reference _Val) const noexcept
		{
			return &_Val;
		}

		void deallocate(pointer _Ptr, size_type _Count) noexcept
		{
			assert(_Count == 1);
			m_spPool->Deallocate(_Ptr);
		}

		pointer allocate(size_type _Count) noexcept
		{
			assert(_Count == 1);
			return (pointer)m_spPool->Allocate();
		}

		pointer allocate(size_type _Count, const void *) noexcept
		{
			return allocate(_Count);
		}

		template<class _Objty, class... _Types>
		void construct(_Objty *_Ptr, _Types&&... _Args)
		{
			::new ((void *)_Ptr) _Objty(std::forward<_Types>(_Args)...);
		}

		template<class _Uty>
		void destroy(_Uty *_Ptr)
		{
			_Ptr->~_Uty();
		}

		size_t max_size() const noexcept
		{
			return (size_t(-1) / sizeof(_Ty));
		}

	private:
		VePoolAllocatorPtr m_spPool;

	};

	template<>
	class pool_allocator<void>
	{
	public:
		typedef void value_type;

		typedef void *pointer;
		typedef const void *const_pointer;

		template<class _Other>
		struct rebind
		{
			typedef pool_allocator<_Other> other;
		};

		pool_allocator() noexcept
		{

		}

		pool_allocator(const pool_allocator<void>&) noexcept
		{

		}

		template<class _Other>
		pool_allocator(const pool_allocator<_Other>&) noexcept
		{

		}

		template<class _Other>
		pool_allocator<void>& operator=(const pool_allocator<_Other>&)
		{
			return *this;
		}
	private:
		VePoolAllocatorPtr m_spPool;

	};

	template<class _Ty, class _Other>
	inline bool operator==(const pool_allocator<_Ty>& _Left,
		const pool_allocator<_Other>& _Right) noexcept
	{
		return _Left.m_spPool == _Right.m_spPool;
	}

	template<class _Ty, class _Other>
	inline bool operator!=(const pool_allocator<_Ty>& _Left,
		const pool_allocator<_Other>& _Right) noexcept
	{
		return _Left.m_spPool != _Right.m_spPool;
	}

	template<class _Ty>
	class vtd_allocator
	{
	public:
		typedef _Ty value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		struct _Shell
		{
			value_type data;
		};

		template<class _Other>
		struct rebind
		{
			typedef allocator<_Other> other;
		};

		static void memory_copy(pointer _Dst, const_pointer _Src, size_type _Count) noexcept
		{
			memcpy(_Dst, _Src, _Count * sizeof(value_type));
		}

		static pointer address(reference _Val) noexcept
		{
			return &_Val;
		}

		static const_pointer address(const_reference _Val) noexcept
		{
			return &_Val;
		}

		static void deallocate(pointer _Ptr) noexcept
		{
			VeFree(_Ptr);
		}

		static pointer allocate(size_type _Count) noexcept
		{
			return (pointer)VeMalloc(_Count * sizeof(value_type));
		}

		static pointer allocate(size_type _Count, pointer _Ptr) noexcept
		{
			return (pointer)VeRealloc(_Ptr, _Count * sizeof(value_type));
		}

		template<class... _Types>
		static void construct(value_type *_Ptr, _Types&&... _Args) noexcept
		{
			::new ((void *)_Ptr) value_type(std::forward<_Types>(_Args)...);
		}

		static void destroy(pointer _Ptr) noexcept
		{
			((_Shell*)_Ptr)->~_Shell();
			_Ptr = nullptr;
		}

		static size_t max_size() noexcept
		{
			return ((size_t)(-1) / sizeof(value_type));
		}

	private:
		vtd_allocator() noexcept = delete;
		~vtd_allocator() noexcept = delete;

	};

}

template<class _Ty,
	class _Alloc = venus::vtd_allocator<_Ty>>
	class VeVector : public vtd::vector<_Ty, _Alloc> {};

template<class _Ty,
	class _Alloc = venus::pool_allocator<_Ty>>
	class VeList : public std::list<_Ty, _Alloc> {};

template<class _Kty,
	class _Pr = std::less<_Kty>,
	class _Alloc = venus::pool_allocator<_Kty>>
	class VeSet : public std::set<_Kty, _Pr, _Alloc> {};

template<class _Kty,
	class _Pr = std::less<_Kty>,
	class _Alloc = venus::pool_allocator < _Kty >>
	class VeMultiSet : public std::multiset<_Kty, _Pr, _Alloc> {};

template<class _Kty,
	class _Ty,
	class _Pr = std::less<_Kty>,
	class _Alloc = venus::pool_allocator<std::pair<const _Kty, _Ty>>>
	class VeMap : public std::map<_Kty, _Ty, _Pr, _Alloc> {};

template<class _Kty,
	class _Ty,
	class _Pr = std::less<_Kty>,
	class _Alloc = venus::pool_allocator<std::pair<const _Kty, _Ty>>>
	class VeMultiMap : public std::multimap<_Kty, _Ty, _Pr, _Alloc> {};

template<class _Kty,
	class _Hasher = std::hash<_Kty>,
	class _Keyeq = std::equal_to<_Kty>,
	class _Alloc = venus::allocator<_Kty>>
	class VeUnorderedSet : public std::unordered_set<_Kty, _Hasher, _Keyeq, _Alloc> {};

template<class _Kty,
	class _Ty,
	class _Hasher = std::hash<_Kty>,
	class _Keyeq = std::equal_to<_Kty>,
	class _Alloc = venus::allocator<std::pair<const _Kty, _Ty>>>
	class VeUnorderedMap : public std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc> {};

#include "VeAllocator.inl"
