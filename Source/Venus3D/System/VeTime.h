////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      System
//  File name:   VeTime.h
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

class VENUS_API VeTime : public VeMemObject
{
	VeNoCopy(VeTime);
public:
	VeTime() noexcept;

	virtual ~VeTime() noexcept;

	inline float GetDeltaTime() noexcept;

	inline double GetDeltaTime64() noexcept;

	inline double GetTime() noexcept;

	inline uint32_t GetTimeUInt() noexcept;

	inline uint64_t GetCurrentCount() noexcept;

	inline void Pause() noexcept;

	inline void Resume() noexcept;

	inline bool GetPaused() noexcept;

	inline void SetInvert(bool bInv = false) noexcept;

	inline bool GetInvert() noexcept;

	void Reset() noexcept;

	void Update() noexcept;

protected:
	uint64_t m_u64Frequency;
	double m_f64InvFrequency;
	uint64_t m_u64StartRecordCount;
	uint64_t m_u64LastRecordCount;
	uint64_t m_u64CurrentCount;

	double m_f64Time;
	double m_f64DeltaTime;
	bool m_bPaused;
	bool m_bInvert;

};

#include "VeTime.inl"
