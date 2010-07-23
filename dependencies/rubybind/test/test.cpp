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

#include <iostream>

#include <rubybind/class.h>
#include <rubybind/module.h>

using namespace std;

class Class
{
public:
	Class() : pfield(1), m_field(2), m_ffield(3.14f)
	{
		cout << "constructing class" << endl;
	}

	Class(int pf) : pfield(pf), m_field(pf), m_ffield(2.15f)
	{
		cout << "constructing with param" << endl;
	}

	~Class()
	{
		cout << "destructing class" << endl;
	}

	void DoPrinting() const
	{
		cout << "in c++: " << endl;
		cout << pfield << ' ' << m_field << ' ' << m_ffield << endl;
	}

	void Increment(int withWhat)
	{
		m_field += withWhat;
	}

	int Add() const
	{
		return m_field + pfield;
	}

	void SetF(float f)
	{
		m_ffield = f;
	}

	float GetF() const
	{
		return m_ffield;
	}

	int pfield;

private:

	int m_field;
	float m_ffield;
};

int main()
{
	VALUE* val = &rb_cObject;

	ruby_init();
	ruby_init_loadpath();	
	ruby_script("embedded");	

	rubybind::module("CPlus")
		.mclass_<Class>("CClass")
			.ctor0()
			.ctor1<int>()
			.def("do_printing", &Class::DoPrinting)
			.def("add", &Class::Add)
			.def("inc", &Class::Increment)
			.def("f", &Class::GetF)
			.def("f=", &Class::SetF);

	rb_load_file("test.rb");
	int status = ruby_exec();

	ruby_cleanup(status);
	return 0;
}