////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      ASync
//  File name:   VeJobSystem.h
//  Created:     2016/07/15 by Albert
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

#define VE_JOB_FG_PRIORITY VE_THREAD_PRIORITY_HIGHEST
#define VE_JOB_BG_PRIORITY VE_THREAD_PRIORITY_BELOW_NORMAL

class VeJob : public VeRefObject
{
public:
	enum Type
	{
		TYPE_FOREGROUND,
		TYPE_FOREGROUND_YIELDABLE,
		TYPE_BACKGROUND,
		TYPE_BACKGROUND_YIELDABLE,
		TYPE_PARALLEL_COMPUTE,
		TYPE_MAX
	};

	VeJob(Type eType) noexcept : m_eType(eType) {}

	Type GetType() noexcept
	{
		return m_eType;
	}

	virtual void Work() noexcept = 0;

private:
	const Type m_eType;

};

VeSmartPointer(VeJob);

class VeJobFunc : public VeJob
{
public:
	VeJobFunc(Type eType, const std::function<void()>& _Fx) noexcept
		: VeJob(eType), m_kWork(_Fx) {}

	virtual void Work() noexcept
	{
		m_kWork();
	}

private:
	std::function<void()> m_kWork;

};

class VENUS_API VeJobSystem : public VeMemObject
{
public:
	VeJobSystem() noexcept = default;

	~VeJobSystem() noexcept;

	void Init(size_t stFGNum, size_t stBGNum) noexcept;

	void Term() noexcept;

	void ParallelCompute(const VeJobPtr& spJob) noexcept;

	void ParallelCompute(const std::function<void()>& _Fx) noexcept
	{
		VeJobPtr spJob = VE_NEW VeJobFunc(VeJob::TYPE_PARALLEL_COMPUTE, _Fx);
		ParallelCompute(spJob);
	}

protected:
	static void ParallelCompute(void* pvJob) noexcept;

	size_t m_stNumFGThreads = 0;
	VeThread* m_pkFGThreads = nullptr;
	size_t m_stNumBGThreads = 0;
	VeThread* m_pkBGThreads = nullptr;

};

#include "VeJobSystem.inl"
