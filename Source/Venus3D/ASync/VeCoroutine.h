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

#ifdef VE_ENABLE_COROUTINE

#define VE_CO_DEFAULT_STACK (640 * 1024)

class VENUS_API VeCoroutine : public VeMemObject
{
	VeNoCopy(VeCoroutine);
public:
	enum State
	{
		STATE_READY,
		STATE_SUSPENDED,
		STATE_RUNNING,
		STATE_DEAD
	};

	typedef void* (*Entry)(void*);

	VeCoroutine(size_t stStackSize = VE_CO_DEFAULT_STACK) noexcept;

	~VeCoroutine() noexcept;

	inline State GetState() noexcept;

	void prepare() noexcept;

	void* start(Entry pfuncEntry, void* pvUserData = nullptr) noexcept;

	void* resume(void* pvUserData = nullptr) noexcept;

protected:
	void* _resume(void* pvUserData) noexcept;

	friend class VeCoenvironment;
	fcontext_stack_t  m_kStack;
	fcontext_t m_hContext;
	VeCoroutine* m_pkPrevious = nullptr;
	State m_eState;

};

class VENUS_API VeCoenvironment : public VeMemObject
{
	VeNoCopy(VeCoenvironment);
public:
	~VeCoenvironment() noexcept;
	
	inline static VeCoroutine* GetMain() noexcept;

	inline static VeCoroutine* GetRunning() noexcept;

	inline static bool IsMainRunning() noexcept;

	static void* yield(void* pvUserData) noexcept;

private:
	static void Entry(fcontext_transfer_t trans) noexcept;

	static VeCoenvironment* GetCurrent() noexcept;

	friend class VeCoroutine;
	friend class Venus3D;
	VeCoenvironment() noexcept;

	VeCoroutine m_kMain;
	VeCoroutine* m_pkRunning;
	VeCoroutine::Entry m_pfuncUserEntry = nullptr;

};

#endif

#include "VeCoroutine.inl"
