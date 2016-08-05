////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Stream
//  File name:   VeStream.h
//  Created:     2016/08/05 by Albert
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

template <class _Reader>
class VeIStream;

template <class _Writer>
class VeOStream;

namespace venus
{
	template <class _Ty>
	struct input_simple
	{
		template <class _Reader>
		static void read(_Ty& val, VeIStream<_Reader>& s) noexcept
		{
			VE_ASSERT_EQ(s.read(&val, sizeof(_Ty)), sizeof(_Ty));
		}
	};

	template <class _Ty>
	struct input_rich;

	template <class _Ty>
	struct input : std::conditional <std::is_trivial<_Ty>::value,
		input_simple<_Ty>, input_rich<_Ty>>::type
	{
	
	};

	template <class _Ty>
	struct output_simple
	{
		template <class _Writer>
		static void write(_Ty&& val, VeOStream<_Writer>& s) noexcept
		{
			VE_ASSERT_EQ(s.write(&val, sizeof(_Ty)), sizeof(_Ty));
		}
	};

	template <class _Ty>
	struct output_rich;

	template <class _Ty>
	struct output : std::conditional <std::is_trivial<_Ty>::value,
		output_simple<_Ty>, output_rich<_Ty>>::type
	{

	};
	
}

template <class _Reader>
class VeIStream : public _Reader
{
public:
	template<class... _Types>
	VeIStream(_Types&&... _Args) noexcept
		: _Reader(std::forward<_Types>(_Args)...) {}

	~VeIStream() noexcept = default;

	template <class _Ty>
	VeIStream& operator >> (_Ty& val) noexcept
	{
		venus::input<_Ty>::read(val, *this);
		return *this;
	}

};

typedef VeIStream<VeMemReader> VeMemIStream;
typedef VeIStream<VeBlobReader> VeBlobIStream;

template <class _Writer>
class VeOStream : public _Writer
{
public:
	template<class... _Types>
	VeOStream(_Types&&... _Args) noexcept
		: _Writer(std::forward<_Types>(_Args)...) {}

	~VeOStream() noexcept = default;

	template <class _Ty>
	VeOStream<_Writer>& operator << (_Ty val) noexcept
	{
		venus::output<_Ty>::write(std::move(val), *this);
		return *this;
	}

};

typedef VeOStream<VeMemWriter> VeMemOStream;
typedef VeOStream<VeBlobWriter> VeBlobOStream;
