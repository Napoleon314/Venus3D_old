////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      ASync
//  File name:   VeCoroutine.h
//  Created:     2016/07/11 by Albert
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

#define VE_CO_DEFAULT_STACK (32 * 1024)

enum VeCoroutineStatus
{
	VE_CO_READY,
	VE_CO_SUSPENDED,
	VE_CO_RUNNING,
	VE_CO_DEAD
};

VeSmartPointer(VeCoroutineBase);
VeSmartPointer(VeCoroutineEnv);

class VENUS_API VeCoroutineBase : public VeRefObject
{
	VeNoCopy(VeCoroutineBase);
	VeNoMove(VeCoroutineBase);
public:
#ifdef BUILD_PLATFORM_WIN
	typedef LPFIBER_START_ROUTINE Entry;
#else
    typedef void (*Entry)(uint32_t, uint32_t);
#endif

protected:
	friend class VeCoroutineEnv;

	VeCoroutineBase() noexcept : m_u32StackSize(0) {}

	VeCoroutineBase(Entry pfuncEntry, uint32_t u32Stack = VE_CO_DEFAULT_STACK) noexcept;

	virtual ~VeCoroutineBase() noexcept;

	void Push() noexcept;

	void Resume() noexcept;

	void Restart(Entry pfuncEntry) noexcept;

	const uint32_t m_u32StackSize;
	VeCoroutineStatus m_eStatus = VE_CO_DEAD;
#ifdef BUILD_PLATFORM_WIN
	HANDLE m_hFiber = nullptr;
#else
	void* m_pvStack = nullptr;
	ucontext_t m_kContext;
#endif
	VeCoroutineEnvPtr m_spEnv;
	VeCoroutineBasePtr m_spPrevious;
	
};

class VENUS_API VeCoroutineEnv : public VeRefObject
{
	VeNoCopy(VeCoroutineEnv);
	VeNoMove(VeCoroutineEnv);
public:
	void Suspend() noexcept;

	void Close() noexcept;

	static const VeCoroutineEnvPtr& GetCurrent() noexcept;

private:
	friend class VeCoroutineBase;
	VeCoroutineEnv() noexcept;

	VeCoroutineBasePtr m_spMain;
	VeCoroutineBasePtr m_spRunning;

};

template <class _Param = void, class _Ret = void>
class VeCoroutine : public VeCoroutineBase
{
#ifdef BUILD_PLATFORM_WIN
	static void WINAPI CoreEntry(void* pvCoroutine) noexcept
    {
#else
    static void CoreEntry(uint32_t low32, uint32_t hi32) noexcept
    {
        void* pvCoroutine = (void*)((uintptr_t)low32 | ((uintptr_t)hi32 << 32));
#endif
		((VeCoroutine*)pvCoroutine)->RunEntry();
		VeCoroutineEnv::GetCurrent()->Close();
	}
public:
	VeCoroutine(std::function<_Ret(VeCoroutine&, _Param)> kEntry,
		uint32_t u32Stack = VE_CO_DEFAULT_STACK) noexcept
		: VeCoroutineBase(&CoreEntry, u32Stack), m_kUserEntry(kEntry)
	{

	}

	_Ret resume(_Param pin) noexcept
	{
		m_ParamSpace = pin;
		if (m_eStatus == VE_CO_DEAD)
		{
			VeCoroutineBase::Restart(&CoreEntry);
		}
		VeCoroutineBase::Resume();
		return m_ReturnSpace;
	}

	_Param yield(_Ret rin) noexcept
	{
		m_ReturnSpace = rin;
		VeCoroutineEnv::GetCurrent()->Suspend();
		return m_ParamSpace;
	}

private:
	void RunEntry() noexcept
	{
		m_ReturnSpace = m_kUserEntry(*this, m_ParamSpace);
	}

	std::function<_Ret(VeCoroutine&,_Param)> m_kUserEntry;
	_Param m_ParamSpace;
	_Ret m_ReturnSpace;

};

template <>
class VeCoroutine<void, void> : public VeCoroutineBase
{
#ifdef BUILD_PLATFORM_WIN
    static void WINAPI CoreEntry(void* pvCoroutine) noexcept
    {
#else
    static void CoreEntry(uint32_t low32, uint32_t hi32) noexcept
    {
        void* pvCoroutine = (void*)((uintptr_t)low32 | ((uintptr_t)hi32 << 32));
#endif
		((VeCoroutine*)pvCoroutine)->RunEntry();
		VeCoroutineEnv::GetCurrent()->Close();
	}
public:
	VeCoroutine(std::function<void(VeCoroutine&)> kEntry,
		uint32_t u32Stack = VE_CO_DEFAULT_STACK) noexcept
		: VeCoroutineBase(&CoreEntry, u32Stack), m_kUserEntry(kEntry)
	{

	}

	virtual ~VeCoroutine() noexcept
	{

	}

	void resume() noexcept
	{
		if (m_eStatus == VE_CO_DEAD)
		{
			VeCoroutineBase::Restart(&CoreEntry);
		}
		VeCoroutineBase::Resume();
	}

	void yield() noexcept
	{
		VeCoroutineEnv::GetCurrent()->Suspend();
	}

private:
	void RunEntry() noexcept
	{
		m_kUserEntry(*this);
	}

	std::function<void(VeCoroutine&)> m_kUserEntry;

};

template <class _Param>
class VeCoroutine<_Param, void> : public VeCoroutineBase
{
#ifdef BUILD_PLATFORM_WIN
    static void WINAPI CoreEntry(void* pvCoroutine) noexcept
    {
#else
    static void CoreEntry(uint32_t low32, uint32_t hi32) noexcept
    {
        void* pvCoroutine = (void*)((uintptr_t)low32 | ((uintptr_t)hi32 << 32));
#endif
		((VeCoroutine*)pvCoroutine)->RunEntry();
		VeCoroutineEnv::GetCurrent()->Close();
	}
public:
	VeCoroutine(std::function<void(VeCoroutine&, _Param)> kEntry,
		uint32_t u32Stack = VE_CO_DEFAULT_STACK) noexcept
		: VeCoroutineBase(&CoreEntry, u32Stack), m_kUserEntry(kEntry)
	{

	}

	void resume(_Param pin) noexcept
	{
		m_ParamSpace = pin;
		if (m_eStatus == VE_CO_DEAD)
		{
			VeCoroutineBase::Restart(&CoreEntry);
		}
		VeCoroutineBase::Resume();
	}

	_Param yield() noexcept
	{
		VeCoroutineEnv::GetCurrent()->Suspend();
		return m_ParamSpace;
	}

private:
	void RunEntry() noexcept
	{
		m_kUserEntry(*this, m_ParamSpace);
	}

	std::function<void(VeCoroutine&, _Param)> m_kUserEntry;
	_Param m_ParamSpace;

};

template <class _Ret>
class VeCoroutine<void, _Ret> : public VeCoroutineBase
{
#ifdef BUILD_PLATFORM_WIN
    static void WINAPI CoreEntry(void* pvCoroutine) noexcept
    {
#else
    static void CoreEntry(uint32_t low32, uint32_t hi32) noexcept
    {
        void* pvCoroutine = (void*)((uintptr_t)low32 | ((uintptr_t)hi32 << 32));
#endif
		((VeCoroutine*)pvCoroutine)->RunEntry();
		VeCoroutineEnv::GetCurrent()->Close();
	}
public:
	VeCoroutine(std::function<_Ret(VeCoroutine&)> kEntry,
		uint32_t u32Stack = VE_CO_DEFAULT_STACK) noexcept
		: VeCoroutineBase(&CoreEntry, u32Stack), m_kUserEntry(kEntry)
	{

	}

	_Ret resume() noexcept
	{
		if (m_eStatus == VE_CO_DEAD)
		{
			VeCoroutineBase::Restart(&CoreEntry);
		}
		VeCoroutineBase::Resume();
		return m_ReturnSpace;
	}

	void yield(_Ret rin) noexcept
	{
		m_ReturnSpace = rin;
		VeCoroutineEnv::GetCurrent()->Suspend();
	}

private:
	void RunEntry() noexcept
	{
		m_ReturnSpace = m_kUserEntry(*this);
	}

	std::function<_Ret(VeCoroutine&)> m_kUserEntry;
	_Ret m_ReturnSpace;

};

template <class _Param, class _Ret>
vtd::intrusive_ptr<VeCoroutine<_Param, _Ret>> VeCreateCoroutine(std::function<_Ret(VeCoroutine<_Param, _Ret>&, _Param)> funcEntry) noexcept
{
	return VE_NEW VeCoroutine<_Param, _Ret>(funcEntry);
}


inline vtd::intrusive_ptr<VeCoroutine<>> VeCreateCoroutine(std::function<void(VeCoroutine<>&)> funcEntry) noexcept
{
	return VE_NEW VeCoroutine<>(funcEntry);
}

#include "VeCoroutine.inl"
