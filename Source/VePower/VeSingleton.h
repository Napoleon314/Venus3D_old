////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeSingleton.h
//  Created:     2015/02/23 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

template <class T>
class VeSingleton : public VeMemObject
{
public:
	VeSingleton() noexcept
	{
		VE_ASSERT(!ms_pSingleton);
		ms_pSingleton = static_cast<T*>(this);
	}

	virtual ~VeSingleton() noexcept
	{
		VE_ASSERT(ms_pSingleton);
		ms_pSingleton = nullptr;
	}

	static bool IsAvailable() noexcept
	{
		return ms_pSingleton ? true : false;
	}

	template<class... _Types>
	static void Create(_Types... pak) noexcept
	{
		VE_NEW T(pak...);
	}

	static void Destory() noexcept
	{
		VE_SAFE_DELETE(ms_pSingleton);
	}

	static T& GetSingleton() noexcept
	{
		VE_ASSERT(ms_pSingleton);
		return (*ms_pSingleton);
	}

	static T* GetSingletonPtr() noexcept
	{
		return ms_pSingleton;
	}

protected:
	static T* ms_pSingleton;

};

template<class T> T* VeSingleton<T>::ms_pSingleton = nullptr;
