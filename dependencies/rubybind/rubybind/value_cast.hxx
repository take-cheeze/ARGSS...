// Copyright (c)  2008 Borislav Stanimirov

// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <boost/function.hpp>

#include "ruby_include.hxx"

namespace rubybind
{
	inline VALUE value_cast(int n)
	{
		return rb_int2inum(n);
	}

	inline VALUE value_cast(double f)
	{
		return rb_float_new(f);
	}

	inline VALUE value_cast(float f)
	{
		return value_cast(double(f));
	}

	template <typename T>
	inline VALUE invoke_value_cast(boost::function1<void, T> func, const T& t1)
	{
		func(t1);
		return Qnil;
	}

	template <typename ReturnType, typename T>
	inline VALUE invoke_value_cast(boost::function1<ReturnType, T> func, const T& t1)
	{
		return value_cast(func(t1));
	}

	template <typename T1, typename T2>
	inline VALUE invoke_value_cast(boost::function2<void, T1, T2> func, const T1& t1, const T2& t2)
	{
		func(t1, t2);
		return Qnil;
	}

	template <typename ReturnType, typename T1, typename T2>
	inline VALUE invoke_value_cast(boost::function2<ReturnType, T1, T2> func, const T1& t1, const T2& t2)
	{
		return value_cast(func(t1, t2));
	}

	template <typename T>
	T from_value(VALUE v);

	template<>
	inline int from_value<int>(VALUE v)
	{
		return rb_num2long(v);
	}

	template<>
	inline double from_value<double>(VALUE v)
	{
		return RFLOAT_VALUE(v); // RFLOAT(v)->value;
	}

	template<>
	inline float from_value<float>(VALUE v)
	{
		return float(from_value<double>(v));
	}
}
