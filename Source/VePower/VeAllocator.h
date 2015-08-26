////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeAllocator.h
//  Created:     2015/03/03 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

VeSmartPointer(VeStackAllocator);

class VE_POWER_API VeStackAllocator : public VeRefObject
{
	VeNoCopy(VeStackAllocator);
public:
	VeStackAllocator(VeSizeT stSize = 102400) noexcept;

	~VeStackAllocator() noexcept;

	inline VeSizeT Space() noexcept;

	inline VeSizeT RemainSpace() noexcept;

	void* Allocate(VeSizeT stSizeInBytes) noexcept;

	void Deallocate() noexcept;

protected:
	const VeSizeT m_stSize;
	VeSpinLock m_kLock;
	VeByte* m_pbyBuffer = nullptr;
	VeByte* m_pbyCurrent = nullptr;
	VeStack<VeSizeT> m_kStack;

};

VeSmartPointer(VePoolAllocator);

class VE_POWER_API VePoolAllocator : public VeRefObject
{
	VeNoCopy(VePoolAllocator);
public:
	VePoolAllocator(VeSizeT stUnitSize,
		VeSizeT stUnitPerChunk = 128, VeSizeT stAlign = 16) noexcept;

	~VePoolAllocator() noexcept;

	inline VeSizeT GetUnitSize() const noexcept;

	inline VeSizeT GetUnitPerChunk() const noexcept;

	inline VeSizeT GetAlign() const noexcept;

	inline void* Allocate() noexcept;

	inline void Deallocate(void* pvMemory) noexcept;

	static const VePoolAllocatorPtr& GetAllocator(VeSizeT stUnitSize) noexcept;

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

	const VeSizeT m_stUnitSize;
	const VeSizeT m_stUnitPerChunk;
	const VeSizeT m_stAlign;
	VeSpinLock m_kLock;
	Chunk* m_pkChunkHead = nullptr;
	void* m_pvFreeHead = nullptr;
#	ifdef VE_MEM_DEBUG
	VeUInt32 m_u32UnitCount = 0;
	VeUInt32 m_u32ChunkCount = 0;
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
		typedef VeSizeT size_type;
		typedef VePtrDiff difference_type;

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

		void deallocate(pointer _Ptr, size_type _Count) noexcept
		{
			VeAlignedFree(_Ptr);
		}

		pointer allocate(size_type _Count) noexcept
		{
			return (pointer)VeAlignedMalloc(_Count * sizeof(_Ty), VE_MAX(alignof(_Ty), sizeof(VeSizeT) * 2));
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
			return (VE_SIZET_MAX / sizeof(_Ty));
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
	inline bool operator!=(const allocator<_Ty>& _Left,
		const allocator<_Other>& _Right) noexcept
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
		typedef VeSizeT size_type;
		typedef VePtrDiff difference_type;

		template<class _Other>
		struct rebind
		{
			typedef pool_allocator<_Other> other;
		};

		pool_allocator() noexcept
		{
			m_spPool = VePoolAllocator::GetAllocator(sizeof(_Ty));
			VE_ASSERT(m_spPool&& sizeof(_Ty) <= m_spPool->GetUnitSize());
		}

		pool_allocator(const pool_allocator<_Ty>& kCopy) noexcept
		{
			m_spPool = kCopy.m_spPool;
			VE_ASSERT(m_spPool&& sizeof(_Ty) <= m_spPool->GetUnitSize());
		}

		template<class _Other>
		pool_allocator(const pool_allocator<_Other>& kCopy) noexcept
		{
			m_spPool = VePoolAllocator::GetAllocator(sizeof(_Ty));
			VE_ASSERT(m_spPool&& sizeof(_Ty) <= m_spPool->GetUnitSize());
		}

		template<class _Other>
		pool_allocator<_Ty>& operator=(const pool_allocator<_Other>&)
		{
			m_spPool = VePoolAllocator::GetAllocator(sizeof(_Ty));
			VE_ASSERT(m_spPool&& sizeof(_Ty) <= m_spPool->GetUnitSize());
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
			VE_ASSERT(_Count == 1);
			m_spPool->Deallocate(_Ptr);
		}

		pointer allocate(size_type _Count) noexcept
		{
			VE_ASSERT(_Count == 1);
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
			return (VE_SIZET_MAX / sizeof(_Ty));
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

}

template<class _Ty,
class _Alloc = venus::allocator<_Ty>>
class VeVector : public std::vector<_Ty, _Alloc> {};

template<class _Ty,
class _Alloc = venus::pool_allocator<_Ty>>
class VeList : public std::list<_Ty, _Alloc> {};

template<class _Kty,
class _Pr = std::less<_Kty>,
class _Alloc = venus::pool_allocator<_Kty>>
class VeSet : public std::set<_Kty, _Pr, _Alloc> {};

template<class _Kty,
class _Ty,
class _Pr = std::less<_Kty>,
class _Alloc = venus::pool_allocator<std::pair<const _Kty, _Ty>>>
class VeMap : public std::map<_Kty, _Ty, _Pr, _Alloc> {};

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
